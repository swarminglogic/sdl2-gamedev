#ifndef UI_TESTGLSTATE_H
#define UI_TESTGLSTATE_H

#include <ui/GlState.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the GlState class.
 *
 * Only doing test-cases that don't require a OpenGL context.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class TestGlState : public CxxTest::TestSuite
{
public:
  void testGlState()
  {
    // Checking that default is false
    TS_ASSERT(!GlState::isBlending());
    // Checking that setting it false is redundant.
    TS_ASSERT(!GlState::blending(false));
    // Checking that setting it true is not redundant
    TS_ASSERT(GlState::blending(true));
    TS_ASSERT(GlState::isBlending());
    // Checking that setting it true is now redundant.
    TS_ASSERT(!GlState::blending(true));
    TS_ASSERT(GlState::isBlending());

    // Same for depth test
    // Checking that default is false
    TS_ASSERT(!GlState::isDepthTest());
    TS_ASSERT(!GlState::depthTest(false));
    TS_ASSERT(GlState::depthTest(true));
    TS_ASSERT(GlState::isDepthTest());
    TS_ASSERT(!GlState::depthTest(true));
    TS_ASSERT(GlState::isDepthTest());

    // Viewport
    // Checking that default is false
    TS_ASSERT_EQUALS(GlState::getViewport(), Rect(0,0,0,0));
    TS_ASSERT(!GlState::viewport(Rect(0,0,0,0)));
    TS_ASSERT(GlState::viewport(Rect(0,0,800,600)));
    TS_ASSERT_EQUALS(GlState::getViewport(), Rect(0,0,800,600));
    TS_ASSERT(!GlState::viewport(Rect(0,0,800,600)));

    // Active texture
    TS_ASSERT_EQUALS(GlState::getActiveTexture(), GL_TEXTURE0);
    TS_ASSERT(!GlState::activeTexture(GL_TEXTURE0));
    TS_ASSERT(GlState::activeTexture(GL_TEXTURE1));
    TS_ASSERT_EQUALS(GlState::getActiveTexture(), GL_TEXTURE1);
    TS_ASSERT(!GlState::activeTexture(GL_TEXTURE1));

    // Program
    TS_ASSERT_EQUALS(GlState::getProgram(), 0);
    TS_ASSERT(!GlState::useProgram(0));
    TS_ASSERT(GlState::useProgram(42));
    TS_ASSERT_EQUALS(GlState::getProgram(), 42);
    TS_ASSERT(!GlState::useProgram(42));
  }

private:
};

#endif
