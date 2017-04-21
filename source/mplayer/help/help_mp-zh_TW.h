// Synced with help_mp-en.h r21655
// Translated by Kenneth Chan <chantk@ctk.sytes.net>
// With reference from help_mp-zh.h
// Synced by Lu Ran <hephooey@fastmail.fm>
// *** Right now, this file is convert from help_mp-zh_CN.h using following perl script:
// http://www.annocpan.org/~DIVEC/Lingua-ZH-HanConvert-0.12/HanConvert.pm
// Converted by Eric Wang <eric7wang gmail com>

// ========================= MPlayer help ===========================

static const char help_text[]=
"用法:   mplayer [�?�項] [URL|路徑/]文件�??\n"
"\n"
"基本�?�項: (完整列表�?�見手册�?)\n"
" -vo <drv>        �?�擇視頻輸出驅動 (查看驅動列表用“-vo help�?)\n"
" -ao <drv>        �?�擇音頻輸出驅動 (查看驅動列表用“-ao help�?)\n"
#ifdef CONFIG_VCD
" vcd://<trackno>  播放 (S)VCD 軌迹號 (原始設備, 無需安挂)\n"
#endif
#ifdef CONFIG_DVDREAD
" dvd://<titleno>  從設備而�?是普通文件上播放 DVD 標題號\n"
" -alang/-slang    �?�擇 DVD 音軌/字幕的語言(使用兩字符的國家代號)\n"
#endif
" -ss <position>   尋找到給定(多少秒或時分秒 hh:mm:ss 的)�?置\n"
" -nosound         �?播放�?�音\n"
" -fs              全�?播放 (或者用 -vm, -zoom, 詳見于手册�?)\n"
" -x <x> -y <y>    設置顯示的分辨率(�??供給 -vm 或者 -zoom 使用)\n"
" -sub <file>      指定字幕文件 (�?�見 -subfps, -subdelay)\n"
" -playlist <file> 指定播放列表文件\n"
" -vid x -aid y    �?�擇用于播放的 x 視頻�?和 y 音頻�?通�?�號\n"
" -fps x -srate y  改變視頻率為 x 幀秒(fps)和音頻率為 y 赫兹(Hz)\n"
" -pp <quality>    使用後期處�?��?�濾器/濾�?� (詳見于手册�?)\n"
" -framedrop       使用丢幀(用于慢機器)\n"
"\n"
"基本控製�?�: (完整的列表�?�見于手册�?, �?�時也請核查 input.conf)\n"
" <-  or  ->       後退/快進 10 秒\n"
" down or up       後退/快進 1 分�?�\n"
" pgdown or pgup   後退/快進 10 分�?�\n"
" < or >           跳到播放列表中的�?一個/後一個\n"
" p or SPACE       暫�?�播放(按任�?�?�繼續)\n"
" q or ESC         �?�止播放并退出程�?\n"
" + or -           調整音頻延�?�增加/�?少 0.1 秒\n"
" o                循環 OSD 模�?:  無/�?�索�?/�?�索�?加計時器\n"
" * or /           增加或�?少 PCM 音�?\n"
" x or z           調整字幕延�?�增加/�?少 0.1 秒\n"
" r or t           上/下調整字幕�?置, �?�見“-vf expand�?\n"
"\n"
" * * *  詳細内容，更多的(高級)�?�項和控製�?�，請�?�見手册�?  * * *\n"
"\n";

// libmpcodecs/ad_dvdpcm.c:
#define MSGTR_SamplesWanted "這個格�?的采様需�?更好的支�?。請�?�係開發者。\n"

// ========================= MPlayer messages ===========================

// mplayer.c:

#define MSGTR_Exiting "\n正在退出..\n"
#define MSGTR_ExitingHow "\n正在退出... (%s)\n"
#define MSGTR_Exit_quit "退出"
#define MSGTR_Exit_eof "文件�?�?�"
#define MSGTR_Exit_error "致命錯誤"
#define MSGTR_IntBySignal "\nMPlayer 被 %d 信號中斷(屬于 %s 模塊)\n"
#define MSGTR_NoHomeDir "找�?到主(HOME)目録\n"
#define MSGTR_GetpathProblem "get_path(\"config\") �?題\n"
#define MSGTR_CreatingCfgFile "創建�?置文件: %s\n"
#define MSGTR_CantLoadFont "�?能加載�?圖字體: %s\n"
#define MSGTR_CantLoadSub "�?能加載字幕: %s\n"
#define MSGTR_DumpSelectedStreamMissing "轉儲: 致命錯誤: 指定的�?�?存在!\n"
#define MSGTR_CantOpenDumpfile "打�?開轉儲文件。\n"
#define MSGTR_CoreDumped "core dumped :)\n"
#define MSGTR_FPSnotspecified "FPS 在文件頭中没有指定或者無效，用 -fps �?�項。\n"
#define MSGTR_TryForceAudioFmtStr "嘗試鎖定為音頻編解碼器驅動�? %s...\n"
#define MSGTR_CantFindAudioCodec "找�?到音頻格�? 0x%X 的編解碼器。\n"
#define MSGTR_TryForceVideoFmtStr "嘗試鎖定為視頻編解碼器驅動�? %s...\n"
#define MSGTR_CantFindVideoCodec "找�?到匹�? -vo 所�?�的和視頻格�? 0x%X 的編解碼器。\n"
#define MSGTR_CannotInitVO "致命錯誤: 無法�?始化視頻驅動!\n"
#define MSGTR_CannotInitAO "�?能打開/�?始化音頻設備 -> 没�?�音。\n"
#define MSGTR_StartPlaying "開始播放...\n"

#define MSGTR_SystemTooSlow "\n\n"\
"         ************************************************\n"\
"         ****      你的係統太“慢�?了, 播放�?了!     ****\n"\
"         ************************************************\n"\
" �?�能的原因, �?題, 和解决辦法：\n"\
"- 最常見的原因：�??壞的或有錯誤的 _音頻_ 驅動\n"\
"  - 試試 -ao sdl 或使用 ALSA  的 OSS 模擬。\n"\
"  - 試驗�?�?�的 -autosync 的值，�?妨從 30 開始。\n"\
"- 視頻輸出太慢\n"\
"  - (�?�考 -vo help)試試 -vo 用�?�?�的驅動或者試試 -framedrop�?\n"\
"- CPU 太慢\n"\
"  - �?�?試圖在慢速 CPU 上播放大的 DVD/DivX! 試試一些 lavdopts �?�項,\n"\
"    例如: -vfm ffmpeg -lavdopts lowres=1:fast:skiploopfilter=all。\n"\
"- �??壞的文件\n"\
"  - 試試下列�?�項的�?�種組�?�: -nobps -ni -forceidx -mc 0。\n"\
"- 太慢的媒體(如 NFS/SMB 安挂點, DVD, VCD 等)\n"\
"  - 試試 -cache 8192。\n"\
"- 你在用 -cache �?�項播放一個�?�交錯的 AVI 文件？\n"\
"  - 試試 -nocache。\n"\
"閲讀 DOCS/zh/video.html 和 DOCS/zh/sound.html 來尋找調整/加速的技巧。\n"\
"如果這些一個都用�?上, 閲讀 DOCS/zh/bugreports.html�?\n\n"

#define MSGTR_NoGui "MPlayer 的編譯没有支�? GUI。\n"
#define MSGTR_GuiNeedsX "MPlayer GUI 需�? X11。\n"
#define MSGTR_Playing "\n正在播放 %s。\n"
#define MSGTR_NoSound "音頻: 没�?�音\n"
#define MSGTR_FPSforced "FPS 鎖定為 %5.3f  (ftime: %5.3f)。\n"
#define MSGTR_AvailableVideoOutputDrivers "�?�用的視頻輸出驅動:\n"
#define MSGTR_AvailableAudioOutputDrivers "�?�用的音頻輸出驅動:\n"
#define MSGTR_AvailableAudioCodecs "�?�用的音頻編解碼器:\n"
#define MSGTR_AvailableVideoCodecs "�?�用的視頻編解碼器:\n"
#define MSGTR_AvailableAudioFm "\n(已編譯進的)�?�用的音頻編解碼器�?/驅動:\n"
#define MSGTR_AvailableVideoFm "\n(已編譯進的)�?�用的視頻編解碼器�?/驅動:\n"
#define MSGTR_AvailableFsType "�?�用的全�?層變�?�模�?:\n"
#define MSGTR_CannotReadVideoProperties "視頻: 無法讀�?�屬性\n"
#define MSGTR_NoStreamFound "找�?到�?媒體。\n"
#define MSGTR_ErrorInitializingVODevice "打開/�?始化 (-vo) 所�?�的視頻輸出設備出錯。\n"
#define MSGTR_ForcedVideoCodec "鎖定的視頻編解碼器: %s\n"
#define MSGTR_ForcedAudioCodec "鎖定的音頻編解碼器: %s\n"
#define MSGTR_Video_NoVideo "視頻: 没視頻\n"
#define MSGTR_NotInitializeVOPorVO "\n致命錯誤: 無法�?始化 (-vf) 視頻�?�濾器或 (-vo) 視頻輸出。\n"
#define MSGTR_Paused "  =====  暫�?�  =====" // no more than 23 characters (status line for audio files)
#define MSGTR_PlaylistLoadUnable "\n無法�?載播放列表 %s\n"
#define MSGTR_Exit_SIGILL_RTCpuSel \
"- “�?�法指令�?導致 MPlayer 崩潰。\n"\
"  這�?�能是我們的新代碼中�?�行時 CPU-檢測的一個錯誤...\n"\
"  請閲讀 DOCS/zh/bugreports.html。\n"
#define MSGTR_Exit_SIGILL \
"- “�?�法指令�?導致 MPlayer 崩潰。\n"\
"  這通常發生在�?�在你所�?�行之上的 CPU �?�?�于\n"\
"  編譯/優化時的 CPU 所造�?的。\n"\
"  證實它�?\n"
#define MSGTR_Exit_SIGSEGV_SIGFPE \
"- �?�度使用 CPU/FPU/RAM 導致 MPlayer 崩潰。\n"\
"  使用 --enable-debug �?新編譯 MPlayer 并用調試程�?“gdb�?�??跟踪和\n"\
"  �??匯編。具體細節看 DOCS/zh/bugreports.html#crash。\n"
#define MSGTR_Exit_SIGCRASH \
"- MPlayer 崩潰了。這�?應該發生。\n"\
"  這�?�能是 MPlayer 代碼中 _或者_ 你的驅動中 _或者_ 你的 gcc 版本中的一個\n"\
"  錯誤。如你覺得這是 MPlayer 的錯誤，請閲讀 DOCS/zh/bugreports.html\n"\
"  并�?�循上�?�的步驟報告錯誤。除�?�你在報告一個�?�能的錯誤時候�??供我們\n"\
"  所需�?的信�?�, �?�則我們�?能也�?會幚助你。\n"
#define MSGTR_LoadingConfig "正在�?載�?置文件 '%s'\n"
#define MSGTR_AddedSubtitleFile "字幕: 添加字幕文件 (%d): %s\n"
#define MSGTR_RemovedSubtitleFile "字幕: 删除字幕文件 (%d): %s\n"
#define MSGTR_ErrorOpeningOutputFile "打開寫入文件 [%s] 失敗!\n"
#define MSGTR_RTCDeviceNotOpenable "打開 %s 失敗: %s (此文件應該能被用户讀�?�。)\n"
#define MSGTR_LinuxRTCInitErrorIrqpSet "Linux RTC �?始化錯誤在 ioctl (rtc_irqp_set %lu): %s\n"
#define MSGTR_IncreaseRTCMaxUserFreq "試圖添加 \"echo %lu > /proc/sys/dev/rtc/max-user-freq\" 到你的係統啟動脚本。\n"
#define MSGTR_LinuxRTCInitErrorPieOn "Linux RTC �?始化錯誤在 ioctl (rtc_pie_on): %s\n"
#define MSGTR_UsingTimingType "正在使用 %s 計時。\n"
#define MSGTR_Getch2InitializedTwice "警告: getch2_init 被調用兩次!\n"
#define MSGTR_DumpstreamFdUnavailable "無法轉儲此�? - 没有�?�用的文件�??述符。\n"
#define MSGTR_CantOpenLibmenuFilterWithThisRootMenu "�?能用根�?�單 %s 打開 libmenu 視頻�?�濾器。\n"
#define MSGTR_AudioFilterChainPreinitError "音頻�?�濾器�?��?啟動錯誤!\n"
#define MSGTR_LinuxRTCReadError "Linux RTC 讀�?�錯誤: %s\n"
#define MSGTR_SoftsleepUnderflow "警告! Softsleep 嚮下溢出!\n"
#define MSGTR_DvdnavNullEvent "DVDNAV 事件為空?!\n"
#define MSGTR_DvdnavHighlightEventBroken "DVDNAV 事件: 高亮事件�??壞\n"
#define MSGTR_DvdnavEvent "DVDNAV 事件: %s\n"
#define MSGTR_DvdnavHighlightHide "DVDNAV 事件: 高亮隱�?\n"
#define MSGTR_DvdnavStillFrame "######################################## DVDNAV 事件: �?�止幀: %d秒\n"
#define MSGTR_DvdnavNavStop "DVDNAV 事件: Nav�?�止\n"
#define MSGTR_DvdnavNavNOP "DVDNAV 事件: Nav無�?作\n"
#define MSGTR_DvdnavNavSpuStreamChangeVerbose "DVDNAV 事件: Nav SPU �?改變: 物�?�: %d/%d/%d �?輯: %d\n"
#define MSGTR_DvdnavNavSpuStreamChange "DVDNAV 事件: Nav SPU �?改變: 物�?�: %d �?輯: %d\n"
#define MSGTR_DvdnavNavAudioStreamChange "DVDNAV 事件: Nav 音頻�?改變: 物�?�: %d �?輯: %d\n"
#define MSGTR_DvdnavNavVTSChange "DVDNAV 事件: Nav VTS 改變\n"
#define MSGTR_DvdnavNavCellChange "DVDNAV 事件: Nav Cell 改變\n"
#define MSGTR_DvdnavNavSpuClutChange "DVDNAV 事件: Nav SPU CLUT 改變\n"
#define MSGTR_DvdnavNavSeekDone "DVDNAV 事件: Nav �?�尋完�?\n"
#define MSGTR_MenuCall "�?�單調用\n"

#define MSGTR_EdlOutOfMem "�?能分�?足够的内存來�?�? EDL 數據。\n"
#define MSGTR_EdlRecordsNo "讀�?� %d EDL 動作。\n"
#define MSGTR_EdlQueueEmpty "没有 EDL 動作�?處�?�。\n"
#define MSGTR_EdlCantOpenForWrite "打�?開 EDL 文件 [%s] 寫入。\n"
#define MSGTR_EdlCantOpenForRead "打�?開 EDL 文件 [%s] 讀�?�。\n"
#define MSGTR_EdlNOsh_video "没有視頻�?能使用 EDL, �?�消中。\n"
#define MSGTR_EdlNOValidLine "無效 EDL 綫: %s\n"
#define MSGTR_EdlBadlyFormattedLine "錯誤格�?的 EDL 綫 [%d], 丢棄。\n"
#define MSGTR_EdlBadLineOverlap "上次�?�止的�?置是 [%f]; 下次開始的�?置在 [%f]。\n"\
"�?一項必須按時間順�?, �?能�?疊。 丢棄。\n"
#define MSGTR_EdlBadLineBadStop "�?�止時間必須是在開始時間之後。\n"
#define MSGTR_EdloutBadStop "EDL 跳�?已�?�消, 上次開始�?置 > �?�止�?置\n"
#define MSGTR_EdloutStartSkip "EDL 跳�?開始, �?按�?� 'i' 以�?�止。\n"
#define MSGTR_EdloutEndSkip "EDL 跳�?�?�?�, 綫已寫入。\n"

// mplayer.c OSD

#define MSGTR_OSDenabled "已啟用"
#define MSGTR_OSDdisabled "已�?�用"
#define MSGTR_OSDAudio "音頻: %s"
#define MSGTR_OSDVideo "視頻: %s"
#define MSGTR_OSDChannel "頻�?�: %s"
#define MSGTR_OSDSubDelay "字幕延�?�: %d 毫秒"
#define MSGTR_OSDSpeed "速度: x %6.2f"
#define MSGTR_OSDosd "OSD: %s"
#define MSGTR_OSDChapter "章節: (%d) %s"

// property values
#define MSGTR_Enabled "已啟用"
#define MSGTR_EnabledEdl "已啟用 EDL"
#define MSGTR_Disabled "已�?�用"
#define MSGTR_HardFrameDrop "强丢幀"
#define MSGTR_Unknown "未知"
#define MSGTR_Bottom "底部"
#define MSGTR_Center "中部"
#define MSGTR_Top "頂部"

// osd bar names
#define MSGTR_Volume "音�?"
#define MSGTR_Panscan "摇移"
#define MSGTR_Gamma "Gamma"
#define MSGTR_Brightness "亮度"
#define MSGTR_Contrast "�?比度"
#define MSGTR_Saturation "飽和度"
#define MSGTR_Hue "色調"

// property state
#define MSGTR_MuteStatus "�?�音: %s"
#define MSGTR_AVDelayStatus "A-V 延�?�: %s"
#define MSGTR_OnTopStatus "總在最�?: %s"
#define MSGTR_RootwinStatus "根窗�?�: %s"
#define MSGTR_BorderStatus "邊框: %s"
#define MSGTR_FramedroppingStatus "丢幀: %s"
#define MSGTR_VSyncStatus "視頻�?�步: %s"
#define MSGTR_SubSelectStatus "字幕: %s"
#define MSGTR_SubPosStatus "字幕�?置: %s/100"
#define MSGTR_SubAlignStatus "字幕�?齊: %s"
#define MSGTR_SubDelayStatus "字幕延�?�: %s"
#define MSGTR_SubVisibleStatus "顯示字幕: %s"
#define MSGTR_SubForcedOnlyStatus "祇用鎖定的字幕: %s"

// mencoder.c:

