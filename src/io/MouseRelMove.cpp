#include <io/MouseRelMove.h>

#include <cassert>


MouseRelMove::MouseRelMove()
  : log_("MouseRelMove"),
    accum_(0, 0)
{
}

MouseRelMove::~MouseRelMove()
{
}

void MouseRelMove::move(Point point)
{
  accum_ += point;
}


Point MouseRelMove::reset()
{
  Point tmp = accum_;
  accum_ = Point(0, 0);
  return tmp;
}


Point MouseRelMove::get() const
{
  return accum_;
}


int MouseRelMove::getX() const
{
  return accum_.x();
}


int MouseRelMove::getY() const
{
  return accum_.y();
}

