#include <math/Point.h>


Point::Point()
  : x_(-1),
    y_(-1)
{
}


Point::~Point()
{
}

Point::Point(int x, int y)
  : x_(x),
    y_(y)
{
}

int Point::y() const
{
  return y_;
}

void Point::setY(int y)
{
  y_ = y;
}

int Point::x() const
{
  return x_;
}

void Point::setX(int x)
{
  x_ = x;
}

void Point::transpose()
{
  const int temp = x_;
  x_ = y_;
  y_ = temp;
}

Point& Point::operator+=(const Point& rhs)
{
  y_ += rhs.y_;
  x_ += rhs.x_;
  return *this;
}

Point& Point::operator-=(const Point& rhs)
{
  y_ -= rhs.y_;
  x_ -= rhs.x_;
  return *this;
}

Point& Point::operator*=(double rhs)
{
  y_ *= rhs;
  x_ *= rhs;
  return *this;
}

Point& Point::operator/=(double rhs)
{
  y_ /= rhs;
  x_ /= rhs;
  return *this;
}
