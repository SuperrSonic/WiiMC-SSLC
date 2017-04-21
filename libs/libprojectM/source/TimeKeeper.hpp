#ifndef TimeKeeper_HPP
#define TimeKeeper_HPP

#include "timer.h"

#define HARD_CUT_DELAY 3

class TimeKeeper
{

public:

  TimeKeeper(double presetDuration, double smoothDuration, double easterEgg);

  void UpdateTimers();

  void StartPreset();
  void StartSmoothing();
  void EndSmoothing();
 
  bool CanHardCut();

  double SmoothRatio();
  bool IsSmoothing();

  double GetRunningTime(); 

  double PresetProgressA();
  double PresetProgressB();

  int PresetFrameA();
  int PresetFrameB();

  double sampledPresetDuration();

  /* The first ticks value of the application */
  struct timeval startTime;

private:

  double _easterEgg;
  double _presetDuration;
  double _presetDurationA;
  double _presetDurationB;
  double _smoothDuration;

  double _currentTime;
  double _presetTimeA;
  double _presetTimeB;
  int _presetFrameA;
  int _presetFrameB;

  bool _isSmoothing;
  

};
#endif
