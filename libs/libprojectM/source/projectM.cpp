/**
 * projectM -- Milkdrop-esque visualisation SDK
 * Copyright (C)2003-2004 projectM Team
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

#include "RenderItemMatcher.hpp"
#include "RenderItemMergeFunction.hpp"
#include "Common.hpp"

#include "timer.h"
#include <iostream>
#include "time.h"
#include <unistd.h>

#include "PipelineContext.hpp"
//#include <xmms/plugin.h>
#include <iostream>
#include "projectM.hpp"
#include "BeatDetect.hpp"
#include "Preset.hpp"
#include "PipelineMerger.hpp"
//#include "menu.h"
#include "PCM.hpp"                    //Sound data handler (buffering, FFT, etc.)

#include <map>

#include "Renderer.hpp"
#include "PresetChooser.hpp"
#include "IdlePreset.hpp"
#include "ConfigFile.h"
#include "TextureManager.hpp"
#include "TimeKeeper.hpp"
#include "RenderItemMergeFunction.hpp"

projectM::~projectM()
{
#ifdef USE_THREADS
    std::cout << "[projectM] thread ";
    printf("c");
    running = false;
    printf("l");
    LWP_CondSignal(condition);
    printf("e");
    LWP_MutexUnlock(mutex);
    printf("a");
    //pthread_detach(thread);
    printf("n");
    LWP_CondDestroy(condition);
    printf("u");
    LWP_MutexDestroy(mutex);
    printf("p");
    std::cout << std::endl;
#endif

    destroyPresetTools();

    if ( _renderer )
        delete ( _renderer );
    if ( _beatDetect )
        delete ( _beatDetect );
    if ( _pcm ) {
        delete ( _pcm );
        _pcm = 0;
    }

    delete(_pipelineContext);
    delete(_pipelineContext2);
}

unsigned projectM::initRenderToTexture()
{
    return _renderer->initRenderToTexture();
}

void projectM::projectM_resetTextures()
{
    _renderer->ResetTextures();
}

projectM::projectM ( std::string config_file, int flags) : 
_pcm(0), _beatDetect ( 0 ), _renderer ( 0 ), _pipelineContext(new PipelineContext()), _pipelineContext2(new PipelineContext()), m_presetPos(0), m_flags(flags)
{
    readConfig(config_file);
    projectM_reset();
    projectM_resetGL();

}

projectM::projectM(pm_config settings, int flags) : 
_pcm(0), _beatDetect ( 0 ), _renderer ( 0 ), _pipelineContext(new PipelineContext()), _pipelineContext2(new PipelineContext()), m_presetPos(0), m_flags(flags)
{
    readSettings(settings);
    projectM_reset();
    projectM_resetGL();
}

bool projectM::writeConfig(const std::string & configFile, const pm_config & settings)
{
    ConfigFile config ( configFile );

    config.add("Max FPS", settings.maxFPS);
    config.add("Mesh X", settings.meshX);
    config.add("Mesh Y", settings.meshY);
    config.add("Texture Size", settings.textureSize);
    config.add("Window Width", settings.windowWidth);
    config.add("Window Height", settings.windowHeight);
    config.add("Smooth Preset Duration", settings.smoothPresetDuration);
    config.add("Preset Duration", settings.presetDuration);
    config.add("Hard Cut Sensitivity", settings.beatSensitivity);
    config.add("Aspect Correction", settings.aspectCorrection);
    config.add("Shuffle Presets", settings.shufflePresets);
    config.add("Pulse Wii Light", settings.pulseWiiLight);
    config.add("Pulse Source", settings.pulseSource);
    config.add("Preset Directory", settings.presetDirectory);
    config.add("Initial Preset Name", settings.initialPresetName);
    
    std::fstream file(configFile.c_str());
    if (file) {
        file << config;
        return true;
    } else
        return false;
}

void projectM::readConfig (const std::string & configFile )
{
    std::cout << "[projectM] config file: " << configFile << std::endl;

    ConfigFile config ( configFile );
    _settings.maxFPS = config.read<int> ( "Max FPS", 35 );
    _settings.meshX = config.read<int> ( "Mesh X", 32 );
    _settings.meshY = config.read<int> ( "Mesh Y", 24 );
    _settings.textureSize = config.read<int> ( "Texture Size", 512 );
    _settings.windowWidth  = config.read<int> ( "Window Width", 512 );
    _settings.windowHeight = config.read<int> ( "Window Height", 512 );
    _settings.smoothPresetDuration =  config.read<int> ( "Smooth Preset Duration", 10);
    _settings.presetDuration = config.read<int> ( "Preset Duration", 15 );
    _settings.beatSensitivity = config.read<float> ( "Hard Cut Sensitivity", 10.0 );
    _settings.aspectCorrection = config.read<bool> ( "Aspect Correction", true );
    _settings.shufflePresets = config.read<bool> ( "Shuffle Presets", true);
    _settings.pulseWiiLight = config.read<bool> ( "Pulse Wii Light", true);
    _settings.pulseSource = (pm_audchar) config.read<int> ( "Pulse Source", PM_AC_BASS);
    _settings.presetDirectory = config.read<string> ( "Preset Directory", "presets" );
    _settings.initialPresetName = config.read<string> ( "Default Preset Name", "" );
    
    projectM_init ();

}


void projectM::readSettings (const pm_config & settings )
{
    _settings.maxFPS = settings.maxFPS;
    _settings.meshX = settings.meshX;
    _settings.meshY = settings.meshY;
    _settings.textureSize = settings.textureSize;
    _settings.windowWidth  = settings.windowWidth;
    _settings.windowHeight = settings.windowHeight;
    _settings.smoothPresetDuration = settings.smoothPresetDuration;
    _settings.presetDuration = settings.presetDuration;
    _settings.beatSensitivity = settings.beatSensitivity;
    _settings.aspectCorrection = settings.aspectCorrection;
    _settings.shufflePresets = settings.shufflePresets;
    _settings.pulseWiiLight = settings.pulseWiiLight;
    _settings.pulseSource = settings.pulseSource;
    _settings.presetDirectory = settings.presetDirectory;
    _settings.initialPresetName = settings.initialPresetName;
    
    projectM_init ();

}

#ifdef USE_THREADS
static void *thread_callback(void *prjm)
{
    projectM *p = (projectM *)prjm;

    p->thread_func(prjm);
    return NULL;
}

void *projectM::thread_func(void *vptr_args)
{
    LWP_MutexLock(mutex);
    //  printf("in thread: %f\n", timeKeeper->PresetProgressB());
    while (true)
    {
        LWP_CondWait(condition, mutex);
        if(!running)
        {
            LWP_MutexUnlock(mutex);
            return NULL;
        }
        evaluateSecondPreset();
    }
}
#endif

void projectM::evaluateSecondPreset()
{
    pipelineContext2().time = timeKeeper->GetRunningTime();
    pipelineContext2().frame = timeKeeper->PresetFrameB();
    pipelineContext2().progress = timeKeeper->PresetProgressB();

    m_activePreset2->Render(*_beatDetect, pipelineContext2());
}

void projectM::renderFrame()
{
    timeKeeper->UpdateTimers();
    
    /*if (timeKeeper->IsSmoothing())
    {
        printf("Smoothing A:%f, B:%f, S:%f\n", timeKeeper->PresetProgressA(), timeKeeper->PresetProgressB(), timeKeeper->SmoothRatio());
    }
    else
    {
        printf("          A:%f\n", timeKeeper->PresetProgressA());
    }*/

    mspf= ( int ) ( 1000.0/ ( float ) _settings.maxFPS ); //milliseconds per frame

    /// @bug whois is responsible for updating this now?"
    pipelineContext().time = timeKeeper->GetRunningTime();
    pipelineContext().frame = timeKeeper->PresetFrameA();
    pipelineContext().progress = timeKeeper->PresetProgressA();

    //m_activePreset->Render(*_beatDetect, pipelineContext());

    _beatDetect->detectFromSamples();

    //m_activePreset->evaluateFrame();

    //if the preset isn't locked and there are more presets
    if ( _renderer->noSwitch==false && !m_presetChooser->empty() )
    {
        //if preset is done and we're not already switching
        if ( timeKeeper->PresetProgressA()>=1.0 && !timeKeeper->IsSmoothing())
        {
            if (_settings.shufflePresets)
                selectRandom(false);
            else
                selectNext(false);
        }
        else if ((_beatDetect->vol-_beatDetect->vol_old>_settings.beatSensitivity ) &&
                 timeKeeper->CanHardCut())
        {
            // printf("Hard Cut\n");
            if (_settings.shufflePresets)
                selectRandom(true);
            else
                selectNext(true);
        }
    }

    if ( timeKeeper->IsSmoothing() && timeKeeper->SmoothRatio() <= 1.0 && !m_presetChooser->empty() )
    {

        //	 printf("start thread\n");
        assert ( m_activePreset2.get() );

        #ifdef USE_THREADS
        LWP_CondSignal(condition);
        LWP_MutexUnlock(mutex);
        #endif
        m_activePreset->Render(*_beatDetect, pipelineContext());
        
        #ifdef USE_THREADS
        LWP_MutexLock(mutex);
        #else
        evaluateSecondPreset();
        #endif

        //PresetMerger::MergePresets(m_activePreset->presetOutputs(), m_activePreset2->presetOutputs(),
        //	timeKeeper->SmoothRatio(), presetInputs.gx, presetInputs.gy);

        Pipeline pipeline;

        pipeline.setStaticPerPixel(_settings.meshX, _settings.meshY);

        assert(_matcher);
        PipelineMerger::mergePipelines( m_activePreset->pipeline(),
                                        m_activePreset2->pipeline(), pipeline, _matcher->matchResults(),
                                        *_merger, timeKeeper->SmoothRatio());

        _renderer->RenderFrame(pipeline, pipelineContext());

        //for (int i = 0; i < _matcher->matchResults().matches.size(); i++) {
            //delete(pipeline.drawables[i]);
        //}
        pipeline.drawables.clear();
    }
    else
    {
        if ( timeKeeper->IsSmoothing() && timeKeeper->SmoothRatio() > 1.0 )
        {
            //printf("End Smooth\n");
            m_activePreset = m_activePreset2;
            timeKeeper->EndSmoothing();
        }
        //printf("Normal\n");

        m_activePreset->Render(*_beatDetect, pipelineContext());
        _renderer->RenderFrame (m_activePreset->pipeline(), pipelineContext());
    }
    
    //	std::cout<< m_activePreset->absoluteFilePath()<<std::endl;
    //	_renderer->presetName = m_activePreset->absoluteFilePath();

    /*glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glClearColor(.3, .3, .3, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glBegin(GL_QUADS);
    glVertex2f(10, 10);
    glVertex2f(100, 10);
    glVertex2f(100, 100);
    glVertex2f(10, 100);
    glEnd();
    
    glFlush();
    
    gxSwapBuffers();*/
    
    count++;
    
    /** Frame-rate limiter */
    /** Compute once per preset */
    if ( this->count%100==0 )
    {
        this->_renderer->realfps=100.0/ ( ( getTicks ( &timeKeeper->startTime )-this->fpsstart ) /1000 );
        this->fpsstart=getTicks ( &timeKeeper->startTime );
    }

    int timediff = getTicks ( &timeKeeper->startTime )-this->timestart;

    if ( timediff < this->mspf )
    {
        // printf("%s:",this->mspf-timediff);
        int sleepTime = ( unsigned int ) ( this->mspf-timediff ) * 1000;
        //		DWRITE ( "usleep: %d\n", sleepTime );
        if ( sleepTime > 0 && sleepTime < 100000 )
        {
            if ( usleep ( sleepTime ) != 0 ) {}}
    }
    this->timestart=getTicks ( &timeKeeper->startTime );

}

