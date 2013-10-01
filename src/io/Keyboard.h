#ifndef IO_KEYBOARD_H
#define IO_KEYBOARD_H

#include <ui/SDL.h>
#include <util/SDL_Event_fwd.h>


/**
 * Keyboard class.
 * Singleton class, for keeping track of keyboard state.
 *
 * Only one actor should ever be responsible for setting it's state,
 * and any actors can poll for key-states, or even copy.
 *
 * It mantains information on last updated timestamp. This allows individual
 * actors to quickly see if if the keyboard has changed state since actor last
 * polled.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class Keyboard
{
public:
  static Keyboard& instance();
  virtual ~Keyboard();

  void handleEvent(const SDL_Event& event);

  bool isDown(SDL_Scancode code) const;
  bool isUpdated(SDL_Scancode code, Uint32 timestamp) const;

private:
  // Singleton
  Keyboard();
  std::array<bool, SDL_NUM_SCANCODES> state_;
  std::array<Uint32, SDL_NUM_SCANCODES> stateTimestamp_;

  // NonCopyable
  Keyboard(const Keyboard& c);
  Keyboard& operator=(const Keyboard& c);
};

#endif
