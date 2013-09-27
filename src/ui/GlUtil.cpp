#include <ui/GlUtil.h>

#include <cassert>


GLuint GlUtil::prepareVertexBuffer(const std::vector<GLfloat>& data,
                                   GLenum target,
                                   GLenum usage)
{
  GLuint buffername = 0;
  glGenBuffers(1, &buffername);
  glBindBuffer(target, buffername);
  glBufferData(GL_ARRAY_BUFFER,
               data.size() * sizeof(data[0]),
               &data[0],
               usage);
  return buffername;
}

GLuint GlUtil::prepareVertexBuffer(GLsizeiptr size,
                                   GLenum target,
                                   GLenum usage)
{
  GLuint buffername = 0;
  glGenBuffers(1, &buffername);
  glBindBuffer(target, buffername);
  glBufferData(GL_ARRAY_BUFFER, size, nullptr, usage);
  return buffername;
}


void GlUtil::fillVertexBuffer(const std::vector<GLfloat>& data,
                              GLenum target)
{
  assert(!data.empty() && "Cannot fill vertex buffer w/empty data!");
  glBufferSubData(target,
                  0,
                  data.size() * sizeof(data[0]),
                  &data[0]);
}
