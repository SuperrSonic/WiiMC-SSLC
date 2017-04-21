/**
 * main.cpp - projectM visualisation example.
 *
 * Copyright (c) 2009 Rhys "Shareese" Koedijk
 *
 * This program/include file is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program/include file is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <gctypes.h>
#include <gccore.h>
#include <wiiuse/wpad.h>

#include <fat.h>

#include <projectM.hpp>
#include "oggplayer.h"

#include <cstdlib>
#include <cstdio>
#include <string>

#define AUDIO_FILE          "sd:/audio/Karsten Koch - Leaving All Behind.ogg"
#define PRESET_DIRECTORY    "sd:/presets"
#define PRESET_NAME         "Geiss - Cosmic Dust 2.milk"

static pm_config config;
static projectM *pm = NULL;

void pcmDecode(u32 *pcm, u32 samples)
{
    // Queue the PCM data for beat analysis
    pm->pcm()->addPCM32Data(pcm, samples);
}

int main(int argc, char **argv)
{
    // Initialise the video sytem
    // TODO: This, once projectM display is sorted out
    
    // Initialise the audio sytem
    AUDIO_Init(NULL);
    
    // Initialise the attached controllers
    WPAD_Init();
    
    // Initialise the attached FAT devices
    fatInitDefault();

    // Build our projectM settings
    config.maxFPS = 35;                         // Maximum frames per second
    config.meshX = 32;                          // Width of PerPixel equation mesh
    config.meshY = 24;                          // Height of PerPixel equation mesh
    config.textureSize = 512;                   // Size of internal rendering texture
    config.windowWidth = 512;                   // Display width (in pixels)
    config.windowHeight = 512;                  // Display height (in pixels)
    config.smoothPresetDuration = 10;           // Preset transition time (in seconds)
    config.presetDuration = 15;                 // Preset durtation (in seconds)
    config.beatSensitivity = 10.0f;             // Lower to make hard cuts more frequent
    config.aspectCorrection = true;             // Custom shape aspect correction
    config.shufflePresets = true;               // Preset shuffling
    config.pulseWiiLight = true;                // Pulse the wii disc slot light in time with the beat
    config.pulseSource = PM_AC_VOLUME;          // The audio characteristic to use for pulsing
    config.presetDirectory = PRESET_DIRECTORY;  // Location of preset directory
    config.initialPresetName = PRESET_NAME;     // Initial preset name
    
    // Initialise projectM
    pm = new projectM(config);
    if (!pm)
        exit(0);
    
    // Play our audio file
    if (!oggPlay(AUDIO_FILE, pcmDecode))
        exit(0);
        
    while (1) {
        
        // Read the latest controller states
        WPAD_ScanPads();
        
        // Get the buttons which were pressed this frame
        u32 pressed = WPAD_ButtonsDown(0);
        
        // Quit if 'HOME' was pressed
        if (pressed & WPAD_BUTTON_HOME)
            break;
        
        // Toggle the wii disc slot light pulsing if '1' was pressed
        if ((pressed & WPAD_BUTTON_1))
            pm->settings().pulseWiiLight = !pm->settings().pulseWiiLight;
        
        // Update the audio buffers
        if (!oggUpdate())
            break;
        
        // Render the next frame in the visualisation/preset
        pm->renderFrame();
        
    }
    
    // Stop our audio file
    oggStop();
    
    // Destroy projectM
    if (pm)
        free(pm);

    // We return to the launcher application via exit
    exit(0);
    
    return 0;
}
