#ifndef MATH_POINT_H
#define MATH_POINT_H

#include <cmath>


/**
 * Point class. Simple class representing an int-pair.
 * Logically represents x and y.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class Point
{
public:
  Point();
  Point(int x, int y);
  virtual ~Point();

  /**
   * @return y
   */
  int y() const;
  void setY(int y);

  /**
   * @return x
   */
  int x() const;
  void setX(int x);

  /**
   * Swaps x and y
   */
  void transpose();

  Point& operator+=(const Point& rhs);
  Point& operator-=(const Point& rhs);
  Point& operator*=(double rhs);
  Point& operator/=(double rhs);

  friend inline const Point operator+(const Point& lhs, const Point& rhs);
  friend inline const Point operator-(const Point& lhs, const Point& rhs);

  // Multiplication/division with float point rounds to nearest integer.
  friend inline const Point operator*(const Point& lhs, double rhs);
  friend inline const Point operator*(double lhs, const Point &rhs);
  friend inline const Point operator/(const Point& lhs, double rhs);

  friend inline bool operator==(const Point& lhs, const Point& rhs);
  friend inline bool operator!=(const Point& lhs, const Point& rhs);


private:
  int x_;
  int y_;
};

inline const Point operator+(const Point& lhs, const Point& rhs)
{return Point(lhs.x_ + rhs.x_, lhs.y_ + rhs.y_);}

inline const Point operator-(const Point& lhs, const Point& rhs)
{return Point(lhs.x_ - rhs.x_, lhs.y_ - rhs.y_);}

inline const Point operator*(const Point& lhs, double rhs)
{return Point(std::round(lhs.x_ * rhs), std::round(lhs.y_ * rhs));}

inline const Point operator*(double lhs, const Point &rhs)
{return Point(std::round(lhs * rhs.x_), std::round(lhs * rhs.y_));}

inline const Point operator/(const Point& lhs, double rhs)
{return Point(std::round(lhs.x_ / rhs), std::round(lhs.y_ / rhs));}

inline bool operator==(const Point& lhs, const Point& rhs)
{return (lhs.x_ == rhs.x_) && (lhs.y_ == rhs.y_);}

inline bool operator!=(const Point& lhs, const Point& rhs)
{return (lhs.x_ != rhs.x_) || (lhs.y_ != rhs.y_);}

#endif