void projectM::projectM_reset()
{
    this->mspf = 0;
    this->timed = 0;
    this->timestart = 0;
    this->count = 0;

    this->fpsstart = 0;

    projectM_resetengine();
}

void projectM::projectM_init ()
{
    double easterEgg = 0.0f; /* EASTER EGG */;
    
    /** Initialise start time */
    timeKeeper = new TimeKeeper(_settings.presetDuration,_settings.smoothPresetDuration, easterEgg);

    /** Nullify frame stash */

    /** Initialise per-pixel matrix calculations */
    /** We need to initialise this before the builtin param db otherwise bass/mid etc won't bind correctly */
    assert ( !_beatDetect );

    if (!_pcm)
        _pcm = new PCM( _settings );
    assert(pcm());
    _beatDetect = new BeatDetect ( _settings, _pcm );

    if ( _settings.maxFPS > 0 )
        mspf= ( int ) ( 1000.0/ ( float ) _settings.maxFPS );
    else mspf = 0;

    this->_renderer = new Renderer ( _settings, _beatDetect );

    running = true;

    initPresetTools(_settings.meshX, _settings.meshY);


    #ifdef USE_THREADS
    LWP_MutexInit(&mutex, true);
    LWP_CondInit(&condition);
    if (LWP_CreateThread(&thread, thread_callback, this, NULL, 0, LWP_PRIO_HIGHEST) != 0)
    {
        
        std::cerr << "[projectM] failed to allocate a thread! try building with option USE_THREADS turned off" << std::endl;;
        exit(EXIT_FAILURE);
    }
    LWP_MutexLock(mutex);
    #endif

    /// @bug order of operatoins here is busted
    //_renderer->setPresetName ( m_activePreset->name() );
    timeKeeper->StartPreset();
    assert(pcm());

}

