// Synced with help_mp-en.h rev. 34785
//
// Translated by JRaSH <jrash06@gmail.com>









// ========================= MPlayer help ===========================

static const char help_text[]=
"用法：            mplayer [选项] [URL|路径/]文件�??\n"
"\n"
"基本选项：        （完整列表�?��?手册页）\n"
" -vo <drv>        选择视频输出驱动程�?（查看驱动程�?列表用“-vo help�?）\n"
" -ao <drv>        选择音频输出驱动程�?（查看驱动程�?列表用“-ao help�?）\n"
#ifdef CONFIG_VCD
" vcd://<曲目�?�>   播放 (S)VCD（超级VCD）曲目（无格�?设备，无需装载）\n"
#endif
#ifdef CONFIG_DVDREAD
" dvd://<标题�?�>   从设备而�?是从普通文件上播放 DVD 标题\n"
#endif
" -alang/-slang    选择 DVD 音频/字幕的语言（使用两字符的国家代�?�）\n"
" -ss <�?置>       定�?至给定的（秒数或时:分:秒 - hh:mm:ss）�?置\n"
" -nosound         �?播放声音\n"
" -fs              全�?播放（或用 -vm�?-zoom，详�?手册相关页�?�）\n"
" -x <x> -y <y>    设置显示的分辨率（用以与 -vm 或 -zoom 一起使用）\n"
" -sub <文件>      指定所使用的字幕文件（�?��? -subfps�?-subdelay）\n"
" -playlist <文件> 指定播放列表文件\n"
" -vid x -aid y    选择播放视频�?（x）和音频�?（y）\n"
" -fps x -srate y  改�?�视频帧率为 x（fps），音频采样率为 y（Hz）\n"
" -pp <质�?>       �?�用�?�期处�?�过滤器（详�?于手册相关页�?�）\n"
" -framedrop       �?�用丢帧（用于�?行慢的机器）\n"
"\n"
"基本控制键：      （完整列表�?手册相关页�?�，也请查阅 input.conf）\n"
" <-  或  ->       �?�退/快进 10 秒\n"
" 上 或 下         �?�退/快进 1 分钟\n"
" pgdown 或 pgup   �?�退/快进 10 分钟\n"
" < 或 >           跳到播放列表中的�?一个/�?�一个\n"
" p 或 空格键      暂�?�影片（按任�?键继续）\n"
" q 或 ESC         �?�止播放并退出程�?\n"
" + 或 -           音频延迟 +/- 0.1 秒\n"
" o                循环切�?� OSD 模�?：无/定�?�?�/定�?�?�加计时器\n"
" * 或 /           增加或�?少 PCM 音�?\n"
" x 或 z           字幕延迟 +/- 0.1 秒\n"
" r 或 t           字幕�?置上移/下移，�?��?“-vf expand�?\n"
"\n"
" * * * �?��?手册相关页�?��?�获�?�具体内容，�?�更多（高级）选项和控制键的信�?� * * *\n"
"\n";

// ========================= MPlayer messages ===========================

// mplayer.c
#define MSGTR_Exiting "\n正在退出..\n"
#define MSGTR_ExitingHow "\n正在退出...（%s）\n"
#define MSGTR_Exit_quit "退出"
#define MSGTR_Exit_eof "文件末尾"
#define MSGTR_Exit_error "致命错误"
#define MSGTR_IntBySignal "\nMPlayer 被 %d 信�?�中断（�?于 %s 模�?�）\n"
#define MSGTR_NoHomeDir "无法找到 HOME 目录\n"
#define MSGTR_GetpathProblem "get_path(\"config\") 调用出现问题\n"
#define MSGTR_CreatingCfgFile "创建�?置文件：%s\n"
#define MSGTR_CantLoadFont "无法加载�?图字体‘%s’\n"
#define MSGTR_CantLoadSub "无法加载字幕‘%s’\n"
#define MSGTR_DumpSelectedStreamMissing "内核导出：致命错误：指定的媒体�?�?存在�?\n"
#define MSGTR_CantOpenDumpfile "无法打开内核导出文件。\n"
#define MSGTR_CoreDumped "内核已导出 :)\n"
#define MSGTR_DumpBytesWrittenPercent "导出：已写入 %"PRIu64" 字节（~%.1f%%）\r"
#define MSGTR_DumpBytesWritten "导出：已写入 %"PRIu64" 字节\r"
#define MSGTR_DumpBytesWrittenTo "导出：已将 %"PRIu64" 字节写入‘%s’中。\n"
#define MSGTR_FPSnotspecified "FPS 在文件头中没有指定或者无效，请使用 -fps 选项。\n"
#define MSGTR_TryForceAudioFmtStr "�?试强制使用音频编解�?器驱动类 %s...\n"
#define MSGTR_CantFindAudioCodec "无法找到音频格�? 0x%X 的编解�?器。\n"
#define MSGTR_TryForceVideoFmtStr "�?试强制使用视频编解�?器驱动类 %s...\n"
#define MSGTR_CantFindVideoCodec "无法找到匹�?所选 -vo �?�数和视频格�? 0x%X 的编解�?器。\n"
#define MSGTR_CannotInitVO "致命错误：无法�?始化视频驱动�?\n"
#define MSGTR_CannotInitAO "无法打开/�?始化音频设备 -> �?用声音。\n"
#define MSGTR_StartPlaying "开始播放...\n"

#define MSGTR_SystemTooSlow "\n\n"\
"         ************************************************\n"\
"         ****     你的系统�?行太“慢�?，无法播放�?     ****\n"\
"         ************************************************\n"\
" �?�能的原因�?问题和�?�通的办法：\n"\
"- 最常�?的原因：�?��??的或有�?洞的 _audio_ 驱动\n"\
"  - 试试 -ao sdl 或使用 ALSA  的 OSS 模拟方�?。\n"\
"  - �?试使用�?�?�的 -autosync 的值，�?妨从 30 开始。\n"\
"- 视频输出�?行慢\n"\
"  - 试试 -vo 用�?�?�的驱动（�?��? -vo help 以获�?�驱动列表）或者试试 -framedrop�?\n"\
"- CPU �?行慢\n"\
"  - �?�?试图在�?行慢的 CPU 上播放大容�?的 DVD/DivX�?试试 lavdopts 中的一些选项，\n"\
"    例如：-vfm ffmpeg -lavdopts lowres=1:fast:skiploopfilter=all。\n"\
"- 文件�?��??\n"\
"  - 试试组�?�使用 -nobps -ni -forceidx -mc 0 这些选项。\n"\
"- 媒体读�?�慢（NFS/SMB 挂载�?DVD�?VCD 等设备）\n"\
"  - 试试 -cache 8192 选项。\n"\
"- 你是�?�在用 -cache 选项播放一个�?�交错�?�并的 AVI 文件？\n"\
"  - 试试 -nocache 选项。\n"\
"阅读 DOCS/zh/video.html 和 DOCS/zh/sound.html，寻找调整/加速的技巧。\n"\
"如果这些一个都帮�?了你，请阅读 DOCS/zh/bugreports.html。\n\n"

#define MSGTR_NoGui "MPlayer 编译时没有包�?�图形界�?�的支�?。\n"
#define MSGTR_GuiNeedsX "MPlayer 的图形界�?�需�? X11。\n"
#define MSGTR_Playing "\n正在播放 %s。\n"
#define MSGTR_NoSound "音频：没有音轨\n"
#define MSGTR_FPSforced "FPS 强制设为 %5.3f（ftime：%5.3f）。\n"
#define MSGTR_AvailableVideoOutputDrivers "�?�用的视频输出驱动：\n"
#define MSGTR_AvailableAudioOutputDrivers "�?�用的音频输出驱动：\n"
#define MSGTR_AvailableAudioCodecs "�?�用的音频编解�?器：\n"
#define MSGTR_AvailableVideoCodecs "�?�用的视频编解�?器：\n"
#define MSGTR_AvailableAudioFm "\n�?�用的（编译时已包�?�的）音频编解�?器类/驱动：\n"
#define MSGTR_AvailableVideoFm "\n�?�用的（编译时已包�?�的）视频编解�?器类/驱动：\n"
#define MSGTR_AvailableFsType "�?�用的全�?图层�?��?�模�?：\n"
#define MSGTR_CannotReadVideoProperties "视频：无法读�?�视频属性\n"
#define MSGTR_NoStreamFound "未找到媒体�?。\n"
#define MSGTR_ErrorInitializingVODevice "打开/�?始化所选的（-vo）视频输出设备出错。\n"
#define MSGTR_ForcedVideoCodec "强制使用视频编解�?器：%s\n"
#define MSGTR_ForcedAudioCodec "强制使用音频编解�?器：%s\n"
#define MSGTR_Video_NoVideo "视频：没有视频�?\n"
#define MSGTR_NotInitializeVOPorVO "\n致命错误：无法�?始化视频过滤器（-vf）或视频输出（-vf）。\n"
#define MSGTR_Paused "  =====  暂�?�  =====" // no more than 23 characters (status line for audio files)
#define MSGTR_PlaylistLoadUnable "\n无法装载播放列表 %s\n"
#define MSGTR_Exit_SIGILL_RTCpuSel \
"- “�?�法指令�?导致 MPlayer 崩溃。\n"\
"  这�?�能是我们新的实时 CPU 侦测代�?中的一个缺陷...\n"\
"  请阅读 DOCS/zh/bugreports.html。\n"
#define MSGTR_Exit_SIGILL \
"- “�?�法指令�?导致 MPlayer 崩溃。\n"\
"  这通常是在你�?行 MPlayer 的 CPU 与其\n"\
"  编译/优化时所针对的 CPU �?�?�时�?�生。\n"\
"  检查是�?�如此�?\n"
#define MSGTR_Exit_SIGSEGV_SIGFPE \
"- �?�?��?�使用 CPU/FPU/RAM 导致 MPlayer 崩溃。\n"\
"  使用 --enable-debug �?新编译 MPlayer 并建立“gdb�?�??跟踪和\n"\
"  �??汇编输出。具体细节�? DOCS/zh/bugreports.html#crash。\n"
#define MSGTR_Exit_SIGCRASH \
"- MPlayer 崩溃了。这�?应该�?�生。\n"\
"  这�?�能是 MPlayer 或者 你的驱动中 或者 你的 gcc 版本中的一个缺陷。\n"\
"  如你觉得这是 MPlayer 的问题，请阅读 DOCS/zh/bugreports.html\n"\
"  并�?�循其中的指示去�?�。除�?�你在报告一个潜在的缺陷时候�??供我们\n"\
"  所需�?的信�?�，�?�则我们�?能也�?会帮助你。\n"
#define MSGTR_LoadingConfig "正在加载�?置 '%s'\n"
#define MSGTR_LoadingProtocolProfile "加载�??议相关的�?置集‘%s’\n"
#define MSGTR_LoadingExtensionProfile "加载扩展组件相关的�?置集‘%s’\n"
#define MSGTR_AddedSubtitleFile "字幕：添加字幕文件（%d）：%s\n"
#define MSGTR_RemovedSubtitleFile "字幕：移除字幕文件（%d）： %s\n"
#define MSGTR_ErrorOpeningOutputFile "未能以写入方�?打开文件 [%s]�?\n"
#define MSGTR_RTCDeviceNotOpenable "未能打开 %s：%s（用户应当有�?��?读�?�该设备。）\n"
#define MSGTR_LinuxRTCInitErrorIrqpSet "ioctl (rtc_irqp_set %lu) 中出现 Linux RTC �?始化错误：%s\n"
#define MSGTR_IncreaseRTCMaxUserFreq "�?试添加 \"echo %lu > /proc/sys/dev/rtc/max-user-freq\" 到你的系统�?�动脚本中。\n"
#define MSGTR_LinuxRTCInitErrorPieOn "ioctl (rtc_pie_on) 中出现 Linux RTC �?始置错误：%s\n"
#define MSGTR_UsingTimingType "正在使用 %s 计时。\n"
#define MSGTR_Getch2InitializedTwice "警告：getch2_init 被两次调用�?\n"
#define MSGTR_DumpstreamFdUnavailable "无法导出该数�?��? - 没有�?�用的文件�??述符。\n"
#define MSGTR_CantOpenLibmenuFilterWithThisRootMenu "无法用根�?��?� %s 打开 libmenu 视频过滤器。\n"
#define MSGTR_AudioFilterChainPreinitError "音频过滤器链预�?始化错误�?\n"
#define MSGTR_LinuxRTCReadError "Linux RTC 读�?�错误：%s\n"
#define MSGTR_SoftsleepUnderflow "警告�?Softsleep 数值下溢�?\n"
#define MSGTR_DvdnavNullEvent "DVDNAV 事件为空？�?\n"
#define MSGTR_DvdnavHighlightEventBroken "DVDNAV 事件：�?�?事件�?��??\n"
#define MSGTR_DvdnavEvent "DVDNAV 事件：%s\n"
#define MSGTR_DvdnavHighlightHide "DVDNAV 事件：�?�?事件�?�?\n"
#define MSGTR_DvdnavStillFrame "######################################## DVDNAV 事件：�?�止帧：%d 秒\n"
#define MSGTR_DvdnavNavStop "DVDNAV 事件：Nav �?�止\n"
#define MSGTR_DvdnavNavNOP "DVDNAV 事件：Nav 无�?作\n"
#define MSGTR_DvdnavNavSpuStreamChangeVerbose "DVDNAV 事件：Nav SPU 数�?��?切�?�：物�?��?置：%d/%d/%d 逻辑�?置：%d\n"
#define MSGTR_DvdnavNavSpuStreamChange "DVDNAV 事件：Nav SPU 数�?��?切�?�：物�?��?置：%d 逻辑�?置：%d\n"
#define MSGTR_DvdnavNavAudioStreamChange "DVDNAV 事件：Nav 音频�?切�?�：物�?��?置：%d 逻辑�?置：%d\n"
#define MSGTR_DvdnavNavVTSChange "DVDNAV 事件：Nav VTS 切�?�\n"
#define MSGTR_DvdnavNavCellChange "DVDNAV 事件：Nav Cell 切�?�\n"
#define MSGTR_DvdnavNavSpuClutChange "DVDNAV 事件：Nav SPU CLUT 切�?�\n"
#define MSGTR_DvdnavNavSeekDone "DVDNAV 事件：Nav 定�?完�?\n"
#define MSGTR_MenuCall "�?��?�调用\n"
#define MSGTR_MasterQuit "选项 -udp-slave：因主动方中止而中止\n"
#define MSGTR_InvalidIP "选项 -udp-ip：无效的 IP 地�?�\n"
#define MSGTR_Forking "正在创建�?进程...\n"
#define MSGTR_Forked "已�?创建�?进程...\n"
#define MSGTR_CouldntStartGdb "无法�?�动 gdb\n"
#define MSGTR_CouldntFork "无法创建�?进程\n"
#define MSGTR_FilenameTooLong "文件�??太长，无法加载用于特定文件或目录的�?置文件\n"
#define MSGTR_AudioDeviceStuck "音频设备已堵塞�?\n"
#define MSGTR_AudioOutputTruncated "音频输入信�?�在尾端截断。\n"
#define MSGTR_ASSCannotAddVideoFilter "ASS：无法添加视频过滤器\n"
#define MSGTR_PtsAfterFiltersMissing "缺失过滤器�?�的 PTS\n"
#define MSGTR_CommandLine "命令行："
#define MSGTR_MenuInitFailed "�?��?��?始化失败。\n"

// --- edit decision lists
#define MSGTR_EdlOutOfMem "无法分�?足够的内存以存放 EDL 数�?�。\n"
#define MSGTR_EdlOutOfMemFile "无法分�?足够的内存以存放 EDL 文件�?? [%s]。\n"
#define MSGTR_EdlRecordsNo "读�?� %d 个 EDL 行为。\n"
#define MSGTR_EdlQueueEmpty "没有 EDL 行为�?处�?�。\n"
#define MSGTR_EdlCantOpenForWrite "无法打开并写入 EDL 文件 [%s]。\n"
#define MSGTR_EdlCantOpenForRead "无法打开并读�?� EDL 文件 [%s]。\n"
#define MSGTR_EdlNOsh_video "无视频�?时无法使用 EDL，�?用该功能。\n"
#define MSGTR_EdlNOValidLine "无效的一行 EDL 文本：%s\n"
#define MSGTR_EdlBadlyFormattedLine "格�?错误的一行 EDL 文本 [%d]，丢弃该行。\n"
#define MSGTR_EdlBadLineOverlap "上一个终止�?置是 [%f]；下一个起始�?置是 [%f]。\n"\
"�?一项内容必须按时间顺�?排列，�?能�?�?�。 丢弃该行。\n"
#define MSGTR_EdlBadLineBadStop "终止时间必须�?于起始时间之�?�。\n"
#define MSGTR_EdloutBadStop "�?�消 EDL 跳跃，上一个开始�?置 > �?�止�?置\n"
#define MSGTR_EdloutStartSkip "开始 EDL 跳跃，�?按键‘i’以指定跳过内容的结尾。\n"
#define MSGTR_EdloutEndSkip "结�?� EDL 跳跃，文本行已写入。\n"

// mplayer.c OSD
#define MSGTR_OSDenabled "已�?�用"
#define MSGTR_OSDdisabled "已�?�用"
#define MSGTR_OSDAudio "音频：%s"
#define MSGTR_OSDVideo "视频：%s"
#define MSGTR_OSDChannel "频�?�：%s"
#define MSGTR_OSDSubDelay "字幕延迟：%d 毫秒"
#define MSGTR_OSDSpeed "播放速度：x %6.2f"
#define MSGTR_OSDosd "OSD：%s"
#define MSGTR_OSDChapter "章节：（%d）%s"
#define MSGTR_OSDAngle "视角：%d/%d"
#define MSGTR_OSDDeinterlace "解除隔行扫�??：%s"
#define MSGTR_OSDCapturing "抓�?�：%s"
#define MSGTR_OSDCapturingFailure "抓�?�失败"

// property values
#define MSGTR_Enabled "已�?�用"
#define MSGTR_EnabledEdl "已�?�用（EDL）"
#define MSGTR_Disabled "已�?�用"
#define MSGTR_HardFrameDrop "强制丢帧"
#define MSGTR_Unknown "未知"
#define MSGTR_Bottom "底部"
#define MSGTR_Center "中部"
#define MSGTR_Top "顶部"
#define MSGTR_SubSourceFile "字幕文件"
#define MSGTR_SubSourceVobsub "VOBSUB"
#define MSGTR_SubSourceDemux "附带字幕"

// OSD bar names
#define MSGTR_Volume "音�?"
#define MSGTR_Panscan "全景模�?"
#define MSGTR_Gamma "伽玛值"
#define MSGTR_Brightness "亮度"
#define MSGTR_Contrast "对比度"
#define MSGTR_Saturation "饱和度"
#define MSGTR_Hue "色调"
#define MSGTR_Balance "�?�衡"

// property state
#define MSGTR_LoopStatus "循环模�?：%s"
#define MSGTR_MuteStatus "�?�音：%s"
#define MSGTR_AVDelayStatus "A-V 延迟: %s"
#define MSGTR_OnTopStatus "�?�?在�?�?�：%s"
#define MSGTR_RootwinStatus "根窗�?�：%s"
#define MSGTR_BorderStatus "边框：%s"
#define MSGTR_FramedroppingStatus "丢帧：%s"
#define MSGTR_VSyncStatus "垂直刷新�?�步：%s"
#define MSGTR_SubSelectStatus "字幕：%s"
#define MSGTR_SubSourceStatus "字幕�?��?：%s"
#define MSGTR_SubPosStatus "字幕�?置：%s/100"
#define MSGTR_SubAlignStatus "字幕对�?方�?：%s"
#define MSGTR_SubDelayStatus "字幕延迟：%s"
#define MSGTR_SubScale "字幕缩放：%s"
#define MSGTR_SubVisibleStatus "显示字幕：%s"
#define MSGTR_SubForcedOnlyStatus "�?�用强制使用的字幕：%s"

