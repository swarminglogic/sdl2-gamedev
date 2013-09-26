#ifndef MATH_SIZE_H
#define MATH_SIZE_H

#include <cmath>


/**
 * Size class. Simple class representing an int-pair.
 * Logically represents width and height.
 *
 * @note No virtual functions to avoid extra pointer to vftable.
 *       Don't inherit from this class.
 * @author SwarmingLogic (Roald Fernandez)
 */
class Size
{
public:
  Size();
  Size(int width, int height);
  ~Size();

  // Many overloads, because I'm lazy.
  int getHeight() const;
  void setHeight(int height);
  int height() const;
  void height(int height);
  int h() const;
  void h(int height);

  int getWidth() const;
  void setWidth(int width);
  int width() const;
  void width(int width);
  int w() const;
  void w(int width);


  /**
   * Swaps width and height
   */
  void transpose();

  Size& operator+=(const Size& rhs);
  Size& operator-=(const Size& rhs);
  Size& operator*=(double rhs);
  Size& operator/=(double rhs);

  friend inline const Size operator+(const Size& lhs, const Size& rhs);
  friend inline const Size operator-(const Size& lhs, const Size& rhs);

  // Multiplication/division with float point rounds to nearest integer.
  friend inline const Size operator*(const Size& lhs, double rhs);
  friend inline const Size operator*(double lhs, const Size &rhs);
  friend inline const Size operator/(const Size& lhs, double rhs);

  friend inline bool operator==(const Size& lhs, const Size& rhs);
  friend inline bool operator!=(const Size& lhs, const Size& rhs);


private:
  int width_;
  int height_;
};

inline const Size operator+(const Size& lhs, const Size& rhs)
{return Size(lhs.width_ + rhs.width_, lhs.height_ + rhs.height_);}

inline const Size operator-(const Size& lhs, const Size& rhs)
{return Size(lhs.width_ - rhs.width_, lhs.height_ - rhs.height_);}

inline const Size operator*(const Size& lhs, double rhs)
{return Size(static_cast<int>(std::round(lhs.width_ * rhs)),
             static_cast<int>(std::round(lhs.height_ * rhs)));}

inline const Size operator*(double lhs, const Size &rhs)
{return Size(static_cast<int>(std::round(lhs * rhs.width_)),
             static_cast<int>(std::round(lhs * rhs.height_)));}

inline const Size operator/(const Size& lhs, double rhs)
{return Size(static_cast<int>(std::round(lhs.width_ / rhs)),
             static_cast<int>(std::round(lhs.height_ / rhs)));}

inline bool operator==(const Size& lhs, const Size& rhs)
{return (lhs.width_ == rhs.width_) && (lhs.height_ == rhs.height_);}

inline bool operator!=(const Size& lhs, const Size& rhs)
{return (lhs.width_ != rhs.width_) || (lhs.height_ != rhs.height_);}

#endif
