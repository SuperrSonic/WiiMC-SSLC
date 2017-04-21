// Translated by:  Jiri Svoboda, jiri.svoboda@seznam.cz
// Updated by:     Tomas Blaha,  tomas.blaha at kapsa.cz
//                 Jiri Heryan
// Synced with r28122


// ========================= MPlayer help ===========================

static const char help_text[]=
"Použití:          mplayer [volby] [url|cesta/]jméno_souboru\n"
"\n"
"Základní volby: (úplný seznam najdete v manuálové stránce)\n"
" -vo <rozhraní>   vybere výstupní video rozhraní (seznam: -vo help)\n"
" -ao <rozhraní>   vybere výstupní audio rozhraní (seznam: -ao help)\n"
#ifdef CONFIG_VCD
" vcd://<�?_stopy>  přehraje (S)VCD (Super Video CD) stopu (z nepřipojeného\n"
"                  zařízení)\n"
#endif
#ifdef CONFIG_DVDREAD
" dvd://<�?_tit>    přehraje DVD titul ze zařízení (mechaniky), místo ze souboru\n"
" -alang/-slang    zvolí jazyk zvuku/titulků na DVD (dvouznakový kód země)\n"
#endif
" -ss <pozice>     převine na zadanou pozici (sekundy nebo hh:mm:ss)\n"
" -nosound         přehrávání beze zvuku\n"
" -fs              celoobrazovkové přehrávání (nebo -vm -zoom, viz manuál)\n"
" -x <x> -y <y>    rozlišení obrazu (pro použití s -vm nebo -zoom)\n"
" -sub <soubor>    zvolí soubor s titulky (viz také -subfps, -subdelay)\n"
" -playlist <soubor> ur�?í soubor s playlistem\n"
" -vid x -aid y    vybere video (x) a audio (y) proud pro přehrání\n"
" -fps x -srate y  změnit video (x fps) a audio (y Hz) frekvence\n"
" -pp <kvalita>    aktivovat postprocessing (podrobnosti v manuálu)\n"
" -framedrop       povolit zahazování snímků (pro pomalé stroje)\n"
"\n"
"Základní klávesy: (úplný seznam je v manuálu, viz také input.conf)\n"
" <-  nebo  ->     převíjení vzad/vpřed o 10 sekund\n"
" dolů �?i nahoru   převíjení vzad/vpřed o  1 minutu\n"
" pgdown �?i pgup   převíjení vzad/vpřed o 10 minut\n"
" < nebo >         posun na předchozí/další soubor v playlistu\n"
" p nebo mezerník  pozastaví přehrávání (pokra�?uje po stisku jakékoliv klávesy)\n"
" q nebo ESC       konec přehrávání a ukon�?ení programu\n"
" + nebo -         upraví zpoždění zvuku v krocích +/- 0,1 sekundy\n"
" o                cyklická změna režimu OSD: nic / pozice / pozice a �?as\n"
" * nebo /         přidá nebo ubere PCM hlasitost\n"
" x nebo z         upraví zpoždění titulků v krocích +/- 0,1 sekundy\n"
" r nebo t         upraví polohu titulků nahoru/dolů, viz také -vf expand\n"
"\n"
" * * * V MAN STR�?NCE NAJDETE PODROBNOSTI, DALŠ�? VOLBY A KL�?VESY * * *\n"
"\n";

// ========================= MPlayer messages ===========================

// mplayer.c
#define MSGTR_Exiting "\nKon�?ím...\n"
#define MSGTR_ExitingHow "\nKon�?ím... (%s)\n"
#define MSGTR_Exit_quit "Konec"
#define MSGTR_Exit_eof "Konec souboru"
#define MSGTR_Exit_error "Kritická chyba"
#define MSGTR_IntBySignal "\nMPlayer přerušen signálem %d v modulu %s.\n"
#define MSGTR_NoHomeDir "Nemohu nalézt domácí adresář.\n"
#define MSGTR_GetpathProblem "Nastal problém s get_path(\"config\")\n"
#define MSGTR_CreatingCfgFile "Vytvářím konfigura�?ní soubor: %s\n"
#define MSGTR_CantLoadFont "Nemohu na�?íst bitmapový font: %s\n"
#define MSGTR_CantLoadSub "Nemohu na�?íst titulky: %s\n"
#define MSGTR_DumpSelectedStreamMissing "dump: Kritická chyba: Chybí požadovaný datový proud!\n"
#define MSGTR_CantOpenDumpfile "Nelze otevřít soubor pro dump.\n"
#define MSGTR_CoreDumped "Jádro odhozeno ;)\n"
#define MSGTR_FPSnotspecified "Údaj o FPS v hlavi�?ce souboru je špatný nebo chybí, použijte volbu -fps!\n"
#define MSGTR_TryForceAudioFmtStr "Pokouším se vynutit rodinu audiokodeku %s...\n"
#define MSGTR_CantFindAudioCodec "Nemohu nalézt kodek pro audio formát 0x%X!\n"
#define MSGTR_TryForceVideoFmtStr "Pokouším se vynutit rodinu videokodeku %s...\n"
#define MSGTR_CantFindVideoCodec "Nemohu nalézt kodek pro vybraný -vo a video formát 0x%X.\n"
#define MSGTR_CannotInitVO "Kritická chyba: Nemohu inicializovat video rozhraní!\n"
#define MSGTR_CannotInitAO "Nepodařilo se otevřít/inicializovat audio zařízení -> nebude zvuk.\n"
#define MSGTR_StartPlaying "Za�?ínám přehrávat...\n"

#define MSGTR_SystemTooSlow "\n\n"\
"         ***********************************************************\n"\
"         ****  Váš systém je příliš POMAL�? pro toto přehrávání! ****\n"\
"         ***********************************************************\n\n"\
"Možné pří�?iny, problémy a řešení:\n"\
"- Nej�?astější: špatný/chybný _zvukový_ ovlada�?!\n"\
"  - Zkuste -ao sdl nebo použijte OSS emulaci z ALSA.\n"\
"  - Pohrajte si s různými hodnotami -autosync, pro za�?átek třeba 30.\n"\
"- Pomalý obrazový výstup\n"\
"  - Zkuste jiný -vo ovlada�? (seznam: -vo help) nebo zkuste -framedrop!\n"\
"- Pomalá CPU\n"\
"  - Nezkoušejte přehrát velké DVD/DivX na pomalé CPU! Zkuste některé lavdopts,\n"\
"    jako -vfm ffmpeg -lavdopts lowres=1:fast:skiploopfilter=all.\n"\
"- Poškozený soubor.\n"\
"  - Zkuste různé kombinace voleb -nobps -ni -forceidx -mc 0.\n"\
"- Přehráváte z pomalého média (NFS/SMB, DVD, VCD, atd.)\n"\
"  - Zkuste -cache 8192.\n"\
"- Používáte -cache pro neprokládané AVI soubory?\n"\
"  - Zkuste -nocache.\n"\
"Tipy na vyladění a zrychlení najdete v DOCS/HTML/en/devices.html.\n"\
"Pokud nic z toho nepomůže, pře�?těte si DOCS/HTML/en/bugreports.html.\n\n"

#define MSGTR_NoGui "MPlayer byl přeložen BEZ podpory GUI.\n"
#define MSGTR_GuiNeedsX "GUI MPlayeru vyžaduje X11.\n"
#define MSGTR_Playing "\nPřehrávám %s\n"
#define MSGTR_NoSound "Audio: žádný zvuk\n"
#define MSGTR_FPSforced "FPS vynuceno na hodnotu %5.3f  (vyn. �?as: %5.3f)\n"
#define MSGTR_AvailableVideoOutputDrivers "Dostupná video rozhraní:\n"
#define MSGTR_AvailableAudioOutputDrivers "Dostupná audio rozhraní:\n"
#define MSGTR_AvailableAudioCodecs "Dostupné audio kodeky:\n"
#define MSGTR_AvailableVideoCodecs "Dostupné video kodeky:\n"
#define MSGTR_AvailableAudioFm "Dostupné (zakompilované) rodiny audio kodeků/ovlada�?ů:\n"
#define MSGTR_AvailableVideoFm "Dostupné (zakompilované) rodiny video kodeků/ovlada�?ů:\n"
#define MSGTR_AvailableFsType "Dostupné režimy změny hladiny při celoobrazovkovém zobrazení:\n"
#define MSGTR_CannotReadVideoProperties "Video: Nelze pře�?íst vlastnosti.\n"
#define MSGTR_NoStreamFound "Nenalezen žádný datový proud.\n"
#define MSGTR_ErrorInitializingVODevice "Chyba při otevírání/inicializaci vybraného video_out (-vo) zařízení.\n"
#define MSGTR_ForcedVideoCodec "Vynucen video kodek: %s\n"
#define MSGTR_ForcedAudioCodec "Vynucen audio kodek: %s\n"
#define MSGTR_Video_NoVideo "Video: Žádné video\n"
#define MSGTR_NotInitializeVOPorVO "\nKritická chyba: Nemohu inicializovat video filtry (-vf) nebo video výstup (-vo)!\n"
#define MSGTR_Paused "===== POZASTAVENO ====="
#define MSGTR_PlaylistLoadUnable "\nNemohu na�?íst playlist %s.\n"
#define MSGTR_Exit_SIGILL_RTCpuSel \
"- MPlayer havaroval kvůli 'Illegal Instruction'.\n"\
"  To může být chyba v kódu pro rozpoznání CPU za běhu...\n"\
"  Prosím, pře�?těte si DOCS/HTML/en/bugreports.html.\n"
#define MSGTR_Exit_SIGILL \
"- MPlayer havaroval kvůli 'Illegal Instruction'.\n"\
"  To se obvykle stává, když se ho pokusíte spustit na CPU odlišném, než pro který\n"\
"  byl přeložen/optimalizován.\n  Ověřte si to!\n"
#define MSGTR_Exit_SIGSEGV_SIGFPE \
"- MPlayer havaroval kvůli špatnému použití CPU/FPU/RAM.\n"\
"  Přeložte MPlayer s volbou --enable-debug , prove�?te 'gdb' backtrace\n"\
"  a disassembly. Detaily najdete v DOCS/HTML/en/bugreports_what.html#bugreports_crash.\n"
#define MSGTR_Exit_SIGCRASH \
"- MPlayer havaroval. To by se nemělo stát.\n"\
"  Může to být chyba v kódu MPlayeru _nebo_ ve vašich ovlada�?ích _nebo_ ve verzi\n"\
"  vašeho gcc. Pokud si myslíte, že je to chyba MPlayeru, pře�?těte si, prosím,\n"\
"  DOCS/HTML/en/bugreports.html a pokra�?ujte podle tam uvedeného návodu. My vám nemůžeme\n"\
"  pomoci, pokud tyto informace neuvedete při ohlašování možné chyby.\n"
#define MSGTR_LoadingConfig "Na�?ítám konfiguraci '%s'\n"
#define MSGTR_LoadingProtocolProfile "Na�?ítám profil protokolu '%s'\n"
#define MSGTR_LoadingExtensionProfile "Na�?ítám profil rozšíření '%s'\n"
#define MSGTR_AddedSubtitleFile "SUB: Přidán soubor s titulky (%d): %s\n"
#define MSGTR_RemovedSubtitleFile "SUB: Odebrán soubor s titulky (%d): %s\n"
#define MSGTR_ErrorOpeningOutputFile "Chyba při otevírání souboru [%s] pro zápis!\n"
#define MSGTR_RTCDeviceNotOpenable "Selhalo otevření %s: %s (by mělo být �?itelné uživatelem.)\n"
#define MSGTR_LinuxRTCInitErrorIrqpSet "Chyba inicializace Linuxových RTC v ioctl (rtc_irqp_set %lu): %s\n"
#define MSGTR_IncreaseRTCMaxUserFreq "Zkuste přidat \"echo %lu > /proc/sys/dev/rtc/max-user-freq\" do startovacích\n skriptů vašeho systému.\n"
#define MSGTR_LinuxRTCInitErrorPieOn "Chyba inicializace Linuxových RTC v ioctl (rtc_pie_on): %s\n"
#define MSGTR_UsingTimingType "Používám %s �?asování.\n"
#define MSGTR_Getch2InitializedTwice "VAROV�?N�?: getch2_init volána dvakrát!\n"
#define MSGTR_DumpstreamFdUnavailable "Nemohu uložit (dump) tento proud - žádný deskriptor souboru není dostupný.\n"
#define MSGTR_CantOpenLibmenuFilterWithThisRootMenu "Nemohu otevřít video filtr libmenu s kořenovým menu %s.\n"
#define MSGTR_AudioFilterChainPreinitError "Chyba při předinicializaci řetězce audio filtrů!\n"
#define MSGTR_LinuxRTCReadError "Chyba při �?tení z Linuxových RTC: %s\n"
#define MSGTR_SoftsleepUnderflow "Varování! Podte�?ení softsleep!\n"
#define MSGTR_DvdnavNullEvent "Nedefinovaná DVDNAV událost?!\n"
#define MSGTR_DvdnavHighlightEventBroken "DVDNAV událost: Vadné zvýrazňování událostí\n"
#define MSGTR_DvdnavEvent "DVDNAV událost: %s\n"
#define MSGTR_DvdnavHighlightHide "DVDNAV událost: Highlight Hide\n"
#define MSGTR_DvdnavStillFrame "######################################## DVDNAV událost: Stojící snímek: %d sek.\n"
#define MSGTR_DvdnavNavStop "DVDNAV událost: Nav Stop\n"
#define MSGTR_DvdnavNavNOP "DVDNAV událost: Nav NOP\n"
#define MSGTR_DvdnavNavSpuStreamChangeVerbose "DVDNAV událost: Nav Změna SPU proudu: fyz: %d/%d/%d logický: %d\n"
#define MSGTR_DvdnavNavSpuStreamChange "DVDNAV událost: Nav Změna SPU proudu: fyz: %d logický: %d\n"
#define MSGTR_DvdnavNavAudioStreamChange "DVDNAV událost: Nav Změna audio proudu: fyz: %d logický: %d\n"
#define MSGTR_DvdnavNavVTSChange "DVDNAV událost: Nav Změna VTS\n"
#define MSGTR_DvdnavNavCellChange "DVDNAV událost: Nav Cell Change\n"
#define MSGTR_DvdnavNavSpuClutChange "DVDNAV událost: Nav Změna SPU CLUT\n"
#define MSGTR_DvdnavNavSeekDone "DVDNAV událost: Nav Převíjení Dokon�?eno\n"
#define MSGTR_MenuCall "Volání menu\n"

// --- edit decision lists (seznamy edita�?ních zásahů)
#define MSGTR_EdlOutOfMem "Nelze alokovat dostatek paměti pro vložení EDL dat.\n"
#define MSGTR_EdlRecordsNo "Na�?ítám %d EDL akcí.\n"
#define MSGTR_EdlQueueEmpty "Veškeré EDL akce již byly provedeny.\n"
#define MSGTR_EdlCantOpenForWrite "Nelze otevřít EDL soubor [%s] pro zápis.\n"
#define MSGTR_EdlCantOpenForRead "Nelze otevřít EDL soubor [%s] pro �?tení.\n"
#define MSGTR_EdlNOsh_video "EDL nelze použít bez videa, vypínám.\n"
#define MSGTR_EdlNOValidLine "Chybná EDL na řádku: %s\n"
#define MSGTR_EdlBadlyFormattedLine "Špatně formátovaná EDL na řádku [%d], zahazuji.\n"
#define MSGTR_EdlBadLineOverlap "Poslední stop zna�?ka byla [%f]; další start je [%f].\n"\
"Vstupy musí být v chronologickém pořadí a nesmí se překrývat. Zahazuji.\n"
#define MSGTR_EdlBadLineBadStop "Časová zna�?ka stop má být za zna�?kou start.\n"
#define MSGTR_EdloutBadStop "EDL: Vynechání zrušeno, poslední start > stop\n"
#define MSGTR_EdloutStartSkip "EDL: Za�?átek vynechaného bloku, stiskněte znovu 'i' pro ukon�?ení bloku.\n"
#define MSGTR_EdloutEndSkip "EDL: Konec vynechaného bloku, řádek zapsán.\n"

// mplayer.c OSD
#define MSGTR_OSDenabled "zapnuto"
#define MSGTR_OSDdisabled "vypnuto"
#define MSGTR_OSDAudio "Zvuk: %s"
#define MSGTR_OSDVideo "Video: %s"
#define MSGTR_OSDChannel "Kanál: %s"
#define MSGTR_OSDSubDelay "Zpoždění tit: %d ms"
#define MSGTR_OSDSpeed "Rychlost: x %6.2f"
#define MSGTR_OSDosd "OSD: %s"
#define MSGTR_OSDChapter "Kapitola: (%d) %s"
#define MSGTR_OSDAngle "Úhel: %d/%d"

// property values
#define MSGTR_Enabled "zapnuto"
#define MSGTR_EnabledEdl "zapnuto (EDL)"
#define MSGTR_Disabled "vypnuto"
#define MSGTR_HardFrameDrop "intenzivní"
#define MSGTR_Unknown "neznámé"
#define MSGTR_Bottom "dolů"
#define MSGTR_Center "na střed"
#define MSGTR_Top "nahoru"
#define MSGTR_SubSourceFile "soubor"
#define MSGTR_SubSourceVobsub "vobsub"
#define MSGTR_SubSourceDemux "vestavěné"

// OSD bar names
#define MSGTR_Volume "Hlasitost"
#define MSGTR_Panscan "Panscan"
#define MSGTR_Gamma "Gama"
#define MSGTR_Brightness "Jas"
#define MSGTR_Contrast "Kontrast"
#define MSGTR_Saturation "Sytost"
#define MSGTR_Hue "Odstín"
#define MSGTR_Balance "Stereováha"

// property state
#define MSGTR_LoopStatus "Loop: %s"
#define MSGTR_MuteStatus "Ztišení: %s"
#define MSGTR_AVDelayStatus "A-V odchylka: %s"
#define MSGTR_OnTopStatus "Zůstat navrchu: %s"
#define MSGTR_RootwinStatus "Kořenové okno: %s"
#define MSGTR_BorderStatus "Ráme�?ek: %s"
#define MSGTR_FramedroppingStatus "Zahazování snímků: %s"
#define MSGTR_VSyncStatus "Vertikální synchronizace: %s"
#define MSGTR_SubSelectStatus "Titulky: %s"
#define MSGTR_SubSourceStatus "Zdroj titulků: %s"
#define MSGTR_SubPosStatus "Umístění titulků: %s/100"
#define MSGTR_SubAlignStatus "Zarovnání titulků: %s"
#define MSGTR_SubDelayStatus "Zpoždění titulků: %s"
#define MSGTR_SubScale "Zvětšení titulků: %s"
#define MSGTR_SubVisibleStatus "Titulky: %s"
#define MSGTR_SubForcedOnlyStatus "Pouze vynucené titulky: %s"

