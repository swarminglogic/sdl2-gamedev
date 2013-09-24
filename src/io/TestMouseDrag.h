#ifndef IO_TESTMOUSEDRAG_H
#define IO_TESTMOUSEDRAG_H

#include <io/MouseDrag.h>
#include <math/Point.h>
#include <math/Pointf.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the MouseDrag class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class TestMouseDrag : public CxxTest::TestSuite
{
public:
  void testBasic()
  {
    const float delta = 0.00001f;
    MouseDrag md;

    // Basic values interface
    TS_ASSERT_EQUALS(md.get(), Point(0, 0));
    TS_ASSERT_EQUALS(md.getX(), 0);
    TS_ASSERT_EQUALS(md.getY(), 0);
  }

  void testStateMachine()
  {
    const float delta = 0.00001f;
    MouseDrag md;

    // State interface
    md.down(Point(40, 20));
    TS_ASSERT_EQUALS(md.get(), Point(0, 0));
    TS_ASSERT_EQUALS(md.getX(), 0);
    TS_ASSERT_EQUALS(md.getY(), 0);

    md.move(Point(45, 20));
    TS_ASSERT_EQUALS(md.get(), Point(5, 0));
    TS_ASSERT_EQUALS(md.getX(), 5);
    TS_ASSERT_EQUALS(md.getY(), 0);

    md.move(Point(30, 15));
    TS_ASSERT_EQUALS(md.get(), Point(-10, -5));
    TS_ASSERT_EQUALS(md.getX(), -10);
    TS_ASSERT_EQUALS(md.getY(), -5);

    md.up(Point(41, 19));
    TS_ASSERT_EQUALS(md.get(), Point(1, -1));

    md.down(Point(50, 150));
    TS_ASSERT_EQUALS(md.get(), Point(0, 0));
    md.move(Point(50, 150));
    TS_ASSERT_EQUALS(md.get(), Point(0, 0));
    md.move(Point(-50, -50));
    TS_ASSERT_EQUALS(md.get(), Point(-100, -200));

    md.reset();
    TS_ASSERT_EQUALS(md.get(), Point(0, 0));
  }

  void testRelativeMovement()
  {
    const float delta = 0.00001f;
    MouseDrag md;
    md.setIsRelative(true);

    // Inital point data is ignored.
    md.down(Point(40, 20));
    TS_ASSERT_EQUALS(md.get(), Point(0, 0));
    TS_ASSERT_EQUALS(md.getX(), 0);
    TS_ASSERT_EQUALS(md.getY(), 0);

    // Moving relative +5, +8
    md.move(Point(5, 8));
    TS_ASSERT_EQUALS(md.get(), Point(5, 8));
    TS_ASSERT_EQUALS(md.getX(), 5);
    TS_ASSERT_EQUALS(md.getY(), 8);

    // Moving relative -1, +10
    md.move(Point(-1, 10));
    TS_ASSERT_EQUALS(md.get(), Point(4, 18));
    TS_ASSERT_EQUALS(md.getX(), 4);
    TS_ASSERT_EQUALS(md.getY(), 18);

    // Releasing mouse coordinate is ignored
    md.up(Point(20, 40));
    TS_ASSERT_EQUALS(md.get(), Point(4, 18));
    TS_ASSERT_EQUALS(md.getX(), 4);
    TS_ASSERT_EQUALS(md.getY(), 18);

    md.down(Point(5, 5));
    TS_ASSERT_EQUALS(md.get(), Point(4, 18));
    TS_ASSERT_EQUALS(md.getX(), 4);
    TS_ASSERT_EQUALS(md.getY(), 18);

    md.move(Point(-3, -17));
    TS_ASSERT_EQUALS(md.get(), Point(1, 1));
    TS_ASSERT_EQUALS(md.getX(), 1);
    TS_ASSERT_EQUALS(md.getY(), 1);
  }

private:
};

#endif