#define MSGTR_UsingPass3ControlFile "使用 pass3 控製文件: %s\n"
#define MSGTR_MissingFilename "\n没有文件�??。\n\n"
#define MSGTR_CannotOpenFile_Device "打�?開文件/設備。\n"
#define MSGTR_CannotOpenDemuxer "打�?開分路器。\n"
#define MSGTR_NoAudioEncoderSelected "\n没有 (-oac) 所�?�的音頻編碼器。(�?�考 -oac help)�?�擇一個或者使用 -nosound。\n"
#define MSGTR_NoVideoEncoderSelected "\n没有 (-ovc) 所�?�的視頻解碼器。(�?�考 -ovc help)�?�擇一個。\n"
#define MSGTR_CannotOpenOutputFile "打�?開輸出文件 '%s'。\n"
#define MSGTR_EncoderOpenFailed "打開編碼器失敗。\n"
#define MSGTR_MencoderWrongFormatAVI "\n警告: 輸出文件格�?是 _AVI_。請查看 -of help。\n"
#define MSGTR_MencoderWrongFormatMPG "\n警告: 輸出文件格�?是 _MPEG_。請查看 -of help。\n"
#define MSGTR_MissingOutputFilename "没有指定輸出文件, 請查看 -o �?�項。"
#define MSGTR_ForcingOutputFourcc "鎖定輸出的 FourCC 為 %x [%.4s]。\n"
#define MSGTR_ForcingOutputAudiofmtTag "鎖定輸出音頻格�?標簽為 0x%x。\n"
#define MSGTR_DuplicateFrames "\n%d 幀�?複!\n"
#define MSGTR_SkipFrame "\n跳幀中!\n"
#define MSGTR_ResolutionDoesntMatch "\n新的視頻文件的解�?度或色彩空間和�?一個�?�?�。\n"
#define MSGTR_FrameCopyFileMismatch "\n所有的視頻文件必須�?有�?�様的幀率, 解�?度和編解碼器�?能使用 -ovc copy。\n"
#define MSGTR_AudioCopyFileMismatch "\n所有的音頻文件必須�?有�?�様的音頻編解碼器和格�?�?能使用 -oac copy。\n"
#define MSGTR_NoAudioFileMismatch "\n�?能把祇有視頻的文件和音頻視頻文件混�?�。試試 -nosound。\n"
#define MSGTR_NoSpeedWithFrameCopy "警告: -speed �?�?證能和 -oac copy 一起正常工作!\n"\
"你的編碼�?�能失敗!\n"
#define MSGTR_ErrorWritingFile "%s: 寫文件錯誤。\n"
#define MSGTR_FlushingVideoFrames "\n清空(flush)視頻幀。\n"
#define MSGTR_FiltersHaveNotBeenConfiguredEmptyFile "�?�濾器尚未�?置! 空文件?\n"
#define MSGTR_RecommendedVideoBitrate "%s CD 推薦的視頻比特率為: %d\n"
#define MSGTR_VideoStreamResult "\n視頻�?: %8.3f kbit/s  (%d B/s)  大�?: %"PRIu64" 字節  %5.3f 秒  %d 幀\n"
#define MSGTR_AudioStreamResult "\n音頻�?: %8.3f kbit/s  (%d B/s)  大�?: %"PRIu64" 字節  %5.3f 秒\n"
#define MSGTR_EdlSkipStartEndCurrent "EDL跳�?�: 開始: %.2f  �?�?�: %.2f   當�?: V: %.2f  A: %.2f     \r"
#define MSGTR_OpenedStream "�?功: 格�?: %d數據: 0x%X - 0x%x\n"
#define MSGTR_VCodecFramecopy "視頻編解碼器: 幀複製 (%dx%d %dbpp fourcc=%x)\n"
#define MSGTR_ACodecFramecopy "音頻編解碼器: 幀複製 (format=%x chans=%d rate=%d bits=%d B/s=%d sample-%d)\n"
#define MSGTR_CBRPCMAudioSelected "已�?� CBR PCM 音頻。\n"
#define MSGTR_MP3AudioSelected "已�?� MP3音頻。\n"
#define MSGTR_CannotAllocateBytes "�?能分�? %d 字節。\n"
#define MSGTR_SettingAudioDelay "設置音頻延�?�為 %5.3fs。\n"
#define MSGTR_SettingVideoDelay "設置視頻延�?�為 %5.3fs。\n"
#define MSGTR_LimitingAudioPreload "�?製音頻�?載值為 0.4s。\n"
#define MSGTR_IncreasingAudioDensity "增加音頻密度為 4。\n"
#define MSGTR_ZeroingAudioPreloadAndMaxPtsCorrection "鎖定音頻�?載值為 0, 最大 PTS 校驗為 0。\n"
#define MSGTR_LameVersion "LAME 版本 %s (%s)\n\n"
#define MSGTR_InvalidBitrateForLamePreset "錯誤: 在此�?設值上指定的比特率超出有效範�?。\n"\
"\n"\
"當使用這種模�?時你必須給定一個在\"8\"到\"320\"之間的數值。\n"\
"\n"\
"更多信�?�，請試試: \"-lameopts preset=help\"\n"
#define MSGTR_InvalidLamePresetOptions "錯誤: 你没有給定一個有效的�?置和/或�?設值�?�項。\n"\
"\n"\
"�?�用的�?置輪廓(profile)包括:\n"\
"\n"\
"   <fast>        standard\n"\
"   <fast>        extreme\n"\
"                 insane\n"\
"   <cbr> (ABR Mode) - ABR 模�?是暗�?�的。�?使用這個�?�項,\n"\
"                      簡單地指定一個比特率就行了。例如:\n"\
"                      \"preset=185\"就�?�以激活這個\n"\
"                      �?設值并使用 185 作為平�?�比特率。\n"\
"\n"\
"    一些例�?:\n"\
"\n"\
"    \"-lameopts fast:preset=standard  \"\n"\
" or \"-lameopts  cbr:preset=192       \"\n"\
" or \"-lameopts      preset=172       \"\n"\
" or \"-lameopts      preset=extreme   \"\n"\
"\n"\
"更多信�?�，請試試: \"-lameopts preset=help\"\n"
#define MSGTR_LamePresetsLongInfo "\n"\
"�?設值開關設計為�??供最好的�?質。\n"\
"\n"\
"它們大多數已經經�?�嚴格的雙盲�?��?�測試來調整和檢驗性能,\n"\
"以�?�到我們�?期的目標。\n"\
"\n"\
"它們�?斷地被�?�級以便和最新的發展�?�?一致,\n"\
"所以應該能給你�??供目�? LAME 所能�??供的將近最好的�?質。\n"\
"\n"\
"�?設值激活:\n"\
"\n"\
"   VBR 模�? (通常情况下的最高�?質):\n"\
"\n"\
"     \"preset=standard\" 此項�?設值顯然應該是大多數人在處�?�大多數的音樂的時候\n"\
"                             所用到的�?�項, 它的�?質已經相當高。\n" \
"\n"\
"     \"preset=extreme\" 如果你有極好的�?�力和相當的設備, 這項�?設值一般會比\n"\
"                             \"standard\"模�?的�?質還�?�??高一點。\n"\
"\n"\
"   CBR 320kbps (�?設值開關�?�項�?的最高�?質):\n"\
"\n"\
"     \"preset=insane\"  �?于大多數人和在大多數情况下, 這個�?�項都顯得有些�?�度。\n"\
"                             但是如果你一定�?有最高�?質并且完全�?關心文件大�?,\n"\
"                             那這正是�?��?�你的。\n"\
"\n"\
"   ABR 模�? (根據給定比特率高�?質, 但�?�?� VBR):\n"\
"\n"\
"     \"preset=<kbps>\"  使用這個�?設值總是會在一個指定的比特率下有�?錯的�?質。\n"\
"                             根據的比特率, �?設值將會决定這種情况下所能�?�到最\n"\
"                             好效果的設置。\n"\
"                             雖然這種方法�?�行, 但它并没有 VBR 模�?那麼�?�活, \n"\
"                             �?�様, 一般也�?��?到 VBR 在高比特率下的�?�等�?質。\n"\
"\n"\
"以下�?�項在相應的�?置文件�?也�?�使用:\n"\
"\n"\
"   <fast>        standard\n"\
"   <fast>        extreme\n"\
"                 insane\n"\
"   <cbr> (ABR Mode) - ABR 模�?是暗�?�的。�?使用這個�?�項,\n"\
"                      簡單地指定一個比特率就行了。例如:\n"\
"                      \"preset=185\"就�?�以激活這個\n"\
"                      �?設值并使用 185 作為平�?�比特率。\n"\
"\n"\
"   \"fast\" - 在一個特定的�?置文件�?啟用這新的快速 VBR 模�?。\n"\
"            速度切�?�的壞處是比特率常常�?比一般情况下的�?高, \n"\
"            �?質也會�?低一點。\n"\
"      警告: 在當�?版本下, 快速�?設值�?�能比一般模�?�??高得太多。\n"\
"\n"\
"   \"cbr\"  - 如果你使用 ABR 模�?(見上)時, 采用特定的比特率, 如\n"\
"            80, 96, 112, 128, 160, 192, 224, 256, 320, 你�?�以使\n"\
"            用\"cbr\"�?�項强製以 CBR 模�?代替標凖 ABR 模�?編碼。\n"\
"            ABR 固然�??供更高的�?質, 但是 CBR 在�?些情况下�?�能會\n"\
"            相當�?�?, 比如從 internet �?一個 MP3 �?。\n"\
"\n"\
"    例如:\n"\
"\n"\
"    \"-lameopts fast:preset=standard  \"\n"\
" 或 \"-lameopts  cbr:preset=192       \"\n"\
" 或 \"-lameopts      preset=172       \"\n"\
" 或 \"-lameopts      preset=extreme   \"\n"\
"\n"\
"\n"\
"ABR 模�?下一些�?�用的别�??:\n"\
"phone => 16kbps/mono        phon+/lw/mw-eu/sw => 24kbps/mono\n"\
"mw-us => 40kbps/mono        voice => 56kbps/mono\n"\
"fm/radio/tape => 112kbps    hifi => 160kbps\n"\
"cd => 192kbps               studio => 256kbps"
#define MSGTR_LameCantInit \
"�?能設定 LAME �?�項, 檢查比特率/采様率, 一些\n"\
"�?�常低的比特率(<32)需�?低采様率(如 -srate 8000)。\n"\
"如果都�?行, 試試使用�?設值。"
#define MSGTR_ConfigFileError "�?置文件錯誤"
#define MSGTR_ErrorParsingCommandLine "解�?命令行錯誤"
#define MSGTR_VideoStreamRequired "視頻�?是必須的!\n"
#define MSGTR_ForcingInputFPS "輸入幀率將被替�?�為 %5.3f。\n"
#define MSGTR_RawvideoDoesNotSupportAudio "RAWVIDEO 輸出文件格�?�?支�?音頻 - �?�用音頻。\n"
#define MSGTR_DemuxerDoesntSupportNosound "目�?此分路器還�?支�? -nosound。\n"
#define MSGTR_MemAllocFailed "内存分�?失敗。\n"
#define MSGTR_NoMatchingFilter "没找到匹�?的 filter/ao 格�?!\n"
#define MSGTR_MP3WaveFormatSizeNot30 "sizeof(MPEGLAYER3WAVEFORMAT)==%d!=30, C 編譯器�?�能挂了?\n"
#define MSGTR_NoLavcAudioCodecName "音頻 LAVC, 没有編解碼器�??!\n"
#define MSGTR_LavcAudioCodecNotFound "音頻 LAVC, 找�?到�?應的編碼器 %s。\n"
#define MSGTR_CouldntAllocateLavcContext "音頻 LAVC, �?能分�?上下文!\n"
#define MSGTR_CouldntOpenCodec "打�?開編解碼器 %s, br=%d。\n"
#define MSGTR_CantCopyAudioFormat "音頻格�? 0x%x 和 '-oac copy' �?兼容, 請試試用 '-oac pcm' 代替或者用 '-fafmttag'。\n"

// cfg-mencoder.h:

#define MSGTR_MEncoderMP3LameHelp "\n\n"\
" vbr=<0-4>     �?�變比特率方�?\n"\
"                0: cbr (常比特率)\n"\
"                1: mt (Mark Taylor VBR 算法)\n"\
"                2: rh (Robert Hegemann VBR 算法 - 默�?)\n"\
"                3: abr (平�?�比特率)\n"\
"                4: mtrh (Mark Taylor Robert Hegemann VBR 算法)\n"\
"\n"\
" abr           平�?�比特率\n"\
"\n"\
" cbr           常比特率\n"\
"               也會在後繼 ABR �?設值模�?中强製以 CBR 模�?編碼。\n"\
"\n"\
" br=<0-1024>   以 kBit 為單�?設置比特率 (僅用于 CBR 和 ABR)\n"\
"\n"\
" q=<0-9>       編碼質�? (0-最高, 9-最低) (僅用于 VBR)\n"\
"\n"\
" aq=<0-9>      算法質�? (0-最好/最慢, 9-最低/最快)\n"\
"\n"\
" ratio=<1-100> 壓縮率\n"\
"\n"\
" vol=<0-10>    設置音頻輸入增益\n"\
"\n"\
" mode=<0-3>    (默�?: 自動)\n"\
"                0: 立體�?�\n"\
"                1: �?��?�立體�?�\n"\
"                2: 雙�?��?�\n"\
"                3: 單�?��?�\n"\
"\n"\
" padding=<0-2>\n"\
"                0: 無\n"\
"                1: 所有\n"\
"                2: 調整\n"\
"\n"\
" fast          在後繼 VBR �?設值模�?中切�?�到更快的編碼方�?，\n"\
"               �?質�?低而比特率�?高。\n"\
"\n"\
" preset=<value> �?�能�??供最高�?質的設置。\n"\
"                 medium: VBR 編碼，�?質：好\n"\
"                 (比特率範�? 150-180 kbps)\n"\
"                 standard:  VBR 編碼, �?質：高\n"\
"                 (比特率範�? 170-210 kbps)\n"\
"                 extreme: VBR 編碼，�?質：�?�常高\n"\
"                 (比特率範�? 200-240 kbps)\n"\
"                 insane:  CBR 編碼，�?質：最高\n"\
"                 (比特率 320 kbps)\n"\
"                 <8-320>: 以給定比特率為平�?�比特率的 ABR 編碼。\n\n"

//codec-cfg.c:
#define MSGTR_DuplicateFourcc "�?複的 FourCC"
#define MSGTR_TooManyFourccs "太多的 FourCCs/formats..."
#define MSGTR_ParseError "解�?錯誤"
#define MSGTR_ParseErrorFIDNotNumber "解�?錯誤(格�? ID �?是一個數字?)"
#define MSGTR_ParseErrorFIDAliasNotNumber "解�?錯誤(格�? ID 别�??�?是一個數字?)"
#define MSGTR_DuplicateFID "�?複的格�? ID"
#define MSGTR_TooManyOut "太多輸出..."
#define MSGTR_InvalidCodecName "\n編解碼器(%s) 的�??稱無效!\n"
#define MSGTR_CodecLacksFourcc "\n編解碼器(%s) 没有 FourCC/format!\n"
#define MSGTR_CodecLacksDriver "\n編解碼器(%s) 没有驅動!\n"
#define MSGTR_CodecNeedsDLL "\n編解碼器(%s) 需�?一個 'dll'!\n"
#define MSGTR_CodecNeedsOutfmt "\n編解碼器(%s) 需�?一個 'outfmt'!\n"
#define MSGTR_CantAllocateComment "�?能為注釋分�?内存。"
#define MSGTR_GetTokenMaxNotLessThanMAX_NR_TOKEN "get_token(): max >= MAX_MR_TOKEN!"
#define MSGTR_CantGetMemoryForLine "�?能為 'line' �?��?�内存: %s\n"
#define MSGTR_CantReallocCodecsp "�?能�?新分�? '*codecsp': %s\n"
#define MSGTR_CodecNameNotUnique "編解碼器�?? '%s' �?唯一。"
#define MSGTR_CantStrdupName "�?能 strdup -> 'name': %s\n"
#define MSGTR_CantStrdupInfo "�?能 strdup -> 'info': %s\n"
#define MSGTR_CantStrdupDriver "�?能 strdup -> 'driver': %s\n"
#define MSGTR_CantStrdupDLL "�?能 strdup -> 'dll': %s"
#define MSGTR_AudioVideoCodecTotals "%d 音頻和 %d 視頻編解碼器\n"
#define MSGTR_CodecDefinitionIncorrect "編解碼器没有正確定義。"
#define MSGTR_OutdatedCodecsConf "此 codecs.conf 太舊，與當�?的 MPlayer �?兼容!"

// fifo.c
#define MSGTR_CannotMakePipe "�?能建立 PIPE!\n"

// parser-mecmd.c, parser-mpcmd.c
#define MSGTR_NoFileGivenOnCommandLine "'--' 表示没有更多�?�項, 但命令行没有給出文件�??。\n"
#define MSGTR_TheLoopOptionMustBeAnInteger "這個loop�?�項必須是個整數: %s\n"
#define MSGTR_UnknownOptionOnCommandLine "命令行有未知的�?�項: -%s\n"
#define MSGTR_ErrorParsingOptionOnCommandLine "解�?命令行�?�項出錯: -%s\n"
#define MSGTR_InvalidPlayEntry "無效的播放�?目 %s\n"
#define MSGTR_NotAnMEncoderOption "-%s �?是一個MEncoder�?�項\n"
#define MSGTR_NoFileGiven "没有給出文件\n"

// m_config.c
#define MSGTR_SaveSlotTooOld "�?存從 lvl %d �?找到的 slot 太舊: %d !!!\n"
#define MSGTR_InvalidCfgfileOption "�?�項 %s �?能在�?置文件�?使用。\n"
#define MSGTR_InvalidCmdlineOption "�?�項 %s �?能在命令行�?使用。\n"
#define MSGTR_InvalidSuboption "錯誤: �?�項 '%s' 没有�?�?�項 '%s'。\n"
#define MSGTR_MissingSuboptionParameter "錯誤: �?�?�項 '%s' (屬于�?�項 '%s') 必須�?有一個�?�數!\n"
#define MSGTR_MissingOptionParameter "錯誤: �?�項 '%s' 必須�?有一個�?�數!\n"
#define MSGTR_OptionListHeader "\n �??字                 類型            最�?       最大     全局  命令行 �?置文件\n\n"
#define MSGTR_TotalOptions "\n總共: %d 個�?�項\n"
#define MSGTR_ProfileInclusionTooDeep "警告: �?置輪廓(Profile)引用太深。\n"
#define MSGTR_NoProfileDefined "没有定義�?置輪廓(Profile)。\n"
#define MSGTR_AvailableProfiles "�?�用的�?置輪廓(Profile):\n"
#define MSGTR_UnknownProfile "未知的�?置輪廓(Profile) '%s'。\n"
#define MSGTR_Profile "�?置輪廓(Profile) %s: %s\n"

// m_property.c
#define MSGTR_PropertyListHeader "\n �??稱                 類型            最�?        最大\n\n"
#define MSGTR_TotalProperties "\n總計: %d �?屬性\n"

// open.c, stream.c:
#define MSGTR_CdDevNotfound "找�?到 CD-ROM 設備 '%s'。\n"
#define MSGTR_ErrTrackSelect "�?�擇 VCD 軌迹出錯。"
#define MSGTR_ReadSTDIN "從標凖輸入中讀�?�...\n"
#define MSGTR_UnableOpenURL "無法打開 URL: %s\n"
#define MSGTR_ConnToServer "連接到�?務器: %s\n"
#define MSGTR_FileNotFound "找�?到文件: '%s'\n"

#define MSGTR_SMBInitError "�?能�?始 libsmbclient 庫: %d\n"
#define MSGTR_SMBFileNotFound "打�?開局域網内的: '%s'\n"
#define MSGTR_SMBNotCompiled "MPlayer 没有編譯�?支�? SMB 的讀�?�。\n"

#define MSGTR_CantOpenDVD "打�?開 DVD 設備: %s (%s)\n"

// stream_dvd.c
#define MSGTR_DVDspeedCantOpen "�?能以寫方�?打開DVD設備, 改變DVD速度需�?寫方�?。\n"
#define MSGTR_DVDrestoreSpeed "�?�複DVD速度... "
#define MSGTR_DVDlimitSpeed "�?製DVD速度至 %dKB/s... "
#define MSGTR_DVDlimitFail "�?製DVD速度失敗。\n"
#define MSGTR_DVDlimitOk "�?製DVD速度�?功。\n"
#define MSGTR_NoDVDSupport "MPlayer 編譯�?�?支�? DVD，退出中。\n"
#define MSGTR_DVDnumTitles "此 DVD 有 %d 個標題。\n"
#define MSGTR_DVDinvalidTitle "無效的 DVD 標題號: %d\n"
#define MSGTR_DVDnumChapters "此 DVD 標題有 %d 章節。\n"
#define MSGTR_DVDinvalidChapter "無效的 DVD 章節號: %d\n"
#define MSGTR_DVDinvalidChapterRange "無效的章節範�? %s\n"
#define MSGTR_DVDinvalidLastChapter "上次無效的 DVD 章節號: %d\n"
#define MSGTR_DVDnumAngles "此 DVD 標題有 %d 個視角。\n"
#define MSGTR_DVDinvalidAngle "無效的 DVD 視角號: %d\n"
#define MSGTR_DVDnoIFO "打�?開 DVD 標題 %d 的 IFO 文件。\n"
#define MSGTR_DVDnoVMG "打�?開 VMG 信�?�!\n"
#define MSGTR_DVDnoVOBs "打�?開標題的 VOBS (VTS_%02d_1.VOB)。\n"
#define MSGTR_DVDnoMatchingAudio "未找到匹�?的 DVD 音頻語言!\n"
#define MSGTR_DVDaudioChannel "已�?� DVD 音頻通�?�: %d 語言: %c%c\n"
#define MSGTR_DVDaudioStreamInfo "音頻�?: %d 格�?: %s (%s) 語言: %s aid: %d。\n"
#define MSGTR_DVDnumAudioChannels "盤上的音頻通�?�數: %d。\n"
#define MSGTR_DVDnoMatchingSubtitle "未找到匹�?的 DVD 字幕語言!\n"
#define MSGTR_DVDsubtitleChannel "已�?� DVD 字幕通�?�: %d 語言: %c%c\n"
#define MSGTR_DVDsubtitleLanguage "字幕號(sid): %d 語言: %s\n"
#define MSGTR_DVDnumSubtitles "盤上的字幕數: %d\n"

// muxer.c, muxer_*.c:
#define MSGTR_TooManyStreams "�?太多!"
#define MSGTR_RawMuxerOnlyOneStream "Rawaudio �?�路器祇支�?一個音頻�?!\n"
#define MSGTR_IgnoringVideoStream "忽略視頻�?!\n"
#define MSGTR_UnknownStreamType "警告, 未知的�?類型: %d\n"
#define MSGTR_WarningLenIsntDivisible "警告, 長度�?能被采様率整除!\n"
#define MSGTR_MuxbufMallocErr "�?�路器幀緩�?無法分�?内存!\n"
#define MSGTR_MuxbufReallocErr "�?�路器幀緩�?無法�?新分�?内存!\n"
#define MSGTR_WritingHeader "正在寫幀頭...\n"
#define MSGTR_WritingTrailer "正在寫索引...\n"

// demuxer.c, demux_*.c:
#define MSGTR_AudioStreamRedefined "警告: 音頻�?頭部 %d 被�?新定義。\n"
#define MSGTR_VideoStreamRedefined "警告: 視頻�?頭部 %d 被�?新定義。\n"
#define MSGTR_TooManyAudioInBuffer "\n緩�?中音頻包太多(%d in %d 字節)。\n"
#define MSGTR_TooManyVideoInBuffer "\n緩�?中視頻包太多(%d in %d 字節)。\n"
#define MSGTR_MaybeNI "(也許你播放了一個�?�交錯的�?/文件或者是編解碼失敗)?\n" \
                      "�?于 AVI 文件, 嘗試用 -ni �?�項鎖定�?�交錯模�?。\n"
#define MSGTR_WorkAroundBlockAlignHeaderBug "AVI: 繞�?� CBR-MP3 nBlockAlign 頭部錯誤!\n"
#define MSGTR_SwitchToNi "\n檢測到糟糕的交錯格�?的 AVI 文件 - 切�?�到 -ni 模�?...\n"
#define MSGTR_InvalidAudioStreamNosound "AVI: 無效的音頻�? ID: %d - 忽略 (nosound)\n"
#define MSGTR_InvalidAudioStreamUsingDefault "AVI: 無效的視頻�? ID: %d - 忽略 (使用默�?值)\n"
#define MSGTR_ON2AviFormat "ON2 AVI 格�?"
#define MSGTR_Detected_XXX_FileFormat "檢測到 %s 文件格�?。\n"
#define MSGTR_DetectedAudiofile "檢測到音頻文件。\n"
#define MSGTR_InvalidMPEGES "MPEG-ES �?無效??? 請�?�係作者, 這�?�能是個錯誤:(\n"
#define MSGTR_FormatNotRecognized "============= 抱歉, 此文件格�?無法辨�?或支�? ===============\n"\
                                  "===     如果此文件是一個 AVI, ASF 或 MPEG �?, 請�?�係作者!    ===\n"
