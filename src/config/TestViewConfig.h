#ifndef CONFIG_TESTVIEWCONFIG_H
#define CONFIG_TESTVIEWCONFIG_H

#include <math/Size.h>
#include <config/ViewConfig.h>

#include <cxxtest/TestSuite.h>

/**
 * Test for the ViewConfig class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class TestViewConfig : public CxxTest::TestSuite
{
public:
  void testConstructor()
  {
    ViewConfig vc(Size(412, 123), "Potatoes", false);
    TS_ASSERT_EQUALS(vc.getScreenSize(), Size(412, 123));
    TS_ASSERT_EQUALS(vc.getWindowTitle(), "Potatoes");
    TS_ASSERT_EQUALS(vc.isFullScreen(), false);

    ViewConfig vc2(Size(14, 33), "Asparges", true);
    TS_ASSERT_EQUALS(vc2.getScreenSize(), Size(14, 33));
    TS_ASSERT_EQUALS(vc2.getWindowTitle(), "Asparges");
    TS_ASSERT_EQUALS(vc2.isFullScreen(), true);
  }

  void testBasic()
  {
    ViewConfig vc;
    const Size screenSize(213, 843);
    const std::string title("The Awesome Title");
    vc.setScreenSize(screenSize);
    vc.setWindowTitle(title);

    TS_ASSERT_EQUALS(vc.getScreenSize(), screenSize);
    TS_ASSERT_EQUALS(vc.getWindowTitle(), title);

    TS_ASSERT(!vc.isFullScreen());
    vc.setIsFullScreen(true);
    TS_ASSERT(vc.isFullScreen());
  }
private:
};

#endif