// mencoder.c
#define MSGTR_UsingPass3ControlFile "使用第三阶段的控制文件：%s\n"
#define MSGTR_MissingFilename "\n没有指定文件�??。\n\n"
#define MSGTR_CannotOpenFile_Device "无法打开文件/设备。\n"
#define MSGTR_CannotOpenDemuxer "无法打开�?分离器。\n"
#define MSGTR_NoAudioEncoderSelected "\n没有选择音频编�?器（-oac）。请选择一个编�?器（�?��? -oac help）或者使用 -nosound。\n"
#define MSGTR_NoVideoEncoderSelected "\n没有选择视频编�?器（-ovc）。请选择一个编�?器（�?��? -ovc help）。\n"
#define MSGTR_CannotOpenOutputFile "无法打开输出文件‘%s’。\n"
#define MSGTR_EncoderOpenFailed "未能打开编�?器。\n"
#define MSGTR_MencoderWrongFormatAVI "\n警告：输出文件格�?是 _AVI_。�?��? -of help。\n"
#define MSGTR_MencoderWrongFormatMPG "\n警告：输出文件格�?是 _MPEG_。�?��? -of help。\n"
#define MSGTR_MissingOutputFilename "没有指定输出文件，请�?��? -o 选项。"
#define MSGTR_ForcingOutputFourcc "强制输出内容的 FourCC 为 %x [%.4s]。\n"
#define MSGTR_ForcingOutputAudiofmtTag "强制输出音频的格�?标签为 0x%x。\n"
#define MSGTR_DuplicateFrames "\n%d 个�?�?的帧�?\n"
#define MSGTR_SkipFrame "\n跳帧中�?\n"
#define MSGTR_ResolutionDoesntMatch "\n新的视频文件和�?一个文件的分辨率或色彩空间�?�?�。\n"
#define MSGTR_FrameCopyFileMismatch "\n所有的视频文件必须使用�?�样的帧率�?分辨率和编解�?器�?能使用 -ovc copy。\n"
#define MSGTR_AudioCopyFileMismatch "\n所有的音频文件必须使用�?�样的音频编解�?器和格�?�?能使用 -oac copy。\n"
#define MSGTR_NoAudioFileMismatch "\n无法把�?�有视频�?的文件与�?�有音频和视频的文件混在一起使用。请�?试使用 -nosound。\n"
#define MSGTR_NoSpeedWithFrameCopy "警告：无法�?�? -speed 能和 -oac copy 一起正常工作�?\n"\
"你的编�?过程�?�能会有错误�?\n"
#define MSGTR_ErrorWritingFile "%s：写入文件出错。\n"
#define MSGTR_FlushingVideoFrames "\n立�?�输出视频帧缓冲中的内容。\n"
#define MSGTR_FiltersHaveNotBeenConfiguredEmptyFile "过滤器尚未�?置�?文件为空？\n"
#define MSGTR_RecommendedVideoBitrate "%s CD 的推�??视频比特率为：%d\n"
#define MSGTR_VideoStreamResult "\n视频�?：%8.3f kbit/s （%d B/s） 大�?：%"PRIu64" 字节  %5.3f 秒  %d 帧\n"
#define MSGTR_AudioStreamResult "\n音频�?：%8.3f kbit/s （%d B/s） 大�?：%"PRIu64" 字节  %5.3f 秒\n"
#define MSGTR_EdlSkipStartEndCurrent "EDL 跳跃：开始�?置：%.2f  结�?��?置：%.2f   当�?�?置：V：%.2f  A：%.2f     \r"
#define MSGTR_OpenedStream "�?功：格�?：%d  数�?�：0x%X - 0x%x\n"
#define MSGTR_VCodecFramecopy "视频编解�?器：�?制帧（%dx%d %dbpp fourcc=%x）\n"
#define MSGTR_ACodecFramecopy "音频编解�?器：�?制帧（format=%x chans=%d rate=%d bits=%d B/s=%d sample-%d）\n"
#define MSGTR_CBRPCMAudioSelected "已选择 CBR PCM 音频。\n"
#define MSGTR_MP3AudioSelected "已选择 MP3音频。\n"
#define MSGTR_CannotAllocateBytes "无法分�?出 %d 字节。\n"
#define MSGTR_SettingAudioDelay "设置音频延迟为 %5.3fs。\n"
#define MSGTR_SettingVideoDelay "设置视频延迟为 %5.3fs。\n"
#define MSGTR_LimitingAudioPreload "�?制音频预加载长度为 0.4s。\n"
#define MSGTR_IncreasingAudioDensity "增加音频密度至 4。\n"
#define MSGTR_ZeroingAudioPreloadAndMaxPtsCorrection "强制音频预加载长度为 0，最大 PTS 校验长度为 0。\n"
#define MSGTR_LameVersion "LAME 版本 %s（%s）\n\n"
#define MSGTR_InvalidBitrateForLamePreset "错误：指定的比特率超出该预设�?置的有效范围。\n"\
"\n"\
"当使用该模�?时，你必须输入一个\"8\"到\"320\"之间的数值。\n"\
"\n"\
"获�?�更多信�?�，请�?试：\"-lameopts preset=help\"\n"
#define MSGTR_InvalidLamePresetOptions "错误：你没有给 preset 指定一个有效的�?置集和/或选项。\n"\
"\n"\
"�?�用的�?置集包括:\n"\
"\n"\
"   <fast>        standard\n"\
"   <fast>        extreme\n"\
"                 insane\n"\
"   <cbr>（ABR 模�?） - 默认使用的是 ABR 模�?。\n"\
"                      �?使用该模�?，�?��?指定一个比特率就行了。\n"\
"                      例如：“preset=185�?�?�用该预设�?置，\n"\
"                      使用 185 作为平�?�比特率。\n"\
"\n"\
"    一些例�?：\n"\
"\n"\
"    \"-lameopts fast:preset=standard  \"\n"\
" or \"-lameopts  cbr:preset=192       \"\n"\
" or \"-lameopts      preset=172       \"\n"\
" or \"-lameopts      preset=extreme   \"\n"\
"\n"\
"�?获�?�更多信�?�，请�?试使用：\"-lameopts preset=help\"\n"
#define MSGTR_LamePresetsLongInfo "\n"\
"预设�?置开关�?过设计以�??供最好的编�?质�?。\n"\
"\n"\
"大多数�?置已�?�?过严格的�?�盲�?��?�的测试和调整，以验�?并达到这个目标。\n"\
"\n"\
\
"这些�?置�?过�?断�?�级以跟上最近的开�?��?果，所以应该能给你�??供目�? LAME 所能�??供的\n"\
"将近最好的质�?。\n"\
"\n"\
\
"�?�?�用这些预设�?置：\n"\
"\n"\
"   使用 VBR 模�?（通常质�?最高）：\n"\
"\n"\
"     “preset=standard�? 该预设�?置在处�?�大多数音�?上，通常大多数人应该是感\n"\
"                         觉�?到差异的，其质�?已�?相当高。\n" \
"\n"\
\
"     “preset=extreme�?  如果你有�?好的�?�力和相当的设备，该预设�?置一般会比\n"\
"                        “standard�?模�?�??供更高一点的质�?。\n"\
"\n"\
\
\
"   使用 CBR 320kbps（预设�?置开关选项里的最高质�?）：\n"\
"\n"\
"     “preset=insane�?   对于大多数人在大多数情况下，该选项通常有些过度。但是\n"\
"                         如果你一定�?有最高质�?并且完全�?关心文件大�?，那这正\n"\
"                         是适�?�你的。\n"\
"\n"\
\
"   使用 ABR 模�?（给定比特率下�??供较高质�?，但�?�?� VBR 质�?高）：\n"\
"\n"\
"     \"preset=<kbps>\"  使用该预设�?置通常会在一个指定的比特率下�??错良好的质�?。\n"\
"                        根�?�输入的比特率，预设�?置将判断该情形下的最优设置。虽\n"\
"                        然该方法行之有效，但并没有 VBR 模�?那么�?�活，并且通常\n"\
"                        在高比特率下达�?到 VBR 所具有的�?�等质�?。\n"\
"\n"\
\
\
\
"以下选项在相应的�?置集中也�?�使用：\n"\
"\n"\
"   <fast>        standard\n"\
"   <fast>        extreme\n"\
"                 insane\n"\
"   <cbr>（ABR Mode）- 默认使用的是 ABR 模�?。�?使用该模�?，�?��?\n"\
"                      指定一个比特率就行了。例如：“preset=185�?\n"\
"                      �?�用该预设�?置，使用 185 作为平�?�比特率。\n"\
"\n"\
\
"   “fast�? - 在特定的�?置集中�?�用新的高速 VBR 模�?。速度开关的�??处是\n"\
"              比特率往往比普通模�?下�?高，并且质�?也会�?低一点。\n"\
\
\
"      警告：在当�?版本下，高速预设�?置�?�能产生比一般模�?高太多的比特率。\n"\
"\n"\
\
"   “cbr�?  - 如果你使用 ABR 模�?（�?上）时指定了一个比特率, 如 80�?\n"\
"              96�?112�?128�?160�?192�?224�?256�?320，你�?�以使用\n"\
"              “cbr�?选项强制以 CBR 模�?编�?代替标准 ABR 模�?。ABR\n"\
"              固然�??供更高的质�?，但是 CBR 在�?些情况下�?�能会相当有\n"\
"              用，比如当在因特网上传�? MP3 �?�?�能�??分�?�?时。\n"\
"\n"\
\
"    举例：\n"\
"\n"\
"    \"-lameopts fast:preset=standard  \"\n"\
" 或 \"-lameopts  cbr:preset=192       \"\n"\
" 或 \"-lameopts      preset=172       \"\n"\
" 或 \"-lameopts      preset=extreme   \"\n"\
"\n"\
"\n"\
"ABR 模�?下有一些�?�用的别�??：\n"\
"phone => 16kbps/mono        phon+/lw/mw-eu/sw => 24kbps/mono\n"\
"mw-us => 40kbps/mono        voice => 56kbps/mono\n"\
"fm/radio/tape => 112kbps    hifi => 160kbps\n"\
"cd => 192kbps               studio => 256kbps"
#define MSGTR_LameCantInit \
"无法设定 LAME 选项，请检查比特率/采样率，一些�?�常低的比特率（<32）需�?低采样率\n"\
"（如 -srate 8000）。\n"\
"如果其它方法都�?行，请�?试使用预设�?置。"
#define MSGTR_ConfigFileError "�?置文件错误"
#define MSGTR_ErrorParsingCommandLine "解�?命令行错误"
#define MSGTR_VideoStreamRequired "必须有视频�?�?\n"
#define MSGTR_ForcingInputFPS "输入帧率将视为 %5.3f。\n"
#define MSGTR_RawvideoDoesNotSupportAudio "RAWVIDEO 输出文件格�?�?支�?音频 - �?用音频。\n"
#define MSGTR_DemuxerDoesntSupportNosound "该�?分离器还�?支�? -nosound。\n"
#define MSGTR_MemAllocFailed "内存分�?失败。\n"
#define MSGTR_NoMatchingFilter "没找到匹�?的过滤器/音频输出格�?!\n"
#define MSGTR_MP3WaveFormatSizeNot30 "sizeof(MPEGLAYER3WAVEFORMAT)==%d!=30，C 编译器�?�能已�?��??？\n"
#define MSGTR_NoLavcAudioCodecName "音频 LAVC，没有指定编解�?器�??称�?\n"
#define MSGTR_LavcAudioCodecNotFound "音频 LAVC，无法找到编解�?器 %s 对应的编�?器。\n"
#define MSGTR_CouldntAllocateLavcContext "音频 LAVC，无法分�?�?行环境�?\n"
#define MSGTR_CouldntOpenCodec "无法打开编解�?器 %s，br=%d。\n"
#define MSGTR_CantCopyAudioFormat "音频格�? 0x%x 与‘-oac copy’�?兼容，请�?试�?��?‘-oac pcm’或者用‘-fafmttag’�?蔽该选项。\n"

// cfg-mencoder.h
#define MSGTR_MEncoderMP3LameHelp "\n\n"\
" vbr=<0-4>     �?��?�比特率方�?\n"\
"                0：cbr（�?�定比特率）\n"\
"                1：mt（Mark Taylor VBR 算法）\n"\
"                2：rh（Robert Hegemann VBR 算法 - 默认值）\n"\
"                3：abr（平�?�比特率）\n"\
"                4：mtrh（Mark Taylor Robert Hegemann VBR 算法）\n"\
"\n"\
" abr           平�?�比特率\n"\
"\n"\
" cbr           �?�定比特率\n"\
"               �?�时在�?��?�指定的 ABR 预设模�?中强制以 CBR 模�?编�?。\n"\
"\n"\
" br=<0-1024>   以 kBit 为�?��?设置比特率（仅用于 CBR 和 ABR）\n"\
"\n"\
" q=<0-9>       编�?质�?（0-最高，9-最低）（仅用于 VBR）\n"\
"\n"\
" aq=<0-9>      算法质�?（0-最好/�?行最慢，9-最差/�?行最快）\n"\
"\n"\
" ratio=<1-100> 压缩率\n"\
"\n"\
" vol=<0-10>    设置音频输入增益\n"\
"\n"\
" mode=<0-3>    （默认值：自动）\n"\
"                0：立体声\n"\
"                1：�?��?�立体声\n"\
"                2：�?�声�?�\n"\
"                3：�?�声�?�\n"\
"\n"\
" padding=<0-2>\n"\
"                0：无\n"\
"                1：所有\n"\
"                2：调整\n"\
"\n"\
" fast          在�?��?�指定 VBR 预设模�?中打开更快的编�?方�?，\n"\
"               质�?较低而比特率较高。\n"\
"\n"\
" preset=<value> �??供尽�?�能最高质�?的设置。\n"\
"                 medium：VBR 编�?，质�?好\n"\
"                 （比特率范围 150-180 kbps）\n"\
"                 standard：VBR 编�?，质�?高\n"\
"                 （比特率范围 170-210 kbps）\n"\
"                 extreme：VBR 编�?，质�?�?�常高\n"\
"                 （比特率范围 200-240 kbps）\n"\
"                 insane：CBR 编�?，质�?最高\n"\
"                 （比特率 320 kbps）\n"\
"                 <8-320>：以给定比特率为平�?�比特率的 ABR 编�?方�?。\n\n"

// codec-cfg.c
#define MSGTR_DuplicateFourcc "FourCC 代�?�?�?"
#define MSGTR_TooManyFourccs "FourCC/格�?代�?过多……"
#define MSGTR_ParseError "解�?错误"
#define MSGTR_ParseErrorFIDNotNumber "解�?错误（格�?代�? ID �?是一个数字？）"
#define MSGTR_ParseErrorFIDAliasNotNumber "解�?错误（格�?代�? ID 别�??�?是一个数字？）"
#define MSGTR_DuplicateFID "格�?代�? ID �?�?"
#define MSGTR_TooManyOut "输出过多……"
#define MSGTR_InvalidCodecName "\n编解�?器（%s）的�??称无效!\n"
#define MSGTR_CodecLacksFourcc "\n编解�?器（%s）没有 FourCC/格�?代�?�?\n"
#define MSGTR_CodecLacksDriver "\n编解�?器（%s）没有驱动程�?�?\n"
#define MSGTR_CodecNeedsDLL "\n编解�?器（%s）缺少一个‘dll’�?\n"
#define MSGTR_CodecNeedsOutfmt "\n编解�?器（%s）缺少一个‘outfmt’!\n"
#define MSGTR_CantAllocateComment "无法为注释文本分�?内存。"
#define MSGTR_GetTokenMaxNotLessThanMAX_NR_TOKEN "get_token()：max >= MAX_MR_TOKEN!"
#define MSGTR_CantGetMemoryForLine "无法获�?�内存�??供给‘line’：%s\n"
#define MSGTR_CantReallocCodecsp "无法�?新分�?‘*codecsp’：%s\n"
#define MSGTR_CodecNameNotUnique "编解�?器�??‘%s’�?�?。"
#define MSGTR_CantStrdupName "无法执行 strdup ->‘name’的�?制：%s\n"
#define MSGTR_CantStrdupInfo "无法执行 strdup ->‘info’的�?制：%s\n"
#define MSGTR_CantStrdupDriver "无法执行 strdup ->‘driver’的�?制：%s\n"
#define MSGTR_CantStrdupDLL "无法执行 strdup ->‘dll’的�?制：%s"
#define MSGTR_AudioVideoCodecTotals "%d 个音频和 %d 个视频编解�?器\n"
#define MSGTR_CodecDefinitionIncorrect "编解�?器没有正确定义。"
#define MSGTR_OutdatedCodecsConf "此 codecs.conf 太旧，与当�?�?�布的 MPlayer 版本�?兼容�?"

// fifo.c
#define MSGTR_CannotMakePipe "无法创建通信管�?��?\n"

// parser-mecmd.c, parser-mpcmd.c
#define MSGTR_NoFileGivenOnCommandLine "‘--’表示�?�?给出更多选项，但命令行上没有给出文件�??。\n"
#define MSGTR_TheLoopOptionMustBeAnInteger "这个循环选项必须是整数：%s\n"
#define MSGTR_UnknownOptionOnCommandLine "命令行上有未知选项：-%s\n"
#define MSGTR_ErrorParsingOptionOnCommandLine "解�?命令行选项出错：-%s\n"
#define MSGTR_InvalidPlayEntry "无效的播放�?�目 %s\n"
#define MSGTR_NotAnMEncoderOption "-%s �?是 MEncoder 的选项\n"
#define MSGTR_NoFileGiven "没有给出文件\n"

// m_config.c
#define MSGTR_SaveSlotTooOld "第 %d 级里找到的�?存�?置太旧：%d�?�?�?\n"
#define MSGTR_InvalidCfgfileOption "选项 %s 无法在�?置文件里使用。\n"
#define MSGTR_InvalidCmdlineOption "选项 %s 无法在命令行里使用。\n"
#define MSGTR_InvalidSuboption "错误:选项‘%s’没有�?选项‘%s’。\n"
#define MSGTR_MissingSuboptionParameter "错误：�?选项‘%s’（属于选项‘%s’）必须�?有一个�?�数�?\n"
#define MSGTR_MissingOptionParameter "错误：选项‘%s’必须�?有一个�?�数�?\n"
#define MSGTR_OptionListHeader "\n �??字                 类型            最�?       最大     全局  命令行 �?置文件\n\n"
#define MSGTR_TotalOptions "\n总共：%d 个选项\n"
#define MSGTR_ProfileInclusionTooDeep "警告：�?置集相互套用的层次太深。\n"
#define MSGTR_NoProfileDefined "未定义�?置集。\n"
#define MSGTR_AvailableProfiles "�?�用的�?置集：\n"
#define MSGTR_UnknownProfile "未知�?置集‘%s’。\n"
#define MSGTR_Profile "�?置集 %s：%s\n"

// m_property.c
#define MSGTR_PropertyListHeader "\n �??称                 类型            最�?        最大\n\n"
#define MSGTR_TotalProperties "\n总计：%d 个属性\n"

// loader/ldt_keeper.c
#define MSGTR_LOADER_DYLD_Warning "警告：�?试使用 DLL 编解�?器，但是环境�?��?\n         DYLD_BIND_AT_LAUNCH 未设定。 这很�?�能造�?程�?崩溃。\n"


// ====================== GUI messages/buttons ========================

// --- labels ---
#define MSGTR_About "关于"
#define MSGTR_FileSelect "选择文件..."
#define MSGTR_SubtitleSelect "选择字幕..."
#define MSGTR_OtherSelect "选择..."
#define MSGTR_AudioFileSelect "选择外部音频通�?�..."
#define MSGTR_FontSelect "选择字体..."
// Note: If you change MSGTR_PlayList please see if it still fits MSGTR_MENU_PlayList
#define MSGTR_PlayList "播放列表"
#define MSGTR_Equalizer "�?�衡器"
#define MSGTR_ConfigureEqualizer "�?置�?�衡器"
#define MSGTR_SkinBrowser "界�?�外观�?置�?览器"
#define MSGTR_Network "正传�?网络媒体�?..."
// Note: If you change MSGTR_Preferences please see if it still fits MSGTR_MENU_Preferences
#define MSGTR_Preferences "首选项"
#define MSGTR_AudioPreferences "音频驱动�?置"
#define MSGTR_NoMediaOpened "未打开媒体内容"
#define MSGTR_Title "标题 %d"
#define MSGTR_NoChapter "无章节"
#define MSGTR_Chapter "章节 %d"
#define MSGTR_NoFileLoaded "未载入文件"

// --- buttons ---
#define MSGTR_Ok "确定"
#define MSGTR_Cancel "�?�消"
#define MSGTR_Add "添加"
#define MSGTR_Remove "删除"
#define MSGTR_Clear "清空"
#define MSGTR_Config "�?置"
#define MSGTR_ConfigDriver "�?置驱动程�?"
#define MSGTR_Browse "�?览"

// --- error messages ---
#define MSGTR_NEMDB "抱歉，没有足够的内存用作绘图缓冲。\n"
#define MSGTR_NEMFMR "抱歉，没有足够的内存用于�?��?�渲染。"
#define MSGTR_IDFGCVD "抱歉，未找到与 GUI 兼容的视频输出驱动。\n"
#define MSGTR_NEEDLAVC "抱歉，未�?新编�?�?无法用你的 DXR3/H+ 设备播放�?� MPEG 文件。\n请�?�用 DXR3/H+ �?置盒中的 lavc 编解�?器。"
#define MSGTR_ICONERROR "未找到图标‘%s’（大�?为 %d）或格�?�?支�?。\n"

// --- skin loader error messages
#define MSGTR_SKIN_ERRORMESSAGE "界�?�外观�?置文件错误，行 %d：%s"
#define MSGTR_SKIN_ERROR_SECTION "没有指定‘%s’的区域。\n"
#define MSGTR_SKIN_ERROR_WINDOW "没有指定‘%s’的窗�?�。\n"
#define MSGTR_SKIN_ERROR_ITEM "该项目‘%s’�?支�?。\n"
#define MSGTR_SKIN_UNKNOWN_ITEM "未知项目‘%s’\n"
#define MSGTR_SKIN_UNKNOWN_NAME "未知�??称‘%s’\n"
#define MSGTR_SKIN_SkinFileNotFound "未找到界�?�外观文件 %s。\n"
#define MSGTR_SKIN_SkinFileNotReadable "无法读�?�界�?�外观文件 %s。\n"
#define MSGTR_SKIN_BITMAP_16bit  "�?图 %s 的颜色精度�?于等于 16 比特，因而无法支�?。\n"
#define MSGTR_SKIN_BITMAP_FileNotFound  "未找到�?图文件 %s\n"
#define MSGTR_SKIN_BITMAP_PNGReadError "%s 中的 PNG 数�?�读�?�错误\n"
#define MSGTR_SKIN_BITMAP_ConversionError "%s 中的数�?�执行 24 比特至 32 比特转�?�出错\n"
#define MSGTR_SKIN_UnknownMessage "未知信�?�‘%s’\n"
#define MSGTR_SKIN_NotEnoughMemory "内存�?足\n"
#define MSGTR_SKIN_TooManyItemsDeclared "声明项目过多。\n"
#define MSGTR_SKIN_FONT_TooManyFontsDeclared "字体的声明过多。\n"
#define MSGTR_SKIN_FONT_FontFileNotFound "未找到字体�??述文件。\n"
#define MSGTR_SKIN_FONT_FontImageNotFound "未找到字体图�?文件。\n"
#define MSGTR_SKIN_FONT_NonExistentFont "字体标识符（%s）�?存在\n"
#define MSGTR_SKIN_UnknownParameter "未知�?�数（%s）\n"
#define MSGTR_SKIN_SKINCFG_SkinNotFound "未找到界�?�外观‘%s’。\n"
#define MSGTR_SKIN_SKINCFG_SelectedSkinNotFound "未找到选定的界�?�外观‘%s’，�?试使用‘default’�?�数...\n"
#define MSGTR_SKIN_SKINCFG_SkinCfgError "处�?��?置文件中的界�?�外观‘%s’时出错\n"
#define MSGTR_SKIN_LABEL "界�?�外观："

// --- GTK menus
#define MSGTR_MENU_AboutMPlayer "关于 MPlayer"
#define MSGTR_MENU_Open "打开..."
#define MSGTR_MENU_PlayFile "播放文件..."
#define MSGTR_MENU_PlayCD "播放 CD..."
#define MSGTR_MENU_PlayVCD "播放 VCD..."
#define MSGTR_MENU_PlayDVD "播放 DVD..."
#define MSGTR_MENU_PlayURL "播放网络链接..."
#define MSGTR_MENU_LoadSubtitle "加载字幕..."
#define MSGTR_MENU_DropSubtitle "丢弃字幕..."
#define MSGTR_MENU_LoadExternAudioFile "加载外部音频文件..."
#define MSGTR_MENU_Playing "播放控制"
#define MSGTR_MENU_Play "播放"
#define MSGTR_MENU_Pause "暂�?�"
#define MSGTR_MENU_Stop "�?�止"
#define MSGTR_MENU_NextStream "下一个"
#define MSGTR_MENU_PrevStream "上一个"
#define MSGTR_MENU_Size "尺寸"
#define MSGTR_MENU_HalfSize   "一�?�尺寸"
#define MSGTR_MENU_NormalSize "正常尺寸"
#define MSGTR_MENU_DoubleSize "�?��?尺寸"
#define MSGTR_MENU_FullScreen "全�?"
#define MSGTR_MENU_CD "CD"
#define MSGTR_MENU_DVD "DVD"
#define MSGTR_MENU_VCD "VCD"
#define MSGTR_MENU_PlayDisc "打开光盘..."
#define MSGTR_MENU_ShowDVDMenu "显示 DVD �?��?�"
#define MSGTR_MENU_Titles "所有标题"
#define MSGTR_MENU_Title "标题 %2d"
#define MSGTR_MENU_None "（无）"
#define MSGTR_MENU_Chapters "所有章节"
#define MSGTR_MENU_Chapter "章节 %2d"
#define MSGTR_MENU_AudioLanguages "音频语言"
#define MSGTR_MENU_SubtitleLanguages "字幕语言"
#define MSGTR_MENU_PlayList MSGTR_PlayList
#define MSGTR_MENU_SkinBrowser "界�?�外观�?置�?览器"
#define MSGTR_MENU_Preferences MSGTR_Preferences
#define MSGTR_MENU_Exit "退出"
#define MSGTR_MENU_Mute "�?�音"
#define MSGTR_MENU_Original "原始的"
#define MSGTR_MENU_AspectRatio "宽高比"
#define MSGTR_MENU_AudioTrack "音频轨迹"
#define MSGTR_MENU_Track "轨迹 %d"
#define MSGTR_MENU_VideoTrack "视频轨迹"
#define MSGTR_MENU_Subtitles "字幕"

// --- equalizer
// Note: If you change MSGTR_EQU_Audio please see if it still fits MSGTR_PREFERENCES_Audio
#define MSGTR_EQU_Audio "音频"
// Note: If you change MSGTR_EQU_Video please see if it still fits MSGTR_PREFERENCES_Video
#define MSGTR_EQU_Video "视频"
#define MSGTR_EQU_Contrast "对比度："
#define MSGTR_EQU_Brightness "亮度："
#define MSGTR_EQU_Hue "色调："
#define MSGTR_EQU_Saturation "饱和度："
#define MSGTR_EQU_Front_Left "�?左"
#define MSGTR_EQU_Front_Right "�?�?�"
#define MSGTR_EQU_Back_Left "�?�左"
#define MSGTR_EQU_Back_Right "�?��?�"
#define MSGTR_EQU_Center "中置"
#define MSGTR_EQU_Bass "低音炮"
#define MSGTR_EQU_All "所有"
#define MSGTR_EQU_Channel1 "声�?� 1："
#define MSGTR_EQU_Channel2 "声�?� 2："
#define MSGTR_EQU_Channel3 "声�?� 3："
#define MSGTR_EQU_Channel4 "声�?� 4："
#define MSGTR_EQU_Channel5 "声�?� 5："
#define MSGTR_EQU_Channel6 "声�?� 6："

