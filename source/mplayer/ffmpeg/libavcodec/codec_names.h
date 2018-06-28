case CODEC_ID_NONE:
    return "none";
case CODEC_ID_MPEG1VIDEO:
    { extern AVCodec ff_mpeg1video_decoder;
      return ff_mpeg1video_decoder.name; }
case CODEC_ID_MPEG2VIDEO:
    { extern AVCodec ff_mpeg2video_decoder;
      return ff_mpeg2video_decoder.name; }
case CODEC_ID_MPEG2VIDEO_XVMC:
    return "mpeg2video_xvmc";
case CODEC_ID_H261:
    { extern AVCodec ff_h261_decoder;
      return ff_h261_decoder.name; }
case CODEC_ID_H263:
    { extern AVCodec ff_h263_decoder;
      return ff_h263_decoder.name; }
case CODEC_ID_MJPEG:
    { extern AVCodec ff_mjpeg_decoder;
      return ff_mjpeg_decoder.name; }
case CODEC_ID_MJPEGB:
    { extern AVCodec ff_mjpegb_decoder;
      return ff_mjpegb_decoder.name; }
//case CODEC_ID_LJPEG:
//    { extern AVCodec ff_ljpeg_encoder;
//      return ff_ljpeg_encoder.name; }
case CODEC_ID_SP5X:
    { extern AVCodec ff_sp5x_decoder;
      return ff_sp5x_decoder.name; }
case CODEC_ID_JPEGLS:
    { extern AVCodec ff_jpegls_decoder;
      return ff_jpegls_decoder.name; }
case CODEC_ID_MPEG4:
    { extern AVCodec ff_mpeg4_decoder;
      return ff_mpeg4_decoder.name; }
case CODEC_ID_RAWVIDEO:
    { extern AVCodec ff_rawvideo_decoder;
      return ff_rawvideo_decoder.name; }
case CODEC_ID_MSMPEG4V1:
    { extern AVCodec ff_msmpeg4v1_decoder;
      return ff_msmpeg4v1_decoder.name; }
case CODEC_ID_MSMPEG4V2:
    { extern AVCodec ff_msmpeg4v2_decoder;
      return ff_msmpeg4v2_decoder.name; }
case CODEC_ID_MSMPEG4V3:
    { extern AVCodec ff_msmpeg4v3_decoder;
      return ff_msmpeg4v3_decoder.name; }
case CODEC_ID_WMV1:
    { extern AVCodec ff_wmv1_decoder;
      return ff_wmv1_decoder.name; }
case CODEC_ID_WMV2:
    { extern AVCodec ff_wmv2_decoder;
      return ff_wmv2_decoder.name; }
//case CODEC_ID_H263P:
//    { extern AVCodec ff_h263p_encoder;
//      return ff_h263p_encoder.name; }
case CODEC_ID_H263I:
    { extern AVCodec ff_h263i_decoder;
      return ff_h263i_decoder.name; }
case CODEC_ID_FLV1:
    return "flv1";
case CODEC_ID_HUFFYUV:
    { extern AVCodec ff_huffyuv_decoder;
      return ff_huffyuv_decoder.name; }
case CODEC_ID_H264:
    { extern AVCodec ff_h264_decoder;
      return ff_h264_decoder.name; }
case CODEC_ID_VP3:
    { extern AVCodec ff_vp3_decoder;
      return ff_vp3_decoder.name; }
case CODEC_ID_THEORA:
    { extern AVCodec ff_theora_decoder;
      return ff_theora_decoder.name; }
case CODEC_ID_4XM:
    return "4xm";
//case CODEC_ID_XAN_WC3:
//    { extern AVCodec ff_xan_wc3_decoder;
//      return ff_xan_wc3_decoder.name; }
//case CODEC_ID_XAN_WC4:
//    { extern AVCodec ff_xan_wc4_decoder;
//      return ff_xan_wc4_decoder.name; }
case CODEC_ID_8BPS:
    return "8bps";
case CODEC_ID_VMDVIDEO:
    { extern AVCodec ff_vmdvideo_decoder;
      return ff_vmdvideo_decoder.name; }
