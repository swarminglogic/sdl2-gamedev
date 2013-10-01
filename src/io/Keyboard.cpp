#include <io/Keyboard.h>

#include <cassert>


Keyboard::Keyboard()
  : state_{false},
    stateTimestamp_{0}
{
}


Keyboard::~Keyboard()
{
}


Keyboard& Keyboard::instance()
{
  static Keyboard instance;
  return instance;
}


void Keyboard::handleEvent(const SDL_Event& event)
{
  assert(event.type == SDL_KEYDOWN || event.type == SDL_KEYUP);
  if (event.key.repeat) return;

  const bool isKeyDown = event.type == SDL_KEYDOWN;
  state_[event.key.keysym.scancode] = isKeyDown;
  stateTimestamp_[event.key.keysym.scancode] = SDL_GetTicks();
}


bool Keyboard::isDown(SDL_Scancode code) const
{
  return state_[code];
}

bool Keyboard::isUpdated(SDL_Scancode code, Uint32 timestamp) const
{
  return stateTimestamp_[code] > timestamp;
}
