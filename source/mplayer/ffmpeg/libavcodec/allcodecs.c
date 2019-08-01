/*
 * Provide registration of all codecs, parsers and bitstream filters for libavcodec.
 * Copyright (c) 2002 Fabrice Bellard
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

/**
 * @file
 * Provide registration of all codecs, parsers and bitstream filters for libavcodec.
 */

#include "avcodec.h"

#define REGISTER_HWACCEL(X,x) { \
          extern AVHWAccel ff_##x##_hwaccel; \
          if(CONFIG_##X##_HWACCEL) av_register_hwaccel(&ff_##x##_hwaccel); }

#define REGISTER_ENCODER(X,x) { \
          extern AVCodec ff_##x##_encoder; \
          if(CONFIG_##X##_ENCODER)  avcodec_register(&ff_##x##_encoder); }
#define REGISTER_DECODER(X,x) { \
          extern AVCodec ff_##x##_decoder; \
          if(CONFIG_##X##_DECODER)  avcodec_register(&ff_##x##_decoder); }
#define REGISTER_ENCDEC(X,x)  REGISTER_ENCODER(X,x); REGISTER_DECODER(X,x)

#define REGISTER_PARSER(X,x) { \
          extern AVCodecParser ff_##x##_parser; \
          if(CONFIG_##X##_PARSER)  av_register_codec_parser(&ff_##x##_parser); }
#define REGISTER_BSF(X,x) { \
          extern AVBitStreamFilter ff_##x##_bsf; \
          if(CONFIG_##X##_BSF)     av_register_bitstream_filter(&ff_##x##_bsf); }

void avcodec_register_all(void)
{
    static int initialized;

    if (initialized)
        return;
    initialized = 1;

    /* hardware accelerators */
    REGISTER_HWACCEL (H263_VAAPI, h263_vaapi);
    REGISTER_HWACCEL (H264_DXVA2, h264_dxva2);
    REGISTER_HWACCEL (H264_VAAPI, h264_vaapi);
    REGISTER_HWACCEL (H264_VDA, h264_vda);
    REGISTER_HWACCEL (MPEG1_VDPAU, mpeg1_vdpau);
    REGISTER_HWACCEL (MPEG2_DXVA2, mpeg2_dxva2);
    REGISTER_HWACCEL (MPEG2_VAAPI, mpeg2_vaapi);
    REGISTER_HWACCEL (MPEG2_VDPAU, mpeg2_vdpau);
    REGISTER_HWACCEL (MPEG4_VAAPI, mpeg4_vaapi);
    REGISTER_HWACCEL (VC1_DXVA2, vc1_dxva2);
    REGISTER_HWACCEL (VC1_VAAPI, vc1_vaapi);
    REGISTER_HWACCEL (WMV3_DXVA2, wmv3_dxva2);
    REGISTER_HWACCEL (WMV3_VAAPI, wmv3_vaapi);

    /* video codecs */
    REGISTER_ENCODER (A64MULTI, a64multi);
    REGISTER_ENCODER (A64MULTI5, a64multi5);
    REGISTER_ENCDEC  (AMV, amv);
    REGISTER_ENCDEC  (AVRP, avrp);
    REGISTER_DECODER (AVS, avs);
    REGISTER_ENCDEC  (AVUI, avui);
    REGISTER_ENCDEC  (AYUV, ayuv);
    REGISTER_DECODER (BMV_VIDEO, bmv_video);
    REGISTER_DECODER (BINK, bink);
    REGISTER_DECODER (CDXL, cdxl);
    REGISTER_DECODER (DIRAC, dirac);
    REGISTER_DECODER (DSICINVIDEO, dsicinvideo);
    REGISTER_DECODER (DXTORY, dxtory);
    REGISTER_DECODER (EIGHTSVX_EXP, eightsvx_exp);
    REGISTER_DECODER (EIGHTSVX_FIB, eightsvx_fib);
    REGISTER_DECODER (ESCAPE130, escape130);
    REGISTER_DECODER (EXR, exr);
    REGISTER_ENCDEC  (FFVHUFF, ffvhuff);
    REGISTER_ENCDEC  (FLV, flv);
    REGISTER_ENCDEC  (H261, h261);
    REGISTER_ENCDEC  (H263, h263);
    REGISTER_DECODER (H263I, h263i);
    REGISTER_ENCODER (H263P, h263p);
    REGISTER_DECODER (H264, h264);
    REGISTER_DECODER (H264_CRYSTALHD, h264_crystalhd);
    REGISTER_DECODER (H264_VDPAU, h264_vdpau);
    REGISTER_ENCDEC  (HUFFYUV, huffyuv);
    REGISTER_DECODER (IFF_BYTERUN1, iff_byterun1);
    REGISTER_DECODER (IFF_ILBM, iff_ilbm);
    REGISTER_ENCDEC  (JPEG2000, jpeg2000);
    REGISTER_ENCDEC  (JPEGLS, jpegls);
    REGISTER_ENCODER (LJPEG, ljpeg);
    REGISTER_ENCDEC  (MJPEG, mjpeg);
    REGISTER_DECODER (MJPEGB, mjpegb);
    REGISTER_DECODER (MPEG_XVMC, mpeg_xvmc);
    REGISTER_ENCDEC  (MPEG1VIDEO, mpeg1video);
    REGISTER_ENCDEC  (MPEG2VIDEO, mpeg2video);
    REGISTER_ENCDEC  (MPEG4, mpeg4);
    REGISTER_DECODER (MPEG4_CRYSTALHD, mpeg4_crystalhd);
    REGISTER_DECODER (MPEG4_VDPAU, mpeg4_vdpau);
    REGISTER_DECODER (MPEGVIDEO, mpegvideo);
    REGISTER_DECODER (MPEG_VDPAU, mpeg_vdpau);
    REGISTER_DECODER (MPEG1_VDPAU, mpeg1_vdpau);
    REGISTER_DECODER (MPEG2_CRYSTALHD, mpeg2_crystalhd);
    REGISTER_DECODER (MSMPEG4_CRYSTALHD, msmpeg4_crystalhd);
    REGISTER_DECODER (MSMPEG4V1, msmpeg4v1);
    REGISTER_ENCDEC  (MSMPEG4V2, msmpeg4v2);
    REGISTER_ENCDEC  (MSMPEG4V3, msmpeg4v3);
    REGISTER_ENCDEC  (PNG, png);
    REGISTER_ENCDEC  (PRORES, prores);
    REGISTER_ENCODER (PRORES_ANATOLIY, prores_anatoliy);
    REGISTER_ENCODER (PRORES_KOSTYA, prores_kostya);
    REGISTER_DECODER (PRORES_LGPL, prores_lgpl);
    REGISTER_ENCDEC  (RAWVIDEO, rawvideo);
    REGISTER_DECODER (SMACKER, smacker);
    REGISTER_DECODER (SP5X, sp5x);
    REGISTER_ENCDEC  (SVQ1, svq1);
    REGISTER_DECODER (SVQ3, svq3);
    REGISTER_DECODER (THEORA, theora);
    REGISTER_DECODER (THP, thp);
    REGISTER_DECODER (UTVIDEO, utvideo);
    REGISTER_ENCDEC  (V308, v308);
    REGISTER_ENCDEC  (V408, v408);
    REGISTER_ENCDEC  (V410, v410);
    REGISTER_DECODER (VBLE, vble);
    REGISTER_DECODER (VC1, vc1);
    REGISTER_DECODER (VC1_CRYSTALHD, vc1_crystalhd);
    REGISTER_DECODER (VC1_VDPAU, vc1_vdpau);
    REGISTER_DECODER (VC1IMAGE, vc1image);
    REGISTER_DECODER (VMDVIDEO, vmdvideo);
    REGISTER_DECODER (VP3, vp3);
    REGISTER_DECODER (VP5, vp5);
    REGISTER_DECODER (VP6, vp6);
    REGISTER_DECODER (VP6A, vp6a);
    REGISTER_DECODER (VP6F, vp6f);
    REGISTER_DECODER (VP8, vp8);
    REGISTER_ENCDEC  (WMV1, wmv1);
    REGISTER_ENCDEC  (WMV2, wmv2);
    REGISTER_DECODER (WMV3, wmv3);
    REGISTER_DECODER (WMV3_CRYSTALHD, wmv3_crystalhd);
    REGISTER_DECODER (WMV3_VDPAU, wmv3_vdpau);
    REGISTER_DECODER (WMV3IMAGE, wmv3image);
    REGISTER_DECODER (WNV1, wnv1);
    REGISTER_DECODER (XAN_WC3, xan_wc3);
    REGISTER_DECODER (XAN_WC4, xan_wc4);
    REGISTER_ENCDEC  (XBM, xbm);
    REGISTER_ENCDEC  (XWD, xwd);
    REGISTER_ENCDEC  (Y41P, y41p);
    REGISTER_ENCDEC  (YUV4, yuv4);
    REGISTER_DECODER (ZEROCODEC, zerocodec);

    /* audio codecs */
    REGISTER_ENCDEC  (AAC, aac);
    REGISTER_DECODER (AAC_LATM, aac_latm);
    REGISTER_ENCDEC  (AC3, ac3);
    REGISTER_ENCODER (AC3_FIXED, ac3_fixed);
    REGISTER_ENCDEC  (ALAC, alac);
    REGISTER_DECODER (ALS, als);
    REGISTER_DECODER (AMRNB, amrnb);
    REGISTER_DECODER (AMRWB, amrwb);
    REGISTER_DECODER (APE, ape);
    REGISTER_DECODER (ATRAC1, atrac1);
    REGISTER_DECODER (ATRAC3, atrac3);
    REGISTER_DECODER (BINKAUDIO_DCT, binkaudio_dct);
    REGISTER_DECODER (BINKAUDIO_RDFT, binkaudio_rdft);
    REGISTER_DECODER (BMV_AUDIO, bmv_audio);
    REGISTER_DECODER (DSICINAUDIO, dsicinaudio);
    REGISTER_ENCDEC  (EAC3, eac3);
    REGISTER_DECODER (FFWAVESYNTH, ffwavesynth);
    REGISTER_ENCDEC  (FLAC, flac);
    REGISTER_ENCDEC  (G723_1, g723_1);
    REGISTER_DECODER (G729, g729);
    REGISTER_DECODER (GSM, gsm);
    REGISTER_DECODER (GSM_MS, gsm_ms);
    REGISTER_DECODER (IMC, imc);
    REGISTER_DECODER (MACE3, mace3);
    REGISTER_DECODER (MACE6, mace6);
    REGISTER_DECODER (MLP, mlp);
    REGISTER_DECODER (MP1, mp1);
    REGISTER_DECODER (MP1FLOAT, mp1float);
    REGISTER_ENCDEC  (MP2, mp2);
    REGISTER_DECODER (MP2FLOAT, mp2float);
    REGISTER_DECODER (MP3, mp3);
    REGISTER_DECODER (MP3FLOAT, mp3float);
    REGISTER_DECODER (MP3ADU, mp3adu);
    REGISTER_DECODER (MP3ADUFLOAT, mp3adufloat);
    REGISTER_DECODER (MP3ON4, mp3on4);
    REGISTER_DECODER (MP3ON4FLOAT, mp3on4float);
    REGISTER_DECODER (MPC7, mpc7);
    REGISTER_DECODER (MPC8, mpc8);
    REGISTER_DECODER (RALF, ralf);
    REGISTER_DECODER (SIPR, sipr);
    REGISTER_DECODER (SMACKAUD, smackaud);
    REGISTER_ENCDEC  (SONIC, sonic);
    REGISTER_ENCODER (SONIC_LS, sonic_ls);
    REGISTER_DECODER (TRUEHD, truehd);
    REGISTER_DECODER (TRUESPEECH, truespeech);
    REGISTER_DECODER (TTA, tta);
    REGISTER_DECODER (TWINVQ, twinvq);
    REGISTER_DECODER (VMDAUDIO, vmdaudio);
    REGISTER_ENCDEC  (VORBIS, vorbis);
    REGISTER_DECODER (WAVPACK, wavpack);
    REGISTER_DECODER (WMALOSSLESS, wmalossless);
    REGISTER_DECODER (WMAPRO, wmapro);
    REGISTER_ENCDEC  (WMAV1, wmav1);
    REGISTER_ENCDEC  (WMAV2, wmav2);
    REGISTER_DECODER (WMAVOICE, wmavoice);
    REGISTER_DECODER (WS_SND1, ws_snd1);

    /* PCM codecs */
    REGISTER_ENCDEC  (PCM_ALAW, pcm_alaw);
    REGISTER_DECODER (PCM_BLURAY, pcm_bluray);
    REGISTER_DECODER (PCM_DVD, pcm_dvd);
    REGISTER_ENCDEC  (PCM_F32BE, pcm_f32be);
    REGISTER_ENCDEC  (PCM_F32LE, pcm_f32le);
    REGISTER_ENCDEC  (PCM_F64BE, pcm_f64be);
    REGISTER_ENCDEC  (PCM_F64LE, pcm_f64le);
    REGISTER_DECODER (PCM_LXF, pcm_lxf);
    REGISTER_ENCDEC  (PCM_MULAW, pcm_mulaw);
    REGISTER_ENCDEC  (PCM_S8, pcm_s8);
    REGISTER_DECODER (PCM_S8_PLANAR, pcm_s8_planar);
    REGISTER_ENCDEC  (PCM_S16BE, pcm_s16be);
    REGISTER_ENCDEC  (PCM_S16LE, pcm_s16le);
    REGISTER_DECODER (PCM_S16LE_PLANAR, pcm_s16le_planar);
    REGISTER_ENCDEC  (PCM_S24BE, pcm_s24be);
    REGISTER_ENCDEC  (PCM_S24DAUD, pcm_s24daud);
    REGISTER_ENCDEC  (PCM_S24LE, pcm_s24le);
    REGISTER_ENCDEC  (PCM_S32BE, pcm_s32be);
    REGISTER_ENCDEC  (PCM_S32LE, pcm_s32le);
    REGISTER_ENCDEC  (PCM_U8, pcm_u8);
    REGISTER_ENCDEC  (PCM_U16BE, pcm_u16be);
    REGISTER_ENCDEC  (PCM_U16LE, pcm_u16le);
    REGISTER_ENCDEC  (PCM_U24BE, pcm_u24be);
    REGISTER_ENCDEC  (PCM_U24LE, pcm_u24le);
    REGISTER_ENCDEC  (PCM_U32BE, pcm_u32be);
    REGISTER_ENCDEC  (PCM_U32LE, pcm_u32le);
    REGISTER_DECODER (PCM_ZORK , pcm_zork);

    /* DPCM codecs */
    REGISTER_DECODER (INTERPLAY_DPCM, interplay_dpcm);
    REGISTER_ENCDEC  (ROQ_DPCM, roq_dpcm);
    REGISTER_DECODER (SOL_DPCM, sol_dpcm);
    REGISTER_DECODER (XAN_DPCM, xan_dpcm);

    /* ADPCM codecs */
    REGISTER_DECODER (ADPCM_4XM, adpcm_4xm);
    REGISTER_ENCDEC  (ADPCM_ADX, adpcm_adx);
    REGISTER_DECODER (ADPCM_CT, adpcm_ct);
    REGISTER_DECODER (ADPCM_EA, adpcm_ea);
    REGISTER_DECODER (ADPCM_EA_MAXIS_XA, adpcm_ea_maxis_xa);
    REGISTER_DECODER (ADPCM_EA_R1, adpcm_ea_r1);
    REGISTER_DECODER (ADPCM_EA_R2, adpcm_ea_r2);
    REGISTER_DECODER (ADPCM_EA_R3, adpcm_ea_r3);
    REGISTER_DECODER (ADPCM_EA_XAS, adpcm_ea_xas);
    REGISTER_ENCDEC  (ADPCM_G722, adpcm_g722);
    REGISTER_ENCDEC  (ADPCM_G726, adpcm_g726);
    REGISTER_DECODER (ADPCM_IMA_AMV, adpcm_ima_amv);
    REGISTER_DECODER (ADPCM_IMA_APC, adpcm_ima_apc);
    REGISTER_DECODER (ADPCM_IMA_DK3, adpcm_ima_dk3);
    REGISTER_DECODER (ADPCM_IMA_DK4, adpcm_ima_dk4);
    REGISTER_DECODER (ADPCM_IMA_EA_EACS, adpcm_ima_ea_eacs);
    REGISTER_DECODER (ADPCM_IMA_EA_SEAD, adpcm_ima_ea_sead);
    REGISTER_DECODER (ADPCM_IMA_ISS, adpcm_ima_iss);
    REGISTER_ENCDEC  (ADPCM_IMA_QT, adpcm_ima_qt);
    REGISTER_DECODER (ADPCM_IMA_SMJPEG, adpcm_ima_smjpeg);
    REGISTER_ENCDEC  (ADPCM_IMA_WAV, adpcm_ima_wav);
    REGISTER_DECODER (ADPCM_IMA_WS, adpcm_ima_ws);
    REGISTER_ENCDEC  (ADPCM_MS, adpcm_ms);
    REGISTER_DECODER (ADPCM_SBPRO_2, adpcm_sbpro_2);
    REGISTER_DECODER (ADPCM_SBPRO_3, adpcm_sbpro_3);
    REGISTER_DECODER (ADPCM_SBPRO_4, adpcm_sbpro_4);
    REGISTER_ENCDEC  (ADPCM_SWF, adpcm_swf);
    REGISTER_DECODER (ADPCM_THP, adpcm_thp);
    REGISTER_DECODER (ADPCM_THP_LE, adpcm_thp_le);
    REGISTER_DECODER (ADPCM_XA, adpcm_xa);
    REGISTER_ENCDEC  (ADPCM_YAMAHA, adpcm_yamaha);

    /* subtitles */
    REGISTER_ENCDEC  (ASS, ass);
    REGISTER_ENCDEC  (DVBSUB, dvbsub);
    REGISTER_ENCDEC  (DVDSUB, dvdsub);
    REGISTER_DECODER (JACOSUB, jacosub);
    REGISTER_DECODER (MICRODVD, microdvd);
    REGISTER_DECODER (PGSSUB, pgssub);
    REGISTER_ENCDEC  (SRT, srt);
    REGISTER_ENCDEC  (XSUB, xsub);

    /* external libraries */
    REGISTER_DECODER (LIBCELT, libcelt);
    REGISTER_ENCODER (LIBFAAC, libfaac);
    REGISTER_ENCDEC  (LIBGSM, libgsm);
    REGISTER_ENCDEC  (LIBGSM_MS, libgsm_ms);
    REGISTER_ENCODER (LIBMP3LAME, libmp3lame);
    REGISTER_ENCDEC  (LIBOPENCORE_AMRNB, libopencore_amrnb);
    REGISTER_DECODER (LIBOPENCORE_AMRWB, libopencore_amrwb);
    REGISTER_ENCDEC  (LIBOPENJPEG, libopenjpeg);
    REGISTER_ENCDEC  (LIBSCHROEDINGER, libschroedinger);
    REGISTER_ENCDEC  (LIBSPEEX, libspeex);
    REGISTER_DECODER (LIBSTAGEFRIGHT_H264, libstagefright_h264);
    REGISTER_ENCODER (LIBTHEORA, libtheora);
    REGISTER_ENCDEC  (LIBUTVIDEO, libutvideo);
    REGISTER_ENCODER (LIBVO_AACENC, libvo_aacenc);
    REGISTER_ENCODER (LIBVO_AMRWBENC, libvo_amrwbenc);
    REGISTER_ENCDEC  (LIBVORBIS, libvorbis);
    REGISTER_ENCDEC  (LIBVPX, libvpx);
    REGISTER_ENCODER (LIBX264, libx264);
    REGISTER_ENCODER (LIBX264RGB, libx264rgb);
    REGISTER_ENCODER (LIBXAVS, libxavs);
    REGISTER_ENCODER (LIBXVID, libxvid);
    REGISTER_ENCODER (LIBAACPLUS, libaacplus);

    /* text */
    REGISTER_DECODER (BINTEXT, bintext);
    REGISTER_DECODER (XBIN, xbin);
    REGISTER_DECODER (IDF, idf);

    /* parsers */
    REGISTER_PARSER  (AAC, aac);
    REGISTER_PARSER  (AAC_LATM, aac_latm);
    REGISTER_PARSER  (AC3, ac3);
    REGISTER_PARSER  (ADX, adx);
    REGISTER_PARSER  (CAVSVIDEO, cavsvideo);
    REGISTER_PARSER  (COOK, cook);
    REGISTER_PARSER  (DCA, dca);
    REGISTER_PARSER  (DIRAC, dirac);
    REGISTER_PARSER  (DNXHD, dnxhd);
    REGISTER_PARSER  (DVBSUB, dvbsub);
    REGISTER_PARSER  (DVDSUB, dvdsub);
    REGISTER_PARSER  (FLAC, flac);
    REGISTER_PARSER  (GSM, gsm);
    REGISTER_PARSER  (H261, h261);
    REGISTER_PARSER  (H263, h263);
    REGISTER_PARSER  (H264, h264);
    REGISTER_PARSER  (MJPEG, mjpeg);
    REGISTER_PARSER  (MLP, mlp);
    REGISTER_PARSER  (MPEG4VIDEO, mpeg4video);
    REGISTER_PARSER  (MPEGAUDIO, mpegaudio);
    REGISTER_PARSER  (MPEGVIDEO, mpegvideo);
    REGISTER_PARSER  (PNG, png);
    REGISTER_PARSER  (PNM, pnm);
    REGISTER_PARSER  (RV30, rv30);
    REGISTER_PARSER  (RV40, rv40);
    REGISTER_PARSER  (VC1, vc1);
    REGISTER_PARSER  (VORBIS, vorbis);
    REGISTER_PARSER  (VP3, vp3);
    REGISTER_PARSER  (VP8, vp8);

    /* bitstream filters */
    REGISTER_BSF     (AAC_ADTSTOASC, aac_adtstoasc);
    REGISTER_BSF     (CHOMP, chomp);
    REGISTER_BSF     (DUMP_EXTRADATA, dump_extradata);
    REGISTER_BSF     (H264_MP4TOANNEXB, h264_mp4toannexb);
    REGISTER_BSF     (IMX_DUMP_HEADER, imx_dump_header);
    REGISTER_BSF     (MJPEG2JPEG, mjpeg2jpeg);
    REGISTER_BSF     (MJPEGA_DUMP_HEADER, mjpega_dump_header);
    REGISTER_BSF     (MP3_HEADER_COMPRESS, mp3_header_compress);
    REGISTER_BSF     (MP3_HEADER_DECOMPRESS, mp3_header_decompress);
    REGISTER_BSF     (MOV2TEXTSUB, mov2textsub);
    REGISTER_BSF     (NOISE, noise);
    REGISTER_BSF     (REMOVE_EXTRADATA, remove_extradata);
    REGISTER_BSF     (TEXT2MOVSUB, text2movsub);
}
