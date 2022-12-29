streamlink --player-external-http --player-external-http-port 8080 --hls-segment-threads 4 http://api.toonamiaftermath.com:3000/est/playlist.m3u8 480p

:: NOTE: If you want PST change 'est' in the url with 'pst'

:: This script will make streamlink output two addresses, one seems static, while the other uses the current IP address
:: but the static one doesn't seem to work in WiiMC, so I use the IP one with the "search" type (see onlinemedia.xml for details.)
:: in order to quickly update if the IP changes.

:: When disconnecting make sure the "stream has ended" message appears in the command window, otherwise re-connecting in WiiMC may hang until exit.

:: Final note, this re-muxes the stream, it doesn't transcode/encode the video or audio, as such it isn't much of a CPU-hog,
:: but for WiiMC do make sure that because the Toonami Aftermath stream is 60 FPS high profile, removing deblocking (Settings->Video) may be required
:: for full-speed. Mostly depends on the content shown.
