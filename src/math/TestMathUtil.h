#ifndef MATH_TESTMATHUTIL_H
#define MATH_TESTMATHUTIL_H

#include <math/MathUtil.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the MathUtil class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class TestMathUtil : public CxxTest::TestSuite
{
public:
  void testNextPow2A()
  {
    TS_ASSERT_EQUALS(MathUtil::nextPow2(1), 1);
    TS_ASSERT_EQUALS(MathUtil::nextPow2(2), 2);
    TS_ASSERT_EQUALS(MathUtil::nextPow2(3), 4);
    TS_ASSERT_EQUALS(MathUtil::nextPow2(4), 4);
    TS_ASSERT_EQUALS(MathUtil::nextPow2(5), 8);
    TS_ASSERT_EQUALS(MathUtil::nextPow2(8), 8);
    TS_ASSERT_EQUALS(MathUtil::nextPow2(9), 16);
    TS_ASSERT_EQUALS(MathUtil::nextPow2(12), 16);
    TS_ASSERT_EQUALS(MathUtil::nextPow2(16), 16);
    TS_ASSERT_EQUALS(MathUtil::nextPow2(127), 128);
    TS_ASSERT_EQUALS(MathUtil::nextPow2(128), 128);
    TS_ASSERT_EQUALS(MathUtil::nextPow2(129), 256);
    TS_ASSERT_EQUALS(MathUtil::nextPow2(131000), 131072);

    unsigned int pof2 = 1;
    for (size_t i = 1 ; i < 100000u ; ++i) {
      TS_ASSERT_EQUALS(MathUtil::nextPow2(i), pof2);
      if (i == pof2) pof2 *= 2;
    }
  }


private:
};

#endif