// mencoder.c
#define MSGTR_UsingPass3ControlFile "Řídicí soubor pro tříprůchodový režim: %s\n"
#define MSGTR_MissingFilename "\nChybí jméno souboru.\n\n"
#define MSGTR_CannotOpenFile_Device "Nelze otevřít soubor/zařízení.\n"
#define MSGTR_CannotOpenDemuxer "Nelze otevřít demuxer.\n"
#define MSGTR_NoAudioEncoderSelected "\nNebyl vybrán audio enkodér (-oac). Nějaký vyberte (viz -oac help) nebo použijte -nosound.\n"
#define MSGTR_NoVideoEncoderSelected "\nNebyl vybrán video enkodér (-ovc). Nějaký vyberte (viz  -ovc help).\n"
#define MSGTR_CannotOpenOutputFile "Nelze otevřít výstupní soubor '%s'\n"
#define MSGTR_EncoderOpenFailed "Selhalo spuštění enkodéru\n"
#define MSGTR_MencoderWrongFormatAVI "\nVAROV�?N�?: FORM�?T V�?STUPN�?HO SOUBORU JE _AVI_. Viz -of help.\n"
#define MSGTR_MencoderWrongFormatMPG "\nVAROV�?N�?: FORM�?T V�?STUPN�?HO SOUBORU JE _MPEG_. Viz -of help.\n"
#define MSGTR_MissingOutputFilename "Nebyl nastaven výstupní soubor, prostudujte si volbu -o."
#define MSGTR_ForcingOutputFourcc "Vynucuji výstupní FourCC na %x [%.4s].\n"
#define MSGTR_ForcingOutputAudiofmtTag "Vynucuji zna�?ku výstupního zvukového formátu 0x%x\n"
#define MSGTR_DuplicateFrames "\n%d opakujících se snímků!\n"
#define MSGTR_SkipFrame "\nPřeskakuji snímek!\n"
#define MSGTR_ResolutionDoesntMatch "\nNový video soubor má jiné rozlišení nebo barevný prostor než jeho předchůdce.\n"
#define MSGTR_FrameCopyFileMismatch "\nVšechny video soubory musí mít shodné fps, rozlišení a kodek pro -ovc copy.\n"
#define MSGTR_AudioCopyFileMismatch "\nVšechny soubory musí používat identický audio kodek a formát pro -oac copy.\n"
#define MSGTR_NoAudioFileMismatch "\nNelze kombinovat neozvu�?ené video soubory s ozvu�?enými. Zkuste -nosound.\n"
#define MSGTR_NoSpeedWithFrameCopy "VAROV�?N�?: volba -speed nemá zaru�?enou správnou funk�?nost spolu s -oac copy!\n"\
"Výsledný film může být vadný!\n"
#define MSGTR_ErrorWritingFile "%s: chyba při zápisu souboru.\n"
#define MSGTR_FlushingVideoFrames "\nVylévám video snímky.\n"
#define MSGTR_FiltersHaveNotBeenConfiguredEmptyFile "Filtry nebyly nakonfiguovány! Prázdný soubor?\n"
#define MSGTR_RecommendedVideoBitrate "Doporu�?ený datový tok videa pro CD %s: %d\n"
#define MSGTR_VideoStreamResult "\nVideo proud: %8.3f kbit/s  (%d B/s)  velikost: %"PRIu64" bajtů  %5.3f sekund  %d snímků\n"
#define MSGTR_AudioStreamResult "\nAudio proud: %8.3f kbit/s  (%d B/s)  velikost: %"PRIu64" bajtů  %5.3f sekund\n"
#define MSGTR_EdlSkipStartEndCurrent "EDL SKIP: Start: %.2f  Konec: %.2f   Sou�?asná: V: %.2f  A: %.2f     \r"
#define MSGTR_OpenedStream "úspěch: formát: %d  data: 0x%X - 0x%x\n"
#define MSGTR_VCodecFramecopy "videokodek: framecopy (%dx%d %dbpp fourcc=%x)\n"
#define MSGTR_ACodecFramecopy "audiokodek: framecopy (formát=%x kanálů=%d frekvence=%d bitů=%d B/s=%d vzorek-%d)\n"
#define MSGTR_CBRPCMAudioSelected "Vybrán CBR PCM zvuk.\n"
#define MSGTR_MP3AudioSelected "Vybrán MP3 zvuk.\n"
#define MSGTR_CannotAllocateBytes "Nelze alokovat %d bajtů.\n"
#define MSGTR_SettingAudioDelay "Nastavuji zpoždění zvuku na %5.3fs.\n"
#define MSGTR_SettingVideoDelay "Nastavuji zpoždění videa na %5.3fs.\n"
#define MSGTR_LimitingAudioPreload "Omezuji předna�?ítání zvuku na 0.4s.\n"
#define MSGTR_IncreasingAudioDensity "Zvyšuji hustotu audia na 4.\n"
#define MSGTR_ZeroingAudioPreloadAndMaxPtsCorrection "Vynucuji předna�?ítání zvuku na 0, max korekci pts  na 0.\n"
#define MSGTR_LameVersion "LAME ve verzi %s (%s)\n\n"
#define MSGTR_InvalidBitrateForLamePreset "Chyba: Specifikovaný datový tok je mimo rozsah pro tento preset režim.\n"\
"\n"\
"Pokud používáte tento režim, musíte zadat hodnotu od \"8\" do \"320\".\n"\
"\n"\
"Další informace viz: \"-lameopts preset=help\"\n"
#define MSGTR_InvalidLamePresetOptions "Chyba: Nezadali jste platný profil a/nebo volby s preset režimem.\n"\
"\n"\
"Dostupné profily jsou:\n"\
"\n"\
"   <fast>        standard\n"\
"   <fast>        extreme\n"\
"                 insane\n"\
"   <cbr> (ABR Mode) - Implikuje režim ABR. Pro jeho použití,\n"\
"                      jednoduše zadejte datový tok. Například:\n"\
"                      \"preset=185\" aktivuje tento režim\n"\
"                      a použije průměrný datový tok 185 kbps.\n"\
"\n"\
"    Několik příkladů:\n"\
"\n"\
"    \"-lameopts fast:preset=standard  \"\n"\
" or \"-lameopts  cbr:preset=192       \"\n"\
" or \"-lameopts      preset=172       \"\n"\
" or \"-lameopts      preset=extreme   \"\n"\
"\n"\
"Další informace viz: \"-lameopts preset=help\"\n"
#define MSGTR_LamePresetsLongInfo "\n"\
"Preset režimy jsou navrženy tak, aby poskytovaly co nejvyšší možnou kvalitu.\n"\
"\n"\
"Většina z nich byla testována a vyladěna pomocí zevrubných zdvojených slepých\n"\
"poslechových testů, za ú�?elem dosažení a ověření této kvality.\n"\
"\n"\
"Nastavení jsou neustále aktualizována v souladu s nejnovějším vývojem\n"\
"a měla by poskytovat prakticky nejvyšší možnou kvalitu, jaká je v sou�?asnosti \n"\
"s kodekem LAME dosažitelná.\n"\
"\n"\
"Aktivace preset režimů:\n"\
"\n"\
"   Pro režimy VBR (všeobecně nejvyšší kvalita):\n"\
"\n"\
"     \"preset=standard\" Tento režim by měl být jasnou volbou\n"\
"                             pro většinu lidí a hudebních žánrů a má\n"\
"                             již vysokou kvalitu.\n"\
"\n"\
"     \"preset=extreme\" Pokud máte výjime�?ně dobrý sluch a odpovídající\n"\
"                             vybavení, tento režim obecně poskytuje\n"\
"                             mírně vyšší kvalitu než režim \"standard\".\n"\
"\n"\
"   Pro CBR 320kbps (nejvyšší možná kvalita ze všech preset režimů):\n"\
"\n"\
"     \"preset=insane\"  Tento režim je pro většinu lidí a situací\n"\
"                             předimenzovaný, ale pokud vyžadujete\n"\
"                             absolutně nejvyšší kvalitu bez ohledu na\n"\
"                             velikost souboru, je toto vaše volba.\n"\
"\n"\
"   Pro režimy ABR (vysoká kvalita při daném datovém toku, ale ne jako VBR):\n"\
"\n"\
"     \"preset=<kbps>\"  Použitím tohoto režimu obvykle dosáhnete dobré\n"\
"                             kvality při daném datovém toku. V závislosti\n"\
"                             na zadaném toku tento preset odvodí optimální\n"\
"                             nastavení pro danou situaci.\n"\
"                             A�?koli tento přístup funguje, není ani zdaleka\n"\
"                             tak flexibilní jako VBR, a obvykle nedosahuje\n"\
"                             stejné úrovně kvality jako VBR na vyšších dato-\n"\
"                             vých tocích.\n"\
"\n"\
"Pro odpovídající profily jsou také dostupné následující volby:\n"\
"\n"\
"   <fast>        standard\n"\
"   <fast>        extreme\n"\
"                 insane\n"\
"   <cbr> (ABR režim) - Implikuje režim ABR. Pro jeho použití,\n"\
"                      jednoduše zadejte datový tok. Například:\n"\
"                      \"preset=185\" aktivuje tento režim\n"\
"                      a použije průměrný datový tok 185 kbps.\n"\
"\n"\
"   \"fast\" - V daném profilu aktivuje novou rychlou VBR kompresi.\n"\
"            Nevýhodou je obvykle mírně vyšší datový tok než v normálním\n"\
"            režimu a také může dojít k mírnému poklesu kvality.\n"\
"   Varování:v sou�?asné verzi může nastavení \"fast\" vést k příliš\n"\
"            vysokému datovému toku ve srovnání s normálním nastavením.\n"\
"\n"\
"   \"cbr\"  - Pokud použijete režim ABR (viz výše) s významným\n"\
"            datovým tokem, např. 80, 96, 112, 128, 160, 192, 224, 256, 320,\n"\
"            můžete použít volbu \"cbr\" k vynucení kódování v režimu CBR\n"\
"            (konstantní tok) namísto standardního ABR režimu. ABR poskytuje\n"\
"            lepší kvalitu, ale CBR může být užite�?ný v situacích jako je\n"\
"            vysílání MP3 proudu po internetu.\n"\
"\n"\
"    Například:\n"\
"\n"\
"      \"-lameopts fast:preset=standard  \"\n"\
" nebo \"-lameopts  cbr:preset=192       \"\n"\
" nebo \"-lameopts      preset=172       \"\n"\
" nebo \"-lameopts      preset=extreme   \"\n"\
"\n"\
"\n"\
"Pro ABR režim je k dispozici několik zkratek:\n"\
"phone => 16kbps/mono        phon+/lw/mw-eu/sw => 24kbps/mono\n"\
"mw-us => 40kbps/mono        voice => 56kbps/mono\n"\
"fm/radio/tape => 112kbps    hifi => 160kbps\n"\
"cd => 192kbps               studio => 256kbps"
#define MSGTR_LameCantInit \
"Nelze nastavit volby pro LAME, ověřte datový_tok/vzorkovou_rychlost. Některé"\
"velmi nízké datové toky (<32) vyžadují nižší vzorkovou rychlost (např. -srate 8000).\n"\
"Pokud vše selže, zkuste preset."
#define MSGTR_ConfigFileError "chyba konfigura�?ního souboru"
#define MSGTR_ErrorParsingCommandLine "chyba při zpracovávání příkazového řádku"
#define MSGTR_VideoStreamRequired "Videoproud je povinný!\n"
#define MSGTR_ForcingInputFPS "Vstupní fps bude interpretováno jako %5.3f\n"
#define MSGTR_RawvideoDoesNotSupportAudio "Výstupní formát souboru RAWVIDEO nepodporuje zvuk - vypínám ho.\n"
#define MSGTR_DemuxerDoesntSupportNosound "Tento demuxer zatím nepodporuje -nosound.\n"
#define MSGTR_MemAllocFailed "Alokace paměti selhala.\n"
#define MSGTR_NoMatchingFilter "Nemohu najít odpovídající filtr/ao formát!\n"
#define MSGTR_MP3WaveFormatSizeNot30 "sizeof(MPEGLAYER3WAVEFORMAT)==%d!=30, možná je vadný překlada�? C?\n"
#define MSGTR_NoLavcAudioCodecName "Audio LAVC, chybí jméno kodeku!\n"
#define MSGTR_LavcAudioCodecNotFound "Audio LAVC, nemohu najít enkodér pro kodek %s.\n"
#define MSGTR_CouldntAllocateLavcContext "Audio LAVC, nemohu alokovat kontext!\n"
#define MSGTR_CouldntOpenCodec "Nelze otevřít kodek %s, br=%d.\n"
#define MSGTR_CantCopyAudioFormat "Audio formát 0x%x je nekompatibilní s '-oac copy', zkuste prosím '-oac pcm',\n nebo použijte '-fafmttag' pro jeho přepsání.\n"

// cfg-mencoder.h
#define MSGTR_MEncoderMP3LameHelp "\n\n"\
" vbr=<0-4>     metoda proměnného datového toku\n"\
"                0: cbr  (konstantní tok)\n"\
"                1: mt   (VBR algoritmus Mark Taylor)\n"\
"                2: rh   (VBR algoritmus Robert Hegemann - výchozí)\n"\
"                3: abr  (průměrný tok)\n"\
"                4: mtrh (VBR alogoritmus Mark Taylor Robert Hegemann)\n"\
"\n"\
" abr           průměrný datový tok\n"\
"\n"\
" cbr           konstantní datový tok\n"\
"               Vynutí také metodu CBR pro následné ABR preset režimy\n"\
"\n"\
" br=<0-1024>   ur�?ení datového toku v kBit (pouze CBR a ABR)\n"\
"\n"\
" q=<0-9>       kvalita (0-nejvyšší, 9-nejnižší) (pouze pro VBR)\n"\
"\n"\
" aq=<0-9>      kvalita algoritmu (0-nejlepší/nejpomalejší, 9-nejhorší/nejrychlejší)\n"\
"\n"\
" ratio=<1-100> kompresní poměr\n"\
"\n"\
" vol=<0-10>    zesílení zvuku\n"\
"\n"\
" mode=<0-3>    (výchozí: auto)\n"\
"                0: stereo\n"\
"                1: joint-stereo\n"\
"                2: dualchannel\n"\
"                3: mono\n"\
"\n"\
" padding=<0-2>\n"\
"                0: ne\n"\
"                1: vše\n"\
"                2: upravit\n"\
"\n"\
" fast          Zapíná rychlejší enkódování pro následné VBR preset režimy,\n"\
"               poskytuje o něco nižší kvalitu a vyšší datový tok.\n"\
"\n"\
" preset=<hodnota> Přednastavené profily poskytující maximální kvalitu.\n"\
"                  medium: enkódování metodou VBR, dobrá kvalita\n"\
"                   (datový tok 150-180 kbps)\n"\
"                  standard: enkódování metodou VBR, vysoká kvalita\n"\
"                   (datový tok 170-210 kbps)\n"\
"                  extreme: enkódování metodou VBR, velmi vysoká kvalita\n"\
"                   (datový tok 200-240 kbps)\n"\
"                  insane: enkódování metodou CBR, nejvyšší preset kvalita\n"\
"                   (datový tok 320 kbps)\n"\
"                  <8-320>: hodnota průměrného datového toku pro metodu ABR.\n\n"

//codec-cfg.c
#define MSGTR_DuplicateFourcc "zdvojené FourCC"
#define MSGTR_TooManyFourccs "příliš mnoho FourCC/formátů..."
#define MSGTR_ParseError "chyba interpretace (parse)"
#define MSGTR_ParseErrorFIDNotNumber "chyba interpretace (ID formátu, nikoli �?íslo?)"
#define MSGTR_ParseErrorFIDAliasNotNumber "chyba interpretace (alias ID formátu, nikoli �?íslo?)"
#define MSGTR_DuplicateFID "zdvojené ID formátu"
#define MSGTR_TooManyOut "příliš mnoho výstupu..."
#define MSGTR_InvalidCodecName "\njméno kodeku(%s) není platné!\n"
#define MSGTR_CodecLacksFourcc "\nkodek(%s) nemá FourCC/formát!\n"
#define MSGTR_CodecLacksDriver "\nkodek(%s) nemá driver!\n"
#define MSGTR_CodecNeedsDLL "\nkodek(%s) vyžaduje 'dll'!\n"
#define MSGTR_CodecNeedsOutfmt "\nkodek(%s) vyžaduje 'outfmt'!\n"
#define MSGTR_CantAllocateComment "Nelze alokovat paměť pro komentář. "
#define MSGTR_GetTokenMaxNotLessThanMAX_NR_TOKEN "get_token(): max >= MAX_MR_TOKEN!"
#define MSGTR_CantGetMemoryForLine "Nemám paměť pro 'line': %s\n"
#define MSGTR_CantReallocCodecsp "Nelze realokovat '*codecsp': %s\n"
#define MSGTR_CodecNameNotUnique "Jméno kodeku '%s' není jedine�?né."
#define MSGTR_CantStrdupName "Nelze provést strdup -> 'name': %s\n"
#define MSGTR_CantStrdupInfo "Nelze provést strdup -> 'info': %s\n"
#define MSGTR_CantStrdupDriver "Nelze provést strdup -> 'driver': %s\n"
#define MSGTR_CantStrdupDLL "Nelze provést strdup -> 'dll': %s"
#define MSGTR_AudioVideoCodecTotals "%d audio & %d video kodeků\n"
#define MSGTR_CodecDefinitionIncorrect "Kodek není správně definován."
#define MSGTR_OutdatedCodecsConf "Tento codecs.conf je příliš starý a nekompatibilní s tímto sestavením  MPlayeru!"

// fifo.c
#define MSGTR_CannotMakePipe "Nelze vytvořit ROURU!\n"

// parser-mecmd.c, parser-mpcmd.c
#define MSGTR_NoFileGivenOnCommandLine "'--' ozna�?uje konec voleb, ale nebyl zadán žádný název souboru na příkazovém řádku.\n"
#define MSGTR_TheLoopOptionMustBeAnInteger "Hodnota volby loop musí být selé �?íslo: %s\n"
#define MSGTR_UnknownOptionOnCommandLine "Neznámá volba na příkazovém řádku: -%s\n"
#define MSGTR_ErrorParsingOptionOnCommandLine "Chyba při zpracování volby na příkazovém řádku: -%s\n"
#define MSGTR_InvalidPlayEntry "Neplatná položka pro přehrávání %s\n"
#define MSGTR_NotAnMEncoderOption "-%s není volbou MEncoderu\n"
#define MSGTR_NoFileGiven "Nezadán soubor\n"

// m_config.c
#define MSGTR_SaveSlotTooOld "Nalezený save slot z lvl %d je příliš starý: %d !!!\n"
#define MSGTR_InvalidCfgfileOption "Volbu %s nelze použít v konfigura�?ním souboru\n"
#define MSGTR_InvalidCmdlineOption "Volbu %s nelze použít z příkazového řádku\n"
#define MSGTR_InvalidSuboption "Chyba: volba '%s' nemá žádnou podvolbu '%s'\n"
#define MSGTR_MissingSuboptionParameter "Chyba: podvolba '%s' volby '%s' musí mít parametr!\n"
#define MSGTR_MissingOptionParameter "Chyba: volba '%s' musí mít parametr!\n"
#define MSGTR_OptionListHeader "\n Název                Typ             Min        Max      Globál  CL    Konfig\n\n"
#define MSGTR_TotalOptions "\nCelkem: %d voleb\n"
#define MSGTR_ProfileInclusionTooDeep "VAROV�?N�?: Příliš hluboké vnořování profilů.\n"
#define MSGTR_NoProfileDefined "Žádný profil nebyl definován.\n"
#define MSGTR_AvailableProfiles "Dostupné profily:\n"
#define MSGTR_UnknownProfile "Neznámý profil '%s'.\n"
#define MSGTR_Profile "Profil %s: %s\n"

// m_property.c
#define MSGTR_PropertyListHeader "\n Název                Typ             Min        Max\n\n"
#define MSGTR_TotalProperties "\nCelkem: %d nastavení\n"

// loader/ldt_keeper.c
#define MSGTR_LOADER_DYLD_Warning "VAROV�?N�?: Pokouším se použít DLL kodeky, ale proměnná prostředí\n        DYLD_BIND_AT_LAUNCH není nastavena. Nejspíš havaruji.\n"


// ====================== GUI messages/buttons ========================

// --- labels ---
#define MSGTR_About "O aplikaci"
#define MSGTR_FileSelect "Vybrat soubor..."
#define MSGTR_SubtitleSelect "Vybrat titulky..."
#define MSGTR_OtherSelect "Vybrat..."
#define MSGTR_AudioFileSelect "Vybrat externí zvukový kanál..."
#define MSGTR_FontSelect "Vybrat font..."
// Poznámka: Pokud změníte MSGTR_PlayList, ujistěte se prosím, že vyhovuje i pro  MSGTR_MENU_PlayList
#define MSGTR_PlayList "Playlist"
#define MSGTR_Equalizer "Ekvalizér"
#define MSGTR_ConfigureEqualizer "Konfigurace ekvalizéru"
#define MSGTR_SkinBrowser "Prohlíže�? témat"
#define MSGTR_Network "Síťové vysílání..."
// Poznámka: Pokud změníte MSGTR_Preferences, ujistěte se prosím, že vyhovuje i pro  MSGTR_MENU_Preferences
#define MSGTR_Preferences "Nastavení" // Předvolby?
#define MSGTR_AudioPreferences "Konfigurace ovlada�?e zvuku"
#define MSGTR_NoMediaOpened "Nic není otevřeno."
#define MSGTR_NoChapter "Žádná kapitola" //bez kapitoly?
#define MSGTR_Chapter "Kapitola %d"
#define MSGTR_NoFileLoaded "Není na�?ten žádný soubor."

// --- buttons ---
#define MSGTR_Ok "OK"
#define MSGTR_Cancel "Zrušit"
#define MSGTR_Add "Přidat"
#define MSGTR_Remove "Odebrat"
#define MSGTR_Clear "Vynulovat"
#define MSGTR_Config "Konfigurace"
#define MSGTR_ConfigDriver "Konfigurovat ovlada�?"
#define MSGTR_Browse "Prohlížet"

// --- error messages ---
#define MSGTR_NEMDB "Bohužel není dostatek paměti pro vykreslovací mezipaměť."
#define MSGTR_NEMFMR "Bohužel není dostatek paměti pro vykreslení menu."
#define MSGTR_IDFGCVD "Bohužel nebyl nalezen video ovlada�? kompatibilní s GUI."
#define MSGTR_NEEDLAVC "Bohužel nelze přehrávat ne-MPEG s kartou DXR3/H+ bez přeenkódování.\nProsím, zapněte lavc v konfiguraci DXR3/H+."

// --- skin loader error messages
#define MSGTR_SKIN_ERRORMESSAGE "[témata] chyba v konfigura�?ním souboru témat na řádce %d: %s"
#define MSGTR_SKIN_SkinFileNotFound "[témata] soubor ( %s ) nenalezen.\n"
#define MSGTR_SKIN_SkinFileNotReadable "[témata] soubor ( %s ) nelze pře�?íst.\n"
#define MSGTR_SKIN_BITMAP_16bit  "Bitmapy s hloubkou 16 bitů a méně nejsou podporovány (%s).\n"
#define MSGTR_SKIN_BITMAP_FileNotFound  "Soubor nenalezen (%s)\n"
#define MSGTR_SKIN_BITMAP_PNGReadError "chyba �?tení PNG (%s)\n"
#define MSGTR_SKIN_BITMAP_ConversionError "chyba konverze z 24 do 32 bitů (%s)\n"
#define MSGTR_SKIN_UnknownMessage "neznámá zpráva: %s\n"
#define MSGTR_SKIN_NotEnoughMemory "nedostatek paměti\n"
#define MSGTR_SKIN_FONT_TooManyFontsDeclared "deklarováno příliš mnoho fontů\n"
#define MSGTR_SKIN_FONT_FontFileNotFound "soubor fontu nebyl nalezen\n"
#define MSGTR_SKIN_FONT_FontImageNotFound "soubor obrazu fontu nebyl nalezen\n"
#define MSGTR_SKIN_FONT_NonExistentFont "neexistující identifikátor fontu (%s)\n"
#define MSGTR_SKIN_UnknownParameter "neznámý parametr (%s)\n"
#define MSGTR_SKIN_SKINCFG_SkinNotFound "Téma nenalezeno (%s).\n"
#define MSGTR_SKIN_SKINCFG_SelectedSkinNotFound "Vybraný skin ( %s ) nenalezen, zkouším 'výchozí'...\n"
#define MSGTR_SKIN_LABEL "Témata:"

// --- GTK menus
#define MSGTR_MENU_AboutMPlayer "O aplikaci MPlayer"
#define MSGTR_MENU_Open "Otevřít..."
#define MSGTR_MENU_PlayFile "Přehrát soubor..."
#define MSGTR_MENU_PlayVCD "Přehrát VCD..."
#define MSGTR_MENU_PlayDVD "Přehrát DVD..."
#define MSGTR_MENU_PlayURL "Přehrát z URL..."
#define MSGTR_MENU_LoadSubtitle "Na�?íst titulky..."
#define MSGTR_MENU_DropSubtitle "Zahodit titulky..."
#define MSGTR_MENU_LoadExternAudioFile "Na�?íst externí soubor se zvukem..."
#define MSGTR_MENU_Playing "Ovládání přehrávání"
#define MSGTR_MENU_Play "Přehrát"
#define MSGTR_MENU_Pause "Pozastavit"
#define MSGTR_MENU_Stop "Stop"
#define MSGTR_MENU_NextStream "Další proud"
#define MSGTR_MENU_PrevStream "Předchozí proud"
#define MSGTR_MENU_Size "Velikost"
#define MSGTR_MENU_HalfSize   "Polovi�?ní velikost"
#define MSGTR_MENU_NormalSize "Normální velikost"
#define MSGTR_MENU_DoubleSize "Dvojnásobná velikost"
#define MSGTR_MENU_FullScreen "Celá obrazovka"
#define MSGTR_MENU_DVD "DVD"
#define MSGTR_MENU_VCD "VCD"
#define MSGTR_MENU_PlayDisc "Přehrát disk..."
#define MSGTR_MENU_ShowDVDMenu "Zobrazit DVD menu"
#define MSGTR_MENU_Titles "Tituly"
#define MSGTR_MENU_Title "Titul %2d"
#define MSGTR_MENU_None "(žádné)"
#define MSGTR_MENU_Chapters "Kapitoly"
#define MSGTR_MENU_Chapter "Kapitola %2d"
#define MSGTR_MENU_AudioLanguages "Jazyk zvuku"
#define MSGTR_MENU_SubtitleLanguages "Jazyk titulků"
#define MSGTR_MENU_PlayList MSGTR_PlayList
#define MSGTR_MENU_SkinBrowser "Prohlíže�? témat"
#define MSGTR_MENU_Preferences MSGTR_Preferences
#define MSGTR_MENU_Exit "Konec"
#define MSGTR_MENU_Mute "Ztlumit"
#define MSGTR_MENU_Original "Původní"
#define MSGTR_MENU_AspectRatio "Poměr stran"
#define MSGTR_MENU_AudioTrack "Audio stopa"
#define MSGTR_MENU_Track "Stopa %d"
#define MSGTR_MENU_VideoTrack "Video stopa"
#define MSGTR_MENU_Subtitles "Titulky"

// --- equalizer
// Poznámka: Pokud změníte MSGTR_EQU_Audio, ujistěte se prosím, že vyhovuje i pro MSGTR_PREFERENCES_Audio
#define MSGTR_EQU_Audio "Zvuk"
// Poznámka: Pokud změníte MSGTR_EQU_Video, ujistěte se prosím, že vyhovuje i pro MSGTR_PREFERENCES_Video
#define MSGTR_EQU_Video "Obraz"
#define MSGTR_EQU_Contrast "Kontrast: "
#define MSGTR_EQU_Brightness "Jas: "
#define MSGTR_EQU_Hue "Odstín: "
#define MSGTR_EQU_Saturation "Sytost: "
#define MSGTR_EQU_Front_Left "Levý přední"
#define MSGTR_EQU_Front_Right "Pravý přední"
#define MSGTR_EQU_Back_Left "Levý zadní"
#define MSGTR_EQU_Back_Right "Pravý zadní"
#define MSGTR_EQU_Center "Středový"
#define MSGTR_EQU_Bass "Basový"
#define MSGTR_EQU_All "Vše"
#define MSGTR_EQU_Channel1 "Kanál 1:"
#define MSGTR_EQU_Channel2 "Kanál 2:"
#define MSGTR_EQU_Channel3 "Kanál 3:"
#define MSGTR_EQU_Channel4 "Kanál 4:"
#define MSGTR_EQU_Channel5 "Kanál 5:"
#define MSGTR_EQU_Channel6 "Kanál 6:"

// --- playlist
#define MSGTR_PLAYLIST_Path "Cesta"
#define MSGTR_PLAYLIST_Selected "Vybrané soubory"
#define MSGTR_PLAYLIST_Files "Soubory"
#define MSGTR_PLAYLIST_DirectoryTree "Adresáře"

