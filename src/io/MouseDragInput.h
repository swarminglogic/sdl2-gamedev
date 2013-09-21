#ifndef IO_MOUSEDRAGINPUT_H
#define IO_MOUSEDRAGINPUT_H

#include <io/MouseDrag.h>
#include <math/Point.h>
#include <math/Pointf.h>
#include <util/SDL_Event_fwd.h>


/**
 * MouseDragInput class.
 * Receives SDL_Events, and manages drag of left, middle and right mouse buttons.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class MouseDragInput
{
public:
  enum MouseButton{
    MB_LEFT,
    MB_RIGHT,
    MB_MIDDLE,
    MB_NONE
  };

  MouseDragInput(MouseButton button);
  MouseDragInput(MouseButton button, float scale, Pointf offset);

  virtual ~MouseDragInput();

  /**
   * Handles SDL_Events, and uses relevant information.
   *
   * @param event
   */
  bool handleEvent(const SDL_Event& event);

  /**
   * Returns drag by left mouse button.;
   */
  Point getDrag() const;

  /**
   * Returns scaled values, from setScale(float).
   * @return drag delta since last [mouse down] event
   */
  Pointf getDragScaled() const;

  // Enables/disables input..
  void enable();
  void disable();

  // Resets particular mouse button history
  void reset();

  float getScale() const;
  void setScale(float scale);

private:
  MouseButton getButton(unsigned int sdlButton) const;
  MouseButton getMotionButton(unsigned int sdlMotionState) const;

  Log log_;
  MouseButton button_;
  Point accum_;
  bool isEnabled_;
  MouseDrag mouseDrag_;
  float scale_;

  // NonCopyable
  MouseDragInput(const MouseDragInput& c);
  MouseDragInput& operator=(const MouseDragInput& c);
};

#endif