// --- playlist
#define MSGTR_PLAYLIST_Path "路径"
#define MSGTR_PLAYLIST_Selected "所选文件"
#define MSGTR_PLAYLIST_Files "所有文件"
#define MSGTR_PLAYLIST_DirectoryTree "目录树"

// --- preferences
#define MSGTR_PREFERENCES_Audio MSGTR_EQU_Audio
#define MSGTR_PREFERENCES_Video MSGTR_EQU_Video
#define MSGTR_PREFERENCES_SubtitleOSD "字幕和 OSD �?��?�"
#define MSGTR_PREFERENCES_Codecs "编解�?器和�?分离器"
// Note: If you change MSGTR_PREFERENCES_Misc see if it still fits MSGTR_PREFERENCES_FRAME_Misc
#define MSGTR_PREFERENCES_Misc "�?�项"
#define MSGTR_PREFERENCES_None "无"
#define MSGTR_PREFERENCES_DriverDefault "驱动程�?默认�?置"
#define MSGTR_PREFERENCES_AvailableDrivers "�?�用驱动："
#define MSGTR_PREFERENCES_DoNotPlaySound "�?播放声音"
#define MSGTR_PREFERENCES_NormalizeSound "音�?规格化"
#define MSGTR_PREFERENCES_EnableEqualizer "�?�用�?�衡器"
#define MSGTR_PREFERENCES_SoftwareMixer "�?�用软件混音器"
#define MSGTR_PREFERENCES_ExtraStereo "�?�用立体声加强"
#define MSGTR_PREFERENCES_Coefficient "�?�数："
#define MSGTR_PREFERENCES_AudioDelay "音频延迟"
#define MSGTR_PREFERENCES_DoubleBuffer "�?�用�?��?缓冲"
#define MSGTR_PREFERENCES_DirectRender "�?�用直接渲染"
#define MSGTR_PREFERENCES_FrameDrop "�?�用丢帧"
#define MSGTR_PREFERENCES_HFrameDrop "�?�用强制丢帧（�?�险）"
#define MSGTR_PREFERENCES_Flip "上下翻转图�?"
#define MSGTR_PREFERENCES_Panscan "全景模�?："
#define MSGTR_PREFERENCES_OSD_LEVEL0 "仅显示字幕"
#define MSGTR_PREFERENCES_OSD_LEVEL1 "显示音�?和定�?�?�"
#define MSGTR_PREFERENCES_OSD_LEVEL2 "显示音�?�?定�?�?��?计时器和进度百分比"
#define MSGTR_PREFERENCES_OSD_LEVEL3 "显示音�?�?定�?�?��?计时器�?进度百分比和时间总长"
#define MSGTR_PREFERENCES_Subtitle "字幕："
#define MSGTR_PREFERENCES_SUB_Delay "延迟："
#define MSGTR_PREFERENCES_SUB_FPS "帧率："
#define MSGTR_PREFERENCES_SUB_POS "�?置："
#define MSGTR_PREFERENCES_SUB_AutoLoad "�?止自动装载字幕"
#define MSGTR_PREFERENCES_SUB_Unicode "Unicode 字幕"
#define MSGTR_PREFERENCES_SUB_MPSUB "转�?�给定的字幕为 MPlayer 字幕格�?"
#define MSGTR_PREFERENCES_SUB_SRT "转�?�给定的字幕为基于时间戳的 SubViewer (SRT) 格�?"
#define MSGTR_PREFERENCES_SUB_Overlap "开关字幕�?�?�模�?"
#define MSGTR_PREFERENCES_SUB_USE_ASS "渲染 SSA/ASS 字幕"
#define MSGTR_PREFERENCES_SUB_ASS_USE_MARGINS "使用留边"
#define MSGTR_PREFERENCES_SUB_ASS_TOP_MARGIN "底部："
#define MSGTR_PREFERENCES_SUB_ASS_BOTTOM_MARGIN "底部："
#define MSGTR_PREFERENCES_Font "字体："
#define MSGTR_PREFERENCES_FontFactor "字体�?�数："
#define MSGTR_PREFERENCES_PostProcess "�?�用�?�期处�?�"
#define MSGTR_PREFERENCES_AutoQuality "自动控制质�?："
#define MSGTR_PREFERENCES_NI "使用�?�交错的 AVI 解�?器"
#define MSGTR_PREFERENCES_IDX "如果需�?的�?，�?建索引表"
#define MSGTR_PREFERENCES_VideoCodecFamily "视频编解�?器类："
#define MSGTR_PREFERENCES_AudioCodecFamily "音频编解�?器类："
#define MSGTR_PREFERENCES_FRAME_OSD_Level "OSD 级别"
#define MSGTR_PREFERENCES_FRAME_Subtitle "字幕"
#define MSGTR_PREFERENCES_FRAME_Font "字体"
#define MSGTR_PREFERENCES_FRAME_PostProcess "�?�期处�?�"
#define MSGTR_PREFERENCES_FRAME_CodecDemuxer "编解�?器和�?分离器"
#define MSGTR_PREFERENCES_FRAME_Cache "缓冲"
#define MSGTR_PREFERENCES_FRAME_Misc MSGTR_PREFERENCES_Misc
#define MSGTR_PREFERENCES_Audio_Device "设备："
#define MSGTR_PREFERENCES_Audio_Mixer "混音器："
#define MSGTR_PREFERENCES_Audio_MixerChannel "混音器声�?�："
#define MSGTR_PREFERENCES_Message "请注�?有些功能�?�有�?�?�播放�?��?能生效�?"
#define MSGTR_PREFERENCES_DXR3_VENC "视频编解�?器："
#define MSGTR_PREFERENCES_DXR3_LAVC "使用 LAVC（FFmpeg）"
#define MSGTR_PREFERENCES_FontEncoding1 "Unicode"
#define MSGTR_PREFERENCES_FontEncoding2 "西欧(ISO-8859-1)"
#define MSGTR_PREFERENCES_FontEncoding3 "西欧(ISO-8859-15)"
#define MSGTR_PREFERENCES_FontEncoding4 "中欧(ISO-8859-2)"
#define MSGTR_PREFERENCES_FontEncoding5 "中欧(ISO-8859-3)"
#define MSGTR_PREFERENCES_FontEncoding6 "波罗的语(ISO-8859-4)"
#define MSGTR_PREFERENCES_FontEncoding7 "斯拉夫语(ISO-8859-5)"
#define MSGTR_PREFERENCES_FontEncoding8 "阿拉伯语(ISO-8859-6)"
#define MSGTR_PREFERENCES_FontEncoding9 "现代希腊语(ISO-8859-7)"
#define MSGTR_PREFERENCES_FontEncoding10 "土耳其语(ISO-8859-9)"
#define MSGTR_PREFERENCES_FontEncoding11 "波罗的语(ISO-8859-13)"
#define MSGTR_PREFERENCES_FontEncoding12 "凯尔特语(ISO-8859-14)"
#define MSGTR_PREFERENCES_FontEncoding13 "希伯�?�语(ISO-8859-8)"
#define MSGTR_PREFERENCES_FontEncoding14 "俄语(KOI8-R)"
#define MSGTR_PREFERENCES_FontEncoding15 "俄语(KOI8-U/RU)"
#define MSGTR_PREFERENCES_FontEncoding16 "简体中文(CP936)"
#define MSGTR_PREFERENCES_FontEncoding17 "�?体中文(BIG5)"
#define MSGTR_PREFERENCES_FontEncoding18 "日语(SHIFT-JIS)"
#define MSGTR_PREFERENCES_FontEncoding19 "韩语(CP949)"
#define MSGTR_PREFERENCES_FontEncoding20 "泰语(CP874)"
#define MSGTR_PREFERENCES_FontEncoding21 "Windows 的西里尔语(CP1251)"
#define MSGTR_PREFERENCES_FontEncoding22 "Windows 的西里尔/中欧语(CP1250)"
#define MSGTR_PREFERENCES_FontEncoding23 "Windows 的阿拉伯语(CP1256)"
#define MSGTR_PREFERENCES_FontNoAutoScale "�?自动缩放"
#define MSGTR_PREFERENCES_FontPropWidth "与影片宽度相称"
#define MSGTR_PREFERENCES_FontPropHeight "与影片高度相称"
#define MSGTR_PREFERENCES_FontPropDiagonal "与影片对角线相称"
#define MSGTR_PREFERENCES_FontEncoding "编�?："
#define MSGTR_PREFERENCES_FontBlur "模糊："
#define MSGTR_PREFERENCES_FontOutLine "轮廓："
#define MSGTR_PREFERENCES_FontTextScale "文字缩放："
#define MSGTR_PREFERENCES_FontOSDScale "OSD 缩放："
#define MSGTR_PREFERENCES_Cache "�?�用/�?用缓存"
#define MSGTR_PREFERENCES_CacheSize "缓存大�?："
#define MSGTR_PREFERENCES_LoadFullscreen "以全�?方�?�?�动"
#define MSGTR_PREFERENCES_SaveWinPos "�?存窗�?��?置"
#define MSGTR_PREFERENCES_XSCREENSAVER "�?�用 XScreenSaver �?�?"
#define MSGTR_PREFERENCES_PlayBar "�?�用播放�?�"
#define MSGTR_PREFERENCES_NoIdle "播放�?�退出"
#define MSGTR_PREFERENCES_AutoSync "�?�用/�?用自动�?�步"
#define MSGTR_PREFERENCES_AutoSyncValue "自动�?�步："
#define MSGTR_PREFERENCES_CDROMDevice "CD-ROM 设备："
#define MSGTR_PREFERENCES_DVDDevice "DVD 设备："
#define MSGTR_PREFERENCES_FPS "电影帧率："
#define MSGTR_PREFERENCES_ShowVideoWindow "当程�?窗动未激活时显示视频窗�?�"
#define MSGTR_PREFERENCES_ArtsBroken "新版 aRts 与 GTK 1.x �?兼容，"\
           "并会使 GMPlayer 崩溃�?"

// -- aboutbox
#define MSGTR_ABOUT_UHU "GUI 的开�?�由 UHU Linux 赞助\n"
#define MSGTR_ABOUT_Contributors "代�?和文档贡献者\n"
#define MSGTR_ABOUT_Codecs_libs_contributions "编解�?器和第三方程�?库\n"
#define MSGTR_ABOUT_Translations "翻译\n"
#define MSGTR_ABOUT_Skins "界�?�外观\n"

// --- messagebox
#define MSGTR_MSGBOX_LABEL_FatalError "致命错误�?"
#define MSGTR_MSGBOX_LABEL_Error "错误�?"
#define MSGTR_MSGBOX_LABEL_Warning "警告�?"

// cfg.c
#define MSGTR_UnableToSaveOption "[cfg] 无法�?存‘%s’选项。\n"

// interface.c
#define MSGTR_DeletingSubtitles "删除字幕。\n"
#define MSGTR_LoadingSubtitles "加载字幕：%s\n"
#define MSGTR_AddingVideoFilter "添加视频滤镜：%s\n"

// mw.c
#define MSGTR_NotAFile "这好�?�?是文件：%s�?\n"

// ws.c
#define MSGTR_WS_RemoteDisplay "[ws] 远程显示界�?�，�?用 XMITSHM。\n"
#define MSGTR_WS_NoXshm "[ws] 抱歉，你的系统�?支�? X 共享内存扩展组件。\n"
#define MSGTR_WS_NoXshape "[ws] 抱歉，你的系统�?支�? XShape 扩展组件。\n"
#define MSGTR_WS_ColorDepthTooLow "[ws] 抱歉，色彩深度太低。\n"
#define MSGTR_WS_TooManyOpenWindows "[ws] 打开窗�?�太多。\n"
#define MSGTR_WS_ShmError "[ws] 共享内存扩展组件错误\n"
#define MSGTR_WS_NotEnoughMemoryDrawBuffer "[ws] 抱歉，内存�?足以用于绘制缓冲。\n"
#define MSGTR_WS_DpmsUnavailable "DPMS �?�?�用？\n"
#define MSGTR_WS_DpmsNotEnabled "无法�?�用 DPMS。\n"

// wsxdnd.c
#define MSGTR_WS_NotAFile "这好�?�?是一个文件...\n"
#define MSGTR_WS_DDNothing "D&D：未返回任何东西�?\n"

// Win32 GUI
#define MSGTR_Close "关闭"
#define MSGTR_Default "默认值"
#define MSGTR_Down "�?�下"
#define MSGTR_Load "加载"
#define MSGTR_Save "�?存"
#define MSGTR_Up "�?�上"
#define MSGTR_DirectorySelect "选择目录..."
#define MSGTR_PlaylistSave "�?存播放列表..."
#define MSGTR_PlaylistSelect "选择播放列表..."
#define MSGTR_SelectTitleChapter "选择标题/章节..."
#define MSGTR_MENU_DebugConsole "调试控制终端"
#define MSGTR_MENU_OnlineHelp "线上帮助内容"
#define MSGTR_MENU_PlayDirectory "播放目录内容..."
#define MSGTR_MENU_SeekBack "�?��?�定�?"
#define MSGTR_MENU_SeekForw "�?��?定�?"
#define MSGTR_MENU_ShowHide "显示/�?�?"
#define MSGTR_MENU_SubtitlesOnOff "显示字幕开/关"
#define MSGTR_PLAYLIST_AddFile "添加文件..."
#define MSGTR_PLAYLIST_AddURL "添加 URL..."
#define MSGTR_PREFERENCES_Priority "进程优先级："
#define MSGTR_PREFERENCES_PriorityHigh "高"
#define MSGTR_PREFERENCES_PriorityLow "低"
#define MSGTR_PREFERENCES_PriorityNormal "标准"
#define MSGTR_PREFERENCES_PriorityNormalAbove "高于标准"
#define MSGTR_PREFERENCES_PriorityNormalBelow "低于标准"
#define MSGTR_PREFERENCES_ShowInVideoWin "在视频窗�?�中显示（仅用于 DirectX）"


// ======================= video output drivers ========================

#define MSGTR_VOincompCodec "所选的视频输出设备与该编解�?器�?兼容。\n"\
                "请�?试在滤镜列表尾部添加缩放滤镜，\n"\
                "例如，用 -vf spp,scale 代替 -vf spp。\n"
#define MSGTR_VO_GenericError "该错误已�?�?�生"
#define MSGTR_VO_UnableToAccess "无法访问"
#define MSGTR_VO_ExistsButNoDirectory "已�?存在，但�?是一个目录。"
#define MSGTR_VO_DirExistsButNotWritable "输出目录已�?存在，但是�?�?�写。"
#define MSGTR_VO_DirExistsAndIsWritable "输出目录已�?存在并且�?�写。"
#define MSGTR_VO_CantCreateDirectory "无法创建输出目录。"
#define MSGTR_VO_CantCreateFile "无法创建输出文件。"
#define MSGTR_VO_DirectoryCreateSuccess "输出目录创建�?功。"
#define MSGTR_VO_ValueOutOfRange "值超出范围"
#define MSGTR_VO_NoValueSpecified "未指定值。"
#define MSGTR_VO_UnknownSuboptions "未知�?选项"

// aspect.c
#define MSGTR_LIBVO_ASPECT_NoSuitableNewResFound "[ASPECT] 警告：无法找到新的�?�适的分辨率�?\n"
#define MSGTR_LIBVO_ASPECT_NoNewSizeFoundThatFitsIntoRes "[ASPECT] 错误：无法找到适�?�分辨率的新尺寸!\n"

// font_load_ft.c
#define MSGTR_LIBVO_FONT_LOAD_FT_NewFaceFailed "调用 New_Face 失败。�?�能字体文件的路径�?对。\n请�??供文本字体文件（~/.mplayer/subfont.tt）。\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_NewMemoryFaceFailed "调用 New_Memory_Face 失败。\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_SubFaceFailed "字幕字体：调用 load_sub_face 失败。\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_SubFontCharsetFailed "字幕字体：调用 prepare_charset 失败。\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_CannotPrepareSubtitleFont "无法设置字幕字体。\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_CannotPrepareOSDFont "无法设置 OSD 字体。\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_CannotGenerateTables "无法生�?映射表。\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_DoneFreeTypeFailed "调用 FT_Done_FreeType 失败。\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_FontconfigNoMatch "Fontconfig 选�?�字体失败。请�?试�?使用...\n"

// sub.c
#define MSGTR_VO_SUB_Seekbar "定�?�?�"
#define MSGTR_VO_SUB_Play "播放"
#define MSGTR_VO_SUB_Pause "暂�?�"
#define MSGTR_VO_SUB_Stop "�?�止"
#define MSGTR_VO_SUB_Rewind "倒回"
#define MSGTR_VO_SUB_Forward "快进"
#define MSGTR_VO_SUB_Clock "计时"
#define MSGTR_VO_SUB_Contrast "对比度"
#define MSGTR_VO_SUB_Saturation "饱和度"
#define MSGTR_VO_SUB_Volume "音�?"
#define MSGTR_VO_SUB_Brightness "亮度"
#define MSGTR_VO_SUB_Hue "色调"
#define MSGTR_VO_SUB_Balance "�?�衡"

// vo_3dfx.c
#define MSGTR_LIBVO_3DFX_Only16BppSupported "[VO_3DFX] �?�支�? 16bpp�?"
#define MSGTR_LIBVO_3DFX_VisualIdIs "[VO_3DFX] �?�视化 ID 是 %lx。\n"
#define MSGTR_LIBVO_3DFX_UnableToOpenDevice "[VO_3DFX] 无法打开 /dev/3dfx。\n"
#define MSGTR_LIBVO_3DFX_Error "[VO_3DFX] 错误：%d。\n"
#define MSGTR_LIBVO_3DFX_CouldntMapMemoryArea "[VO_3DFX] 未能映射 3dfx 内存区域：%p,%p,%d。\n"
#define MSGTR_LIBVO_3DFX_DisplayInitialized "[VO_3DFX] �?始化完毕：%p。\n"
#define MSGTR_LIBVO_3DFX_UnknownSubdevice "[VO_3DFX] 未知�?设备：%s。\n"

// vo_aa.c
#define MSGTR_VO_AA_HelpHeader "\n\n以下是 aalib vo_aa 的�?选项：\n"
#define MSGTR_VO_AA_AdditionalOptions "vo_aa �??供的附加选项：\n" \
"  help        显示此帮助信�?�\n" \
"  osdcolor    设定 OSD 颜色\n  subcolor    设定字幕颜色\n" \
"        颜色�?�数有：\n           0 ：标准\n" \
"           1 ：模糊\n           2 ：加粗\n           3 ：字体加粗\n" \
"           4 ：�??色\n           5 ：特殊\n\n\n"

// vo_dxr3.c
#define MSGTR_LIBVO_DXR3_UnableToLoadNewSPUPalette "[VO_DXR3] 无法载入新的 SPU 调色�?��?\n"
#define MSGTR_LIBVO_DXR3_UnableToSetPlaymode "[VO_DXR3] 无法设置播放模�?�?\n"
#define MSGTR_LIBVO_DXR3_UnableToSetSubpictureMode "[VO_DXR3] 无法设置字幕模�?�?\n"
#define MSGTR_LIBVO_DXR3_UnableToGetTVNorm "[VO_DXR3] 无法获�?�电视制�?�?\n"
#define MSGTR_LIBVO_DXR3_AutoSelectedTVNormByFrameRate "[VO_DXR3] 根�?�帧速率自动选择电视制�?："
#define MSGTR_LIBVO_DXR3_UnableToSetTVNorm "[VO_DXR3] 无法设置电视制�?�?\n"
#define MSGTR_LIBVO_DXR3_SettingUpForNTSC "[VO_DXR3] 设置为 NTSC 模�?。\n"
#define MSGTR_LIBVO_DXR3_SettingUpForPALSECAM "[VO_DXR3] 设置为 PAL/SECAM 模�?。\n"
#define MSGTR_LIBVO_DXR3_SettingAspectRatioTo43 "[VO_DXR3] 宽高比设为 4:3。\n"
#define MSGTR_LIBVO_DXR3_SettingAspectRatioTo169 "[VO_DXR3] 宽高比设为 16:9。\n"
#define MSGTR_LIBVO_DXR3_OutOfMemory "[VO_DXR3] 内存耗尽\n"
#define MSGTR_LIBVO_DXR3_UnableToAllocateKeycolor "[VO_DXR3] 无法分�?关键色�?\n"
#define MSGTR_LIBVO_DXR3_UnableToAllocateExactKeycolor "[VO_DXR3] 无法精确分�?关键色，使用最接近的匹�?（0x%lx）。\n"
#define MSGTR_LIBVO_DXR3_Uninitializing "[VO_DXR3] 正在终止。\n"
#define MSGTR_LIBVO_DXR3_FailedRestoringTVNorm "[VO_DXR3] 未能�?��?电视制�?�?\n"
#define MSGTR_LIBVO_DXR3_EnablingPrebuffering "[VO_DXR3] �?�用预缓冲模�?。\n"
#define MSGTR_LIBVO_DXR3_UsingNewSyncEngine "[VO_DXR3] 使用新的�?�步引擎。\n"
#define MSGTR_LIBVO_DXR3_UsingOverlay "[VO_DXR3] 使用覆盖模�?。\n"
#define MSGTR_LIBVO_DXR3_ErrorYouNeedToCompileMplayerWithX11 "[VO_DXR3] 错误：覆盖需�?在安装 X11 库和头文件的�?�件下编译。\n"
#define MSGTR_LIBVO_DXR3_WillSetTVNormTo "[VO_DXR3] 将电视制�?设置为："
#define MSGTR_LIBVO_DXR3_AutoAdjustToMovieFrameRatePALPAL60 "自动调节画�?�的帧率（PAL/PAL-60）"
#define MSGTR_LIBVO_DXR3_AutoAdjustToMovieFrameRatePALNTSC "自动调节画�?�的帧率（PAL/NTSC）"
#define MSGTR_LIBVO_DXR3_UseCurrentNorm "使用当�?制�?。"
#define MSGTR_LIBVO_DXR3_UseUnknownNormSuppliedCurrentNorm "未知制�?，使用当�?制�?。"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingTrying "[VO_DXR3] 打开 %s 以写入错误，�?试 /dev/em8300。\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingTryingMV "[VO_DXR3] 打开 %s 以写入错误，�?试 /dev/em8300_mv。\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingAsWell "[VO_DXR3] 打开 /dev/em8300 以写入�?�样错误�?\n跳出。\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingAsWellMV "[VO_DXR3] 打开 /dev/em8300_mv 以写入�?�样错误�?\n跳出。\n"
#define MSGTR_LIBVO_DXR3_Opened "[VO_DXR3] 打开：%s。\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingTryingSP "[VO_DXR3] 打开 %s 写入错误，�?试 /dev/em8300_sp。\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingAsWellSP "[VO_DXR3] 打开 /dev/em8300_sp 以写入�?�样错误�?\n跳出。\n"
#define MSGTR_LIBVO_DXR3_UnableToOpenDisplayDuringHackSetup "[VO_DXR3] 设置覆盖模�?破解方�?时无法打开显示设备�?\n"
#define MSGTR_LIBVO_DXR3_UnableToInitX11 "[VO_DXR3] 无法�?始化 X11�?\n"
#define MSGTR_LIBVO_DXR3_FailedSettingOverlayAttribute "[VO_DXR3] 未能设置覆盖模�?属性。\n"
#define MSGTR_LIBVO_DXR3_FailedSettingOverlayScreen "[VO_DXR3] 未能设置覆盖�?幕�?\n退出。\n"
#define MSGTR_LIBVO_DXR3_FailedEnablingOverlay "[VO_DXR3] 未能�?�用覆盖模�?�?\n退出。\n"
#define MSGTR_LIBVO_DXR3_FailedSettingOverlayBcs "[VO_DXR3] 未能设置覆盖模�? bcs�?\n"
#define MSGTR_LIBVO_DXR3_FailedGettingOverlayYOffsetValues "[VO_DXR3] 未能获�?�覆盖模�?的 Y-�??移�?�?\n退出。\n"
#define MSGTR_LIBVO_DXR3_FailedGettingOverlayXOffsetValues "[VO_DXR3] 未能获�?�覆盖模�?的 X-�??移�?�?\n退出。\n"
#define MSGTR_LIBVO_DXR3_FailedGettingOverlayXScaleCorrection "[VO_DXR3] 未能获�?�覆盖模�?的 X-比例校正�?\n退出。\n"
#define MSGTR_LIBVO_DXR3_YOffset "[VO_DXR3] Y-�??移�?：%d。\n"
#define MSGTR_LIBVO_DXR3_XOffset "[VO_DXR3] X-�??移�?：%d。\n"
#define MSGTR_LIBVO_DXR3_XCorrection "[VO_DXR3] X-比例校正：%d。\n"
#define MSGTR_LIBVO_DXR3_FailedResizingOverlayWindow "[VO_DXR3] 未能设置覆盖窗�?�大�?�?\n"
#define MSGTR_LIBVO_DXR3_FailedSetSignalMix "[VO_DXR3] 未能设置信�?�混�?�模�?�?\n"

