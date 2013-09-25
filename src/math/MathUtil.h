#ifndef MATH_MATHUTIL_H
#define MATH_MATHUTIL_H

/**
 * MathUtil class.
 * Static functions.
 * @author SwarmingLogic (Roald Fernandez)
 */
class MathUtil
{
public:
  static unsigned int nextPow2(unsigned int v);
private:
  MathUtil();
  virtual ~MathUtil();

  // NonCopyable
  MathUtil(const MathUtil& c);
  MathUtil& operator=(const MathUtil& c);
};

#endif