case CODEC_ID_ZLIB:
    return "zlib";
case CODEC_ID_TSCC:
    return "tscc";
case CODEC_ID_VIXL:
    return "vixl";
case CODEC_ID_PNG:
    return "png";
case CODEC_ID_FFVHUFF:
    { extern AVCodec ff_ffvhuff_decoder;
      return ff_ffvhuff_decoder.name; }
case CODEC_ID_VC1:
    { extern AVCodec ff_vc1_decoder;
      return ff_vc1_decoder.name; }
case CODEC_ID_WMV3:
    { extern AVCodec ff_wmv3_decoder;
      return ff_wmv3_decoder.name; }
case CODEC_ID_WNV1:
    { extern AVCodec ff_wnv1_decoder;
      return ff_wnv1_decoder.name; }
case CODEC_ID_AVS:
    { extern AVCodec ff_avs_decoder;
      return ff_avs_decoder.name; }
case CODEC_ID_FLASHSV:
    return "flashsv";
case CODEC_ID_JPEG2000:
    { extern AVCodec ff_jpeg2000_decoder;
      return ff_jpeg2000_decoder.name; }
case CODEC_ID_VP5:
    { extern AVCodec ff_vp5_decoder;
      return ff_vp5_decoder.name; }
case CODEC_ID_VP6:
    { extern AVCodec ff_vp6_decoder;
      return ff_vp6_decoder.name; }
case CODEC_ID_VP6F:
    { extern AVCodec ff_vp6f_decoder;
      return ff_vp6f_decoder.name; }
case CODEC_ID_DSICINVIDEO:
    { extern AVCodec ff_dsicinvideo_decoder;
      return ff_dsicinvideo_decoder.name; }
case CODEC_ID_DXA:
    return "dxa";
case CODEC_ID_THP:
    { extern AVCodec ff_thp_decoder;
      return ff_thp_decoder.name; }
case CODEC_ID_VP6A:
    { extern AVCodec ff_vp6a_decoder;
      return ff_vp6a_decoder.name; }
case CODEC_ID_AMV:
    { extern AVCodec ff_amv_decoder;
      return ff_amv_decoder.name; }
//case CODEC_ID_DIRAC:
//    { extern AVCodec ff_dirac_decoder;
//      return ff_dirac_decoder.name; }
case CODEC_ID_CMV:
    return "cmv";
case CODEC_ID_MAD:
    return "mad";
case CODEC_ID_FLASHSV2:
    return "flashsv2";
case CODEC_ID_IFF_ILBM:
    { extern AVCodec ff_iff_ilbm_decoder;
      return ff_iff_ilbm_decoder.name; }
case CODEC_ID_IFF_BYTERUN1:
    { extern AVCodec ff_iff_byterun1_decoder;
      return ff_iff_byterun1_decoder.name; }
case CODEC_ID_VP8:
    { extern AVCodec ff_vp8_decoder;
      return ff_vp8_decoder.name; }
case CODEC_ID_A64_MULTI:
    return "a64_multi";
case CODEC_ID_A64_MULTI5:
    return "a64_multi5";
