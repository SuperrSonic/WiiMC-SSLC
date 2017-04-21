/**
 * projectM -- Milkdrop-esque visualisation SDK
 *
 * Copyright (C)2003-2007 projectM Team
 * Copyright (C)2009 Rhys "Shareese" Koedijk
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * See 'LICENSE.txt' included within this release
 *
 */

#ifndef _PROJECTM_HPP
#define _PROJECTM_HPP

#include <gctypes.h>
#include <gccore.h>
#include <ogc/lwp.h>
#include <ogc/mutex.h>
#include <ogc/cond.h>

#include <string>

#include <projectM/PCM.hpp>
#include <projectM/BeatDetect.hpp>

/* projectM initialisation flags */
#define PM_FLAG_NONE                    0x0
#define PM_FLAG_DISABLE_PLAYLIST_LOAD   0x1

/* Forward declarations */
class Renderer;
class Preset;
class PresetIterator;
class PresetChooser;
class PresetLoader;
class TimeKeeper;
class Pipeline;
class PipelineContext;
class RenderItemMatcher;
class MasterRenderItemMerge;

/**
 * pm_audchar - Audio characteristics.
 */
typedef enum _pm_audchar {
    PM_AC_NONE = 0,                     /* None/disabled */
    PM_AC_BASS,                         /* Bass frequency (low) */
    PM_AC_MIDDLE,                       /* Middle frequency (mid) */
    PM_AC_TREBLE,                       /* Treble frequency (high) */
    PM_AC_VOLUME,                       /* Overall volume */
} pm_audchar;

/**
 * pm_config - projectM configuration.
 */
typedef struct _pm_config {
    int maxFPS;
    int meshX;
    int meshY;
    int textureSize;
    int windowWidth;
    int windowHeight;
    int smoothPresetDuration;
    int presetDuration;
    float beatSensitivity;
    bool aspectCorrection;
    bool shufflePresets;
    bool pulseWiiLight;
    pm_audchar pulseSource;
    std::string presetDirectory;
    std::string initialPresetName;      /* ... */
} pm_config;

/**
 * A functor class that allows users of this library to specify random preset behavior.
 */
class RandomizerFunctor
{
    public:
        RandomizerFunctor(PresetChooser & chooser) ;
        virtual ~RandomizerFunctor();
        virtual double operator() (int index);
        
    private:
        const PresetChooser & m_chooser;

};

/**
 * Audio visualisation manager.
 * Encapsulation of the ProjectM engine
 */
class projectM
{
    public:

        /**
         * projectM constructor.
         *
         * @param CONFIGFILE The location of the configuration file to load from
         * @param FLAGS Additional flags
         */
        projectM (std::string configFile, int flags = PM_FLAG_NONE);
        
        /**
         * projectM constructor.
         *
         * @param SETTINGS The configuration to use
         * @param FLAGS Additional flags
         */
        projectM (pm_config settings, int flags = PM_FLAG_NONE);
        
        virtual ~projectM ();
        
        void renderFrame();
        
        /// Occurs when active preset has switched. Switched to index is returned
        virtual void presetSwitchedEvent(bool isHardCut, unsigned int index) const {};
        
        
        /// Writes a settings configuration to the specified file
        static bool writeConfig(const std::string &filepath, const pm_config &settings);

        /// Sets preset iterator position to the passed in index
        void selectPresetPosition(unsigned int index);

        /// Plays a preset immediately
        void selectPreset(unsigned int index, bool hardCut = true);

        /// Removes a preset from the play list. If it is playing then it will continue as normal until next switch
        void removePreset(unsigned int index);

        /// Sets the randomization functor. If set to null, the traversal will move in order according to the playlist
        void setRandomizer(RandomizerFunctor * functor);

        /// Tell projectM to play a particular preset when it chooses to switch
        /// If the preset is locked the queued item will be not switched to until the lock is released
        /// Subsequent calls to this function effectively nullifies previous calls.
        void queuePreset(unsigned int index);

