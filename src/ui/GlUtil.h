#ifndef UI_GLUTIL_H
#define UI_GLUTIL_H

#include <cassert>
#include <vector>

#include <ui/GlState.h>
#include <ui/SDL_opengl.h>


/**
 * GlUtil class.
 *
 * Static utility functions for GL calls and operations.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class GlUtil
{
public:
  /**
   * Creates a vertex buffer, fills buffer w/data, returns name of buffer.
   */
  template <typename T>
  static GLuint prepareVertexBuffer(const std::vector<T>& data,
                                    GlState::BufferTarget target =
                                        GlState::ARRAY_BUFFER,
                                    GLenum usage = GL_STATIC_DRAW)
  {
    GLuint buffername = 0;
    glGenBuffers(1, &buffername);
    GlState::bindBuffer(target, buffername);
    GlState::bufferData(target,
                        data.size() * sizeof(data[0]),
                        &data[0],
                        usage);
    return buffername;
  }

  /**
   * Creates a vertex buffer, does NOT fill buffer w/data
   * Returns name of buffer.
   *
   * Use fillVertexBuffer to fill w/data.
   */
  static GLuint allocateVertexBuffer(GLsizeiptr size,
                                    GlState::BufferTarget target =
                                        GlState::ARRAY_BUFFER,
                                     GLenum usage = GL_STATIC_DRAW);

  /**
   * Fills vertex buffer with data.
   * Make sure the buffer is bound to the target array prior to calling.
   */
  template <typename T>
  static void fillVertexBuffer(const std::vector<T>& data,
                               GlState::BufferTarget target =
                                   GlState::ARRAY_BUFFER)
  {
    assert(!data.empty() && "Cannot fill vertex buffer w/empty data!");
    GlState::bufferSubData(target,
                           0,
                           data.size() * sizeof(data[0]),
                           &data[0]);
  }

private:
  GlUtil();
 ~GlUtil();

  // NonCopyable
  GlUtil(const GlUtil& c);
  GlUtil& operator=(const GlUtil& c);
};

#endif