//case CODEC_ID_PRORES:
//    { extern AVCodec ff_prores_decoder;
//      return ff_prores_decoder.name; }
//case CODEC_ID_WMV3IMAGE:
//    { extern AVCodec ff_wmv3image_decoder;
//      return ff_wmv3image_decoder.name; }
//case CODEC_ID_VC1IMAGE:
//    { extern AVCodec ff_vc1image_decoder;
//      return ff_vc1image_decoder.name; }
//case CODEC_ID_UTVIDEO:
//    { extern AVCodec ff_utvideo_decoder;
//      return ff_utvideo_decoder.name; }
//case CODEC_ID_BMV_VIDEO:
//    { extern AVCodec ff_bmv_video_decoder;
//      return ff_bmv_video_decoder.name; }
//case CODEC_ID_VBLE:
//    { extern AVCodec ff_vble_decoder;
//      return ff_vble_decoder.name; }
//case CODEC_ID_DXTORY:
//    { extern AVCodec ff_dxtory_decoder;
//      return ff_dxtory_decoder.name; }
//case CODEC_ID_V410:
//    { extern AVCodec ff_v410_decoder;
//      return ff_v410_decoder.name; }
//case CODEC_ID_XWD:
//    { extern AVCodec ff_xwd_decoder;
//      return ff_xwd_decoder.name; }
//case CODEC_ID_Y41P:
//    { extern AVCodec ff_y41p_decoder;
//      return ff_y41p_decoder.name; }
//case CODEC_ID_ESCAPE130:
//    { extern AVCodec ff_escape130_decoder;
//      return ff_escape130_decoder.name; }
//case CODEC_ID_AVRP:
//    { extern AVCodec ff_avrp_decoder;
//      return ff_avrp_decoder.name; }
case CODEC_ID_G2M:
    return "g2m";
//case CODEC_ID_AYUV:
//    { extern AVCodec ff_ayuv_decoder;
//      return ff_ayuv_decoder.name; }
//case CODEC_ID_V308:
//    { extern AVCodec ff_v308_decoder;
//      return ff_v308_decoder.name; }
//case CODEC_ID_V408:
//    { extern AVCodec ff_v408_decoder;
//      return ff_v408_decoder.name; }
//case CODEC_ID_YUV4:
//    { extern AVCodec ff_yuv4_decoder;
//      return ff_yuv4_decoder.name; }
case CODEC_ID_PCM_S16LE:
    { extern AVCodec ff_pcm_s16le_decoder;
      return ff_pcm_s16le_decoder.name; }
case CODEC_ID_PCM_S16BE:
    { extern AVCodec ff_pcm_s16be_decoder;
      return ff_pcm_s16be_decoder.name; }
case CODEC_ID_PCM_U16LE:
    { extern AVCodec ff_pcm_u16le_decoder;
      return ff_pcm_u16le_decoder.name; }
case CODEC_ID_PCM_U16BE:
    { extern AVCodec ff_pcm_u16be_decoder;
      return ff_pcm_u16be_decoder.name; }
case CODEC_ID_PCM_S8:
    { extern AVCodec ff_pcm_s8_decoder;
      return ff_pcm_s8_decoder.name; }
case CODEC_ID_PCM_U8:
    { extern AVCodec ff_pcm_u8_decoder;
      return ff_pcm_u8_decoder.name; }
case CODEC_ID_PCM_MULAW:
    { extern AVCodec ff_pcm_mulaw_decoder;
      return ff_pcm_mulaw_decoder.name; }
case CODEC_ID_PCM_ALAW:
    { extern AVCodec ff_pcm_alaw_decoder;
      return ff_pcm_alaw_decoder.name; }
case CODEC_ID_PCM_S32LE:
    { extern AVCodec ff_pcm_s32le_decoder;
      return ff_pcm_s32le_decoder.name; }
case CODEC_ID_PCM_S32BE:
    { extern AVCodec ff_pcm_s32be_decoder;
      return ff_pcm_s32be_decoder.name; }
case CODEC_ID_PCM_U32LE:
    { extern AVCodec ff_pcm_u32le_decoder;
      return ff_pcm_u32le_decoder.name; }
case CODEC_ID_PCM_U32BE:
    { extern AVCodec ff_pcm_u32be_decoder;
      return ff_pcm_u32be_decoder.name; }
case CODEC_ID_PCM_S24LE:
    { extern AVCodec ff_pcm_s24le_decoder;
      return ff_pcm_s24le_decoder.name; }
case CODEC_ID_PCM_S24BE:
    { extern AVCodec ff_pcm_s24be_decoder;
      return ff_pcm_s24be_decoder.name; }
case CODEC_ID_PCM_U24LE:
    { extern AVCodec ff_pcm_u24le_decoder;
      return ff_pcm_u24le_decoder.name; }
case CODEC_ID_PCM_U24BE:
    { extern AVCodec ff_pcm_u24be_decoder;
      return ff_pcm_u24be_decoder.name; }