// vo_jpeg.c
#define MSGTR_VO_JPEG_ProgressiveJPEG "�?�用�?显 JPEG。"
#define MSGTR_VO_JPEG_NoProgressiveJPEG "�?�用�?显 JPEG。"
#define MSGTR_VO_JPEG_BaselineJPEG "�?�用基本 JPEG。"
#define MSGTR_VO_JPEG_NoBaselineJPEG "�?�用基本 JPEG。"

// vo_mga.c
#define MSGTR_LIBVO_MGA_AspectResized "[VO_MGA] aspect()：改�?�大�?为 %dx%d。\n"
#define MSGTR_LIBVO_MGA_Uninit "[VO] 终止�?\n"

// mga_template.c
#define MSGTR_LIBVO_MGA_ErrorInConfigIoctl "[MGA] mga_vid_config ioctl 错误（mga_vid.o 版本错误？）"
#define MSGTR_LIBVO_MGA_CouldNotGetLumaValuesFromTheKernelModule "[MGA] 无法在内核模�?�中获得亮度值�?\n"
#define MSGTR_LIBVO_MGA_CouldNotSetLumaValuesFromTheKernelModule "[MGA] 无法在内核模�?�中设置亮度值�?\n"
#define MSGTR_LIBVO_MGA_ScreenWidthHeightUnknown "[MGA] �?幕宽度/高度未知�?\n"
#define MSGTR_LIBVO_MGA_InvalidOutputFormat "[MGA] 无效的输出格�? %0X\n"
#define MSGTR_LIBVO_MGA_IncompatibleDriverVersion "[MGA] 你的 mga_vid 驱动版本与该 MPlayer 的版本�?兼容�?\n"
#define MSGTR_LIBVO_MGA_CouldntOpen "[MGA] 无法打开：%s\n"
#define MSGTR_LIBVO_MGA_ResolutionTooHigh "[MGA] �?分辨率至少有一维超过了 1023x1023。\n[MGA] 需用软件或用 -lavdopts lowres=1 �?新缩放。\n"
#define MSGTR_LIBVO_MGA_mgavidVersionMismatch "[MGA] 内核（%u）与 MPlayer（%u）的 mga_vid 驱动版本�?��?匹�?。\n"

// vo_null.c
#define MSGTR_LIBVO_NULL_UnknownSubdevice "[VO_NULL] 未知�?设备：%s。\n"

// vo_png.c
#define MSGTR_LIBVO_PNG_Warning1 "[VO_PNG] 警告：压缩级别设置为 0，�?用压缩�?\n"
#define MSGTR_LIBVO_PNG_Warning2 "[VO_PNG] 信�?�：使用 -vo png:z=<n> 设置 0 到 9 之间的压缩级别。\n"
#define MSGTR_LIBVO_PNG_Warning3 "[VO_PNG] 信�?�:（0 = �?压缩，1 = 最快，压缩率最低 - 9 最好，最慢的压缩）\n"
#define MSGTR_LIBVO_PNG_ErrorOpeningForWriting "\n[VO_PNG] 打开‘%s’以写入错误�?\n"
#define MSGTR_LIBVO_PNG_ErrorInCreatePng "[VO_PNG] create_png 错误。\n"

// vo_pnm.c
#define MSGTR_VO_PNM_ASCIIMode "�?�用 ASCII 模�?。"
#define MSGTR_VO_PNM_RawMode "�?�用原生模�?。"
#define MSGTR_VO_PNM_PPMType "将�?写入 PPM 文件。"
#define MSGTR_VO_PNM_PGMType "将�?写入 PGM 文件。"
#define MSGTR_VO_PNM_PGMYUVType "将�?写入 PGMYUV 文件。"

// vo_sdl.c
#define MSGTR_LIBVO_SDL_CouldntGetAnyAcceptableSDLModeForOutput "[VO_SDL] 无法获得�?�接�?�的 SDL 模�?以用于输出。\n"
#define MSGTR_LIBVO_SDL_SetVideoModeFailed "[VO_SDL] set_video_mode：SDL_SetVideoMode 失败：%s。\n"
#define MSGTR_LIBVO_SDL_MappingI420ToIYUV "[VO_SDL] 将 I420 映射到 IYUV。\n"
#define MSGTR_LIBVO_SDL_UnsupportedImageFormat "[VO_SDL] �?支�?的图�?格�?（0x%X）。\n"
#define MSGTR_LIBVO_SDL_InfoPleaseUseVmOrZoom "[VO_SDL] 信�?� - 请使用 -vm 或 -zoom 切�?�到最佳分辨率。\n"
#define MSGTR_LIBVO_SDL_FailedToSetVideoMode "[VO_SDL] 设置视频模�?失败：%s。\n"
#define MSGTR_LIBVO_SDL_CouldntCreateAYUVOverlay "[VO_SDL] 未能创建 YUV 覆盖模�?：%s。\n"
#define MSGTR_LIBVO_SDL_CouldntCreateARGBSurface "[VO_SDL] 未能创建 RGB 表�?�图层：%s。\n"
#define MSGTR_LIBVO_SDL_UsingDepthColorspaceConversion "[VO_SDL] 使用深度/颜色空间转�?�，这会�?慢速度（%ibpp -> %ibpp）。\n"
#define MSGTR_LIBVO_SDL_UnsupportedImageFormatInDrawslice "[VO_SDL] draw_slice �?支�?的图�?格�?，请�?�系 MPlayer 的开�?�者�?\n"
#define MSGTR_LIBVO_SDL_BlitFailed "[VO_SDL] �?�?�传输失败：%s。\n"
#define MSGTR_LIBVO_SDL_InitializationFailed "[VO_SDL] �?始化 SDL 模�?失败: %s。\n"
#define MSGTR_LIBVO_SDL_UsingDriver "[VO_SDL] 使用驱动：%s。\n"

// vo_svga.c
#define MSGTR_LIBVO_SVGA_ForcedVidmodeNotAvailable "[VO_SVGA] 强制使用的 vid_mode %d（%s）�?�?�用。\n"
#define MSGTR_LIBVO_SVGA_ForcedVidmodeTooSmall "[VO_SVGA] 强制使用的 vid_mode %d（%s）太�?。\n"
#define MSGTR_LIBVO_SVGA_Vidmode "[VO_SVGA] Vid_mode：%d，%dx%d %dbpp。\n"
#define MSGTR_LIBVO_SVGA_VgasetmodeFailed "[VO_SVGA] Vga_setmode（%d）失败。\n"
#define MSGTR_LIBVO_SVGA_VideoModeIsLinearAndMemcpyCouldBeUsed "[VO_SVGA] 线性的视频模�?，�?�以使用 memcpy 传输图�?。\n"
#define MSGTR_LIBVO_SVGA_VideoModeHasHardwareAcceleration "[VO_SVGA] 硬件加速的视频模�?，�?�以使用 put_image。\n"
#define MSGTR_LIBVO_SVGA_IfItWorksForYouIWouldLikeToKnow "[VO_SVGA] 如果工作正常请告诉我。\n[VO_SVGA]（使用 `mplayer test.avi -v -v -v -v &> svga.log` 生�?日志文件并�?��?）。谢谢�?\n"
#define MSGTR_LIBVO_SVGA_VideoModeHas "[VO_SVGA] 视频模�?�?�用 %d 内存页。\n"
#define MSGTR_LIBVO_SVGA_CenteringImageStartAt "[VO_SVGA] 居中图�?。超始�?置（%d，%d）\n"
#define MSGTR_LIBVO_SVGA_UsingVidix "[VO_SVGA] 使用 VIDIX。w=%i h=%i  mw=%i mh=%i\n"

// vo_tdfx_vid.c
#define MSGTR_LIBVO_TDFXVID_Move "[VO_TDXVID] 移动 %d(%d) x %d => %d。\n"
#define MSGTR_LIBVO_TDFXVID_AGPMoveFailedToClearTheScreen "[VO_TDFXVID] AGP 移动�?作未能清空�?幕。\n"
#define MSGTR_LIBVO_TDFXVID_BlitFailed "[VO_TDFXVID] �?�?�传输失败。\n"
#define MSGTR_LIBVO_TDFXVID_NonNativeOverlayFormatNeedConversion "[VO_TDFXVID] �?�原生支�?的覆盖格�?需�?转�?�。\n"
#define MSGTR_LIBVO_TDFXVID_UnsupportedInputFormat "[VO_TDFXVID] �?支�?的输入格�? 0x%x。\n"
#define MSGTR_LIBVO_TDFXVID_OverlaySetupFailed "[VO_TDFXVID] 覆盖模�?设置失败。\n"
#define MSGTR_LIBVO_TDFXVID_OverlayOnFailed "[VO_TDFXVID] 覆盖模�?打开失败。\n"
#define MSGTR_LIBVO_TDFXVID_OverlayReady "[VO_TDFXVID] 覆盖模�?就绪：%d(%d) x %d @ %d => %d(%d) x %d @ %d。\n"
#define MSGTR_LIBVO_TDFXVID_TextureBlitReady "[VO_TDFXVID] 纹�?��?�?�传输就绪：%d(%d) x %d @ %d => %d(%d) x %d @ %d。\n"
#define MSGTR_LIBVO_TDFXVID_OverlayOffFailed "[VO_TDFXVID] 覆盖模�?关闭失败\n"
#define MSGTR_LIBVO_TDFXVID_CantOpen "[VO_TDFXVID] 无法打开 %s：%s。\n"
#define MSGTR_LIBVO_TDFXVID_CantGetCurrentCfg "[VO_TDFXVID] 无法获得当�?�?置：%s。\n"
#define MSGTR_LIBVO_TDFXVID_MemmapFailed "[VO_TDFXVID] Memmap 失败�?�?�?\n"
#define MSGTR_LIBVO_TDFXVID_GetImageTodo "获得图�? todo。\n"
#define MSGTR_LIBVO_TDFXVID_AgpMoveFailed "[VO_TDFXVID] AGP 移动�?作失败。\n"
#define MSGTR_LIBVO_TDFXVID_SetYuvFailed "[VO_TDFXVID] 设置 YUV 失败。\n"
#define MSGTR_LIBVO_TDFXVID_AgpMoveFailedOnYPlane "[VO_TDFXVID] Y 平�?� AGP 移动�?作失败。\n"
#define MSGTR_LIBVO_TDFXVID_AgpMoveFailedOnUPlane "[VO_TDFXVID] U 平�?� AGP 移动�?作失败。\n"
#define MSGTR_LIBVO_TDFXVID_AgpMoveFailedOnVPlane "[VO_TDFXVID] V 平�?� AGP 移动�?作失败。\n"
#define MSGTR_LIBVO_TDFXVID_UnknownFormat "[VO_TDFXVID] 未知格�?：0x%x。\n"

// vo_tdfxfb.c
#define MSGTR_LIBVO_TDFXFB_CantOpen "[VO_TDFXFB] 无法打开 %s：%s。\n"
#define MSGTR_LIBVO_TDFXFB_ProblemWithFbitgetFscreenInfo "[VO_TDFXFB] FBITGET_FSCREENINFO ioctl 故障：%s。\n"
#define MSGTR_LIBVO_TDFXFB_ProblemWithFbitgetVscreenInfo "[VO_TDFXFB] FBITGET_VSCREENINFO ioctl 故障：%s。\n"
#define MSGTR_LIBVO_TDFXFB_ThisDriverOnlySupports "[VO_TDFXFB] 该驱动仅支�? 3Dfx Banshee�?Voodoo3 和 Voodoo 5。\n"
#define MSGTR_LIBVO_TDFXFB_OutputIsNotSupported "[VO_TDFXFB] �?支�? %d bpp 输出。\n"
#define MSGTR_LIBVO_TDFXFB_CouldntMapMemoryAreas "[VO_TDFXFB] 无法映射内存区域：%s。\n"
#define MSGTR_LIBVO_TDFXFB_BppOutputIsNotSupported "[VO_TDFXFB] �?支�? %d bpp 输出（这�?情况�?应出现）。\n"
#define MSGTR_LIBVO_TDFXFB_SomethingIsWrongWithControl "[VO_TDFXFB] 呃�?control() 有点问题。\n"
#define MSGTR_LIBVO_TDFXFB_NotEnoughVideoMemoryToPlay "[VO_TDFXFB] 没有足够的显存播放此影片，请�?试使用较低的分辨率。\n"
#define MSGTR_LIBVO_TDFXFB_ScreenIs "[VO_TDFXFB] �?幕尺寸为 %dx%d 色深 %d bpp，输入尺寸 %dx%d 色深 %d bpp，标准化尺寸 %dx%d。\n"

// vo_tga.c
#define MSGTR_LIBVO_TGA_UnknownSubdevice "[VO_TGA] 未知�?设备：%s。\n"

// vo_vesa.c
#define MSGTR_LIBVO_VESA_FatalErrorOccurred "[VO_VESA] �?�生致命错误�?无法继续。\n"
#define MSGTR_LIBVO_VESA_UnknownSubdevice "[VO_VESA] 未知�?设备：‘%s’。\n"
#define MSGTR_LIBVO_VESA_YouHaveTooLittleVideoMemory "[VO_VESA] 显存太�?无法支�?该模�?：\n[VO_VESA] 需�?：%08lX 现有：%08lX。\n"
#define MSGTR_LIBVO_VESA_YouHaveToSpecifyTheCapabilitiesOfTheMonitor "[VO_VESA] 需�?指定显示器的性能。未改�?�刷新频率。\n"
#define MSGTR_LIBVO_VESA_UnableToFitTheMode "[VO_VESA] 模�?超出显示器的�?制。未改�?�刷新频率。\n"
#define MSGTR_LIBVO_VESA_DetectedInternalFatalError "[VO_VESA] 检测到内部致命错误：init 在 preinit �?被调用。\n"
#define MSGTR_LIBVO_VESA_SwitchFlipIsNotSupported "[VO_VESA] �?支�? -flip 命令。\n"
#define MSGTR_LIBVO_VESA_PossibleReasonNoVbe2BiosFound "[VO_VESA] �?�能的原因：未找到 VBE2 BIOS。\n"
#define MSGTR_LIBVO_VESA_FoundVesaVbeBiosVersion "[VO_VESA] 找到 VESA VBE BIOS 版本 %x.%x 修订版本：%x。\n"
#define MSGTR_LIBVO_VESA_VideoMemory "[VO_VESA] 显存：%u Kb。\n"
#define MSGTR_LIBVO_VESA_Capabilites "[VO_VESA] VESA 性能：%s %s %s %s %s。\n"
#define MSGTR_LIBVO_VESA_BelowWillBePrintedOemInfo "[VO_VESA] �?�?�?下�?�将显示 OEM 信�?��?�?�?\n"
#define MSGTR_LIBVO_VESA_YouShouldSee5OemRelatedLines "[VO_VESA] 应该看到 5 行 OEM 的相关内容；�?�则说明 vm86 有问题。\n"
#define MSGTR_LIBVO_VESA_OemInfo "[VO_VESA] OEM 信�?�：%s。\n"
#define MSGTR_LIBVO_VESA_OemRevision "[VO_VESA] OEM 版本：%x。\n"
#define MSGTR_LIBVO_VESA_OemVendor "[VO_VESA] OEM 销售商：%s。\n"
#define MSGTR_LIBVO_VESA_OemProductName "[VO_VESA] OEM 产�?�??：%s。\n"
#define MSGTR_LIBVO_VESA_OemProductRev "[VO_VESA] OEM 产�?版本：%s。\n"
#define MSGTR_LIBVO_VESA_Hint "[VO_VESA] �??示：为使用电视输出你应当在�?�动�?�?�入电视接头。\n"\
"[VO_VESA] 因为 VESA BIOS �?�在自检的时候执行�?始化。\n"
#define MSGTR_LIBVO_VESA_UsingVesaMode "[VO_VESA] 使用 VESA 模�? (%u) = %x [%ux%u@%u]\n"
#define MSGTR_LIBVO_VESA_CantInitializeSwscaler "[VO_VESA] 无法�?始化软件缩放功能。\n"
#define MSGTR_LIBVO_VESA_CantUseDga "[VO_VESA] 无法使用 DGA。强制使用组交�?�模�?。 :(\n"
#define MSGTR_LIBVO_VESA_UsingDga "[VO_VESA] 使用 DGA（物�?�资�?: %08lXh, %08lXh）"
#define MSGTR_LIBVO_VESA_CantUseDoubleBuffering "[VO_VESA] 无法使用�?��?缓冲：显存�?足。\n"
#define MSGTR_LIBVO_VESA_CantFindNeitherDga "[VO_VESA] 无法找到 DGA 或是�?��?分�?的视频窗�?�框架。\n"
#define MSGTR_LIBVO_VESA_YouveForcedDga "[VO_VESA] 已强制使用 DGA。正在退出\n"
#define MSGTR_LIBVO_VESA_CantFindValidWindowAddress "[VO_VESA] 无法找到有效的视频窗�?�地�?�。\n"
#define MSGTR_LIBVO_VESA_UsingBankSwitchingMode "[VO_VESA] 使用组交�?�模�?（物�?�资�?：%08lXh, %08lXh）。\n"
#define MSGTR_LIBVO_VESA_CantAllocateTemporaryBuffer "[VO_VESA] 无法分�?临时缓冲。\n"
#define MSGTR_LIBVO_VESA_SorryUnsupportedMode "[VO_VESA] 抱歉，�?支�?该模�?——请�?试 -x 640 -zoom。\n"
#define MSGTR_LIBVO_VESA_OhYouReallyHavePictureOnTv "[VO_VESA] 噢，你的电视上有图�?了�?\n"
#define MSGTR_LIBVO_VESA_CantInitialozeLinuxVideoOverlay "[VO_VESA] 无法�?始化 Linux 视频覆盖模�?。\n"
#define MSGTR_LIBVO_VESA_UsingVideoOverlay "[VO_VESA] 使用视频覆盖模�?：%s。\n"
#define MSGTR_LIBVO_VESA_CantInitializeVidixDriver "[VO_VESA] 无法�?始化 VIDIX 驱动。\n"
#define MSGTR_LIBVO_VESA_UsingVidix "[VO_VESA] 正在使用 VIDIX。\n"
#define MSGTR_LIBVO_VESA_CantFindModeFor "[VO_VESA] 无法找到适�?�的模�?用于：%ux%u@%u。\n"
#define MSGTR_LIBVO_VESA_InitializationComplete "[VO_VESA] VESA �?始化完�?。\n"

// vesa_lvo.c
#define MSGTR_LIBVO_VESA_ThisBranchIsNoLongerSupported "[VESA_LVO] 这个分支版本已�?�?�?维护。\n[VESA_LVO] 请使用 -vo vesa:vidix。\n"
#define MSGTR_LIBVO_VESA_CouldntOpen "[VESA_LVO] 无法打开：‘%s’\n"
#define MSGTR_LIBVO_VESA_InvalidOutputFormat "[VESA_LVI] 无效的输出格�?：%s(%0X)\n"
#define MSGTR_LIBVO_VESA_IncompatibleDriverVersion "[VESA_LVO] fb_vid 驱动版本与该版本 MPlayer �?兼容�?\n"

// vo_x11.c
#define MSGTR_LIBVO_X11_DrawFrameCalled "[VO_X11] 已调用 draw_frame()�?�?�?\n"

// vo_xv.c
#define MSGTR_LIBVO_XV_DrawFrameCalled "[VO_XV] 已调用 draw_frame()�?�?�?\n"
#define MSGTR_LIBVO_XV_SharedMemoryNotSupported "[VO_XV] �?支�?共享内存模�?\n回�?到正常 Xv 模�?。\n"
#define MSGTR_LIBVO_XV_XvNotSupportedByX11 "[VO_XV] 对�?起，该 X11 版本/驱动�?支�? Xv\n[VO_XV] ******** 请�?试使用 -vo x11 或  -vo sdl  *********\n"
#define MSGTR_LIBVO_XV_XvQueryAdaptorsFailed  "[VO_XV] XvQueryAdaptors 失败。\n"
#define MSGTR_LIBVO_XV_InvalidPortParameter "[VO_XV] 无效的端�?��?�数，改用端�?� 0。\n"
#define MSGTR_LIBVO_XV_CouldNotGrabPort "[VO_XV] 无法绑定端�?� %i。\n"
#define MSGTR_LIBVO_XV_CouldNotFindFreePort "[VO_XV] 无法找到空闲的 Xvideo 端�?� - 或许�?�一进程已�?正在使用\n"\
"[VO_XV] 该端�?�。关闭所有视频应用程�?并�?次�?试。如果那样�?��?然无效，则\n"\
"[VO_XV] �?��?‘mplayer -vo help’获�?�其它（�?� xv）视频输出驱动的信�?�。\n"
#define MSGTR_LIBVO_XV_NoXvideoSupport "[VO_XV] 好�?没有�?�用的 Xvideo 能够支�?你的显�?�。\n"\
"[VO_XV] �?行‘xvinfo’检查其是�?�支�? Xv 并阅读\n"\
"[VO_XV] DOCS/HTML/en/video.html#xv�?\n"\
"[VO_XV] �?��?‘mplayer -vo help’获�?�其它（�?� xv）视频输出驱动的信�?�。\n"\
"[VO_XV] 试试 -vo x11。\n"
#define MSGTR_VO_XV_ImagedimTooHigh "�?图�?尺寸过大：%ux%u（最大值�?制为 %ux%u）\n"

// vo_yuv4mpeg.c
#define MSGTR_VO_YUV4MPEG_InterlacedHeightDivisibleBy4 "隔行扫�??模�?�?求图�?高度能被 4 整除。"
#define MSGTR_VO_YUV4MPEG_InterlacedLineBufAllocFail "无法为隔行扫�??模�?分�?扫�??行缓冲。"
#define MSGTR_VO_YUV4MPEG_WidthDivisibleBy2 "图�?宽度必须能被 2 整除。"
#define MSGTR_VO_YUV4MPEG_OutFileOpenError "无法获�?�内存或文件�?�柄以写入“%s�?�?"
#define MSGTR_VO_YUV4MPEG_OutFileWriteError "图�?写入输出错误�?"
#define MSGTR_VO_YUV4MPEG_UnknownSubDev "未知�?设备：%s"
#define MSGTR_VO_YUV4MPEG_InterlacedTFFMode "使用隔行扫�??输出模�?，上�?�扫�??场优先方�?。"
#define MSGTR_VO_YUV4MPEG_InterlacedBFFMode "使用隔行扫�??输出模�?，下�?�扫�??场优先方�?。"
#define MSGTR_VO_YUV4MPEG_ProgressiveMode "使用（默认的）�?行扫�??帧模�?。"