#define MSGTR_SettingProcessPriority "設置進程優先級: %s\n"
#define MSGTR_FilefmtFourccSizeFpsFtime "[V] 文件格�?:%d  fourcc:0x%X  大�?:%dx%d  幀速:%5.3f  幀時間:=%6.4f\n"
#define MSGTR_CannotInitializeMuxer "�?能�?始化muxer。"
#define MSGTR_MissingVideoStream "未找到視頻�?。\n"
#define MSGTR_MissingAudioStream "未找到音頻�?...  -> 没�?�音。\n"
#define MSGTR_MissingVideoStreamBug "没有視頻�?!? 請�?�係作者, 這�?�能是個錯誤:(\n"

#define MSGTR_DoesntContainSelectedStream "分路: 文件中没有所�?�擇的音頻或視頻�?。\n"

#define MSGTR_NI_Forced "鎖定為"
#define MSGTR_NI_Detected "檢測到"
#define MSGTR_NI_Message "%s �?�交錯 AVI 文件模�?!\n"

#define MSGTR_UsingNINI "使用�?�交錯的�??壞的 AVI 文件格�?。\n"
#define MSGTR_CouldntDetFNo "無法决定幀數(用于絶�?�?�索)。\n"
#define MSGTR_CantSeekRawAVI "無法在原始的 AVI �?中�?�索。(需�?索引, 嘗試使用 -idx �?�項。)  \n"
#define MSGTR_CantSeekFile "�?能在此文件中�?�索。\n"

#define MSGTR_MOVcomprhdr "MOV: 支�?壓縮的文件頭需�? ZLIB!\n"
#define MSGTR_MOVvariableFourCC "MOV: 警告: 檢測到�?�變的 FourCC!?\n"
#define MSGTR_MOVtooManyTrk "MOV: 警告: 軌迹太多。"
#define MSGTR_DetectedTV "檢測到 TV! ;-)\n"
#define MSGTR_ErrorOpeningOGGDemuxer "無法打開 Ogg 分路器。\n"
#define MSGTR_CannotOpenAudioStream "打�?開音頻�?: %s\n"
#define MSGTR_CannotOpenSubtitlesStream "打�?開字幕�?: %s\n"
#define MSGTR_OpeningAudioDemuxerFailed "打開音頻分路器: %s 失敗\n"
#define MSGTR_OpeningSubtitlesDemuxerFailed "打開字幕分路器: %s 失敗\n"
#define MSGTR_TVInputNotSeekable "TV 輸入�?能�?�索! (�?�能�?�索應該用來更�?�頻�?�;)\n"
#define MSGTR_DemuxerInfoChanged "分路器信�?� %s 已變�? %s\n"
#define MSGTR_ClipInfo "剪輯信�?�: \n"

#define MSGTR_LeaveTelecineMode "\ndemux_mpg: 檢測到 30fps 的 NTSC 内容, 改變幀率中。\n"
#define MSGTR_EnterTelecineMode "\ndemux_mpg: 檢測到 24fps 漸進的 NTSC 内容, 改變幀率中。\n"

#define MSGTR_CacheFill "\r緩存填充: %5.2f%% (%"PRId64" 字節)   "
#define MSGTR_NoBindFound "找�?到�?� '%s' 的�?��?定。\n"
#define MSGTR_FailedToOpen "打開 %s 失敗。\n"

// dec_video.c & dec_audio.c:
#define MSGTR_CantOpenCodec "打�?開解碼器。\n"
#define MSGTR_CantCloseCodec "�?能關閉解碼器。\n"

#define MSGTR_MissingDLLcodec "錯誤: 打�?開所需的 DirectShow 編解碼器: %s\n"
#define MSGTR_ACMiniterror "�?能加載/�?始化 Win32/ACM 音頻解碼器(缺少 DLL 文件?)。\n"
#define MSGTR_MissingLAVCcodec "在 libavcodec 中找�?到解碼器 '%s'...\n"

#define MSGTR_MpegNoSequHdr "MPEG: 致命錯誤: �?�索�?列頭時�?�到 EOF。\n"
#define MSGTR_CannotReadMpegSequHdr "致命錯誤: �?能讀�?��?列頭。\n"
#define MSGTR_CannotReadMpegSequHdrEx "致命錯誤: �?能讀�?��?列頭擴展。\n"
#define MSGTR_BadMpegSequHdr "MPEG: 糟糕的�?列頭。\n"
#define MSGTR_BadMpegSequHdrEx "MPEG: 糟糕的�?列頭擴展。\n"

#define MSGTR_ShMemAllocFail "�?能分�?共享内存。\n"
#define MSGTR_CantAllocAudioBuf "�?能分�?音頻輸出緩�?。\n"

#define MSGTR_UnknownAudio "未知或缺少音頻格�? -> 没有�?�音\n"

#define MSGTR_UsingExternalPP "[PP] 使用外部的後處�?��?�濾器, max q = %d。\n"
#define MSGTR_UsingCodecPP "[PP] 使用編解碼器的後處�?��?�濾器, max q = %d。\n"
#define MSGTR_VideoCodecFamilyNotAvailableStr "請求的視頻編解碼器�? [%s] (vfm=%s) �?�?�用。\n請在編譯時啟用它。\n"
#define MSGTR_AudioCodecFamilyNotAvailableStr "請求的音頻編解碼器�? [%s] (afm=%s) �?�?�用。\n請在編譯時啟用它。\n"
#define MSGTR_OpeningVideoDecoder "打開視頻解碼器: [%s] %s\n"
#define MSGTR_SelectedVideoCodec "已�?�視頻編解碼器: [%s] vfm: %s (%s)\n"
#define MSGTR_OpeningAudioDecoder "打開音頻解碼器: [%s] %s\n"
#define MSGTR_SelectedAudioCodec "已�?�音頻編解碼器: [%s] afm: %s (%s)\n"
#define MSGTR_VDecoderInitFailed "VDecoder �?始化失敗 :(\n"
#define MSGTR_ADecoderInitFailed "ADecoder �?始化失敗 :(\n"
#define MSGTR_ADecoderPreinitFailed "ADecoder �?�?始化失敗 :(\n"

// LIRC:
#define MSGTR_LIRCopenfailed "打開紅外�?�控支�?失敗。你將無法使用�?�控器。\n"
#define MSGTR_LIRCcfgerr "讀�?� LIRC �?置文件 %s 失敗。\n"

// vf.c
#define MSGTR_CouldNotFindVideoFilter "找�?到視頻濾�?� '%s'。\n"
#define MSGTR_CouldNotOpenVideoFilter "打�?開視頻濾�?� '%s'。\n"
#define MSGTR_OpeningVideoFilter "打開視頻濾�?�: "
#define MSGTR_CannotFindColorspace "找�?到匹�?的色彩空間, 甚至�?��?�入 'scale' 也�?行 :(\n"

// vd.c
#define MSGTR_CodecDidNotSet "VDec: 編解碼器無法設置 sh->disp_w 和 sh->disp_h, 嘗試繞�?�。\n"
#define MSGTR_CouldNotFindColorspace "找�?到匹�?的色彩空間 - �?新嘗試 -vf scale...\n"
#define MSGTR_MovieAspectIsSet "電影寬高比為 %.2f:1 - �?放大到正確的電影寬高比。\n"
#define MSGTR_MovieAspectUndefined "電影寬高比未定義 - 没使用�?放大。\n"

// vd_dshow.c, vd_dmo.c
#define MSGTR_DownloadCodecPackage "你需�?�?�級/安�?二進製編解碼器包。\n請訪�? http://www.mplayerhq.hu/dload.html\n"

// x11_common.c
#define MSGTR_EwmhFullscreenStateFailed "\nX11: �?能發�? EWMH 全�?事件!\n"
#define MSGTR_CouldNotFindXScreenSaver "xscreensaver_disable: 找�?到�?�?(XScreenSaver)窗�?�。\n"
#define MSGTR_SelectedVideoMode "XF86VM: 已�?�視頻模�? %dx%d (圖�?大�? %dx%d)。\n"

#define MSGTR_InsertingAfVolume "[混音器] 没有硬件混音, �?�入音�?�?�濾器。\n"
#define MSGTR_NoVolume "[混音器] 没有�?�用的音�?控製。\n"

// ====================== GUI messages/buttons ========================

#ifdef CONFIG_GUI

// --- labels ---
#define MSGTR_About "關于"
#define MSGTR_FileSelect "�?�擇文件..."
#define MSGTR_SubtitleSelect "�?�擇字幕..."
#define MSGTR_OtherSelect "�?�擇..."
#define MSGTR_AudioFileSelect "�?�擇外部音頻通�?�..."
#define MSGTR_FontSelect "�?�擇字體..."
// Note: If you change MSGTR_PlayList please see if it still fits MSGTR_MENU_PlayList
#define MSGTR_PlayList "播放列表"
#define MSGTR_Equalizer "�?�衡器"
#define MSGTR_ConfigureEqualizer "�?置�?�衡器"
#define MSGTR_SkinBrowser "皮膚�?覽器"
#define MSGTR_Network "網絡�?媒體..."
// Note: If you change MSGTR_Preferences please see if it still fits MSGTR_MENU_Preferences
#define MSGTR_Preferences "首�?�項"
#define MSGTR_AudioPreferences "音頻驅動�?置"
#define MSGTR_NoMediaOpened "没有打開媒體"
#define MSGTR_NoChapter "没有章節"
#define MSGTR_Chapter "章節 %d"
#define MSGTR_NoFileLoaded "没有載入文件"

// --- buttons ---
#define MSGTR_Ok "確定"
#define MSGTR_Cancel "�?�消"
#define MSGTR_Add "添加"
#define MSGTR_Remove "删除"
#define MSGTR_Clear "清空"
#define MSGTR_Config "�?置"
#define MSGTR_ConfigDriver "�?置驅動"
#define MSGTR_Browse "�?覽"

// --- error messages ---
#define MSGTR_NEMDB "抱歉, 没有足够的内存用于繪製緩�?。"
#define MSGTR_NEMFMR "抱歉, 没有足够的内存用于�?�單渲染。"
#define MSGTR_IDFGCVD "抱歉, 未找到 GUI-兼容的視頻輸出驅動。"
#define MSGTR_NEEDLAVC "抱歉, �?能用没有�?編碼的 DXR3/H+ 設備播放 non-MPEG 文件\n請啟用 DXR3/H+ �?置盒中的 lavc。"

// --- skin loader error messages
#define MSGTR_SKIN_ERRORMESSAGE "[skin] 皮膚�?置文件的 %d: %s行出錯"
#define MSGTR_SKIN_SkinFileNotFound "[skin] 文件 (%s) 没找到。\n"
#define MSGTR_SKIN_SkinFileNotReadable "[skin] 文件 (%s) �?�?�讀。\n"
#define MSGTR_SKIN_BITMAP_16bit  "�?支�?少于 16 比特色深的�?圖 (%s)。\n"
#define MSGTR_SKIN_BITMAP_FileNotFound  "找�?到文件 (%s)\n"
#define MSGTR_SKIN_BITMAP_PNGReadError "PNG 讀�?�錯誤 (%s)\n"
#define MSGTR_SKIN_BITMAP_ConversionError "24 比特到 32 比特的轉�?�發生錯誤 (%s)\n"
#define MSGTR_SKIN_UnknownMessage "未知信�?�: %s\n"
#define MSGTR_SKIN_NotEnoughMemory "内存�?够\n"
#define MSGTR_SKIN_FONT_TooManyFontsDeclared "�?�明字體太多。\n"
#define MSGTR_SKIN_FONT_FontFileNotFound "找�?到字體文件。\n"
#define MSGTR_SKIN_FONT_FontImageNotFound "找�?到字體圖�?文件。\n"
#define MSGTR_SKIN_FONT_NonExistentFont "�?存在的字體標簽 (%s)\n"
#define MSGTR_SKIN_UnknownParameter "未知�?�數 (%s)\n"
#define MSGTR_SKIN_SKINCFG_SkinNotFound "找�?到皮膚 (%s)。\n"
#define MSGTR_SKIN_SKINCFG_SelectedSkinNotFound "没找到�?�定的皮膚 (%s), 試�?�使用默�?的...\n"
#define MSGTR_SKIN_LABEL "Skins:"

// --- gtk menus
#define MSGTR_MENU_AboutMPlayer "關于 MPlayer"
#define MSGTR_MENU_Open "打開..."
#define MSGTR_MENU_PlayFile "播放文件..."
#define MSGTR_MENU_PlayVCD "播放 VCD..."
#define MSGTR_MENU_PlayDVD "播放 DVD..."
#define MSGTR_MENU_PlayURL "播放 URL..."
#define MSGTR_MENU_LoadSubtitle "加載字幕..."
#define MSGTR_MENU_DropSubtitle "丢棄字幕..."
#define MSGTR_MENU_LoadExternAudioFile "加載外部音頻文件..."
#define MSGTR_MENU_Playing "播放控製"
#define MSGTR_MENU_Play "播放"
#define MSGTR_MENU_Pause "暫�?�"
#define MSGTR_MENU_Stop "�?�止"
#define MSGTR_MENU_NextStream "下一個"
#define MSGTR_MENU_PrevStream "上一個"
#define MSGTR_MENU_Size "尺寸"
#define MSGTR_MENU_HalfSize   "一�?�尺寸"
#define MSGTR_MENU_NormalSize "正常尺寸"
#define MSGTR_MENU_DoubleSize "雙�?尺寸"
#define MSGTR_MENU_FullScreen "全�?"
#define MSGTR_MENU_DVD "DVD"
#define MSGTR_MENU_VCD "VCD"
#define MSGTR_MENU_PlayDisc "打開盤..."
#define MSGTR_MENU_ShowDVDMenu "顯示 DVD �?�單"
#define MSGTR_MENU_Titles "標題"
#define MSGTR_MENU_Title "標題 %2d"
#define MSGTR_MENU_None "(none)"
#define MSGTR_MENU_Chapters "章節"
#define MSGTR_MENU_Chapter "章節 %2d"
#define MSGTR_MENU_AudioLanguages "音頻語言"
#define MSGTR_MENU_SubtitleLanguages "字幕語言"
#define MSGTR_MENU_PlayList MSGTR_PlayList
#define MSGTR_MENU_SkinBrowser "皮膚�?覽器"
#define MSGTR_MENU_Preferences MSGTR_Preferences
#define MSGTR_MENU_Exit "退出"
#define MSGTR_MENU_Mute "�?�音"
#define MSGTR_MENU_Original "原始的"
#define MSGTR_MENU_AspectRatio "寬高比"
#define MSGTR_MENU_AudioTrack "音頻軌迹"
#define MSGTR_MENU_Track "軌迹 %d"
#define MSGTR_MENU_VideoTrack "視頻軌迹"
#define MSGTR_MENU_Subtitles "字幕"

// --- equalizer
// Note: If you change MSGTR_EQU_Audio please see if it still fits MSGTR_PREFERENCES_Audio
#define MSGTR_EQU_Audio "音頻"
// Note: If you change MSGTR_EQU_Video please see if it still fits MSGTR_PREFERENCES_Video
#define MSGTR_EQU_Video "視頻"
#define MSGTR_EQU_Contrast "�?比度: "
#define MSGTR_EQU_Brightness "亮度: "
#define MSGTR_EQU_Hue "色調: "
#define MSGTR_EQU_Saturation "飽和度: "
#define MSGTR_EQU_Front_Left "�?左"
#define MSGTR_EQU_Front_Right "�?�?�"
#define MSGTR_EQU_Back_Left "後左"
#define MSGTR_EQU_Back_Right "後�?�"
#define MSGTR_EQU_Center "中間"
#define MSGTR_EQU_Bass "低音"
#define MSGTR_EQU_All "所有"
#define MSGTR_EQU_Channel1 "�?��?� 1:"
#define MSGTR_EQU_Channel2 "�?��?� 2:"
#define MSGTR_EQU_Channel3 "�?��?� 3:"
#define MSGTR_EQU_Channel4 "�?��?� 4:"
#define MSGTR_EQU_Channel5 "�?��?� 5:"
#define MSGTR_EQU_Channel6 "�?��?� 6:"

// --- playlist
#define MSGTR_PLAYLIST_Path "路徑"
#define MSGTR_PLAYLIST_Selected "所�?�文件"
#define MSGTR_PLAYLIST_Files "所有文件"
#define MSGTR_PLAYLIST_DirectoryTree "目録樹"

// --- preferences
#define MSGTR_PREFERENCES_Audio MSGTR_EQU_Audio
#define MSGTR_PREFERENCES_Video MSGTR_EQU_Video
#define MSGTR_PREFERENCES_SubtitleOSD "字幕和 OSD "
#define MSGTR_PREFERENCES_Codecs "編解碼器和分路器"
// Note: If you change MSGTR_PREFERENCES_Misc see if it still fits MSGTR_PREFERENCES_FRAME_Misc
#define MSGTR_PREFERENCES_Misc "其他"

