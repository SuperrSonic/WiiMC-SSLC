/*
 * Opus decoder using libopus
 * Copyright (c) 2012 Nicolas George
 *
 * This file is part of Libav.
 *
 * Libav is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Libav is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Libav; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "include/opus.h"
#include "include/opus_multistream.h"

#include "libavutil/common.h"
#include "libavutil/avassert.h"
#include "libavutil/intreadwrite.h"
#include "avcodec.h"
#include "vorbis.h"
#include "mathops.h"

struct libopus_context {
    OpusMSDecoder *dec;
    AVFrame frame;
};

static int opus_error_to_averror(int err)
{
    switch (err) {
    case OPUS_BAD_ARG:
        return AVERROR(EINVAL);
    case OPUS_BUFFER_TOO_SMALL:
        return AVERROR_UNKNOWN;
    case OPUS_INTERNAL_ERROR:
        return AVERROR(EFAULT);
    case OPUS_INVALID_PACKET:
        return AVERROR_INVALIDDATA;
    case OPUS_UNIMPLEMENTED:
        return AVERROR(ENOSYS);
    case OPUS_INVALID_STATE:
        return AVERROR_UNKNOWN;
    case OPUS_ALLOC_FAIL:
        return AVERROR(ENOMEM);
    default:
        return AVERROR(EINVAL);
    }
}

static inline void reorder(uint8_t *data, unsigned channels, unsigned bps,
                           unsigned samples, const uint8_t *map)
{
    uint8_t tmp[8 * 4];
    unsigned i;

    av_assert1(channels * bps <= sizeof(tmp));
    for (; samples > 0; samples--) {
        for (i = 0; i < channels; i++)
            memcpy(tmp + bps * i, data + bps * map[i], bps);
        memcpy(data, tmp, bps * channels);
        data += bps * channels;
    }
}

#define OPUS_HEAD_SIZE 19

static av_cold int libopus_decode_init(AVCodecContext *avc)
{
    struct libopus_context *opus = avc->priv_data;
    int ret, channel_map = 0, gain_db = 0, nb_streams, nb_coupled;
    uint8_t mapping_stereo[] = { 0, 1 }, *mapping;

    avc->sample_rate    = 48000;
    avc->sample_fmt     = avc->request_sample_fmt == AV_SAMPLE_FMT_FLT ?
                          AV_SAMPLE_FMT_FLT : AV_SAMPLE_FMT_S16;
    avc->channel_layout = avc->channels > 8 ? 0 :
                          ff_vorbis_channel_layouts[avc->channels - 1];

    if (avc->extradata_size >= OPUS_HEAD_SIZE) {
        gain_db     = sign_extend(AV_RL16(avc->extradata + 16), 16);
        channel_map = AV_RL8 (avc->extradata + 18);
    }
    if (avc->extradata_size >= OPUS_HEAD_SIZE + 2 + avc->channels) {
        nb_streams = avc->extradata[OPUS_HEAD_SIZE + 0];
        nb_coupled = avc->extradata[OPUS_HEAD_SIZE + 1];
        if (nb_streams + nb_coupled != avc->channels)
            av_log(avc, AV_LOG_WARNING, "Inconsistent channel mapping.\n");
        mapping = avc->extradata + OPUS_HEAD_SIZE + 2;
    } else {
        if (avc->channels > 2 || channel_map) {
            av_log(avc, AV_LOG_ERROR,
                   "No channel mapping for %d channels.\n", avc->channels);
            return AVERROR(EINVAL);
        }
        nb_streams = 1;
        nb_coupled = avc->channels > 1;
        mapping    = mapping_stereo;
    }

    opus->dec = opus_multistream_decoder_create(avc->sample_rate, avc->channels,
                                                nb_streams, nb_coupled,
                                                mapping, &ret);
    if (!opus->dec) {
        av_log(avc, AV_LOG_ERROR, "Unable to create decoder: %s\n",
               opus_strerror(ret));
        return opus_error_to_averror(ret);
    }

    ret = opus_multistream_decoder_ctl(opus->dec, OPUS_SET_GAIN(gain_db));
    if (ret != OPUS_OK)
        av_log(avc, AV_LOG_WARNING, "Failed to set gain: %s\n",
               opus_strerror(ret));

    avc->delay = 3840;  /* Decoder delay (in samples) at 48kHz */
    avcodec_get_frame_defaults(&opus->frame);
    avc->coded_frame = &opus->frame;
    return 0;
}

static av_cold int libopus_decode_close(AVCodecContext *avc)
{
    struct libopus_context *opus = avc->priv_data;

    opus_multistream_decoder_destroy(opus->dec);
    return 0;
}

#define MAX_FRAME_SIZE (960 * 6)

static int libopus_decode(AVCodecContext *avc, void *frame,
                          int *got_frame_ptr, AVPacket *pkt)
{
    struct libopus_context *opus = avc->priv_data;
    int ret, nb_samples;

    opus->frame.nb_samples = MAX_FRAME_SIZE;
    ret = avc->get_buffer(avc, &opus->frame);
    if (ret < 0) {
        av_log(avc, AV_LOG_ERROR, "get_buffer() failed\n");
        return ret;
    }

    if (avc->sample_fmt == AV_SAMPLE_FMT_S16)
        nb_samples = opus_multistream_decode(opus->dec, pkt->data, pkt->size,
                                             (opus_int16 *)opus->frame.data[0],
                                             opus->frame.nb_samples, 0);
    else
        nb_samples = opus_multistream_decode_float(opus->dec, pkt->data, pkt->size,
                                                   (float *)opus->frame.data[0],
                                                   opus->frame.nb_samples, 0);

    if (nb_samples < 0) {
        av_log(avc, AV_LOG_ERROR, "Decoding error: %s\n",
               opus_strerror(nb_samples));
        return opus_error_to_averror(nb_samples);
    }

    if (avc->channels > 3 && avc->channels <= 8) {
        const uint8_t *m = ff_vorbis_channel_layout_offsets[avc->channels - 1];
        if (avc->sample_fmt == AV_SAMPLE_FMT_S16)
            reorder(opus->frame.data[0], avc->channels, 2, nb_samples, m);
        else
            reorder(opus->frame.data[0], avc->channels, 4, nb_samples, m);
    }

    opus->frame.nb_samples = nb_samples;
    *(AVFrame *)frame = opus->frame;
    *got_frame_ptr = 1;
    return pkt->size;
}

static void libopus_flush(AVCodecContext *avc)
{
    struct libopus_context *opus = avc->priv_data;

    opus_multistream_decoder_ctl(opus->dec, OPUS_RESET_STATE);
}

AVCodec ff_libopus_decoder = {
    .name           = "libopus",
    .type           = AVMEDIA_TYPE_AUDIO,
    .id             = CODEC_ID_OPUS,
    .priv_data_size = sizeof(struct libopus_context),
    .init           = libopus_decode_init,
    .close          = libopus_decode_close,
    .decode         = libopus_decode,
    .flush          = libopus_flush,
    .capabilities   = CODEC_CAP_DR1,
    .long_name      = NULL_IF_CONFIG_SMALL("libopus Opus"),
    .sample_fmts    = (const enum AVSampleFormat[]){ AV_SAMPLE_FMT_FLT,
                                                     AV_SAMPLE_FMT_S16,
                                                     AV_SAMPLE_FMT_NONE },
};