// --- preferences
#define MSGTR_PREFERENCES_Audio MSGTR_EQU_Audio
#define MSGTR_PREFERENCES_Video MSGTR_EQU_Video
#define MSGTR_PREFERENCES_SubtitleOSD "Titulky & OSD"
#define MSGTR_PREFERENCES_Codecs "Kodeky & demuxer"
// Poznámka: Pokud změníte MSGTR_PREFERENCES_Misc, ujistěte se prosím, že vyhovuje i pro MSGTR_PREFERENCES_FRAME_Misc
#define MSGTR_PREFERENCES_Misc "Ostatní"
#define MSGTR_PREFERENCES_None "Nic"
#define MSGTR_PREFERENCES_DriverDefault "výchozí nastavení"
#define MSGTR_PREFERENCES_AvailableDrivers "Dostupné ovlada�?e:"
#define MSGTR_PREFERENCES_DoNotPlaySound "Nepřehrávat zvuk"
#define MSGTR_PREFERENCES_NormalizeSound "Normalizovat zvuk"
#define MSGTR_PREFERENCES_EnableEqualizer "Aktivovat ekvalizér"
#define MSGTR_PREFERENCES_SoftwareMixer "Aktivovat softwarový směšova�?"
#define MSGTR_PREFERENCES_ExtraStereo "Aktivovat extra stereo"
#define MSGTR_PREFERENCES_Coefficient "Koeficient:"
#define MSGTR_PREFERENCES_AudioDelay "Zpoždění zvuku"
#define MSGTR_PREFERENCES_DoubleBuffer "Aktivovat dvojitou vyrovnávací paměť"
#define MSGTR_PREFERENCES_DirectRender "Aktivovat direct rendering"
#define MSGTR_PREFERENCES_FrameDrop "Aktivovat zahazování snímků"
#define MSGTR_PREFERENCES_HFrameDrop "Aktivovat TVRDÉ zahazování snímků (nebezpe�?né)"
#define MSGTR_PREFERENCES_Flip "Převrátit obraz vzhůru nohama"
#define MSGTR_PREFERENCES_Panscan "Panscan:"
#define MSGTR_PREFERENCES_Subtitle "Titulky:"
#define MSGTR_PREFERENCES_SUB_Delay "Zpoždění: "
#define MSGTR_PREFERENCES_SUB_FPS "FPS:"
#define MSGTR_PREFERENCES_SUB_POS "Pozice: "
#define MSGTR_PREFERENCES_SUB_AutoLoad "Vypnout automatické na�?tení titulků"
#define MSGTR_PREFERENCES_SUB_Unicode "Titulky v UNICODE"
#define MSGTR_PREFERENCES_SUB_MPSUB "Převést dané titulky do vlastního formátu MPlayeru"
#define MSGTR_PREFERENCES_SUB_SRT "Převést dané titulky do �?asově orientovaného formátu SubViewer (SRT)"
#define MSGTR_PREFERENCES_SUB_Overlap "Zapnout překrývání titulků"
#define MSGTR_PREFERENCES_SUB_USE_ASS "SSA/ASS renderování titulku"
#define MSGTR_PREFERENCES_SUB_ASS_USE_MARGINS "Použijí se okraje"
#define MSGTR_PREFERENCES_SUB_ASS_TOP_MARGIN "Horní: "
#define MSGTR_PREFERENCES_SUB_ASS_BOTTOM_MARGIN "Dolní: "
#define MSGTR_PREFERENCES_Font "Font:"
#define MSGTR_PREFERENCES_FontFactor "Zvětšení Fontu:"
#define MSGTR_PREFERENCES_PostProcess "Aktivovat postprocessing"
#define MSGTR_PREFERENCES_AutoQuality "Automatické řízení kvality:"
#define MSGTR_PREFERENCES_NI "Použít parser pro neprokládaný AVI formát"
#define MSGTR_PREFERENCES_IDX "Znovu sestavit tabulku indexů, pokud je to třeba"
#define MSGTR_PREFERENCES_VideoCodecFamily "Rodina video kodeku:"
#define MSGTR_PREFERENCES_AudioCodecFamily "Rodina audio kodeku:"
#define MSGTR_PREFERENCES_FRAME_OSD_Level "Typ OSD"
#define MSGTR_PREFERENCES_FRAME_Subtitle "Titulky"
#define MSGTR_PREFERENCES_FRAME_Font "Font"
#define MSGTR_PREFERENCES_FRAME_PostProcess "Postprocessing"
#define MSGTR_PREFERENCES_FRAME_CodecDemuxer "Kodek & demuxer"
#define MSGTR_PREFERENCES_FRAME_Cache "Vyrovnávací paměť"
#define MSGTR_PREFERENCES_FRAME_Misc MSGTR_PREFERENCES_Misc
#define MSGTR_PREFERENCES_Audio_Device "Zařízení:"
#define MSGTR_PREFERENCES_Audio_Mixer "Mixér:"
#define MSGTR_PREFERENCES_Audio_MixerChannel "Kanál mixéru:"
#define MSGTR_PREFERENCES_Message "Pozor, některá nastavení potřebují pro svou funkci restartovat přehrávání!"
#define MSGTR_PREFERENCES_DXR3_VENC "Video enkodér:"
#define MSGTR_PREFERENCES_DXR3_LAVC "Použít LAVC (FFmpeg)"
#define MSGTR_PREFERENCES_FontEncoding1 "Unicode"
#define MSGTR_PREFERENCES_FontEncoding2 "Západoevropské jazyky (ISO-8859-1)"
#define MSGTR_PREFERENCES_FontEncoding3 "Západoevropské jazyky s Eurem (ISO-8859-15)"
#define MSGTR_PREFERENCES_FontEncoding4 "Slovanské/středoevropské jazyky (ISO-8859-2)"
#define MSGTR_PREFERENCES_FontEncoding5 "Esperanto, gaelština, maltézština, ture�?tina (ISO-8859-3)"
#define MSGTR_PREFERENCES_FontEncoding6 "Staré Baltské kódování (ISO-8859-4)"
#define MSGTR_PREFERENCES_FontEncoding7 "Cyrilice (ISO-8859-5)"
#define MSGTR_PREFERENCES_FontEncoding8 "Arabština (ISO-8859-6)"
#define MSGTR_PREFERENCES_FontEncoding9 "Moderní ře�?tina (ISO-8859-7)"
#define MSGTR_PREFERENCES_FontEncoding10 "Ture�?tina (ISO-8859-9)"
#define MSGTR_PREFERENCES_FontEncoding11 "Baltické (ISO-8859-13)"
#define MSGTR_PREFERENCES_FontEncoding12 "Keltština (ISO-8859-14)"
#define MSGTR_PREFERENCES_FontEncoding13 "Hebrejština (ISO-8859-8)"
#define MSGTR_PREFERENCES_FontEncoding14 "Ruština (KOI8-R)"
#define MSGTR_PREFERENCES_FontEncoding15 "Ukrajinština, běloruština (KOI8-U/RU)"
#define MSGTR_PREFERENCES_FontEncoding16 "Jednoduchá �?ínština (CP936)"
#define MSGTR_PREFERENCES_FontEncoding17 "Tradi�?ní �?ínština (BIG5)"
#define MSGTR_PREFERENCES_FontEncoding18 "Japonština (SHIFT-JIS)"
#define MSGTR_PREFERENCES_FontEncoding19 "Korejština (CP949)"
#define MSGTR_PREFERENCES_FontEncoding20 "Thajština (CP874)"
#define MSGTR_PREFERENCES_FontEncoding21 "Cyrilické Windows (CP1251)"
#define MSGTR_PREFERENCES_FontEncoding22 "Slovanské/středoevropské Windows (CP1250)"
#define MSGTR_PREFERENCES_FontEncoding23 "Arabské Windows (CP1256)"
#define MSGTR_PREFERENCES_FontNoAutoScale "Bez automatické velikosti"
#define MSGTR_PREFERENCES_FontPropWidth "Propor�?ně dle šířky obrazu"
#define MSGTR_PREFERENCES_FontPropHeight "Propor�?ně dle výšky obrazu"
#define MSGTR_PREFERENCES_FontPropDiagonal "Propor�?ně dle úhlopří�?ky"
#define MSGTR_PREFERENCES_FontEncoding "Kódování:"
#define MSGTR_PREFERENCES_FontBlur "Rozmazání:"
#define MSGTR_PREFERENCES_FontOutLine "Obrys:"
#define MSGTR_PREFERENCES_FontTextScale "Velikost textu:"
#define MSGTR_PREFERENCES_FontOSDScale "Velikost OSD:"
#define MSGTR_PREFERENCES_Cache "Zapnout vyrovnávací paměť"
#define MSGTR_PREFERENCES_CacheSize "Velikost vyrovnávací paměti: "
#define MSGTR_PREFERENCES_LoadFullscreen "Spustit přes celou obrazovku"
#define MSGTR_PREFERENCES_SaveWinPos "Uložit pozici okna"
#define MSGTR_PREFERENCES_XSCREENSAVER "Zastavit XScreenSaver"
#define MSGTR_PREFERENCES_PlayBar "Aktivovat playbar"
#define MSGTR_PREFERENCES_AutoSync "Zapnout automatickou synchronizaci"
#define MSGTR_PREFERENCES_AutoSyncValue "Automatická synchronizace: "
#define MSGTR_PREFERENCES_CDROMDevice "Zařízení CD-ROM:"
#define MSGTR_PREFERENCES_DVDDevice "Zařízení DVD:"
#define MSGTR_PREFERENCES_FPS "Snímková rychlost (FPS):"
#define MSGTR_PREFERENCES_ShowVideoWindow "Zobrazovat video okno při ne�?innosti"
#define MSGTR_PREFERENCES_ArtsBroken "Novější verze aRts jsou nekompatibilní "\
           "s GTK 1.x a zhavarují GMPlayer!"

// -- aboutbox
#define MSGTR_ABOUT_UHU "Vývoj GUI je sponzorován firmou UHU Linux\n"
#define MSGTR_ABOUT_Contributors "Přispěvatelé kódu a dokumentace\n"
#define MSGTR_ABOUT_Codecs_libs_contributions "Kodeky a knihovny třetích stran\n"
#define MSGTR_ABOUT_Translations "Překlady\n"
#define MSGTR_ABOUT_Skins "Skiny\n"

// --- messagebox
#define MSGTR_MSGBOX_LABEL_FatalError "Kritická chyba!"
#define MSGTR_MSGBOX_LABEL_Error "Chyba!"
#define MSGTR_MSGBOX_LABEL_Warning "Varování!"

// cfg.c
#define MSGTR_UnableToSaveOption "[cfg] Nelze uložit volbu '%s'.\n"

// interface.c
#define MSGTR_DeletingSubtitles "[GUI] Mažu titulky.\n"
#define MSGTR_LoadingSubtitles "[GUI] Na�?ítám titulky: %s\n"
#define MSGTR_AddingVideoFilter "[GUI] Přidávám video filtr: %s\n"

// mw.c
#define MSGTR_NotAFile "Toto nevypadá jako soubor: %s !\n"

// ws.c
#define MSGTR_WS_RemoteDisplay "[ws] Vzdálený display, vypínám XMITSHM.\n"
#define MSGTR_WS_NoXshm "[ws] Promiňte, ale váš systém nepodporuje rozšíření X shared memory.\n"
#define MSGTR_WS_NoXshape "[ws] Promiňte, ale váš systém nepodporuje rozšíření XShape.\n"
#define MSGTR_WS_ColorDepthTooLow "[ws] Promiňte, ale barevná hloubka je příliš malá.\n"
#define MSGTR_WS_TooManyOpenWindows "[ws] Příliš mnoho otevřených oken.\n"
#define MSGTR_WS_ShmError "[ws] chyba rozšíření shared memory\n"
#define MSGTR_WS_NotEnoughMemoryDrawBuffer "[ws] Promiňte, nedostatek paměti pro vykreslení bufferu.\n"
#define MSGTR_WS_DpmsUnavailable "DPMS není k dispozici?\n"
#define MSGTR_WS_DpmsNotEnabled "Nelze zapnout DPMS.\n"

// wsxdnd.c
#define MSGTR_WS_NotAFile "Toto nevypadá jako soubor...\n"
#define MSGTR_WS_DDNothing "D&D: Nic se nevrátilo!\n"


// ======================= video output drivers ========================

#define MSGTR_VOincompCodec "Vybrané video_out zařízení je nekompatibilní s tímto kodekem.\n"\
                "Zkuste přidat filtr scale do svého řetězce filtrů,\n"\
                "�?ili -vf spp,scale namísto -vf spp.\n"
#define MSGTR_VO_GenericError "Tato chyba nastala"
#define MSGTR_VO_UnableToAccess "Nemám přístup k"
#define MSGTR_VO_ExistsButNoDirectory "již existuje, ale není to adresář."
#define MSGTR_VO_DirExistsButNotWritable "Výstupní adresář již existuje, ale nelze do něj zapisovat."
#define MSGTR_VO_DirExistsAndIsWritable "Výstupní adresář již existuje a lze do něj zapisovat."
#define MSGTR_VO_CantCreateDirectory "Nelze vytvořit výstupní adresář."
#define MSGTR_VO_CantCreateFile "Nelze vytvořit výstupní soubor."
#define MSGTR_VO_DirectoryCreateSuccess "Úspěšně vytvořen výstupní adresář."
#define MSGTR_VO_ValueOutOfRange "hodnota mimo rozsah"
#define MSGTR_VO_NoValueSpecified "Nebyla zadána hodnota."
#define MSGTR_VO_UnknownSuboptions "neznámá(é) podvolba(y)"

// aspect.c
#define MSGTR_LIBVO_ASPECT_NoSuitableNewResFound "[ASPECT] Varování: Nenalezeno vhodné nové rozlišení!\n"
#define MSGTR_LIBVO_ASPECT_NoNewSizeFoundThatFitsIntoRes "[ASPECT] Chyba: Nenalezen nový rozměr vhodný pro dané rozlišení!\n"

// font_load_ft.c
#define MSGTR_LIBVO_FONT_LOAD_FT_NewFaceFailed "New_Face selhalo. Možná je chybná cesta k fontu.\nDodejte prosím soubor fontu pro texty (~/.mplayer/subfont.ttf).\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_NewMemoryFaceFailed "New_Memory_Face selhalo.\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_SubFaceFailed "font titulků: load_sub_face selhalo.\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_SubFontCharsetFailed "font titulků: prepare_charset selhalo.\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_CannotPrepareSubtitleFont "Nelze připravit font titulků.\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_CannotPrepareOSDFont "Nelze připravit font pro OSD.\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_CannotGenerateTables "Nelze generovat tabulky fontu.\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_DoneFreeTypeFailed "FT_Done_FreeType selhalo.\n"

// sub.c
#define MSGTR_VO_SUB_Seekbar "Pozice"
#define MSGTR_VO_SUB_Play "Přehrávání"
#define MSGTR_VO_SUB_Pause "Pauza"
#define MSGTR_VO_SUB_Stop "Stop"
#define MSGTR_VO_SUB_Rewind "Zpět"
#define MSGTR_VO_SUB_Forward "Vpřed"
#define MSGTR_VO_SUB_Clock "Hodiny"
#define MSGTR_VO_SUB_Contrast "Kontrast"
#define MSGTR_VO_SUB_Saturation "Sytost"
#define MSGTR_VO_SUB_Volume "Hlasitost"
#define MSGTR_VO_SUB_Brightness "Jas"
#define MSGTR_VO_SUB_Hue "Tónování"
#define MSGTR_VO_SUB_Balance "Váha"

// vo_3dfx.c
#define MSGTR_LIBVO_3DFX_Only16BppSupported "[VO_3DFX] Podporována pouze 16bit barevná hloubka!"
#define MSGTR_LIBVO_3DFX_VisualIdIs "[VO_3DFX] Visual ID je  %lx.\n"
#define MSGTR_LIBVO_3DFX_UnableToOpenDevice "[VO_3DFX] Nelze otevřít /dev/3dfx.\n"
#define MSGTR_LIBVO_3DFX_Error "[VO_3DFX] Chyba: %d.\n"
#define MSGTR_LIBVO_3DFX_CouldntMapMemoryArea "[VO_3DFX] Nelze mapovat 3dfx paměťové prostory: %p,%p,%d.\n"
#define MSGTR_LIBVO_3DFX_DisplayInitialized "[VO_3DFX] Inicializováno: %p.\n"
#define MSGTR_LIBVO_3DFX_UnknownSubdevice "[VO_3DFX] Neznámé podzařízení: %s.\n"

// vo_aa.c
#define MSGTR_VO_AA_HelpHeader "\n\nZde jsou podvolby aalib vo_aa:\n"
#define MSGTR_VO_AA_AdditionalOptions "Dodate�?né volby vo_aa zajišťují:\n" \
"  help        vypíše tuto nápovědu\n" \
"  osdcolor    nastaví barvu OSD\n  subcolor    nastaví barvu titulků\n" \
"        parametry barev jsou:\n           0 : normal\n" \
"           1 : dim\n           2 : bold\n           3 : boldfont\n" \
"           4 : reverse\n           5 : special\n\n\n"

// vo_dxr3.c
#define MSGTR_LIBVO_DXR3_UnableToLoadNewSPUPalette "[VO_DXR3] Nelze nahrát novou SPU paletu!\n"
#define MSGTR_LIBVO_DXR3_UnableToSetPlaymode "[VO_DXR3] Nelze nastavit přehrávací režim!\n"
#define MSGTR_LIBVO_DXR3_UnableToSetSubpictureMode "[VO_DXR3] Nelze nastavit režim subpicture!\n"
#define MSGTR_LIBVO_DXR3_UnableToGetTVNorm "[VO_DXR3] Nelze získat TV normu!\n"
#define MSGTR_LIBVO_DXR3_AutoSelectedTVNormByFrameRate "[VO_DXR3] TV norma zvolena automaticky podle snímkové rychlosti: "
#define MSGTR_LIBVO_DXR3_UnableToSetTVNorm "[VO_DXR3] Nelze nastavit TV normu!\n"
#define MSGTR_LIBVO_DXR3_SettingUpForNTSC "[VO_DXR3] Nastavuji NTSC.\n"
#define MSGTR_LIBVO_DXR3_SettingUpForPALSECAM "[VO_DXR3] Nastavuji PAL/SECAM.\n"
#define MSGTR_LIBVO_DXR3_SettingAspectRatioTo43 "[VO_DXR3] Nastavuji poměr stran na 4:3.\n"
#define MSGTR_LIBVO_DXR3_SettingAspectRatioTo169 "[VO_DXR3] Nastavuji poměr stran na 16:9.\n"
#define MSGTR_LIBVO_DXR3_OutOfMemory "[VO_DXR3] došla paměť\n"
#define MSGTR_LIBVO_DXR3_UnableToAllocateKeycolor "[VO_DXR3] Nelze alokovat klí�?ovací barvu!\n"
#define MSGTR_LIBVO_DXR3_UnableToAllocateExactKeycolor "[VO_DXR3] Nelze alokovat přesně klí�?ovací barvu,\n    používám nejbližší možnou (0x%lx).\n"
#define MSGTR_LIBVO_DXR3_Uninitializing "[VO_DXR3] Odinicializuji.\n"
#define MSGTR_LIBVO_DXR3_FailedRestoringTVNorm "[VO_DXR3] Selhalo obnovení TV normy!\n"
#define MSGTR_LIBVO_DXR3_EnablingPrebuffering "[VO_DXR3] Zapínám předsunutou vyrovnávací paměť.\n"
#define MSGTR_LIBVO_DXR3_UsingNewSyncEngine "[VO_DXR3] Používám nový synchroniza�?ní mechanizmus.\n"
#define MSGTR_LIBVO_DXR3_UsingOverlay "[VO_DXR3] Používám overlay.\n"
#define MSGTR_LIBVO_DXR3_ErrorYouNeedToCompileMplayerWithX11 "[VO_DXR3] Chyba: Overlay vyžaduje kompilaci s nainstalovanými knihovnami\n    a hlavi�?kovými soubory X11.\n"
#define MSGTR_LIBVO_DXR3_WillSetTVNormTo "[VO_DXR3] Nastavím TV normu na: "
#define MSGTR_LIBVO_DXR3_AutoAdjustToMovieFrameRatePALPAL60 "samopřizpůsobení snímkové rychlosti filmu (PAL/PAL-60)"
#define MSGTR_LIBVO_DXR3_AutoAdjustToMovieFrameRatePALNTSC "samopřizpůsobení snímkové rychlosti filmu (PAL/NTSC)"
#define MSGTR_LIBVO_DXR3_UseCurrentNorm "Použiji sou�?asnou normu."
#define MSGTR_LIBVO_DXR3_UseUnknownNormSuppliedCurrentNorm "Zadanou normu neznám. Použiji sou�?asnou."
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingTrying "[VO_DXR3] Nelze otevřít %s pro zápis, zkouším /dev/em8300.\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingTryingMV "[VO_DXR3] Nelze otevřít %s pro zápis, zkouším /dev/em8300_mv.\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingAsWell "[VO_DXR3] Ani /dev/em8300 nelze otevřít pro zápis!\nVzdávám to.\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingAsWellMV "[VO_DXR3] Ani /dev/em8300_mv nelze otevřít pro zápis!\nVzdávám to.\n"
#define MSGTR_LIBVO_DXR3_Opened "[VO_DXR3] Otevřeno: %s.\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingTryingSP "[VO_DXR3] Nelze otevřít %s pro zápis, zkouším /dev/em8300_sp instead.\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingAsWellSP "[VO_DXR3] Ani /dev/em8300_sp nelze otevřít pro zápis!\nVzdávám to.\n"
#define MSGTR_LIBVO_DXR3_UnableToOpenDisplayDuringHackSetup "[VO_DXR3] Nelze otevřít obrazovku během overlay hack nastavení!\n"
#define MSGTR_LIBVO_DXR3_UnableToInitX11 "[VO_DXR3] Nelze inicializovat X11!\n"
#define MSGTR_LIBVO_DXR3_FailedSettingOverlayAttribute "[VO_DXR3] Nelze nastavit overlay atribut.\n"
#define MSGTR_LIBVO_DXR3_FailedSettingOverlayScreen "[VO_DXR3] Selhalo nastavení overlay obrazu!\nKon�?ím.\n"
#define MSGTR_LIBVO_DXR3_FailedEnablingOverlay "[VO_DXR3] Selhalo zapnutí overlay!\nKon�?ím.\n"
#define MSGTR_LIBVO_DXR3_FailedResizingOverlayWindow "[VO_DXR3] Selhala změna velikosti overlay okna!\n"
#define MSGTR_LIBVO_DXR3_FailedSettingOverlayBcs "[VO_DXR3] Selhalo nastavení overlay bcs!\n"
#define MSGTR_LIBVO_DXR3_FailedGettingOverlayYOffsetValues "[VO_DXR3] Selhalo nastavení overlay Y offsetů!\nKon�?ím.\n"
#define MSGTR_LIBVO_DXR3_FailedGettingOverlayXOffsetValues "[VO_DXR3] Selhalo nastavení overlay X offsetů!\nKon�?ím.\n"
#define MSGTR_LIBVO_DXR3_FailedGettingOverlayXScaleCorrection "[VO_DXR3] Selhalo nastavení overlay X korekce rozlišení!\nKon�?ím.\n"
#define MSGTR_LIBVO_DXR3_YOffset "[VO_DXR3] Y offset: %d.\n"
#define MSGTR_LIBVO_DXR3_XOffset "[VO_DXR3] X offset: %d.\n"
#define MSGTR_LIBVO_DXR3_XCorrection "[VO_DXR3] X korekce: %d.\n"
#define MSGTR_LIBVO_DXR3_FailedSetSignalMix "[VO_DXR3] Selhalo nastavení signálové mixáže!\n"

