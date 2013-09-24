#include <io/MouseDrag.h>

#include <cassert>


MouseDrag::MouseDrag()
  : log_("MouseDrag"),
    initial_(0, 0),
    current_(0, 0),
    isDragging_(false),
    isRelative_(false)
{
}


MouseDrag::~MouseDrag()
{
}


void MouseDrag::down(Point point)
{
  assert(!isDragging_);

  if (isRelative_) {
    initial_ = Point(0, 0);
  } else {
    initial_ = point;
    current_ = point;
  }
  isDragging_ = true;
}



bool MouseDrag::move(Point point)
{
  if (!isDragging_)
    return false;
  if (isRelative_)
    current_ += point;
  else
    current_ = point;
  return true;
}


Point MouseDrag::up(Point point)
{
  if (!isRelative_)
    current_ = point;
  isDragging_ = false;
  return get();
}


void MouseDrag::up()
{
  isDragging_ = false;
}


void MouseDrag::reset()
{
  initial_ = Point(0,0);
  current_ = Point(0,0);
}


Point MouseDrag::get() const
{
  return current_ - initial_;
}


int MouseDrag::getX() const
{
  return deltaX();
}


int MouseDrag::getY() const
{
  return deltaY();
}


int MouseDrag::deltaX() const
{
  return current_.x() - initial_.x();
}


int MouseDrag::deltaY() const
{
  return current_.y() - initial_.y();
}


bool MouseDrag::isRelative() const
{return isRelative_;}
void MouseDrag::setIsRelative(bool isRelativeEnabled)
{isRelative_ = isRelativeEnabled;}
