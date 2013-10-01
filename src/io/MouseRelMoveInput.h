#ifndef IO_MOUSERELMOVEINPUT_H
#define IO_MOUSERELMOVEINPUT_H

#include <io/MouseRelMove.h>
#include <math/Point.h>
#include <math/Pointf.h>
#include <util/SDL_Event_fwd.h>


/**
 * MouseRelMoveInput class.
 * Receives SDL_Events, and manages drag of left, middle and right mouse buttons.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class MouseRelMoveInput
{
public:
  MouseRelMoveInput();
  MouseRelMoveInput(float scale, Pointf offset);

  virtual ~MouseRelMoveInput();

  /**
   * Handles SDL_Events, and uses relevant information.
   *
   * @param event
   */
  bool handleEvent(const SDL_Event& event);

  /**
   * Returns drag by left mouse button.;
   */
  Point get() const;

  /**
   * Returns scaled values, from setScale(float).
   * @return drag delta since last [mouse down] event
   */
  Pointf getScaled() const;

  // Enables/disables input..
  void enable();
  void disable();

  // Resets particular mouse button history
  void reset();

  float getScale() const;
  void setScale(float scale);

private:
  Log log_;

  Point accum_;
  bool isEnabled_;
  MouseRelMove mouseRelMove_;
  float scale_;

  // NonCopyable
  MouseRelMoveInput(const MouseRelMoveInput& c);
  MouseRelMoveInput& operator=(const MouseRelMoveInput& c);
};

#endif