// vo_jpeg.c
#define MSGTR_VO_JPEG_ProgressiveJPEG "Zapnut progresivní JPEG."
#define MSGTR_VO_JPEG_NoProgressiveJPEG "Vypnut progresivní JPEG."
#define MSGTR_VO_JPEG_BaselineJPEG "Zapnut základní JPEG."
#define MSGTR_VO_JPEG_NoBaselineJPEG "Vypnut základní JPEG."

// vo_mga.c
#define MSGTR_LIBVO_MGA_AspectResized "[VO_MGA] aspect(): škálováno na %dx%d.\n"
#define MSGTR_LIBVO_MGA_Uninit "[VO] odinicializace!\n"

// mga_template.c
#define MSGTR_LIBVO_MGA_ErrorInConfigIoctl "[MGA] chyba v mga_vid_config ioctl (špatná verze mga_vid.o?)"
#define MSGTR_LIBVO_MGA_CouldNotGetLumaValuesFromTheKernelModule "[MGA] Nelze získat jasové hodnoty z jaderného modulu!\n"
#define MSGTR_LIBVO_MGA_CouldNotSetLumaValuesFromTheKernelModule "[MGA] Nelze nastavit jasové hodnoty z jaderného modulu!\n"
#define MSGTR_LIBVO_MGA_ScreenWidthHeightUnknown "[MGA] Neznámá šířka/výška obrazovky!\n"
#define MSGTR_LIBVO_MGA_InvalidOutputFormat "[MGA] neplatný výstupní formát %0X\n"
#define MSGTR_LIBVO_MGA_IncompatibleDriverVersion "[MGA] Verze vašeho mga_vid ovlůada�?e je nekompatibilní s touto verzí MPlayeru!\n"
#define MSGTR_LIBVO_MGA_CouldntOpen "[MGA] Nelze otevřít: %s\n"
#define MSGTR_LIBVO_MGA_ResolutionTooHigh "[MGA] Rozlišení zdroje přesahuje 1023x1023 nejméně v jednom směru.\n[MGA] Přeškálujte softwarově, nebo použijte -lavdopts lowres=1.\n"
#define MSGTR_LIBVO_MGA_mgavidVersionMismatch "[MGA] neshoda verzí jaderného (%u) a MPlayerova (%u) mga_vid ovlada�?e\n"

// vo_null.c
#define MSGTR_LIBVO_NULL_UnknownSubdevice "[VO_NULL] Neznámé podzařízení: %s.\n"

// vo_png.c
#define MSGTR_LIBVO_PNG_Warning1 "[VO_PNG] Varování: kompresní úroveň nastavena na 0, komprese vypnuta!\n"
#define MSGTR_LIBVO_PNG_Warning2 "[VO_PNG] Info: Použijte -vo png:z=<n> pro nastavení kompresní úrovně od 0 do 9.\n"
#define MSGTR_LIBVO_PNG_Warning3 "[VO_PNG] Info: (0 = bez komprese, 1 = nejrychlejší, nejmenší - 9 nejlepší, nejpomalejší koprese)\n"
#define MSGTR_LIBVO_PNG_ErrorOpeningForWriting "\n[VO_PNG] Nelze otevřít '%s' pro zápis!\n"
#define MSGTR_LIBVO_PNG_ErrorInCreatePng "[VO_PNG] Chyba v create_png.\n"

// vo_pnm.c
#define MSGTR_VO_PNM_ASCIIMode "Zapnut ASCII režim."
#define MSGTR_VO_PNM_RawMode "Zapnut surový (Raw) režim."
#define MSGTR_VO_PNM_PPMType "Budou zapisovány PPM soubory."
#define MSGTR_VO_PNM_PGMType "Budou zapisovány PGM soubory."
#define MSGTR_VO_PNM_PGMYUVType "Budou zapisovány PGMYUV soubory."

// vo_sdl.c
#define MSGTR_LIBVO_SDL_CouldntGetAnyAcceptableSDLModeForOutput "[VO_SDL] Nelze získat přijatelný SDL Mode pro výstup.\n"
#define MSGTR_LIBVO_SDL_SetVideoModeFailed "[VO_SDL] set_video_mode: SDL_SetVideoMode selhalo: %s.\n"
#define MSGTR_LIBVO_SDL_MappingI420ToIYUV "[VO_SDL] Mapuji I420 na IYUV.\n"
#define MSGTR_LIBVO_SDL_UnsupportedImageFormat "[VO_SDL] Nepodporovaný formát obrazu (0x%X).\n"
#define MSGTR_LIBVO_SDL_InfoPleaseUseVmOrZoom "[VO_SDL] Info - použijte prosím -vm nebo -zoom pro přepnutí do nejlepřího rozlišení.\n"
#define MSGTR_LIBVO_SDL_FailedToSetVideoMode "[VO_SDL] Selhalo nbastavení video režimu: %s.\n"
#define MSGTR_LIBVO_SDL_CouldntCreateAYUVOverlay "[VO_SDL] Nelze vytvořit YUV overlay: %s.\n"
#define MSGTR_LIBVO_SDL_CouldntCreateARGBSurface "[VO_SDL] Nelze vytvořit RGB podklad: %s.\n"
#define MSGTR_LIBVO_SDL_UsingDepthColorspaceConversion "[VO_SDL] Použití konverze hloubky na barevnou paletu vše zpomalí (%ibpp -> %ibpp).\n"
#define MSGTR_LIBVO_SDL_UnsupportedImageFormatInDrawslice "[VO_SDL] Nepodporovaný formát obrazu v draw_slice, kontaktujte vývojáře MPlayeru!\n"
#define MSGTR_LIBVO_SDL_BlitFailed "[VO_SDL] Blit selhalo: %s.\n"
#define MSGTR_LIBVO_SDL_InitializationFailed "[VO_SDL] Inicializace SDL selhala: %s.\n"
#define MSGTR_LIBVO_SDL_UsingDriver "[VO_SDL] Používám ovlada�?: %s.\n"

// vo_svga.c
#define MSGTR_LIBVO_SVGA_ForcedVidmodeNotAvailable "[VO_SVGA] Vynucený vid_mode %d (%s) není k dispozici.\n"
#define MSGTR_LIBVO_SVGA_ForcedVidmodeTooSmall "[VO_SVGA] Vynucený vid_mode %d (%s) je moc malý.\n"
#define MSGTR_LIBVO_SVGA_Vidmode "[VO_SVGA] Vid_mode: %d, %dx%d %dbpp.\n"
#define MSGTR_LIBVO_SVGA_VgasetmodeFailed "[VO_SVGA] Vga_setmode(%d) selhalo.\n"
#define MSGTR_LIBVO_SVGA_VideoModeIsLinearAndMemcpyCouldBeUsed "[VO_SVGA] Videorežim je lineární a memcpy lze použít pro transfer obrazu.\n"
#define MSGTR_LIBVO_SVGA_VideoModeHasHardwareAcceleration "[VO_SVGA] Videorežim má hardwarovou akceleraci a put_image lze použít.\n"
#define MSGTR_LIBVO_SVGA_IfItWorksForYouIWouldLikeToKnow "[VO_SVGA] Rád bych věděl zda vám to funguje.\n[VO_SVGA] (pošlete log získaný takto: `mplayer test.avi -v -v -v -v &> svga.log`). Thx!\n"
#define MSGTR_LIBVO_SVGA_VideoModeHas "[VO_SVGA] Videorežim má %d stran.\n"
#define MSGTR_LIBVO_SVGA_CenteringImageStartAt "[VO_SVGA] Centruji obraz. Po�?átek na (%d,%d)\n"
#define MSGTR_LIBVO_SVGA_UsingVidix "[VO_SVGA] Používám VIDIX. w=%i h=%i  mw=%i mh=%i\n"

// vo_tdfx_vid.c
#define MSGTR_LIBVO_TDFXVID_Move "[VO_TDXVID] Přesun %d(%d) x %d => %d.\n"
#define MSGTR_LIBVO_TDFXVID_AGPMoveFailedToClearTheScreen "[VO_TDFXVID] AGP přesun nesmazal obrazovku.\n"
#define MSGTR_LIBVO_TDFXVID_BlitFailed "[VO_TDFXVID] Blit selhalo.\n"
#define MSGTR_LIBVO_TDFXVID_NonNativeOverlayFormatNeedConversion "[VO_TDFXVID] Nenativní overlay formát vyžaduje konverzi.\n"
#define MSGTR_LIBVO_TDFXVID_UnsupportedInputFormat "[VO_TDFXVID] Nepodporovaný vstupní formát 0x%x.\n"
#define MSGTR_LIBVO_TDFXVID_OverlaySetupFailed "[VO_TDFXVID] Nastavení overlay selhalo.\n"
#define MSGTR_LIBVO_TDFXVID_OverlayOnFailed "[VO_TDFXVID] Zapnutí overlay selhalo.\n"
#define MSGTR_LIBVO_TDFXVID_OverlayReady "[VO_TDFXVID] Overlay připraveno: %d(%d) x %d @ %d => %d(%d) x %d @ %d.\n"
#define MSGTR_LIBVO_TDFXVID_TextureBlitReady "[VO_TDFXVID] Texture blit připraveno: %d(%d) x %d @ %d => %d(%d) x %d @ %d.\n"
#define MSGTR_LIBVO_TDFXVID_OverlayOffFailed "[VO_TDFXVID] Vypnutí overlay selhalo\n"
#define MSGTR_LIBVO_TDFXVID_CantOpen "[VO_TDFXVID] Nelze otevřít %s: %s.\n"
#define MSGTR_LIBVO_TDFXVID_CantGetCurrentCfg "[VO_TDFXVID] Nelze získat sou�?asnou konfiguraci: %s.\n"
#define MSGTR_LIBVO_TDFXVID_MemmapFailed "[VO_TDFXVID] Memmap selhalo !!!!!\n"
#define MSGTR_LIBVO_TDFXVID_GetImageTodo "Get image todo.\n" //I have no idea how to translate this.
#define MSGTR_LIBVO_TDFXVID_AgpMoveFailed "[VO_TDFXVID] AGP přesun selhal.\n"
#define MSGTR_LIBVO_TDFXVID_SetYuvFailed "[VO_TDFXVID] Nastavení YUV selhalo.\n"
#define MSGTR_LIBVO_TDFXVID_AgpMoveFailedOnYPlane "[VO_TDFXVID] AGP přesun selhal v kanálu Y.\n"
#define MSGTR_LIBVO_TDFXVID_AgpMoveFailedOnUPlane "[VO_TDFXVID] AGP přesun selhal v kanálu U.\n"
#define MSGTR_LIBVO_TDFXVID_AgpMoveFailedOnVPlane "[VO_TDFXVID] AGP přesun selhal v kanálu V.\n"
#define MSGTR_LIBVO_TDFXVID_UnknownFormat "[VO_TDFXVID] neznámý formát: 0x%x.\n"

// vo_tdfxfb.c
#define MSGTR_LIBVO_TDFXFB_CantOpen "[VO_TDFXFB] Nelze otevřít %s: %s.\n"
#define MSGTR_LIBVO_TDFXFB_ProblemWithFbitgetFscreenInfo "[VO_TDFXFB] Problém s FBITGET_FSCREENINFO ioctl: %s.\n"
#define MSGTR_LIBVO_TDFXFB_ProblemWithFbitgetVscreenInfo "[VO_TDFXFB] Problém s FBITGET_VSCREENINFO ioctl: %s.\n"
#define MSGTR_LIBVO_TDFXFB_ThisDriverOnlySupports "[VO_TDFXFB] Toto rozhraní podporuje pouze 3Dfx Banshee, Voodoo3 a Voodoo 5.\n"
#define MSGTR_LIBVO_TDFXFB_OutputIsNotSupported "[VO_TDFXFB] %d bpp výstup není podporován.\n"
#define MSGTR_LIBVO_TDFXFB_CouldntMapMemoryAreas "[VO_TDFXFB] Nelze mapovat paměťové rozsahy: %s.\n"
#define MSGTR_LIBVO_TDFXFB_BppOutputIsNotSupported "[VO_TDFXFB] %d bpp výstup není podporován (Tak to by se nikdy nemělo stát.).\n"
#define MSGTR_LIBVO_TDFXFB_SomethingIsWrongWithControl "[VO_TDFXFB] Jejda! Něco je špatně s control().\n"
#define MSGTR_LIBVO_TDFXFB_NotEnoughVideoMemoryToPlay "[VO_TDFXFB] Není dost paměti pro přehrátí tohoto filmu. Zkuste nižší rozlišení.\n"
#define MSGTR_LIBVO_TDFXFB_ScreenIs "[VO_TDFXFB] Obraz má %dx%d při %d bpp, vstup je %dx%d při %d bpp, norma je %dx%d.\n"

// vo_tga.c
#define MSGTR_LIBVO_TGA_UnknownSubdevice "[VO_TGA] Neznámé podzařízení: %s.\n"

// vo_vesa.c
#define MSGTR_LIBVO_VESA_FatalErrorOccurred "[VO_VESA] Nastala fatální chyba! Nelze pokra�?ovat.\n"
#define MSGTR_LIBVO_VESA_UnknownSubdevice "[VO_VESA] neznámé podzařízení: '%s'.\n"
#define MSGTR_LIBVO_VESA_YouHaveTooLittleVideoMemory "[VO_VESA] Máte malou video paměť pro tento režim:\n[VO_VESA] Vyžadováno: %08lX poskytnuto: %08lX.\n"
#define MSGTR_LIBVO_VESA_YouHaveToSpecifyTheCapabilitiesOfTheMonitor "[VO_VESA] Musíte poskytnout schopnosti monitoru. Neměním obnovovací kmito�?et.\n"
#define MSGTR_LIBVO_VESA_UnableToFitTheMode "[VO_VESA] Režim přesahuje schopnosti monitoru. Neměním obnovovací kmito�?et.\n"
#define MSGTR_LIBVO_VESA_DetectedInternalFatalError "[VO_VESA] Zjištěna interní fatální chyba: init je voláno před preinit.\n"
#define MSGTR_LIBVO_VESA_SwitchFlipIsNotSupported "[VO_VESA] Volba -flip není podporována.\n"
#define MSGTR_LIBVO_VESA_PossibleReasonNoVbe2BiosFound "[VO_VESA] Možný důvod: Nenalezen VBE2 BIOS.\n"
#define MSGTR_LIBVO_VESA_FoundVesaVbeBiosVersion "[VO_VESA] Nalezen VESA VBE BIOS Verze %x.%x Revize: %x.\n"
#define MSGTR_LIBVO_VESA_VideoMemory "[VO_VESA] Video paměť: %u Kb.\n"
#define MSGTR_LIBVO_VESA_Capabilites "[VO_VESA] VESA Capabilities: %s %s %s %s %s.\n"
#define MSGTR_LIBVO_VESA_BelowWillBePrintedOemInfo "[VO_VESA] !!! OEM info bude vypsáno níže !!!\n"
#define MSGTR_LIBVO_VESA_YouShouldSee5OemRelatedLines "[VO_VESA] Níže byste měli vidět 5 řádků k OEM; Pokud ne, máte vadný vm86.\n"
#define MSGTR_LIBVO_VESA_OemInfo "[VO_VESA] OEM info: %s.\n"
#define MSGTR_LIBVO_VESA_OemRevision "[VO_VESA] OEM Revize: %x.\n"
#define MSGTR_LIBVO_VESA_OemVendor "[VO_VESA] OEM vydavatel: %s.\n"
#define MSGTR_LIBVO_VESA_OemProductName "[VO_VESA] OEM Název produktu: %s.\n"
#define MSGTR_LIBVO_VESA_OemProductRev "[VO_VESA] OEM Revize produktu: %s.\n"
#define MSGTR_LIBVO_VESA_Hint "[VO_VESA] Tip: Pro funk�?ní TV-Out byste měli mít zapojen TV konektor\n"\
"[VO_VESA] před bootem, jelikož VESA BIOS se inicializuje pouze během POST.\n"
#define MSGTR_LIBVO_VESA_UsingVesaMode "[VO_VESA] Používám VESA režim (%u) = %x [%ux%u@%u]\n"
#define MSGTR_LIBVO_VESA_CantInitializeSwscaler "[VO_VESA] Nelze inicializovat softwarový škálova�?.\n"
#define MSGTR_LIBVO_VESA_CantUseDga "[VO_VESA] Nelze použít DGA. Vynucuji režim bank switching. :(\n"
#define MSGTR_LIBVO_VESA_UsingDga "[VO_VESA] Používám DGA (fyzické zdroje: %08lXh, %08lXh)"
#define MSGTR_LIBVO_VESA_CantUseDoubleBuffering "[VO_VESA] Nelze použít dvojitý buffer: nedostatek video paměti.\n"
#define MSGTR_LIBVO_VESA_CantFindNeitherDga "[VO_VESA] Nelze nalézt ani DGA ani relokovatelné okno.\n"
#define MSGTR_LIBVO_VESA_YouveForcedDga "[VO_VESA] Vynutili jste DGA. Kon�?ím\n"
#define MSGTR_LIBVO_VESA_CantFindValidWindowAddress "[VO_VESA] Nelze najít platnou adresu okna.\n"
#define MSGTR_LIBVO_VESA_UsingBankSwitchingMode "[VO_VESA] Používám bank switching režim (fyzické zdroje: %08lXh, %08lXh).\n"
#define MSGTR_LIBVO_VESA_CantAllocateTemporaryBuffer "[VO_VESA] Nelze alokovat do�?asný buffer.\n"
#define MSGTR_LIBVO_VESA_SorryUnsupportedMode "[VO_VESA] Promiňte, nepodporovaný režim -- zkuste -x 640 -zoom.\n"
#define MSGTR_LIBVO_VESA_OhYouReallyHavePictureOnTv "[VO_VESA] Páni, vážně máte obraz na TV!\n"
#define MSGTR_LIBVO_VESA_CantInitialozeLinuxVideoOverlay "[VO_VESA] Nelze inicializovat Linux Video Overlay.\n"
#define MSGTR_LIBVO_VESA_UsingVideoOverlay "[VO_VESA] Používám video overlay: %s.\n"
#define MSGTR_LIBVO_VESA_CantInitializeVidixDriver "[VO_VESA] Nelze inicializovat VIDIX ovlada�?.\n"
#define MSGTR_LIBVO_VESA_UsingVidix "[VO_VESA] Používám VIDIX.\n"
#define MSGTR_LIBVO_VESA_CantFindModeFor "[VO_VESA] Nelze najít režim pro: %ux%u@%u.\n"
#define MSGTR_LIBVO_VESA_InitializationComplete "[VO_VESA] Inicializace VESA je dokon�?ena.\n"

// vesa_lvo.c
#define MSGTR_LIBVO_VESA_ThisBranchIsNoLongerSupported "[VESA_LVO] Tato řada již není podporována.\n[VESA_LVO] Použijte prosím -vo vesa:vidix.\n"
#define MSGTR_LIBVO_VESA_CouldntOpen "[VESA_LVO] Nelze otevřít: '%s'\n"
#define MSGTR_LIBVO_VESA_InvalidOutputFormat "[VESA_LVI] Neplatný výstupní formát: %s(%0X)\n"
#define MSGTR_LIBVO_VESA_IncompatibleDriverVersion "[VESA_LVO] Verze vašeho fb_vid ovlada�?e je nekompatibilní s touto verzí MPlayeru!\n"

// vo_x11.c
#define MSGTR_LIBVO_X11_DrawFrameCalled "[VO_X11] draw_frame() zavolána!!!!!!\n"

// vo_xv.c
#define MSGTR_LIBVO_XV_DrawFrameCalled "[VO_XV] draw_frame() zavolána!!!!!!\n"
#define MSGTR_LIBVO_XV_SharedMemoryNotSupported "[VO_XV] Sdílená paměť není podporována\nVracím se k běžnému Xv.\n"
#define MSGTR_LIBVO_XV_XvNotSupportedByX11 "[VO_XV] Promiňte, Xv není touto verzí/ovlada�?em X11 podporováno\n[VO_XV] ******** Zkuste -vo x11  or  -vo sdl  *********\n"
#define MSGTR_LIBVO_XV_XvQueryAdaptorsFailed  "[VO_XV] XvQueryAdaptors selhalo.\n"
#define MSGTR_LIBVO_XV_InvalidPortParameter "[VO_XV] Zadán neplatný port, měním na port 0.\n"
#define MSGTR_LIBVO_XV_CouldNotGrabPort "[VO_XV] Nelze zabrat port %i.\n"
#define MSGTR_LIBVO_XV_CouldNotFindFreePort "[VO_XV] Nelze nalézt volný Xvideo port - možná je využit jiným procesem\n"\
"[VO_XV] Zavřete všechny video aplikace a zkuste to znovu. Pokud to nepomůže,\n"\
"[VO_XV] vyberte si pomocí 'mplayer -vo help' jiné (ne-xv) video rozhraní.\n"
#define MSGTR_LIBVO_XV_NoXvideoSupport "[VO_XV] Zdá se, že popora Xvideo pro vaši video kartu není k dispozici.\n"\
"[VO_XV] Spusťte 'xvinfo' pro ověření podpory Xv a pře�?těte si\n"\
"[VO_XV] DOCS/HTML/cs/video.html#xv!\n"\
"[VO_XV] Viz také 'mplayer -vo help' pro jiné (ne-xv) video rozhraní.\n"\
"[VO_XV] Zkuste -vo x11.\n"
#define MSGTR_VO_XV_ImagedimTooHigh "Rozměry zdrojového obrazu jsou příliš velké: %ux%u (maximum je %ux%u)\n"

// vo_yuv4mpeg.c
#define MSGTR_VO_YUV4MPEG_InterlacedHeightDivisibleBy4 "Prokládaný režim obrazu vyžaduje výšku obrazu dělitelnou 4."
#define MSGTR_VO_YUV4MPEG_InterlacedLineBufAllocFail "Nelze alokovat řádkovou vyrovnávací paměť pro režim prokládaného obrazu."
#define MSGTR_VO_YUV4MPEG_WidthDivisibleBy2 "Šířka obrazu musí být dělitelná 2."
#define MSGTR_VO_YUV4MPEG_OutFileOpenError "Nelze získat paměť nebo ukazatel souboru pro zápis \"%s\"!"
#define MSGTR_VO_YUV4MPEG_OutFileWriteError "Chyba při zápisu obrázku na výstup!"
#define MSGTR_VO_YUV4MPEG_UnknownSubDev "Neznámé podzařízení: %s"
#define MSGTR_VO_YUV4MPEG_InterlacedTFFMode "Používám prokládaný výstupní režim, horní pole napřed."
#define MSGTR_VO_YUV4MPEG_InterlacedBFFMode "Používám prokládaný výstupní režim, dolní pole napřed."
#define MSGTR_VO_YUV4MPEG_ProgressiveMode "Používám (výchozí) neprokládaný snímkový režim."

