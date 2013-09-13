#ifndef UTIL_TIMER_H
#define UTIL_TIMER_H

#include <SDL2/SDL.h>


/**
 * Timer class for doing timing functions.
 *
 * Has wrap-around at approx 1193 hours (49 days).
 *
 * @author Roald Fernandez
 */
class Timer
{
public:
  Timer();

  virtual ~Timer();

  void start();
  void stop();
  void pause();
  void resume();
  Uint32 reset();
  void togglePauseResume();
  void toggleStopStart();

  Uint32 getTicks() const;
  float getSeconds() const;
  bool isPaused() const;
  bool isStopped() const;
  bool isRunning() const;

private:
  enum Event{
    STOPPING = 0,
    STARTING,
    PAUSING,
    RESUMING,
    RESTARTING
  };

  enum State{
    STOPPED = 0,
    RUNNING,
    PAUSED
  };

  bool isState(State state) const;
  void eventTriggered(Event event);

  State state_;
  Uint32 ticksWhenStarted_;
  Uint32 ticksAccum_;
};

#endif
