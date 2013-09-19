#include <math/Sizef.h>


Sizef::Sizef()
  : widthv_(0.0f),
    heightv_(0.0f)
{}


Sizef::Sizef(float widthv, float heightv)
  : widthv_(widthv),
    heightv_(heightv)
{}


Sizef::~Sizef()
{}


float Sizef::getHeight() const
{ return heightv_; }
void Sizef::setHeight(float heightv)
{ heightv_ = heightv; }

float Sizef::height() const
{ return heightv_; }
void Sizef::height(float heightv)
{ heightv_ = heightv; }


float Sizef::getWidth() const
{ return widthv_; }
void Sizef::setWidth(float widthv)
{ widthv_ = widthv; }


float Sizef::width() const
{ return widthv_; }
void Sizef::width(float widthv)
{ widthv_ = widthv; }


void Sizef::transpose()
{
  const float temp = widthv_;
  widthv_ = heightv_;
  heightv_ = temp;
}

Sizef& Sizef::operator+=(const Sizef& rhs)
{
  heightv_ += rhs.heightv_;
  widthv_ += rhs.widthv_;
  return *this;
}

Sizef& Sizef::operator-=(const Sizef& rhs)
{
  heightv_ -= rhs.heightv_;
  widthv_ -= rhs.widthv_;
  return *this;
}

Sizef& Sizef::operator*=(float rhs)
{
  heightv_ *= rhs;
  widthv_ *= rhs;
  return *this;
}

Sizef& Sizef::operator/=(float rhs)
{
  heightv_ /= heightv_ / rhs;
  widthv_ /= widthv_ / rhs;
  return *this;
}

