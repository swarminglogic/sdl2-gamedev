#include <math/MathUtil.h>



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

