#ifndef MATH_SIZEF_H
#define MATH_SIZEF_H

#include <cmath>
#include <limits>


/**
 * Sizef class. Simple class representing an float-pair.
 * Logically represents width and height.
 *
 * @note No virtual functions to avoid extra pointer to vftable.
 *       Don't inherit from this class.
 * @author SwarmingLogic (Roald Fernandez)
 */
class Sizef
{
public:
  Sizef();
  Sizef(float width, float height);
  ~Sizef();

  /**
   * @return height
   */
  float getHeight() const;
  void setHeight(float height);

  // Short hand get/set
  float height() const;
  void height(float height);


  /**
   * @return width
   */
  float getWidth() const;
  void setWidth(float width);

  // Short hand get/set
  float width() const;
  void width(float width);

  /**
   * Swaps width and height
   */
  void transpose();

  Sizef& operator+=(const Sizef& rhs);
  Sizef& operator-=(const Sizef& rhs);
  Sizef& operator*=(float rhs);
  Sizef& operator/=(float rhs);

  friend inline const Sizef operator+(const Sizef& lhs, const Sizef& rhs);
  friend inline const Sizef operator-(const Sizef& lhs, const Sizef& rhs);

  // Multiplication/division with float sizef rounds to nearest integer.
  friend inline const Sizef operator*(const Sizef& lhs, float rhs);
  friend inline const Sizef operator*(float lhs, const Sizef &rhs);
  friend inline const Sizef operator/(const Sizef& lhs, float rhs);

  friend inline bool operator==(const Sizef& lhs, const Sizef& rhs);
  friend inline bool operator!=(const Sizef& lhs, const Sizef& rhs);


private:
  float widthv_;
  float heightv_;
};

inline const Sizef operator+(const Sizef& lhs, const Sizef& rhs)
{return Sizef(lhs.widthv_ + rhs.widthv_, lhs.heightv_ + rhs.heightv_);}

inline const Sizef operator-(const Sizef& lhs, const Sizef& rhs)
{return Sizef(lhs.widthv_ - rhs.widthv_, lhs.heightv_ - rhs.heightv_);}

inline const Sizef operator*(const Sizef& lhs, float rhs)
{return Sizef(lhs.widthv_ * rhs, lhs.heightv_ * rhs);}

inline const Sizef operator*(float lhs, const Sizef &rhs)
{return Sizef(lhs * rhs.widthv_, lhs * rhs.heightv_);}

inline const Sizef operator/(const Sizef& lhs, float rhs)
{return Sizef(lhs.widthv_ / rhs, lhs.heightv_ / rhs);}

inline bool operator==(const Sizef& lhs, const Sizef& rhs)
{
  static const float eps = std::numeric_limits<float>::epsilon();
  return ((std::fabs(lhs.widthv_ - rhs.widthv_) < eps) &&
          (std::fabs(lhs.heightv_ - rhs.heightv_) < eps));
}

inline bool operator!=(const Sizef& lhs, const Sizef& rhs)
{return !(lhs == rhs);}

#endif