/* Reinitializes the engine variables to a default (conservative and sane) value */
void projectM::projectM_resetengine()
{

    if ( _beatDetect != NULL )
    {
        _beatDetect->reset();
    }

    /// @bug call factory clear here?
    std::cerr << "call factory clear here?" << std::endl;
    //	abort();


}

/** Resets OpenGL state */
void projectM::projectM_resetGL ()
{

    /** Stash the new dimensions */

    _renderer->reset ();
}

int projectM::initPresetTools(int gx, int gy)
{

    /* Set the seed to the current time in seconds */
    srand ( time ( NULL ) );

    // Ensure there is no trailing slash on the end of the preset directory
    if (_settings.presetDirectory.rfind('/') == _settings.presetDirectory.size() - 1)
        _settings.presetDirectory.erase(_settings.presetDirectory.rfind('/'));

    std::string url = (m_flags & PM_FLAG_DISABLE_PLAYLIST_LOAD) ? std::string() : _settings.presetDirectory;

    if ( ( m_presetLoader = new PresetLoader ( gx, gy, url) ) == 0 )
    {
        m_presetLoader = 0;
        std::cerr << "[projectM] error allocating preset loader" << std::endl;
        return -1;
    }

    if ( ( m_presetChooser = new PresetChooser ( *m_presetLoader ) ) == 0 )
    {
        delete ( m_presetLoader );

        m_presetChooser = 0;
        m_presetLoader = 0;

        std::cerr << "[projectM] error allocating preset chooser" << std::endl;
        return -1;
    }

    // Start the iterator
    if (!m_presetPos)
        m_presetPos = new PresetIterator();

    // Initialize a preset queue position as well
    //	m_presetQueuePos = new PresetIterator();

    // Start at end ptr- this allows next/previous to easily be done from this position.
    *m_presetPos = m_presetChooser->end();

    // Load the initial preset
    std::cerr << "[projectM] Allocating initial preset..." << std::endl;
    std::string preset;
    if (_settings.initialPresetName.empty()) {
        preset = PresetFactory::IDLE_PRESET_PROTOCOL + "://" + IdlePresets::IDLE_PRESET_NAME;
    } else {
        preset = _settings.presetDirectory + "/" + _settings.initialPresetName;
    }
    m_activePreset = m_presetLoader->loadPreset(preset);
    
    _renderer->SetPipeline(m_activePreset->pipeline());

    // Case where no valid presets exist in directory. Could also mean
    // playlist initialization was deferred
    if (m_presetChooser->empty())
    {
        std::cerr << "[projectM] warning: no valid files found in preset directory \""
        << m_presetLoader->directoryName() << "\"" << std::endl;
    }

    _matcher = new RenderItemMatcher();
    _merger = new MasterRenderItemMerge();
    //_merger->add(new WaveFormMergeFunction());
    _merger->add(new ShapeMerge());
    _merger->add(new BorderMerge());
    //_merger->add(new BorderMergeFunction());

    /// @bug These should be requested by the preset factories.
    _matcher->distanceFunction().addMetric(new ShapeXYDistance());

    //std::cerr << "[projectM] Idle preset allocated." << std::endl;

    projectM_resetengine();

    //std::cerr << "[projectM] engine has been reset." << std::endl;
    return 0;
}

