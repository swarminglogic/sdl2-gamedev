#include <util/Size.h>


Size::Size()
  : width_(-1),
    height_(-1)
{
}


Size::~Size()
{
}

Size::Size(int width, int height)
  : width_(width),
    height_(height)
{
}

int Size::height() const
{
  return height_;
}

void Size::setHeight(int height)
{
  height_ = height;
}

int Size::width() const
{
  return width_;
}

void Size::setWidth(int width)
{
  width_ = width;
}

void Size::transpose()
{
  const int temp = width_;
  width_ = height_;
  height_ = temp;
}

Size& Size::operator+=(const Size& rhs)
{
  height_ += rhs.height_;
  width_ += rhs.width_;
  return *this;
}

Size& Size::operator-=(const Size& rhs)
{
  height_ -= rhs.height_;
  width_ -= rhs.width_;
  return *this;
}

Size& Size::operator*=(double rhs)
{
  height_ *= rhs;
  width_ *= rhs;
  return *this;
}

Size& Size::operator/=(double rhs)
{
  height_ /= rhs;
  width_ /= rhs;
  return *this;
}
