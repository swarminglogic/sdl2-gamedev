#include <io/MouseDragInput.h>

#include <ui/SDL.h>


MouseDragInput::MouseDragInput(MouseButton button)
  :   log_("MouseDragInput"),
      button_(button),
      accum_(0, 0),
      isEnabled_(true),
      mouseDrag_(),
      scale_(1.0f)
{
}

MouseDragInput::MouseDragInput(MouseButton button, float scale, Pointf offset)
  :   log_("MouseDragInput"),
      button_(button),
      accum_(0, 0),
      isEnabled_(true),
      mouseDrag_(),
      scale_(scale)
{
  accum_ = Point(static_cast<int>(offset.x() / scale_),
                 static_cast<int>(offset.y() / scale_));
}

MouseDragInput::~MouseDragInput()
{
}


bool MouseDragInput::handleEvent(const SDL_Event& event)
{
  if (!isEnabled_) return false;

  // Initial values
  MouseButton pushedButton;

  // Acquire from SDL_event
  switch (event.type) {
  case SDL_MOUSEBUTTONUP:
    pushedButton = getButton(event.button.button);
    if (pushedButton == button_) {
      accum_ += mouseDrag_.up(Point(event.button.x, event.button.y));
      mouseDrag_.reset();
      return true;
    }
    break;

  case SDL_MOUSEBUTTONDOWN:
    pushedButton = getButton(event.button.button);
    if (pushedButton == button_) {
      mouseDrag_.down(Point(event.button.x, event.button.y));
      return true;
    }
    break;

  case SDL_MOUSEMOTION:
    pushedButton = getMotionButton(event.motion.state);
    if (pushedButton == button_)
      return mouseDrag_.move(Point(event.motion.x, event.motion.y));
    break;

  default: break;
  }
  return false;
}


MouseDragInput::MouseButton MouseDragInput::getButton(
  Uint32 sdlButton) const
{
  if (sdlButton == SDL_BUTTON_LEFT )
    return MB_LEFT;
  if (sdlButton == SDL_BUTTON_RIGHT )
    return MB_RIGHT;
  if (sdlButton == SDL_BUTTON_MIDDLE )
    return MB_MIDDLE;

  return MB_NONE;
}


MouseDragInput::MouseButton MouseDragInput::getMotionButton(
    unsigned int sdlMotionState) const
{
  if (sdlMotionState & SDL_BUTTON_LMASK)
    return MB_LEFT;
  if (sdlMotionState & SDL_BUTTON_MMASK)
    return MB_MIDDLE;
  if (sdlMotionState & SDL_BUTTON_RMASK)
    return MB_RIGHT;

  return MB_NONE;
}


Point MouseDragInput::getDrag() const
{
  return accum_ + mouseDrag_.get();
}


Pointf MouseDragInput::getDragScaled() const
{
  Point drag = getDrag();
  return Pointf(static_cast<float>(drag.x()) * scale_,
                static_cast<float>(drag.y()) * scale_);
}

void MouseDragInput::enable()
{
  isEnabled_ = true;
}


void MouseDragInput::disable()
{
  isEnabled_ = false;
  mouseDrag_.reset();
}


void MouseDragInput::reset()
{
  accum_ = Point(0, 0);
  mouseDrag_.reset();
}


float MouseDragInput::getScale() const
{return scale_;}
void MouseDragInput::setScale(float scale)
{scale_ = scale;}