void projectM::destroyPresetTools()
{

    if ( m_presetPos )
        delete ( m_presetPos );

    /// @slow might not be necessary
    m_presetPos = 0;

    if ( m_presetChooser )
        delete ( m_presetChooser );

    /// @slow might not be necessary
    m_presetChooser = 0;

    if ( m_presetLoader )
        delete ( m_presetLoader );

    /// @slow might not be necessary
    m_presetLoader = 0;

}

/// @bug queuePreset case isn't handled
void projectM::removePreset(unsigned int index) {

    unsigned int chooserIndex = **m_presetPos;

    m_presetLoader->removePreset(index);


    // Case: no more presets, set iterator to end
    if (m_presetChooser->empty())
        *m_presetPos = m_presetChooser->end();

    // Case: chooser index has become one less due to removal of an index below it
    else if (chooserIndex > index) {
        chooserIndex--;
        *m_presetPos = m_presetChooser->begin(chooserIndex);
    }

    // Case: we have deleted the active preset position
    // Set iterator to end of chooser
    else if (chooserIndex == index) {
        *m_presetPos = m_presetChooser->end();
    }

}

unsigned int projectM::addPresetURL ( const std::string & presetURL, const std::string & presetName, int rating )
{
    bool restorePosition = false;

    if (*m_presetPos == m_presetChooser->end())
        restorePosition = true;

    int index = m_presetLoader->addPresetURL ( presetURL, presetName, rating);

    if (restorePosition)
        *m_presetPos = m_presetChooser->end();

    return index;
}