// vobsub_vidix.c
#define MSGTR_LIBVO_SUB_VIDIX_CantStartPlayback "[VO_SUB_VIDIX] 无法开始播放：%s\n"
#define MSGTR_LIBVO_SUB_VIDIX_CantStopPlayback "[VO_SUB_VIDIX] 无法�?�止播放：%s\n"
#define MSGTR_LIBVO_SUB_VIDIX_InterleavedUvForYuv410pNotSupported "[VO_SUB_VIDIX] �?支�? YUV410P 下的交错 UV 模�?。\n"
#define MSGTR_LIBVO_SUB_VIDIX_DummyVidixdrawsliceWasCalled "[VO_SUB_VIDIX] 调用伪 vidix_draw_slice()。\n"
#define MSGTR_LIBVO_SUB_VIDIX_DummyVidixdrawframeWasCalled "[VO_SUB_VIDIX] 调用伪 vidix_draw_frame()。\n"
#define MSGTR_LIBVO_SUB_VIDIX_UnsupportedFourccForThisVidixDriver "[VO_SUB_VIDIX] 该 VIDIX 驱动�?支�?的 FourCC 代�?：%x (%s)。\n"
#define MSGTR_LIBVO_SUB_VIDIX_VideoServerHasUnsupportedResolution "[VO_SUB_VIDIX] 视频�?务器使用了�?支�?分辨率（%dx%d），支�?的分辨率：%dx%d-%dx%d。\n"
#define MSGTR_LIBVO_SUB_VIDIX_VideoServerHasUnsupportedColorDepth "[VO_SUB_VIDIX] 视频�?务器使用了 VIDIX �?支�?的色彩深度（%d）。\n"
#define MSGTR_LIBVO_SUB_VIDIX_DriverCantUpscaleImage "[VO_SUB_VIDIX] VIDIX 驱动无法放大图�?（%d%d -> %d%d）。\n"
#define MSGTR_LIBVO_SUB_VIDIX_DriverCantDownscaleImage "[VO_SUB_VIDIX] VIDIX 驱动无法缩�?图�?（%d%d -> %d%d）。\n"
#define MSGTR_LIBVO_SUB_VIDIX_CantConfigurePlayback "[VO_SUB_VIDIX] 无法�?置播放功能：%s。\n"
#define MSGTR_LIBVO_SUB_VIDIX_YouHaveWrongVersionOfVidixLibrary "[VO_SUB_VIDIX] VIDIX 库版本错误。\n"
#define MSGTR_LIBVO_SUB_VIDIX_CouldntFindWorkingVidixDriver "[VO_SUB_VIDIX] 无法找到�?�用的 VIDIX 驱动。\n"
#define MSGTR_LIBVO_SUB_VIDIX_CouldntGetCapability "[VO_SUB_VIDIX] 无法获得性能信�?�：%s。\n"

// x11_common.c
#define MSGTR_EwmhFullscreenStateFailed "\nX11：无法�?��? EWMH 全�?事件消�?��?\n"
#define MSGTR_CouldNotFindXScreenSaver "xscreensaver_disable：无法找到 XScreenSaver 窗�?�。\n"
#define MSGTR_SelectedVideoMode "XF86VM：已选择视频模�? %dx%d（图�?大�? %dx%d）。\n"

#define MSGTR_InsertingAfVolume "[混音器] 没有硬件混音支�?，�?�入音�?过滤器。\n"
#define MSGTR_NoVolume "[混音器] 没有�?�用的音�?控制器。\n"
#define MSGTR_NoBalance "[混音器] 没有�?�用的�?�衡控制器。\n"

// old vo drivers that have been replaced
#define MSGTR_VO_PGM_HasBeenReplaced "PGM 视频输出驱动已替�?�为 -vo pnm:pgmyuv。\n"
#define MSGTR_VO_MD5_HasBeenReplaced "MD5 视频输出驱动已替�?�为 -vo md5sum。\n"


// ======================= audio output drivers ========================

// audio_out.c
#define MSGTR_AO_ALSA9_1x_Removed "音频输出：alsa9 和 alsa1x 模�?�已�?支�?，请用 -ao alsa 代替。\n"
#define MSGTR_AO_NoSuchDriver "无此音频驱动‘%.*s’\n"
#define MSGTR_AO_FailedInit "未能�?始化音频驱动‘%s’\n"

// ao_oss.c
#define MSGTR_AO_OSS_CantOpenMixer "[AO OSS] audio_setup：无法打开混音设备 %s：%s\n"
#define MSGTR_AO_OSS_ChanNotFound "[AO OSS] audio_setup：声�?�混音器没有‘%s’声�?�，使用默认声�?�。\n"
#define MSGTR_AO_OSS_CantOpenDev "[AO OSS] audio_setup：无法打开音频设备 %s：%s\n"
#define MSGTR_AO_OSS_CantMakeFd "[AO OSS] audio_setup：无法�?止对于文件�??述符的访问：%s\n"
#define MSGTR_AO_OSS_CantSet "[AO OSS] 无法设定音频设备 %s 至 %s 的输出，�?试使用 %s...\n"
#define MSGTR_AO_OSS_CantSetChans "[AO OSS] audio_setup：设置音频设备为 %d 声�?�失败。\n"
#define MSGTR_AO_OSS_CantUseGetospace "[AO OSS] audio_setup：驱动�?支�? SNDCTL_DSP_GETOSPACE :-(\n"
#define MSGTR_AO_OSS_CantUseSelect "[AO OSS]\n   ***  你的音频驱动�?支�? select()  ***\n 需在 config.h 中�?置 #undef HAVE_AUDIO_SELECT �?��?新编译 MPlayer�?\n\n"
#define MSGTR_AO_OSS_CantReopen "[AO OSS]\n致命错误：*** 无法�?新打开或�?新�?�动音频设备 *** %s\n"
#define MSGTR_AO_OSS_UnknownUnsupportedFormat "[AO OSS] 未知/�?支�?的 OSS 格�?：%x。\n"

// ao_arts.c
#define MSGTR_AO_ARTS_CantInit "[AO ARTS] %s\n"
#define MSGTR_AO_ARTS_ServerConnect "[AO ARTS] 已连接到声音�?务设备。\n"
#define MSGTR_AO_ARTS_CantOpenStream "[AO ARTS] 无法打开音频�?。\n"
#define MSGTR_AO_ARTS_StreamOpen "[AO ARTS] 音频�?已打开。\n"
#define MSGTR_AO_ARTS_BufferSize "[AO ARTS] 缓冲大�?：%d\n"

// ao_dxr2.c
#define MSGTR_AO_DXR2_SetVolFailed "[AO DXR2] 未能设定音�?为 %d。\n"
#define MSGTR_AO_DXR2_UnsupSamplerate "[AO DXR2] �?支�? %d Hz，�?试对音频�?新采样。\n"

// ao_esd.c
#define MSGTR_AO_ESD_CantOpenSound "[AO ESD] esd_open_sound 失败：%s\n"
#define MSGTR_AO_ESD_LatencyInfo "[AO ESD] 延迟：[�?务器：%0.2fs，网络：%0.2fs]（调整值 %0.2fs）\n"
#define MSGTR_AO_ESD_CantOpenPBStream "[AO ESD] 打开 ESD 播放�?失败：%s\n"

// ao_mpegpes.c
#define MSGTR_AO_MPEGPES_CantSetMixer "[AO MPEGPES] DVB 音频组件混音器错误：%s。\n"
#define MSGTR_AO_MPEGPES_UnsupSamplerate "[AO MPEGPES] �?支�? %d Hz，�?试对音频�?新采样。\n"

// ao_pcm.c
#define MSGTR_AO_PCM_FileInfo "[AO PCM] 文件: %s (%s)\nPCM：采样率：%iHz 声�?�：%s 格�? %s\n"
#define MSGTR_AO_PCM_HintInfo "[AO PCM] 信�?�：使用 -benchmark -vc null -vo null -ao pcm:fast �?�以更快地转储数�?�\n[AO PCM] 信�?�：�?写入 WAVE 文件，使用 -ao pcm:waveheader（默认模�?）。\n"
#define MSGTR_AO_PCM_CantOpenOutputFile "[AO PCM] 打开并写入 %s 失败�?\n"

// ao_sdl.c
#define MSGTR_AO_SDL_INFO "[AO SDL] 采样率：%iHz 声�?�：%s 格�? %s\n"
#define MSGTR_AO_SDL_DriverInfo "[AO SDL] 使用 %s 音频驱动。\n"
#define MSGTR_AO_SDL_UnsupportedAudioFmt "[AO SDL] �?支�?的音频格�?：0x%x。\n"
#define MSGTR_AO_SDL_CantInit "[AO SDL] SDL 音频�?始化失败：%s\n"
#define MSGTR_AO_SDL_CantOpenAudio "[AO SDL] 无法打开音频：%s\n"

// ao_sgi.c
#define MSGTR_AO_SGI_INFO "[AO SGI] 控制。\n"
#define MSGTR_AO_SGI_InitInfo "[AO SGI] �?始化：采样率：%iHz 声�?�：%s 格�? %s\n"
#define MSGTR_AO_SGI_InvalidDevice "[AO SGI] 播放：无效的设备。\n"
#define MSGTR_AO_SGI_CantSetParms_Samplerate "[AO SGI] �?始化：setparams 失败：%s\n无法设置为所需的采样率。\n"
#define MSGTR_AO_SGI_CantSetAlRate "[AO SGI] �?始化：AL_RATE �?适用于给定的音频�?��?。\n"
#define MSGTR_AO_SGI_CantGetParms "[AO SGI] �?始化：getparams 失败：%s\n"
#define MSGTR_AO_SGI_SampleRateInfo "[AO SGI] �?始化：当�?的采样率为 %f（所需的速率为 %f）\n"
#define MSGTR_AO_SGI_InitConfigError "[AO SGI] �?始化：%s\n"
#define MSGTR_AO_SGI_InitOpenAudioFailed "[AO SGI] �?始化：无法打开音频声�?�：%s\n"
#define MSGTR_AO_SGI_Uninit "[AO SGI] 终止：...\n"
#define MSGTR_AO_SGI_Reset "[AO SGI] �?�?�：...\n"
#define MSGTR_AO_SGI_PauseInfo "[AO SGI] audio_pause：...\n"
#define MSGTR_AO_SGI_ResumeInfo "[AO SGI] audio_resume：...\n"

// ao_sun.c
#define MSGTR_AO_SUN_RtscSetinfoFailed "[AO SUN] rtsc：SETINFO 失败。\n"
#define MSGTR_AO_SUN_RtscWriteFailed "[AO SUN] rtsc：写入失败。\n"
#define MSGTR_AO_SUN_CantOpenAudioDev "[AO SUN] 无法打开音频设备 %s，%s  -> nosound。\n"
#define MSGTR_AO_SUN_UnsupSampleRate "[AO SUN] 音频设置：你的声�?��?支�? %d 声�?�，%s，%d Hz 采样率。\n"
#define MSGTR_AO_SUN_CantUseSelect "[AO SUN]\n   ***  你的音频驱动�?支�? select()  ***\n需�?在 config.h 中设置 #undef HAVE_AUDIO_SELECT 并�?新编译 MPlayer�?\n\n"
#define MSGTR_AO_SUN_CantReopenReset "[AO SUN]\n致命错误：*** 无法�?新打开/�?�?�音频设备（%s）***\n"

// ao_alsa.c
#define MSGTR_AO_ALSA_InvalidMixerIndexDefaultingToZero "[AO_ALSA] 无效的混音器索引�?�。�?�默认值 0。\n"
#define MSGTR_AO_ALSA_MixerOpenError "[AO_ALSA] 打开混音器错误：%s\n"
#define MSGTR_AO_ALSA_MixerAttachError "[AO_ALSA] 混音器接入 %s 错误：%s\n"
#define MSGTR_AO_ALSA_MixerRegisterError "[AO_ALSA] 混音器登记错误：%s\n"
#define MSGTR_AO_ALSA_MixerLoadError "[AO_ALSA] 混音器装载错误：%s\n"
#define MSGTR_AO_ALSA_UnableToFindSimpleControl "[AO_ALSA] 无法找到简�?�控制器‘%s’，%i。\n"
#define MSGTR_AO_ALSA_ErrorSettingLeftChannel "[AO_ALSA] 设置左声�?�错误，%s\n"
#define MSGTR_AO_ALSA_ErrorSettingRightChannel "[AO_ALSA] 设置�?�声�?�错误，%s\n"
#define MSGTR_AO_ALSA_CommandlineHelp "\n[AO_ALSA] -ao alsa 命令行帮助：\n"\
"[AO_ALSA] 示例：mplayer -ao alsa:device=hw=0.3\n"\
"[AO_ALSA]   设置第一�?�声�?�为第四硬件设备。\n\n"\
"[AO_ALSA] 选项：\n"\
"[AO_ALSA]   noblock\n"\
"[AO_ALSA]     以�?�阻塞模�?打开设备。\n"\
"[AO_ALSA]   device=<设备�??>\n"\
"[AO_ALSA]     设置设备（将字符中的‘,’替�?�为‘.’而‘:’替�?�为‘=’）\n"
#define MSGTR_AO_ALSA_ChannelsNotSupported "[AO_ALSA] �?支�? %d 声�?�。\n"
#define MSGTR_AO_ALSA_OpenInNonblockModeFailed "[AO_ALSA] 以�?�阻塞模�?打开失败，�?试以阻塞模�?打开。\n"
#define MSGTR_AO_ALSA_PlaybackOpenError "[AO_ALSA] 打开播放模�?错误：%s\n"
#define MSGTR_AO_ALSA_ErrorSetBlockMode "[AL_ALSA] 设置阻塞模�? %s 错误。\n"
#define MSGTR_AO_ALSA_UnableToGetInitialParameters "[AO_ALSA] 无法得到�?始�?�数：%s\n"
#define MSGTR_AO_ALSA_UnableToSetAccessType "[AO_ALSA] 无法设置访问类型：%s\n"
#define MSGTR_AO_ALSA_FormatNotSupportedByHardware "[AO_ALSA] 硬件�?支�?的格�? %s，�?试默认格�?。\n"
#define MSGTR_AO_ALSA_UnableToSetFormat "[AO_ALSA] 无法设置格�?：%s\n"
#define MSGTR_AO_ALSA_UnableToSetChannels "[AO_ALSA] 无法设置声�?�：%s\n"
#define MSGTR_AO_ALSA_UnableToDisableResampling "[AO_ALSA] 无法�?用�?抽样模�?：%s\n"
#define MSGTR_AO_ALSA_UnableToSetSamplerate2 "[AO_ALSA] 无法设置采样率-2：%s\n"
#define MSGTR_AO_ALSA_UnableToSetBufferTimeNear "[AO_ALSA] 无法设置缓冲时间为约：%s\n"
#define MSGTR_AO_ALSA_UnableToGetPeriodSize "[AO ALSA] 无法获得周期大�?：%s\n"
#define MSGTR_AO_ALSA_UnableToSetPeriods "[AO_ALSA] 无法设置周期：%s\n"
#define MSGTR_AO_ALSA_UnableToSetHwParameters "[AO_ALSA] 无法设置 hw-parameters：%s\n"
#define MSGTR_AO_ALSA_UnableToGetBufferSize "[AO_ALSA] 无法获得缓冲大�?：%s\n"
#define MSGTR_AO_ALSA_UnableToGetSwParameters "[AO_ALSA] 无法获得 sw-parameters：%s\n"
#define MSGTR_AO_ALSA_UnableToSetSwParameters "[AO_ALSA] 无法设置 sw-parameters：%s\n"
#define MSGTR_AO_ALSA_UnableToGetBoundary "[AO_ALSA] 无法获得边界信�?�：%s\n"
#define MSGTR_AO_ALSA_UnableToSetStartThreshold "[AO_ALSA] 无法设置开始阈值：%s\n"
#define MSGTR_AO_ALSA_UnableToSetStopThreshold "[AO_ALSA] 无法设置�?�止阈值：%s\n"
#define MSGTR_AO_ALSA_UnableToSetSilenceSize "[AO_ALSA] 无法设置�?�音大�?：%s\n"
#define MSGTR_AO_ALSA_PcmCloseError "[AO_ALSA] pcm 关闭错误：%s\n"
#define MSGTR_AO_ALSA_NoHandlerDefined "[AO_ALSA] 未定义处�?�设备�?\n"
#define MSGTR_AO_ALSA_PcmPrepareError "[AO_ALSA] pcm 准备错误：%s\n"
#define MSGTR_AO_ALSA_PcmPauseError "[AO_ALSA] pcm 暂�?�错误：%s\n"
#define MSGTR_AO_ALSA_PcmDropError "[AO_ALSA] pcm 丢弃错误：%s\n"
#define MSGTR_AO_ALSA_PcmResumeError "[AO_ALSA] pcm 继续播放错误：%s\n"
#define MSGTR_AO_ALSA_DeviceConfigurationError "[AO_ALSA] 设备�?置错误。"
#define MSGTR_AO_ALSA_PcmInSuspendModeTryingResume "[AO_ALSA] Pcm 处于挂起模�?，�?试继续播放。\n"
#define MSGTR_AO_ALSA_WriteError "[AO_ALSA] 写入错误：%s\n"
#define MSGTR_AO_ALSA_TryingToResetSoundcard "[AO_ALSA] �?试�?�?�声�?�。\n"
#define MSGTR_AO_ALSA_CannotGetPcmStatus "[AO_ALSA] 无法获得 pcm 状�?：%s\n"

// ao_plugin.c
#define MSGTR_AO_PLUGIN_InvalidPlugin "[AO PLUGIN] 无效�?�件：%s\n"


// ======================= audio filters ================================

// af_scaletempo.c
#define MSGTR_AF_ValueOutOfRange MSGTR_VO_ValueOutOfRange

// af_ladspa.c
#define MSGTR_AF_LADSPA_AvailableLabels "�?�用标签"
#define MSGTR_AF_LADSPA_WarnNoInputs "警告�?该 LADSPA �?�件没有音频输入。\n输入的音频信�?�将会丢失。"
#define MSGTR_AF_LADSPA_ErrMultiChannel "尚未支�?多声�?�（>2）�?�件。\n�?�使用�?�声�?�或立体声�?�件。"
#define MSGTR_AF_LADSPA_ErrNoOutputs "该 LADSPA �?�件没有音频输出。"
#define MSGTR_AF_LADSPA_ErrInOutDiff "LADSPA �?�件的音频输入和音频输出的数目�?相等。"
#define MSGTR_AF_LADSPA_ErrFailedToLoad "装载失败"
#define MSGTR_AF_LADSPA_ErrNoDescriptor "指定的库文件中未能找到 ladspa_descriptor() 函数。"
#define MSGTR_AF_LADSPA_ErrLabelNotFound "未能在�?�件库里找到标签。"
#define MSGTR_AF_LADSPA_ErrNoSuboptions "未指定�?选项。"
#define MSGTR_AF_LADSPA_ErrNoLibFile "未指定库文件。"
#define MSGTR_AF_LADSPA_ErrNoLabel "未指定过滤器标签。"
#define MSGTR_AF_LADSPA_ErrNotEnoughControls "命令行中指定的控件�?足。"
#define MSGTR_AF_LADSPA_ErrControlBelow "%s：输入控件 #%d 低于下�? %0.4f。\n"
#define MSGTR_AF_LADSPA_ErrControlAbove "%s：输入控件 #%d 高于上�? %0.4f。\n"

// format.c
#define MSGTR_AF_FORMAT_UnknownFormat "未知格�?"


// ========================== INPUT =========================================

// joystick.c
#define MSGTR_INPUT_JOYSTICK_CantOpen "无法打开�?纵�?�设备 %s：%s\n"
#define MSGTR_INPUT_JOYSTICK_ErrReading "读�?��?纵�?�设备时出错：%s\n"
#define MSGTR_INPUT_JOYSTICK_LoosingBytes "�?纵�?�：丢失了 %d 字节的数�?�\n"
#define MSGTR_INPUT_JOYSTICK_WarnLostSync "�?纵�?�：�?始化警告事件，与驱动程�?失去�?�步。\n"
#define MSGTR_INPUT_JOYSTICK_WarnUnknownEvent "�?作�?�未知警告事件类型 %d\n"

// appleir.c
#define MSGTR_INPUT_APPLE_IR_CantOpen "无法打开 Apple IR 设备：%s\n"

// input.c
#define MSGTR_INPUT_INPUT_ErrCantRegister2ManyCmdFds "命令文件�??述符太多，无法注册文件�??述符 %d。\n"
#define MSGTR_INPUT_INPUT_ErrCantRegister2ManyKeyFds "按键文件�??述符太多，无法注册文件�??述符 %d。\n"
#define MSGTR_INPUT_INPUT_ErrArgMustBeInt "命令 %s：�?�数 %d �?是整数。\n"
#define MSGTR_INPUT_INPUT_ErrArgMustBeFloat "命令 %s：�?�数 %d �?是浮点数。\n"
#define MSGTR_INPUT_INPUT_ErrUnterminatedArg "命令 %s：�?�数 %d 无结�?�符。\n"
#define MSGTR_INPUT_INPUT_ErrUnknownArg "未知�?�数 %d\n"
#define MSGTR_INPUT_INPUT_Err2FewArgs "命令 %s 需�?至少 %d 个�?�数，然而�?��?�现了 %d 个。\n"
#define MSGTR_INPUT_INPUT_ErrReadingCmdFd "读�?�命令文件�??述符 %d 时�?�生错误：%s\n"
#define MSGTR_INPUT_INPUT_ErrCmdBufferFullDroppingContent "文件�??述符 %d 的命令缓冲已满：丢弃文件内容。\n"
#define MSGTR_INPUT_INPUT_ErrInvalidCommandForKey "绑定按键 %s 的命令无效"
#define MSGTR_INPUT_INPUT_ErrSelect "选定错误：%s\n"
#define MSGTR_INPUT_INPUT_ErrOnKeyInFd "按键输入文件�??述符 %d 错误\n"
#define MSGTR_INPUT_INPUT_ErrDeadKeyOnFd "按键输入文件�??述符 %d 出现无效按键\n"
#define MSGTR_INPUT_INPUT_Err2ManyKeyDowns "�?�时有过多的按键事件�?�生\n"
#define MSGTR_INPUT_INPUT_ErrOnCmdFd "命令文件�??述符 %d 错误\n"
#define MSGTR_INPUT_INPUT_ErrReadingInputConfig "读�?�输入�?置文件 %s 时�?�生错误: %s\n"
#define MSGTR_INPUT_INPUT_ErrUnknownKey "未知键‘%s’\n"
#define MSGTR_INPUT_INPUT_ErrUnfinishedBinding "未完�?的绑定 %s\n"
#define MSGTR_INPUT_INPUT_ErrBuffer2SmallForKeyName "此按键�??的缓冲太�?：%s\n"
#define MSGTR_INPUT_INPUT_ErrNoCmdForKey "未找到按键 %s 的命令"
#define MSGTR_INPUT_INPUT_ErrBuffer2SmallForCmd "命令 %s 的缓冲过�?\n"
#define MSGTR_INPUT_INPUT_ErrWhyHere "怎么会执行到这里了？\n"
#define MSGTR_INPUT_INPUT_ErrCantInitJoystick "无法�?始化输入�?纵�?�\n"
#define MSGTR_INPUT_INPUT_ErrCantOpenFile "无法打开 %s：%s\n"
#define MSGTR_INPUT_INPUT_ErrCantInitAppleRemote "无法�?始化 Apple Remote �?�控器。\n"