// vosub_vidix.c
#define MSGTR_LIBVO_SUB_VIDIX_CantStartPlayback "[VO_SUB_VIDIX] Nelze za�?ít přehrávat: %s\n"
#define MSGTR_LIBVO_SUB_VIDIX_CantStopPlayback "[VO_SUB_VIDIX] Nelze skon�?it přehrávání: %s\n"
#define MSGTR_LIBVO_SUB_VIDIX_InterleavedUvForYuv410pNotSupported "[VO_SUB_VIDIX] Prokládané UV pro YUV410P není podporováno.\n"
#define MSGTR_LIBVO_SUB_VIDIX_DummyVidixdrawsliceWasCalled "[VO_SUB_VIDIX] Dummy vidix_draw_slice() zavolána.\n"
#define MSGTR_LIBVO_SUB_VIDIX_DummyVidixdrawframeWasCalled "[VO_SUB_VIDIX] Dummy vidix_draw_frame() zavolána.\n"
#define MSGTR_LIBVO_SUB_VIDIX_UnsupportedFourccForThisVidixDriver "[VO_SUB_VIDIX] Nepodporované FourCC pro tento VIDIX ovlada�?: %x (%s).\n"
#define MSGTR_LIBVO_SUB_VIDIX_VideoServerHasUnsupportedResolution "[VO_SUB_VIDIX] Video server nepodporuje rozlišení (%dx%d), podporuje: %dx%d-%dx%d.\n"
#define MSGTR_LIBVO_SUB_VIDIX_VideoServerHasUnsupportedColorDepth "[VO_SUB_VIDIX] Video server nepodporuje ve vidix barevnou hloubku (%d).\n"
#define MSGTR_LIBVO_SUB_VIDIX_DriverCantUpscaleImage "[VO_SUB_VIDIX] VIDIX ovlada�? nemůže zvětšit obraz (%d%d -> %d%d).\n"
#define MSGTR_LIBVO_SUB_VIDIX_DriverCantDownscaleImage "[VO_SUB_VIDIX] VIDIX ovlada�? nemůže zmenšit obraz (%d%d -> %d%d).\n"
#define MSGTR_LIBVO_SUB_VIDIX_CantConfigurePlayback "[VO_SUB_VIDIX] Nelze konfigurovat přehrávání: %s.\n"
#define MSGTR_LIBVO_SUB_VIDIX_YouHaveWrongVersionOfVidixLibrary "[VO_SUB_VIDIX] Máte špatnou verzi knihovny VIDIX.\n"
#define MSGTR_LIBVO_SUB_VIDIX_CouldntFindWorkingVidixDriver "[VO_SUB_VIDIX] Nelze najít funk�?ní VIDIX ovlada�?.\n"
#define MSGTR_LIBVO_SUB_VIDIX_CouldntGetCapability "[VO_SUB_VIDIX] Nelze získat schopnost: %s.\n"

// x11_common.c
#define MSGTR_EwmhFullscreenStateFailed "\nX11: Nelze odeslat událost EWMH fullscreen!\n"
#define MSGTR_CouldNotFindXScreenSaver "xscreensaver_disable: Nelze nalézt okno XScreenSaveru.\n"
#define MSGTR_SelectedVideoMode "XF86VM: Vybraný video režim %dx%d pro velikost obrazu %dx%d.\n"

#define MSGTR_InsertingAfVolume "[Mixer] Hardwarová mixáž není k dispozici, vkládám filtr volume.\n"
#define MSGTR_NoVolume "[Mixer] Ovládání hlasitosti není k dislozici.\n"
#define MSGTR_NoBalance "[Mixer] Stereováha není k dispozici.\n"

// old vo drivers that have been replaced
#define MSGTR_VO_PGM_HasBeenReplaced "Výstupní videorozhraní pgm bylo nahrazeno -vo pnm:pgmyuv.\n"
#define MSGTR_VO_MD5_HasBeenReplaced "Výstupní videorozhraní md5 bylo nahrazeno -vo md5sum.\n"


// ======================= audio output drivers ========================

// audio_out.c
#define MSGTR_AO_ALSA9_1x_Removed "audio_out: moduly alsa9 a alsa1x byly odstraněny, místo nich použijte -ao alsa.\n"
#define MSGTR_AO_NoSuchDriver "Takové audio rozhraní není '%.*s'\n"
#define MSGTR_AO_FailedInit "Selhala inicializace audio rozhraní '%s'\n"

// ao_oss.c
#define MSGTR_AO_OSS_CantOpenMixer "[AO OSS] audio_setup: Nelze otevřít mixážní zařízení %s: %s\n"
#define MSGTR_AO_OSS_ChanNotFound "[AO OSS] audio_setup: Mixer zvukové karty nemá kanál '%s', používám výchozí.\n"
#define MSGTR_AO_OSS_CantOpenDev "[AO OSS] audio_setup: Nelze otevřít zvukové zařízení %s: %s\n"
#define MSGTR_AO_OSS_CantMakeFd "[AO OSS] audio_setup: Nelze provést blokování souborového deskriptoru: %s\n"
#define MSGTR_AO_OSS_CantSet "[AO OSS] Zvukové zařízení %s nelze nastavit na výstup %s, zkouším %s...\n"
#define MSGTR_AO_OSS_CantSetChans "[AO OSS] audio_setup: Selhalo nastavení výstupního zvukového zařízení na %d kanálů.\n"
#define MSGTR_AO_OSS_CantUseGetospace "[AO OSS] audio_setup: Ovlada�? nepodporuje SNDCTL_DSP_GETOSPACE :-(\n"
#define MSGTR_AO_OSS_CantUseSelect "[AO OSS]\n   ***  Ovlada�? Vaší zvukové karty NEPODPORUJE select()  ***\n Překompilujte MPlayer s #undef HAVE_AUDIO_SELECT v config.h !\n\n"
#define MSGTR_AO_OSS_CantReopen "[AO OSS]\nKritická chyba: *** NELZE ZNOVUOTEVŘ�?T / RESTARTOVAT ZVUKOVÉ ZAŘ�?ZEN�? *** %s\n"
#define MSGTR_AO_OSS_UnknownUnsupportedFormat "[AO OSS] Neznámý/nepodporovaný OSS formát: %x.\n"

// ao_arts.c
#define MSGTR_AO_ARTS_CantInit "[AO ARTS] %s\n"
#define MSGTR_AO_ARTS_ServerConnect "[AO ARTS] Připojen ke zvukovému serveru.\n"
#define MSGTR_AO_ARTS_CantOpenStream "[AO ARTS] Nelze otevřít datový proud.\n"
#define MSGTR_AO_ARTS_StreamOpen "[AO ARTS] Datový proud otevřen.\n"
#define MSGTR_AO_ARTS_BufferSize "[AO ARTS] velikost vyrovnávací paměti: %d\n"

// ao_dxr2.c
#define MSGTR_AO_DXR2_SetVolFailed "[AO DXR2] Nastavení hlasitosti na %d selhalo.\n"
#define MSGTR_AO_DXR2_UnsupSamplerate "[AO DXR2] %d Hz není podporováno, zkuste převzorkovat.\n"

// ao_esd.c
#define MSGTR_AO_ESD_CantOpenSound "[AO ESD] esd_open_sound selhalo: %s\n"
#define MSGTR_AO_ESD_LatencyInfo "[AO ESD] latence: [server: %0.2fs, síť: %0.2fs] (upravuji %0.2fs)\n"
#define MSGTR_AO_ESD_CantOpenPBStream "[AO ESD] selhalo otevření datového proudu ESD pro přehrávání: %s\n"

// ao_mpegpes.c
#define MSGTR_AO_MPEGPES_CantSetMixer "[AO MPEGPES] selhalo nastavení DVB zvukového mixeru: %s.\n"
#define MSGTR_AO_MPEGPES_UnsupSamplerate "[AO MPEGPES] %d Hz není podporováno, zkuste převzorkovat.\n"

// ao_pcm.c
#define MSGTR_AO_PCM_FileInfo "[AO PCM] Soubor: %s (%s)\nPCM: Vzorkování: %iHz Kanál(y): %s Formát %s\n"
#define MSGTR_AO_PCM_HintInfo "[AO PCM] Info:  Nejrychlejší extrakce dosáhnete s -vc null -vo null -ao pcm:fast\n[AO PCM] Info: Pro zápis WAVE souborů použijte -ao pcm:waveheader (výchozí).\n"
#define MSGTR_AO_PCM_CantOpenOutputFile "[AO PCM] Selhalo otevření %s pro zápis!\n"

// ao_sdl.c
#define MSGTR_AO_SDL_INFO "[AO SDL] Vzorkování: %iHz Kanál(y): %s Formát %s\n"
#define MSGTR_AO_SDL_DriverInfo "[AO SDL] používám zvukový ovlada�? %s.\n"
#define MSGTR_AO_SDL_UnsupportedAudioFmt "[AO SDL] Nepodporovaný formát zvuku: 0x%x.\n"
#define MSGTR_AO_SDL_CantInit "[AO SDL] Inicializace SDL Audio selhala: %s\n"
#define MSGTR_AO_SDL_CantOpenAudio "[AO SDL] Nelze otevřít zvuk: %s\n"

// ao_sgi.c
#define MSGTR_AO_SGI_INFO "[AO SGI] ovládání.\n"
#define MSGTR_AO_SGI_InitInfo "[AO SGI] init: Vzorkování: %iHz Kanál(y): %s Formát %s\n"
#define MSGTR_AO_SGI_InvalidDevice "[AO SGI] přehrávání: neplatné zařízení.\n"
#define MSGTR_AO_SGI_CantSetParms_Samplerate "[AO SGI] init: selhalo setparams: %s\nNelze nastavit požadované vzorkování.\n"
#define MSGTR_AO_SGI_CantSetAlRate "[AO SGI] init: AL_RATE nebyl přijat daným zdrojem.\n"
#define MSGTR_AO_SGI_CantGetParms "[AO SGI] init: selhalo getparams: %s\n"
#define MSGTR_AO_SGI_SampleRateInfo "[AO SGI] init: vzorkování je nyní %f (požadovaný kmito�?et je %f)\n"
#define MSGTR_AO_SGI_InitConfigError "[AO SGI] init: %s\n"
#define MSGTR_AO_SGI_InitOpenAudioFailed "[AO SGI] init: Nelze otevřít zvukový kanál: %s\n"
#define MSGTR_AO_SGI_Uninit "[AO SGI] uninit: ...\n"
#define MSGTR_AO_SGI_Reset "[AO SGI] reset: ...\n"
#define MSGTR_AO_SGI_PauseInfo "[AO SGI] audio_pause: ...\n"
#define MSGTR_AO_SGI_ResumeInfo "[AO SGI] audio_resume: ...\n"

// ao_sun.c
#define MSGTR_AO_SUN_RtscSetinfoFailed "[AO SUN] rtsc: selhalo SETINFO.\n"
#define MSGTR_AO_SUN_RtscWriteFailed "[AO SUN] rtsc: zápis selhal.\n"
#define MSGTR_AO_SUN_CantOpenAudioDev "[AO SUN] Nelze otevřít zvukové zařízení %s, %s  -> nebude zvuk.\n"
#define MSGTR_AO_SUN_UnsupSampleRate "[AO SUN] audio_setup: Vaše karta nepodporuje %d kanálové, %s, %d Hz vzorkování.\n"
#define MSGTR_AO_SUN_CantUseSelect "[AO SUN]\n   ***  Ovlada�? Vaší zvukové karty NEPODPORUJE select()  ***\n Překompilujte MPlayer s #undef HAVE_AUDIO_SELECT v config.h !\n\n"
#define MSGTR_AO_SUN_CantReopenReset "[AO SUN]\nKritická chyba: *** NELZE ZNOVUOTEVŘ�?T / RESTARTOVAT ZVUKOVÉ ZAŘ�?ZEN�? (%s) ***\n"

// ao_alsa.c
#define MSGTR_AO_ALSA_InvalidMixerIndexDefaultingToZero "[AO_ALSA] Neplatný index směšova�?ů. Používám výchozí 0.\n"
#define MSGTR_AO_ALSA_MixerOpenError "[AO_ALSA] Chyba otevření směšova�?e: %s\n"
#define MSGTR_AO_ALSA_MixerAttachError "[AO_ALSA] Připojení směšova�?e %s skon�?ilo chybou: %s\n"
#define MSGTR_AO_ALSA_MixerRegisterError "[AO_ALSA] Chyba registrace směšova�?e: %s\n"
#define MSGTR_AO_ALSA_MixerLoadError "[AO_ALSA] Chyba na�?tení směšova�?e: %s\n"
#define MSGTR_AO_ALSA_UnableToFindSimpleControl "[AO_ALSA] Nemohu nalézt přepína�? '%s',%i.\n"
#define MSGTR_AO_ALSA_ErrorSettingLeftChannel "[AO_ALSA] Chyba nastavení levého kanálu, %s\n"
#define MSGTR_AO_ALSA_ErrorSettingRightChannel "[AO_ALSA] Chyba nastavení pravého kanálu, %s\n"
#define MSGTR_AO_ALSA_CommandlineHelp "\n[AO_ALSA] -ao alsa nápověda:\n"\
"[AO_ALSA] Příklad: mplayer -ao alsa:device=hw=0.3\n"\
"[AO_ALSA]   Nastaví první kartu �?tvrté hardwarové zařízení.\n\n"\
"[AO_ALSA] Volby:\n"\
"[AO_ALSA]   noblock\n"\
"[AO_ALSA]     Otevře zařízení v neblokujícím režimu.\n"\
"[AO_ALSA]   device=<název_zařízení>\n"\
"[AO_ALSA]     Nastaví zařízení (změňte , za . a : za =)\n"
#define MSGTR_AO_ALSA_ChannelsNotSupported "[AO_ALSA] %d kanálů není podporováno.\n"
#define MSGTR_AO_ALSA_OpenInNonblockModeFailed "[AO_ALSA] Otevření v neblokujícím režimu selhalo, zkouším otevřít v blokujícím.\n"
#define MSGTR_AO_ALSA_PlaybackOpenError "[AO_ALSA] Chyba otevření přehrávání: %s\n"
#define MSGTR_AO_ALSA_ErrorSetBlockMode "[AL_ALSA] Chyba nastavení blokujícího režimu %s.\n"
#define MSGTR_AO_ALSA_UnableToGetInitialParameters "[AO_ALSA] Nelze získat po�?áte�?ní parametry: %s\n"
#define MSGTR_AO_ALSA_UnableToSetAccessType "[AO_ALSA] Nelze nastavit typ přístupu: %s\n"
#define MSGTR_AO_ALSA_FormatNotSupportedByHardware "[AO_ALSA] Formát %s není hardwarově podporován, zkouším výchozí.\n"
#define MSGTR_AO_ALSA_UnableToSetFormat "[AO_ALSA] Nelze nastavit formát: %s\n"
#define MSGTR_AO_ALSA_UnableToSetChannels "[AO_ALSA] Nelze nastavit kanály: %s\n"
#define MSGTR_AO_ALSA_UnableToDisableResampling "[AO_ALSA] Nelze vypnout převzorkování: %s\n"
#define MSGTR_AO_ALSA_UnableToSetSamplerate2 "[AO_ALSA] Nelze nastavit vzorkovací kmito�?et-2: %s\n"
#define MSGTR_AO_ALSA_UnableToSetBufferTimeNear "[AO_ALSA] Nelze nastavit �?asový buffer okolo: %s\n"
#define MSGTR_AO_ALSA_UnableToGetPeriodSize "[AO ALSA] Nelze získat velikost periody: %s\n"
#define MSGTR_AO_ALSA_UnableToSetPeriods "[AO_ALSA] Nelze nastavit periody: %s\n"
#define MSGTR_AO_ALSA_UnableToSetHwParameters "[AO_ALSA] Nelze nastavit hw parametry: %s\n"
#define MSGTR_AO_ALSA_UnableToGetBufferSize "[AO_ALSA] Nelze získat velikost bufferu: %s\n"
#define MSGTR_AO_ALSA_UnableToGetSwParameters "[AO_ALSA] Nelze získat sw parametry: %s\n"
#define MSGTR_AO_ALSA_UnableToSetSwParameters "[AO_ALSA] Nelze nastavit sw parametry: %s\n"
#define MSGTR_AO_ALSA_UnableToGetBoundary "[AO_ALSA] Nelze získat boundary: %s\n"
#define MSGTR_AO_ALSA_UnableToSetStartThreshold "[AO_ALSA] Nelze nastavit startovací práh: %s\n"
#define MSGTR_AO_ALSA_UnableToSetStopThreshold "[AO_ALSA] Nelze nastavit stop práh: %s\n"
#define MSGTR_AO_ALSA_UnableToSetSilenceSize "[AO_ALSA] Nelze nastavit velikost ticha: %s\n"
#define MSGTR_AO_ALSA_PcmCloseError "[AO_ALSA] chyba zavření pcm: %s\n"
#define MSGTR_AO_ALSA_NoHandlerDefined "[AO_ALSA] Nebyl definován handler!\n"
#define MSGTR_AO_ALSA_PcmPrepareError "[AO_ALSA] chyba přípravy pcm: %s\n"
#define MSGTR_AO_ALSA_PcmPauseError "[AO_ALSA] chyba pauzy pcm: %s\n"
#define MSGTR_AO_ALSA_PcmDropError "[AO_ALSA] chyba shození pcm: %s\n"
#define MSGTR_AO_ALSA_PcmResumeError "[AO_ALSA] chyba obnovení pcm: %s\n"
#define MSGTR_AO_ALSA_DeviceConfigurationError "[AO_ALSA] Chyba konfigurace zařízení."
#define MSGTR_AO_ALSA_PcmInSuspendModeTryingResume "[AO_ALSA] Pcm v režimu suspend, zkouším obnovit.\n"
#define MSGTR_AO_ALSA_WriteError "[AO_ALSA] Chyba zápisu: %s\n"
#define MSGTR_AO_ALSA_TryingToResetSoundcard "[AO_ALSA] Zkouším resetovat zvukovou kartu.\n"
#define MSGTR_AO_ALSA_CannotGetPcmStatus "[AO_ALSA] Nelze získat status pcm: %s\n"

// ao_plugin.c
#define MSGTR_AO_PLUGIN_InvalidPlugin "[AO PLUGIN] neplatný zásuvný modul: %s\n"


// ======================= audio filters ================================

// af_scaletempo.c
#define MSGTR_AF_ValueOutOfRange MSGTR_VO_ValueOutOfRange

// af_ladspa.c
#define MSGTR_AF_LADSPA_AvailableLabels "dostupné názvy v"
#define MSGTR_AF_LADSPA_WarnNoInputs "VAROV�?N�?! Tento LADSPA plugin nemá audio vstupy.\n  Vstupní audio signál bude ztracen."
#define MSGTR_AF_LADSPA_ErrMultiChannel "Vícekanálové (>2) pluginy nejsou podporovány (zatím).\n  Používejte pouze mono a stereo pluginy."
#define MSGTR_AF_LADSPA_ErrNoOutputs "Tento LADSPA plugin nemá audio výstupy."
#define MSGTR_AF_LADSPA_ErrInOutDiff "Po�?et audio vstupů LADSPA pluginu je odlišný od po�?tu audio výstupů."
#define MSGTR_AF_LADSPA_ErrFailedToLoad "selhalo na�?tení"
#define MSGTR_AF_LADSPA_ErrNoDescriptor "Nelze nalézt funkci ladspa_descriptor() v uvedené knihovně."
#define MSGTR_AF_LADSPA_ErrLabelNotFound "Nelze nalézt požadovaný název v knihovně pluginů."
#define MSGTR_AF_LADSPA_ErrNoSuboptions "Nebyla zadány žádné podvolby."
#define MSGTR_AF_LADSPA_ErrNoLibFile "Nebyla zadána žádná knihovna."
#define MSGTR_AF_LADSPA_ErrNoLabel "Nebyl zadán název žádného filtru."
#define MSGTR_AF_LADSPA_ErrNotEnoughControls "Na příkazovém řádku bylo uvedeno málo voleb."
#define MSGTR_AF_LADSPA_ErrControlBelow "%s: Vstupní voli�? #%d je nižší než minimální hodnota %0.4f.\n"
#define MSGTR_AF_LADSPA_ErrControlAbove "%s: Vstupní voli�? #%d je vyšší než maximální hodnota %0.4f.\n"

// format.c
#define MSGTR_AF_FORMAT_UnknownFormat "neznámý formát "


// ========================== INPUT =========================================

// joystick.c
#define MSGTR_INPUT_JOYSTICK_CantOpen "Nelze otevřít zařízení joysticku %s: %s\n"
#define MSGTR_INPUT_JOYSTICK_ErrReading "Chyba při �?tení zařízení joysticku: %s\n"
#define MSGTR_INPUT_JOYSTICK_LoosingBytes "Joystick: Uvolnili jsme %d bajtů dat\n"
#define MSGTR_INPUT_JOYSTICK_WarnLostSync "Joystick: warning init event, ztratili jsme synchronizaci s ovlada�?em.\n"
#define MSGTR_INPUT_JOYSTICK_WarnUnknownEvent "Joystick: varování, neznámý typ události %d\n"

// appleir.c
#define MSGTR_INPUT_APPLE_IR_CantOpen "Nelze otevřít Apple IR zařízení: %s\n"

// input.c
#define MSGTR_INPUT_INPUT_ErrCantRegister2ManyCmdFds "Příliš mnoho souborových deskriptorů příkazů, nelze registrovat\n deskriptor souboru %d.\n"
#define MSGTR_INPUT_INPUT_ErrCantRegister2ManyKeyFds "Příliš mnoho souborových deskriptorů klávesnice, nelze registrovat\n deskriptor souboru %d.\n"
#define MSGTR_INPUT_INPUT_ErrArgMustBeInt "Příkaz %s: argument %d není typu integer.\n"
#define MSGTR_INPUT_INPUT_ErrArgMustBeFloat "Příkaz %s: argument %d není typu float.\n"
#define MSGTR_INPUT_INPUT_ErrUnterminatedArg "Příkaz %s: argument %d není ukon�?en.\n"
#define MSGTR_INPUT_INPUT_ErrUnknownArg "Neznámý argument %d\n"
#define MSGTR_INPUT_INPUT_Err2FewArgs "Příkaz %s vyžaduje aspoň %d argumentů, nalezli jsme jich však pouze %d.\n"
#define MSGTR_INPUT_INPUT_ErrReadingCmdFd "Chyba při �?tení příkazového deskriptoru souboru %d: %s\n"
#define MSGTR_INPUT_INPUT_ErrCmdBufferFullDroppingContent "Vyrovnávací paměť deskriptoru souboru příkazů %d je plná: zahazuji obsah.\n"
#define MSGTR_INPUT_INPUT_ErrInvalidCommandForKey "Špatný příkaz pro přiřazení klávese %s"
#define MSGTR_INPUT_INPUT_ErrSelect "Chyba výběru: %s\n"
#define MSGTR_INPUT_INPUT_ErrOnKeyInFd "Chyba v deskriptoru souboru klávesového vstupu %d\n"
#define MSGTR_INPUT_INPUT_ErrDeadKeyOnFd "Vstup mrtvé klávesy z deskriptoru souboru %d\n"
#define MSGTR_INPUT_INPUT_Err2ManyKeyDowns "Příliš mnoho sou�?asně stisknutých kláves\n"
#define MSGTR_INPUT_INPUT_ErrOnCmdFd "Chyba na cmd fd %d\n"
#define MSGTR_INPUT_INPUT_ErrReadingInputConfig "Chyba při �?tení input konfigura�?ního souboru %s: %s\n"
#define MSGTR_INPUT_INPUT_ErrUnknownKey "Neznámá klávesa '%s'\n"
#define MSGTR_INPUT_INPUT_ErrUnfinishedBinding "Nedokon�?ené přiřazení %s\n"
#define MSGTR_INPUT_INPUT_ErrBuffer2SmallForKeyName "Příliš malá vyrovnávací paměť pro tento název klávesy: %s\n"
#define MSGTR_INPUT_INPUT_ErrNoCmdForKey "Nenalezen příkaz pro tla�?ítko %s"
#define MSGTR_INPUT_INPUT_ErrBuffer2SmallForCmd "Příliš malá vyrovnávací paměť pro příkaz %s\n"
#define MSGTR_INPUT_INPUT_ErrWhyHere "Co tady děláme?\n"
#define MSGTR_INPUT_INPUT_ErrCantInitJoystick "Nelze inicializovat vstupní joystick\n"
#define MSGTR_INPUT_INPUT_ErrCantOpenFile "Nelze otevřít %s: %s\n"
#define MSGTR_INPUT_INPUT_ErrCantInitAppleRemote "Nelze inicializovat vstupní Apple Remote.\n"

// lirc.c
#define MSGTR_LIRCopenfailed "Selhalo otevření podpory LIRC. Nebudete moci používat dálkové ovládání.\n"
#define MSGTR_LIRCcfgerr "Nepovedlo se pře�?íst konfigura�?ní soubor LIRC %s.\n"


// ========================== LIBMPDEMUX ===================================