#define MSGTR_PREFERENCES_None "None"
#define MSGTR_PREFERENCES_DriverDefault "默�?驅動"
#define MSGTR_PREFERENCES_AvailableDrivers "�?�用驅動:"
#define MSGTR_PREFERENCES_DoNotPlaySound "�?播放�?�音"
#define MSGTR_PREFERENCES_NormalizeSound "�?�音標凖化"
#define MSGTR_PREFERENCES_EnableEqualizer "啟用�?�衡器"
#define MSGTR_PREFERENCES_SoftwareMixer "啟用軟件混音器"
#define MSGTR_PREFERENCES_ExtraStereo "啟用立體�?�加强"
#define MSGTR_PREFERENCES_Coefficient "�?�數:"
#define MSGTR_PREFERENCES_AudioDelay "音頻延�?�"
#define MSGTR_PREFERENCES_DoubleBuffer "啟用雙�?緩�?"
#define MSGTR_PREFERENCES_DirectRender "啟用直接渲染"
#define MSGTR_PREFERENCES_FrameDrop "啟用丢幀"
#define MSGTR_PREFERENCES_HFrameDrop "啟用强製丢幀(�?�險)"
#define MSGTR_PREFERENCES_Flip "上下翻轉圖�?"
#define MSGTR_PREFERENCES_Panscan "摇移: "
#define MSGTR_PREFERENCES_Subtitle "字幕:"
#define MSGTR_PREFERENCES_SUB_Delay "延�?�: "
#define MSGTR_PREFERENCES_SUB_FPS "幀率:"
#define MSGTR_PREFERENCES_SUB_POS "�?置: "
#define MSGTR_PREFERENCES_SUB_AutoLoad "�?�用字幕自動�?載"
#define MSGTR_PREFERENCES_SUB_Unicode "Unicode 字幕"
#define MSGTR_PREFERENCES_SUB_MPSUB "轉�?�給定的字幕�?為 MPlayer 的字幕文件"
#define MSGTR_PREFERENCES_SUB_SRT "轉�?�給定的字幕�?為基于時間的 SubViewer (SRT) 格�?"
#define MSGTR_PREFERENCES_SUB_Overlap "啟/�?�用字幕�?疊"
#define MSGTR_PREFERENCES_SUB_USE_ASS "SSA/ASS 字幕�??供中"
#define MSGTR_PREFERENCES_SUB_ASS_USE_MARGINS "使用邊空白"
#define MSGTR_PREFERENCES_SUB_ASS_TOP_MARGIN "上: "
#define MSGTR_PREFERENCES_SUB_ASS_BOTTOM_MARGIN "下: "
#define MSGTR_PREFERENCES_Font "字體:"
#define MSGTR_PREFERENCES_FontFactor "字體效果:"
#define MSGTR_PREFERENCES_PostProcess "啟用後期處�?�"
#define MSGTR_PREFERENCES_AutoQuality "自動�?質控製: "
#define MSGTR_PREFERENCES_NI "使用�?�交錯的 AVI 解�?器"
#define MSGTR_PREFERENCES_IDX "如果需�?的話, �?建索引表"
#define MSGTR_PREFERENCES_VideoCodecFamily "視頻解碼器�?:"
#define MSGTR_PREFERENCES_AudioCodecFamily "音頻解碼器�?:"
#define MSGTR_PREFERENCES_FRAME_OSD_Level "OSD 級别"
#define MSGTR_PREFERENCES_FRAME_Subtitle "字幕"
#define MSGTR_PREFERENCES_FRAME_Font "字體"
#define MSGTR_PREFERENCES_FRAME_PostProcess "後期處�?�"
#define MSGTR_PREFERENCES_FRAME_CodecDemuxer "編解碼器和分路器"
#define MSGTR_PREFERENCES_FRAME_Cache "緩存"
#define MSGTR_PREFERENCES_FRAME_Misc MSGTR_PREFERENCES_Misc
#define MSGTR_PREFERENCES_Audio_Device "設備:"
#define MSGTR_PREFERENCES_Audio_Mixer "混音器:"
#define MSGTR_PREFERENCES_Audio_MixerChannel "混音通�?�:"
#define MSGTR_PREFERENCES_Message "請注�?, 有些功能祇有�?新播放後能生效。"
#define MSGTR_PREFERENCES_DXR3_VENC "視頻編碼器:"
#define MSGTR_PREFERENCES_DXR3_LAVC "使用 LAVC (FFmpeg)"
#define MSGTR_PREFERENCES_FontEncoding1 "Unicode"
#define MSGTR_PREFERENCES_FontEncoding2 "西�?(ISO-8859-1)"
#define MSGTR_PREFERENCES_FontEncoding3 "西�?(ISO-8859-15)"
#define MSGTR_PREFERENCES_FontEncoding4 "中�?(ISO-8859-2)"
#define MSGTR_PREFERENCES_FontEncoding5 "中�?(ISO-8859-3)"
#define MSGTR_PREFERENCES_FontEncoding6 "波羅的語(ISO-8859-4)"
#define MSGTR_PREFERENCES_FontEncoding7 "斯拉夫語(ISO-8859-5)"
#define MSGTR_PREFERENCES_FontEncoding8 "阿拉伯語(ISO-8859-6)"
#define MSGTR_PREFERENCES_FontEncoding9 "�?�代希臘語(ISO-8859-7)"
#define MSGTR_PREFERENCES_FontEncoding10 "土耳其語(ISO-8859-9)"
#define MSGTR_PREFERENCES_FontEncoding11 "波羅的語(ISO-8859-13)"
#define MSGTR_PREFERENCES_FontEncoding12 "凱爾特語(ISO-8859-14)"
#define MSGTR_PREFERENCES_FontEncoding13 "希伯來語(ISO-8859-8)"
#define MSGTR_PREFERENCES_FontEncoding14 "俄語(KOI8-R)"
#define MSGTR_PREFERENCES_FontEncoding15 "俄語(KOI8-U/RU)"
#define MSGTR_PREFERENCES_FontEncoding16 "簡體中文(CP936)"
#define MSGTR_PREFERENCES_FontEncoding17 "�?體中文(BIG5)"
#define MSGTR_PREFERENCES_FontEncoding18 "日語(SHIFT-JIS)"
#define MSGTR_PREFERENCES_FontEncoding19 "韓語(CP949)"
#define MSGTR_PREFERENCES_FontEncoding20 "泰語(CP874)"
#define MSGTR_PREFERENCES_FontEncoding21 "Windows 的西�?爾語(CP1251)"
#define MSGTR_PREFERENCES_FontEncoding22 "Windows 的西�?爾/中�?語(CP1250)"
#define MSGTR_PREFERENCES_FontNoAutoScale "�?自動縮放"
#define MSGTR_PREFERENCES_FontPropWidth "寬度�?比例"
#define MSGTR_PREFERENCES_FontPropHeight "高度�?比例"
#define MSGTR_PREFERENCES_FontPropDiagonal "�?角綫�?比例"
#define MSGTR_PREFERENCES_FontEncoding "編碼:"
#define MSGTR_PREFERENCES_FontBlur "模糊:"
#define MSGTR_PREFERENCES_FontOutLine "輪廓:"
#define MSGTR_PREFERENCES_FontTextScale "文字縮放:"
#define MSGTR_PREFERENCES_FontOSDScale "OSD 縮放:"
#define MSGTR_PREFERENCES_Cache "打開/關閉緩存"
#define MSGTR_PREFERENCES_CacheSize "緩存大�?: "
#define MSGTR_PREFERENCES_LoadFullscreen "以全�?方�?啟動"
#define MSGTR_PREFERENCES_SaveWinPos "�?存窗�?��?置"
#define MSGTR_PREFERENCES_XSCREENSAVER "�?�用�?�?(XScreenSaver)"
#define MSGTR_PREFERENCES_PlayBar "使用播放�?"
#define MSGTR_PREFERENCES_AutoSync "自�?�步 打開/關閉"
#define MSGTR_PREFERENCES_AutoSyncValue "自�?�步: "
#define MSGTR_PREFERENCES_CDROMDevice "CD-ROM 設備:"
#define MSGTR_PREFERENCES_DVDDevice "DVD 設備:"
#define MSGTR_PREFERENCES_FPS "電影的幀率:"
#define MSGTR_PREFERENCES_ShowVideoWindow "在�?�激活狀態下顯示視頻窗�?�"
#define MSGTR_PREFERENCES_ArtsBroken "新版 aRts 與 GTK 1.x �?兼容, "\
           "會使 GMPlayer 崩潰!"

#define MSGTR_ABOUT_UHU "GUI 開發由 UHU Linux 贊助\n"
#define MSGTR_ABOUT_Contributors "代碼和文檔貢�?�者\n"
#define MSGTR_ABOUT_Codecs_libs_contributions "編解碼器和第三方庫\n"
#define MSGTR_ABOUT_Translations "翻譯\n"
#define MSGTR_ABOUT_Skins "皮膚\n"

// --- messagebox
#define MSGTR_MSGBOX_LABEL_FatalError "致命錯誤!"
#define MSGTR_MSGBOX_LABEL_Error "錯誤!"
#define MSGTR_MSGBOX_LABEL_Warning "警告!"

// cfg.c

#define MSGTR_UnableToSaveOption "[cfg] 無法�?存 '%s' �?�項。\n"

// interface.c

#define MSGTR_DeletingSubtitles "[GUI] 删除字幕。\n"
#define MSGTR_LoadingSubtitles "[GUI] 導入字幕: %s\n"
#define MSGTR_AddingVideoFilter "[GUI] 添加視頻�?�濾器: %s\n"

// mw.c

#define MSGTR_NotAFile "這好�?�?是文件: %s !\n"

// ws.c

#define MSGTR_WS_RemoteDisplay "[ws] �?�程顯示, �?�用 XMITSHM。\n"
#define MSGTR_WS_NoXshm "[ws] 抱歉, 你的係統�?支�? X 共享内存擴展。\n"
#define MSGTR_WS_NoXshape "[ws] 抱歉, 你的係統�?支�? XShape 擴展。\n"
#define MSGTR_WS_ColorDepthTooLow "[ws] 抱歉, 色彩深度太低。\n"
#define MSGTR_WS_TooManyOpenWindows "[ws] 打開窗�?�太多。\n"
#define MSGTR_WS_ShmError "[ws] 共享内存擴展錯誤\n"
#define MSGTR_WS_NotEnoughMemoryDrawBuffer "[ws] 抱歉, 内存�?够繪製緩�?。\n"
#define MSGTR_WS_DpmsUnavailable "DPMS �?�?�用?\n"
#define MSGTR_WS_DpmsNotEnabled "�?能啟用 DPMS。\n"

// wsxdnd.c

#define MSGTR_WS_NotAFile "這好�?�?是一個文件...\n"
#define MSGTR_WS_DDNothing "D&D: 没有任何�?�西返回!\n"

#endif

// ======================= VO Video Output drivers ========================

#define MSGTR_VOincompCodec "�?�定的視頻輸出設備和這個編解碼器�?兼容。\n"\
                "試�?�添加縮放�?�濾器, 例如以 -vf spp,scale 來代替 -vf spp。\n"
#define MSGTR_VO_GenericError "這個錯誤已經發生"
#define MSGTR_VO_UnableToAccess "無法訪�?"
#define MSGTR_VO_ExistsButNoDirectory "已經存在, 但�?是一個目録。"
#define MSGTR_VO_DirExistsButNotWritable "輸出目録已經存在, 但是�?�?�寫。"
#define MSGTR_VO_DirExistsAndIsWritable "輸出目録已經存在并且�?�寫。"
#define MSGTR_VO_CantCreateDirectory "無法創建輸出目録。"
#define MSGTR_VO_CantCreateFile "無法創建輸出文件。"
#define MSGTR_VO_DirectoryCreateSuccess "輸出目録創建�?功。"
#define MSGTR_VO_ValueOutOfRange "值超出範�?"
#define MSGTR_VO_NoValueSpecified "没有指定值。"
#define MSGTR_VO_UnknownSuboptions "未知�?�?�項"

// vo_aa.c

#define MSGTR_VO_AA_HelpHeader "\n\n這是 aalib vo_aa �?�?�項:\n"
#define MSGTR_VO_AA_AdditionalOptions "vo_aa �??供的附加�?�項:\n" \
"  help        顯示此幚助信�?�\n" \
"  osdcolor    設定 OSD 顔色\n  subcolor    設定字幕顔色\n" \
"        顔色�?�數有:\n           0 : 一般\n" \
"           1 : 模糊\n           2 : 粗\n           3 : 粗字體\n" \
"           4 : �??色\n           5 : 特殊\n\n\n"

// vo_jpeg.c
#define MSGTR_VO_JPEG_ProgressiveJPEG "啟用漸顯 JPEG。"
#define MSGTR_VO_JPEG_NoProgressiveJPEG "�?�用漸顯 JPEG。"
#define MSGTR_VO_JPEG_BaselineJPEG "啟用基綫 JPEG。"
#define MSGTR_VO_JPEG_NoBaselineJPEG "�?�用基綫 JPEG。"

// vo_pnm.c
#define MSGTR_VO_PNM_ASCIIMode "啟用 ASCII 模�?。"
#define MSGTR_VO_PNM_RawMode "啟用 Raw 模�?。"
#define MSGTR_VO_PNM_PPMType "將�?寫入 PPM 文件。"
#define MSGTR_VO_PNM_PGMType "將�?寫入 PGM 文件。"
#define MSGTR_VO_PNM_PGMYUVType "將�?寫入 PGMYUV 文件。"

// vo_yuv4mpeg.c
#define MSGTR_VO_YUV4MPEG_InterlacedHeightDivisibleBy4 "交錯模�?�?求圖�?高度能被 4 整除。"
#define MSGTR_VO_YUV4MPEG_InterlacedLineBufAllocFail "無法為交錯模�?分�?綫緩�?。"
#define MSGTR_VO_YUV4MPEG_WidthDivisibleBy2 "圖�?寬度必須能被 2 整除。"
#define MSGTR_VO_YUV4MPEG_OutFileOpenError "�?能�?�得内存或文件�?�柄以寫入 \"%s\"!"
#define MSGTR_VO_YUV4MPEG_OutFileWriteError "圖�?寫到輸出錯誤!"
#define MSGTR_VO_YUV4MPEG_UnknownSubDev "未知的�?設備: %s"
#define MSGTR_VO_YUV4MPEG_InterlacedTFFMode "使用交錯輸出模�?, �?場(奇數圖場)優先。"
#define MSGTR_VO_YUV4MPEG_InterlacedBFFMode "使用交錯輸出模�?, 後場(�?�數圖場)優先。"
#define MSGTR_VO_YUV4MPEG_ProgressiveMode "使用(默�?的) 漸顯幀模�?。"

// sub.c
#define MSGTR_VO_SUB_Seekbar "�?�索�?"
#define MSGTR_VO_SUB_Play "播放"
#define MSGTR_VO_SUB_Pause "暫�?�"
#define MSGTR_VO_SUB_Stop "�?�止"
#define MSGTR_VO_SUB_Rewind "後退"
#define MSGTR_VO_SUB_Forward "快進"
#define MSGTR_VO_SUB_Clock "計時"
#define MSGTR_VO_SUB_Contrast "�?比度"
#define MSGTR_VO_SUB_Saturation "飽和度"
#define MSGTR_VO_SUB_Volume "音�?"
#define MSGTR_VO_SUB_Brightness "亮度"
#define MSGTR_VO_SUB_Hue "色調"

// vo_xv.c
#define MSGTR_VO_XV_ImagedimTooHigh "�?圖�?尺寸太大: %ux%u (上�?是 %ux%u)\n"

// Old vo drivers that have been replaced

#define MSGTR_VO_PGM_HasBeenReplaced "PGM 視頻輸出驅動已經被 -vo pnm:pgmyuv 代替。\n"
#define MSGTR_VO_MD5_HasBeenReplaced "MD5 視頻輸出驅動已經被 -vo md5sum 代替。\n"

// ======================= AO Audio Output drivers ========================

// libao2

// audio_out.c
#define MSGTR_AO_ALSA9_1x_Removed "音頻輸出: alsa9 和 alsa1x 模塊已被删除, 請用 -ao alsa 代替。\n"

// ao_oss.c
#define MSGTR_AO_OSS_CantOpenMixer "[AO OSS] 音頻設置: 無法打開混音器設備 %s: %s\n"
#define MSGTR_AO_OSS_ChanNotFound "[AO OSS] 音頻設置: �?��?�混音器没有'%s', 使用默�?通�?�。\n"
#define MSGTR_AO_OSS_CantOpenDev "[AO OSS] 音頻設置: 無法打開音頻設備 %s: %s\n"
#define MSGTR_AO_OSS_CantMakeFd "[AO OSS] 音頻設置: 無法建立文件�??述塊: %s\n"
#define MSGTR_AO_OSS_CantSet "[AO OSS] 無法設定音頻設備 %s 到 %s 的輸出, 試�?�使用 %s...\n"
#define MSGTR_AO_OSS_CantSetChans "[AO OSS] 音頻設置: 設定音頻設備到 %d 通�?�失敗。\n"
#define MSGTR_AO_OSS_CantUseGetospace "[AO OSS] 音頻設置: 驅動�?支�? SNDCTL_DSP_GETOSPACE :-(\n"
#define MSGTR_AO_OSS_CantUseSelect "[AO OSS]\n   ***  你的音頻驅動�?支�? select()  ***\n 請用 config.h 中的 #undef HAVE_AUDIO_SELECT �?新編譯 MPlayer!\n\n"
#define MSGTR_AO_OSS_CantReopen "[AO OSS]\n致命錯誤: *** 無法�?新打開或�?設音頻設備 *** %s\n"
#define MSGTR_AO_OSS_UnknownUnsupportedFormat "[AO OSS] 未知/�?支�?的 OSS 格�?: %x。\n"

// ao_arts.c
#define MSGTR_AO_ARTS_CantInit "[AO ARTS] %s\n"
#define MSGTR_AO_ARTS_ServerConnect "[AO ARTS] 已連接到�?�音設備。\n"
#define MSGTR_AO_ARTS_CantOpenStream "[AO ARTS] 無法打開一個�?。\n"
#define MSGTR_AO_ARTS_StreamOpen "[AO ARTS] �?已經打開。\n"
#define MSGTR_AO_ARTS_BufferSize "[AO ARTS] 緩�?大�?: %d\n"

// ao_dxr2.c
#define MSGTR_AO_DXR2_SetVolFailed "[AO DXR2] 設定音�?為 %d 失敗。\n"
#define MSGTR_AO_DXR2_UnsupSamplerate "[AO DXR2] �?支�? %d Hz, 試試�?采様。\n"

// ao_esd.c
#define MSGTR_AO_ESD_CantOpenSound "[AO ESD] esd_open_sound 失敗: %s\n"
#define MSGTR_AO_ESD_LatencyInfo "[AO ESD] 延�?�: [�?務器: %0.2fs, 網絡: %0.2fs] (調整 %0.2fs)\n"
#define MSGTR_AO_ESD_CantOpenPBStream "[AO ESD] 打開 ESD 播放�?失敗: %s\n"

// ao_mpegpes.c
#define MSGTR_AO_MPEGPES_CantSetMixer "[AO MPEGPES] DVB 音頻設置混音器錯誤: %s。\n"
#define MSGTR_AO_MPEGPES_UnsupSamplerate "[AO MPEGPES] �?支�? %d Hz, 試試�?采様。\n"

// ao_null.c
// This one desn't even  have any mp_msg nor printf's?? [CHECK]

// ao_pcm.c
#define MSGTR_AO_PCM_FileInfo "[AO PCM] 文件: %s (%s)\nPCM: 采様率: %iHz 通�?�: %s 格�? %s\n"
#define MSGTR_AO_PCM_HintInfo "[AO PCM] 信�?�: 用 -vc null -vo null �?�以更快速的轉儲\n[AO PCM] 信�?�: 如果�?寫 WAVE 文件, 使用 -ao pcm:waveheader (默�?)。\n"
#define MSGTR_AO_PCM_CantOpenOutputFile "[AO PCM] 打開寫 %s 失敗!\n"

// ao_sdl.c
#define MSGTR_AO_SDL_INFO "[AO SDL] 采様率: %iHz 通�?�: %s 格�? %s\n"
#define MSGTR_AO_SDL_DriverInfo "[AO SDL] 使用 %s 音頻驅動。\n"
#define MSGTR_AO_SDL_UnsupportedAudioFmt "[AO SDL] �?支�?的音頻格�?: 0x%x。\n"
#define MSGTR_AO_SDL_CantInit "[AO SDL] SDL 音頻�?始化失敗: %s\n"
#define MSGTR_AO_SDL_CantOpenAudio "[AO SDL] 無法打開音頻: %s\n"

// ao_sgi.c
#define MSGTR_AO_SGI_INFO "[AO SGI] 控製。\n"
#define MSGTR_AO_SGI_InitInfo "[AO SGI] �?始: 采様率: %iHz 通�?�: %s 格�? %s\n"
#define MSGTR_AO_SGI_InvalidDevice "[AO SGI] 播放: 無效的設備。\n"
#define MSGTR_AO_SGI_CantSetParms_Samplerate "[AO SGI] �?始: 設定�?�數失敗: %s\n�?能設定需�?的采様率。\n"
#define MSGTR_AO_SGI_CantSetAlRate "[AO SGI] �?始: AL_RATE 在給定的�?上�?能用。\n"
#define MSGTR_AO_SGI_CantGetParms "[AO SGI] �?始: �?��?��?�數失敗: %s\n"
#define MSGTR_AO_SGI_SampleRateInfo "[AO SGI] �?始: 當�?的采様率為 %f (需�?的速率是 %f)\n"
#define MSGTR_AO_SGI_InitConfigError "[AO SGI] �?始: %s\n"
#define MSGTR_AO_SGI_InitOpenAudioFailed "[AO SGI] �?始: 無法打開音頻通�?�: %s\n"
#define MSGTR_AO_SGI_Uninit "[AO SGI] �??�?始: ...\n"
#define MSGTR_AO_SGI_Reset "[AO SGI] �?置: ...\n"
#define MSGTR_AO_SGI_PauseInfo "[AO SGI] 音頻暫�?�: ...\n"
#define MSGTR_AO_SGI_ResumeInfo "[AO SGI] 音頻�?�複: ...\n"

// ao_sun.c
#define MSGTR_AO_SUN_RtscSetinfoFailed "[AO SUN] rtsc: SETINFO 失敗。\n"
#define MSGTR_AO_SUN_RtscWriteFailed "[AO SUN] rtsc: 寫失敗。\n"
#define MSGTR_AO_SUN_CantOpenAudioDev "[AO SUN] 無法打開音頻設備 %s, %s  -> 没�?�音。\n"
#define MSGTR_AO_SUN_UnsupSampleRate "[AO SUN] 音頻設置: 你的�?��?��?支�? %d 通�?�, %s, %d Hz 采様率。\n"
#define MSGTR_AO_SUN_CantUseSelect "[AO SUN]\n   ***  你的音頻驅動�?支�? select()  ***\n用 config.h 中的 #undef HAVE_AUDIO_SELECT �?新編譯 MPlayer!\n\n"
#define MSGTR_AO_SUN_CantReopenReset "[AO SUN]\n致命錯誤: *** 無法�?新打開或�?設音頻設備 (%s) ***\n"

