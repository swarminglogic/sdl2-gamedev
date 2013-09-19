#include <math/Point.h>


Point::Point()
  : xv_(-1),
    yv_(-1)
{}


Point::Point(int xv, int yv)
  : xv_(xv),
    yv_(yv)
{}


Point::~Point()
{}


int Point::getY() const
{ return yv_; }
void Point::setY(int yv)
{ yv_ = yv; }

int Point::y() const
{ return yv_; }
void Point::y(int yv)
{ yv_ = yv; }


int Point::getX() const
{ return xv_; }
void Point::setX(int xv)
{ xv_ = xv; }


int Point::x() const
{ return xv_; }
void Point::x(int xv)
{ xv_ = xv; }


void Point::transpose()
{
  const int temp = xv_;
  xv_ = yv_;
  yv_ = temp;
}

Point& Point::operator+=(const Point& rhs)
{
  yv_ += rhs.yv_;
  xv_ += rhs.xv_;
  return *this;
}

Point& Point::operator-=(const Point& rhs)
{
  yv_ -= rhs.yv_;
  xv_ -= rhs.xv_;
  return *this;
}

Point& Point::operator*=(double rhs)
{
  yv_ = static_cast<int>(yv_ * rhs);
  xv_  = static_cast<int>(xv_ * rhs);
  return *this;
}

Point& Point::operator/=(double rhs)
{
  yv_ = static_cast<int>(yv_ / rhs);
  xv_  = static_cast<int>(xv_ / rhs);
  return *this;
}