// lirc.c
#define MSGTR_LIRCopenfailed "�?�用 LIRC 支�?失败。将无法使用你的�?�控器。\n"
#define MSGTR_LIRCcfgerr "读�?� LIRC �?置文件 %s 失败。\n"


// ========================== LIBMPDEMUX ===================================

// muxer.c, muxer_*.c
#define MSGTR_TooManyStreams "媒体�?太多�?"
#define MSGTR_RawMuxerOnlyOneStream "原生音频�?�?�并器�?�支�?一个音频�?�?\n"
#define MSGTR_IgnoringVideoStream "忽略视频�?�?\n"
#define MSGTR_UnknownStreamType "警告，未知的媒体�?类型：%d\n"
#define MSGTR_WarningLenIsntDivisible "警告，音频长度无法被采样率整除�?\n"
#define MSGTR_MuxbufMallocErr "�?�?�并器帧缓冲无法分�?内存�?\n"
#define MSGTR_MuxbufReallocErr "�?�路器帧缓冲无法�?新分�?内存�?\n"
#define MSGTR_WritingHeader "正在写入帧头...\n"
#define MSGTR_WritingTrailer "正在写入索引...\n"

// demuxer.c, demux_*.c
#define MSGTR_AudioStreamRedefined "警告：�?�?定义了音频�?头部 %d。\n"
#define MSGTR_VideoStreamRedefined "警告: �?�?定义了视频�?头部 %d。\n"
#define MSGTR_TooManyAudioInBuffer "\n缓冲中音频包太多（%d 个包存在于 %d 字节中）。\n"
#define MSGTR_TooManyVideoInBuffer "\n缓冲中视频包太多（%d 个包存在于 %d 字节中）。\n"
#define MSGTR_MaybeNI "�?�能播放了一个�?�交错�?�并的媒体�?/文件，或者是编解�?�?行失败？\n" \
                      "对于 AVI 文件，�?��?试用 -ni 选项强制使用�?�交错模�?。\n"
#define MSGTR_WorkAroundBlockAlignHeaderBug "AVI：�?�开 CBR-MP3 nBlockAlign 头部缺陷�?\n"
#define MSGTR_SwitchToNi "\n检测到未正确交错�?�并的 AVI 文件 - 切�?�到 -ni 模�?...\n"
#define MSGTR_InvalidAudioStreamNosound "AVI：无效的音频�? ID: %d - 忽略 (nosound)\n"
#define MSGTR_InvalidAudioStreamUsingDefault "AVI: 无效的视频�? ID：%d - 忽略（使用默认值）\n"
#define MSGTR_ON2AviFormat "ON2 AVI 格�?"
#define MSGTR_Detected_XXX_FileFormat "检测到文件格�? %s。\n"
#define MSGTR_DetectedAudiofile "检测到音频文件。\n"
#define MSGTR_InvalidMPEGES "无法的 MPEG-ES 媒体�?？？？请�?�系开�?�者，这�?�能是软件的缺陷 :(\n"
#define MSGTR_FormatNotRecognized "============= 抱歉, 此文件格�?无法辨认或支�? ===============\n"\
                                  "===     如果此文件是 AVI�?ASF 或 MPEG 媒体�?，请�?�系开�?�者�?   ===\n"
#define MSGTR_SettingProcessPriority "设置进程优先级：%s\n"
#define MSGTR_FilefmtFourccSizeFpsFtime "[V] 文件格�?:%d  fourcc代�?:0x%X  大�?:%dx%d  帧速:%5.3f  �?帧时长:=%6.4f\n"
#define MSGTR_CannotInitializeMuxer "无法�?始化�?�?�并器。"
#define MSGTR_MissingVideoStream "未找到视频�?。\n"
#define MSGTR_MissingAudioStream "未找到音频�?...  -> 没有声音。\n"
#define MSGTR_MissingVideoStreamBug "没有视频�?�?？请�?�系开�?�者，这�?�能是软件的缺陷 :(\n"

#define MSGTR_DoesntContainSelectedStream "�?分离：文件中没有所选择的音频或视频�?。\n"

#define MSGTR_NI_Forced "强制使用"
#define MSGTR_NI_Detected "检测到"
#define MSGTR_NI_Message "%s �?�交错�?�并的 AVI 文件模�?�?\n"

#define MSGTR_UsingNINI "使用�?�交错�?�并的无效的 AVI 文件格�?。\n"
#define MSGTR_CouldntDetFNo "无法确定（�?对定�?所用的）帧数。\n"
#define MSGTR_CantSeekRawAVI "无法在原生的 AVI �?中定�?。(需�?索引信�?�，请�?试使用 -idx 选项。)\n"
#define MSGTR_CantSeekFile "无法在此文件中定�?。\n"

#define MSGTR_MOVcomprhdr "MOV：需�? ZLIB 以支�?�?过压缩的文件头�?\n"
#define MSGTR_MOVvariableFourCC "MOV：警告：检测到�?��?�的 FourCC 代�?�?？\n"
#define MSGTR_MOVtooManyTrk "MOV：警告：轨�?�太多。"
#define MSGTR_DetectedTV "检测到电视信�?��?;-)\n"
#define MSGTR_ErrorOpeningOGGDemuxer "无法打开 Ogg �?分离器。\n"
#define MSGTR_CannotOpenAudioStream "无法打开音频�?：%s\n"
#define MSGTR_CannotOpenSubtitlesStream "无法打开字幕�?：%s\n"
#define MSGTR_OpeningAudioDemuxerFailed "未能打开音频分离器：%s\n"
#define MSGTR_OpeningSubtitlesDemuxerFailed "未能打开字幕�?分离器：%s\n"
#define MSGTR_TVInputNotSeekable "电视信�?�输入无法定�?�?（定�?功能�?�能应该用于更�?�频�?�）\n"
#define MSGTR_DemuxerInfoChanged "�?分离器信�?� %s 已�?�为 %s\n"
#define MSGTR_ClipInfo "媒体剪辑信�?�：\n"

#define MSGTR_LeaveTelecineMode "\ndemux_mpg：检测到 30000/1001fps 的 NTSC 内容，切�?�帧率。\n"
#define MSGTR_EnterTelecineMode "\ndemux_mpg：检测到 24000/1001fps 的�?行扫�?? NTSC 内容，切�?�帧率。\n"

#define MSGTR_CacheFill "\r缓冲已填充：%5.2f%%（%"PRId64" 字节）"
#define MSGTR_NoBindFound "按键‘%s’未绑定功能。\n"
#define MSGTR_FailedToOpen "未能打开 %s。\n"

#define MSGTR_VideoID "[%s] 找到视频�?，-vid %d\n"
#define MSGTR_AudioID "[%s] 找到音频�?，-aid %d\n"
#define MSGTR_SubtitleID "[%s] 找到字幕�?，-sid %d\n"

// asfheader.c
#define MSGTR_MPDEMUX_ASFHDR_HeaderSizeOver1MB "致命错误：文件头部大�?超过 1 MB（%d）�?\n请�?�系 MPlayer 的作者, 并且�?��?或上传此文件。\n"
#define MSGTR_MPDEMUX_ASFHDR_HeaderMallocFailed "无法为文件头部分�? %d 字节的存放空间。\n"
#define MSGTR_MPDEMUX_ASFHDR_EOFWhileReadingHeader "读 ASF 头部时�?�到文件结尾，文件�?��??或�?完整？\n"
#define MSGTR_MPDEMUX_ASFHDR_DVRWantsLibavformat "DVR �?�能�?�能与 libavformat 一�?�使用，如果有问题请�?试 -demuxer 35。\n"
#define MSGTR_MPDEMUX_ASFHDR_NoDataChunkAfterHeader "文件头部之�?�没有数�?�段�?\n"
#define MSGTR_MPDEMUX_ASFHDR_AudioVideoHeaderNotFound "ASF：未找到音频或视频头部 - 文件�?��??？\n"
#define MSGTR_MPDEMUX_ASFHDR_InvalidLengthInASFHeader "ASF 头部长度无效�?\n"
#define MSGTR_MPDEMUX_ASFHDR_DRMLicenseURL "DRM 许�?��? URL 地�?�：%s\n"
#define MSGTR_MPDEMUX_ASFHDR_DRMProtected "该文件�?过了 DRM 加密�?护，无法在 Mplayer 中播放�?\n"

// aviheader.c
#define MSGTR_MPDEMUX_AVIHDR_EmptyList "**空列表？�?\n"
#define MSGTR_MPDEMUX_AVIHDR_WarnNotExtendedAVIHdr "** 警告：这�?是扩展格�?的 AVI 头部..\n"
#define MSGTR_MPDEMUX_AVIHDR_BuildingODMLidx "AVI：ODML：构造 ODML 索引 (%d superindexchunks)。\n"
#define MSGTR_MPDEMUX_AVIHDR_BrokenODMLfile "AVI：ODML：检测到�?��??的（�?完整的？）文件。将使用传统的索引模�?。\n"
#define MSGTR_MPDEMUX_AVIHDR_CantReadIdxFile "无法读索引文件 %s：%s\n"
#define MSGTR_MPDEMUX_AVIHDR_NotValidMPidxFile "%s �?是有效的 MPlayer 索引文件。\n"
#define MSGTR_MPDEMUX_AVIHDR_FailedMallocForIdxFile "无法为�?�自 %s 的索引数�?�分�?内存。\n"
#define MSGTR_MPDEMUX_AVIHDR_PrematureEOF "过早结�?�的索引文件 %s\n"
#define MSGTR_MPDEMUX_AVIHDR_IdxFileLoaded "已加载索引文件：%s\n"
#define MSGTR_MPDEMUX_AVIHDR_GeneratingIdx "正在生�?索引：%3lu %s     \r"
#define MSGTR_MPDEMUX_AVIHDR_IdxGeneratedForHowManyChunks "AVI：为 %d 段数�?�生�?索引表�?\n"
#define MSGTR_MPDEMUX_AVIHDR_Failed2WriteIdxFile "无法写入索引文件 %s：%s\n"
#define MSGTR_MPDEMUX_AVIHDR_IdxFileSaved "已�?存索引文件：%s\n"

// demux_audio.c
#define MSGTR_MPDEMUX_AUDIO_BadID3v2TagSize "音频分离器：ID3v2 标签大�?异常：大于�?的长度（%u）。\n"
#define MSGTR_MPDEMUX_AUDIO_DamagedAppendedID3v2Tag "音频分离器：检测到末尾中的 ID3v2 标签已�?��??。\n"
#define MSGTR_MPDEMUX_AUDIO_UnknownFormat "音频�?分解器：未知格�? %d。\n"

// demux_demuxers.c
#define MSGTR_MPDEMUX_DEMUXERS_FillBufferError "fill_buffer 错误：�?分解器错误：�?� vd�?ad 或 sd。\n"

// demux_mkv.c
#define MSGTR_MPDEMUX_MKV_ZlibInitializationFailed "[mkv] zlib �?始化失败。\n"
#define MSGTR_MPDEMUX_MKV_ZlibDecompressionFailed "[mkv] zlib 解压失败。\n"
#define MSGTR_MPDEMUX_MKV_LzoInitializationFailed "[mkv] lzo �?始化失败。\n"
#define MSGTR_MPDEMUX_MKV_LzoDecompressionFailed "[mkv] lzo 解压失败。\n"
#define MSGTR_MPDEMUX_MKV_TrackEncrypted "[mkv] 轨�?� %u 已加密，但解密还未实现。跳过该轨�?�。\n"
#define MSGTR_MPDEMUX_MKV_UnknownContentEncoding "[mkv] 轨�?� %u 内容的编�?类型未知。跳过该轨�?�。\n"
#define MSGTR_MPDEMUX_MKV_UnknownCompression "[mkv] 轨�?� %u 使用了未知/�?支�?的压缩算法（%u）。\n[mkv] 跳过该轨�?�。\n"
#define MSGTR_MPDEMUX_MKV_ZlibCompressionUnsupported "[mkv] 轨�?� %u 已�? zlib 压缩，但 mplayer 未以支�? zlib 压缩\n[mkv] 选项编译。跳过该轨�?�。\n"
#define MSGTR_MPDEMUX_MKV_TrackIDName "[mkv] 轨�?� ID %u：%s (%s) \"%s\"，%s\n"
#define MSGTR_MPDEMUX_MKV_TrackID "[mkv] 轨�?� ID %u：%s (%s), %s\n"
#define MSGTR_MPDEMUX_MKV_UnknownCodecID "[mkv] 未知的/�?支�?的编解�?器 ID（%s），也�?�能是编解�?器�?有数�?�\n[mkv] 缺少/�?��??（轨�?� %u）。\n"
#define MSGTR_MPDEMUX_MKV_FlacTrackDoesNotContainValidHeaders "[mkv] FLAC 轨�?�未�?�有效的头部信�?�。\n"
#define MSGTR_MPDEMUX_MKV_UnknownAudioCodec "[mkv] 未知/�?支�?的音频编解�?器 ID‘%s’（轨�?� %u），也�?�能是编解�?器\n[mkv] �?有数�?�缺少/�?��??。\n"
#define MSGTR_MPDEMUX_MKV_SubtitleTypeNotSupported "[mkv] 未支�?字幕类型‘%s’。\n"
#define MSGTR_MPDEMUX_MKV_WillPlayVideoTrack "[mkv] 将播放视频轨�?� %u。\n"
#define MSGTR_MPDEMUX_MKV_NoVideoTrackFound "[mkv] 未找到/�?需�?视频轨�?�。\n"
#define MSGTR_MPDEMUX_MKV_NoAudioTrackFound "[mkv] 未找到/�?需�?音频轨�?�。\n"
#define MSGTR_MPDEMUX_MKV_WillDisplaySubtitleTrack "[mkv] 将播放字幕轨�?� %u。\n"
#define MSGTR_MPDEMUX_MKV_NoBlockDurationForSubtitleTrackFound "[mkv] 警告：所找到的字幕轨�?�没有字幕段�?续时间信�?�。\n"
#define MSGTR_MPDEMUX_MKV_TooManySublines "[mkv] Warning：需渲染的字幕过多，跳过。\n"
#define MSGTR_MPDEMUX_MKV_TooManySublinesSkippingAfterFirst "\n[mkv] 警告：需渲染的字幕过多，路过�? %i 个�?�的字幕。n"

// demux_nuv.c
#define MSGTR_MPDEMUX_NUV_NoVideoBlocksInFile "文件中没有视频数�?�段。\n"

// demux_xmms.c
#define MSGTR_MPDEMUX_XMMS_FoundPlugin "找到�?�件：%s（%s）。\n"
#define MSGTR_MPDEMUX_XMMS_ClosingPlugin "关闭�?�件：%s。\n"
#define MSGTR_MPDEMUX_XMMS_WaitForStart "等待 XMMS �?�件开始播放‘%s’...\n"


// ========================== LIBMENU ===================================

// common
#define MSGTR_LIBMENU_NoEntryFoundInTheMenuDefinition "[MENU] �?��?�定义中未找到�?��?�项。\n"

// libmenu/menu.c
#define MSGTR_LIBMENU_SyntaxErrorAtLine "[MENU] 语法错误：第 %d 行\n"
#define MSGTR_LIBMENU_MenuDefinitionsNeedANameAttrib "[MENU] �?��?�定义需�?�??称属性（第 %d 行）。\n"
#define MSGTR_LIBMENU_BadAttrib "[MENU] 无效属性 %s=%s，�?��?�‘%s’的第 %d 行\n"
#define MSGTR_LIBMENU_UnknownMenuType "[MENU] 未知�?��?�类型‘%s’（第 %d 行）\n"
#define MSGTR_LIBMENU_CantOpenConfigFile "[MENU] 无法打开�?��?��?置文件：%s\n"
#define MSGTR_LIBMENU_ConfigFileIsTooBig "[MENU] �?置文件过大（> %d KB）\n"
#define MSGTR_LIBMENU_ConfigFileIsEmpty "[MENU] �?置文件为空。\n"
#define MSGTR_LIBMENU_MenuNotFound "[MENU] 未找到�?��?� %s。\n"
#define MSGTR_LIBMENU_MenuInitFailed "[MENU] �?��?�‘%s’：�?始化失败。\n"
#define MSGTR_LIBMENU_UnsupportedOutformat "[MENU] �?支�?该输出格�?�?\n"

// libmenu/menu_cmdlist.c
#define MSGTR_LIBMENU_ListMenuEntryDefinitionsNeedAName "[MENU] 定义列表�?��?�项需�?给出�??称（行 %d）。\n"
#define MSGTR_LIBMENU_ListMenuNeedsAnArgument "[MENU] 列表�?��?�需�?给出�?�数。\n"

// libmenu/menu_console.c
#define MSGTR_LIBMENU_WaitPidError "[MENU] Waitpid 错误：%s。\n"
#define MSGTR_LIBMENU_SelectError "[MENU] Select 错误。\n"
#define MSGTR_LIBMENU_ReadErrorOnChildFD "[MENU] �?进程的文件�??述符读�?�错误：%s。\n"
#define MSGTR_LIBMENU_ConsoleRun "[MENU] 终端�?行：%s ...\n"
#define MSGTR_LIBMENU_AChildIsAlreadyRunning "[MENU] �?进程已�?�?行。\n"
#define MSGTR_LIBMENU_ForkFailed "[MENU] Fork 失败�?�?�?\n"
#define MSGTR_LIBMENU_WriteError "[MENU] write 错误\n"

// libmenu/menu_filesel.c
#define MSGTR_LIBMENU_OpendirError "[MENU] opendir 错误：%s\n"
#define MSGTR_LIBMENU_ReallocError "[MENU] realloc 错误：%s\n"
#define MSGTR_LIBMENU_MallocError "[MENU] 内存分�?错误：%s\n"
#define MSGTR_LIBMENU_ReaddirError "[MENU] readdir 错误：%s\n"
#define MSGTR_LIBMENU_CantOpenDirectory "[MENU] 无法打开目录 %s。\n"

// libmenu/menu_param.c
#define MSGTR_LIBMENU_SubmenuDefinitionNeedAMenuAttribut "[MENU] 定义�?�?��?�需给出‘menu’属性。\n"
#define MSGTR_LIBMENU_InvalidProperty "[MENU] 首选项�?��?�中存在无效的属性值‘%s’。（行 %d）。\n"
#define MSGTR_LIBMENU_PrefMenuEntryDefinitionsNeed "[MENU] 定义首选项�?��?��?�目需给出一个有效的‘property’或‘txt’属性（行 %d）。\n"
#define MSGTR_LIBMENU_PrefMenuNeedsAnArgument "[MENU] 首选项�?��?�需�?给出�?�数。\n"

// libmenu/menu_pt.c
#define MSGTR_LIBMENU_CantfindTheTargetItem "[MENU] 无法找到目标项目？？\n"
#define MSGTR_LIBMENU_FailedToBuildCommand "[MENU] 未能生�?�?��?�命令：%s。\n"

// libmenu/menu_txt.c
#define MSGTR_LIBMENU_MenuTxtNeedATxtFileName "[MENU] 文本�?��?�需�?给出文本文件�??（�?�数文件）。\n"
#define MSGTR_LIBMENU_MenuTxtCantOpen "[MENU] 无法打开 %s。\n"
#define MSGTR_LIBMENU_WarningTooLongLineSplitting "[MENU] 警告，行过长。将其分割。\n"
#define MSGTR_LIBMENU_ParsedLines "[MENU] 已解�?行 %d。\n"

// libmenu/vf_menu.c
#define MSGTR_LIBMENU_UnknownMenuCommand "[MENU] 未知命令：‘%s’。\n"
#define MSGTR_LIBMENU_FailedToOpenMenu "[MENU] 未能打开�?��?�：‘%s’。\n"


// ========================== LIBMPCODECS ===================================

// dec_video.c & dec_audio.c:
#define MSGTR_CantOpenCodec "无法打开解�?器。\n"
#define MSGTR_CantCloseCodec "无法关闭解�?器。\n"

#define MSGTR_MissingDLLcodec "错误：无法打开所需的 DirectShow 编解�?器：%s\n"
#define MSGTR_ACMiniterror "无法加载/�?始化 Win32/ACM 音频解�?器（缺少 DLL 文件？）。\n"
#define MSGTR_MissingLAVCcodec "无法在 libavcodec 中找到解�?器‘%s’...\n"

#define MSGTR_MpegNoSequHdr "MPEG：致命错误：�?�索 MPEG �?列头部时�?�到文件结尾符。\n"
#define MSGTR_CannotReadMpegSequHdr "致命错误：无法读�?� MPEG �?列头部。\n"
#define MSGTR_CannotReadMpegSequHdrEx "致命错误：无法读�?� MPEG �?列头部扩展数�?�。\n"
#define MSGTR_BadMpegSequHdr "MPEG：无效的 MPEG �?列头部。\n"
#define MSGTR_BadMpegSequHdrEx "MPEG：无效的 MPEG �?列头部扩展数�?�。\n"

#define MSGTR_ShMemAllocFail "无法分�?共享内存。\n"
#define MSGTR_CantAllocAudioBuf "无法分�?音频输出缓冲。\n"

#define MSGTR_UnknownAudio "音频格�?未知或缺失 -> 无声音输出\n"

#define MSGTR_UsingExternalPP "[PP] 使用外部的�?�期处�?�过滤器，max q = %d。\n"
#define MSGTR_UsingCodecPP "[PP] 使用编解�?器的�?�期处�?�过滤器，max q = %d。\n"
#define MSGTR_VideoCodecFamilyNotAvailableStr "所请求使用的视频编解�?器类 [%s] (vfm=%s) �?�?�用。\n需在编译时�?�用该模�?�。\n"
#define MSGTR_AudioCodecFamilyNotAvailableStr "所请求使用的音频编解�?器类 [%s] (afm=%s) �?�?�用。\n需在编译时�?�用该模�?�。\n"
#define MSGTR_OpeningVideoDecoder "正打开视频解�?器：[%s] %s\n"
#define MSGTR_SelectedVideoCodec "所选视频编解�?器为：[%s] vfm: %s (%s)\n"
#define MSGTR_OpeningAudioDecoder "正打开音频解�?器：[%s] %s\n"
#define MSGTR_SelectedAudioCodec "所选音频编解�?器为：[%s] afm: %s (%s)\n"
#define MSGTR_VDecoderInitFailed "VDecoder �?始化失败 :(\n"
#define MSGTR_ADecoderInitFailed "ADecoder �?始化失败 :(\n"
#define MSGTR_ADecoderPreinitFailed "ADecoder 预先�?始化失败 :(\n"

// libmpcodecs/ad_dvdpcm.c:
#define MSGTR_SamplesWanted "需�?该格�?的编�?样本以优化相关技术支�?。有�?请�?�系开�?�者。\n"

// libmpcodecs/ad_libdv.c
#define MSGTR_MPCODECS_AudioFramesizeDiffers "[AD_LIBDV] 警告�?音频帧大�?�?一致�?read=%d  hdr=%d。\n"

// vd.c
#define MSGTR_CodecDidNotSet "VDec：编解�?器无法设置 sh->disp_w 和 sh->disp_h，�?试使用替代方法。\n"
#define MSGTR_CouldNotFindColorspace "无法找到匹�?的色彩空间 - 使用 -vf scale �?试...\n"
#define MSGTR_MovieAspectIsSet "电影宽高比为 %.2f:1 - 预先放大到正确的电影宽高比。\n"
#define MSGTR_MovieAspectUndefined "电影宽高比未定义 - 没有使用预先放大功能。\n"