// ao_alsa.c
#define MSGTR_AO_ALSA_InvalidMixerIndexDefaultingToZero "[AO_ALSA] 無效的混音索引。�?�默�?值 0。\n"
#define MSGTR_AO_ALSA_MixerOpenError "[AO_ALSA] 混音打開錯誤: %s\n"
#define MSGTR_AO_ALSA_MixerAttachError "[AO_ALSA] 混音接上 %s 錯誤: %s\n"
#define MSGTR_AO_ALSA_MixerRegisterError "[AO_ALSA] 混音注册錯誤: %s\n"
#define MSGTR_AO_ALSA_MixerLoadError "[AO_ALSA] 混音�?載錯誤: %s\n"
#define MSGTR_AO_ALSA_UnableToFindSimpleControl "[AO_ALSA] 無法找到控製 '%s',%i。\n"
#define MSGTR_AO_ALSA_ErrorSettingLeftChannel "[AO_ALSA] 錯誤設置左�?��?�, %s\n"
#define MSGTR_AO_ALSA_ErrorSettingRightChannel "[AO_ALSA] 錯誤設置�?��?��?�, %s\n"
#define MSGTR_AO_ALSA_CommandlineHelp "\n[AO_ALSA] -ao alsa 命令行幚助:\n"\
"[AO_ALSA] 示例: mplayer -ao alsa:device=hw=0.3\n"\
"[AO_ALSA]   設置第一�?�第四硬件設備。\n\n"\
"[AO_ALSA] �?�項:\n"\
"[AO_ALSA]   noblock\n"\
"[AO_ALSA]     以 non-blocking 模�?打開設備。\n"\
"[AO_ALSA]   device=<device-name>\n"\
"[AO_ALSA]     設置設備 (change , to . and : to =)\n"
#define MSGTR_AO_ALSA_ChannelsNotSupported "[AO_ALSA] %d �?��?��?被支�?。\n"
#define MSGTR_AO_ALSA_OpenInNonblockModeFailed "[AO_ALSA] 打開 nonblock-模�? 失敗, 試�?�打開 block-模�?。\n"
#define MSGTR_AO_ALSA_PlaybackOpenError "[AO_ALSA] 回放打開錯誤: %s\n"
#define MSGTR_AO_ALSA_ErrorSetBlockMode "[AL_ALSA] 錯誤設置 block-模�? %s。\n"
#define MSGTR_AO_ALSA_UnableToGetInitialParameters "[AO_ALSA] 無法得到�?始�?�數: %s\n"
#define MSGTR_AO_ALSA_UnableToSetAccessType "[AO_ALSA] 無法設置訪�?類型: %s\n"
#define MSGTR_AO_ALSA_FormatNotSupportedByHardware "[AO_ALSA] 格�? %s �?被硬件支�?, 試試默�?的。\n"
#define MSGTR_AO_ALSA_UnableToSetFormat "[AO_ALSA] 無法設置格�?: %s\n"
#define MSGTR_AO_ALSA_UnableToSetChannels "[AO_ALSA] 無法設置�?��?�: %s\n"
#define MSGTR_AO_ALSA_UnableToDisableResampling "[AO_ALSA] 無法�?�用�?抽様: %s\n"
#define MSGTR_AO_ALSA_UnableToSetSamplerate2 "[AO_ALSA] 無法設置 采様率-2: %s\n"
#define MSGTR_AO_ALSA_UnableToSetBufferTimeNear "[AO_ALSA] 無法設置緩�?時間約: %s\n"
#define MSGTR_AO_ALSA_UnableToGetPeriodSize "[AO ALSA] 無法�?�得�?�段大�?: %s\n"
#define MSGTR_AO_ALSA_UnableToSetPeriods "[AO_ALSA] 無法設置�?�段: %s\n"
#define MSGTR_AO_ALSA_UnableToSetHwParameters "[AO_ALSA] 無法設置 hw-parameters: %s\n"
#define MSGTR_AO_ALSA_UnableToGetBufferSize "[AO_ALSA] 無法�?�得緩�?大�?: %s\n"
#define MSGTR_AO_ALSA_UnableToGetSwParameters "[AO_ALSA] 無法�?�得 sw-parameters: %s\n"
#define MSGTR_AO_ALSA_UnableToSetSwParameters "[AO_ALSA] 無法設置 sw-parameters: %s\n"
#define MSGTR_AO_ALSA_UnableToGetBoundary "[AO_ALSA] 無法�?�得邊界: %s\n"
#define MSGTR_AO_ALSA_UnableToSetStartThreshold "[AO_ALSA] 無法設置開始點: %s\n"
#define MSGTR_AO_ALSA_UnableToSetStopThreshold "[AO_ALSA] 無法設置�?�止點: %s\n"
#define MSGTR_AO_ALSA_UnableToSetSilenceSize "[AO_ALSA] 無法設置安�?�大�?: %s\n"
#define MSGTR_AO_ALSA_PcmCloseError "[AO_ALSA] pcm 關閉錯誤: %s\n"
#define MSGTR_AO_ALSA_NoHandlerDefined "[AO_ALSA] 没定義處�?�器!\n"
#define MSGTR_AO_ALSA_PcmPrepareError "[AO_ALSA] pcm 凖備錯誤: %s\n"
#define MSGTR_AO_ALSA_PcmPauseError "[AO_ALSA] pcm 暫�?�錯誤: %s\n"
#define MSGTR_AO_ALSA_PcmDropError "[AO_ALSA] pcm 丢棄錯誤: %s\n"
#define MSGTR_AO_ALSA_PcmResumeError "[AO_ALSA] pcm �?�複錯誤: %s\n"
#define MSGTR_AO_ALSA_DeviceConfigurationError "[AO_ALSA] 設備�?置錯誤。"
#define MSGTR_AO_ALSA_PcmInSuspendModeTryingResume "[AO_ALSA] Pcm 在挂機模�?, 試�?��?�複。\n"
#define MSGTR_AO_ALSA_WriteError "[AO_ALSA] 寫錯誤: %s\n"
#define MSGTR_AO_ALSA_TryingToResetSoundcard "[AO_ALSA] 試�?��?置�?��?�。\n"
#define MSGTR_AO_ALSA_CannotGetPcmStatus "[AO_ALSA] �?能�?�得 pcm 狀態: %s\n"

// ao_plugin.c

#define MSGTR_AO_PLUGIN_InvalidPlugin "[AO PLUGIN] 無效�?�件: %s\n"

// ======================= AF Audio Filters ================================

// libaf

// af_ladspa.c

#define MSGTR_AF_LADSPA_AvailableLabels "�?�用的標簽"
#define MSGTR_AF_LADSPA_WarnNoInputs "警告! 此 LADSPA �?�件没有音頻輸入。\n 以後的音頻信號將會丢失。"
#define MSGTR_AF_LADSPA_ErrMultiChannel "�?�在還�?支�?多通�?�(>2)�?�件。\n 祇能使用單�?��?�或立體�?��?��?�件。"
#define MSGTR_AF_LADSPA_ErrNoOutputs "此 LADSPA �?�件没有音頻輸出。"
#define MSGTR_AF_LADSPA_ErrInOutDiff "LADSPA �?�件的音頻輸入和音頻輸出的數目�?相等。"
#define MSGTR_AF_LADSPA_ErrFailedToLoad "�?載失敗"
#define MSGTR_AF_LADSPA_ErrNoDescriptor "在指定的庫文件�?找�?到 ladspa_descriptor() 函數。"
#define MSGTR_AF_LADSPA_ErrLabelNotFound "在�?�件庫�?找�?到標簽。"
#define MSGTR_AF_LADSPA_ErrNoSuboptions "没有指定�?�?�項標簽。"
#define MSGTR_AF_LADSPA_ErrNoLibFile "没有指定庫文件。"
#define MSGTR_AF_LADSPA_ErrNoLabel "没有指定�?�濾器標簽。"
#define MSGTR_AF_LADSPA_ErrNotEnoughControls "命令行指定的控製項�?够。"
#define MSGTR_AF_LADSPA_ErrControlBelow "%s: 輸入控製 #%d 低于下�? %0.4f。\n"
#define MSGTR_AF_LADSPA_ErrControlAbove "%s: 輸入控製 #%d 高于上�? %0.4f。\n"

// format.c

#define MSGTR_AF_FORMAT_UnknownFormat "未知格�?"

// ========================== INPUT =========================================

// joystick.c

#define MSGTR_INPUT_JOYSTICK_CantOpen "打�?開�?縱�?�設備 %s: %s\n"
#define MSGTR_INPUT_JOYSTICK_ErrReading "讀�?縱�?�設備時發生錯誤: %s\n"
#define MSGTR_INPUT_JOYSTICK_LoosingBytes "�?縱�?�: 丢失了 %d 字節的數據\n"
#define MSGTR_INPUT_JOYSTICK_WarnLostSync "�?縱�?�: 警告�?始事件, 失去了和驅動的�?�步。\n"
#define MSGTR_INPUT_JOYSTICK_WarnUnknownEvent "�?作�?�警告未知事件類型%d\n"

// input.c

#define MSGTR_INPUT_INPUT_ErrCantRegister2ManyCmdFds "命令文件�??述符太多, �?能注册文件�??述符 %d。\n"
#define MSGTR_INPUT_INPUT_ErrCantRegister2ManyKeyFds "�?�文件�??述符太多, 無法注册文件�??述符 %d。\n"
#define MSGTR_INPUT_INPUT_ErrArgMustBeInt "命令 %s: �?�數 %d �?是整數。\n"
#define MSGTR_INPUT_INPUT_ErrArgMustBeFloat "命令 %s: �?�數 %d �?是浮點數。\n"
#define MSGTR_INPUT_INPUT_ErrUnterminatedArg "命令 %s: �?�數 %d 無�?�?�符。\n"
#define MSGTR_INPUT_INPUT_ErrUnknownArg "未知�?�數 %d\n"
#define MSGTR_INPUT_INPUT_Err2FewArgs "命令 %s 需�?至少 %d 個�?�數, 然而祇發�?�了 %d 個。\n"
#define MSGTR_INPUT_INPUT_ErrReadingCmdFd "讀�?�命令文件�??述符 %d 時發生錯誤: %s\n"
#define MSGTR_INPUT_INPUT_ErrCmdBufferFullDroppingContent "文件�??述符 %d 的命令緩存已滿: 正在丢失内容。\n"
#define MSGTR_INPUT_INPUT_ErrInvalidCommandForKey "�?定�?� %s 的命令無效"
#define MSGTR_INPUT_INPUT_ErrSelect "�?�定錯誤: %s\n"
#define MSGTR_INPUT_INPUT_ErrOnKeyInFd "�?�輸入文件�??述符 %d 發生錯誤\n"
#define MSGTR_INPUT_INPUT_ErrDeadKeyOnFd "�?�輸入文件�??述符 %d 得到死�?�\n"
#define MSGTR_INPUT_INPUT_Err2ManyKeyDowns "�?�時有太多的按�?�事件發生\n"
#define MSGTR_INPUT_INPUT_ErrOnCmdFd "命令文件�??述符 %d 發生錯誤\n"
#define MSGTR_INPUT_INPUT_ErrReadingInputConfig "當讀�?�輸入�?置文件 %s 時發生錯誤: %s\n"
#define MSGTR_INPUT_INPUT_ErrUnknownKey "未知�?� '%s'\n"
#define MSGTR_INPUT_INPUT_ErrUnfinishedBinding "未完�?的�?定 %s\n"
#define MSGTR_INPUT_INPUT_ErrBuffer2SmallForKeyName "此�?��??的緩存太�?: %s\n"
#define MSGTR_INPUT_INPUT_ErrNoCmdForKey "找�?到�?� %s 的命令"
#define MSGTR_INPUT_INPUT_ErrBuffer2SmallForCmd "此命令的緩存太�?: %s\n"
#define MSGTR_INPUT_INPUT_ErrWhyHere "怎麼會�?�行到這�?了?\n"
#define MSGTR_INPUT_INPUT_ErrCantInitJoystick "�?能�?始�?�輸入法�?縱�?�\n"
#define MSGTR_INPUT_INPUT_ErrCantOpenFile "打�?開 %s: %s\n"
#define MSGTR_INPUT_INPUT_ErrCantInitAppleRemote "�?能�?始化 Apple Remote �?�控器。\n"

// ========================== LIBMPDEMUX ===================================

// url.c

#define MSGTR_MPDEMUX_URL_StringAlreadyEscaped "字符轉義好�?已發生在 url_escape %c%c1%c2\n"

// ai_alsa.c

#define MSGTR_MPDEMUX_AIALSA_CannotSetSamplerate "無法設置采様率。\n"
#define MSGTR_MPDEMUX_AIALSA_CannotSetBufferTime "無法設置緩�?時間。\n"
#define MSGTR_MPDEMUX_AIALSA_CannotSetPeriodTime "無法設置間隔時間。\n"

// ai_alsa.c

#define MSGTR_MPDEMUX_AIALSA_PcmBrokenConfig "此 PCM 的�?置文件�??壞: �?置�?�?�用。\n"
#define MSGTR_MPDEMUX_AIALSA_UnavailableAccessType "訪�?類型�?�?�用。\n"
#define MSGTR_MPDEMUX_AIALSA_UnavailableSampleFmt "采様文件�?�?�用。\n"
#define MSGTR_MPDEMUX_AIALSA_UnavailableChanCount "通�?�記數�?�?�用 - 使用默�?: %d\n"
#define MSGTR_MPDEMUX_AIALSA_CannotInstallHWParams "無法安�?硬件�?�數: %s"
#define MSGTR_MPDEMUX_AIALSA_PeriodEqualsBufferSize "�?能使用等于緩�?大�?的間隔 (%u == %lu)\n"
#define MSGTR_MPDEMUX_AIALSA_CannotInstallSWParams "無法安�?軟件�?�數:\n"
#define MSGTR_MPDEMUX_AIALSA_ErrorOpeningAudio "打開音頻錯誤: %s\n"
#define MSGTR_MPDEMUX_AIALSA_AlsaXRUN "ALSA xrun!!! (至少 %.3f ms)\n"
#define MSGTR_MPDEMUX_AIALSA_AlsaXRUNPrepareError "ALSA xrun: 凖備錯誤: %s"
#define MSGTR_MPDEMUX_AIALSA_AlsaReadWriteError "ALSA 讀/寫錯誤"

// ai_oss.c

#define MSGTR_MPDEMUX_AIOSS_Unable2SetChanCount "無法設置通�?�數: %d\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2SetStereo "無法設置立體�?�: %d\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2Open "無法打開 '%s': %s\n"
#define MSGTR_MPDEMUX_AIOSS_UnsupportedFmt "�?支�?的格�?\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2SetAudioFmt "無法設置音頻格�?。"
#define MSGTR_MPDEMUX_AIOSS_Unable2SetSamplerate "無法設置采様率: %d\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2SetTrigger "無法設置觸發器: %d\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2GetBlockSize "無法�?�得塊大�?!\n"
#define MSGTR_MPDEMUX_AIOSS_AudioBlockSizeZero "音頻塊大�?是零, 設�? %d!\n"
#define MSGTR_MPDEMUX_AIOSS_AudioBlockSize2Low "音頻塊大�?太�?, 設�? %d!\n"

// asfheader.c

#define MSGTR_MPDEMUX_ASFHDR_HeaderSizeOver1MB "致命: 頭部的大�?超�?� 1 MB (%d)!\n請�?�係 MPlayer 的作者, 并且發�?或上傳此文件。\n"
#define MSGTR_MPDEMUX_ASFHDR_HeaderMallocFailed "�?能為頭部分�? %d 字節的空間。\n"
#define MSGTR_MPDEMUX_ASFHDR_EOFWhileReadingHeader "讀 ASF 頭部時�?�到 EOF, 文件�??壞或�?完整?\n"
#define MSGTR_MPDEMUX_ASFHDR_DVRWantsLibavformat "DVR �?�能祇能和 libavformat 一起工作, 如果有�?題請試試 -demuxer 35\n"
#define MSGTR_MPDEMUX_ASFHDR_NoDataChunkAfterHeader "没有數據塊緊隨頭部之後!\n"
#define MSGTR_MPDEMUX_ASFHDR_AudioVideoHeaderNotFound "ASF: 找�?到音頻或視頻頭部 - 文件�??壞?\n"
#define MSGTR_MPDEMUX_ASFHDR_InvalidLengthInASFHeader "無效的 ASF 頭部長度!\n"

// asf_mmst_streaming.c

#define MSGTR_MPDEMUX_MMST_WriteError "寫錯誤\n"
#define MSGTR_MPDEMUX_MMST_EOFAlert "\n�??醒! EOF 文件�?�?�\n"
#define MSGTR_MPDEMUX_MMST_PreHeaderReadFailed "頭部�?讀�?�失敗\n"
#define MSGTR_MPDEMUX_MMST_InvalidHeaderSize "無效的頭部大�?, 正在放棄。\n"
#define MSGTR_MPDEMUX_MMST_HeaderDataReadFailed "讀頭部數據失敗。\n"
#define MSGTR_MPDEMUX_MMST_packet_lenReadFailed "讀 packet_len 失敗。\n"
#define MSGTR_MPDEMUX_MMST_InvalidRTSPPacketSize "無效的 RTSP 包大�?, 正在放棄。\n"
#define MSGTR_MPDEMUX_MMST_CmdDataReadFailed "讀命令數據失敗。\n"
#define MSGTR_MPDEMUX_MMST_HeaderObject "頭部�?象\n"
#define MSGTR_MPDEMUX_MMST_DataObject "數據�?象\n"
#define MSGTR_MPDEMUX_MMST_FileObjectPacketLen "文件�?象, 包長 = %d (%d)\n"
#define MSGTR_MPDEMUX_MMST_StreamObjectStreamID "�?�?象, �? ID: %d\n"
#define MSGTR_MPDEMUX_MMST_2ManyStreamID "ID 太多, 跳�?��?。"
#define MSGTR_MPDEMUX_MMST_UnknownObject "未知的�?象\n"
#define MSGTR_MPDEMUX_MMST_MediaDataReadFailed "讀媒體數據失敗\n"
#define MSGTR_MPDEMUX_MMST_MissingSignature "簽�??缺失\n"
#define MSGTR_MPDEMUX_MMST_PatentedTechnologyJoke "一切�?�?�。感�?你下載一個包�?�專利�?護的媒體文件。\n"
#define MSGTR_MPDEMUX_MMST_UnknownCmd "未知命令 %02x\n"
#define MSGTR_MPDEMUX_MMST_GetMediaPacketErr "get_media_packet 錯誤 : %s\n"
#define MSGTR_MPDEMUX_MMST_Connected "已連接\n"

// asf_streaming.c

#define MSGTR_MPDEMUX_ASF_StreamChunkSize2Small "啊…… stream_chunck 大�?太�?了: %d\n"
#define MSGTR_MPDEMUX_ASF_SizeConfirmMismatch "size_confirm �?匹�?!: %d %d\n"
#define MSGTR_MPDEMUX_ASF_WarnDropHeader "警告: 丢掉頭部????\n"
#define MSGTR_MPDEMUX_ASF_ErrorParsingChunkHeader "解�?�?�塊頭部時發生錯誤\n"
#define MSGTR_MPDEMUX_ASF_NoHeaderAtFirstChunk "没�?�到作為第一個�?�塊的頭部!!!!\n"
#define MSGTR_MPDEMUX_ASF_BufferMallocFailed "�?能分�? %d 字節的緩�?。\n"
#define MSGTR_MPDEMUX_ASF_ErrReadingNetworkStream "讀網絡�?時發生錯誤。\n"
#define MSGTR_MPDEMUX_ASF_ErrChunk2Small "錯誤: �?�塊太�?。\n"
#define MSGTR_MPDEMUX_ASF_ErrSubChunkNumberInvalid "錯誤: �?�?�塊號無效。\n"
#define MSGTR_MPDEMUX_ASF_Bandwidth2SmallCannotPlay "帶寬太�?, 文件�?能播放!\n"
#define MSGTR_MPDEMUX_ASF_Bandwidth2SmallDeselectedAudio "帶寬太�?, �?�消�?�定音頻�?。\n"
#define MSGTR_MPDEMUX_ASF_Bandwidth2SmallDeselectedVideo "帶寬太�?, �?�消�?�定視頻�?。\n"
#define MSGTR_MPDEMUX_ASF_InvalidLenInHeader "無效的 ASF 頭部長度!\n"
#define MSGTR_MPDEMUX_ASF_ErrReadingChunkHeader "讀�?�塊頭部時發生錯誤。\n"
#define MSGTR_MPDEMUX_ASF_ErrChunkBiggerThanPacket "錯誤: chunk_size > packet_size\n"
#define MSGTR_MPDEMUX_ASF_ErrReadingChunk "讀�?�塊時發生錯誤。\n"
#define MSGTR_MPDEMUX_ASF_ASFRedirector "=====> ASF 轉嚮器\n"
#define MSGTR_MPDEMUX_ASF_InvalidProxyURL "無效的代�?� URL\n"
#define MSGTR_MPDEMUX_ASF_UnknownASFStreamType "未知的 ASF �?類型\n"
#define MSGTR_MPDEMUX_ASF_Failed2ParseHTTPResponse "解�? HTTP 響應失敗。\n"
#define MSGTR_MPDEMUX_ASF_ServerReturn "�?務器返回 %d:%s\n"
#define MSGTR_MPDEMUX_ASF_ASFHTTPParseWarnCuttedPragma "ASF HTTP 解�?警告 : Pragma %s 被從 %zu 字節切到 %zu\n"
#define MSGTR_MPDEMUX_ASF_SocketWriteError "Socket 寫錯誤: %s\n"
#define MSGTR_MPDEMUX_ASF_HeaderParseFailed "解�?頭部失敗。\n"
#define MSGTR_MPDEMUX_ASF_NoStreamFound "找�?到�?。\n"
#define MSGTR_MPDEMUX_ASF_UnknownASFStreamingType "未知 ASF �?類型\n"
#define MSGTR_MPDEMUX_ASF_InfoStreamASFURL "STREAM_ASF, URL: %s\n"
#define MSGTR_MPDEMUX_ASF_StreamingFailed "失敗, 正在退出。\n"

// audio_in.c

#define MSGTR_MPDEMUX_AUDIOIN_ErrReadingAudio "\n讀音頻錯誤: %s\n"
#define MSGTR_MPDEMUX_AUDIOIN_XRUNSomeFramesMayBeLeftOut "從交�?��?�行中�?�複, �?些幀�?�能丢失了!\n"
#define MSGTR_MPDEMUX_AUDIOIN_ErrFatalCannotRecover "致命錯誤, 無法�?�複!\n"
#define MSGTR_MPDEMUX_AUDIOIN_NotEnoughSamples "\n音頻采様�?够!\n"

// aviheader.c

