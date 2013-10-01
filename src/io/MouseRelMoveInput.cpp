#include <io/MouseRelMoveInput.h>

#include <cassert>

#include <ui/SDL.h>


MouseRelMoveInput::MouseRelMoveInput()
  :   MouseRelMoveInput(1.0f, Pointf(0.0f, 0.0f))
{
}

MouseRelMoveInput::MouseRelMoveInput(float scale, Pointf offset)
  :   log_("MouseRelMoveInput"),
      accum_(static_cast<int>(offset.x() / scale),
             static_cast<int>(offset.y() / scale)),
      isEnabled_(true),
      mouseRelMove_(),
      scale_(scale)
{
}

MouseRelMoveInput::~MouseRelMoveInput()
{
}


bool MouseRelMoveInput::handleEvent(const SDL_Event& event)
{
  if (!isEnabled_) return false;
  if (!SDL_GetRelativeMouseMode())
    return false;

  if (event.type == SDL_MOUSEMOTION) {
    mouseRelMove_.move(Point(event.motion.xrel, event.motion.yrel));
    return true;
  } else {
    return false;
  }
}


Point MouseRelMoveInput::get() const
{
  return accum_ + mouseRelMove_.get();
}


Pointf MouseRelMoveInput::getScaled() const
{
  Point rel = get();
  return Pointf(static_cast<float>(rel.x()) * scale_,
                static_cast<float>(rel.y()) * scale_);
}

void MouseRelMoveInput::enable()
{
  isEnabled_ = true;
}


void MouseRelMoveInput::disable()
{
  isEnabled_ = false;
  mouseRelMove_.reset();
}


void MouseRelMoveInput::reset()
{
  accum_ = Point(0, 0);
  mouseRelMove_.reset();
}


float MouseRelMoveInput::getScale() const
{return scale_;}
void MouseRelMoveInput::setScale(float scale)
{scale_ = scale;}