case CODEC_ID_PCM_S24DAUD:
    { extern AVCodec ff_pcm_s24daud_decoder;
      return ff_pcm_s24daud_decoder.name; }
case CODEC_ID_PCM_ZORK:
    { extern AVCodec ff_pcm_zork_decoder;
      return ff_pcm_zork_decoder.name; }
case CODEC_ID_PCM_S16LE_PLANAR:
    { extern AVCodec ff_pcm_s16le_planar_decoder;
      return ff_pcm_s16le_planar_decoder.name; }
case CODEC_ID_PCM_DVD:
    { extern AVCodec ff_pcm_dvd_decoder;
      return ff_pcm_dvd_decoder.name; }
case CODEC_ID_PCM_F32BE:
    { extern AVCodec ff_pcm_f32be_decoder;
      return ff_pcm_f32be_decoder.name; }
case CODEC_ID_PCM_F32LE:
    { extern AVCodec ff_pcm_f32le_decoder;
      return ff_pcm_f32le_decoder.name; }
case CODEC_ID_PCM_F64BE:
    { extern AVCodec ff_pcm_f64be_decoder;
      return ff_pcm_f64be_decoder.name; }
case CODEC_ID_PCM_F64LE:
    { extern AVCodec ff_pcm_f64le_decoder;
      return ff_pcm_f64le_decoder.name; }
case CODEC_ID_PCM_BLURAY:
    { extern AVCodec ff_pcm_bluray_decoder;
      return ff_pcm_bluray_decoder.name; }
case CODEC_ID_PCM_LXF:
    { extern AVCodec ff_pcm_lxf_decoder;
      return ff_pcm_lxf_decoder.name; }
case CODEC_ID_PCM_S8_PLANAR:
    { extern AVCodec ff_pcm_s8_planar_decoder;
      return ff_pcm_s8_planar_decoder.name; }
case CODEC_ID_ADPCM_IMA_QT:
    { extern AVCodec ff_adpcm_ima_qt_decoder;
      return ff_adpcm_ima_qt_decoder.name; }
case CODEC_ID_ADPCM_IMA_WAV:
    { extern AVCodec ff_adpcm_ima_wav_decoder;
      return ff_adpcm_ima_wav_decoder.name; }
case CODEC_ID_ADPCM_IMA_DK3:
    { extern AVCodec ff_adpcm_ima_dk3_decoder;
      return ff_adpcm_ima_dk3_decoder.name; }
case CODEC_ID_ADPCM_IMA_DK4:
    { extern AVCodec ff_adpcm_ima_dk4_decoder;
      return ff_adpcm_ima_dk4_decoder.name; }
case CODEC_ID_ADPCM_IMA_WS:
    { extern AVCodec ff_adpcm_ima_ws_decoder;
      return ff_adpcm_ima_ws_decoder.name; }
case CODEC_ID_ADPCM_IMA_SMJPEG:
    { extern AVCodec ff_adpcm_ima_smjpeg_decoder;
      return ff_adpcm_ima_smjpeg_decoder.name; }
case CODEC_ID_ADPCM_MS:
    { extern AVCodec ff_adpcm_ms_decoder;
      return ff_adpcm_ms_decoder.name; }
case CODEC_ID_ADPCM_4XM:
    { extern AVCodec ff_adpcm_4xm_decoder;
      return ff_adpcm_4xm_decoder.name; }
case CODEC_ID_ADPCM_XA:
    { extern AVCodec ff_adpcm_xa_decoder;
      return ff_adpcm_xa_decoder.name; }
case CODEC_ID_ADPCM_ADX:
    { extern AVCodec ff_adpcm_adx_decoder;
      return ff_adpcm_adx_decoder.name; }
case CODEC_ID_ADPCM_EA:
    { extern AVCodec ff_adpcm_ea_decoder;
      return ff_adpcm_ea_decoder.name; }
case CODEC_ID_ADPCM_G726:
    { extern AVCodec ff_adpcm_g726_decoder;
      return ff_adpcm_g726_decoder.name; }