// vd_dshow.c, vd_dmo.c
#define MSGTR_DownloadCodecPackage "你需�?�?�级/安装已编译编解�?器包。\n请访问 http:\/\/www.mplayerhq.hu/dload.html\n"

// libmpcodecs/vd_dmo.c vd_dshow.c vd_vfw.c
#define MSGTR_MPCODECS_CouldntAllocateImageForCinepakCodec "[VD_DMO] 无法为 cinepak 编解�?器分�?进程映�?。\n"

// libmpcodecs/vd_ffmpeg.c
#define MSGTR_MPCODECS_XVMCAcceleratedCodec "[VD_FFMPEG] �? XVMC 加速的编解�?器。\n"
#define MSGTR_MPCODECS_ArithmeticMeanOfQP "[VD_FFMPEG] QP 的算术平�?�值：%2.4f，QP 的调和平�?�值：%2.4f\n"
#define MSGTR_MPCODECS_DRIFailure "[VD_FFMPEG] DRI 失败。\n"
#define MSGTR_MPCODECS_CouldntAllocateImageForCodec "[VD_FFMPEG] 无法为编解�?器分�?进程镜�?。\n"
#define MSGTR_MPCODECS_XVMCAcceleratedMPEG2 "[VD_FFMPEG] �? XVMC 加速的 MPEG-2。\n"
#define MSGTR_MPCODECS_TryingPixfmt "[VD_FFMPEG] �?试 pixfmt=%d。\n"
#define MSGTR_MPCODECS_McGetBufferShouldWorkOnlyWithXVMC "[VD_FFMPEG] Mc_get_buffer �?�能用于 XVMC 加速�?�?"
#define MSGTR_MPCODECS_UnexpectedInitVoError "[VD_FFMPEG] 预料外的 Init_vo 错误。\n"
#define MSGTR_MPCODECS_UnrecoverableErrorRenderBuffersNotTaken "[VD_FFMPEG] 无法�?��?的错误，未获得渲染缓冲。\n"
#define MSGTR_MPCODECS_OnlyBuffersAllocatedByVoXvmcAllowed "[VD_FFMPEG] �?��?许 vo_xvmc 分�?的缓冲。\n"

// libmpcodecs/ve_lavc.c
#define MSGTR_MPCODECS_HighQualityEncodingSelected "[VE_LAVC] 已选择高�?质编�?（�?�实时编�?）�?\n"
#define MSGTR_MPCODECS_UsingConstantQscale "[VE_LAVC] 使用�?�定的 qscale = %f（VBR）。\n"

// libmpcodecs/ve_raw.c
#define MSGTR_MPCODECS_OutputWithFourccNotSupported "[VE_RAW] �?支�? FourCC [%x] 的原生输出�?\n"
#define MSGTR_MPCODECS_NoVfwCodecSpecified "[VE_RAW] 未指定必须的 VfW 编解�?器�?�?\n"

// vf.c
#define MSGTR_CouldNotFindVideoFilter "无法找到视频滤镜‘%s’。\n"
#define MSGTR_CouldNotOpenVideoFilter "无法打开视频滤镜‘%s’。\n"
#define MSGTR_OpeningVideoFilter "打开视频滤镜："
#define MSGTR_CannotFindColorspace "�?�使通过�?�值方�?，也无法找到匹�?的色彩空间 :(\n"

// libmpcodecs/vf_crop.c
#define MSGTR_MPCODECS_CropBadPositionWidthHeight "[CROP] 无效的�?置/宽度/高度 - �?剪区域在原始图�?范围外�?\n"

// libmpcodecs/vf_cropdetect.c
#define MSGTR_MPCODECS_CropArea "[CROP] �?剪区域：X: %d..%d  Y: %d..%d  (-vf crop=%d:%d:%d:%d)。\n"

// libmpcodecs/vf_format.c, vf_palette.c, vf_noformat.c
#define MSGTR_MPCODECS_UnknownFormatName "[VF_FORMAT] 未知格�?�??：‘%s’。\n"

// libmpcodecs/vf_framestep.c vf_noformat.c vf_palette.c vf_tile.c
#define MSGTR_MPCODECS_ErrorParsingArgument "[VF_FRAMESTEP] 解�?�?�数错误。\n"

// libmpcodecs/ve_vfw.c
#define MSGTR_MPCODECS_CompressorType "压缩器类型：%.4lx\n"
#define MSGTR_MPCODECS_CompressorSubtype "压缩器�?类型：%.4lx\n"
#define MSGTR_MPCODECS_CompressorFlags "压缩器标记：%lu，版本：%lu，ICM 版本：%lu\n"
#define MSGTR_MPCODECS_Flags "标记："
#define MSGTR_MPCODECS_Quality " 质�?"

// libmpcodecs/vf_expand.c
#define MSGTR_MPCODECS_FullDRNotPossible "无法使用完全 DR 模�?，�?试使用 SLICES�?\n"
#define MSGTR_MPCODECS_WarnNextFilterDoesntSupportSlices  "警告�?下一个滤镜�?支�? SLICES，准备使用 sig11...\n"
#define MSGTR_MPCODECS_FunWhydowegetNULL "为什么返回的是 NULL？？\n"

// libmpcodecs/vf_test.c, vf_yuy2.c, vf_yvu9.c
#define MSGTR_MPCODECS_WarnNextFilterDoesntSupport "下一个滤镜/视频输出�?支�? %s :(\n"


// ================================== LIBASS ====================================

// ass_bitmap.c
#define MSGTR_LIBASS_FT_Glyph_To_BitmapError "[ass] FT_Glyph_To_Bitmap 出错 %d \n"
#define MSGTR_LIBASS_UnsupportedPixelMode "[ass] 该�?素模�?�?支�?：%d\n"
#define MSGTR_LIBASS_GlyphBBoxTooLarge "[ass] 文本边界框太大：%dx%dpx\n"

// ass.c
#define MSGTR_LIBASS_NoStyleNamedXFoundUsingY "[ass] [%p] 警告：未找到样�?‘%s’，将使用‘%s’\n"
#define MSGTR_LIBASS_BadTimestamp "[ass] 时间戳错误\n"
#define MSGTR_LIBASS_BadEncodedDataSize "[ass] 编�?数�?�大�?错误\n"
#define MSGTR_LIBASS_FontLineTooLong "[ass] 字体的行高太长：%d, %s\n"
#define MSGTR_LIBASS_EventFormatHeaderMissing "[ass] 事件格�?头部数�?��?存在\n"
#define MSGTR_LIBASS_ErrorOpeningIconvDescriptor "[ass] 打开 iconv �??述符出错。\n"
#define MSGTR_LIBASS_ErrorRecodingFile "[ass] 记录到文件时出错。\n"
#define MSGTR_LIBASS_FopenFailed "[ass] ass_read_file(%s)：fopen 失败\n"
#define MSGTR_LIBASS_FseekFailed "[ass] ass_read_file(%s)：fseek 失败\n"
#define MSGTR_LIBASS_RefusingToLoadSubtitlesLargerThan100M "[ass] ass_read_file(%s)：拒�?装入大于 100M 的字幕\n"
#define MSGTR_LIBASS_ReadFailed "读�?�失败，%d：%s\n"
#define MSGTR_LIBASS_AddedSubtitleFileMemory "[ass] 已添加字幕文件：<内存> (%d 个样�?, %d 个事件)\n"
#define MSGTR_LIBASS_AddedSubtitleFileFname "[ass] 已添加字幕文件：%s (%d 个样�?, %d 个事件)\n"
#define MSGTR_LIBASS_FailedToCreateDirectory "[ass] 创建目录失败 %s\n"
#define MSGTR_LIBASS_NotADirectory "[ass] 这�?是目录：%s\n"

// ass_cache.c
#define MSGTR_LIBASS_TooManyFonts "[ass] 字体太多\n"
#define MSGTR_LIBASS_ErrorOpeningFont "[ass] 打开字体出错：%s, %d\n"

// ass_fontconfig.c
#define MSGTR_LIBASS_SelectedFontFamilyIsNotTheRequestedOne "[ass] fontconfig：所选字体�?是所�?求使用的：'%s' != '%s'\n"
#define MSGTR_LIBASS_UsingDefaultFontFamily "[ass] fontconfig_select：使用默认的字体类：(%s, %d, %d) -> %s, %d\n"
#define MSGTR_LIBASS_UsingDefaultFont "[ass] fontconfig_select: 使用缺�?字体: (%s, %d, %d) -> %s, %d\n"
#define MSGTR_LIBASS_UsingArialFontFamily "[ass] fontconfig_select：使用‘Arial’字体类：(%s, %d, %d) -> %s, %d\n"
#define MSGTR_LIBASS_FcInitLoadConfigAndFontsFailed "[ass] FcInitLoadConfigAndFonts 失败。\n"
#define MSGTR_LIBASS_UpdatingFontCache "[ass] 更新字体缓冲。\n"
#define MSGTR_LIBASS_BetaVersionsOfFontconfigAreNotSupported "[ass] �?支�?测试版本的 fontconfig。\n[ass] 在报告该缺陷�?请先更新至最新版。\n"
#define MSGTR_LIBASS_FcStrSetAddFailed "[ass] FcStrSetAdd 失败。\n"
#define MSGTR_LIBASS_FcDirScanFailed "[ass] FcDirScan 失败。\n"
#define MSGTR_LIBASS_FcDirSave "[ass] FcDirSave 失败。\n"
#define MSGTR_LIBASS_FcConfigAppFontAddDirFailed "[ass] FcConfigAppFontAddDir 失败\n"
#define MSGTR_LIBASS_FontconfigDisabledDefaultFontWillBeUsed "[ass] Fontconfig 已�?用，将�?�使用默认字体。\n"
#define MSGTR_LIBASS_FunctionCallFailed "[ass] %s 失败\n"

// ass_render.c
#define MSGTR_LIBASS_NeitherPlayResXNorPlayResYDefined "[ass] PlayResX 和 PlayResY �?�未定义，使用 384x288。\n"
#define MSGTR_LIBASS_PlayResYUndefinedSettingY "[ass] PlayResY 未定义，设为 %d。\n"
#define MSGTR_LIBASS_PlayResXUndefinedSettingX "[ass] PlayResX 未定义，设为 %d。\n"
#define MSGTR_LIBASS_FT_Init_FreeTypeFailed "[ass] FT_Init_FreeType 失败。\n"
#define MSGTR_LIBASS_Init "[ass] �?始化\n"
#define MSGTR_LIBASS_InitFailed "[ass] �?始化失败。\n"
#define MSGTR_LIBASS_BadCommand "[ass] 命令无效：%c%c\n"
#define MSGTR_LIBASS_ErrorLoadingGlyph  "[ass] 装入字形出错。\n"
#define MSGTR_LIBASS_FT_Glyph_Stroke_Error "[ass] FT_Glyph_Stroke 错误 %d\n"
#define MSGTR_LIBASS_UnknownEffectType_InternalError "[ass] 未知的效果类型 (内部错误)\n"
#define MSGTR_LIBASS_NoStyleFound "[ass] 未找到样�?�?\n"
#define MSGTR_LIBASS_EmptyEvent "[ass] 空事件�?\n"
#define MSGTR_LIBASS_MAX_GLYPHS_Reached "[ass] 到达 MAX_GLYPHS 上�?：事件 %d, 开始 = %llu, �?续时长 = %llu\n 文本 = %s\n"
#define MSGTR_LIBASS_EventHeightHasChanged "[ass] 警告�?事件高度已改�?��?\n"

// ass_font.c
#define MSGTR_LIBASS_GlyphNotFoundReselectingFont "[ass] 未找到字形 0x%X，(%s, %d, %d) 选用�?�一�?字体\n"
#define MSGTR_LIBASS_GlyphNotFound "[ass] 未在字体中找到字形 0x%X 以用于 (%s, %d, %d)\n"
#define MSGTR_LIBASS_ErrorOpeningMemoryFont "[ass] 打开内存字体出错：%s\n"
#define MSGTR_LIBASS_NoCharmaps "[ass] 字体缺少字符映射表\n"
#define MSGTR_LIBASS_NoCharmapAutodetected "[ass] 未自动测出字符映射表，�?试用第一个。\n"


// ================================== stream ====================================

// ai_alsa.c
#define MSGTR_MPDEMUX_AIALSA_CannotSetSamplerate "无法设置采样率。\n"
#define MSGTR_MPDEMUX_AIALSA_CannotSetBufferTime "无法设置缓冲时间。\n"
#define MSGTR_MPDEMUX_AIALSA_CannotSetPeriodTime "无法设置采样间隔时间。\n"

// ai_alsa.c
#define MSGTR_MPDEMUX_AIALSA_PcmBrokenConfig "该 PCM 的�?置文件已�?��??：无�?�用�?置。\n"
#define MSGTR_MPDEMUX_AIALSA_UnavailableAccessType "无�?�用访问类型。\n"
#define MSGTR_MPDEMUX_AIALSA_UnavailableSampleFmt "无�?�用采样格�?。\n"
#define MSGTR_MPDEMUX_AIALSA_UnavailableChanCount "无�?�用声�?�数值 - 使用默认值：%d\n"
#define MSGTR_MPDEMUX_AIALSA_CannotInstallHWParams "无法设置硬件�?�数：%s"
#define MSGTR_MPDEMUX_AIALSA_PeriodEqualsBufferSize "无法使用长度等于缓冲大�?的采样间隔 (%u == %lu)\n"
#define MSGTR_MPDEMUX_AIALSA_CannotInstallSWParams "无法设置软件�?�数：\n"
#define MSGTR_MPDEMUX_AIALSA_ErrorOpeningAudio "打开音频出错：%s\n"
#define MSGTR_MPDEMUX_AIALSA_AlsaXRUN "ALSA xrun!!!（长度至少为 %.3f ms）\n"
#define MSGTR_MPDEMUX_AIALSA_AlsaXRUNPrepareError "ALSA xrun：准备出错：%s"
#define MSGTR_MPDEMUX_AIALSA_AlsaReadWriteError "ALSA 读/写错误"

// ai_oss.c
#define MSGTR_MPDEMUX_AIOSS_Unable2SetChanCount "无法设置声�?�数：%d\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2SetStereo "无法设置立体声：%d\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2Open "无法打开‘%s’：%s\n"
#define MSGTR_MPDEMUX_AIOSS_UnsupportedFmt "格�?�?支�?\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2SetAudioFmt "无法设置音频格�?。"
#define MSGTR_MPDEMUX_AIOSS_Unable2SetSamplerate "无法设置采样率：%d\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2SetTrigger "无法设置触�?�器：%d\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2GetBlockSize "无法获�?�数�?��?�的大�?�?\n"
#define MSGTR_MPDEMUX_AIOSS_AudioBlockSizeZero "音频数�?��?�的大�?为零，设为 %d�?\n"
#define MSGTR_MPDEMUX_AIOSS_AudioBlockSize2Low "音频数�?��?�的大�?太�?，设为 %d�?\n"

// asf_mmst_streaming.c
#define MSGTR_MPDEMUX_MMST_WriteError "写入出错\n"
#define MSGTR_MPDEMUX_MMST_EOFAlert "\n�??示�?到达文件末端\n"
#define MSGTR_MPDEMUX_MMST_PreHeaderReadFailed "预头部数�?��?�失败\n"
#define MSGTR_MPDEMUX_MMST_InvalidHeaderSize "头部数�?�大�?无效，放弃�?作。\n"
#define MSGTR_MPDEMUX_MMST_HeaderDataReadFailed "读�?�头部数�?�失败。\n"
#define MSGTR_MPDEMUX_MMST_packet_lenReadFailed "读�?� packet_len 失败。\n"
#define MSGTR_MPDEMUX_MMST_InvalidRTSPPacketSize "RTSP 包大�?无效，放弃�?作。\n"
#define MSGTR_MPDEMUX_MMST_CmdDataReadFailed "读�?�命令数�?�失败。\n"
#define MSGTR_MPDEMUX_MMST_HeaderObject "头部数�?�对象\n"
#define MSGTR_MPDEMUX_MMST_DataObject "数�?�对象\n"
#define MSGTR_MPDEMUX_MMST_FileObjectPacketLen "文件对象，包长 = %d (%d)\n"
#define MSGTR_MPDEMUX_MMST_StreamObjectStreamID "�?对象，�? ID: %d\n"
#define MSGTR_MPDEMUX_MMST_2ManyStreamID "ID 过多，已跳过该数�?��?。"
#define MSGTR_MPDEMUX_MMST_UnknownObject "未知对象\n"
#define MSGTR_MPDEMUX_MMST_MediaDataReadFailed "读�?�媒体数�?�失败\n"
#define MSGTR_MPDEMUX_MMST_MissingSignature "签�??数�?��?存在\n"
#define MSGTR_MPDEMUX_MMST_PatentedTechnologyJoke "所有�?作结�?�。感谢下载�?�有知识产�?��?��?�专利�?护技术的媒体文件。\n"
#define MSGTR_MPDEMUX_MMST_UnknownCmd "未知命令 %02x\n"
#define MSGTR_MPDEMUX_MMST_GetMediaPacketErr "get_media_packet 出错：%s\n"
#define MSGTR_MPDEMUX_MMST_Connected "已连接\n"

// asf_streaming.c
#define MSGTR_MPDEMUX_ASF_StreamChunkSize2Small "噢，stream_chunck 的大�?过�?：%d\n"
#define MSGTR_MPDEMUX_ASF_SizeConfirmMismatch "size_confirm �?匹�?：%d %d\n"
#define MSGTR_MPDEMUX_ASF_WarnDropHeader "警告：丢弃头部数�?�？？\n"
#define MSGTR_MPDEMUX_ASF_ErrorParsingChunkHeader "解�?数�?�区�?�头部时出错\n"
#define MSGTR_MPDEMUX_ASF_NoHeaderAtFirstChunk "未获得作为第一个区�?�的头部数�?��?�?\n"
#define MSGTR_MPDEMUX_ASF_BufferMallocFailed "错误：无法分�?长度为 %d 字节的缓冲。\n"
#define MSGTR_MPDEMUX_ASF_ErrReadingNetworkStream "读�?�网络�?时出错。\n"
#define MSGTR_MPDEMUX_ASF_ErrChunk2Small "错误：区�?�太�?。\n"
#define MSGTR_MPDEMUX_ASF_ErrSubChunkNumberInvalid "错误：�?区�?��?�无效。\n"
#define MSGTR_MPDEMUX_ASF_Bandwidth2SmallCannotPlay "带宽太�?，无法播放文件�?\n"
#define MSGTR_MPDEMUX_ASF_Bandwidth2SmallDeselectedAudio "带宽太�?，�?�消选定的音频�?。\n"
#define MSGTR_MPDEMUX_ASF_Bandwidth2SmallDeselectedVideo "带宽太�?，�?�消选定的视频�?。\n"
#define MSGTR_MPDEMUX_ASF_InvalidLenInHeader "ASF 头部数�?�长度无效�?\n"
#define MSGTR_MPDEMUX_ASF_ErrReadingChunkHeader "读�?�区�?�的头部数�?�时出错。\n"
#define MSGTR_MPDEMUX_ASF_ErrChunkBiggerThanPacket "错误：chunk_size > packet_size\n"
#define MSGTR_MPDEMUX_ASF_ErrReadingChunk "读�?�数�?�区�?�时出错。\n"
#define MSGTR_MPDEMUX_ASF_ASFRedirector "=====> ASF �?定�?�器\n"
#define MSGTR_MPDEMUX_ASF_InvalidProxyURL "代�?� URL 无效\n"
#define MSGTR_MPDEMUX_ASF_UnknownASFStreamType "未知的 ASF �?类型\n"
#define MSGTR_MPDEMUX_ASF_Failed2ParseHTTPResponse "解�? HTTP �?应失败。\n"
#define MSGTR_MPDEMUX_ASF_ServerReturn "�?务器返回 %d:%s\n"
#define MSGTR_MPDEMUX_ASF_ASFHTTPParseWarnCuttedPragma "ASF HTTP 解�?警告：指令 %s 被从 %zu 字节截断至 %zu 字节处\n"
#define MSGTR_MPDEMUX_ASF_SocketWriteError "Socket 写入出错：%s\n"
#define MSGTR_MPDEMUX_ASF_HeaderParseFailed "解�?头部数�?�失败。\n"
#define MSGTR_MPDEMUX_ASF_NoStreamFound "未找到数�?��?。\n"
#define MSGTR_MPDEMUX_ASF_UnknownASFStreamingType "未知 ASF �?类型\n"
#define MSGTR_MPDEMUX_ASF_InfoStreamASFURL "STREAM_ASF，URL：%s\n"
#define MSGTR_MPDEMUX_ASF_StreamingFailed "�?作失败，退出中。\n"

// audio_in.c
#define MSGTR_MPDEMUX_AUDIOIN_ErrReadingAudio "\n读�?�音频出错：%s\n"
#define MSGTR_MPDEMUX_AUDIOIN_XRUNSomeFramesMayBeLeftOut "已从交�?�干扰状�?�?��?, �?些帧�?�能已丢失�?\n"
#define MSGTR_MPDEMUX_AUDIOIN_ErrFatalCannotRecover "致命错误，无法�?��?�?\n"
#define MSGTR_MPDEMUX_AUDIOIN_NotEnoughSamples "\n音频采样数�?��?足�?\n"

// cache2.c
#define MSGTR_MPDEMUX_CACHE2_NonCacheableStream "\r该媒体�?�?�?�缓存。\n"
#define MSGTR_MPDEMUX_CACHE2_ReadFileposDiffers "!!! read_filepos 有出入!!! 请报告该缺陷...\n"

// network.c
#define MSGTR_MPDEMUX_NW_UnknownAF "未知网络地�?�类 %d\n"
#define MSGTR_MPDEMUX_NW_ResolvingHostForAF "正在解�? %s 用于 %s...\n"
#define MSGTR_MPDEMUX_NW_CantResolv "无解�?网络�??用于 %s：%s\n"
#define MSGTR_MPDEMUX_NW_ConnectingToServer "正在连接到�?务器 %s[%s]：%d...\n"
#define MSGTR_MPDEMUX_NW_CantConnect2Server "使用 %s 连接�?务器失败\n"
#define MSGTR_MPDEMUX_NW_SelectFailed "select 调用失败。\n"
#define MSGTR_MPDEMUX_NW_ConnTimeout "连接超时\n"
#define MSGTR_MPDEMUX_NW_GetSockOptFailed "getsockopt 失败：%s\n"
#define MSGTR_MPDEMUX_NW_ConnectError "连接错误：%s\n"
#define MSGTR_MPDEMUX_NW_InvalidProxySettingTryingWithout "代�?�设置无效... �?试�?使用代�?�。\n"
#define MSGTR_MPDEMUX_NW_CantResolvTryingWithoutProxy "无法解�? AF_INET 的远程主机�??。�?试�?使用代�?�。\n"
#define MSGTR_MPDEMUX_NW_ErrSendingHTTPRequest "�?��? HTTP 请求时出错：未完全�?�出所有请求。\n"
#define MSGTR_MPDEMUX_NW_ReadFailed "读�?�失败。\n"
#define MSGTR_MPDEMUX_NW_Read0CouldBeEOF "http_read_response 读到 0（例如：文件末端）。\n"
#define MSGTR_MPDEMUX_NW_AuthFailed "验�?失败。请使用 -user 和 -passwd 选项�?�指定你的\n"\
"用户�??/密�?，以�??供给一组 URL 使用，或者使用如下的 URL 格�?：\n"\
"http://username:password@hostname/file\n"
#define MSGTR_MPDEMUX_NW_AuthRequiredFor "%s 需�?验�?\n"
#define MSGTR_MPDEMUX_NW_AuthRequired "需�?验�?。\n"
#define MSGTR_MPDEMUX_NW_NoPasswdProvidedTryingBlank "未�??供密�?，�?试使用空密�?。\n"
#define MSGTR_MPDEMUX_NW_ErrServerReturned "�?务器返回 %d：%s\n"
#define MSGTR_MPDEMUX_NW_CacheSizeSetTo "缓存大�?设为 %d �?�字节\n"

