#ifndef MATH_MATHUTIL_H
#define MATH_MATHUTIL_H

#include <math/Pointf.h>
#include <math/Size.h>


/**
 * MathUtil class.
 * Static functions.
 * @author SwarmingLogic (Roald Fernandez)
 */
class MathUtil
{
public:
  /**
   * Calculates the next greater or equal power of two.
   * 3 -> 4
   * 6 -> 8,
   * 9 -> 16, etc
   */
  static unsigned int nextPow2(unsigned int v);


  /**
   * Translates texture coordinates from original image texture coordinates, to
   * translated coordinates, as a result of the image not being a power of two.
   */
  static float nextPow2TexCoord(float original, int dimension);

  static Pointf nextPow2TexCoord(const Pointf& original,
                                 const Size& dimension);

private:
  MathUtil();
  virtual ~MathUtil();

  // NonCopyable
  MathUtil(const MathUtil& c);
  MathUtil& operator=(const MathUtil& c);
};

#endif