// muxer.c, muxer_*.c:
#define MSGTR_TooManyStreams "Příliš mnoho datových proudů!"
#define MSGTR_RawMuxerOnlyOneStream "Muxer surového zvuku podporuje pouze jeden zvukový proud!\n"
#define MSGTR_IgnoringVideoStream "Ignoruji video proud!\n"
#define MSGTR_UnknownStreamType "Varování, neznámý typ datového proudu: %d\n"
#define MSGTR_WarningLenIsntDivisible "Varování, délka není násobkem velikosti vzorku!\n"
#define MSGTR_MuxbufMallocErr "Nelze alokovat paměť pro snímkovou vyrovnávací paměť muxeru!\n"
#define MSGTR_MuxbufReallocErr "Nelze realokovat paměť pro snímkovou vyrovnávací paměť muxeru!\n"
#define MSGTR_WritingHeader "Zapisuji hlavi�?ku...\n"
#define MSGTR_WritingTrailer "Zapisuji index...\n"

// demuxer.c, demux_*.c:
#define MSGTR_AudioStreamRedefined "VAROV�?N�?: Hlavi�?ka audio proudu %d předefinována!\n"
#define MSGTR_VideoStreamRedefined "VAROV�?N�?: Hlavi�?ka video proudu %d předefinována!\n"
#define MSGTR_TooManyAudioInBuffer "\nPříliš mnoho audio paketů ve vyrovnávací paměti: (%d v %d bajtech)\n"
#define MSGTR_TooManyVideoInBuffer "\nPříliš mnoho video paketů ve vyrovnávací paměti: (%d v %d bajtech)\n"
#define MSGTR_MaybeNI "Možná přehráváte neprokládaný proud/soubor nebo kodek selhal?\n"\
                      "V AVI souborech zkuste vynutit neprokládaný režim pomocí volby -ni.\n"
#define MSGTR_WorkAroundBlockAlignHeaderBug "AVI: Obcházím chybu CBR-MP3 nBlockAlign header!\n"
#define MSGTR_SwitchToNi "\nDetekován špatně prokládaný AVI soubor - přepínám do -ni režimu...\n"
#define MSGTR_InvalidAudioStreamNosound "AVI: neplatný audio proud ID: %d - ignoruji (nebude zvuk)\n"
#define MSGTR_InvalidAudioStreamUsingDefault "AVI: neplatný video proud ID: %d - ignoruji (používám výchozí)\n"
#define MSGTR_ON2AviFormat "ON2 AVI formát"
#define MSGTR_Detected_XXX_FileFormat "Detekován formát souboru %s.\n"
#define MSGTR_DetectedAudiofile "Detekován zvukový soubor.\n"
#define MSGTR_InvalidMPEGES "Špatný MPEG-ES proud??? Kontaktujte autora, možná to je chyba :(\n"
#define MSGTR_FormatNotRecognized "======= Bohužel, formát tohoto souboru nebyl rozpoznán/není podporován =======\n"\
                                  "==== Pokud je soubor AVI, ASF nebo MPEG proud, kontaktujte prosím autora! ====\n"
#define MSGTR_SettingProcessPriority "Nastavuji prioritu procesu: %s\n"
#define MSGTR_FilefmtFourccSizeFpsFtime "[V] fmtsouboru:%d  fourcc:0x%X  velikost:%dx%d  fps:%5.3f  f�?as:=%6.4f\n"
#define MSGTR_CannotInitializeMuxer "Nelze inicializovat muxer."
#define MSGTR_MissingVideoStream "Nebyl nalezen video proud.\n"
#define MSGTR_MissingAudioStream "Nebyl nalezen audio proud -> bez zvuku.\n"
#define MSGTR_MissingVideoStreamBug "Chybí video proud!? Kontaktujte autora, může to být chyba :(\n"

#define MSGTR_DoesntContainSelectedStream "demux: Soubor neobsahuje zvolený audio nebo video proud.\n"

#define MSGTR_NI_Forced "Vynucen"
#define MSGTR_NI_Detected "Detekován"
#define MSGTR_NI_Message "%s NEPROKL�?DAN�? formát AVI souboru.\n"

#define MSGTR_UsingNINI "Používám NEPROKL�?DANÉ vadné formátování AVI souboru.\n"
#define MSGTR_CouldntDetFNo "Nelze ur�?it po�?et snímků (pro absolutní posun)\n"
#define MSGTR_CantSeekRawAVI "Nelze se posouvat v surových (raw) AVI proudech! (Potřebuji index, zkuste použít volbu -idx.)\n"
#define MSGTR_CantSeekFile "Nemohu se posouvat v tomto souboru.\n"

#define MSGTR_MOVcomprhdr "MOV: Komprimované hlavi�?ky vyžadují ZLIB!\n"
#define MSGTR_MOVvariableFourCC "MOV: VAROV�?N�?: Proměnná FourCC detekována!?\n"
#define MSGTR_MOVtooManyTrk "MOV: VAROV�?N�?: příliš mnoho stop"
#define MSGTR_DetectedTV "Detekována TV! ;-)\n"
#define MSGTR_ErrorOpeningOGGDemuxer "Nelze otevřít Ogg demuxer.\n"
#define MSGTR_CannotOpenAudioStream "Nemohu otevřít audio proud: %s\n"
#define MSGTR_CannotOpenSubtitlesStream "Nemohu otevřít proud s titulky: %s\n"
#define MSGTR_OpeningAudioDemuxerFailed "Nepovedlo se otevřít audio demuxer: %s\n"
#define MSGTR_OpeningSubtitlesDemuxerFailed "Nepovedlo se otevřít demuxer pro titulky: %s\n"
#define MSGTR_TVInputNotSeekable "TV vstup neumožňuje posun! (\"Posun\" bude pravděpodobně použit pro změnu kanálů ;)\n"
#define MSGTR_DemuxerInfoChanged "Info demuxeru %s změněno na %s\n"
#define MSGTR_ClipInfo "Informace o klipu:\n"

#define MSGTR_LeaveTelecineMode "\ndemux_mpg: detekováno 30000/1001 fps NTSC, přepínám frekvenci snímků.\n"
#define MSGTR_EnterTelecineMode "\ndemux_mpg: detekováno 24000/1001 fps progresivní NTSC, přepínám frekvenci snímků.\n"

#define MSGTR_CacheFill "\rNaplnění cache: %5.2f%% (%"PRId64" bajtů)   "
#define MSGTR_NoBindFound "Tla�?ítko '%s' nemá přiřazenu žádnou funkci.\n"
#define MSGTR_FailedToOpen "Selhalo otevření %s.\n"

#define MSGTR_VideoID "Nalezen video proud [%s], -vid %d\n"
#define MSGTR_AudioID "Nalezen audio proud [%s], -aid %d\n"
#define MSGTR_SubtitleID "Nalezen titulkový proud [%s], -sid %d\n"

// asfheader.c
#define MSGTR_MPDEMUX_ASFHDR_HeaderSizeOver1MB "FATAL: velikost hlavi�?ky je větší než 1 MB (%d)!\nKontaktujte prosím tvůrce MPlayeru a nahrajte/pošlete jim tento soubor.\n"
#define MSGTR_MPDEMUX_ASFHDR_HeaderMallocFailed "Nemohu alokovat %d bajtů pro hlavi�?ku.\n"
#define MSGTR_MPDEMUX_ASFHDR_EOFWhileReadingHeader "konec souboru při �?tení ASF hlavi�?ky, poškozený/neúplný soubor?\n"
#define MSGTR_MPDEMUX_ASFHDR_DVRWantsLibavformat "DVR bude pravděpodobně pracovat pouze s libavformat, v případě problémů zkuste -demuxer 35\n"
#define MSGTR_MPDEMUX_ASFHDR_NoDataChunkAfterHeader "Po hlavi�?ce nenásleduje žádný datový chunk!\n"
#define MSGTR_MPDEMUX_ASFHDR_AudioVideoHeaderNotFound "ASF: ani audio ani video hlavi�?ky nebyly nalezeny - vadný soubor?\n"
#define MSGTR_MPDEMUX_ASFHDR_InvalidLengthInASFHeader "Nesprávná délka v hlavi�?ce ASF!\n"
#define MSGTR_MPDEMUX_ASFHDR_DRMLicenseURL "DRM Licence URL: %s\n"
#define MSGTR_MPDEMUX_ASFHDR_DRMProtected "Tento soubor byl zatížen DRM šifrou, v MPlayeru nelze přehrát!\n"

// aviheader.c
#define MSGTR_MPDEMUX_AVIHDR_EmptyList "** prázdný seznam?!\n"
#define MSGTR_MPDEMUX_AVIHDR_WarnNotExtendedAVIHdr "** Varování: toto není rozšířená AVI hlavi�?ka..\n"
#define MSGTR_MPDEMUX_AVIHDR_BuildingODMLidx "AVI: ODML: Vytvářím ODML index (%d superindexchunků).\n"
#define MSGTR_MPDEMUX_AVIHDR_BrokenODMLfile "AVI: ODML: Detekován vadný (neúplný?) soubor. Použije se tradi�?ní index.\n"
#define MSGTR_MPDEMUX_AVIHDR_CantReadIdxFile "Nelze �?íst indexový soubor %s: %s\n"
#define MSGTR_MPDEMUX_AVIHDR_NotValidMPidxFile "%s není planý indexový soubor pro MPlayer.\n"
#define MSGTR_MPDEMUX_AVIHDR_FailedMallocForIdxFile "Nemohu alokovat paměť pro data indexu od %s.\n"
#define MSGTR_MPDEMUX_AVIHDR_PrematureEOF "před�?asný konec indexového souboru %s\n"
#define MSGTR_MPDEMUX_AVIHDR_IdxFileLoaded "Nahrán indexový soubor: %s\n"
#define MSGTR_MPDEMUX_AVIHDR_GeneratingIdx "Generuji index: %3lu %s     \r"
#define MSGTR_MPDEMUX_AVIHDR_IdxGeneratedForHowManyChunks "AVI: Vygenerována tabulka indexu pro %d chunků!\n"
#define MSGTR_MPDEMUX_AVIHDR_Failed2WriteIdxFile "Nelze zapsat indexový soubor %s: %s\n"
#define MSGTR_MPDEMUX_AVIHDR_IdxFileSaved "Uložen indexový soubor: %s\n"

// demux_audio.c
#define MSGTR_MPDEMUX_AUDIO_UnknownFormat "Audio demuxer: neznámý formát %d.\n"

// demux_demuxers.c
#define MSGTR_MPDEMUX_DEMUXERS_FillBufferError "fill_buffer chyba: špatný demuxer: ani vd, ad nebo sd.\n"

// demux_mkv.c
#define MSGTR_MPDEMUX_MKV_ZlibInitializationFailed "[mkv] inicializace zlib selhala.\n"
#define MSGTR_MPDEMUX_MKV_ZlibDecompressionFailed "[mkv] zlib dekomprese selhala.\n"
#define MSGTR_MPDEMUX_MKV_LzoInitializationFailed "[mkv] inicializace lzo selhala.\n"
#define MSGTR_MPDEMUX_MKV_LzoDecompressionFailed "[mkv] lzo dekomprese selhala.\n"
#define MSGTR_MPDEMUX_MKV_TrackEncrypted "[mkv] Stopa �?íslo %u byla zašifrována a dešifrování nebylo dosud\n[mkv] implementováno. Přeskakuji stopu.\n"
#define MSGTR_MPDEMUX_MKV_UnknownContentEncoding "[mkv] Nezanámý typ enkódování stopy %u. Přeskakuji stopu.\n"
#define MSGTR_MPDEMUX_MKV_UnknownCompression "[mkv] Stopa %u byla komprimována neznámým/nepodporovaným kompresním\n[mkv] algoritmem (%u). Přeskakuji stopu.\n"
#define MSGTR_MPDEMUX_MKV_ZlibCompressionUnsupported "[mkv] Stopa %u byla komprimována pomocí zlib, ale MPlayer nebyl kompilován\n[mkv] s podporou zlib komprese. Přeskakuji stopu.\n"
#define MSGTR_MPDEMUX_MKV_TrackIDName "[mkv] Stopa ID %u: %s (%s) \"%s\", %s\n"
#define MSGTR_MPDEMUX_MKV_TrackID "[mkv] Stopa ID %u: %s (%s), %s\n"
#define MSGTR_MPDEMUX_MKV_UnknownCodecID "[mkv] Neznámé/nepodporované ID kodeku (%s) nebo chybějící/vadná CodecPrivate\n[mkv] data (stopa %u).\n"
#define MSGTR_MPDEMUX_MKV_FlacTrackDoesNotContainValidHeaders "[mkv] Stopa FLAC neobsahuje platné hlavi�?ky.\n"
#define MSGTR_MPDEMUX_MKV_UnknownAudioCodec "[mkv] Neznámé/nepodporované ID audio kodeku '%s' pro stopu %u nebo chybějící/chybná\n[mkv] private codec data.\n"
#define MSGTR_MPDEMUX_MKV_SubtitleTypeNotSupported "[mkv] Typ titulků '%s' není podporován.\n"
#define MSGTR_MPDEMUX_MKV_WillPlayVideoTrack "[mkv] Budu přehrávat video stopu %u.\n"
#define MSGTR_MPDEMUX_MKV_NoVideoTrackFound "[mkv] Žádná video stopa nenalezena/požadována.\n"
#define MSGTR_MPDEMUX_MKV_NoAudioTrackFound "[mkv] Žádná audio stopa nenalezena/požadována.\n"
#define MSGTR_MPDEMUX_MKV_WillDisplaySubtitleTrack "[mkv] Budu zobrazovat titulkovou stopu %u.\n"
#define MSGTR_MPDEMUX_MKV_NoBlockDurationForSubtitleTrackFound "[mkv] Varování: Nebylo nalezeno žádné BlockDuration pro titulkovou stopu.\n"
#define MSGTR_MPDEMUX_MKV_TooManySublines "[mkv] Varování: příliš mnoho titulků k renderování, přeskakuji.\n"
#define MSGTR_MPDEMUX_MKV_TooManySublinesSkippingAfterFirst "\n[mkv] Varování: příliš mnoho titulků k renderování, přeskakuji po prvních %i.\n"

// demux_nuv.c
#define MSGTR_MPDEMUX_NUV_NoVideoBlocksInFile "V souboru nejsou žádné bloky videa.\n"

// demux_xmms.c
#define MSGTR_MPDEMUX_XMMS_FoundPlugin "Nalezen plugin: %s (%s).\n"
#define MSGTR_MPDEMUX_XMMS_ClosingPlugin "Uzavírám plugin: %s.\n"
#define MSGTR_MPDEMUX_XMMS_WaitForStart "Čekám až XMMS plugin zahájí přehrávání '%s'...\n"


// ========================== LIBMENU ===================================

// common
#define MSGTR_LIBMENU_NoEntryFoundInTheMenuDefinition "[MENU] V definici menu není žádná položka.\n"

// libmenu/menu.c
#define MSGTR_LIBMENU_SyntaxErrorAtLine "[MENU] syntaktická chyba na řádku: %d\n"
#define MSGTR_LIBMENU_MenuDefinitionsNeedANameAttrib "[MENU] V definici menu je potřeba jmenný atribut (řádek %d)\n"
#define MSGTR_LIBMENU_BadAttrib "[MENU] špatný atribut %s=%s v menu '%s' na řádku %d\n"
#define MSGTR_LIBMENU_UnknownMenuType "[MENU] neznámý typ menu '%s' na řádce %d\n"
#define MSGTR_LIBMENU_CantOpenConfigFile "[MENU] Nemohu otevřít konfigura�?ní soubor menu: %s\n"
#define MSGTR_LIBMENU_ConfigFileIsTooBig "[MENU] Konfigura�?ní soubor je příliš velký. (> %d KB)\n"
#define MSGTR_LIBMENU_ConfigFileIsEmpty "[MENU] Konfigura�?ní soubor je prázdný.\n"
#define MSGTR_LIBMENU_MenuNotFound "[MENU] Menu %s nebylo nalezeno.\n"
#define MSGTR_LIBMENU_MenuInitFailed "[MENU] Menu '%s': Selhala inicializace.\n"
#define MSGTR_LIBMENU_UnsupportedOutformat "[MENU] Nepodporovaný výstupní formát!\n"

// libmenu/menu_cmdlist.c
#define MSGTR_LIBMENU_ListMenuEntryDefinitionsNeedAName "[MENU] Položky typu seznam vyžadují název (řádek %d).\n"
#define MSGTR_LIBMENU_ListMenuNeedsAnArgument "[MENU] Položka typu seznam vyžaduje argument.\n"

// libmenu/menu_console.c
#define MSGTR_LIBMENU_WaitPidError "[MENU] Chyba při �?ekání na PID: %s.\n"
#define MSGTR_LIBMENU_SelectError "[MENU] Chyba výběru.\n"
#define MSGTR_LIBMENU_ReadErrorOnChildFD "[MENU] Chyba �?tení na popisova�?i souboru potomka: %s.\n"
#define MSGTR_LIBMENU_ConsoleRun "[MENU] Spuštění v konsoli: %s ...\n"
#define MSGTR_LIBMENU_AChildIsAlreadyRunning "[MENU] Potomek už běží.\n"
#define MSGTR_LIBMENU_ForkFailed "[MENU] Forkování selhalo!!!\n"
#define MSGTR_LIBMENU_WriteError "[MENU] chyba při zápisu.\n"

// libmenu/menu_filesel.c
#define MSGTR_LIBMENU_OpendirError "[MENU] chyba při otevírání adresáře: %s\n"
#define MSGTR_LIBMENU_ReallocError "[MENU] chyba při relokaci: %s\n"
#define MSGTR_LIBMENU_MallocError "[MENU] chyba při alokaci paměti: %s\n"
#define MSGTR_LIBMENU_ReaddirError "[MENU] chyba �?tení adresáře: %s\n"
#define MSGTR_LIBMENU_CantOpenDirectory "[MENU] Nelze otevřít adresář %s.\n"

// libmenu/menu_param.c
#define MSGTR_LIBMENU_SubmenuDefinitionNeedAMenuAttribut "[MENU] Při definici podmenu je potřeba uvést atribut 'menu'.\n"
#define MSGTR_LIBMENU_InvalidProperty "[MENU] Neplatná vlastnost '%s' v pref menu. (řádek %d).\n"
#define MSGTR_LIBMENU_PrefMenuEntryDefinitionsNeed "[MENU] Preferen�?ní položka menu vyžaduje korektní atribut 'property' nebo 'txt' (řádek %d).\n"
#define MSGTR_LIBMENU_PrefMenuNeedsAnArgument "[MENU] Preferen�?ní menu vyžaduje argument.\n"

// libmenu/menu_pt.c
#define MSGTR_LIBMENU_CantfindTheTargetItem "[MENU] Nemohu nalézt cílovou položku??\n"
#define MSGTR_LIBMENU_FailedToBuildCommand "[MENU] Selhalo sestavení příkazu: %s.\n"

// libmenu/menu_txt.c
#define MSGTR_LIBMENU_MenuTxtNeedATxtFileName "[MENU] Textové menu vyžaduje název souboru txt (parametrický soubor).\n"
#define MSGTR_LIBMENU_MenuTxtCantOpen "[MENU] Nelze otevřít: %s.\n"
#define MSGTR_LIBMENU_WarningTooLongLineSplitting "[MENU] Pozor, řádka je příliš dlouhá. Rozděluju ji.\n"
#define MSGTR_LIBMENU_ParsedLines "[MENU] Zpracováno %d řádků.\n"

// libmenu/vf_menu.c
#define MSGTR_LIBMENU_UnknownMenuCommand "[MENU] Neznámý příkaz: '%s'.\n"
#define MSGTR_LIBMENU_FailedToOpenMenu "[MENU] Nemohu otevřít menu: '%s'.\n"


// ========================== LIBMPCODECS ===================================

// dec_video.c & dec_audio.c
#define MSGTR_CantOpenCodec "Nelze otevřít kodek.\n"
#define MSGTR_CantCloseCodec "Nelze uzavřít kodek.\n"

#define MSGTR_MissingDLLcodec "CHYBA: Nelze otevřít požadovaný DirectShow kodek %s.\n"
#define MSGTR_ACMiniterror "Nemohu na�?íst/inicializovat Win32/ACM audio kodek (chybí DLL soubor?).\n"
#define MSGTR_MissingLAVCcodec "Nemohu najít kodek '%s' v libavcodec...\n"

#define MSGTR_MpegNoSequHdr "MPEG: KRITICK�? CHYBA: Konec souboru v průběhu vyhledávání hlavi�?ky sekvence.\n"
#define MSGTR_CannotReadMpegSequHdr "KRITICK�? CHYBA: Nelze pře�?íst hlavi�?ku sekvence.\n"
#define MSGTR_CannotReadMpegSequHdrEx "KRITICK�? CHYBA: Nelze pře�?íst rozšíření hlavi�?ky sekvence.\n"
#define MSGTR_BadMpegSequHdr "MPEG: Špatná hlavi�?ka sekvence.\n"
#define MSGTR_BadMpegSequHdrEx "MPEG: Špatné rozšíření hlavi�?ky sekvence.\n"

#define MSGTR_ShMemAllocFail "Nelze alokovat sdílenou paměť\n"
#define MSGTR_CantAllocAudioBuf "Nelze alokovat vyrovnávací paměť pro zvukový výstup\n"

#define MSGTR_UnknownAudio "Neznámý/chybějící audio formát -> nebude zvuk.\n"

#define MSGTR_UsingExternalPP "[PP] Používám externí filtr pro postprocessing, max q = %d.\n"
#define MSGTR_UsingCodecPP "[PP] Používám integrovaný postprocessing kodeku, max q = %d.\n"
#define MSGTR_VideoCodecFamilyNotAvailableStr "Požadovaná rodina video kodeku [%s] (vfm=%s) není dostupná.\nAktivujte ji při kompilaci.\n"
#define MSGTR_AudioCodecFamilyNotAvailableStr "Požadovaná rodina audio kodeku [%s] (afm=%s) není dostupná.\nAktivujte ji při kompilaci.\n"
#define MSGTR_OpeningVideoDecoder "Otevírám video dekodér: [%s] %s\n"
#define MSGTR_SelectedVideoCodec "Vybrán video kodek: [%s] vfm: %s (%s)\n"
#define MSGTR_OpeningAudioDecoder "Otevírám audio dekodér: [%s] %s\n"
#define MSGTR_SelectedAudioCodec "Vybrán audio kodek: [%s] afm: %s (%s)\n"
#define MSGTR_VDecoderInitFailed "Video dekodér - inicializace selhala :(\n"
#define MSGTR_ADecoderInitFailed "Audio dekodér - inicializace selhala :(\n"
#define MSGTR_ADecoderPreinitFailed "Audio dekodér - předinicializace selhala :(\n"

// ad_dvdpcm.c:
#define MSGTR_SamplesWanted "Vzorky tohoto formátu potřebujeme pro zlepšení podpory. Kontaktujte prosím vývojáře.\n"

// libmpcodecs/ad_libdv.c
#define MSGTR_MPCODECS_AudioFramesizeDiffers "[AD_LIBDV] Varování! Velikost rámce zvuku se liší! pře�?teno=%d  hlavi�?ka=%d.\n"

// vd.c
#define MSGTR_CodecDidNotSet "VDek: Kodek nenastavil sh->disp_w a sh->disp_h, pokouším se to obejít.\n"
#define MSGTR_CouldNotFindColorspace "Nemohu nalézt spole�?ný barevný prostor - zkouším to znovu s -vf scale...\n"
#define MSGTR_MovieAspectIsSet "Poměr stran obrazu filmu je %.2f:1 - škáluji na správný poměr.\n"
#define MSGTR_MovieAspectUndefined "Poměr stran obrazu filmu není definován - neměním velikost.\n"

// vd_dshow.c, vd_dmo.c
#define MSGTR_DownloadCodecPackage "Potřebujete aktualizovat nebo nainstalovat binární kodeky.\nJděte na http://www.mplayerhq.hu/dload.html\n"

