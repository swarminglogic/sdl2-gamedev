#include <ui/VoidRenderer.h>

#include <ui/SDL_opengl.h>


VoidRenderer::VoidRenderer()
  : vao_(0)
{
}


VoidRenderer::~VoidRenderer()
{
}

void VoidRenderer::initialize()
{
  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void VoidRenderer::render(float)
{
  const GLfloat color[] = { 0.1f, 0.2f, 0.3f, 1.0f };
  glClearBufferfv(GL_COLOR, 0, color);
  glClear(GL_DEPTH_BUFFER_BIT);
}

void VoidRenderer::finalize()
{
}

