#include <ui/TexturedQuadRender.h>

#include <math/MathUtil.h>
#include <ui/GlState.h>
#include <ui/GlUtil.h>
#include <util/Asset.h>


TexturedQuadRender::TexturedQuadRender()
  :  log_("TexturedQuadRender"),
     program_(),
     viewport_(0, 0),
     rect_(0, 0, 0, 0),
     surface_(),
     vertexBuffer_(0),
     vertices_(),
     texParamId_(-1)
{
  surface_.setIsMaxFiltering(false);
}


TexturedQuadRender::~TexturedQuadRender()
{
}


void TexturedQuadRender::initialize()
{
  log_.d("Initializing");
  program_.setShader(ShaderProgram::VERTEX,
                     Asset::shader("texturedquad.vert"));
  program_.setShader(ShaderProgram::FRAGMENT,
                     Asset::shader("texturedquad.frag"));
  updateShader();

  float s = 1.0f;
  vertices_ = {
    -s, -s, 0.0f,
     s, -s, 0.0f,
    -s,  s, 0.0f,
     s,  s, 0.0f
  };
  vertexBuffer_  = GlUtil::prepareVertexBuffer(vertices_);
}



void TexturedQuadRender::render(float time)
{
  if(program_.isModified()) updateShader();
  GlState::enable(GlState::BLEND);
  GlState::enable(GlState::DEPTH_TEST);

  GlState::useProgram(program_.get());

  // Uniform Tex1
  GlState::activeTexture(GL_TEXTURE0);
  surface_.glBind();
  if (texParamId_)
    glUniform1i(texParamId_, 0);

  // Uniform Time
  if (timeParamId_)
    glUniform1f(timeParamId_, time);

  // vec3 vpos
  glEnableVertexAttribArray(0);
  GlState::bindBuffer(GlState::ARRAY_BUFFER, vertexBuffer_);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glDisableVertexAttribArray(0);
}


void TexturedQuadRender::handleResize(int width, int height)
{
  if (viewport_.w() == width && viewport_.h() == height)
    return;

  viewport_ = Size(width, height);
}


void TexturedQuadRender::loadImage(const std::string& filename)
{
  surface_.loadImage(filename);
  rect_.w(surface_.getWidth());
  rect_.h(surface_.getHeight());
}


void TexturedQuadRender::setSurface(SDL_Surface& surface)
{
  surface_.setSurface(surface);
  rect_.setSize(surface_.getSize());
}

void TexturedQuadRender::setGlTextureId(GLuint textureId, Size size)
{
  rect_.setSize(size);
  surface_.setGlTextureId(textureId, size);
}

void TexturedQuadRender::updateShader()
{
  program_.compile();
  texParamId_  = glGetUniformLocation(program_.get(), "Tex1");
  timeParamId_ = glGetUniformLocation(program_.get(), "time");
}
