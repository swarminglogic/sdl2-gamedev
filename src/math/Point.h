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
  int getY() const;
  void setY(int y);

  // Short hand get/set
  int y() const;
  void y(int y);


  /**
   * @return x
   */
  int getX() const;
  void setX(int x);

  // Short hand get/set
  int x() const;
  void x(int x);

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
  int xv_;
  int yv_;
};

inline const Point operator+(const Point& lhs, const Point& rhs)
{return Point(lhs.xv_ + rhs.xv_, lhs.yv_ + rhs.yv_);}

inline const Point operator-(const Point& lhs, const Point& rhs)
{return Point(lhs.xv_ - rhs.xv_, lhs.yv_ - rhs.yv_);}

inline const Point operator*(const Point& lhs, double rhs)
{return Point(static_cast<int>(std::round(lhs.xv_ * rhs)),
              static_cast<int>(std::round(lhs.yv_ * rhs)));}

inline const Point operator*(double lhs, const Point &rhs)
{return Point(static_cast<int>(std::round(lhs * rhs.xv_)),
              static_cast<int>(std::round(lhs * rhs.yv_)));}

inline const Point operator/(const Point& lhs, double rhs)
{return Point(static_cast<int>(std::round(lhs.xv_ / rhs)),
              static_cast<int>(std::round(lhs.yv_ / rhs)));}

inline bool operator==(const Point& lhs, const Point& rhs)
{return (lhs.xv_ == rhs.xv_) && (lhs.yv_ == rhs.yv_);}

inline bool operator!=(const Point& lhs, const Point& rhs)
{return (lhs.xv_ != rhs.xv_) || (lhs.yv_ != rhs.yv_);}

#endif
