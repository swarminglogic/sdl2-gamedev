#ifndef MATH_POINTF_H
#define MATH_POINTF_H

#include <cmath>
#include <limits>


/**
 * Pointf class. Simple class representing an float-pair.
 * Logically represents x and y.
 *
 * @note No virtual functions to avoid extra pointer to vftable.
 *       Don't inherit from this class.
 * @author SwarmingLogic (Roald Fernandez)
 */
class Pointf
{
public:
  Pointf();
  Pointf(float x, float y);
  ~Pointf();

  /**
   * @return y
   */
  float getY() const;
  void setY(float y);

  // Short hand get/set
  float y() const;
  void y(float y);


  /**
   * @return x
   */
  float getX() const;
  void setX(float x);

  // Short hand get/set
  float x() const;
  void x(float x);

  /**
   * Swaps x and y
   */
  void transpose();

  Pointf& operator+=(const Pointf& rhs);
  Pointf& operator-=(const Pointf& rhs);
  Pointf& operator*=(float rhs);
  Pointf& operator/=(float rhs);

  friend inline const Pointf operator+(const Pointf& lhs, const Pointf& rhs);
  friend inline const Pointf operator-(const Pointf& lhs, const Pointf& rhs);

  // Multiplication/division with float pointF rounds to nearest integer.
  friend inline const Pointf operator*(const Pointf& lhs, float rhs);
  friend inline const Pointf operator*(float lhs, const Pointf &rhs);
  friend inline const Pointf operator/(const Pointf& lhs, float rhs);

  friend inline bool operator==(const Pointf& lhs, const Pointf& rhs);
  friend inline bool operator!=(const Pointf& lhs, const Pointf& rhs);


private:
  float xv_;
  float yv_;
};

inline const Pointf operator+(const Pointf& lhs, const Pointf& rhs)
{return Pointf(lhs.xv_ + rhs.xv_, lhs.yv_ + rhs.yv_);}

inline const Pointf operator-(const Pointf& lhs, const Pointf& rhs)
{return Pointf(lhs.xv_ - rhs.xv_, lhs.yv_ - rhs.yv_);}

inline const Pointf operator*(const Pointf& lhs, float rhs)
{return Pointf(lhs.xv_ * rhs, lhs.yv_ * rhs);}

inline const Pointf operator*(float lhs, const Pointf &rhs)
{return Pointf(lhs * rhs.xv_, lhs * rhs.yv_);}

inline const Pointf operator/(const Pointf& lhs, float rhs)
{return Pointf(lhs.xv_ / rhs, lhs.yv_ / rhs);}

inline bool operator==(const Pointf& lhs, const Pointf& rhs)
{
  static const float eps = std::numeric_limits<float>::epsilon();
  return ((std::fabs(lhs.xv_ - rhs.xv_) < eps) &&
          (std::fabs(lhs.yv_ - rhs.yv_) < eps));
}

inline bool operator!=(const Pointf& lhs, const Pointf& rhs)
{return !(lhs == rhs);}

#endif