case CODEC_ID_ADPCM_CT:
    { extern AVCodec ff_adpcm_ct_decoder;
      return ff_adpcm_ct_decoder.name; }
case CODEC_ID_ADPCM_SWF:
    { extern AVCodec ff_adpcm_swf_decoder;
      return ff_adpcm_swf_decoder.name; }
case CODEC_ID_ADPCM_YAMAHA:
    { extern AVCodec ff_adpcm_yamaha_decoder;
      return ff_adpcm_yamaha_decoder.name; }
case CODEC_ID_ADPCM_SBPRO_4:
    { extern AVCodec ff_adpcm_sbpro_4_decoder;
      return ff_adpcm_sbpro_4_decoder.name; }
case CODEC_ID_ADPCM_SBPRO_3:
    { extern AVCodec ff_adpcm_sbpro_3_decoder;
      return ff_adpcm_sbpro_3_decoder.name; }
case CODEC_ID_ADPCM_SBPRO_2:
    { extern AVCodec ff_adpcm_sbpro_2_decoder;
      return ff_adpcm_sbpro_2_decoder.name; }
case CODEC_ID_ADPCM_THP:
    { extern AVCodec ff_adpcm_thp_decoder;
      return ff_adpcm_thp_decoder.name; }
case CODEC_ID_ADPCM_IMA_AMV:
    { extern AVCodec ff_adpcm_ima_amv_decoder;
      return ff_adpcm_ima_amv_decoder.name; }
case CODEC_ID_ADPCM_EA_R1:
    { extern AVCodec ff_adpcm_ea_r1_decoder;
      return ff_adpcm_ea_r1_decoder.name; }
case CODEC_ID_ADPCM_EA_R3:
    { extern AVCodec ff_adpcm_ea_r3_decoder;
      return ff_adpcm_ea_r3_decoder.name; }
case CODEC_ID_ADPCM_EA_R2:
    { extern AVCodec ff_adpcm_ea_r2_decoder;
      return ff_adpcm_ea_r2_decoder.name; }
case CODEC_ID_ADPCM_IMA_EA_SEAD:
    { extern AVCodec ff_adpcm_ima_ea_sead_decoder;
      return ff_adpcm_ima_ea_sead_decoder.name; }
case CODEC_ID_ADPCM_IMA_EA_EACS:
    { extern AVCodec ff_adpcm_ima_ea_eacs_decoder;
      return ff_adpcm_ima_ea_eacs_decoder.name; }
case CODEC_ID_ADPCM_EA_XAS:
    { extern AVCodec ff_adpcm_ea_xas_decoder;
      return ff_adpcm_ea_xas_decoder.name; }
case CODEC_ID_ADPCM_EA_MAXIS_XA:
    { extern AVCodec ff_adpcm_ea_maxis_xa_decoder;
      return ff_adpcm_ea_maxis_xa_decoder.name; }
case CODEC_ID_ADPCM_IMA_ISS:
    { extern AVCodec ff_adpcm_ima_iss_decoder;
      return ff_adpcm_ima_iss_decoder.name; }
case CODEC_ID_ADPCM_G722:
    { extern AVCodec ff_adpcm_g722_decoder;
      return ff_adpcm_g722_decoder.name; }
case CODEC_ID_ADPCM_IMA_APC:
    { extern AVCodec ff_adpcm_ima_apc_decoder;
      return ff_adpcm_ima_apc_decoder.name; }
case CODEC_ID_AMR_NB:
    return "amr_nb";
case CODEC_ID_AMR_WB:
    return "amr_wb";
case CODEC_ID_XAN_DPCM:
    { extern AVCodec ff_xan_dpcm_decoder;
      return ff_xan_dpcm_decoder.name; }
case CODEC_ID_SOL_DPCM:
    { extern AVCodec ff_sol_dpcm_decoder;
      return ff_sol_dpcm_decoder.name; }
case CODEC_ID_MP2:
    { extern AVCodec ff_mp2_decoder;
      return ff_mp2_decoder.name; }
