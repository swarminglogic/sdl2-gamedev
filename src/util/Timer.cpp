#include <util/Timer.h>

#include <cassert>


Timer::Timer()
  : state_(STOPPED),
    ticksWhenStarted_(0),
    ticksAccum_(0)
{
}


Timer::~Timer()
{
}


void Timer::eventTriggered(Event event)
{
  // Global events
  if (event == RESTARTING) {
    state_ = STOPPED;
    return;
  }

  // State dependent events
  switch(state_) {
  case STOPPED:
    if (event == STARTING) {
      ticksWhenStarted_ = SDL_GetTicks();
      ticksAccum_ = 0;
      state_ = RUNNING;
    }
    break;
  case RUNNING:
    if (event == STOPPING) {
      state_ = STOPPED;
    }
    else if (event == PAUSING) {
      ticksAccum_ += (SDL_GetTicks() - ticksWhenStarted_);
      state_ = PAUSED;
    }
    break;
  case PAUSED:
    if (event == STOPPING)
      state_ = STOPPED;
    else if (event == RESUMING) {
      ticksWhenStarted_ = SDL_GetTicks();
      state_ = RUNNING;
    }
    break;
  }
}

void Timer::start()
{
  eventTriggered(STARTING);
}


void Timer::stop()
{
  eventTriggered(STOPPING);
}


void Timer::pause()
{
  eventTriggered(PAUSING);
}


void Timer::resume()
{
  eventTriggered(RESUMING);
}


Uint32 Timer::reset()
{
  const int ticks = getTicks();
  eventTriggered(RESTARTING);
  return ticks;
}


void Timer::togglePauseResume()
{
  if (isState(STOPPED))
    return;
  else if (isState(RUNNING))
    eventTriggered(PAUSING);
  else if (isState(PAUSED))
    eventTriggered(RESUMING);
}


void Timer::toggleStopStart()
{
  if (isState(STOPPED))
    eventTriggered(STARTING);
  else
    eventTriggered(STOPPING);
}


Uint32 Timer::getTicks() const
{
  if (isState(STOPPED))
    return 0;
  else if (isState(PAUSED))
    return ticksAccum_;
  else if (isState(RUNNING))
    return ticksAccum_ + (SDL_GetTicks() - ticksWhenStarted_);

  return 0;
}

float Timer::getSeconds() const
{
  return static_cast<float>(getTicks()) / 1000.0f;
}


bool Timer::isPaused() const
{
  return isState(PAUSED);
}


bool Timer::isStopped() const
{
  return isState(STOPPED);
}


bool Timer::isRunning() const
{
  return isState(RUNNING);
}

bool Timer::isState(State state) const
{
  return (state_ == state);
}
