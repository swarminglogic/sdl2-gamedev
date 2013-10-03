#include <ui/GlState.h>


GlState::GlState()
  : isBlending_(false),
    isDepthTest_(false),
    viewport_(Rect(0,0, 0,0)),
    activeTexture_(GL_TEXTURE0),
    program_(0)
{}
GlState::~GlState()
{}

GlState& GlState::instance()
{
  static GlState instance;
  return instance;
}


bool GlState::blending(bool toggle)
{return instance().f_blending(toggle);}
bool GlState::depthTest(bool toggle)
{return instance().f_depthTest(toggle);}
bool GlState::viewport(const Rect& vp)
{return instance().f_viewport(vp);}
bool GlState::activeTexture(GLenum activeTex)
{return instance().f_activeTexture(activeTex);}
bool GlState::useProgram(GLuint program)
{return instance().f_useProgram(program);}


bool GlState::isBlending()
{return instance().f_isBlending();}
bool GlState::isDepthTest()
{return instance().f_isDepthTest();}
Rect GlState::getViewport()
{return instance().f_getViewport();}
GLenum GlState::getActiveTexture()
{return instance().f_getActiveTexture();}
GLuint GlState::getProgram()
{return instance().f_getProgram();}



bool GlState::f_blending(bool toggle)
{
  const bool change = toggle != isBlending_;
  if (change) {
    toggle ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
    isBlending_ = toggle;
  }
  return change;
}

bool GlState::f_depthTest(bool toggle)
{
  const bool change = toggle != isDepthTest_;
  if (change) {
    toggle ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
    isDepthTest_ = toggle;
  }
  return change;
}

bool GlState::f_viewport(const Rect& vp)
{
  const bool change = vp != viewport_;
  if (change) {
    glViewport(vp.x(), vp.y(), vp.w(), vp.h());
    viewport_ = vp;
  }
  return change;
}

bool GlState::f_activeTexture(GLenum activeTex)
{
  const bool change = activeTex != activeTexture_;
  if (change) {
    glActiveTexture(activeTex);
    activeTexture_ = activeTex;
  }
  return change;
}

bool GlState::f_useProgram(GLuint program)
{
  const bool change = program != program_;
  if (change) {
    glUseProgram(program);
    program_ = program;
  }
  return change;
}


bool GlState::f_isBlending() const
{return isBlending_;}
bool GlState::f_isDepthTest() const
{return isDepthTest_;}
Rect GlState::f_getViewport() const
{return viewport_;}
GLenum GlState::f_getActiveTexture() const
{return activeTexture_;}
GLuint GlState::f_getProgram() const
{return program_;}

