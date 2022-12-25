for %%A IN (*.mkv) do (
ffmpeg -i "%%~A" -vf scale='640:480:flags=lanczos',setdar='16/9' -map 0:s? -vcodec libx264 -profile:v baseline -level:v 3.0 -crf 20 -refs 1 -me_method umh -subq 9 -me_range 24 -g 60 -keyint_min 31 -sc_threshold 0 -mixed-refs 1 -rc-lookahead 60 -qcomp 0.50 -qmin 8 -qmax 51 -pix_fmt yuv420p -x264opts "no-deblock:stitchable:vbv-maxrate=5250:vbv-bufsize=7000" -codec:a copy -codec:s copy "%%~A_wii.mkv"
echo  Converting video for Wii...
::  Parameter Checklist:
:: 1. scale  = adjust as needed, lanczos is good for downscaling
:: 2. setdar = adjust as needed
:: 3. crf    = adjust as needed, 22 to 16 usually enough for decent to great quality
:: 4. g      = keyint, double the FPS in int (29.97 becomes 30 * 2 = 60)
:: 5. minkey = keyint - FPS as int + 1
:: 6. rc-look= keyint
:: 7. vbv    = adjust as needed
:: NOTE - some ffmpeg versions might not like the -map setting for copying subs, remove if needed.

:: NOTE 2 - I like to target 720x480 for ~24 fps videos and 640x480 for ~30 fps
:: I try to support text subs using libass, this usually adds a big overhead to decoding
:: so I try to avoid things like deblocking and too many reference frames
:: if MEM1 is filled performance will be really bad, so that's another reason to stick to few
:: ref frames to avoid using too much memory.
::
:: It's also worth noting that even though it's faster to set a CRF value, it's also harder
:: to control with vbv limits, and so it may be the case that 2-pass would be preferred for better performance.
)