#define MSGTR_MPDEMUX_AVIHDR_EmptyList "**空列表?!\n"
#define MSGTR_MPDEMUX_AVIHDR_WarnNotExtendedAVIHdr "** 警告: 這�?是擴展的 AVI 頭部..\n"
#define MSGTR_MPDEMUX_AVIHDR_BuildingODMLidx "AVI: ODML: 建造 ODML 索引 (%d superindexchunks)。\n"
#define MSGTR_MPDEMUX_AVIHDR_BrokenODMLfile "AVI: ODML: 檢測到�??壞的(�?完整的?)文件。將使用傳統的索引。\n"
#define MSGTR_MPDEMUX_AVIHDR_CantReadIdxFile "�?能讀索引文件 %s: %s\n"
#define MSGTR_MPDEMUX_AVIHDR_NotValidMPidxFile "%s �?是有效的 MPlayer 索引文件。\n"
#define MSGTR_MPDEMUX_AVIHDR_FailedMallocForIdxFile "無法為來自 %s 的索引數據分�?内存。\n"
#define MSGTR_MPDEMUX_AVIHDR_PrematureEOF "�?�早�?�?�的索引文件 %s\n"
#define MSGTR_MPDEMUX_AVIHDR_IdxFileLoaded "�?載索引文件: %s\n"
#define MSGTR_MPDEMUX_AVIHDR_GeneratingIdx "正在生�?索引: %3lu %s     \r"
#define MSGTR_MPDEMUX_AVIHDR_IdxGeneratedForHowManyChunks "AVI: 為 %d �?�塊生�?索引表!\n"
#define MSGTR_MPDEMUX_AVIHDR_Failed2WriteIdxFile "無法寫索引文件 %s: %s\n"
#define MSGTR_MPDEMUX_AVIHDR_IdxFileSaved "已�?存索引文件: %s\n"

// cache2.c

#define MSGTR_MPDEMUX_CACHE2_NonCacheableStream "\r此�?�?�?�緩存。\n"
#define MSGTR_MPDEMUX_CACHE2_ReadFileposDiffers "!!! read_filepos �?�?�!!! 請報告此錯誤...\n"

// cdda.c

#define MSGTR_MPDEMUX_CDDA_CantOpenCDDADevice "打�?開 CDDA 設備。\n"
#define MSGTR_MPDEMUX_CDDA_CantOpenDisc "打�?開盤。\n"
#define MSGTR_MPDEMUX_CDDA_AudioCDFoundWithNTracks "發�?�音頻 CD，共 %d 音軌。\n"

// cddb.c

#define MSGTR_MPDEMUX_CDDB_FailedToReadTOC "讀�?� TOC 失敗。\n"
#define MSGTR_MPDEMUX_CDDB_FailedToOpenDevice "打開 %s 設備失敗。\n"
#define MSGTR_MPDEMUX_CDDB_NotAValidURL "�?是有效的 URL\n"
#define MSGTR_MPDEMUX_CDDB_FailedToSendHTTPRequest "發�? HTTP 請求失敗。\n"
#define MSGTR_MPDEMUX_CDDB_FailedToReadHTTPResponse "讀�?� HTTP 響應失敗。\n"
#define MSGTR_MPDEMUX_CDDB_HTTPErrorNOTFOUND "没有發�?�。\n"
#define MSGTR_MPDEMUX_CDDB_HTTPErrorUnknown "未知錯誤代碼\n"
#define MSGTR_MPDEMUX_CDDB_NoCacheFound "找�?到緩存。\n"
#define MSGTR_MPDEMUX_CDDB_NotAllXMCDFileHasBeenRead "没有讀出所有的 xmcd 文件。\n"
#define MSGTR_MPDEMUX_CDDB_FailedToCreateDirectory "創建目録 %s 失敗。\n"
#define MSGTR_MPDEMUX_CDDB_NotAllXMCDFileHasBeenWritten "没有寫入所有的 xmcd 文件。\n"
#define MSGTR_MPDEMUX_CDDB_InvalidXMCDDatabaseReturned "返回了無效的 xmcd 數據庫文件。\n"
#define MSGTR_MPDEMUX_CDDB_UnexpectedFIXME "�?外。請修複\n"
#define MSGTR_MPDEMUX_CDDB_UnhandledCode "未處�?�的代碼\n"
#define MSGTR_MPDEMUX_CDDB_UnableToFindEOL "無法找到行�?�?�。\n"
#define MSGTR_MPDEMUX_CDDB_ParseOKFoundAlbumTitle "解�?完�?，找到: %s\n"
#define MSGTR_MPDEMUX_CDDB_AlbumNotFound "没發�?�專輯。\n"
#define MSGTR_MPDEMUX_CDDB_ServerReturnsCommandSyntaxErr "�?務器返回: 命令語法錯誤\n"
#define MSGTR_MPDEMUX_CDDB_NoSitesInfoAvailable "没有�?�用的站點信�?�。\n"
#define MSGTR_MPDEMUX_CDDB_FailedToGetProtocolLevel "�?�得�?�議級别失敗。\n"
#define MSGTR_MPDEMUX_CDDB_NoCDInDrive "驅動器�?没有 CD。\n"

// cue_read.c

#define MSGTR_MPDEMUX_CUEREAD_UnexpectedCuefileLine "[bincue] �?外的 cue 文件行: %s\n"
#define MSGTR_MPDEMUX_CUEREAD_BinFilenameTested "[bincue] bin 文件�??測試: %s\n"
#define MSGTR_MPDEMUX_CUEREAD_CannotFindBinFile "[bincue] 找�?到 bin 文件 - 正在放棄。\n"
#define MSGTR_MPDEMUX_CUEREAD_UsingBinFile "[bincue] 正在使用 bin 文件 %s。\n"
#define MSGTR_MPDEMUX_CUEREAD_UnknownModeForBinfile "[bincue] 未知的 bin 文件模�?。�?應該發生。正在�?�止。\n"
#define MSGTR_MPDEMUX_CUEREAD_CannotOpenCueFile "[bincue] 打�?開 %s。\n"
#define MSGTR_MPDEMUX_CUEREAD_ErrReadingFromCueFile "[bincue] 讀�?� %s 出錯\n"
#define MSGTR_MPDEMUX_CUEREAD_ErrGettingBinFileSize "[bincue] 得到 bin 文件大�?時出錯。\n"
#define MSGTR_MPDEMUX_CUEREAD_InfoTrackFormat "音軌 %02d:  format=%d  %02d:%02d:%02d\n"
#define MSGTR_MPDEMUX_CUEREAD_UnexpectedBinFileEOF "[bincue] �?外的 bin 文件�?�?�\n"
#define MSGTR_MPDEMUX_CUEREAD_CannotReadNBytesOfPayload "[bincue] 無法讀�?��?載的 %d 字節。\n"
#define MSGTR_MPDEMUX_CUEREAD_CueStreamInfo_FilenameTrackTracksavail "CUE stream_open, filename=%s, track=%d, �?�用音軌: %d -> %d\n"

// network.c

#define MSGTR_MPDEMUX_NW_UnknownAF "未知地�?��? %d\n"
#define MSGTR_MPDEMUX_NW_ResolvingHostForAF "正在解�? %s (為 %s)...\n"
#define MSGTR_MPDEMUX_NW_CantResolv "�?能為 %s 解�?�??字: %s\n"
#define MSGTR_MPDEMUX_NW_ConnectingToServer "正在連接到�?務器 %s[%s]: %d...\n"
#define MSGTR_MPDEMUX_NW_CantConnect2Server "連接�?務器失敗: %s\n"
#define MSGTR_MPDEMUX_NW_SelectFailed "�?�擇失敗。\n"
#define MSGTR_MPDEMUX_NW_ConnTimeout "連接超時\n"
#define MSGTR_MPDEMUX_NW_GetSockOptFailed "getsockopt 失敗: %s\n"
#define MSGTR_MPDEMUX_NW_ConnectError "連接錯誤: %s\n"
#define MSGTR_MPDEMUX_NW_InvalidProxySettingTryingWithout "無效的代�?�設置... 試�?��?用代�?�。\n"
#define MSGTR_MPDEMUX_NW_CantResolvTryingWithoutProxy "�?能為 AF_INET 解�?�?�程主機�??。試�?��?用代�?�。\n"
#define MSGTR_MPDEMUX_NW_ErrSendingHTTPRequest "發�? HTTP 請求時發生錯誤: 没有發出所有請求。\n"
#define MSGTR_MPDEMUX_NW_ReadFailed "讀失敗。\n"
#define MSGTR_MPDEMUX_NW_Read0CouldBeEOF "http_read_response 讀進 0 (如: EOF)。\n"
#define MSGTR_MPDEMUX_NW_AuthFailed "�?證失敗。請使用 -user 和 -passwd �?�項來指定你的\n"\
"用户�??/密碼, 以便�??供給一組 URL, 或者使用 URL 格�?:\n"\
"http://username:password@hostname/file\n"
#define MSGTR_MPDEMUX_NW_AuthRequiredFor "%s 需�?�?證\n"
#define MSGTR_MPDEMUX_NW_AuthRequired "需�?�?證。\n"
#define MSGTR_MPDEMUX_NW_NoPasswdProvidedTryingBlank "没有給定密碼, 試�?�使用空密碼。\n"
#define MSGTR_MPDEMUX_NW_ErrServerReturned "�?務器返回 %d: %s\n"
#define MSGTR_MPDEMUX_NW_CacheSizeSetTo "緩存大�?設為 %d K字節\n"

// demux_audio.c

#define MSGTR_MPDEMUX_AUDIO_UnknownFormat "音頻分路器: 未知格�? %d。\n"

// demux_demuxers.c

#define MSGTR_MPDEMUX_DEMUXERS_FillBufferError "fill_buffer 錯誤: 分路器錯誤: �?是 vd, ad 或 sd。\n"

// demux_mkv.c
#define MSGTR_MPDEMUX_MKV_ZlibInitializationFailed "[mkv] zlib �?始化失敗。\n"
#define MSGTR_MPDEMUX_MKV_ZlibDecompressionFailed "[mkv] zlib 解壓失敗。\n"
#define MSGTR_MPDEMUX_MKV_LzoInitializationFailed "[mkv] lzo �?始化失敗。\n"
#define MSGTR_MPDEMUX_MKV_LzoDecompressionFailed "[mkv] lzo 解壓失敗。\n"
#define MSGTR_MPDEMUX_MKV_TrackEncrypted "[mkv] 軌迹號 %u 已加密但解密還没\n[mkv] 實�?�。跳�?�軌迹。\n"
#define MSGTR_MPDEMUX_MKV_UnknownContentEncoding "[mkv] 軌迹 %u 的内容編碼類型未知。跳�?�軌迹。\n"
#define MSGTR_MPDEMUX_MKV_UnknownCompression "[mkv] 軌迹 %u 已壓縮, 用了未知的/�?支�?的壓縮\n[mkv] 算法(%u)。跳�?�軌迹。\n"
#define MSGTR_MPDEMUX_MKV_ZlibCompressionUnsupported "[mkv] 軌迹 %u 已用 zlib 壓縮但 mplayer 還没編譯�?\n[mkv] 支�? zlib 壓縮。跳�?�軌迹。\n"
#define MSGTR_MPDEMUX_MKV_TrackIDName "[mkv] 軌迹 ID %u: %s (%s) \"%s\", %s\n"
#define MSGTR_MPDEMUX_MKV_TrackID "[mkv] 軌迹 ID %u: %s (%s), %s\n"
#define MSGTR_MPDEMUX_MKV_UnknownCodecID "[mkv] 未知的/�?支�?的 CodecID (%s) 或者缺少的/壞的 CodecPrivate\n[mkv] 數據(軌迹 %u)。\n"
#define MSGTR_MPDEMUX_MKV_FlacTrackDoesNotContainValidHeaders "[mkv] FLAC 軌迹没�?�有效的頭部。\n"
#define MSGTR_MPDEMUX_MKV_UnknownAudioCodec "[mkv] 未知的/�?支�?的音頻編解碼器 ID '%s' �?于軌迹 %u 或者缺少的/有缺點的\n[mkv] 編解碼器�?有數據。\n"
#define MSGTR_MPDEMUX_MKV_SubtitleTypeNotSupported "[mkv] �?支�?字幕類型 '%s'。\n"
#define MSGTR_MPDEMUX_MKV_WillPlayVideoTrack "[mkv] 將播放視頻軌迹 %u。\n"
#define MSGTR_MPDEMUX_MKV_NoVideoTrackFound "[mkv] 没有找到/所�?的視頻軌迹。\n"
#define MSGTR_MPDEMUX_MKV_NoAudioTrackFound "[mkv] 没有找到/所�?的音頻軌迹。\n"
#define MSGTR_MPDEMUX_MKV_WillDisplaySubtitleTrack "[mkv] 將播放字幕軌迹 %u。\n"
#define MSGTR_MPDEMUX_MKV_NoBlockDurationForSubtitleTrackFound "[mkv] 警告: �?于所找到的字幕軌迹没有 BlockDuration。\n"
#define MSGTR_MPDEMUX_MKV_TooManySublines "[mkv] Warning: 太多的字幕�?渲染, 跳�?�。\n"
#define MSGTR_MPDEMUX_MKV_TooManySublinesSkippingAfterFirst "\n[mkv] 警告: 太多的字幕�?渲染, %i 以後跳�?�。n"

// demux_nuv.c

#define MSGTR_MPDEMUX_NUV_NoVideoBlocksInFile "文件中没有視頻塊。\n"

// demux_xmms.c

#define MSGTR_MPDEMUX_XMMS_FoundPlugin "找到�?�件: %s (%s)。\n"
#define MSGTR_MPDEMUX_XMMS_ClosingPlugin "關閉�?�件: %s。\n"

// ========================== LIBMPMENU ===================================

// common

#define MSGTR_LIBMENU_NoEntryFoundInTheMenuDefinition "[MENU] �?�單定義中没有找到�?目。\n"

// libmenu/menu.c
#define MSGTR_LIBMENU_SyntaxErrorAtLine "[MENU] 語法錯誤: 行 %d\n"
#define MSGTR_LIBMENU_MenuDefinitionsNeedANameAttrib "[MENU] �?�單定義需�?�??稱屬性 (行 %d)。\n"
#define MSGTR_LIBMENU_BadAttrib "[MENU] 錯誤屬性 %s=%s，在�?�單 '%s' 的 %d 行\n"
#define MSGTR_LIBMENU_UnknownMenuType "[MENU] 未知�?�單類型 '%s' (行 %d)\n"
#define MSGTR_LIBMENU_CantOpenConfigFile "[MENU] 打�?開�?�單�?置文件: %s\n"
#define MSGTR_LIBMENU_ConfigFileIsTooBig "[MENU] �?置文件�?�長 (> %d KB)\n"
#define MSGTR_LIBMENU_ConfigFileIsEmpty "[MENU] �?置文件為空。\n"
#define MSGTR_LIBMENU_MenuNotFound "[MENU] 没找到�?�單 %s。\n"
#define MSGTR_LIBMENU_MenuInitFailed "[MENU] �?�單 '%s': �?始化失敗。\n"
#define MSGTR_LIBMENU_UnsupportedOutformat "[MENU] 輸出格�?�?支�?!!!!\n"

// libmenu/menu_cmdlist.c
#define MSGTR_LIBMENU_ListMenuEntryDefinitionsNeedAName "[MENU] 列表�?�單�?目的定義需�?�??稱 (行 %d)。\n"
#define MSGTR_LIBMENU_ListMenuNeedsAnArgument "[MENU] 列表�?�單需�?�?�數。\n"

// libmenu/menu_console.c
#define MSGTR_LIBMENU_WaitPidError "[MENU] Waitpid 錯誤: %s。\n"
#define MSGTR_LIBMENU_SelectError "[MENU] Select 錯誤。\n"
#define MSGTR_LIBMENU_ReadErrorOnChildFD "[MENU] �?進程的文件�??述符讀�?�錯誤: %s。\n"
#define MSGTR_LIBMENU_ConsoleRun "[MENU] 終端�?�行: %s ...\n"
#define MSGTR_LIBMENU_AChildIsAlreadyRunning "[MENU] �?進程已經�?�行。\n"
#define MSGTR_LIBMENU_ForkFailed "[MENU] Fork 失敗!!!\n"
#define MSGTR_LIBMENU_WriteError "[MENU] write 錯誤\n"

// libmenu/menu_filesel.c
#define MSGTR_LIBMENU_OpendirError "[MENU] opendir 錯誤: %s\n"
#define MSGTR_LIBMENU_ReallocError "[MENU] realloc 錯誤: %s\n"
#define MSGTR_LIBMENU_MallocError "[MENU] 内存分�?錯誤: %s\n"
#define MSGTR_LIBMENU_ReaddirError "[MENU] readdir 錯誤: %s\n"
#define MSGTR_LIBMENU_CantOpenDirectory "[MENU] 打�?開目録 %s。\n"

// libmenu/menu_param.c
#define MSGTR_LIBMENU_SubmenuDefinitionNeedAMenuAttribut "[MENU] �?�?�單定義需�? 'menu' 屬性。\n"
#define MSGTR_LIBMENU_PrefMenuEntryDefinitionsNeed "[MENU] 首�?�項�?�單�?目的定義需�?有效的 'property' 屬性 (行 %d)。\n"
#define MSGTR_LIBMENU_PrefMenuNeedsAnArgument "[MENU] 首�?�項�?�單需�?�?�數。\n"

// libmenu/menu_pt.c
#define MSGTR_LIBMENU_CantfindTheTargetItem "[MENU] 找�?到目標項 ????\n"
#define MSGTR_LIBMENU_FailedToBuildCommand "[MENU] 生�?命令失敗: %s。\n"

// libmenu/menu_txt.c
#define MSGTR_LIBMENU_MenuTxtNeedATxtFileName "[MENU] 文本�?�單需�?文本文件�??(�?�數文件)。\n"
#define MSGTR_LIBMENU_MenuTxtCantOpen "[MENU] 打�?開 %s。\n"
#define MSGTR_LIBMENU_WarningTooLongLineSplitting "[MENU] 警告, 行�?�長. 分割之。\n"
#define MSGTR_LIBMENU_ParsedLines "[MENU] 解�?了行 %d。\n"

// libmenu/vf_menu.c
#define MSGTR_LIBMENU_UnknownMenuCommand "[MENU] 未知命令: '%s'。\n"
#define MSGTR_LIBMENU_FailedToOpenMenu "[MENU] 打開�?�單失敗: '%s'。\n"

// ========================== LIBMPCODECS ===================================

// libmpcodecs/ad_libdv.c
#define MSGTR_MPCODECS_AudioFramesizeDiffers "[AD_LIBDV] 警告! 音頻幀大�?�?一致! read=%d  hdr=%d。\n"

// libmpcodecs/vd_dmo.c vd_dshow.c vd_vfw.c
#define MSGTR_MPCODECS_CouldntAllocateImageForCinepakCodec "[VD_DMO] 無法為 cinepak 編解碼器分�?圖�?。\n"

// libmpcodecs/vd_ffmpeg.c
#define MSGTR_MPCODECS_XVMCAcceleratedCodec "[VD_FFMPEG] XVMC 加速的編解碼器。\n"
#define MSGTR_MPCODECS_ArithmeticMeanOfQP "[VD_FFMPEG] QP 的算術平�?�值: %2.4f, QP 的調和平�?�值: %2.4f\n"
#define MSGTR_MPCODECS_DRIFailure "[VD_FFMPEG] DRI 失敗。\n"
#define MSGTR_MPCODECS_CouldntAllocateImageForCodec "[VD_FFMPEG] 無法為編解碼器分�?圖�?。\n"
#define MSGTR_MPCODECS_XVMCAcceleratedMPEG2 "[VD_FFMPEG] XVMC-加速的 MPEG-2。\n"
#define MSGTR_MPCODECS_TryingPixfmt "[VD_FFMPEG] 嘗試 pixfmt=%d。\n"
#define MSGTR_MPCODECS_McGetBufferShouldWorkOnlyWithXVMC "[VD_FFMPEG] Mc_get_buffer 祇能用于 XVMC 加速!!"
#define MSGTR_MPCODECS_UnexpectedInitVoError "[VD_FFMPEG] Init_vo �?外錯誤。\n"
#define MSGTR_MPCODECS_UnrecoverableErrorRenderBuffersNotTaken "[VD_FFMPEG] 無法�?�複的錯誤, 渲染緩�?無法�?�得。\n"
#define MSGTR_MPCODECS_OnlyBuffersAllocatedByVoXvmcAllowed "[VD_FFMPEG] 祇�?許 vo_xvmc 分�?的緩�?。\n"

// libmpcodecs/ve_lavc.c
#define MSGTR_MPCODECS_HighQualityEncodingSelected "[VE_LAVC] 已�?�高�?質編碼 (�?�實時)!\n"
#define MSGTR_MPCODECS_UsingConstantQscale "[VE_LAVC] 使用常數的 qscale = %f (VBR)。\n"

// libmpcodecs/ve_raw.c
#define MSGTR_MPCODECS_OutputWithFourccNotSupported "[VE_RAW] �?支�? FourCC [%x] 的 raw 輸出!\n"
#define MSGTR_MPCODECS_NoVfwCodecSpecified "[VE_RAW] 未指定需�?的 VfW 編解碼器!!\n"

// libmpcodecs/vf_crop.c
#define MSGTR_MPCODECS_CropBadPositionWidthHeight "[CROP] 錯誤的�?置/寬度/高度 - 切割�?�域在原始圖�?外!\n"

// libmpcodecs/vf_cropdetect.c
#define MSGTR_MPCODECS_CropArea "[CROP] 切割�?�域: X: %d..%d  Y: %d..%d  (-vf crop=%d:%d:%d:%d)。\n"

// libmpcodecs/vf_format.c, vf_palette.c, vf_noformat.c
#define MSGTR_MPCODECS_UnknownFormatName "[VF_FORMAT] 未知格�?�??: '%s'。\n"