case CODEC_ID_MP3:
    { extern AVCodec ff_mp3_decoder;
      return ff_mp3_decoder.name; }
case CODEC_ID_AAC:
    { extern AVCodec ff_aac_decoder;
      return ff_aac_decoder.name; }
case CODEC_ID_AC3:
    { extern AVCodec ff_ac3_decoder;
      return ff_ac3_decoder.name; }
case CODEC_ID_DTS:
    return "dts";
/*case CODEC_ID_VORBIS:
    { extern AVCodec ff_vorbis_decoder;
      return ff_vorbis_decoder.name; }*/
case CODEC_ID_DVAUDIO:
    return "dvaudio";
case CODEC_ID_WMAV1:
    { extern AVCodec ff_wmav1_decoder;
      return ff_wmav1_decoder.name; }
case CODEC_ID_WMAV2:
    { extern AVCodec ff_wmav2_decoder;
      return ff_wmav2_decoder.name; }
case CODEC_ID_MACE3:
    { extern AVCodec ff_mace3_decoder;
      return ff_mace3_decoder.name; }
case CODEC_ID_MACE6:
    { extern AVCodec ff_mace6_decoder;
      return ff_mace6_decoder.name; }
case CODEC_ID_VMDAUDIO:
    { extern AVCodec ff_vmdaudio_decoder;
      return ff_vmdaudio_decoder.name; }
case CODEC_ID_FLAC:
    { extern AVCodec ff_flac_decoder;
      return ff_flac_decoder.name; }
case CODEC_ID_MP3ADU:
    { extern AVCodec ff_mp3adu_decoder;
      return ff_mp3adu_decoder.name; }
case CODEC_ID_MP3ON4:
    { extern AVCodec ff_mp3on4_decoder;
      return ff_mp3on4_decoder.name; }
case CODEC_ID_ALAC:
    { extern AVCodec ff_alac_decoder;
      return ff_alac_decoder.name; }
case CODEC_ID_WESTWOOD_SND1:
    return "westwood_snd1";
case CODEC_ID_GSM:
    { extern AVCodec ff_gsm_decoder;
      return ff_gsm_decoder.name; }
case CODEC_ID_TRUESPEECH:
    { extern AVCodec ff_truespeech_decoder;
      return ff_truespeech_decoder.name; }
case CODEC_ID_TTA:
    { extern AVCodec ff_tta_decoder;
      return ff_tta_decoder.name; }
case CODEC_ID_WAVPACK:
    { extern AVCodec ff_wavpack_decoder;
      return ff_wavpack_decoder.name; }
case CODEC_ID_DSICINAUDIO:
    { extern AVCodec ff_dsicinaudio_decoder;
      return ff_dsicinaudio_decoder.name; }
case CODEC_ID_IMC:
    { extern AVCodec ff_imc_decoder;
      return ff_imc_decoder.name; }
case CODEC_ID_MUSEPACK7:
    return "musepack7";
case CODEC_ID_MLP:
    { extern AVCodec ff_mlp_decoder;
      return ff_mlp_decoder.name; }
case CODEC_ID_GSM_MS:
    { extern AVCodec ff_gsm_ms_decoder;
      return ff_gsm_ms_decoder.name; }
case CODEC_ID_ATRAC3:
    { extern AVCodec ff_atrac3_decoder;
      return ff_atrac3_decoder.name; }
case CODEC_ID_VOXWARE:
    return "voxware";
case CODEC_ID_APE:
    { extern AVCodec ff_ape_decoder;
      return ff_ape_decoder.name; }
case CODEC_ID_MUSEPACK8:
    return "musepack8";
case CODEC_ID_SPEEX:
    return "speex";
case CODEC_ID_WMAVOICE:
    { extern AVCodec ff_wmavoice_decoder;
      return ff_wmavoice_decoder.name; }
case CODEC_ID_WMAPRO:
    { extern AVCodec ff_wmapro_decoder;
      return ff_wmapro_decoder.name; }
//case CODEC_ID_WMALOSSLESS:
//    { extern AVCodec ff_wmalossless_decoder;
//      return ff_wmalossless_decoder.name; }
case CODEC_ID_ATRAC3P:
    return "atrac3p";