// libmpcodecs/vd_dmo.c vd_dshow.c vd_vfw.c
#define MSGTR_MPCODECS_CouldntAllocateImageForCinepakCodec "[VD_DMO] Nemohu alokovat obraz pro kodek cinepak.\n"

// libmpcodecs/vd_ffmpeg.c
#define MSGTR_MPCODECS_XVMCAcceleratedCodec "[VD_FFMPEG] XVMC akcelerovaný kodek.\n"
#define MSGTR_MPCODECS_ArithmeticMeanOfQP "[VD_FFMPEG] Aritmetický průměr QP: %2.4f, harmonický průměr QP: %2.4f\n"
#define MSGTR_MPCODECS_DRIFailure "[VD_FFMPEG] DRI selhalo.\n"
#define MSGTR_MPCODECS_CouldntAllocateImageForCodec "[VD_FFMPEG] Nemohu alokovat obraz pro kodek.\n"
#define MSGTR_MPCODECS_XVMCAcceleratedMPEG2 "[VD_FFMPEG] XVMC-akcelerovaný MPEG-2.\n"
#define MSGTR_MPCODECS_TryingPixfmt "[VD_FFMPEG] Zkouším pixfmt=%d.\n"
#define MSGTR_MPCODECS_McGetBufferShouldWorkOnlyWithXVMC "[VD_FFMPEG] mc_get_buffer by mělo fungovat jen s XVMC akcelerací!"
#define MSGTR_MPCODECS_UnexpectedInitVoError "[VD_FFMPEG] Neo�?ekávaná chyba init_vo.\n"
#define MSGTR_MPCODECS_UnrecoverableErrorRenderBuffersNotTaken "[VD_FFMPEG] Neodstranitelná chyba, vykreslovací buffery nepoužity.\n"
#define MSGTR_MPCODECS_OnlyBuffersAllocatedByVoXvmcAllowed "[VD_FFMPEG] Povoleny jsou jen buffery alokované pomocí vo_xvmc.\n"

// libmpcodecs/ve_lavc.c
#define MSGTR_MPCODECS_HighQualityEncodingSelected "[VE_LAVC] Vybráno vysoce kvalitní kódování (nebude probíhat v reálném �?ase)!\n"
#define MSGTR_MPCODECS_UsingConstantQscale "[VE_LAVC] Používám konstantní qscale = %f (VBR).\n"

// libmpcodecs/ve_raw.c
#define MSGTR_MPCODECS_OutputWithFourccNotSupported "[VE_RAW] Surový výstup s fourcc [%x] není podporován!\n"
#define MSGTR_MPCODECS_NoVfwCodecSpecified "[VE_RAW] Požadovaný VfW kodek nebyl specifikován!\n"

// vf.c
#define MSGTR_CouldNotFindVideoFilter "Nemohu nalézt video filtr '%s'\n"
#define MSGTR_CouldNotOpenVideoFilter "Nemohu otevřít video filtr '%s'\n"
#define MSGTR_OpeningVideoFilter "Otevírám video filtr: "
#define MSGTR_CannotFindColorspace "Ani při vložení 'scale' nemohu nalézt spole�?ný barevný prostor :(\n"

// libmpcodecs/vf_crop.c
#define MSGTR_MPCODECS_CropBadPositionWidthHeight "[CROP] Špatná pozice/šířka/výška - ořezová oblast zasahuje mimo originál!\n"

// libmpcodecs/vf_cropdetect.c
#define MSGTR_MPCODECS_CropArea "[CROP] Ořezová oblast: X: %d..%d  Y: %d..%d  (-vf crop=%d:%d:%d:%d).\n"

// libmpcodecs/vf_format.c, vf_palette.c, vf_noformat.c
#define MSGTR_MPCODECS_UnknownFormatName "[VF_FORMAT] Neznámý název formátu: '%s'.\n"

// libmpcodecs/vf_framestep.c vf_noformat.c vf_palette.c vf_tile.c
#define MSGTR_MPCODECS_ErrorParsingArgument "[VF_FRAMESTEP] Chyba při zpracování argumentu.\n"

// libmpcodecs/ve_vfw.c
#define MSGTR_MPCODECS_CompressorType "Typ komprese: %.4lx\n"
#define MSGTR_MPCODECS_CompressorSubtype "Podtyp komprese: %.4lx\n"
#define MSGTR_MPCODECS_CompressorFlags "Příznaky kompresoru: %lu, verze %lu, verze ICM: %lu\n"
#define MSGTR_MPCODECS_Flags "Příznaky:"
#define MSGTR_MPCODECS_Quality " kvalita"

// libmpcodecs/vf_expand.c
#define MSGTR_MPCODECS_FullDRNotPossible "Plný DR není možný, zkouším místo něj SLICES!\n"
#define MSGTR_MPCODECS_WarnNextFilterDoesntSupportSlices  "Varování! Další filtr nepodporuje SLICES, o�?ekávejte sig11...\n"
#define MSGTR_MPCODECS_FunWhydowegetNULL "Pro�? jsme dostali NULL??\n"

// libmpcodecs/vf_test.c, vf_yuy2.c, vf_yvu9.c
#define MSGTR_MPCODECS_WarnNextFilterDoesntSupport "%s není dalším filtrem/vo podporován :(\n"

// ================================== LIBASS ====================================

// ass_bitmap.c
#define MSGTR_LIBASS_FT_Glyph_To_BitmapError "[ass] FT_Glyph_To_Bitmap chyba %d \n"
#define MSGTR_LIBASS_UnsupportedPixelMode "[ass] Nepodporovaný pixelový režim: %d\n"
#define MSGTR_LIBASS_GlyphBBoxTooLarge "[ass] Příliš velký Glyph bounding box: %dx%dpx\n"

// ass.c
#define MSGTR_LIBASS_NoStyleNamedXFoundUsingY "[ass] [%p] Varování: nenalezen styl jménem '%s', používám '%s'\n"
#define MSGTR_LIBASS_BadTimestamp "[ass] špatná �?asová zna�?ka\n"
#define MSGTR_LIBASS_BadEncodedDataSize "[ass] špatně enkódovaný rozměr dat\n"
#define MSGTR_LIBASS_FontLineTooLong "[ass] Řádek fontu je příliš dlouhý: %d, %s\n"
#define MSGTR_LIBASS_EventFormatHeaderMissing "[ass] Chybí hlavi�?ka formátu události\n"
#define MSGTR_LIBASS_ErrorOpeningIconvDescriptor "[ass] chyba otevření iconv deskriptoru.\n"
#define MSGTR_LIBASS_ErrorRecodingFile "[ass] chyba překódování souboru.\n"
#define MSGTR_LIBASS_FopenFailed "[ass] ass_read_file(%s): fopen selhalo\n"
#define MSGTR_LIBASS_FseekFailed "[ass] ass_read_file(%s): fseek selhalo\n"
#define MSGTR_LIBASS_RefusingToLoadSubtitlesLargerThan100M "[ass] ass_read_file(%s): Odmítám nahrát titulky delší než 100M\n"
#define MSGTR_LIBASS_ReadFailed "Čtení selhalo, %d: %s\n"
#define MSGTR_LIBASS_AddedSubtitleFileMemory "[ass] Přidán soubos s titulky: <memory> (%d stylů, %d událostí)\n"
#define MSGTR_LIBASS_AddedSubtitleFileFname "[ass] Přidán soubos s titulky: %s (%d stylů, %d událostí)\n"
#define MSGTR_LIBASS_FailedToCreateDirectory "[ass] Selhalo vytvoření adresáře %s\n"
#define MSGTR_LIBASS_NotADirectory "[ass] Není adresář: %s\n"

// ass_cache.c
#define MSGTR_LIBASS_TooManyFonts "[ass] Příliš mnoho fontů\n"
#define MSGTR_LIBASS_ErrorOpeningFont "[ass] Chyba otevření fontů: %s, %d\n"

// ass_fontconfig.c
#define MSGTR_LIBASS_SelectedFontFamilyIsNotTheRequestedOne "[ass] fontconfig: Vybraná rodina fontu není tou požadovanou: '%s' != '%s'\n"
#define MSGTR_LIBASS_UsingDefaultFontFamily "[ass] fontconfig_select: Používám výchozí rodinu fontu: (%s, %d, %d) -> %s, %d\n"
#define MSGTR_LIBASS_UsingDefaultFont "[ass] fontconfig_select: Používám výchozí font: (%s, %d, %d) -> %s, %d\n"
#define MSGTR_LIBASS_UsingArialFontFamily "[ass] fontconfig_select: Používám rodinu fontu 'Arial': (%s, %d, %d) -> %s, %d\n"
#define MSGTR_LIBASS_FcInitLoadConfigAndFontsFailed "[ass] FcInitLoadConfigAndFonts selhalo.\n"
#define MSGTR_LIBASS_UpdatingFontCache "[ass] Aktualizuji cache fontu.\n"
#define MSGTR_LIBASS_BetaVersionsOfFontconfigAreNotSupported "[ass] Beta verze fontconfigu nejsou podporovány.\n[ass] Aktualizujte před hlášením jakýchkoli chyb.\n"
#define MSGTR_LIBASS_FcStrSetAddFailed "[ass] FcStrSetAdd selhalo.\n"
#define MSGTR_LIBASS_FcDirScanFailed "[ass] FcDirScan selhalo.\n"
#define MSGTR_LIBASS_FcDirSave "[ass] FcDirSave selhalo.\n"
#define MSGTR_LIBASS_FcConfigAppFontAddDirFailed "[ass] FcConfigAppFontAddDir selhalo\n"
#define MSGTR_LIBASS_FontconfigDisabledDefaultFontWillBeUsed "[ass] Fontconfig zakázán, bude použit jen výchozí font.\n"
#define MSGTR_LIBASS_FunctionCallFailed "[ass] %s selhalo\n"

// ass_render.c
#define MSGTR_LIBASS_NeitherPlayResXNorPlayResYDefined "[ass] Není definována PlayResX ani PlayResY. Předpokládám 384x288.\n"
#define MSGTR_LIBASS_PlayResYUndefinedSettingY "[ass] PlayResY nedefinována, nastavuji %d.\n"
#define MSGTR_LIBASS_PlayResXUndefinedSettingX "[ass] PlayResX nedefinována, nastavuji %d.\n"
#define MSGTR_LIBASS_FT_Init_FreeTypeFailed "[ass] FT_Init_FreeType selhalo.\n"
#define MSGTR_LIBASS_Init "[ass] Init\n"
#define MSGTR_LIBASS_InitFailed "[ass] Init selhal.\n"
#define MSGTR_LIBASS_BadCommand "[ass] Špatný příkaz: %c%c\n"
#define MSGTR_LIBASS_ErrorLoadingGlyph  "[ass] Chyba nahrání glyfu.\n"
#define MSGTR_LIBASS_FT_Glyph_Stroke_Error "[ass] FT_Glyph_Stroke chyba %d \n"
#define MSGTR_LIBASS_UnknownEffectType_InternalError "[ass] Neznámý typ efektu (interní chyba)\n"
#define MSGTR_LIBASS_NoStyleFound "[ass] Žádný styl nenalezen!\n"
#define MSGTR_LIBASS_EmptyEvent "[ass] Prázdná událost!\n"
#define MSGTR_LIBASS_MAX_GLYPHS_Reached "[ass] Dosaženo MAX_GLYPHS: událost %d, start = %llu, trvání = %llu\n Text = %s\n"
#define MSGTR_LIBASS_EventHeightHasChanged "[ass] Varování! Změněna výška události!  \n"

// ass_font.c
#define MSGTR_LIBASS_GlyphNotFoundReselectingFont "[ass] Glyf 0x%X nenalezen, měním font pro (%s, %d, %d)\n"
#define MSGTR_LIBASS_GlyphNotFound "[ass] Glyf 0x%X nenalezen ve fontu pro (%s, %d, %d)\n"
#define MSGTR_LIBASS_ErrorOpeningMemoryFont "[ass] Chyba otevření fontu v paměti: %s\n"
#define MSGTR_LIBASS_NoCharmaps "[ass] řez fontu bez tabulek znaků\n"
#define MSGTR_LIBASS_NoCharmapAutodetected "[ass] žádná tabulka znaků nedetekována automaticky, zkouším první\n"


// ================================== stream ====================================

// ai_alsa.c
#define MSGTR_MPDEMUX_AIALSA_CannotSetSamplerate "Nelze nastavit vzorkovací kmito�?et.\n"
#define MSGTR_MPDEMUX_AIALSA_CannotSetBufferTime "Nelze nastavit �?as bufferu.\n"
#define MSGTR_MPDEMUX_AIALSA_CannotSetPeriodTime "Nelze nastavit �?as periody.\n"

// ai_alsa.c
#define MSGTR_MPDEMUX_AIALSA_PcmBrokenConfig "Vadná konfigurace pro toto PCM: žádná dostupná konfigurace.\n"
#define MSGTR_MPDEMUX_AIALSA_UnavailableAccessType "Nedostupný typ přístupu.\n"
#define MSGTR_MPDEMUX_AIALSA_UnavailableSampleFmt "Nedostupný formát vzorku.\n"
#define MSGTR_MPDEMUX_AIALSA_UnavailableChanCount "Nedostupný po�?et kanálů - použiji výchozí: %d\n"
#define MSGTR_MPDEMUX_AIALSA_CannotInstallHWParams "Nelze instalovat hardwarové paramametry: %s"
#define MSGTR_MPDEMUX_AIALSA_PeriodEqualsBufferSize "Nelze použít periodu odpovídající velikosti bufferu (%u == %lu)\n"
#define MSGTR_MPDEMUX_AIALSA_CannotInstallSWParams "Nelze instalovat softwarové parametry:\n"
#define MSGTR_MPDEMUX_AIALSA_ErrorOpeningAudio "Nelze otevřít audio: %s\n"
#define MSGTR_MPDEMUX_AIALSA_AlsaXRUN "ALSA xrun!!! (aspoň %.3f ms dlouhý)\n"
#define MSGTR_MPDEMUX_AIALSA_AlsaXRUNPrepareError "ALSA xrun: připravuji chybu: %s"
#define MSGTR_MPDEMUX_AIALSA_AlsaReadWriteError "ALSA chyba �?tení/zápisu"

// ai_oss.c
#define MSGTR_MPDEMUX_AIOSS_Unable2SetChanCount "Nelze nastavit po�?et kanálů: %d\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2SetStereo "Nelze nastavit stereo: %d\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2Open "Nelze otevřít '%s': %s\n"
#define MSGTR_MPDEMUX_AIOSS_UnsupportedFmt "nepodporovaný formát\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2SetAudioFmt "Nelze nastavit audio formát."
#define MSGTR_MPDEMUX_AIOSS_Unable2SetSamplerate "Nelze nastavit vzorkovací kmito�?et: %d\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2SetTrigger "Nelze nastavit spoušť: %d\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2GetBlockSize "Nelze zjistit velikost bloku!\n"
#define MSGTR_MPDEMUX_AIOSS_AudioBlockSizeZero "Velikost zvukového bloku je nulová, nastavuji ji na %d!\n"
#define MSGTR_MPDEMUX_AIOSS_AudioBlockSize2Low "Velikost zvukového bloku je příliš malá, nastavuji ji na %d!\n"

// asf_mmst_streaming.c
#define MSGTR_MPDEMUX_MMST_WriteError "chyba zápisu\n"
#define MSGTR_MPDEMUX_MMST_EOFAlert "\nVýstraha! EOF\n"
#define MSGTR_MPDEMUX_MMST_PreHeaderReadFailed "�?tení pre-hlavi�?ky selhalo\n"
#define MSGTR_MPDEMUX_MMST_InvalidHeaderSize "Špatná velikost hlavi�?ky, vzdávám to.\n"
#define MSGTR_MPDEMUX_MMST_HeaderDataReadFailed "Čtení dat hlavi�?ky selhalo.\n"
#define MSGTR_MPDEMUX_MMST_packet_lenReadFailed "Selhalo �?tení packet_len.\n"
#define MSGTR_MPDEMUX_MMST_InvalidRTSPPacketSize "Špatná velikost RTSP paketu, vzdávám to.\n"
#define MSGTR_MPDEMUX_MMST_CmdDataReadFailed "Selhalo �?tení příkazových dat.\n"
#define MSGTR_MPDEMUX_MMST_HeaderObject "hlavi�?kový objekt\n"
#define MSGTR_MPDEMUX_MMST_DataObject "datový objekt\n"
#define MSGTR_MPDEMUX_MMST_FileObjectPacketLen "souborový objekt, délka paketu = %d (%d)\n"
#define MSGTR_MPDEMUX_MMST_StreamObjectStreamID "proudový objekt, ID datového proudu: %d\n"
#define MSGTR_MPDEMUX_MMST_2ManyStreamID "Příliš mnoho ID, proud přesko�?en."
#define MSGTR_MPDEMUX_MMST_UnknownObject "neznámý objekt\n"
#define MSGTR_MPDEMUX_MMST_MediaDataReadFailed "Čtení media dat selhalo.\n"
#define MSGTR_MPDEMUX_MMST_MissingSignature "chybí signatura\n"
#define MSGTR_MPDEMUX_MMST_PatentedTechnologyJoke "Vše hotovo. Děkujeme, že jste si stáhli mediální soubor obsahující proprietární a patentovanou technologii.\n"
#define MSGTR_MPDEMUX_MMST_UnknownCmd "neznámý příkaz %02x\n"
#define MSGTR_MPDEMUX_MMST_GetMediaPacketErr "chyba get_media_packet: %s\n"
#define MSGTR_MPDEMUX_MMST_Connected "Připojeno\n"

// asf_streaming.c
#define MSGTR_MPDEMUX_ASF_StreamChunkSize2Small "Ahhhh, velikost stream_chunck je příliš malá: %d\n"
#define MSGTR_MPDEMUX_ASF_SizeConfirmMismatch "size_confirm nesouhlasí!: %d %d\n"
#define MSGTR_MPDEMUX_ASF_WarnDropHeader "Varování: zahozena hlavi�?ka ????\n"
#define MSGTR_MPDEMUX_ASF_ErrorParsingChunkHeader "Chyba při parsování hlavi�?ky chunku\n"
#define MSGTR_MPDEMUX_ASF_NoHeaderAtFirstChunk "Hlavi�?ka nedošla jako první chunk !!!!\n"
#define MSGTR_MPDEMUX_ASF_BufferMallocFailed "Chyba: nelze alokovat %d bajtů vyrovnávací paměti.\n"
#define MSGTR_MPDEMUX_ASF_ErrReadingNetworkStream "Chyba při �?tení proudu ze sítě.\n"
#define MSGTR_MPDEMUX_ASF_ErrChunk2Small "Chyba: chunk je příliš malý.\n"
#define MSGTR_MPDEMUX_ASF_ErrSubChunkNumberInvalid "Chyba: po�?et sub chunků je nesprávný.\n"
#define MSGTR_MPDEMUX_ASF_Bandwidth2SmallCannotPlay "Příliš malá přenosová rychlost, soubor nelze přehrávat!\n"
#define MSGTR_MPDEMUX_ASF_Bandwidth2SmallDeselectedAudio "Příliš malá přenosová rychlost, odvolaný audio proud.\n"
#define MSGTR_MPDEMUX_ASF_Bandwidth2SmallDeselectedVideo "Příliš malá přenosová rychlost, odvolaný video proud.\n"
#define MSGTR_MPDEMUX_ASF_InvalidLenInHeader "Nesprávná délka v ASF hlavi�?ce!\n"
#define MSGTR_MPDEMUX_ASF_ErrReadingChunkHeader "Chyba při �?tení hlavi�?ky chunku.\n"
#define MSGTR_MPDEMUX_ASF_ErrChunkBiggerThanPacket "Chyba: chunk_size > packet_size\n"
#define MSGTR_MPDEMUX_ASF_ErrReadingChunk "Chyba při �?tení chunku.\n"
#define MSGTR_MPDEMUX_ASF_ASFRedirector "=====> ASF Redirector\n"
#define MSGTR_MPDEMUX_ASF_InvalidProxyURL "neplatná proxy URL\n"
#define MSGTR_MPDEMUX_ASF_UnknownASFStreamType "neznámý typ ASF proudu\n"
#define MSGTR_MPDEMUX_ASF_Failed2ParseHTTPResponse "Selhalo parsování HTTP odpovědi.\n"
#define MSGTR_MPDEMUX_ASF_ServerReturn "Server vrátil %d:%s\n"
#define MSGTR_MPDEMUX_ASF_ASFHTTPParseWarnCuttedPragma "ASF HTTP PARSE VAROV�?N�?: Pragma %s zkrácena z %zu bajtů na %zu\n"
#define MSGTR_MPDEMUX_ASF_SocketWriteError "Chyba zápisu soketu: %s\n"
#define MSGTR_MPDEMUX_ASF_HeaderParseFailed "Selhalo parsování hlavi�?ky\n"
#define MSGTR_MPDEMUX_ASF_NoStreamFound "Nenalezen datový proud\n"
#define MSGTR_MPDEMUX_ASF_UnknownASFStreamingType "Neznámý typ ASF proudu\n"
#define MSGTR_MPDEMUX_ASF_InfoStreamASFURL "STREAM_ASF, URL: %s\n"
#define MSGTR_MPDEMUX_ASF_StreamingFailed "Selhalo, kon�?ím.\n"

// audio_in.c
#define MSGTR_MPDEMUX_AUDIOIN_ErrReadingAudio "\nChyba při �?tení audia: %s\n"
#define MSGTR_MPDEMUX_AUDIOIN_XRUNSomeFramesMayBeLeftOut "Zotaveno z cross-run, některé snímky mohly být vynechány!\n"
#define MSGTR_MPDEMUX_AUDIOIN_ErrFatalCannotRecover "Kritická chyba, nelze zotavit!\n"
#define MSGTR_MPDEMUX_AUDIOIN_NotEnoughSamples "\nNedostatek audio vzorků!\n"

// cache2.c
#define MSGTR_MPDEMUX_CACHE2_NonCacheableStream "\rTento proud nelze ukládat do vyrovnávací paměti.\n"
#define MSGTR_MPDEMUX_CACHE2_ReadFileposDiffers "!!! read_filepos se liší !!! Ohlaste tuto chybu...\n"

// network.c
#define MSGTR_MPDEMUX_NW_UnknownAF "Neznámá rodina adres %d\n"
#define MSGTR_MPDEMUX_NW_ResolvingHostForAF "Resolvuji %s pro %s...\n"
#define MSGTR_MPDEMUX_NW_CantResolv "Nelze resolvovat jméno pro %s: %s\n"
#define MSGTR_MPDEMUX_NW_ConnectingToServer "Připojuji se k serveru %s[%s]: %d...\n"
#define MSGTR_MPDEMUX_NW_CantConnect2Server "Selhalo připojení k serveru pomocí %s\n"
#define MSGTR_MPDEMUX_NW_SelectFailed "Select selhal.\n"
#define MSGTR_MPDEMUX_NW_ConnTimeout "spojení vypršelo\n"
#define MSGTR_MPDEMUX_NW_GetSockOptFailed "getsockopt selhal: %s\n"
#define MSGTR_MPDEMUX_NW_ConnectError "chyba spojení: %s\n"
#define MSGTR_MPDEMUX_NW_InvalidProxySettingTryingWithout "Špatné nastavení proxy... Zkouším bez proxy.\n"
#define MSGTR_MPDEMUX_NW_CantResolvTryingWithoutProxy "Nelze resolvovat jméno vzdáleného systému pro AF_INET. Zkouším bez proxy.\n"
#define MSGTR_MPDEMUX_NW_ErrSendingHTTPRequest "Chyba při odesílání HTTP požadavku: Nebyl odeslán celý požadavek.\n"
#define MSGTR_MPDEMUX_NW_ReadFailed "Chyba při �?tení.\n"
#define MSGTR_MPDEMUX_NW_Read0CouldBeEOF "http_read_response pře�?etlo 0 (to je EOF).\n"
#define MSGTR_MPDEMUX_NW_AuthFailed "Autentifikace selhala. Použijte volby -user a -passwd pro zadání svého\n"\
"uživatelského_jména/hesla pro seznam URL, nebo URL v následující formě:\n"\
"http://uživatelské_jméno:heslo@jméno_serveru/soubor\n"
#define MSGTR_MPDEMUX_NW_AuthRequiredFor "Pro %s je vyžadována autentifikace\n"
#define MSGTR_MPDEMUX_NW_AuthRequired "Vyžadována autentifikace.\n"
#define MSGTR_MPDEMUX_NW_NoPasswdProvidedTryingBlank "Nezadáno heslo, zkouším prázdné heslo.\n"
#define MSGTR_MPDEMUX_NW_ErrServerReturned "Server vrátil %d: %s\n"
#define MSGTR_MPDEMUX_NW_CacheSizeSetTo "Vyrovnávací paměť nastavena na %d KBajtů\n"

