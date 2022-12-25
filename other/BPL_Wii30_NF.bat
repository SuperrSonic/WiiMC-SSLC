for %%A IN (*.mkv) do (
ffmpeg -i "%%~A" -vf scale='640:480:flags=lanczos',setdar='16/9' -vcodec libx264 -profile:v baseline -level:v 3.0 -vb 1350k -pass 1 -refs 3 -me_method umh -subq 9 -me_range 24 -g 60 -keyint_min 31 -sc_threshold 0 -mixed-refs 1 -rc-lookahead 60 -qcomp 0.50 -qmin 8 -qmax 51 -pix_fmt yuv420p -x264opts "no-deblock:stitchable:vbv-maxrate=2025:vbv-bufsize=2700" -an -f null -
ffmpeg -i "%%~A" -vf scale='640:480:flags=lanczos',setdar='16/9' -vcodec libx264 -profile:v baseline -level:v 3.0 -vb 1350k -pass 2 -refs 3 -me_method umh -subq 9 -me_range 24 -g 60 -keyint_min 31 -sc_threshold 0 -mixed-refs 1 -rc-lookahead 60 -qcomp 0.50 -qmin 8 -qmax 51 -pix_fmt yuv420p -x264opts "no-deblock:stitchable:vbv-maxrate=2025:vbv-bufsize=2700" -codec:a copy "%%~A_wii.mkv"
echo  Converting video for Wii...
)
:: Emulate Netflix BPL 3.0 - used for Wii based on ~2018 revision
::  Parameter Checklist:
:: 1. scale    = adjust as needed, lanczos is good for downscaling
:: 2. setdar   = adjust as needed
:: 3. bitrate  = adjust as needed
:: 4. refs     = adjust as needed
:: 5. g        = keyint, double the FPS in int (29.97 becomes 30 * 2 = 60)
:: 6. minkey   = keyint - FPS as int + 1
:: 7. rc-look  = keyint
:: 8. vbv-buf  = bitrate * 2
:: 9. vbv-rate = vbv-buf * 0.75

:: NOTE 1 - Wii app did not support deblock, despite the videos using it
::  this created annoying artifacts in bitrate starved vids, a simple fix
::  is to just disable deblocking, as done here.

:: NOTE 2 - Wii app used OGG VORBIS at 64 KBPS and 128 KBPS in stereo
::  while this seems correct, because it was encrypted, it's unknown
::  what samplerate was used, naturally it would be 48kHz like other
::  NF audio profiles, but this info might've been stored in the file headers
::  and it seems to be likly that it used 44100 Hz, despite this 48kHz would be
::  way more ideal, because other rates need to be resampled which costs CPU.

:: in any case: -acodec libvorbis -ab 128k -ac 2 -ar 48000

:: NOTE 3 - Since this is a very fast profile on Wii, you could enable more ref frames
::  or 8x8dct, but note that 8x8dct will enable high profile and some settings will change.
::  Only enable deblock for less than 30 fps, it's too slow otherwise.

:: NOTE 4 - How are these settings known if the BPL profile is encrypted?
::  with the encryption, we can only know the frame size, fps, bitrate and number of ref frames
::  BUT the BPL profile is used for trailers, teasers, previews, and shorts, which are NOT encrypted
::  anyone who can request a specific profile can access these encodes from a browser
::  But what if these files are different from the ACTUAL content used by the app??
::  It's possible, but highly unlikely. On the other hand keeping track of recipe revisions
::  is a problem, when NF moved away from the classic bitrate ladder, the Wii app could load
::  main profile up to 750 kbps 30fps, on the old ladder this was always 512x384 but after
::  the changes this could typically be the max MPL size of 720x480, and framedrops were possible.
::  However, the baseline profile remained quite consistent after many years.
PAUSE