void projectM::selectPreset ( unsigned int index, bool hardCut)
{

    if (m_presetChooser->empty())
        return;

    if (!hardCut) {
                timeKeeper->StartSmoothing();
    }

    *m_presetPos = m_presetChooser->begin(index);

    if (!hardCut) {
        switchPreset(m_activePreset2);
    } else {
        switchPreset(m_activePreset);
        timeKeeper->StartPreset();
    }

    presetSwitchedEvent(hardCut, **m_presetPos);

}

void projectM::selectRandom(const bool hardCut) {
    
    if (m_presetChooser->empty())
        return;
    
    if (!hardCut) {
        timeKeeper->StartSmoothing();
    }
    
    *m_presetPos = m_presetChooser->weightedRandom();
    
    if (!hardCut) {
        switchPreset(m_activePreset2);
    } else {
        switchPreset(m_activePreset);
        timeKeeper->StartPreset();
    }
    
    presetSwitchedEvent(hardCut, **m_presetPos);
    
}

void projectM::selectPrevious(const bool hardCut) {
    
    if (m_presetChooser->empty())
        return;
    
    if (!hardCut) {
        timeKeeper->StartSmoothing();
    }
    
    m_presetChooser->previousPreset(*m_presetPos);
    
    if (!hardCut) {
        switchPreset(m_activePreset2);
    } else {
        switchPreset(m_activePreset);
        timeKeeper->StartPreset();
    }
    
    presetSwitchedEvent(hardCut, **m_presetPos);
    
    //      m_activePreset =  m_presetPos->allocate();
    //      _renderer->SetPipeline(m_activePreset->pipeline());
    //      _renderer->setPresetName(m_activePreset->name());
    
    //timeKeeper->StartPreset();
    
}