// open.c, stream.c:
#define MSGTR_CdDevNotfound "CD-ROM zařízení '%s' nebylo nalezeno.\n"
#define MSGTR_ErrTrackSelect "Chyba při výběru VCD stopy."
#define MSGTR_ReadSTDIN "Čtu ze std. vstupu...\n"
#define MSGTR_UnableOpenURL "Nelze otevřít URL: %s\n"
#define MSGTR_ConnToServer "Připojeno k serveru: %s\n"
#define MSGTR_FileNotFound "Soubor nenalezen: '%s'\n"

#define MSGTR_SMBInitError "Nelze inicializovat knihovnu libsmbclient: %d\n"
#define MSGTR_SMBFileNotFound "Nemohu otevřít soubor ze sítě: '%s'\n"
#define MSGTR_SMBNotCompiled "MPlayer nebyl přeložen s podporou �?tení SMB.\n"

#define MSGTR_CantOpenDVD "Nelze otevřít DVD zařízení: %s (%s)\n"

// stream_cdda.c
#define MSGTR_MPDEMUX_CDDA_CantOpenCDDADevice "Nelze otevřít CDDA zařízení.\n"
#define MSGTR_MPDEMUX_CDDA_CantOpenDisc "Nelze otevřít disk.\n"
#define MSGTR_MPDEMUX_CDDA_AudioCDFoundWithNTracks "Nalezeno audio CD s %d stopami\n"

// stream_cddb.c
#define MSGTR_MPDEMUX_CDDB_FailedToReadTOC "Selhalo �?tení TOC.\n"
#define MSGTR_MPDEMUX_CDDB_FailedToOpenDevice "Selhalo otevření zařízení %s.\n"
#define MSGTR_MPDEMUX_CDDB_NotAValidURL "neplatná URL\n"
#define MSGTR_MPDEMUX_CDDB_FailedToSendHTTPRequest "Selhalo odeslání HTTP požadavku.\n"
#define MSGTR_MPDEMUX_CDDB_FailedToReadHTTPResponse "Selhalo �?tení HTTP odpovědi.\n"
#define MSGTR_MPDEMUX_CDDB_HTTPErrorNOTFOUND "Není k dispozici.\n"
#define MSGTR_MPDEMUX_CDDB_HTTPErrorUnknown "neznámý error kód\n"
#define MSGTR_MPDEMUX_CDDB_NoCacheFound "Vyrovnávací paměť nenalezena.\n"
#define MSGTR_MPDEMUX_CDDB_NotAllXMCDFileHasBeenRead "Nebyl pře�?ten celý xmcd soubor.\n"
#define MSGTR_MPDEMUX_CDDB_FailedToCreateDirectory "Selhalo vytvoření adresáře %s.\n"
#define MSGTR_MPDEMUX_CDDB_NotAllXMCDFileHasBeenWritten "Nebyl zapsán celý xmcd soubor.\n"
#define MSGTR_MPDEMUX_CDDB_InvalidXMCDDatabaseReturned "Vrácen chybný soubor xmcd databáze.\n"
#define MSGTR_MPDEMUX_CDDB_UnexpectedFIXME "neo�?ekávané UROB-SI-S�?M\n"
#define MSGTR_MPDEMUX_CDDB_UnhandledCode "neošetřený kód\n"
#define MSGTR_MPDEMUX_CDDB_UnableToFindEOL "Nelze nalést konec řádku.\n"
#define MSGTR_MPDEMUX_CDDB_ParseOKFoundAlbumTitle "Parsování OK, nalezeno: %s\n"
#define MSGTR_MPDEMUX_CDDB_AlbumNotFound "Album nenalezeno.\n"
#define MSGTR_MPDEMUX_CDDB_ServerReturnsCommandSyntaxErr "Server vrátil: Syntaktická chyba příkazu\n"
#define MSGTR_MPDEMUX_CDDB_NoSitesInfoAvailable "Nejsou informace o sitech (serverech).\n"
#define MSGTR_MPDEMUX_CDDB_FailedToGetProtocolLevel "Selhalo získání úrovně protokolu.\n"
#define MSGTR_MPDEMUX_CDDB_NoCDInDrive "V mechanice není CD.\n"

// stream_cue.c
#define MSGTR_MPDEMUX_CUEREAD_UnexpectedCuefileLine "[bincue] Neo�?ekávaný řádek v cue souboru: %s\n"
#define MSGTR_MPDEMUX_CUEREAD_BinFilenameTested "[bincue] otestován bin soubor: %s\n"
#define MSGTR_MPDEMUX_CUEREAD_CannotFindBinFile "[bincue] Nelze nalézt bin soubor - vzdávám to.\n"
#define MSGTR_MPDEMUX_CUEREAD_UsingBinFile "[bincue] Používám bin soubor %s.\n"
#define MSGTR_MPDEMUX_CUEREAD_UnknownModeForBinfile "[bincue] neznámý režim pro bin soubor. To by se nemělo stát. Kon�?ím.\n"
#define MSGTR_MPDEMUX_CUEREAD_CannotOpenCueFile "[bincue] Nelze otevřít %s\n"
#define MSGTR_MPDEMUX_CUEREAD_ErrReadingFromCueFile "[bincue] Chyba �?tení z  %s\n"
#define MSGTR_MPDEMUX_CUEREAD_ErrGettingBinFileSize "[bincue] Chyba získání velikosti bin souboru.\n"
#define MSGTR_MPDEMUX_CUEREAD_InfoTrackFormat "stopa %02d:  format=%d  %02d:%02d:%02d\n"
#define MSGTR_MPDEMUX_CUEREAD_UnexpectedBinFileEOF "[bincue] neo�?ekávaný konec bin souboru\n"
#define MSGTR_MPDEMUX_CUEREAD_CannotReadNBytesOfPayload "[bincue] Nelze pře�?íst %d bajtů 'payloadu'.\n"
#define MSGTR_MPDEMUX_CUEREAD_CueStreamInfo_FilenameTrackTracksavail "CUE stream_open, soubor=%s, stopa=%d, dostupné stopy: %d -> %d\n"

// stream_dvd.c
#define MSGTR_DVDspeedCantOpen "Nemohu otevřít DVD zařízení pro zápis, změna DVD rychlosti vyžaduje právo zápisu.\n"
#define MSGTR_DVDrestoreSpeed "Obnovuji DVD rychlost... "
#define MSGTR_DVDlimitSpeed "Omezuji DVD rychlost na %dKB/s... "
#define MSGTR_DVDlimitFail "selhalo\n"
#define MSGTR_DVDlimitOk "úspěch\n"
#define MSGTR_NoDVDSupport "MPlayer byl zkompilován bez podpory DVD, kon�?ím.\n"
#define MSGTR_DVDnumTitles "Na tomto DVD je %d titul(ů).\n"
#define MSGTR_DVDinvalidTitle "Neplatné �?íslo DVD titulu: %d\n"
#define MSGTR_DVDnumChapters "V tomto DVD titulu je %d kapitol.\n"
#define MSGTR_DVDinvalidChapter "Neplatné �?íslo DVD kapitoly: %d\n"
#define MSGTR_DVDinvalidChapterRange "Nesprávně nastavený rozsah kapitol %s\n"
#define MSGTR_DVDinvalidLastChapter "Neplatné �?íslo poslední DVD kapitoly: %d\n"
#define MSGTR_DVDnumAngles "Tento DVD titul má %d úhlů pohledu.\n"
#define MSGTR_DVDinvalidAngle "Neplatné �?íslo DVD úhlu pohledu: %d\n"
#define MSGTR_DVDnoIFO "Nelze otevřít IFO soubor pro DVD titul %d.\n"
#define MSGTR_DVDnoVMG "Nelze otevřít VMG info!\n"
#define MSGTR_DVDnoVOBs "Nelze otevřít VOBy titulu (VTS_%02d_1.VOB).\n"
#define MSGTR_DVDnoMatchingAudio "DVD zvuk v požadovaném jazyce nebyl nalezen!\n"
#define MSGTR_DVDaudioChannel "Vybrán DVD zvukový kanál: %d jazyk: %c%c\n"
#define MSGTR_DVDaudioStreamInfo "audio proud: %d formát: %s (%s) jazyk: %s aid: %d.\n"
#define MSGTR_DVDnumAudioChannels "po�?et zvukových kanálů na disku: %d.\n"
#define MSGTR_DVDnoMatchingSubtitle "DVD titulky v požadovaném jazyce nebyly nalezeny!\n"
#define MSGTR_DVDsubtitleChannel "Vybrán DVD titulkový kanál: %d jazyk: %c%c\n"
#define MSGTR_DVDsubtitleLanguage "titulky ( sid ): %d jazyk: %s\n"
#define MSGTR_DVDnumSubtitles "po�?et sad titulků na disku: %d\n"

// stream_radio.c
#define MSGTR_RADIO_ChannelNamesDetected "[radio] Detekovány názvy stanic.\n"
#define MSGTR_RADIO_WrongFreqForChannel "[radio] Nesprávná frekvence pro stanici %s\n"
#define MSGTR_RADIO_WrongChannelNumberFloat "[radio] Nesprávné �?íslo kanálu: %.2f\n"
#define MSGTR_RADIO_WrongChannelNumberInt "[radio] Nesprávné �?íslo kanálu: %d\n"
#define MSGTR_RADIO_WrongChannelName "[radio] Nesprávné jméno kanálu: %s\n"
#define MSGTR_RADIO_FreqParameterDetected "[radio] Radio parametr detekován jako frekvence.\n"
#define MSGTR_RADIO_GetTunerFailed "[radio] Varování: ioctl get tuner selhala: %s. Nastavuji frac na %d.\n"
#define MSGTR_RADIO_NotRadioDevice "[radio] %s není rádiovým zařízením!\n"
#define MSGTR_RADIO_SetFreqFailed "[radio] ioctl set frequency 0x%x (%.2f) selhala: %s\n"
#define MSGTR_RADIO_GetFreqFailed "[radio] ioctl get frequency selhala: %s\n"
#define MSGTR_RADIO_SetMuteFailed "[radio] ioctl set mute selhala: %s\n"
#define MSGTR_RADIO_QueryControlFailed "[radio] ioctl query control selhala: %s\n"
#define MSGTR_RADIO_GetVolumeFailed "[radio] ioctl get volume selhala: %s\n"
#define MSGTR_RADIO_SetVolumeFailed "[radio] ioctl set volume selhala: %s\n"
#define MSGTR_RADIO_DroppingFrame "\n[radio] příliš špatné - zahazuji audio rámec (%d bajtů)!\n"
#define MSGTR_RADIO_BufferEmpty "[radio] grab_audio_frame: prázdná vyrovnávací paměť, �?ekám na %d bajtů dat.\n"
#define MSGTR_RADIO_AudioInitFailed "[radio] audio_in_init selhala: %s\n"
#define MSGTR_RADIO_AllocateBufferFailed "[radio] nemohu alokovat vyrovnávací paměť zvuku (blok=%d,buf=%d): %s\n"
#define MSGTR_RADIO_CurrentFreq "[radio] Sou�?asná frekvence: %.2f\n"
#define MSGTR_RADIO_SelectedChannel "[radio] Zvolený kanál: %d - %s (frekv: %.2f)\n"
#define MSGTR_RADIO_ChangeChannelNoChannelList "[radio] Nelze změnit kanál: nezadán seznam kanálů.\n"
#define MSGTR_RADIO_UnableOpenDevice "[radio] Nelze otevřít '%s': %s\n"
#define MSGTR_RADIO_InitFracFailed "[radio] init_frac selhala.\n"
#define MSGTR_RADIO_WrongFreq "[radio] Špatná frekvence: %.2f\n"
#define MSGTR_RADIO_UsingFreq "[radio] Používám frekvuenci: %.2f.\n"
#define MSGTR_RADIO_AudioInInitFailed "[radio] audio_in_init selhala.\n"
#define MSGTR_RADIO_AudioInSetupFailed "[radio] volání audio_in_setup selhalo: %s\n"
#define MSGTR_RADIO_ClearBufferFailed "[radio] Vypráznění vyrovnávací paměti selhalo: %s\n"
#define MSGTR_RADIO_StreamEnableCacheFailed "[radio] Volání do stream_enable_cache selhalo: %s\n"
#define MSGTR_RADIO_DriverUnknownStr "[radio] Neznámé jméno ovlada�?e: %s\n"
#define MSGTR_RADIO_DriverV4L2 "[radio] Používám V4Lv2 rádio rozhraní.\n"
#define MSGTR_RADIO_DriverV4L "[radio] Používám V4Lv1 rádio rozhraní.\n"
#define MSGTR_RADIO_DriverBSDBT848 "[radio] Používám *BSD BT848 rádio rozhraní.\n"

//tv.c
#define MSGTR_TV_BogusNormParameter "tv.c: norm_from_string(%s): Bogus norm parametr, nastaveno %s.\n"
#define MSGTR_TV_NoVideoInputPresent "Chyba: Žádný video vstup není k dispozici!\n"
#define MSGTR_TV_UnknownImageFormat ""\
"==================================================================\n"\
" VAROV�?N�?: VYŽADOV�?N NEZN�?M�? NEBO NETESTOVAN�? FORM�?T OBRAZU (0x%x)\n"\
" To může způsobit vadné přehrávání, nebo pád programu! Hlášení\n"\
" chyb budou ignorována. Měli byste zkusit YV12 (což je výchozí\n"\
" barevný prostor) a pře�?íst si dokumentaci!\n"\
"==================================================================\n"
#define MSGTR_TV_CannotSetNorm "Chyba: Nelze nastavit normu!\n"
#define MSGTR_TV_MJP_WidthHeight "  MJP: šířka %d výška %d\n"
#define MSGTR_TV_UnableToSetWidth "Nelze nastavit požadovanou šířku: %d\n"
#define MSGTR_TV_UnableToSetHeight "Nelze nastavit požadovanou výšku: %d\n"
#define MSGTR_TV_NoTuner "Vybraný vstup nemá tuner!\n"
#define MSGTR_TV_UnableFindChanlist "Nelze nalézt vybraný seznam kanálů! (%s)\n"
#define MSGTR_TV_ChannelFreqParamConflict "Nemůžete nastavit kmito�?et a kanál sou�?asně!\n"
#define MSGTR_TV_ChannelNamesDetected "Detekovány názvy TV kanálů.\n"
#define MSGTR_TV_NoFreqForChannel "Nelze nalézt kmito�?et pro kanál %s (%s)\n"
#define MSGTR_TV_SelectedChannel3 "Zvolený kanál: %s - %s (kmit: %.3f)\n"
#define MSGTR_TV_SelectedChannel2 "Zvolený kanál: %s (kmit: %.3f)\n"
#define MSGTR_TV_UnsupportedAudioType "Audio typu '%s (%x)' nepodporováno!\n"
#define MSGTR_TV_AvailableDrivers "Dostupné ovlada�?e:\n"
#define MSGTR_TV_DriverInfo "Zvolený ovlada�?: %s\n název: %s\n autor: %s\n popis: %s\n"
#define MSGTR_TV_NoSuchDriver "Chybí ovlada�?: %s\n"
#define MSGTR_TV_DriverAutoDetectionFailed "Autodetekce TV ovlada�?e selhala.\n"
#define MSGTR_TV_UnknownColorOption "Zadána neznámá color volba (%d)!\n"
#define MSGTR_TV_NoTeletext "Žádný teletext"
#define MSGTR_TV_Bt848IoctlFailed "tvi_bsdbt848: Volání %s ioctl selhalo. Chyba: %s\n"
#define MSGTR_TV_Bt848InvalidAudioRate "tvi_bsdbt848: Špatný vzorkovací kmito�?et zvuku. Chyba: %s\n"
#define MSGTR_TV_Bt848ErrorOpeningBktrDev "tvi_bsdbt848: Nelze otevřít bktr zařízení. Chyba: %s\n"
#define MSGTR_TV_Bt848ErrorOpeningTunerDev "tvi_bsdbt848: Nelze otevřít tuner zařízení. Chyba: %s\n"
#define MSGTR_TV_Bt848ErrorOpeningDspDev "tvi_bsdbt848: Nelze otevřít dsp zařízení. Chyba: %s\n"
#define MSGTR_TV_Bt848ErrorConfiguringDsp "tvi_bsdbt848: Konfigurace dsp selhala. Chyba: %s\n"
#define MSGTR_TV_Bt848ErrorReadingAudio "tvi_bsdbt848: Chyba �?tení audio dat. Chyba: %s\n"
#define MSGTR_TV_Bt848MmapFailed "tvi_bsdbt848: mmap selhala. Chyba: %s\n"
#define MSGTR_TV_Bt848FrameBufAllocFailed "tvi_bsdbt848: Alokace Frame bufferu selhala. Chyba: %s\n"
#define MSGTR_TV_Bt848ErrorSettingWidth "tvi_bsdbt848: Chyba nastavení šířky obrazu. Chyba: %s\n"
#define MSGTR_TV_Bt848ErrorSettingHeight "tvi_bsdbt848: Chyba nastavení výšky obrazu. Chyba: %s\n"
#define MSGTR_TV_Bt848UnableToStopCapture "tvi_bsdbt848: Nelze zastavit zachytávání. Chyba: %s\n"
#define MSGTR_TV_TTSupportedLanguages "Podporované jazyky teletextu:\n"
#define MSGTR_TV_TTSelectedLanguage "Vybraný výchozí jazyk teletextu: %s\n"
#define MSGTR_TV_ScannerNotAvailableWithoutTuner "Vyhledávání kanálů není bez tuneru možné\n"

//tvi_dshow.c
#define MSGTR_TVI_DS_UnableConnectInputVideoDecoder  "Nelze připojit zadaný vstup k video dekodéru. Chyba:0x%x\n"
#define MSGTR_TVI_DS_UnableConnectInputAudioDecoder  "Nelze připojit zadaný vstup k audio dekodéru. Chyba:0x%x\n"
#define MSGTR_TVI_DS_UnableSelectVideoFormat "tvi_dshow: Nelze zvolit video formát. Chyba:0x%x\n"
#define MSGTR_TVI_DS_UnableSelectAudioFormat "tvi_dshow: Nelze zvolit audio formát. Chyba:0x%x\n"
#define MSGTR_TVI_DS_UnableGetMediaControlInterface "tvi_dshow: Nelze získat IMediaControl rozhraní. Chyba:0x%x\n"
#define MSGTR_TVI_DS_UnableStartGraph "tvi_dshow: Nelze spustit graph! Chyba:0x%x\n"
#define MSGTR_TVI_DS_DeviceNotFound "tvi_dshow: Zařízení #%d nenalezeno\n"
#define MSGTR_TVI_DS_UnableGetDeviceName "tvi_dshow: Nelze získat název pro zařízení #%d\n"
#define MSGTR_TVI_DS_UsingDevice "tvi_dshow: Používám zařízení #%d: %s\n"
#define MSGTR_TVI_DS_DirectGetFreqFailed "tvi_dshow: Nelze získat kmito�?et přímo. Použije se tabulka kanálů z OS.\n"
//following phrase will be printed near the selected audio/video input
#define MSGTR_TVI_DS_UnableExtractFreqTable "tvi_dshow: Nelze nahrát kmito�?tovou tabulku z kstvtune.ax\n"
#define MSGTR_TVI_DS_WrongDeviceParam "tvi_dshow: Špatný parametr zařízení: %s\n"
#define MSGTR_TVI_DS_WrongDeviceIndex "tvi_dshow: Špatný index zařízení: %d\n"
#define MSGTR_TVI_DS_WrongADeviceParam "tvi_dshow: Špatný parametr audio zařízení: %s\n"
#define MSGTR_TVI_DS_WrongADeviceIndex "tvi_dshow: Špatný index audio zařízení: %d\n"

#define MSGTR_TVI_DS_SamplerateNotsupported "tvi_dshow: Vzorkovací kmito�?et %d zařízení nepodporuje. Měním na první dostupný.\n"
#define MSGTR_TVI_DS_VideoAdjustigNotSupported "tvi_dshow: Nastavování jasu/tónu/sytosti/kontrastu zařízení nepodporuje.\n"

#define MSGTR_TVI_DS_ChangingWidthHeightNotSupported "tvi_dshow: Změnu výšky/šířky videa zařízení nepodporuje.\n"
#define MSGTR_TVI_DS_SelectingInputNotSupported  "tvi_dshow: Volbu zdroje pro zachytávání zařízení nepodporuje.\n"
#define MSGTR_TVI_DS_ErrorParsingAudioFormatStruct "tvi_dshow: Nelze parsovat strukturu audio formátu.\n"
#define MSGTR_TVI_DS_ErrorParsingVideoFormatStruct "tvi_dshow: Nelze parsovat strukturu video formátu.\n"
#define MSGTR_TVI_DS_UnableSetAudioMode "tvi_dshow: Nelze nastavit audio režim %d. Chyba:0x%x\n"
#define MSGTR_TVI_DS_UnsupportedMediaType "tvi_dshow: Nepodporovaný typ média předán do %s\n"
#define MSGTR_TVI_DS_UnableGetsupportedVideoFormats "tvi_dshow: Nelze získat podporované formáty médií z video pinu. Chyba:0x%x\n"
#define MSGTR_TVI_DS_UnableGetsupportedAudioFormats "tvi_dshow: Nelze získat podporované formáty médií z audio pinu. Chyba:0x%x Disabling audio.\n"
#define MSGTR_TVI_DS_UnableFindNearestChannel "tvi_dshow: Nelze najít nejbližší kanál v systémové kmito�?tové tabulce\n"
#define MSGTR_TVI_DS_UnableToSetChannel "tvi_dshow: Nelze přepnout na nejbližší kanál ze systémové kmito�?tové tabulky. Chyba:0x%x\n"
#define MSGTR_TVI_DS_UnableTerminateVPPin "tvi_dshow: Nelze ukon�?it VideoPort pin s jakýmkoli filtrem v graphu. Chyba:0x%x\n"
#define MSGTR_TVI_DS_UnableBuildVideoSubGraph "tvi_dshow: Nelze sestavit video chain of capture graph. Chyba:0x%x\n"
#define MSGTR_TVI_DS_UnableBuildAudioSubGraph "tvi_dshow: Nelze sestavit audio chain of capture graph. Chyba:0x%x\n"
#define MSGTR_TVI_DS_UnableBuildVBISubGraph "tvi_dshow: Nelze sestavit VBI chain of capture graph. Chyba:0x%x\n"
#define MSGTR_TVI_DS_GraphInitFailure "tvi_dshow: Inicializace directshow graph selhala.\n"
#define MSGTR_TVI_DS_NoVideoCaptureDevice "tvi_dshow: Nelze najít zachytávací video zařízení\n"
#define MSGTR_TVI_DS_NoAudioCaptureDevice "tvi_dshow: Nelze najít zachytávací audio zařízení\n"
#define MSGTR_TVI_DS_GetActualMediatypeFailed "tvi_dshow: Nelze získat �?inný typ média (Chyba:0x%x). Předpokládám shodu s požadovaným.\n"

// url.c
#define MSGTR_MPDEMUX_URL_StringAlreadyEscaped "Řetězec se zdá být již eskejpován v url_escape %c%c1%c2\n"
