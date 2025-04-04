/*
 * ASF compatible demuxer
 * Copyright (c) 2000, 2001 Fabrice Bellard
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

//#define DEBUG

#include "libavutil/bswap.h"
#include "libavutil/common.h"
#include "libavutil/avstring.h"
#include "libavutil/dict.h"
#include "libavutil/mathematics.h"
#include "libavutil/opt.h"
#include "avformat.h"
#include "internal.h"
#include "avio_internal.h"
#include "id3v2.h"
#include "riff.h"
#include "asf.h"
#include "asfcrypt.h"
#include "avlanguage.h"

typedef struct {
    const AVClass *class;
    int asfid2avid[128];                 ///< conversion table from asf ID 2 AVStream ID
    ASFStream streams[128];              ///< it's max number and it's not that big
    uint32_t stream_bitrates[128];       ///< max number of streams, bitrate for each (for streaming)
    AVRational dar[128];
    char stream_languages[128][6];       ///< max number of streams, language for each (RFC1766, e.g. en-US)
    /* non streamed additonnal info */
    /* packet filling */
    int packet_size_left;
    /* only for reading */
    uint64_t data_offset;                ///< beginning of the first data packet
    uint64_t data_object_offset;         ///< data object offset (excl. GUID & size)
    uint64_t data_object_size;           ///< size of the data object
    int index_read;

    ASFMainHeader hdr;

    int packet_flags;
    int packet_property;
    int packet_timestamp;
    int packet_segsizetype;
    int packet_segments;
    int packet_seq;
    int packet_replic_size;
    int packet_key_frame;
    int packet_padsize;
    unsigned int packet_frag_offset;
    unsigned int packet_frag_size;
    int64_t packet_frag_timestamp;
    int packet_multi_size;
    int packet_obj_size;
    int packet_time_delta;
    int packet_time_start;
    int64_t packet_pos;

    int stream_index;

    ASFStream* asf_st;                   ///< currently decoded stream

    int no_resync_search;
} ASFContext;

static const AVOption options[] = {
    {"no_resync_search", "Don't try to resynchronize by looking for a certain optional start code", offsetof(ASFContext, no_resync_search), AV_OPT_TYPE_INT, {.dbl = 0}, 0, 1, AV_OPT_FLAG_DECODING_PARAM },
    { NULL },
};

static const AVClass asf_class = {
    .class_name = "asf demuxer",
    .item_name  = av_default_item_name,
    .option     = options,
    .version    = LIBAVUTIL_VERSION_INT,
};

#undef NDEBUG
#include <assert.h>

#define ASF_MAX_STREAMS 127
#define FRAME_HEADER_SIZE 17
// Fix Me! FRAME_HEADER_SIZE may be different.

#ifdef DEBUG
static const ff_asf_guid stream_bitrate_guid = { /* (http://get.to/sdp) */
    0xce, 0x75, 0xf8, 0x7b, 0x8d, 0x46, 0xd1, 0x11, 0x8d, 0x82, 0x00, 0x60, 0x97, 0xc9, 0xa2, 0xb2
};

