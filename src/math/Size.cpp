#include <math/Size.h>


Size::Size()
  : width_(0),
    height_(0)
{}


Size::Size(int widthv, int heightv)
  : width_(widthv),
    height_(heightv)
{}


Size::~Size()
{}


int Size::getHeight() const
{ return height_; }
void Size::setHeight(int heightv)
{ height_ = heightv; }

int Size::height() const
{ return height_; }
void Size::height(int heightv)
{ height_ = heightv; }


int Size::getWidth() const
{ return width_; }
void Size::setWidth(int widthv)
{ width_ = widthv; }


int Size::width() const
{ return width_; }
void Size::width(int widthv)
{ width_ = widthv; }


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
  height_ = static_cast<int>(height_ * rhs);
  width_  = static_cast<int>(width_ * rhs);
  return *this;
}

Size& Size::operator/=(double rhs)
{
  height_ = static_cast<int>(height_ / rhs);
  width_  = static_cast<int>(width_ / rhs);
  return *this;
}

