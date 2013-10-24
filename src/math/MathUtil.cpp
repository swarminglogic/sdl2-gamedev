#include <math/MathUtil.h>

#include <algorithm>
#include <cassert>


float MathUtil::lerp(float min, float max, float a)
{
  assert(min <= max);
  a = clamp(a);
  return min + (max - min) * a;
}


float MathUtil::clamp(float value, float min, float max)
{
  return std::min(std::max(value, min), max);
}


float MathUtil::clamp(float value)
{
  return clamp(value, 0.0f, 1.0f);
}


unsigned int MathUtil::nextPow2(unsigned int v)
{
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v++;
  return v;
}

unsigned int MathUtil::nextSquare(unsigned int v)
{
  unsigned int base = nextSquareBase(v);
  return base*base;
}


unsigned int MathUtil::nextSquareBase(unsigned int v)
{
  unsigned int base =
    static_cast<unsigned int>(std::ceil(std::sqrt(static_cast<float>(v))));
  return base;
}


unsigned int MathUtil::prevSquare(unsigned int v)
{
  unsigned int base = prevSquareBase(v);
  return base*base;
}


unsigned int MathUtil::prevSquareBase(unsigned int v)
{
  if (v == 0) return 0u;
  unsigned int base =
    static_cast<unsigned int>(std::floor(std::sqrt(static_cast<float>(v))));
  return base;
}


bool MathUtil::isPow2(unsigned int v)
{
  return v && !(v & (v - 1));
}


bool MathUtil::isSquare(unsigned int v)
{
  return v == nextSquare(v);
}


float MathUtil::nextPow2TexCoord(float original, int dimension)
{
  const float ratio = (static_cast<float>(dimension) /
                       static_cast<float>(nextPow2(dimension)));
  return original * ratio;
}


Pointf MathUtil::nextPow2TexCoord(const Pointf& original,
                                  const Size& dimension)
{
  return Pointf(nextPow2TexCoord(original.x(), dimension.w()),
                nextPow2TexCoord(original.y(), dimension.h()));
}