// libmpcodecs/vf_framestep.c vf_noformat.c vf_palette.c vf_tile.c
#define MSGTR_MPCODECS_ErrorParsingArgument "[VF_FRAMESTEP] 解�?�?�數錯誤。\n"

// libmpcodecs/ve_vfw.c
#define MSGTR_MPCODECS_CompressorType "壓縮類型: %.4lx\n"
#define MSGTR_MPCODECS_CompressorSubtype "副壓縮類型: %.4lx\n"
#define MSGTR_MPCODECS_CompressorFlags "壓縮標記: %lu, 版本 %lu, ICM 版本: %lu\n"
#define MSGTR_MPCODECS_Flags "標記:"
#define MSGTR_MPCODECS_Quality "�?質"

// libmpcodecs/vf_expand.c
#define MSGTR_MPCODECS_FullDRNotPossible "無法完全使用 DR, 嘗試使用 SLICES!\n"
#define MSGTR_MPCODECS_WarnNextFilterDoesntSupportSlices  "警告! 下一個濾�?��?支�? SLICES, 等�?� sig11...\n"
#define MSGTR_MPCODECS_FunWhydowegetNULL "為什麼我們得到了 NULL??\n"

// libmpcodecs/vf_test.c, vf_yuy2.c, vf_yvu9.c
#define MSGTR_MPCODECS_WarnNextFilterDoesntSupport "下一個濾�?�/視頻輸出�?支�? %s :(\n"

// ================================== LIBMPVO ====================================

// mga_template.c

#define MSGTR_LIBVO_MGA_ErrorInConfigIoctl "[MGA] mga_vid_config ioctl 錯誤 (mga_vid.o 版本錯誤?)"
#define MSGTR_LIBVO_MGA_CouldNotGetLumaValuesFromTheKernelModule "[MGA] 無法在内核模塊中�?�得 luma 值!\n"
#define MSGTR_LIBVO_MGA_CouldNotSetLumaValuesFromTheKernelModule "[MGA] 無法在内核模塊中設置 luma 值!\n"
#define MSGTR_LIBVO_MGA_ScreenWidthHeightUnknown "[MGA] �?幕寬度/高度未知!\n"
#define MSGTR_LIBVO_MGA_InvalidOutputFormat "[MGA] 無效的輸出格�? %0X\n"
#define MSGTR_LIBVO_MGA_IncompatibleDriverVersion "[MGA] 你的 mga_vid 驅動版本與 MPlayer 的版本�?兼容!\n"
#define MSGTR_LIBVO_MGA_CouldntOpen "[MGA] 打�?開: %s\n"
#define MSGTR_LIBVO_MGA_ResolutionTooHigh "[MGA] 原分辨率至少有一維大于 1023x1023。請用軟件或用 -lavdopts lowres=1 �?新縮放\n"

// libvo/vesa_lvo.c

#define MSGTR_LIBVO_VESA_ThisBranchIsNoLongerSupported "[VESA_LVO] 這個分支已經�?�?維護。\n[VESA_LVO] 請使用 -vo vesa:vidix。\n"
#define MSGTR_LIBVO_VESA_CouldntOpen "[VESA_LVO] 打�?開: '%s'\n"
#define MSGTR_LIBVO_VESA_InvalidOutputFormat "[VESA_LVI] 無效的輸出格�?: %s(%0X)\n"
#define MSGTR_LIBVO_VESA_IncompatibleDriverVersion "[VESA_LVO] 你的 fb_vid 驅動版本與 MPlayer 的版本�?兼容!\n"

// libvo/vo_3dfx.c

#define MSGTR_LIBVO_3DFX_Only16BppSupported "[VO_3DFX] 祇支�? 16bpp!"
#define MSGTR_LIBVO_3DFX_VisualIdIs "[VO_3DFX] �?�視 ID 是  %lx。\n"
#define MSGTR_LIBVO_3DFX_UnableToOpenDevice "[VO_3DFX] 無法打開 /dev/3dfx。\n"
#define MSGTR_LIBVO_3DFX_Error "[VO_3DFX] 錯誤: %d。\n"
#define MSGTR_LIBVO_3DFX_CouldntMapMemoryArea "[VO_3DFX] 没能映射 3dfx 内存�?�域: %p,%p,%d。\n"
#define MSGTR_LIBVO_3DFX_DisplayInitialized "[VO_3DFX] �?始化: %p。\n"
#define MSGTR_LIBVO_3DFX_UnknownSubdevice "[VO_3DFX] 未知�?設備: %s。\n"

// libvo/aspect.c
#define MSGTR_LIBVO_ASPECT_NoSuitableNewResFound "[ASPECT] 警告: 無法找到新的�?��?�的分辨率!\n"
#define MSGTR_LIBVO_ASPECT_NoNewSizeFoundThatFitsIntoRes "[ASPECT] 錯誤: 無法找到�?��?�分辨率的新尺寸!\n"

// libvo/vo_dxr3.c

#define MSGTR_LIBVO_DXR3_UnableToLoadNewSPUPalette "[VO_DXR3] 無法載入新的 SPU 調色�?�!\n"
#define MSGTR_LIBVO_DXR3_UnableToSetPlaymode "[VO_DXR3] 無法設置播放模�?!\n"
#define MSGTR_LIBVO_DXR3_UnableToSetSubpictureMode "[VO_DXR3] 無法設置 subpicture 模�?!\n"
#define MSGTR_LIBVO_DXR3_UnableToGetTVNorm "[VO_DXR3] 無法�?��?�電視製�?!\n"
#define MSGTR_LIBVO_DXR3_AutoSelectedTVNormByFrameRate "[VO_DXR3] 利用幀速率自動�?�擇電視製�?: "
#define MSGTR_LIBVO_DXR3_UnableToSetTVNorm "[VO_DXR3] 無法設置電視製�?!\n"
#define MSGTR_LIBVO_DXR3_SettingUpForNTSC "[VO_DXR3] 設置 NTSC。\n"
#define MSGTR_LIBVO_DXR3_SettingUpForPALSECAM "[VO_DXR3] 設置 PAL/SECAM。\n"
#define MSGTR_LIBVO_DXR3_SettingAspectRatioTo43 "[VO_DXR3] 寬高比設為 4:3。\n"
#define MSGTR_LIBVO_DXR3_SettingAspectRatioTo169 "[VO_DXR3] 寬高比設為 16:9。\n"
#define MSGTR_LIBVO_DXR3_OutOfMemory "[VO_DXR3] 内存耗盡\n"
#define MSGTR_LIBVO_DXR3_UnableToAllocateKeycolor "[VO_DXR3] 無法分�? keycolor!\n"
#define MSGTR_LIBVO_DXR3_UnableToAllocateExactKeycolor "[VO_DXR3] 無法精確分�? keycolor, 使用最接近的匹�? (0x%lx)。\n"
#define MSGTR_LIBVO_DXR3_Uninitializing "[VO_DXR3] �??�?始化(釋放資�?)。\n"
#define MSGTR_LIBVO_DXR3_FailedRestoringTVNorm "[VO_DXR3] �?�複電視製�?失敗!\n"
#define MSGTR_LIBVO_DXR3_EnablingPrebuffering "[VO_DXR3] 啟用�?緩�?。\n"
#define MSGTR_LIBVO_DXR3_UsingNewSyncEngine "[VO_DXR3] 使用新的�?�步引擎。\n"
#define MSGTR_LIBVO_DXR3_UsingOverlay "[VO_DXR3] 使用覆蓋。\n"
#define MSGTR_LIBVO_DXR3_ErrorYouNeedToCompileMplayerWithX11 "[VO_DXR3] 錯誤: 覆蓋需�?安�? X11 的庫和頭文件後編譯。\n"
#define MSGTR_LIBVO_DXR3_WillSetTVNormTo "[VO_DXR3] 將電視製�?設置為: "
#define MSGTR_LIBVO_DXR3_AutoAdjustToMovieFrameRatePALPAL60 "自動調節電影的幀速率 (PAL/PAL-60)"
#define MSGTR_LIBVO_DXR3_AutoAdjustToMovieFrameRatePALNTSC "自動調節電影的幀速率 (PAL/NTSC)"
#define MSGTR_LIBVO_DXR3_UseCurrentNorm "使用當�?製�?。"
#define MSGTR_LIBVO_DXR3_UseUnknownNormSuppliedCurrentNorm "未知製�?，使用當�?製�?。"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingTrying "[VO_DXR3] 打開 %s 寫入錯誤, 嘗試 /dev/em8300。\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingTryingMV "[VO_DXR3] 打開 %s 寫入錯誤, 嘗試 /dev/em8300_mv。\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingAsWell "[VO_DXR3] 打開 /dev/em8300 寫入錯誤!\n跳出。\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingAsWellMV "[VO_DXR3] 打開 /dev/em8300_mv 寫入錯誤!\n跳出。\n"
#define MSGTR_LIBVO_DXR3_Opened "[VO_DXR3] 打開: %s。\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingTryingSP "[VO_DXR3] 打開 %s 寫入錯誤, 嘗試 /dev/em8300_sp。\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingAsWellSP "[VO_DXR3] 打開 /dev/em8300_sp 寫入錯誤!\n跳出。\n"
#define MSGTR_LIBVO_DXR3_UnableToOpenDisplayDuringHackSetup "[VO_DXR3] 在 overlay hack 設置中無法打開顯示設備!\n"
#define MSGTR_LIBVO_DXR3_UnableToInitX11 "[VO_DXR3] 無法�?始化 X11!\n"
#define MSGTR_LIBVO_DXR3_FailedSettingOverlayAttribute "[VO_DXR3] 設置覆蓋屬性失敗。\n"
#define MSGTR_LIBVO_DXR3_FailedSettingOverlayScreen "[VO_DXR3] 設置覆蓋�?幕失敗!\n退出。\n"
#define MSGTR_LIBVO_DXR3_FailedEnablingOverlay "[VO_DXR3] 啟用覆蓋失敗!\n退出。\n"
#define MSGTR_LIBVO_DXR3_FailedSettingOverlayBcs "[VO_DXR3] 設置 overlay bcs 失敗!\n"
#define MSGTR_LIBVO_DXR3_FailedGettingOverlayYOffsetValues "[VO_DXR3] �?��?�覆蓋的 Y-�??移�?失敗!\n退出。\n"
#define MSGTR_LIBVO_DXR3_FailedGettingOverlayXOffsetValues "[VO_DXR3] �?��?�覆蓋的 X-�??移�?失敗!\n退出。\n"
#define MSGTR_LIBVO_DXR3_FailedGettingOverlayXScaleCorrection "[VO_DXR3] �?��?�覆蓋的 X-比例校正失敗!\n退出。\n"
#define MSGTR_LIBVO_DXR3_YOffset "[VO_DXR3] Y-�??移�?: %d。\n"
#define MSGTR_LIBVO_DXR3_XOffset "[VO_DXR3] X-�??移�?: %d。\n"
#define MSGTR_LIBVO_DXR3_XCorrection "[VO_DXR3] X-比例校正: %d。\n"
#define MSGTR_LIBVO_DXR3_FailedResizingOverlayWindow "[VO_DXR3] 設置覆蓋窗�?�大�?失敗!\n"
#define MSGTR_LIBVO_DXR3_FailedSetSignalMix "[VO_DXR3] 設置信號混�?�失敗!\n"

// libvo/vo_mga.c

#define MSGTR_LIBVO_MGA_AspectResized "[VO_MGA] aspect(): 改變大�?為 %dx%d。\n"
#define MSGTR_LIBVO_MGA_Uninit "[VO] �??�?始化(釋放資�?)!\n"

// libvo/vo_null.c

#define MSGTR_LIBVO_NULL_UnknownSubdevice "[VO_NULL] 未知�?設備: %s。\n"

// libvo/vo_png.c

#define MSGTR_LIBVO_PNG_Warning1 "[VO_PNG] 警告: 壓縮級别設置為 0, �?�用壓縮!\n"
#define MSGTR_LIBVO_PNG_Warning2 "[VO_PNG] 信�?�: 使用 -vo png:z=<n> 設置 0 到 9 的壓縮級别。\n"
#define MSGTR_LIBVO_PNG_Warning3 "[VO_PNG] 信�?�: (0 = �?壓縮, 1 = 最快，壓縮率最低 - 9 最好，最慢的壓縮)\n"
#define MSGTR_LIBVO_PNG_ErrorOpeningForWriting "\n[VO_PNG] 打開 '%s' 寫入錯誤!\n"
#define MSGTR_LIBVO_PNG_ErrorInCreatePng "[VO_PNG] create_png 錯誤。\n"

// libvo/vo_sdl.c

#define MSGTR_LIBVO_SDL_CouldntGetAnyAcceptableSDLModeForOutput "[VO_SDL] 無法�?�得�?�用的 SDL 輸出模�?。\n"
#define MSGTR_LIBVO_SDL_SetVideoModeFailed "[VO_SDL] set_video_mode: SDL_SetVideoMode 失敗: %s。\n"
#define MSGTR_LIBVO_SDL_MappingI420ToIYUV "[VO_SDL] I420 映射到 IYUV。\n"
#define MSGTR_LIBVO_SDL_UnsupportedImageFormat "[VO_SDL] �?支�?的圖�?格�? (0x%X)。\n"
#define MSGTR_LIBVO_SDL_InfoPleaseUseVmOrZoom "[VO_SDL] 信�?� - 請使用 -vm 或 -zoom 切�?�到最佳分辨率。\n"
#define MSGTR_LIBVO_SDL_FailedToSetVideoMode "[VO_SDL] 設置視頻模�?失敗: %s。\n"
#define MSGTR_LIBVO_SDL_CouldntCreateAYUVOverlay "[VO_SDL] 没能創建 YUV 覆蓋: %s。\n"
#define MSGTR_LIBVO_SDL_CouldntCreateARGBSurface "[VO_SDL] 没能創建 RGB 表�?�: %s。\n"
#define MSGTR_LIBVO_SDL_UsingDepthColorspaceConversion "[VO_SDL] 使用深度/顔色空間轉�?�, 這會�?慢速度 (%ibpp -> %ibpp)。\n"
#define MSGTR_LIBVO_SDL_UnsupportedImageFormatInDrawslice "[VO_SDL] draw_slice �?支�?的圖�?格�?, �?�係 MPlayer 的開發者!\n"
#define MSGTR_LIBVO_SDL_BlitFailed "[VO_SDL] Blit 失敗: %s。\n"
#define MSGTR_LIBVO_SDL_InitializationFailed "[VO_SDL] �?始化 SDL 失敗: %s。\n"
#define MSGTR_LIBVO_SDL_UsingDriver "[VO_SDL] 使用驅動: %s。\n"

// libvo/vobsub_vidix.c

#define MSGTR_LIBVO_SUB_VIDIX_CantStartPlayback "[VO_SUB_VIDIX] �?能開始播放: %s\n"
#define MSGTR_LIBVO_SUB_VIDIX_CantStopPlayback "[VO_SUB_VIDIX] �?能�?�止播放: %s\n"
#define MSGTR_LIBVO_SUB_VIDIX_InterleavedUvForYuv410pNotSupported "[VO_SUB_VIDIX] YUV410P �?支�?交錯的 UV。\n"
#define MSGTR_LIBVO_SUB_VIDIX_DummyVidixdrawsliceWasCalled "[VO_SUB_VIDIX] 調用 dummy vidix_draw_slice()。\n"
#define MSGTR_LIBVO_SUB_VIDIX_DummyVidixdrawframeWasCalled "[VO_SUB_VIDIX] 調用 dummy vidix_draw_frame()。\n"
#define MSGTR_LIBVO_SUB_VIDIX_UnsupportedFourccForThisVidixDriver "[VO_SUB_VIDIX] 此 VIDIX 驅動�?支�? FourCC: %x (%s)。\n"
#define MSGTR_LIBVO_SUB_VIDIX_VideoServerHasUnsupportedResolution "[VO_SUB_VIDIX] 視頻�?務器�?支�?分辨率 (%dx%d), 支�?的分辨率: %dx%d-%dx%d。\n"
#define MSGTR_LIBVO_SUB_VIDIX_VideoServerHasUnsupportedColorDepth "[VO_SUB_VIDIX] VIDIX �?支�?視頻�?務器的色深 (%d)。\n"
#define MSGTR_LIBVO_SUB_VIDIX_DriverCantUpscaleImage "[VO_SUB_VIDIX] VIDIX 驅動�?能放大圖�? (%d%d -> %d%d)。\n"
#define MSGTR_LIBVO_SUB_VIDIX_DriverCantDownscaleImage "[VO_SUB_VIDIX] VIDIX 驅動�?能縮�?圖�? (%d%d -> %d%d)。\n"
#define MSGTR_LIBVO_SUB_VIDIX_CantConfigurePlayback "[VO_SUB_VIDIX] �?能�?置回放: %s。\n"
#define MSGTR_LIBVO_SUB_VIDIX_YouHaveWrongVersionOfVidixLibrary "[VO_SUB_VIDIX] VIDIX 庫版本錯誤。\n"
#define MSGTR_LIBVO_SUB_VIDIX_CouldntFindWorkingVidixDriver "[VO_SUB_VIDIX] 無法找到能工作的 VIDIX 驅動。\n"
#define MSGTR_LIBVO_SUB_VIDIX_CouldntGetCapability "[VO_SUB_VIDIX] 無法�?�得兼容性: %s。\n"

// libvo/vo_svga.c

#define MSGTR_LIBVO_SVGA_ForcedVidmodeNotAvailable "[VO_SVGA] 鎖定的 vid_mode %d (%s) �?�?�用。\n"
#define MSGTR_LIBVO_SVGA_ForcedVidmodeTooSmall "[VO_SVGA] 鎖定的 vid_mode %d (%s) 太�?。\n"
#define MSGTR_LIBVO_SVGA_Vidmode "[VO_SVGA] Vid_mode: %d, %dx%d %dbpp。\n"
#define MSGTR_LIBVO_SVGA_VgasetmodeFailed "[VO_SVGA] Vga_setmode(%d) 失敗。\n"
#define MSGTR_LIBVO_SVGA_VideoModeIsLinearAndMemcpyCouldBeUsed "[VO_SVGA] 綫性的視頻模�?�?�以使用 memcpy �?作圖�?。\n"
#define MSGTR_LIBVO_SVGA_VideoModeHasHardwareAcceleration "[VO_SVGA] 硬件加速的視頻模�?�?�以使用 put_image。\n"
#define MSGTR_LIBVO_SVGA_IfItWorksForYouIWouldLikeToKnow "[VO_SVGA] 如果工作正常請告訴我。\n[VO_SVGA] (發�? `mplayer test.avi -v -v -v -v &> svga.log` 生�?的日志文件)。�?!\n"
#define MSGTR_LIBVO_SVGA_VideoModeHas "[VO_SVGA] 視頻模�?有 %d �?。\n"
#define MSGTR_LIBVO_SVGA_CenteringImageStartAt "[VO_SVGA] 圖�?居中。始于 (%d,%d)\n"
#define MSGTR_LIBVO_SVGA_UsingVidix "[VO_SVGA] 使用 VIDIX. w=%i h=%i  mw=%i mh=%i\n"

// libvo/vo_tdfxfb.c

#define MSGTR_LIBVO_TDFXFB_CantOpen "[VO_TDFXFB] 打�?開 %s: %s。\n"
#define MSGTR_LIBVO_TDFXFB_ProblemWithFbitgetFscreenInfo "[VO_TDFXFB] FBITGET_FSCREENINFO ioctl 出錯: %s。\n"
#define MSGTR_LIBVO_TDFXFB_ProblemWithFbitgetVscreenInfo "[VO_TDFXFB] FBITGET_VSCREENINFO ioctl 出錯: %s。\n"
#define MSGTR_LIBVO_TDFXFB_ThisDriverOnlySupports "[VO_TDFXFB] 此驅動僅支�? 3Dfx Banshee, Voodoo3 和 Voodoo 5。\n"
#define MSGTR_LIBVO_TDFXFB_OutputIsNotSupported "[VO_TDFXFB] %d bpp 輸出�?支�?。\n"
#define MSGTR_LIBVO_TDFXFB_CouldntMapMemoryAreas "[VO_TDFXFB] 無法映射内存�?�域: %s。\n"
#define MSGTR_LIBVO_TDFXFB_BppOutputIsNotSupported "[VO_TDFXFB] %d bpp 輸出�?支�? (應該永�?��?會發生)。\n"
#define MSGTR_LIBVO_TDFXFB_SomethingIsWrongWithControl "[VO_TDFXFB] Eik! control() 出錯。\n"
#define MSGTR_LIBVO_TDFXFB_NotEnoughVideoMemoryToPlay "[VO_TDFXFB] 没有足够的顯存播放此片，嘗試較低的分辨率。\n"
#define MSGTR_LIBVO_TDFXFB_ScreenIs "[VO_TDFXFB] �?幕 %dx%d 色深 %d bpp, 輸入 %dx%d 色深 %d bpp, 輸出 %dx%d。\n"

// libvo/vo_tdfx_vid.c