        /// Returns true if a preset is queued up to play next
        bool isPresetQueued() const;

        /// Removes entire playlist, The currently loaded preset will end up sticking until new presets are added
        void clearPlaylist();

        /// Turn on or off a lock that prevents projectM from switching to another preset
        void setPresetLock(bool isLocked);

        /// Returns true if the active preset is locked
        bool isPresetLocked() const;

        /// Returns index of currently active preset. In the case where the active
        /// preset was removed from the playlist, this function will return the element
        /// before active preset (thus the next in order preset is invariant with respect
        /// to the removal)
        bool selectedPresetIndex(unsigned int & index) const;

        /// Add a preset url to the play list. Appended to bottom. Returns index of preset
        unsigned int addPresetURL(const std::string & presetURL, const std::string & presetName, int rating);

        /// Insert a preset url to the play list at the suggested index.
        void insertPresetURL(unsigned int index,
                        const std::string & presetURL, const std::string & presetName, int rating);

        /// Returns true if the selected preset position points to an actual preset in the
        /// currently loaded playlist
        bool presetPositionValid() const;

        /// Returns the url associated with a preset index
        std::string getPresetURL(unsigned int index) const;

        /// Returns the preset name associated with a preset index
        std::string getPresetName ( unsigned int index ) const;

        /// Returns the rating associated with a preset index
        int getPresetRating (unsigned int index) const;

        void changePresetRating (unsigned int index, int rating);

        /// Returns the size of the play list
        unsigned int getPlaylistSize() const;

        void evaluateSecondPreset();

        inline pm_config & settings() {
            return _settings;
        }
        
        inline PCM * pcm() {
            return _pcm;
        }
        
        inline BeatDetect * beatDetect() {
            return _beatDetect;
        }

        void *thread_func(void *vptr_args);

    private:
    
        /** !!! LEAVE ME ALONE !!! **/
        
        PCM *_pcm;
        BeatDetect *_beatDetect;
        Renderer *_renderer;
        PipelineContext *_pipelineContext;
        PipelineContext *_pipelineContext2;
        pm_config _settings;
       
        /* Windowed dimensions */
        int wvw;
        int wvh;

        /* Timing information */
        int mspf;
        int timed;
        int timestart;
        int count;
        float fpsstart;
        
        PipelineContext & pipelineContext() { return *_pipelineContext; }
        PipelineContext & pipelineContext2() { return *_pipelineContext2; }
        
        unsigned initRenderToTexture();
        
        double sampledPresetDuration();
        
        void readConfig(const std::string &configFile);
        void readSettings(const pm_config &settings);
        void projectM_init();
        void projectM_reset();
        void projectM_resetGL();
        void projectM_resetTextures();
        void selectPrevious(const bool);
        void selectNext(const bool);
        void selectRandom(const bool);
        
        void projectM_initengine();
        void projectM_resetengine();

        /// Initializes preset loading / management libraries
        int initPresetTools(int gx, int gy);

        /// Deinitialize all preset related tools. Usually done before projectM cleanup
        void destroyPresetTools();

        void switchPreset(std::auto_ptr<Preset> & targetPreset);
        
        /// The current position of the directory iterator
        PresetIterator * m_presetPos;

        /// Required by the preset chooser. Manages a loaded preset directory
        PresetLoader * m_presetLoader;

        /// Provides accessor functions to choose presets
        PresetChooser * m_presetChooser;

        /// Currently loaded preset
        std::auto_ptr<Preset> m_activePreset;

        /// Destination preset when smooth preset switching
        std::auto_ptr<Preset> m_activePreset2;

        TimeKeeper *timeKeeper;

        int m_flags;

        RenderItemMatcher * _matcher;
        MasterRenderItemMerge * _merger;
        mutex_t mutex;
        cond_t condition;
        lwp_t thread;
        bool running;

        Pipeline* currentPipe;

};

#endif