case CODEC_ID_EAC3:
    { extern AVCodec ff_eac3_decoder;
      return ff_eac3_decoder.name; }
case CODEC_ID_SIPR:
    { extern AVCodec ff_sipr_decoder;
      return ff_sipr_decoder.name; }
case CODEC_ID_MP1:
    { extern AVCodec ff_mp1_decoder;
      return ff_mp1_decoder.name; }
case CODEC_ID_TWINVQ:
    { extern AVCodec ff_twinvq_decoder;
      return ff_twinvq_decoder.name; }
case CODEC_ID_TRUEHD:
    { extern AVCodec ff_truehd_decoder;
      return ff_truehd_decoder.name; }
case CODEC_ID_MP4ALS:
    return "mp4als";
case CODEC_ID_ATRAC1:
    { extern AVCodec ff_atrac1_decoder;
      return ff_atrac1_decoder.name; }
case CODEC_ID_AAC_LATM:
    { extern AVCodec ff_aac_latm_decoder;
      return ff_aac_latm_decoder.name; }
case CODEC_ID_QDMC:
    return "qdmc";
case CODEC_ID_CELT:
    return "celt";
//case CODEC_ID_G723_1:
//    { extern AVCodec ff_g723_1_decoder;
//      return ff_g723_1_decoder.name; }
//case CODEC_ID_G729:
//    { extern AVCodec ff_g729_decoder;
//      return ff_g729_decoder.name; }
//case CODEC_ID_8SVX_EXP:
//    return "8svx_exp";
//case CODEC_ID_8SVX_FIB:
//    return "8svx_fib";
//case CODEC_ID_BMV_AUDIO:
//    { extern AVCodec ff_bmv_audio_decoder;
//      return ff_bmv_audio_decoder.name; }
//case CODEC_ID_FFWAVESYNTH:
//    { extern AVCodec ff_ffwavesynth_decoder;
//      return ff_ffwavesynth_decoder.name; }
case CODEC_ID_8SVX_RAW:
    return "8svx_raw";
case CODEC_ID_SONIC:
    { extern AVCodec ff_sonic_decoder;
      return ff_sonic_decoder.name; }
//case CODEC_ID_SONIC_LS:
//    { extern AVCodec ff_sonic_ls_encoder;
//      return ff_sonic_ls_encoder.name; }
case CODEC_ID_DVD_SUBTITLE:
    return "dvd_subtitle";
case CODEC_ID_DVB_SUBTITLE:
    return "dvb_subtitle";
case CODEC_ID_TEXT:
    return "text";
case CODEC_ID_XSUB:
    { extern AVCodec ff_xsub_decoder;
      return ff_xsub_decoder.name; }
case CODEC_ID_SSA:
    return "ssa";
case CODEC_ID_MOV_TEXT:
    return "mov_text";
case CODEC_ID_HDMV_PGS_SUBTITLE:
    return "hdmv_pgs_subtitle";
case CODEC_ID_DVB_TELETEXT:
    return "dvb_teletext";
case CODEC_ID_SRT:
    { extern AVCodec ff_srt_decoder;
      return ff_srt_decoder.name; }
case CODEC_ID_MICRODVD:
    return "microdvd";
case CODEC_ID_TTF:
    return "ttf";
//case CODEC_ID_BINTEXT:
//    { extern AVCodec ff_bintext_decoder;
//      return ff_bintext_decoder.name; }
//case CODEC_ID_XBIN:
//    { extern AVCodec ff_xbin_decoder;
//      return ff_xbin_decoder.name; }
//case CODEC_ID_IDF:
//    { extern AVCodec ff_idf_decoder;
//      return ff_idf_decoder.name; }
case CODEC_ID_PROBE:
    return "probe";
case CODEC_ID_MPEG2TS:
    return "mpeg2ts";
case CODEC_ID_MPEG4SYSTEMS:
    return "mpeg4systems";
case CODEC_ID_FFMETADATA:
    return "ffmetadata";