#define PRINT_IF_GUID(g,cmp) \
if (!ff_guidcmp(g, &cmp)) \
    av_dlog(NULL, "(GUID: %s) ", #cmp)

static void print_guid(const ff_asf_guid *g)
{
    int i;
    PRINT_IF_GUID(g, ff_asf_header);
    else PRINT_IF_GUID(g, ff_asf_file_header);
    else PRINT_IF_GUID(g, ff_asf_stream_header);
    else PRINT_IF_GUID(g, ff_asf_audio_stream);
    else PRINT_IF_GUID(g, ff_asf_audio_conceal_none);
    else PRINT_IF_GUID(g, ff_asf_video_stream);
    else PRINT_IF_GUID(g, ff_asf_video_conceal_none);
    else PRINT_IF_GUID(g, ff_asf_command_stream);
    else PRINT_IF_GUID(g, ff_asf_comment_header);
    else PRINT_IF_GUID(g, ff_asf_codec_comment_header);
    else PRINT_IF_GUID(g, ff_asf_codec_comment1_header);
    else PRINT_IF_GUID(g, ff_asf_data_header);
    else PRINT_IF_GUID(g, ff_asf_simple_index_header);
    else PRINT_IF_GUID(g, ff_asf_head1_guid);
    else PRINT_IF_GUID(g, ff_asf_head2_guid);
    else PRINT_IF_GUID(g, ff_asf_my_guid);
    else PRINT_IF_GUID(g, ff_asf_ext_stream_header);
    else PRINT_IF_GUID(g, ff_asf_extended_content_header);
    else PRINT_IF_GUID(g, ff_asf_ext_stream_embed_stream_header);
    else PRINT_IF_GUID(g, ff_asf_ext_stream_audio_stream);
    else PRINT_IF_GUID(g, ff_asf_metadata_header);
	else PRINT_IF_GUID(g, ff_asf_metadata_library_header);
    else PRINT_IF_GUID(g, ff_asf_marker_header);
    else PRINT_IF_GUID(g, stream_bitrate_guid);
    else PRINT_IF_GUID(g, ff_asf_language_guid);
    else
        av_dlog(NULL, "(GUID: unknown) ");
    for(i=0;i<16;i++)
        av_dlog(NULL, " 0x%02x,", (*g)[i]);
    av_dlog(NULL, "}\n");
}
#undef PRINT_IF_GUID
#else
#define print_guid(g)
#endif

static int asf_probe(AVProbeData *pd)
{
    /* check file header */
    if (!ff_guidcmp(pd->buf, &ff_asf_header))
        return AVPROBE_SCORE_MAX;
    else
        return 0;
}

static int get_value(AVIOContext *pb, int type){
    switch(type){
        case 2: return avio_rl32(pb);
        case 3: return avio_rl32(pb);
        case 4: return avio_rl64(pb);
        case 5: return avio_rl16(pb);
        default:return INT_MIN;
    }
}

#include "../../utils/mem2_manager.h"
extern u8 *pos_pic;
extern int embedded_pic;
//extern int wiim_inf;
int wm_picture_type2 = 0;

/* MSDN claims that this should be "compatible with the ID3 frame, APIC",
 * but in reality this is only loosely similar */
static int asf_read_picture(AVFormatContext *s, int len)
{
    //AVPacket pkt = { 0 };
    const CodecMime *mime = ff_id3v2_mime_tags;
    enum  CodecID      id = CODEC_ID_NONE;
    char mimetype[64];
    uint8_t  *desc = NULL;
    AVStream   *st = NULL;
    int ret, type, picsize, desc_len;

    /* type + picsize + mime + desc */
    if (len < 1 + 4 + 2 + 2) {
        av_log(s, AV_LOG_ERROR, "Invalid attached picture size: %d.\n", len);
        return AVERROR_INVALIDDATA;
    }

	/* if the file size is big the position of wd/picture different. */
	if(wm_picture_type2) {
		avio_seek(s->pb, -len, SEEK_CUR);
		wm_picture_type2 = 0;
	}

    /* picture type */
    type = avio_r8(s->pb);
    len--;
    if (type >= FF_ARRAY_ELEMS(ff_id3v2_picture_types) || type < 0) {
        av_log(s, AV_LOG_WARNING, "Unknown attached picture type: %d.\n", type);
        type = 0;
    }

    /* picture data size */
    picsize = avio_rl32(s->pb);
    len -= 4;

    /* picture MIME type */
    len -= avio_get_str16le(s->pb, len, mimetype, sizeof(mimetype));
    while (mime->id != CODEC_ID_NONE) {
        if (!strncmp(mime->str, mimetype, sizeof(mimetype))) {
            id = mime->id;
            break;
        }
        mime++;
    }

//wiim_inf = type;
    if (id == CODEC_ID_NONE) {
        av_log(s, AV_LOG_ERROR, "Unknown attached picture mimetype: %s.\n",
               mimetype);
        return 0;
    }

    if (picsize >= len) {
        av_log(s, AV_LOG_ERROR, "Invalid attached picture data size: %d >= %d.\n",
               picsize, len);
        return AVERROR_INVALIDDATA;
    }

    /* picture description */
    desc_len = (len - picsize) * 2 + 1;
    desc     = av_malloc(desc_len);
    if (!desc)
        return AVERROR(ENOMEM);
    len -= avio_get_str16le(s->pb, len - picsize, desc, desc_len);

    //aqui 'ta el memleak
    /*ret = av_get_packet(s->pb, &pkt, picsize);
    if (ret < 0)
        goto fail;*/

    st = avformat_new_stream(s, NULL);
    if (!st) {
        ret = AVERROR(ENOMEM);
        goto fail;
    }

	pos_pic = (u8 *)mem2_memalign(32, 1.5*1024*1024, MEM2_OTHER);
	avio_seek(s->pb, -picsize, SEEK_CUR);
	
	//limit length to avoid crash
	if(picsize > 1.5*1024*1024)
		picsize = 1.5*1024*1024;
	
	avio_read(s->pb, pos_pic, picsize);
	embedded_pic = 1;
//	wiim_inf = 23;

    if (*desc)
        av_dict_set(&st->metadata, "title", desc, AV_DICT_DONT_STRDUP_VAL);
    else
        av_freep(&desc);

    av_dict_set(&st->metadata, "comment", ff_id3v2_picture_types[type], 0);

    return 0;

fail:
    av_freep(&desc);
    //av_free_packet(&pkt);
    return ret;
}

static void get_tag(AVFormatContext *s, const char *key, int type, int len)
{
    char *value;
    int64_t off = avio_tell(s->pb);

    if ((unsigned)len >= (UINT_MAX - 1)/2)
        return;

    value = av_malloc(2*len+1);
    if (!value)
        goto finish;

    if (type == 0) {         // UTF16-LE
        avio_get_str16le(s->pb, len, value, 2*len + 1);
    } else if (type == -1) { // ASCII
        avio_read(s->pb, value, len);
        value[len]=0;
    } else if (type > 1 && type <= 5) {  // boolean or DWORD or QWORD or WORD
        uint64_t num = get_value(s->pb, type);
        snprintf(value, len, "%"PRIu64, num);
	} else if (type == 1 && !strcmp(key, "WM/Picture")) { // handle cover art
        asf_read_picture(s, len);
        goto finish;
	} else if (type == 6) { // (don't) handle GUID
        av_log(s, AV_LOG_DEBUG, "Unsupported GUID value in tag %s.\n", key);
        goto finish;
    } else {
        av_log(s, AV_LOG_DEBUG, "Unsupported value type %d in tag %s.\n", type, key);
        goto finish;
    }
    if (*value)
        av_dict_set(&s->metadata, key, value, 0);
finish:
    av_freep(&value);
    avio_seek(s->pb, off + len, SEEK_SET);
}

static int asf_read_file_properties(AVFormatContext *s, int64_t size)
{
    ASFContext *asf = s->priv_data;
    AVIOContext *pb = s->pb;

    ff_get_guid(pb, &asf->hdr.guid);
    asf->hdr.file_size          = avio_rl64(pb);
    asf->hdr.create_time        = avio_rl64(pb);
    avio_rl64(pb);                               /* number of packets */
    asf->hdr.play_time          = avio_rl64(pb);
    asf->hdr.send_time          = avio_rl64(pb);
    asf->hdr.preroll            = avio_rl32(pb);
    asf->hdr.ignore             = avio_rl32(pb);
    asf->hdr.flags              = avio_rl32(pb);
    asf->hdr.min_pktsize        = avio_rl32(pb);
    asf->hdr.max_pktsize        = avio_rl32(pb);
    if (asf->hdr.min_pktsize >= (1U<<29))
        return AVERROR_INVALIDDATA;
    asf->hdr.max_bitrate        = avio_rl32(pb);
    s->packet_size = asf->hdr.max_pktsize;

    return 0;
}

static int asf_read_stream_properties(AVFormatContext *s, int64_t size)
{
    ASFContext *asf = s->priv_data;
    AVIOContext *pb = s->pb;
    AVStream *st;
    ASFStream *asf_st;
    ff_asf_guid g;
    enum AVMediaType type;
    int type_specific_size, sizeX;
    unsigned int tag1;
    int64_t pos1, pos2, start_time;
    int test_for_ext_stream_audio, is_dvr_ms_audio=0;

    if (s->nb_streams == ASF_MAX_STREAMS) {
        av_log(s, AV_LOG_ERROR, "too many streams\n");
        return AVERROR(EINVAL);
    }

    pos1 = avio_tell(pb);

    st = avformat_new_stream(s, NULL);
    if (!st)
        return AVERROR(ENOMEM);
    avpriv_set_pts_info(st, 32, 1, 1000); /* 32 bit pts in ms */
    asf_st = av_mallocz(sizeof(ASFStream));
    if (!asf_st)
        return AVERROR(ENOMEM);
    st->priv_data = asf_st;
    start_time = asf->hdr.preroll;

    asf_st->stream_language_index = 128; // invalid stream index means no language info

    if(!(asf->hdr.flags & 0x01)) { // if we aren't streaming...
        st->duration = asf->hdr.play_time /
            (10000000 / 1000) - start_time;
    }
    ff_get_guid(pb, &g);

    test_for_ext_stream_audio = 0;
    if (!ff_guidcmp(&g, &ff_asf_audio_stream)) {
        type = AVMEDIA_TYPE_AUDIO;
    } else if (!ff_guidcmp(&g, &ff_asf_video_stream)) {
        type = AVMEDIA_TYPE_VIDEO;
    } else if (!ff_guidcmp(&g, &ff_asf_jfif_media)) {
        type = AVMEDIA_TYPE_VIDEO;
        st->codec->codec_id = CODEC_ID_MJPEG;
    } else if (!ff_guidcmp(&g, &ff_asf_command_stream)) {
        type = AVMEDIA_TYPE_DATA;
    } else if (!ff_guidcmp(&g, &ff_asf_ext_stream_embed_stream_header)) {
        test_for_ext_stream_audio = 1;
        type = AVMEDIA_TYPE_UNKNOWN;
    } else {
        return -1;
    }
    ff_get_guid(pb, &g);
    avio_skip(pb, 8); /* total_size */
    type_specific_size = avio_rl32(pb);
    avio_rl32(pb);
    st->id = avio_rl16(pb) & 0x7f; /* stream id */
    // mapping of asf ID to AV stream ID;
    asf->asfid2avid[st->id] = s->nb_streams - 1;

    avio_rl32(pb);

    if (test_for_ext_stream_audio) {
        ff_get_guid(pb, &g);
        if (!ff_guidcmp(&g, &ff_asf_ext_stream_audio_stream)) {
            type = AVMEDIA_TYPE_AUDIO;
            is_dvr_ms_audio=1;
            ff_get_guid(pb, &g);
            avio_rl32(pb);
            avio_rl32(pb);
            avio_rl32(pb);
            ff_get_guid(pb, &g);
            avio_rl32(pb);
        }
    }

    st->codec->codec_type = type;
    if (type == AVMEDIA_TYPE_AUDIO) {
        int ret = ff_get_wav_header(pb, st->codec, type_specific_size);
        if (ret < 0)
            return ret;
        if (is_dvr_ms_audio) {
            // codec_id and codec_tag are unreliable in dvr_ms
            // files. Set them later by probing stream.
            st->request_probe= 1;
            st->codec->codec_tag = 0;
        }
        if (st->codec->codec_id == CODEC_ID_AAC) {
            st->need_parsing = AVSTREAM_PARSE_NONE;
        } else {
            st->need_parsing = AVSTREAM_PARSE_FULL;
        }
        /* We have to init the frame size at some point .... */
        pos2 = avio_tell(pb);
        if (size >= (pos2 + 8 - pos1 + 24)) {
            asf_st->ds_span = avio_r8(pb);
            asf_st->ds_packet_size = avio_rl16(pb);
            asf_st->ds_chunk_size = avio_rl16(pb);
            avio_rl16(pb); //ds_data_size
            avio_r8(pb);   //ds_silence_data
        }
        //printf("Descrambling: ps:%d cs:%d ds:%d s:%d  sd:%d\n",
        //       asf_st->ds_packet_size, asf_st->ds_chunk_size,
        //       asf_st->ds_data_size, asf_st->ds_span, asf_st->ds_silence_data);
        if (asf_st->ds_span > 1) {
            if (!asf_st->ds_chunk_size
                    || (asf_st->ds_packet_size/asf_st->ds_chunk_size <= 1)
                    || asf_st->ds_packet_size % asf_st->ds_chunk_size)
                asf_st->ds_span = 0; // disable descrambling
        }
    } else if (type == AVMEDIA_TYPE_VIDEO &&
            size - (avio_tell(pb) - pos1 + 24) >= 51) {
        avio_rl32(pb);
        avio_rl32(pb);
        avio_r8(pb);
        avio_rl16(pb);        /* size */
        sizeX= avio_rl32(pb); /* size */
        st->codec->width = avio_rl32(pb);
        st->codec->height = avio_rl32(pb);
        /* not available for asf */
        avio_rl16(pb); /* panes */
        st->codec->bits_per_coded_sample = avio_rl16(pb); /* depth */
        tag1 = avio_rl32(pb);
        avio_skip(pb, 20);
        //                av_log(s, AV_LOG_DEBUG, "size:%d tsize:%d sizeX:%d\n", size, total_size, sizeX);
        if (sizeX > 40) {
            st->codec->extradata_size = sizeX - 40;
            st->codec->extradata = av_mallocz(st->codec->extradata_size + FF_INPUT_BUFFER_PADDING_SIZE);
            avio_read(pb, st->codec->extradata, st->codec->extradata_size);
        }

        /* Extract palette from extradata if bpp <= 8 */
        /* This code assumes that extradata contains only palette */
        /* This is true for all paletted codecs implemented in libavcodec */
        if (st->codec->extradata_size && (st->codec->bits_per_coded_sample <= 8)) {
#if HAVE_BIGENDIAN
            int i;
            for (i = 0; i < FFMIN(st->codec->extradata_size, AVPALETTE_SIZE)/4; i++)
                asf_st->palette[i] = av_bswap32(((uint32_t*)st->codec->extradata)[i]);
#else
            memcpy(asf_st->palette, st->codec->extradata,
                   FFMIN(st->codec->extradata_size, AVPALETTE_SIZE));
#endif
            asf_st->palette_changed = 1;
        }

        st->codec->codec_tag = tag1;
        st->codec->codec_id = ff_codec_get_id(ff_codec_bmp_tags, tag1);
        if(tag1 == MKTAG('D', 'V', 'R', ' ')){
            st->need_parsing = AVSTREAM_PARSE_FULL;
            // issue658 containse wrong w/h and MS even puts a fake seq header with wrong w/h in extradata while a correct one is in te stream. maximum lameness
            st->codec->width  =
                st->codec->height = 0;
            av_freep(&st->codec->extradata);
            st->codec->extradata_size=0;
        }
        if(st->codec->codec_id == CODEC_ID_H264)
            st->need_parsing = AVSTREAM_PARSE_FULL_ONCE;
    }
    pos2 = avio_tell(pb);
    avio_skip(pb, size - (pos2 - pos1 + 24));

    return 0;
}

static int asf_read_ext_stream_properties(AVFormatContext *s, int64_t size)
{
    ASFContext *asf = s->priv_data;
    AVIOContext *pb = s->pb;
    ff_asf_guid g;
    int ext_len, payload_ext_ct, stream_ct, i;
    uint32_t leak_rate, stream_num;
    unsigned int stream_languageid_index;

    avio_rl64(pb); // starttime
    avio_rl64(pb); // endtime
    leak_rate = avio_rl32(pb); // leak-datarate
    avio_rl32(pb); // bucket-datasize
    avio_rl32(pb); // init-bucket-fullness
    avio_rl32(pb); // alt-leak-datarate
    avio_rl32(pb); // alt-bucket-datasize
    avio_rl32(pb); // alt-init-bucket-fullness
    avio_rl32(pb); // max-object-size
    avio_rl32(pb); // flags (reliable,seekable,no_cleanpoints?,resend-live-cleanpoints, rest of bits reserved)
    stream_num = avio_rl16(pb); // stream-num

    stream_languageid_index = avio_rl16(pb); // stream-language-id-index
    if (stream_num < 128)
        asf->streams[stream_num].stream_language_index = stream_languageid_index;

    avio_rl64(pb); // avg frametime in 100ns units
    stream_ct = avio_rl16(pb); //stream-name-count
    payload_ext_ct = avio_rl16(pb); //payload-extension-system-count

    if (stream_num < 128)
        asf->stream_bitrates[stream_num] = leak_rate;

    for (i=0; i<stream_ct; i++){
        avio_rl16(pb);
        ext_len = avio_rl16(pb);
        avio_skip(pb, ext_len);
    }

    for (i=0; i<payload_ext_ct; i++){
        ff_get_guid(pb, &g);
        avio_skip(pb, 2);
        ext_len=avio_rl32(pb);
        avio_skip(pb, ext_len);
    }

    return 0;
}

static int asf_read_content_desc(AVFormatContext *s, int64_t size)
{
    AVIOContext *pb = s->pb;
    int len1, len2, len3, len4, len5;

    len1 = avio_rl16(pb);
    len2 = avio_rl16(pb);
    len3 = avio_rl16(pb);
    len4 = avio_rl16(pb);
    len5 = avio_rl16(pb);
    get_tag(s, "title"    , 0, len1);
    get_tag(s, "author"   , 0, len2);
    get_tag(s, "copyright", 0, len3);
    get_tag(s, "comment"  , 0, len4);
    avio_skip(pb, len5);

    return 0;
}

static int asf_read_ext_content_desc(AVFormatContext *s, int64_t size)
{
    AVIOContext *pb = s->pb;
    ASFContext *asf = s->priv_data;
    int desc_count, i, ret;

    desc_count = avio_rl16(pb);
    for(i=0;i<desc_count;i++) {
        int name_len,value_type,value_len;
        char name[1024];

        name_len = avio_rl16(pb);
        if (name_len%2)     // must be even, broken lavf versions wrote len-1
            name_len += 1;
        if ((ret = avio_get_str16le(pb, name_len, name, sizeof(name))) < name_len)
            avio_skip(pb, name_len - ret);
        value_type = avio_rl16(pb);
        value_len  = avio_rl16(pb);
        if (!value_type && value_len%2)
            value_len += 1;
        /**
         * My sample has that stream set to 0 maybe that mean the container.
         * Asf stream count start at 1. I am using 0 to the container value since it's unused
         */
        if (!strcmp(name, "AspectRatioX")){
            asf->dar[0].num= get_value(s->pb, value_type);
        } else if(!strcmp(name, "AspectRatioY")){
            asf->dar[0].den= get_value(s->pb, value_type);
        } else
            get_tag(s, name, value_type, value_len);
    }

    return 0;
}

static int asf_read_language_list(AVFormatContext *s, int64_t size)
{
    AVIOContext *pb = s->pb;
    ASFContext *asf = s->priv_data;
    int j, ret;
    int stream_count = avio_rl16(pb);
    for(j = 0; j < stream_count; j++) {
        char lang[6];
        unsigned int lang_len = avio_r8(pb);
        if ((ret = avio_get_str16le(pb, lang_len, lang, sizeof(lang))) < lang_len)
            avio_skip(pb, lang_len - ret);
        if (j < 128)
            av_strlcpy(asf->stream_languages[j], lang, sizeof(*asf->stream_languages));
    }

    return 0;
}

static int asf_read_metadata(AVFormatContext *s, int64_t size)
{
    AVIOContext *pb = s->pb;
    ASFContext *asf = s->priv_data;
    int n, stream_num, name_len, value_len, value_num;
    int ret, i;
    n = avio_rl16(pb);

    for(i=0;i<n;i++) {
        char name[1024];

        avio_rl16(pb); //lang_list_index
        stream_num= avio_rl16(pb);
        name_len=   avio_rl16(pb);
        avio_skip(pb, 2); /* value_type */
        value_len=  avio_rl32(pb);

        if ((ret = avio_get_str16le(pb, name_len, name, sizeof(name))) < name_len)
            avio_skip(pb, name_len - ret);
        //av_log(s, AV_LOG_ERROR, "%d %d %d %d %d <%s>\n", i, stream_num, name_len, value_type, value_len, name);
        value_num= avio_rl16(pb);//we should use get_value() here but it does not work 2 is le16 here but le32 elsewhere
        avio_skip(pb, value_len - 2);

		if (!strcmp(name, "WM/Picture")) {
			//so far so good
			wm_picture_type2 = 1;
			asf_read_picture(s, value_len);
			//wiim_inf = value_len;
		}

        if(stream_num<128){
            if     (!strcmp(name, "AspectRatioX")) asf->dar[stream_num].num= value_num;
            else if(!strcmp(name, "AspectRatioY")) asf->dar[stream_num].den= value_num;
        }
    }

    return 0;
}

static int asf_read_marker(AVFormatContext *s, int64_t size)
{
    AVIOContext *pb = s->pb;
    int i, count, name_len, ret;
    char name[1024];

    avio_rl64(pb);            // reserved 16 bytes
    avio_rl64(pb);            // ...
    count = avio_rl32(pb);    // markers count
    avio_rl16(pb);            // reserved 2 bytes
    name_len = avio_rl16(pb); // name length
    for(i=0;i<name_len;i++){
        avio_r8(pb); // skip the name
    }

    for(i=0;i<count;i++){
        int64_t pres_time;
        int name_len;

        avio_rl64(pb);             // offset, 8 bytes
        pres_time = avio_rl64(pb); // presentation time
        avio_rl16(pb);             // entry length
        avio_rl32(pb);             // send time
        avio_rl32(pb);             // flags
        name_len = avio_rl32(pb);  // name length
        if ((ret = avio_get_str16le(pb, name_len * 2, name, sizeof(name))) < name_len)
            avio_skip(pb, name_len - ret);
        avpriv_new_chapter(s, i, (AVRational){1, 10000000}, pres_time, AV_NOPTS_VALUE, name );
    }

    return 0;
}

static int asf_read_header(AVFormatContext *s)
{
    ASFContext *asf = s->priv_data;
    ff_asf_guid g;
    AVIOContext *pb = s->pb;
    int i;
    int64_t gsize;

    ff_get_guid(pb, &g);
    if (ff_guidcmp(&g, &ff_asf_header))
        return -1;
    avio_rl64(pb);
    avio_rl32(pb);
    avio_r8(pb);
    avio_r8(pb);
    memset(&asf->asfid2avid, -1, sizeof(asf->asfid2avid));
    for(;;) {
        uint64_t gpos= avio_tell(pb);
        ff_get_guid(pb, &g);
        gsize = avio_rl64(pb);
        av_dlog(s, "%08"PRIx64": ", gpos);
        print_guid(&g);
        av_dlog(s, "  size=0x%"PRIx64"\n", gsize);
        if (!ff_guidcmp(&g, &ff_asf_data_header)) {
            asf->data_object_offset = avio_tell(pb);
            // if not streaming, gsize is not unlimited (how?), and there is enough space in the file..
            if (!(asf->hdr.flags & 0x01) && gsize >= 100) {
                asf->data_object_size = gsize - 24;
            } else {
                asf->data_object_size = (uint64_t)-1;
            }
            break;
        }
        if (gsize < 24)
            return -1;
        if (!ff_guidcmp(&g, &ff_asf_file_header)) {
            int ret = asf_read_file_properties(s, gsize);
            if (ret < 0)
                return ret;
        } else if (!ff_guidcmp(&g, &ff_asf_stream_header)) {
            asf_read_stream_properties(s, gsize);
        } else if (!ff_guidcmp(&g, &ff_asf_comment_header)) {
            asf_read_content_desc(s, gsize);
        } else if (!ff_guidcmp(&g, &ff_asf_language_guid)) {
            asf_read_language_list(s, gsize);
        } else if (!ff_guidcmp(&g, &ff_asf_extended_content_header)) {
            asf_read_ext_content_desc(s, gsize);
        } else if (!ff_guidcmp(&g, &ff_asf_metadata_header)) {
            asf_read_metadata(s, gsize);
		} else if (!ff_guidcmp(&g, &ff_asf_metadata_library_header)) {
            asf_read_metadata(s, gsize);
        } else if (!ff_guidcmp(&g, &ff_asf_ext_stream_header)) {
            asf_read_ext_stream_properties(s, gsize);

            // there could be a optional stream properties object to follow
            // if so the next iteration will pick it up
            continue;
        } else if (!ff_guidcmp(&g, &ff_asf_head1_guid)) {
            ff_get_guid(pb, &g);
            avio_skip(pb, 6);
            continue;
        } else if (!ff_guidcmp(&g, &ff_asf_marker_header)) {
            asf_read_marker(s, gsize);
        } else if (url_feof(pb)) {
            return -1;
        } else {
            if (!s->keylen) {
                if (!ff_guidcmp(&g, &ff_asf_content_encryption)) {
                    unsigned int len;
                    AVPacket pkt;
                    av_log(s, AV_LOG_WARNING, "DRM protected stream detected, decoding will likely fail!\n");
                    len= avio_rl32(pb);
                    av_log(s, AV_LOG_DEBUG, "Secret data:\n");
                    av_get_packet(pb, &pkt, len); av_hex_dump_log(s, AV_LOG_DEBUG, pkt.data, pkt.size); av_free_packet(&pkt);
                    len= avio_rl32(pb);
                    get_tag(s, "ASF_Protection_Type", -1, len);
                    len= avio_rl32(pb);
                    get_tag(s, "ASF_Key_ID", -1, len);
                    len= avio_rl32(pb);
                    get_tag(s, "ASF_License_URL", -1, len);
                } else if (!ff_guidcmp(&g, &ff_asf_ext_content_encryption)) {
                    av_log(s, AV_LOG_WARNING, "Ext DRM protected stream detected, decoding will likely fail!\n");
                    av_dict_set(&s->metadata, "encryption", "ASF Extended Content Encryption", 0);
                } else if (!ff_guidcmp(&g, &ff_asf_digital_signature)) {
                    av_log(s, AV_LOG_INFO, "Digital signature detected!\n");
                }
            }
        }
        if(avio_tell(pb) != gpos + gsize)
            av_log(s, AV_LOG_DEBUG, "gpos mismatch our pos=%"PRIu64", end=%"PRIu64"\n", avio_tell(pb)-gpos, gsize);
        avio_seek(pb, gpos + gsize, SEEK_SET);
    }
    ff_get_guid(pb, &g);
    avio_rl64(pb);
    avio_r8(pb);
    avio_r8(pb);
    if (url_feof(pb))
        return -1;
    asf->data_offset = avio_tell(pb);
    asf->packet_size_left = 0;


    for(i=0; i<128; i++){
        int stream_num= asf->asfid2avid[i];
        if(stream_num>=0){
            AVStream *st = s->streams[stream_num];
            if (!st->codec->bit_rate)
                st->codec->bit_rate = asf->stream_bitrates[i];
            if (asf->dar[i].num > 0 && asf->dar[i].den > 0){
                av_reduce(&st->sample_aspect_ratio.num,
                          &st->sample_aspect_ratio.den,
                          asf->dar[i].num, asf->dar[i].den, INT_MAX);
            } else if ((asf->dar[0].num > 0) && (asf->dar[0].den > 0) && (st->codec->codec_type==AVMEDIA_TYPE_VIDEO)) // Use ASF container value if the stream doesn't AR set.
                av_reduce(&st->sample_aspect_ratio.num,
                          &st->sample_aspect_ratio.den,
                          asf->dar[0].num, asf->dar[0].den, INT_MAX);

//av_log(s, AV_LOG_INFO, "i=%d, st->codec->codec_type:%d, dar %d:%d sar=%d:%d\n", i, st->codec->codec_type, dar[i].num, dar[i].den, st->sample_aspect_ratio.num, st->sample_aspect_ratio.den);

            // copy and convert language codes to the frontend
            if (asf->streams[i].stream_language_index < 128) {
                const char *rfc1766 = asf->stream_languages[asf->streams[i].stream_language_index];
                if (rfc1766 && strlen(rfc1766) > 1) {
                    const char primary_tag[3] = { rfc1766[0], rfc1766[1], '\0' }; // ignore country code if any
                    const char *iso6392 = av_convert_lang_to(primary_tag, AV_LANG_ISO639_2_BIBL);
                    if (iso6392)
                        av_dict_set(&st->metadata, "language", iso6392, 0);
                }
            }
        }
    }

    ff_metadata_conv(&s->metadata, NULL, ff_asf_metadata_conv);

    return 0;
}

#define DO_2BITS(bits, var, defval) \
    switch (bits & 3) \
    { \
    case 3: var = avio_rl32(pb); rsize += 4; break; \
    case 2: var = avio_rl16(pb); rsize += 2; break; \
    case 1: var = avio_r8(pb);   rsize++; break; \
    default: var = defval; break; \
    }

/**
 * Load a single ASF packet into the demuxer.
 * @param s demux context
 * @param pb context to read data from
 * @return 0 on success, <0 on error
 */
static int ff_asf_get_packet(AVFormatContext *s, AVIOContext *pb)
{
    ASFContext *asf = s->priv_data;
    uint32_t packet_length, padsize;
    int rsize = 8;
    int c, d, e, off;

    // if we do not know packet size, allow skipping up to 32 kB
    off= 32768;
    if (asf->no_resync_search)
        off = 3;
    else if (s->packet_size > 0)
        off= (avio_tell(pb) - s->data_offset) % s->packet_size + 3;

    c=d=e=-1;
    while(off-- > 0){
        c=d; d=e;
        e= avio_r8(pb);
        if(c == 0x82 && !d && !e)
            break;
    }

    if (c != 0x82) {
        /**
         * This code allows handling of -EAGAIN at packet boundaries (i.e.
         * if the packet sync code above triggers -EAGAIN). This does not
         * imply complete -EAGAIN handling support at random positions in
         * the stream.
         */
        if (pb->error == AVERROR(EAGAIN))
            return AVERROR(EAGAIN);
        if (!url_feof(pb))
            av_log(s, AV_LOG_ERROR, "ff asf bad header %x  at:%"PRId64"\n", c, avio_tell(pb));
    }
    if ((c & 0x8f) == 0x82) {
        if (d || e) {
            if (!url_feof(pb))
                av_log(s, AV_LOG_ERROR, "ff asf bad non zero\n");
            return -1;
        }
        c= avio_r8(pb);
        d= avio_r8(pb);
        rsize+=3;
    }else if(!url_feof(pb)){
        avio_seek(pb, -1, SEEK_CUR); //FIXME
    }

    asf->packet_flags    = c;
    asf->packet_property = d;

    DO_2BITS(asf->packet_flags >> 5, packet_length, s->packet_size);
    DO_2BITS(asf->packet_flags >> 1, padsize, 0); // sequence ignored
    DO_2BITS(asf->packet_flags >> 3, padsize, 0); // padding length

    //the following checks prevent overflows and infinite loops
    if(!packet_length || packet_length >= (1U<<29)){
        av_log(s, AV_LOG_ERROR, "invalid packet_length %d at:%"PRId64"\n", packet_length, avio_tell(pb));
        return -1;
    }
    if(padsize >= packet_length){
        av_log(s, AV_LOG_ERROR, "invalid padsize %d at:%"PRId64"\n", padsize, avio_tell(pb));
        return -1;
    }

    asf->packet_timestamp = avio_rl32(pb);
    avio_rl16(pb); /* duration */
    // rsize has at least 11 bytes which have to be present

    if (asf->packet_flags & 0x01) {
        asf->packet_segsizetype = avio_r8(pb); rsize++;
        asf->packet_segments = asf->packet_segsizetype & 0x3f;
    } else {
        asf->packet_segments = 1;
        asf->packet_segsizetype = 0x80;
    }
    if (rsize > packet_length - padsize) {
        asf->packet_size_left = 0;
        av_log(s, AV_LOG_ERROR,
               "invalid packet header length %d for pktlen %d-%d at %"PRId64"\n",
               rsize, packet_length, padsize, avio_tell(pb));
        return -1;
    }
    asf->packet_size_left = packet_length - padsize - rsize;
    if (packet_length < asf->hdr.min_pktsize)
        padsize += asf->hdr.min_pktsize - packet_length;
    asf->packet_padsize = padsize;
    av_dlog(s, "packet: size=%d padsize=%d  left=%d\n", s->packet_size, asf->packet_padsize, asf->packet_size_left);
    return 0;
}

/**
 *
 * @return <0 if error
 */
static int asf_read_frame_header(AVFormatContext *s, AVIOContext *pb){
    ASFContext *asf = s->priv_data;
    int rsize = 1;
    int num = avio_r8(pb);
    int64_t ts0, ts1 av_unused;

    asf->packet_segments--;
    asf->packet_key_frame = num >> 7;
    asf->stream_index = asf->asfid2avid[num & 0x7f];
    // sequence should be ignored!
    DO_2BITS(asf->packet_property >> 4, asf->packet_seq, 0);
    DO_2BITS(asf->packet_property >> 2, asf->packet_frag_offset, 0);
    DO_2BITS(asf->packet_property, asf->packet_replic_size, 0);
//printf("key:%d stream:%d seq:%d offset:%d replic_size:%d\n", asf->packet_key_frame, asf->stream_index, asf->packet_seq, //asf->packet_frag_offset, asf->packet_replic_size);
    if (rsize+asf->packet_replic_size > asf->packet_size_left) {
        av_log(s, AV_LOG_ERROR, "packet_replic_size %d is invalid\n", asf->packet_replic_size);
        return -1;
    }
    if (asf->packet_replic_size >= 8) {
        asf->packet_obj_size = avio_rl32(pb);
        if(asf->packet_obj_size >= (1<<24) || asf->packet_obj_size <= 0){
            av_log(s, AV_LOG_ERROR, "packet_obj_size invalid\n");
			asf->packet_obj_size = 0;
            return -1;
        }
        asf->packet_frag_timestamp = avio_rl32(pb); // timestamp
        if(asf->packet_replic_size >= 8+38+4){
//            for(i=0; i<asf->packet_replic_size-8; i++)
//                av_log(s, AV_LOG_DEBUG, "%02X ",avio_r8(pb));
//            av_log(s, AV_LOG_DEBUG, "\n");
            avio_skip(pb, 10);
            ts0= avio_rl64(pb);
            ts1= avio_rl64(pb);
            avio_skip(pb, 12);
            avio_rl32(pb);
            avio_skip(pb, asf->packet_replic_size - 8 - 38 - 4);
            if(ts0!= -1) asf->packet_frag_timestamp= ts0/10000;
            else         asf->packet_frag_timestamp= AV_NOPTS_VALUE;
        }else
            avio_skip(pb, asf->packet_replic_size - 8);
        rsize += asf->packet_replic_size; // FIXME - check validity
    } else if (asf->packet_replic_size==1){
        // multipacket - frag_offset is beginning timestamp
        asf->packet_time_start = asf->packet_frag_offset;
        asf->packet_frag_offset = 0;
        asf->packet_frag_timestamp = asf->packet_timestamp;

        asf->packet_time_delta = avio_r8(pb);
        rsize++;
    }else if(asf->packet_replic_size!=0){
        av_log(s, AV_LOG_ERROR, "unexpected packet_replic_size of %d\n", asf->packet_replic_size);
        return -1;
    }
    if (asf->packet_flags & 0x01) {
        DO_2BITS(asf->packet_segsizetype >> 6, asf->packet_frag_size, 0); // 0 is illegal
        if (rsize > asf->packet_size_left) {
            av_log(s, AV_LOG_ERROR, "packet_replic_size is invalid\n");
            return -1;
        } else if(asf->packet_frag_size > asf->packet_size_left - rsize){
            if (asf->packet_frag_size > asf->packet_size_left - rsize + asf->packet_padsize) {
                av_log(s, AV_LOG_ERROR, "packet_frag_size is invalid (%d-%d)\n", asf->packet_size_left, rsize);
                return -1;
            } else {
                int diff = asf->packet_frag_size - (asf->packet_size_left - rsize);
                asf->packet_size_left += diff;
                asf->packet_padsize   -= diff;
            }
        }
        //printf("Fragsize %d\n", asf->packet_frag_size);
    } else {
        asf->packet_frag_size = asf->packet_size_left - rsize;
        //printf("Using rest  %d %d %d\n", asf->packet_frag_size, asf->packet_size_left, rsize);
    }
    if (asf->packet_replic_size == 1) {
        asf->packet_multi_size = asf->packet_frag_size;
        if (asf->packet_multi_size > asf->packet_size_left)
            return -1;
    }
    asf->packet_size_left -= rsize;
    //printf("___objsize____  %d   %d    rs:%d\n", asf->packet_obj_size, asf->packet_frag_offset, rsize);

    return 0;
}

/**
 * Parse data from individual ASF packets (which were previously loaded
 * with asf_get_packet()).
 * @param s demux context
 * @param pb context to read data from
 * @param pkt pointer to store packet data into
 * @return 0 if data was stored in pkt, <0 on error or 1 if more ASF
 *          packets need to be loaded (through asf_get_packet())
 */
static int ff_asf_parse_packet(AVFormatContext *s, AVIOContext *pb, AVPacket *pkt)
{
    ASFContext *asf = s->priv_data;
    ASFStream *asf_st = 0;
    for (;;) {
        int ret;
        if(url_feof(pb))
            return AVERROR_EOF;
        if (asf->packet_size_left < FRAME_HEADER_SIZE
            || asf->packet_segments < 1) {
            //asf->packet_size_left <= asf->packet_padsize) {
            int ret = asf->packet_size_left + asf->packet_padsize;
            //printf("PacketLeftSize:%d  Pad:%d Pos:%"PRId64"\n", asf->packet_size_left, asf->packet_padsize, avio_tell(pb));
            assert(ret>=0);
            /* fail safe */
            avio_skip(pb, ret);

            asf->packet_pos= avio_tell(pb);
            if (asf->data_object_size != (uint64_t)-1 &&
                (asf->packet_pos - asf->data_object_offset >= asf->data_object_size))
                return AVERROR_EOF; /* Do not exceed the size of the data object */
            return 1;
        }
        if (asf->packet_time_start == 0) {
            if(asf_read_frame_header(s, pb) < 0){
                asf->packet_segments= 0;
                continue;
            }
            if (asf->stream_index < 0
                || s->streams[asf->stream_index]->discard >= AVDISCARD_ALL
                || (!asf->packet_key_frame && s->streams[asf->stream_index]->discard >= AVDISCARD_NONKEY)
                ) {
                asf->packet_time_start = 0;
                /* unhandled packet (should not happen) */
                avio_skip(pb, asf->packet_frag_size);
                asf->packet_size_left -= asf->packet_frag_size;
                if(asf->stream_index < 0)
                    av_log(s, AV_LOG_ERROR, "ff asf skip %d (unknown stream)\n", asf->packet_frag_size);
                continue;
            }
            asf->asf_st = s->streams[asf->stream_index]->priv_data;
        }
        asf_st = asf->asf_st;

        if (asf->packet_replic_size == 1) {
            // frag_offset is here used as the beginning timestamp
            asf->packet_frag_timestamp = asf->packet_time_start;
            asf->packet_time_start += asf->packet_time_delta;
            asf->packet_obj_size = asf->packet_frag_size = avio_r8(pb);
            asf->packet_size_left--;
            asf->packet_multi_size--;
            if (asf->packet_multi_size < asf->packet_obj_size)
            {
                asf->packet_time_start = 0;
                avio_skip(pb, asf->packet_multi_size);
                asf->packet_size_left -= asf->packet_multi_size;
                continue;
            }
            asf->packet_multi_size -= asf->packet_obj_size;
            //printf("COMPRESS size  %d  %d  %d   ms:%d\n", asf->packet_obj_size, asf->packet_frag_timestamp, asf->packet_size_left, asf->packet_multi_size);
        }
        if(   /*asf->packet_frag_size == asf->packet_obj_size*/
              asf_st->frag_offset + asf->packet_frag_size <= asf_st->pkt.size
           && asf_st->frag_offset + asf->packet_frag_size > asf->packet_obj_size){
            av_log(s, AV_LOG_INFO, "ignoring invalid packet_obj_size (%d %d %d %d)\n",
                asf_st->frag_offset, asf->packet_frag_size,
                asf->packet_obj_size, asf_st->pkt.size);
            asf->packet_obj_size= asf_st->pkt.size;
        }

        if (   asf_st->pkt.size != asf->packet_obj_size
            || asf_st->frag_offset + asf->packet_frag_size > asf_st->pkt.size) { //FIXME is this condition sufficient?
            if(asf_st->pkt.data){
                av_log(s, AV_LOG_INFO, "freeing incomplete packet size %d, new %d\n", asf_st->pkt.size, asf->packet_obj_size);
                asf_st->frag_offset = 0;
                av_free_packet(&asf_st->pkt);
            }
            /* new packet */
            av_new_packet(&asf_st->pkt, asf->packet_obj_size);
            asf_st->seq = asf->packet_seq;
            asf_st->pkt.dts = asf->packet_frag_timestamp - asf->hdr.preroll;
            asf_st->pkt.stream_index = asf->stream_index;
            asf_st->pkt.pos =
            asf_st->packet_pos= asf->packet_pos;
            if (asf_st->pkt.data && asf_st->palette_changed) {
                uint8_t *pal;
                pal = av_packet_new_side_data(&asf_st->pkt, AV_PKT_DATA_PALETTE,
                                              AVPALETTE_SIZE);
                if (!pal) {
                    av_log(s, AV_LOG_ERROR, "Cannot append palette to packet\n");
                } else {
                    memcpy(pal, asf_st->palette, AVPALETTE_SIZE);
                    asf_st->palette_changed = 0;
                }
            }
//printf("new packet: stream:%d key:%d packet_key:%d audio:%d size:%d\n",
//asf->stream_index, asf->packet_key_frame, asf_st->pkt.flags & AV_PKT_FLAG_KEY,
//s->streams[asf->stream_index]->codec->codec_type == AVMEDIA_TYPE_AUDIO, asf->packet_obj_size);
            if (s->streams[asf->stream_index]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
                asf->packet_key_frame = 1;
            if (asf->packet_key_frame)
                asf_st->pkt.flags |= AV_PKT_FLAG_KEY;
        }

        /* read data */
        //printf("READ PACKET s:%d  os:%d  o:%d,%d  l:%d   DATA:%p\n",
        //       s->packet_size, asf_st->pkt.size, asf->packet_frag_offset,
        //       asf_st->frag_offset, asf->packet_frag_size, asf_st->pkt.data);
        asf->packet_size_left -= asf->packet_frag_size;
        if (asf->packet_size_left < 0)
            continue;

        if(   asf->packet_frag_offset >= asf_st->pkt.size
           || asf->packet_frag_size > asf_st->pkt.size - asf->packet_frag_offset){
            av_log(s, AV_LOG_ERROR, "packet fragment position invalid %u,%u not in %u\n",
                asf->packet_frag_offset, asf->packet_frag_size, asf_st->pkt.size);
            continue;
        }

        ret = avio_read(pb, asf_st->pkt.data + asf->packet_frag_offset,
                         asf->packet_frag_size);
        if (ret != asf->packet_frag_size) {
            if (ret < 0 || asf->packet_frag_offset + ret == 0)
                return ret < 0 ? ret : AVERROR_EOF;
            if (asf_st->ds_span > 1) {
                // scrambling, we can either drop it completely or fill the remainder
                // TODO: should we fill the whole packet instead of just the current
                // fragment?
                memset(asf_st->pkt.data + asf->packet_frag_offset + ret, 0,
                       asf->packet_frag_size - ret);
                ret = asf->packet_frag_size;
            } else
                // no scrambling, so we can return partial packets
                av_shrink_packet(&asf_st->pkt, asf->packet_frag_offset + ret);
        }
        if (s->key && s->keylen == 20)
            ff_asfcrypt_dec(s->key, asf_st->pkt.data + asf->packet_frag_offset,
                            ret);
        asf_st->frag_offset += ret;
        /* test if whole packet is read */
        if (asf_st->frag_offset == asf_st->pkt.size) {
            //workaround for macroshit radio DVR-MS files
            if(   s->streams[asf->stream_index]->codec->codec_id == CODEC_ID_MPEG2VIDEO
               && asf_st->pkt.size > 100){
                int i;
                for(i=0; i<asf_st->pkt.size && !asf_st->pkt.data[i]; i++);
                if(i == asf_st->pkt.size){
                    av_log(s, AV_LOG_DEBUG, "discarding ms fart\n");
                    asf_st->frag_offset = 0;
                    av_free_packet(&asf_st->pkt);
                    continue;
                }
            }

            /* return packet */
            if (asf_st->ds_span > 1) {
              if(asf_st->pkt.size != asf_st->ds_packet_size * asf_st->ds_span){
                    av_log(s, AV_LOG_ERROR, "pkt.size != ds_packet_size * ds_span (%d %d %d)\n", asf_st->pkt.size, asf_st->ds_packet_size, asf_st->ds_span);
              }else{
                /* packet descrambling */
                uint8_t *newdata = av_malloc(asf_st->pkt.size + FF_INPUT_BUFFER_PADDING_SIZE);
                if (newdata) {
                    int offset = 0;
                    memset(newdata + asf_st->pkt.size, 0, FF_INPUT_BUFFER_PADDING_SIZE);
                    while (offset < asf_st->pkt.size) {
                        int off = offset / asf_st->ds_chunk_size;
                        int row = off / asf_st->ds_span;
                        int col = off % asf_st->ds_span;
                        int idx = row + col * asf_st->ds_packet_size / asf_st->ds_chunk_size;
                        //printf("off:%d  row:%d  col:%d  idx:%d\n", off, row, col, idx);

                        assert(offset + asf_st->ds_chunk_size <= asf_st->pkt.size);
                        assert(idx+1 <= asf_st->pkt.size / asf_st->ds_chunk_size);
                        memcpy(newdata + offset,
                               asf_st->pkt.data + idx * asf_st->ds_chunk_size,
                               asf_st->ds_chunk_size);
                        offset += asf_st->ds_chunk_size;
                    }
                    av_free(asf_st->pkt.data);
                    asf_st->pkt.data = newdata;
                }
              }
            }
            asf_st->frag_offset = 0;
            *pkt= asf_st->pkt;
            //printf("packet %d %d\n", asf_st->pkt.size, asf->packet_frag_size);
            asf_st->pkt.size = 0;
            asf_st->pkt.data = 0;
            asf_st->pkt.side_data_elems = 0;
            asf_st->pkt.side_data = NULL;
            break; // packet completed
        }
    }
    return 0;
}

static int asf_read_packet(AVFormatContext *s, AVPacket *pkt)
{
    ASFContext *asf = s->priv_data;

    for (;;) {
        int ret;

        /* parse cached packets, if any */
        if ((ret = ff_asf_parse_packet(s, s->pb, pkt)) <= 0)
            return ret;
        if ((ret = ff_asf_get_packet(s, s->pb)) < 0)
            assert(asf->packet_size_left < FRAME_HEADER_SIZE || asf->packet_segments < 1);
        asf->packet_time_start = 0;
    }
}

// Added to support seeking after packets have been read
// If information is not reset, read_packet fails due to
// leftover information from previous reads
static void asf_reset_header(AVFormatContext *s)
{
    ASFContext *asf = s->priv_data;
    ASFStream *asf_st;
    int i;

    asf->packet_size_left = 0;
    asf->packet_segments = 0;
    asf->packet_flags = 0;
    asf->packet_property = 0;
    asf->packet_timestamp = 0;
    asf->packet_segsizetype = 0;
    asf->packet_segments = 0;
    asf->packet_seq = 0;
    asf->packet_replic_size = 0;
    asf->packet_key_frame = 0;
    asf->packet_padsize = 0;
    asf->packet_frag_offset = 0;
    asf->packet_frag_size = 0;
    asf->packet_frag_timestamp = 0;
    asf->packet_multi_size = 0;
    asf->packet_obj_size = 0;
    asf->packet_time_delta = 0;
    asf->packet_time_start = 0;

    for(i=0; i<s->nb_streams; i++){
        asf_st= s->streams[i]->priv_data;
		if (!asf_st)
            continue;
        av_free_packet(&asf_st->pkt);
        asf_st->frag_offset=0;
        asf_st->seq=0;
    }
    asf->asf_st= NULL;
}

static int asf_read_close(AVFormatContext *s)
{
    asf_reset_header(s);

    return 0;
}

static int64_t asf_read_pts(AVFormatContext *s, int stream_index, int64_t *ppos, int64_t pos_limit)
{
    AVPacket pkt1, *pkt = &pkt1;
    ASFStream *asf_st;
    int64_t pts;
    int64_t pos= *ppos;
    int i;
    int64_t start_pos[ASF_MAX_STREAMS];

    for(i=0; i<s->nb_streams; i++){
        start_pos[i]= pos;
    }

    if (s->packet_size > 0)
        pos= (pos+s->packet_size-1-s->data_offset)/s->packet_size*s->packet_size+ s->data_offset;
    *ppos= pos;
    if (avio_seek(s->pb, pos, SEEK_SET) < 0)
        return AV_NOPTS_VALUE;

//printf("asf_read_pts\n");
    asf_reset_header(s);
    for(;;){
        if (av_read_frame(s, pkt) < 0){
            av_log(s, AV_LOG_INFO, "asf_read_pts failed\n");
            return AV_NOPTS_VALUE;
        }

        pts = pkt->dts;

        av_free_packet(pkt);
        if(pkt->flags&AV_PKT_FLAG_KEY){
            i= pkt->stream_index;

            asf_st= s->streams[i]->priv_data;

//            assert((asf_st->packet_pos - s->data_offset) % s->packet_size == 0);
            pos= asf_st->packet_pos;

            av_add_index_entry(s->streams[i], pos, pts, pkt->size, pos - start_pos[i] + 1, AVINDEX_KEYFRAME);
            start_pos[i]= asf_st->packet_pos + 1;

            if(pkt->stream_index == stream_index)
               break;
        }
    }

    *ppos= pos;
//printf("found keyframe at %"PRId64" stream %d stamp:%"PRId64"\n", *ppos, stream_index, pts);

    return pts;
}

static void asf_build_simple_index(AVFormatContext *s, int stream_index)
{
    ff_asf_guid g;
    ASFContext *asf = s->priv_data;
    int64_t current_pos= avio_tell(s->pb);

    if(avio_seek(s->pb, asf->data_object_offset + asf->data_object_size, SEEK_SET) < 0) {
        asf->index_read= -1;
        return;
    }

    ff_get_guid(s->pb, &g);

    /* the data object can be followed by other top-level objects,
       skip them until the simple index object is reached */
    while (ff_guidcmp(&g, &ff_asf_simple_index_header)) {
        int64_t gsize= avio_rl64(s->pb);
        if (gsize < 24 || url_feof(s->pb)) {
            avio_seek(s->pb, current_pos, SEEK_SET);
            asf->index_read= -1;
            return;
        }
        avio_skip(s->pb, gsize-24);
        ff_get_guid(s->pb, &g);
    }

    {
        int64_t itime, last_pos=-1;
        int pct, ict;
        int i;
        int64_t av_unused gsize= avio_rl64(s->pb);
        ff_get_guid(s->pb, &g);
        itime=avio_rl64(s->pb);
        pct=avio_rl32(s->pb);
        ict=avio_rl32(s->pb);
        av_log(s, AV_LOG_DEBUG, "itime:0x%"PRIx64", pct:%d, ict:%d\n",itime,pct,ict);

        for (i=0;i<ict;i++){
            int pktnum=avio_rl32(s->pb);
            int pktct =avio_rl16(s->pb);
            int64_t pos      = s->data_offset + s->packet_size*(int64_t)pktnum;
            int64_t index_pts= FFMAX(av_rescale(itime, i, 10000) - asf->hdr.preroll, 0);

            if(pos != last_pos){
            av_log(s, AV_LOG_DEBUG, "pktnum:%d, pktct:%d  pts: %"PRId64"\n", pktnum, pktct, index_pts);
            av_add_index_entry(s->streams[stream_index], pos, index_pts, s->packet_size, 0, AVINDEX_KEYFRAME);
            last_pos=pos;
            }
        }
        asf->index_read= ict > 0;
    }
    avio_seek(s->pb, current_pos, SEEK_SET);
}

static int asf_read_seek(AVFormatContext *s, int stream_index, int64_t pts, int flags)
{
    ASFContext *asf = s->priv_data;
    AVStream *st = s->streams[stream_index];

    if (s->packet_size <= 0)
        return -1;

    /* Try using the protocol's read_seek if available */
    if(s->pb) {
        int ret = avio_seek_time(s->pb, stream_index, pts, flags);
        if(ret >= 0)
            asf_reset_header(s);
        if (ret != AVERROR(ENOSYS))
            return ret;
    }

    if (!asf->index_read)
        asf_build_simple_index(s, stream_index);

    if((asf->index_read > 0 && st->index_entries)){
        int index= av_index_search_timestamp(st, pts, flags);
        if(index >= 0) {
            /* find the position */
            uint64_t pos = st->index_entries[index].pos;

            /* do the seek */
            av_log(s, AV_LOG_DEBUG, "SEEKTO: %"PRId64"\n", pos);
            if(avio_seek(s->pb, pos, SEEK_SET) < 0)
                return -1;
            asf_reset_header(s);
            return 0;
        }
    }
    /* no index or seeking by index failed */
    if (ff_seek_frame_binary(s, stream_index, pts, flags) < 0)
        return -1;
    asf_reset_header(s);
    return 0;
}

AVInputFormat ff_asf_demuxer = {
    .name           = "asf",
    .long_name      = NULL_IF_CONFIG_SMALL("ASF format"),
    .priv_data_size = sizeof(ASFContext),
    .read_probe     = asf_probe,
    .read_header    = asf_read_header,
    .read_packet    = asf_read_packet,
    .read_close     = asf_read_close,
    .read_seek      = asf_read_seek,
    .read_timestamp = asf_read_pts,
    .flags          = AVFMT_NOBINSEARCH | AVFMT_NOGENSEARCH,
    .priv_class     = &asf_class,
};