void projectM::selectNext(const bool hardCut) {
    
    if (m_presetChooser->empty())
        return;
    
    if (!hardCut) {
        timeKeeper->StartSmoothing();
        std::cout << "start smoothing" << std::endl;
    }
    
    std::cout << "getting next preset" << std::endl;
    m_presetChooser->nextPreset(*m_presetPos);
    
    if (!hardCut) {
        switchPreset(m_activePreset2);
    } else {
        switchPreset(m_activePreset);
        timeKeeper->StartPreset();
    }
    presetSwitchedEvent(hardCut, **m_presetPos);
    
    
}

void projectM::switchPreset(std::auto_ptr<Preset> & targetPreset) {

    targetPreset = m_presetPos->allocate();

    // Set preset name here- event is not done because at the moment this function is oblivious to smooth/hard switches
    _renderer->SetPipeline(targetPreset->pipeline());

}

void projectM::setPresetLock ( bool isLocked )
{
    _renderer->noSwitch = isLocked;
}

bool projectM::isPresetLocked() const
{
    return _renderer->noSwitch;
}

std::string projectM::getPresetURL ( unsigned int index ) const
{
    return m_presetLoader->getPresetURL(index);
}

int projectM::getPresetRating ( unsigned int index ) const
{
    return m_presetLoader->getPresetRating(index);
}

std::string projectM::getPresetName ( unsigned int index ) const
{
    return m_presetLoader->getPresetName(index);
}

void projectM::clearPlaylist ( )
{
    m_presetLoader->clear();
    *m_presetPos = m_presetChooser->end();
}

void projectM::selectPresetPosition(unsigned int index) {
    *m_presetPos = m_presetChooser->begin(index);
}

bool projectM::selectedPresetIndex(unsigned int & index) const {

    if (*m_presetPos == m_presetChooser->end())
        return false;

    index = **m_presetPos;
    return true;
}


bool projectM::presetPositionValid() const {

    return (*m_presetPos != m_presetChooser->end());
}

unsigned int projectM::getPlaylistSize() const
{
    return m_presetLoader->size();
}

void projectM:: changePresetRating (unsigned int index, int rating) {
    m_presetLoader->setRating(index, rating);
}

void projectM::insertPresetURL(unsigned int index, const std::string & presetURL, const std::string & presetName, int rating)
{
    bool atEndPosition = false;

    int newSelectedIndex = 0;


    if (*m_presetPos == m_presetChooser->end()) // Case: preset not selected
    {
        atEndPosition = true;
    }

    else if (**m_presetPos < index) // Case: inserting before selected preset
    {
        newSelectedIndex = **m_presetPos;
    }

    else if (**m_presetPos > index) // Case: inserting after selected preset
    {
        newSelectedIndex++;
    }

    else  // Case: inserting at selected preset
    {
        newSelectedIndex++;
    }

    m_presetLoader->insertPresetURL (index, presetURL, presetName, rating);

    if (atEndPosition)
        *m_presetPos = m_presetChooser->end();
    else
        *m_presetPos = m_presetChooser->begin(newSelectedIndex);


}
