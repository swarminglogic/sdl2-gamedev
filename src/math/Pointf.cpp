#include <math/Pointf.h>


Pointf::Pointf()
  : xv_(0.0f),
    yv_(0.0f)
{}


Pointf::Pointf(float xv, float yv)
  : xv_(xv),
    yv_(yv)
{}


Pointf::~Pointf()
{}


float Pointf::getY() const
{ return yv_; }
void Pointf::setY(float yv)
{ yv_ = yv; }

float Pointf::y() const
{ return yv_; }
void Pointf::y(float yv)
{ yv_ = yv; }


float Pointf::getX() const
{ return xv_; }
void Pointf::setX(float xv)
{ xv_ = xv; }


float Pointf::x() const
{ return xv_; }
void Pointf::x(float xv)
{ xv_ = xv; }


void Pointf::transpose()
{
  const float temp = xv_;
  xv_ = yv_;
  yv_ = temp;
}

Pointf& Pointf::operator+=(const Pointf& rhs)
{
  yv_ += rhs.yv_;
  xv_ += rhs.xv_;
  return *this;
}

Pointf& Pointf::operator-=(const Pointf& rhs)
{
  yv_ -= rhs.yv_;
  xv_ -= rhs.xv_;
  return *this;
}

Pointf& Pointf::operator*=(float rhs)
{
  yv_ *= rhs;
  xv_ *= rhs;
  return *this;
}

Pointf& Pointf::operator/=(float rhs)
{
  yv_ /= yv_ / rhs;
  xv_ /= xv_ / rhs;
  return *this;
}

