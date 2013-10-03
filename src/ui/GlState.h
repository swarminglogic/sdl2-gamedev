#ifndef UI_GLSTATE_H
#define UI_GLSTATE_H

#include <math/Rect.h>
#include <ui/SDL_opengl.h>


/**
 * GlState class.
 *
 * Singleton class for managing OpenGL state, and handling
 * OpenGL state-related changing calls.
 *
 * This keeps track of enabled/disabled parameters, and bindings,
 * and thus avoids redundant opengl state changes.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class GlState
{
public:
  /**
   * Functions to set state.
   *
   * All functions return true if they actually change the state, false if
   * NO-OP.
   */
  static bool blending(bool toggle);
  static bool depthTest(bool toggle);
  static bool viewport(const Rect& viewport);
  static bool activeTexture(GLenum activeTexture);
  static bool useProgram(GLuint program);

  static bool isBlending();
  static bool isDepthTest();
  static Rect getViewport();
  static GLenum getActiveTexture();
  static GLuint getProgram();

private:
  static GlState& instance();

  bool f_blending(bool toggle);
  bool f_depthTest(bool toggle);
  bool f_viewport(const Rect& viewport);
  bool f_activeTexture(GLenum activeTexture);
  bool f_useProgram(GLuint program);

  /**
   * Functions to get state information.
   */
  bool f_isBlending() const;
  bool f_isDepthTest() const;
  Rect f_getViewport() const;
  GLenum f_getActiveTexture() const;
  GLuint f_getProgram() const;


  bool isBlending_;
  bool isDepthTest_;
  Rect viewport_;
  GLenum activeTexture_;
  GLuint program_;

  GlState();
  ~GlState();

  // NonCopyable
  GlState(const GlState& c);
  GlState& operator=(const GlState& c);
};

#endif
