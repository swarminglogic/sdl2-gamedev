#include <ui/VoidRenderer.h>

#include <ui/SDL_opengl.h>


VoidRenderer::VoidRenderer()
{
}


VoidRenderer::~VoidRenderer()
{
}

void VoidRenderer::initialize()
{
}

void VoidRenderer::render(float)
{
  const GLfloat color[] = { 0.1f, 0.2f, 0.3f, 1.0f };
  glClearBufferfv(GL_COLOR, 0, color);
}

void VoidRenderer::finalize()
{
}