#define MSGTR_LIBVO_TDFXVID_Move "[VO_TDXVID] Move %d(%d) x %d => %d。\n"
#define MSGTR_LIBVO_TDFXVID_AGPMoveFailedToClearTheScreen "[VO_TDFXVID] AGP move 清除�?幕失敗。\n"
#define MSGTR_LIBVO_TDFXVID_BlitFailed "[VO_TDFXVID] Blit 失敗。\n"
#define MSGTR_LIBVO_TDFXVID_NonNativeOverlayFormatNeedConversion "[VO_TDFXVID] �?�本地支�?的覆蓋格�?需�?轉�?�。\n"
#define MSGTR_LIBVO_TDFXVID_UnsupportedInputFormat "[VO_TDFXVID] �?支�?的輸入格�? 0x%x。\n"
#define MSGTR_LIBVO_TDFXVID_OverlaySetupFailed "[VO_TDFXVID] 覆蓋設置失敗。\n"
#define MSGTR_LIBVO_TDFXVID_OverlayOnFailed "[VO_TDFXVID] 覆蓋打開失敗。\n"
#define MSGTR_LIBVO_TDFXVID_OverlayReady "[VO_TDFXVID] 覆蓋凖備完�?: %d(%d) x %d @ %d => %d(%d) x %d @ %d。\n"
#define MSGTR_LIBVO_TDFXVID_TextureBlitReady "[VO_TDFXVID] 紋�?� blit 凖備完�?: %d(%d) x %d @ %d => %d(%d) x %d @ %d。\n"
#define MSGTR_LIBVO_TDFXVID_OverlayOffFailed "[VO_TDFXVID] 覆蓋關閉失敗\n"
#define MSGTR_LIBVO_TDFXVID_CantOpen "[VO_TDFXVID] 打�?開 %s: %s。\n"
#define MSGTR_LIBVO_TDFXVID_CantGetCurrentCfg "[VO_TDFXVID] 没能�?�得當�?�?置: %s。\n"
#define MSGTR_LIBVO_TDFXVID_MemmapFailed "[VO_TDFXVID] Memmap 失敗 !!!!!\n"
#define MSGTR_LIBVO_TDFXVID_GetImageTodo "�?�得圖�?格�? todo。\n"
#define MSGTR_LIBVO_TDFXVID_AgpMoveFailed "[VO_TDFXVID] AGP move 失敗。\n"
#define MSGTR_LIBVO_TDFXVID_SetYuvFailed "[VO_TDFXVID] 設置 YUV 失敗。\n"
#define MSGTR_LIBVO_TDFXVID_AgpMoveFailedOnYPlane "[VO_TDFXVID] AGP move �?作 Y plane 失敗。\n"
#define MSGTR_LIBVO_TDFXVID_AgpMoveFailedOnUPlane "[VO_TDFXVID] AGP move �?作 U plane 失敗。\n"
#define MSGTR_LIBVO_TDFXVID_AgpMoveFailedOnVPlane "[VO_TDFXVID] AGP move �?作 V plane 失敗。\n"
#define MSGTR_LIBVO_TDFXVID_UnknownFormat "[VO_TDFXVID] 未知格�? 0x%x。\n"

// libvo/vo_tga.c

#define MSGTR_LIBVO_TGA_UnknownSubdevice "[VO_TGA] 未知�?設備: %s。\n"

// libvo/vo_vesa.c

#define MSGTR_LIBVO_VESA_FatalErrorOccurred "[VO_VESA] 發生致命錯誤! �?能繼續。\n"
#define MSGTR_LIBVO_VESA_UnknownSubdevice "[VO_VESA] 未知�?設備: '%s'。\n"
#define MSGTR_LIBVO_VESA_YouHaveTooLittleVideoMemory "[VO_VESA] 顯存太�?�?能支�?這個模�?:\n[VO_VESA] 需�?: %08lX �?�用: %08lX。\n"
#define MSGTR_LIBVO_VESA_YouHaveToSpecifyTheCapabilitiesOfTheMonitor "[VO_VESA] 你需�?設置顯示器的兼容性。�?�?改變刷新率。\n"
#define MSGTR_LIBVO_VESA_UnableToFitTheMode "[VO_VESA] 模�?超出顯示器的�?製。�?�?改變刷新率。\n"
#define MSGTR_LIBVO_VESA_DetectedInternalFatalError "[VO_VESA] 檢測到内部致命錯誤: �?始化在�?�?始化�?被調用。\n"
#define MSGTR_LIBVO_VESA_SwitchFlipIsNotSupported "[VO_VESA] -flip 命令�?支�?。\n"
#define MSGTR_LIBVO_VESA_PossibleReasonNoVbe2BiosFound "[VO_VESA] �?�能的原因: 找�?到 VBE2 BIOS。\n"
#define MSGTR_LIBVO_VESA_FoundVesaVbeBiosVersion "[VO_VESA] 找到 VESA VBE BIOS 版本 %x.%x 修訂版本: %x。\n"
#define MSGTR_LIBVO_VESA_VideoMemory "[VO_VESA] 顯存: %u Kb。\n"
#define MSGTR_LIBVO_VESA_Capabilites "[VO_VESA] VESA 兼容性: %s %s %s %s %s。\n"
#define MSGTR_LIBVO_VESA_BelowWillBePrintedOemInfo "[VO_VESA] !!! 下�?�顯示 OEM 信�?� !!!\n"
#define MSGTR_LIBVO_VESA_YouShouldSee5OemRelatedLines "[VO_VESA] 你應該看到 5 行 OEM 相關内容; �?�則, 你的 vm86 有�?題。\n"
#define MSGTR_LIBVO_VESA_OemInfo "[VO_VESA] OEM 信�?�: %s。\n"
#define MSGTR_LIBVO_VESA_OemRevision "[VO_VESA] OEM 版本: %x。\n"
#define MSGTR_LIBVO_VESA_OemVendor "[VO_VESA] OEM 發行商: %s。\n"
#define MSGTR_LIBVO_VESA_OemProductName "[VO_VESA] OEM 產�?�??: %s。\n"
#define MSGTR_LIBVO_VESA_OemProductRev "[VO_VESA] OEM 產�?版本: %s。\n"
#define MSGTR_LIBVO_VESA_Hint "[VO_VESA] �??示: 為使用電視輸出你需�?在啟動之�?�?�入 TV 接�?�。\n"\
"[VO_VESA] 因為 VESA BIOS 祇在自舉的時候�?始化自己。\n"
#define MSGTR_LIBVO_VESA_UsingVesaMode "[VO_VESA] 使用 VESA 模�? (%u) = %x [%ux%u@%u]\n"
#define MSGTR_LIBVO_VESA_CantInitializeSwscaler "[VO_VESA] �?能�?始化軟件縮放。\n"
#define MSGTR_LIBVO_VESA_CantUseDga "[VO_VESA] �?能使用 DGA。鎖定�?�域切�?�模�?。 :(\n"
#define MSGTR_LIBVO_VESA_UsingDga "[VO_VESA] 使用 DGA (物�?�資�?: %08lXh, %08lXh)"
#define MSGTR_LIBVO_VESA_CantUseDoubleBuffering "[VO_VESA] �?能使用雙緩�?: 顯存�?足。\n"
#define MSGTR_LIBVO_VESA_CantFindNeitherDga "[VO_VESA] 未找到 DGA 也�?能�?新分�?窗�?�的大�?。\n"
#define MSGTR_LIBVO_VESA_YouveForcedDga "[VO_VESA] 你鎖定了 DGA。退出中\n"
#define MSGTR_LIBVO_VESA_CantFindValidWindowAddress "[VO_VESA] 未找到�?�用的窗�?�地�?�。\n"
#define MSGTR_LIBVO_VESA_UsingBankSwitchingMode "[VO_VESA] 使用�?�域切�?�模�? (物�?�資�?: %08lXh, %08lXh)。\n"
#define MSGTR_LIBVO_VESA_CantAllocateTemporaryBuffer "[VO_VESA] �?能分�?臨時緩�?。\n"
#define MSGTR_LIBVO_VESA_SorryUnsupportedMode "[VO_VESA] 抱歉, 模�?�?支�? -- 試試 -x 640 -zoom。\n"
#define MSGTR_LIBVO_VESA_OhYouReallyHavePictureOnTv "[VO_VESA] 啊你的電視機上有圖�?了!\n"
#define MSGTR_LIBVO_VESA_CantInitialozeLinuxVideoOverlay "[VO_VESA] �?能�?始化 Linux Video Overlay。\n"
#define MSGTR_LIBVO_VESA_UsingVideoOverlay "[VO_VESA] 使用視頻覆蓋: %s。\n"
#define MSGTR_LIBVO_VESA_CantInitializeVidixDriver "[VO_VESA] �?能�?始化 VIDIX 驅動。\n"
#define MSGTR_LIBVO_VESA_UsingVidix "[VO_VESA] 使用 VIDIX 中。\n"
#define MSGTR_LIBVO_VESA_CantFindModeFor "[VO_VESA] 未找到�?��?� %ux%u@%u 的模�?。\n"
#define MSGTR_LIBVO_VESA_InitializationComplete "[VO_VESA] VESA �?始化完�?。\n"

// libvo/vo_x11.c

#define MSGTR_LIBVO_X11_DrawFrameCalled "[VO_X11] 調用 draw_frame()!!!!!!\n"

// libvo/vo_xv.c

#define MSGTR_LIBVO_XV_DrawFrameCalled "[VO_XV] 調用 draw_frame()!!!!!!\n"
#define MSGTR_LIBVO_XV_SharedMemoryNotSupported "[VO_XV] �?支�?共享内存\n回複到正常 Xv。\n"
#define MSGTR_LIBVO_XV_XvNotSupportedByX11 "[VO_XV] �?�?起, 此 X11 版本/驅動�?支�? Xv\n[VO_XV] ******** 試試使用  -vo x11  或  -vo sdl  *********\n"
#define MSGTR_LIBVO_XV_XvQueryAdaptorsFailed  "[VO_XV] XvQueryAdaptors 失敗.\n"
#define MSGTR_LIBVO_XV_InvalidPortParameter "[VO_XV] 無效端�?��?�數, 端�?� 0 �?載。\n"
#define MSGTR_LIBVO_XV_CouldNotGrabPort "[VO_XV] �?能抓�?�端�?� %i.\n"
#define MSGTR_LIBVO_XV_CouldNotFindFreePort "[VO_XV] 未找到空閑 Xvideo 端�?� - 或許�?�一�?�程已\n"\
"[VO_XV] 在使用。請關閉所有的應用程�?�?試。如果那様�?�\n"\
"[VO_XV] 没用, 請�?�見 'mplayer -vo help' 找其它 (�?�-xv) 視頻輸出驅動。\n"
#define MSGTR_LIBVO_XV_NoXvideoSupport "[VO_XV] 好�?�?存在 Xvideo 支�?你�?�用的顯�?�。\n"\
"[VO_XV] �?�行 'xvinfo' 證實有 Xv 的支�?并閲讀\n"\
"[VO_XV] DOCS/HTML/en/video.html#xv!\n"\
"[VO_XV] 請�?�見 'mplayer -vo help' 找其它 (�?�-xv) 視頻輸出驅動。\n"\
"[VO_XV] 試試 -vo x11.\n"


// loader/ldt_keeper.c

#define MSGTR_LOADER_DYLD_Warning "警告: 嘗試使用 DLL 編解碼器, 但是環境變�?\n         DYLD_BIND_AT_LAUNCH 未設定。 這很�?�能造�?程�?崩潰。\n"

// stream/stream_radio.c

#define MSGTR_RADIO_ChannelNamesDetected "[radio] 檢測到廣播通�?��??。\n"
#define MSGTR_RADIO_WrongFreqForChannel "[radio] 錯誤的通�?�頻率 %s\n"
#define MSGTR_RADIO_WrongChannelNumberFloat "[radio] 錯誤的通�?�號: %.2f\n"
#define MSGTR_RADIO_WrongChannelNumberInt "[radio] 錯誤的通�?�號: %d\n"
#define MSGTR_RADIO_WrongChannelName "[radio] 錯誤的通�?��??: %s\n"
#define MSGTR_RADIO_FreqParameterDetected "[radio] 檢測到廣播頻率�?�數。\n"
#define MSGTR_RADIO_GetTunerFailed "[radio] Warning: ioctl �?��?�調諧器失敗: %s。設置 frac 為 %d。\n"
#define MSGTR_RADIO_NotRadioDevice "[radio] %s 决�?是廣播設備!\n"
#define MSGTR_RADIO_SetFreqFailed "[radio] ioctl 設定頻率為 0x%x (%.2f) failed: %s\n"
#define MSGTR_RADIO_GetFreqFailed "[radio] ioctl �?��?�頻率失敗: %s\n"
#define MSGTR_RADIO_SetMuteFailed "[radio] ioctl 設定�?�音失敗: %s\n"
#define MSGTR_RADIO_QueryControlFailed "[radio] ioctl 查詢控製失敗: %s\n"
#define MSGTR_RADIO_GetVolumeFailed "[radio] ioctl �?��?�音�?失敗: %s\n"
#define MSGTR_RADIO_SetVolumeFailed "[radio] ioctl 設定音�?失敗: %s\n"
#define MSGTR_RADIO_DroppingFrame "\n[radio] 太糟糕 - 丢失音頻幀 (%d 字節)!\n"
#define MSGTR_RADIO_BufferEmpty "[radio] grab_audio_frame: 緩�?為空, 等待 %d 字節數據。\n"
#define MSGTR_RADIO_AudioInitFailed "[radio] audio_in_init 失敗: %s\n"
#define MSGTR_RADIO_AllocateBufferFailed "[radio] �?能分�?音頻緩�? (block=%d,buf=%d): %s\n"
#define MSGTR_RADIO_CurrentFreq "[radio] 當�?頻率: %.2f\n"
#define MSGTR_RADIO_SelectedChannel "[radio] 已�?�通�?�: %d - %s (freq: %.2f)\n"
#define MSGTR_RADIO_ChangeChannelNoChannelList "[radio] �?能改變通�?�: 無給定的通�?�列表。\n"
#define MSGTR_RADIO_UnableOpenDevice "[radio] 無法打開 '%s': %s\n"
#define MSGTR_RADIO_InitFracFailed "[radio] init_frac 失敗。\n"
#define MSGTR_RADIO_WrongFreq "[radio] 錯誤頻率: %.2f\n"
#define MSGTR_RADIO_UsingFreq "[radio] 使用頻率: %.2f。\n"
#define MSGTR_RADIO_AudioInInitFailed "[radio] audio_in_init 失敗。\n"
#define MSGTR_RADIO_AudioInSetupFailed "[radio] audio_in_setup 調用失敗: %s\n"
#define MSGTR_RADIO_ClearBufferFailed "[radio] 清空緩�?失敗: %s\n"
#define MSGTR_RADIO_StreamEnableCacheFailed "[radio] 調用 stream_enable_cache 失敗: %s\n"
#define MSGTR_RADIO_DriverUnknownStr "[radio] 未知驅動�??: %s\n"
#define MSGTR_RADIO_DriverV4L2 "[radio] 使用 V4Lv2 廣播接�?�。\n"
#define MSGTR_RADIO_DriverV4L "[radio] 使用 V4Lv1 廣播接�?�。\n"
#define MSGTR_RADIO_DriverBSDBT848 "[radio] 使用 *BSD BT848 廣播接�?�。\n"

// ================================== LIBASS ====================================

// ass_bitmap.c
#define MSGTR_LIBASS_FT_Glyph_To_BitmapError "[ass] FT_Glyph_To_Bitmap 出錯 %d \n"
#define MSGTR_LIBASS_UnsupportedPixelMode "[ass] �?支�?的象素模�?: %d\n"

// ass.c
#define MSGTR_LIBASS_NoStyleNamedXFoundUsingY "[ass] [%p] 警告: 没有找到風格(style) '%s', 將使用 '%s'\n"
#define MSGTR_LIBASS_BadTimestamp "[ass] 錯誤的時間戳\n"
#define MSGTR_LIBASS_BadEncodedDataSize "[ass] 錯誤的編碼數據大�?\n"
#define MSGTR_LIBASS_FontLineTooLong "[ass] 字體行太長: %d, %s\n"
#define MSGTR_LIBASS_EventFormatHeaderMissing "[ass] 未找到事件格�?頭\n"
#define MSGTR_LIBASS_ErrorOpeningIconvDescriptor "[ass] 打開iconv�??述符出錯。\n"
#define MSGTR_LIBASS_ErrorRecodingFile "[ass] 記録到文件出錯。\n"
#define MSGTR_LIBASS_FopenFailed "[ass] ass_read_file(%s): 文件打開(fopen)失敗\n"
#define MSGTR_LIBASS_FseekFailed "[ass] ass_read_file(%s): 文件定�?(fseek)失敗\n"
#define MSGTR_LIBASS_RefusingToLoadSubtitlesLargerThan100M "[ass] ass_read_file(%s): 拒絶�?入大于100M的字幕\n"
#define MSGTR_LIBASS_ReadFailed "讀失敗, %d: %s\n"
#define MSGTR_LIBASS_AddedSubtitleFileMemory "[ass] 已加入字幕文件: <内存> (%d styles, %d events)\n"
#define MSGTR_LIBASS_AddedSubtitleFileFname "[ass] 已加入字幕文件: %s (%d styles, %d events)\n"
#define MSGTR_LIBASS_FailedToCreateDirectory "[ass] 創建目録失敗 %s\n"
#define MSGTR_LIBASS_NotADirectory "[ass] �?是一個目録: %s\n"

// ass_cache.c
#define MSGTR_LIBASS_TooManyFonts "[ass] 太多字體\n"
#define MSGTR_LIBASS_ErrorOpeningFont "[ass] 打開字體出錯: %s, %d\n"

// ass_fontconfig.c
#define MSGTR_LIBASS_SelectedFontFamilyIsNotTheRequestedOne "[ass] fontconfig: �?�中的字體家�?�?是�?求的: '%s' != '%s'\n"
#define MSGTR_LIBASS_UsingDefaultFontFamily "[ass] fontconfig_select: 使用缺�?字體家�?: (%s, %d, %d) -> %s, %d\n"
#define MSGTR_LIBASS_UsingDefaultFont "[ass] fontconfig_select: 使用缺�?字體: (%s, %d, %d) -> %s, %d\n"
#define MSGTR_LIBASS_UsingArialFontFamily "[ass] fontconfig_select: 使用 'Arial' 字體家�?: (%s, %d, %d) -> %s, %d\n"
#define MSGTR_LIBASS_FcInitLoadConfigAndFontsFailed "[ass] FcInitLoadConfigAndFonts 失敗。\n"
#define MSGTR_LIBASS_UpdatingFontCache "[ass] 更新字體緩存�?�。\n"
#define MSGTR_LIBASS_BetaVersionsOfFontconfigAreNotSupported "[ass] �?支�?測試版的fontconfig。\n[ass] 在報告bug�?請先更新。\n"
#define MSGTR_LIBASS_FcStrSetAddFailed "[ass] FcStrSetAdd 失敗。\n"
#define MSGTR_LIBASS_FcDirScanFailed "[ass] FcDirScan 失敗。\n"
#define MSGTR_LIBASS_FcDirSave "[ass] FcDirSave 失敗。\n"
#define MSGTR_LIBASS_FcConfigAppFontAddDirFailed "[ass] FcConfigAppFontAddDir 失敗\n"
#define MSGTR_LIBASS_FontconfigDisabledDefaultFontWillBeUsed "[ass] Fontconfig 已�?用, 將祇使用缺�?字體。\n"
#define MSGTR_LIBASS_FunctionCallFailed "[ass] %s 失敗\n"

// ass_render.c
#define MSGTR_LIBASS_NeitherPlayResXNorPlayResYDefined "[ass] PlayResX 和 PlayResY 都没有定義. �?�定為 384x288。\n"
#define MSGTR_LIBASS_PlayResYUndefinedSettingY "[ass] PlayResY 未定義, 設為 %d。\n"
#define MSGTR_LIBASS_PlayResXUndefinedSettingX "[ass] PlayResX 未定義, 設為 %d。\n"
#define MSGTR_LIBASS_FT_Init_FreeTypeFailed "[ass] FT_Init_FreeType 失敗。\n"
#define MSGTR_LIBASS_Init "[ass] �?始化\n"
#define MSGTR_LIBASS_InitFailed "[ass] �?始化失敗。\n"
#define MSGTR_LIBASS_BadCommand "[ass] 錯誤的命令: %c%c\n"
#define MSGTR_LIBASS_ErrorLoadingGlyph  "[ass] �?入字形出錯。\n"
#define MSGTR_LIBASS_FT_Glyph_Stroke_Error "[ass] FT_Glyph_Stroke 錯誤 %d \n"
#define MSGTR_LIBASS_UnknownEffectType_InternalError "[ass] 未知的效果類型 (内部錯誤)\n"
#define MSGTR_LIBASS_NoStyleFound "[ass] 找�?到風格(style)!\n"
#define MSGTR_LIBASS_EmptyEvent "[ass] 空事件!\n"
#define MSGTR_LIBASS_MAX_GLYPHS_Reached "[ass] �?�到了字形最大值: 事件 %d, 開始 = %llu, 時長 = %llu\n 文本 = %s\n"
#define MSGTR_LIBASS_EventHeightHasChanged "[ass] 警告! 事件高度(height) 已改變!  \n"

// ass_font.c
#define MSGTR_LIBASS_GlyphNotFoundReselectingFont "[ass] 字形 0x%X 未找到, �?新�?�擇字體 (%s, %d, %d)\n"
#define MSGTR_LIBASS_GlyphNotFound "[ass] 字形 0x%X 未在字體中找到 (%s, %d, %d)\n"
#define MSGTR_LIBASS_ErrorOpeningMemoryFont "[ass] 打開内存字體出錯: %s\n"