// open.c, stream.c:
#define MSGTR_CdDevNotfound "未找到 CD-ROM 设备‘%s’。\n"
#define MSGTR_ErrTrackSelect "选择 VCD 轨�?�出错。"
#define MSGTR_ReadSTDIN "正在从标准输入中读�?�...\n"
#define MSGTR_UnableOpenURL "无法打开 URL：%s\n"
#define MSGTR_ConnToServer "已连接到�?务器：%s\n"
#define MSGTR_FileNotFound "未找到文件：‘%s’\n"

#define MSGTR_SMBInitError "无法�?始化 libsmbclient 库：%d\n"
#define MSGTR_SMBFileNotFound "无法通过局域网打开：‘%s’\n"
#define MSGTR_SMBNotCompiled "MPlayer 编译时未包�?� SMB 读�?�的支�?。\n"

#define MSGTR_CantOpenBluray "无法打开�?光设备：%s\n"
#define MSGTR_CantOpenDVD "无法打开 DVD 设备：%s（%s）\n"

#define MSGTR_URLParsingFailed "URL %s 解�?失败\n"
#define MSGTR_FailedSetStreamOption "未能设置媒体�?选项 %s=%s\n"
#define MSGTR_StreamNeedType "媒体�?需有类型�?\n"
#define MSGTR_StreamProtocolNULL "媒体�?类型 %s 的 protocols == NULL，这是软件缺陷\n"
#define MSGTR_StreamCantHandleURL "未找到媒体�?以处�?� URL %s\n"
#define MSGTR_StreamNULLFilename "open_output_stream()，文件�??为 NULL，请报告该缺陷\n"
#define MSGTR_StreamErrorWritingCapture "写入�?�?�获文件错误：%s\n"
#define MSGTR_StreamSeekFailed "�?定�?失败\n"
#define MSGTR_StreamNotSeekable "该�?无法定�?�?\n"
#define MSGTR_StreamCannotSeekBackward "无法在线性媒体�?中�?��?�定�?�?\n"

// stream_cdda.c
#define MSGTR_MPDEMUX_CDDA_CantOpenCDDADevice "无法打开 CDDA 设备。\n"
#define MSGTR_MPDEMUX_CDDA_CantOpenDisc "无法打开光盘。\n"
#define MSGTR_MPDEMUX_CDDA_AudioCDFoundWithNTracks "�?�现音频 CD，共 %d 音轨。\n"

// stream_cddb.c
#define MSGTR_MPDEMUX_CDDB_FailedToReadTOC "读�?� TOC 失败。\n"
#define MSGTR_MPDEMUX_CDDB_FailedToOpenDevice "打开 %s 设备失败。\n"
#define MSGTR_MPDEMUX_CDDB_NotAValidURL "�?是有效的 URL\n"
#define MSGTR_MPDEMUX_CDDB_FailedToSendHTTPRequest "�?��? HTTP 请求失败。\n"
#define MSGTR_MPDEMUX_CDDB_FailedToReadHTTPResponse "读�?� HTTP �?应失败。\n"
#define MSGTR_MPDEMUX_CDDB_HTTPErrorNOTFOUND "未找到。\n"
#define MSGTR_MPDEMUX_CDDB_HTTPErrorUnknown "未知错误代�?\n"
#define MSGTR_MPDEMUX_CDDB_NoCacheFound "未找到缓存。\n"
#define MSGTR_MPDEMUX_CDDB_NotAllXMCDFileHasBeenRead "没有读出所有的 xmcd 文件。\n"
#define MSGTR_MPDEMUX_CDDB_FailedToCreateDirectory "创建目录 %s 失败。\n"
#define MSGTR_MPDEMUX_CDDB_NotAllXMCDFileHasBeenWritten "没有写入所有的 xmcd 文件。\n"
#define MSGTR_MPDEMUX_CDDB_InvalidXMCDDatabaseReturned "返回的 xmcd 数�?�库文件无效。\n"
#define MSGTR_MPDEMUX_CDDB_UnexpectedFIXME "�?常情况。请修�?\n"
#define MSGTR_MPDEMUX_CDDB_UnhandledCode "未处�?�的代�?\n"
#define MSGTR_MPDEMUX_CDDB_UnableToFindEOL "无法找到行末尾。\n"
#define MSGTR_MPDEMUX_CDDB_ParseOKFoundAlbumTitle "解�?�?功，找到：%s\n"
#define MSGTR_MPDEMUX_CDDB_AlbumNotFound "未找到专辑。\n"
#define MSGTR_MPDEMUX_CDDB_ServerReturnsCommandSyntaxErr "�?务器返回内容：命令语法错误\n"
#define MSGTR_MPDEMUX_CDDB_NoSitesInfoAvailable "无�?�用的站点信�?�。\n"
#define MSGTR_MPDEMUX_CDDB_FailedToGetProtocolLevel "获得�??议级别失败。\n"
#define MSGTR_MPDEMUX_CDDB_NoCDInDrive "驱动器里没有 CD。\n"

// stream_cue.c
#define MSGTR_MPDEMUX_CUEREAD_UnexpectedCuefileLine "[bincue] cue 文件中�?应出现的一行内容：%s\n"
#define MSGTR_MPDEMUX_CUEREAD_BinFilenameTested "[bincue] 已测试 bin 文件�??：%s\n"
#define MSGTR_MPDEMUX_CUEREAD_CannotFindBinFile "[bincue] 无法找到 bin 文件 - 正在放弃�?试。\n"
#define MSGTR_MPDEMUX_CUEREAD_UsingBinFile "[bincue] 正在使用 bin 文件 %s。\n"
#define MSGTR_MPDEMUX_CUEREAD_UnknownModeForBinfile "[bincue] 未知 bin 文件模�?。�?应出现该现象。正在中断�?作。\n"
#define MSGTR_MPDEMUX_CUEREAD_CannotOpenCueFile "[bincue] 无法打开 %s。\n"
#define MSGTR_MPDEMUX_CUEREAD_ErrReadingFromCueFile "[bincue] 读�?� %s 出错\n"
#define MSGTR_MPDEMUX_CUEREAD_ErrGettingBinFileSize "[bincue] 获�?� bin 文件大�?时出错。\n"
#define MSGTR_MPDEMUX_CUEREAD_InfoTrackFormat "音轨 %02d： 格�?=%d  %02d:%02d:%02d\n"
#define MSGTR_MPDEMUX_CUEREAD_UnexpectedBinFileEOF "[bincue] bin 文件�?�到�?应出现的结尾\n"
#define MSGTR_MPDEMUX_CUEREAD_CannotReadNBytesOfPayload "[bincue] 无法读�?� %d 字节的负载数�?�。\n"
#define MSGTR_MPDEMUX_CUEREAD_CueStreamInfo_FilenameTrackTracksavail "CUE stream_open, 文件�??=%s, 音轨=%d, �?�用音轨：%d -> %d\n"

// stream_dvd.c
#define MSGTR_DVDspeedCantOpen "无法以写入方�?打开 DVD 设备，改�?� DVD 速度需有写入的�?��?。\n"
#define MSGTR_DVDrestoreSpeed "�?��? DVD 速度... "
#define MSGTR_DVDlimitSpeed "�?制 DVD 速度为 %dKB/s... "
#define MSGTR_DVDlimitFail "失败。\n"
#define MSGTR_DVDlimitOk "�?功。\n"
#define MSGTR_NoDVDSupport "MPlayer 编译时未包括对 DVD 的支�?，正在退出。\n"
#define MSGTR_DVDnumTitles "该 DVD 有 %d 个标题。\n"
#define MSGTR_DVDinvalidTitle "DVD 标题编�?�无效：%d\n"
#define MSGTR_DVDnumChapters "该 DVD 标题有 %d 章节。\n"
#define MSGTR_DVDinvalidChapter "DVD 章节编�?�无效：%d\n"
#define MSGTR_DVDinvalidChapterRange "章节范围的说明数�?�无效 %s\n"
#define MSGTR_DVDinvalidLastChapter "上次播放的 DVD 章节编�?�无效：%d\n"
#define MSGTR_DVDnumAngles "该 DVD 标题有 %d 个视角。\n"
#define MSGTR_DVDinvalidAngle "DVD 视角编�?�无效：%d\n"
#define MSGTR_DVDnoIFO "无法打开 DVD 标题 %d 的 IFO 文件。\n"
#define MSGTR_DVDnoVMG "无法打开 VMG 信�?��?\n"
#define MSGTR_DVDnoVOBs "无法打开标题的 VOBS 文件（VTS_%02d_1.VOB）。\n"
#define MSGTR_DVDnoMatchingAudio "未找到匹�?语言的 DVD 音频�?\n"
#define MSGTR_DVDaudioChannel "已选择 DVD 音频的频�?�：%d 语言：%c%c\n"
#define MSGTR_DVDaudioStreamInfo "音频�?：%d 格�?：%s (%s) 语言：%s 音频�?�：%d。\n"
#define MSGTR_DVDnumAudioChannels "光盘的音频频�?�数：%d。\n"
#define MSGTR_DVDnoMatchingSubtitle "未找到匹�?语言的 DVD 字幕�?\n"
#define MSGTR_DVDsubtitleChannel "已选 DVD 字幕频�?�：%d 语言：%c%c\n"
#define MSGTR_DVDsubtitleLanguage "字幕�?�：%d 语言：%s\n"
#define MSGTR_DVDnumSubtitles "光盘上的字幕数：%d\n"

// stream_bluray.c
#define MSGTR_BlurayNoDevice "未指定�?光 DVD 的设备/�?置...\n"
#define MSGTR_BlurayNoTitles "无法在此找到与�?光 DVD 相兼容的标题内容。\n"
#define MSGTR_BlurayOK "�?功打开�?光 DVD。\n"

// stream_radio.c
#define MSGTR_RADIO_ChannelNamesDetected "[radio] 已检测到无线电频�?��??。\n"
#define MSGTR_RADIO_WrongFreqForChannel "[radio] 频�?� %s 的频率错误\n"
#define MSGTR_RADIO_WrongChannelNumberFloat "[radio] 错误的通�?��?�: %.2f\n"
#define MSGTR_RADIO_WrongChannelNumberInt "[radio] 频�?��?�错误：%d\n"
#define MSGTR_RADIO_WrongChannelName "[radio] 频�?��??错误：%s\n"
#define MSGTR_RADIO_FreqParameterDetected "[radio] 已检测到无线电频率�?�数。\n"
#define MSGTR_RADIO_GetTunerFailed "[radio] 警告：ioctl 获�?�无线电调�?器失败：%s。设置 frac 为 %d。\n"
#define MSGTR_RADIO_NotRadioDevice "[radio] %s �?�无线电设备�?\n"
#define MSGTR_RADIO_SetFreqFailed "[radio] ioctl 设置频率 0x%x (%.2f) 失败：%s\n"
#define MSGTR_RADIO_GetFreqFailed "[radio] ioctl 获�?�频率失败：%s\n"
#define MSGTR_RADIO_SetMuteFailed "[radio] ioctl 设置�?�音失败：%s\n"
#define MSGTR_RADIO_QueryControlFailed "[radio] ioctl 查询控制状�?失败：%s\n"
#define MSGTR_RADIO_GetVolumeFailed "[radio] ioctl 获�?�音�?失败：%s\n"
#define MSGTR_RADIO_SetVolumeFailed "[radio] ioctl 设置音�?失败：%s\n"
#define MSGTR_RADIO_DroppingFrame "\n[radio] 太糟了 - 丢弃音频帧（%d 字节）�?\n"
#define MSGTR_RADIO_BufferEmpty "[radio] grab_audio_frame：缓冲为空，等待 %d 字节的数�?�。\n"
#define MSGTR_RADIO_AudioInitFailed "[radio] audio_in_init 失败：%s\n"
#define MSGTR_RADIO_AllocateBufferFailed "[radio] 无法分�?音频缓冲 (block=%d,buf=%d)：%s\n"
#define MSGTR_RADIO_CurrentFreq "[radio] 当�?频率：%.2f\n"
#define MSGTR_RADIO_SelectedChannel "[radio] 已选择频�?�：%d - %s（频率：%.2f）\n"
#define MSGTR_RADIO_ChangeChannelNoChannelList "[radio] 无法切�?�频�?�：未给出的频�?�列表。\n"
#define MSGTR_RADIO_UnableOpenDevice "[radio] 无法打开‘%s’：%s\n"
#define MSGTR_RADIO_InitFracFailed "[radio] init_frac 失败。\n"
#define MSGTR_RADIO_WrongFreq "[radio] 频率错误：%.2f\n"
#define MSGTR_RADIO_UsingFreq "[radio] 使用频率：%.2f。\n"
#define MSGTR_RADIO_AudioInInitFailed "[radio] audio_in_init 失败。\n"
#define MSGTR_RADIO_AudioInSetupFailed "[radio] audio_in_setup 调用失败：%s\n"
#define MSGTR_RADIO_ClearBufferFailed "[radio] 清空缓冲失败：%s\n"
#define MSGTR_RADIO_StreamEnableCacheFailed "[radio] 调用 stream_enable_cache 失败：%s\n"
#define MSGTR_RADIO_DriverUnknownStr "[radio] 未知驱动�??称：%s\n"
#define MSGTR_RADIO_DriverV4L2 "[radio] 使用 V4Lv2 无线电接�?�。\n"
#define MSGTR_RADIO_DriverV4L "[radio] 使用 V4Lv1 无线电接�?�。\n"
#define MSGTR_RADIO_DriverBSDBT848 "[radio] 使用 *BSD BT848 无线电接�?�。\n"

//tv.c
#define MSGTR_TV_BogusNormParameter "tv.c：norm_from_string(%s)：规格化�?�数�?设置�?�法 %s。\n"
#define MSGTR_TV_NoVideoInputPresent "错误：视频输入�?存在�?\n"
#define MSGTR_TV_UnknownImageFormat ""\
"==================================================================\n"\
" 警告：请求输出的图�?格�?未�?测试或未知（0x%x）\n"\
" 这�?�能导致播放故障或程�?崩溃�?缺陷报告将被忽略�?你应该�?次�?试使用\n"\
" YV12（这是默认的色彩空间）并阅读文档�?\n"\
"==================================================================\n"

#define MSGTR_TV_CannotSetNorm "错误：无法设置规格化�?�数�?\n"
#define MSGTR_TV_MJP_WidthHeight "  MJP：宽 %d 高 %d\n"
#define MSGTR_TV_UnableToSetWidth "无法设置为所请求的宽度：%d\n"
#define MSGTR_TV_UnableToSetHeight "无法设置为所请求的高度：%d\n"
#define MSGTR_TV_NoTuner "所选输入没有频�?�调�?器�?\n"
#define MSGTR_TV_UnableFindChanlist "无法找到所选频�?�列表�?（%s）\n"
#define MSGTR_TV_ChannelFreqParamConflict "无法�?�时设置频率和频�?��?\n"
#define MSGTR_TV_ChannelNamesDetected "已检测到电视频�?��??称。\n"
#define MSGTR_TV_NoFreqForChannel "无法找到 %s（%s）频�?�的频率\n"
#define MSGTR_TV_SelectedChannel3 "已选择频�?�：%s - %s（频率：%.3f）\n"
#define MSGTR_TV_SelectedChannel2 "已选择频�?�：%s（频率：%.3f）\n"
#define MSGTR_TV_UnsupportedAudioType "音频类型‘%s（%x）’�?支�?�?\n"
#define MSGTR_TV_AvailableDrivers "�?�用驱动：\n"
#define MSGTR_TV_DriverInfo "已选择驱动：%s\n �??称：%s\n 作者：%s\n 注释：%s\n"
#define MSGTR_TV_NoSuchDriver "没有这�?驱动：%s\n"
#define MSGTR_TV_DriverAutoDetectionFailed "自动检测电视驱动失败。\n"
#define MSGTR_TV_UnknownColorOption "使用了未知色彩选项（%d）�?\n"
#define MSGTR_TV_NoTeletext "无图文电视功能"
#define MSGTR_TV_Bt848IoctlFailed "tvi_bsdbt848：调用 %s ioctl 失败。错误信�?�：%s\n"
#define MSGTR_TV_Bt848InvalidAudioRate "tvi_bsdbt848：无效的音频�?率值。错误信�?�：%s\n"
#define MSGTR_TV_Bt848ErrorOpeningBktrDev "tvi_bsdbt848：无法打开 bktr 设备。错误信�?�：%s\n"
#define MSGTR_TV_Bt848ErrorOpeningTunerDev "tvi_bsdbt848：无法打开调�?设备。错误信�?�：%s\n"
#define MSGTR_TV_Bt848ErrorOpeningDspDev "tvi_bsdbt848：无法打开 dsp 设备。错误信�?�：%s\n"
#define MSGTR_TV_Bt848ErrorConfiguringDsp "tvi_bsdbt848：�?置 dsp 失败。错误信�?�：%s\n"
#define MSGTR_TV_Bt848ErrorReadingAudio "tvi_bsdbt848：读�?�音频数�?�出错。错误信�?�：%s\n"
#define MSGTR_TV_Bt848MmapFailed "tvi_bsdbt848：调用 mmap 失败。错误信�?�：%s\n"
#define MSGTR_TV_Bt848FrameBufAllocFailed "tvi_bsdbt848：分�?帧缓冲失败。错误信�?�：%s\n"
#define MSGTR_TV_Bt848ErrorSettingWidth "tvi_bsdbt848：设置画�?�宽度出错。错误信�?�：%s\n"
#define MSGTR_TV_Bt848ErrorSettingHeight "tvi_bsdbt848：设置画�?�高度出错。错误信�?�：%s\n"
#define MSGTR_TV_Bt848UnableToStopCapture "tvi_bsdbt848：无法�?�止�?��?�视频。错误信�?�：%s\n"
#define MSGTR_TV_TTSupportedLanguages "支�?的图文电视语言：\n"
#define MSGTR_TV_TTSelectedLanguage "已选择默认图文电视语言：%s\n"
#define MSGTR_TV_ScannerNotAvailableWithoutTuner "没有频�?�调�?器无法使用频�?��?�索器\n"

//tvi_dshow.c
#define MSGTR_TVI_DS_UnableConnectInputVideoDecoder  "无法将所给输入连接至视频解�?器。错误代�?：0x%x\n"
#define MSGTR_TVI_DS_UnableConnectInputAudioDecoder  "无法将所给输入连接至音频解�?器。错误代�?：0x%x\n"
#define MSGTR_TVI_DS_UnableSelectVideoFormat "tvi_dshow：无法选用视频格�?。错误代�?：Error:0x%x\n"
#define MSGTR_TVI_DS_UnableSelectAudioFormat "tvi_dshow：无法选用音频格�?。错误代�?：0x%x\n"
#define MSGTR_TVI_DS_UnableGetMediaControlInterface "tvi_dshow：无法获�?� IMediaControl 接�?�。错误代�?：0x%x\n"
#define MSGTR_TVI_DS_UnableStartGraph "tvi_dshow：无法�?�动�?程图�?错误代�?：0x%x\n"
#define MSGTR_TVI_DS_DeviceNotFound "tvi_dshow：未找到设备 #%d\n"
#define MSGTR_TVI_DS_UnableGetDeviceName "tvi_dshow：无法获�?�设备 #%d 的�??称\n"
#define MSGTR_TVI_DS_UsingDevice "tvi_dshow：使用设备 #%d：%s\n"
#define MSGTR_TVI_DS_DirectGetFreqFailed "tvi_dshow：无法直接获�?�频率值。将使用�?作系统内置的频�?�表。\n"
#define MSGTR_TVI_DS_UnableExtractFreqTable "tvi_dshow：无法从 kstvtune.ax 加载频率对照表\n"
#define MSGTR_TVI_DS_WrongDeviceParam "tvi_dshow：设备�?�数错误：%s\n"
#define MSGTR_TVI_DS_WrongDeviceIndex "tvi_dshow：设备索引错误：%d\n"
#define MSGTR_TVI_DS_WrongADeviceParam "tvi_dshow：音频设备�?�数错误：%s\n"
#define MSGTR_TVI_DS_WrongADeviceIndex "tvi_dshow：音频设备索引错误：%d\n"

#define MSGTR_TVI_DS_SamplerateNotsupported "tvi_dshow：设备�?支�? %d 采样率。退回使用第一个�?�用的值。\n"
#define MSGTR_TVI_DS_VideoAdjustigNotSupported "tvi_dshow：设备�?支�?调整亮度/色度/饱和度/对比度\n"

#define MSGTR_TVI_DS_ChangingWidthHeightNotSupported "tvi_dshow：设备�?支�?改�?�视频的宽度/高度。\n"
#define MSGTR_TVI_DS_SelectingInputNotSupported  "tvi_dshow：设备�?支�?选择视频�?��?�的�?��?\n"
#define MSGTR_TVI_DS_ErrorParsingAudioFormatStruct "tvi_dshow：无法解�?音频格�?的结构。\n"
#define MSGTR_TVI_DS_ErrorParsingVideoFormatStruct "tvi_dshow：无法解�?视频格�?的结构。\n"
#define MSGTR_TVI_DS_UnableSetAudioMode "tvi_dshow：无法设置音频模�? %d。错误代�?：0x%x\n"
#define MSGTR_TVI_DS_UnsupportedMediaType "tvi_dshow：传递至 %s 的媒体格�?�?支�?\n"
#define MSGTR_TVI_DS_UnableGetsupportedVideoFormats "tvi_dshow：无法从视频端获�?��?�支�?的媒体格�?。错误代�?：0x%x\n"
#define MSGTR_TVI_DS_UnableGetsupportedAudioFormats "tvi_dshow：无法从音频端获�?��?�支�?的媒体格�?。错误代�?：0x%x �?用音频。\n"
#define MSGTR_TVI_DS_UnableFindNearestChannel "tvi_dshow：无法在系统频率对照表中找到最接近的频�?�\n"
#define MSGTR_TVI_DS_UnableToSetChannel "tvi_dshow：无法切�?�至系统频率对照表所标出的最接近的频�?�。错误代�?：0x%x\n"
#define MSGTR_TVI_DS_UnableTerminateVPPin "tvi_dshow：无法使用�?程图中的任何过滤器终止 VideoPort 端。错误代�?：0x%x\n"
#define MSGTR_TVI_DS_UnableBuildVideoSubGraph "tvi_dshow：无法建立�?��?��?程图中的视频处�?�链。错误代�?：0x%x\n"
#define MSGTR_TVI_DS_UnableBuildAudioSubGraph "tvi_dshow：无法建立�?��?��?程图中的音频处�?�链。错误代�?：0x%x\n"
#define MSGTR_TVI_DS_UnableBuildVBISubGraph "tvi_dshow：无法建立�?��?��?程图中的 VBI 链。错误代�?：0x%x\n"
#define MSGTR_TVI_DS_GraphInitFailure "tvi_dshow：Directshow �?程图�?始化失败。\n"
#define MSGTR_TVI_DS_NoVideoCaptureDevice "tvi_dshow：无法找到视频�?��?�设备\n"
#define MSGTR_TVI_DS_NoAudioCaptureDevice "tvi_dshow：无法找到音频�?��?�设备\n"
#define MSGTR_TVI_DS_GetActualMediatypeFailed "tvi_dshow：无法识别媒体的实际类型（错误代�?：0x%x）。使用与所请求相�?�的类型。\n"

// url.c
#define MSGTR_MPDEMUX_URL_StringAlreadyEscaped "字符串似乎已�?�?过 url_escape %c%c1%c2 �?��?了。\n"

// subtitles
#define MSGTR_SUBTITLES_SubRip_UnknownFontColor "SubRip：字幕�?�有未知字体颜色：%s\n"

