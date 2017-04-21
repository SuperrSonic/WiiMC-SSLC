
#include <sys/time.h>

#include "TimeKeeper.hpp"
#include "RandomNumberGenerators.hpp"

TimeKeeper::TimeKeeper(double presetDuration, double smoothDuration, double easterEgg)
  {
    _smoothDuration = smoothDuration;
    _presetDuration = presetDuration;
    _easterEgg = easterEgg;

	gettimeofday ( &this->startTime, NULL );

	UpdateTimers();
  }

  void TimeKeeper::UpdateTimers()
  {
	_currentTime = getTicks ( &startTime ) * 0.001;

	_presetFrameA++;
	_presetFrameB++;

  }

  void TimeKeeper::StartPreset()
  {
    _isSmoothing = false;
    _presetTimeA = _currentTime;
    _presetFrameA = 1;
    _presetDurationA = sampledPresetDuration();
  }
  void TimeKeeper::StartSmoothing()
  {
    _isSmoothing = true;
    _presetTimeB = _currentTime;
    _presetFrameB = 1;
    _presetDurationB = sampledPresetDuration();
  }
  void TimeKeeper::EndSmoothing()
  {
    _isSmoothing = false;
    _presetTimeA = _presetTimeB;
    _presetFrameA = _presetFrameB;
    _presetDurationA = _presetDurationB;
  }

  bool TimeKeeper::CanHardCut()
  {
    return ((_currentTime - _presetTimeA) > HARD_CUT_DELAY);
  }

  double TimeKeeper::SmoothRatio()
  {
    return (_currentTime - _presetTimeB) / _smoothDuration;
  }
  bool TimeKeeper::IsSmoothing()
  {
    return _isSmoothing;
  }

  double TimeKeeper::GetRunningTime()
  {
    return _currentTime;
  }

  double TimeKeeper::PresetProgressA()
  {
    if (_isSmoothing) return 1.0;
    else return (_currentTime - _presetTimeA) / _presetDurationA;
  }
  double TimeKeeper::PresetProgressB()
  {
    return (_currentTime - _presetTimeB) / _presetDurationB;
  }

int TimeKeeper::PresetFrameB()
  {
    return _presetFrameB;
  }

int TimeKeeper::PresetFrameA()
  {
    return _presetFrameA;
  }

double TimeKeeper::sampledPresetDuration() {
		return fmax(1, fmin(60, RandomNumberGenerators::gaussian
			(_presetDuration, _easterEgg)));
}
