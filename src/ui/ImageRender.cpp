#include <ui/ImageRender.h>

#include <math/MathUtil.h>
#include <ui/GlState.h>
#include <ui/GlUtil.h>
#include <util/Asset.h>


ImageRender::ImageRender()
  :  log_("ImageRender"),
     program_(),
     viewport_(0, 0),
     rect_(0, 0, 0, 0),
     surface_(),
     zoomFactor_(1u),
     vertexBuffer_(0),
     textureBuffer_(0),
     vertices_(),
     texcoords_(),
     viewportParamId_(-1),
     texParamId_(-1)
{
  surface_.setIsMaxFiltering(false);
}


ImageRender::~ImageRender()
{
}


void ImageRender::initialize()
{
  log_.d("Initializing");
  program_.setShader(ShaderProgram::VERTEX,
                     AssetShader("2dscreentexture.vert"));
  program_.setShader(ShaderProgram::FRAGMENT,
                     AssetShader("2dscreentexture.frag"));
  updateShader();

  vertexBuffer_  = GlUtil::allocateVertexBuffer(3*4*sizeof(GLfloat));
  textureBuffer_ = GlUtil::allocateVertexBuffer(2*4*sizeof(GLfloat));
}


void ImageRender::render(float) const
{
  GlState::enable(GlState::BLEND);
  GlState::enable(GlState::DEPTH_TEST);

  // Uniform Viewport
  GlState::useProgram(program_.get());
  glUniform2iv(viewportParamId_, 1, viewport_.getData());

  // Uniform Tex1
  GlState::activeTexture(GL_TEXTURE0);
  surface_.glBind();
  glUniform1i(texParamId_, 0);

  // vec3 vpos
  glEnableVertexAttribArray(0);
  GlState::bindBuffer(GlState::ARRAY_BUFFER, vertexBuffer_);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  // vec2 tpos
  glEnableVertexAttribArray(1);
  GlState::bindBuffer(GlState::ARRAY_BUFFER, textureBuffer_);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // Draw 0 vertices, starting from 0.
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

void ImageRender::refresh()
{
  if(program_.isModified()) updateShader();
}



void ImageRender::handleResize(int width, int height)
{
  if (viewport_.w() == width && viewport_.h() == height)
    return;

  viewport_ = Size(width, height);
  updateQuad();
}


void ImageRender::loadImage(const std::string& filename)
{
  surface_.loadImage(filename);
  rect_.w(surface_.getWidth());
  rect_.h(surface_.getHeight());
  updateQuad();
  updateTex();
}


void ImageRender::setSurface(SDL_Surface& surface)
{
  surface_.setSurface(surface);
  rect_.setSize(surface_.getSize());
  updateQuad();
  updateTex();
}

void ImageRender::setGlTextureId(GLuint textureId, Size size)
{
  rect_.setSize(size);
  surface_.setGlTextureId(textureId, size);
  updateQuad();
}



Point ImageRender::getPosition() const
{return rect_.getPosition();}
void ImageRender::setPosition(Point position)
{rect_.setPosition(position);}
void ImageRender::setPosition(int x, int y)
{setPosition(Point(x, y));}



void ImageRender::setZoomFactor(unsigned char zoomFactor)
{
  if (!MathUtil::isPow2(zoomFactor))
    log_.w() << zoomFactor << " is not a power of two! "
      "Cannot render pixel perfect." << Log::end;
  zoomFactor_ = zoomFactor;
  updateQuad();
}


void ImageRender::prepareVertices()
{
  int xi = rect_.w();
  int yi = rect_.h();

  const GLfloat x = static_cast<float>(xi * 2 * zoomFactor_);
  const GLfloat y = static_cast<float>(yi * 2 * zoomFactor_);
  const GLfloat z = 0.0f;

  const float xoff = 0.375f + static_cast<float>(rect_.x() - viewport_.w());
  const float yoff = 0.375f + static_cast<float>(viewport_.h() - rect_.y());

  const GLfloat A[] = {0 + xoff, -y + yoff, z};
  const GLfloat B[] = {x + xoff, -y + yoff, z};
  const GLfloat C[] = {0 + xoff,  0 + yoff, z};
  const GLfloat D[] = {x + xoff,  0 + yoff, z};

  vertices_ = { A[0], A[1], A[2],
                B[0], B[1], B[2],
                C[0], C[1], C[2],
                D[0], D[1], D[2] };

  GlState::bindBuffer(GlState::ARRAY_BUFFER, vertexBuffer_);
  GlUtil::fillVertexBuffer(vertices_);
}


void ImageRender::prepareTexcoords()
{
  const float xmin = surface_.glTexCoordX(0.0f);
  const float ymin = surface_.glTexCoordY(0.0f);
  const float xmax = surface_.glTexCoordX(1.0f);
  const float ymax = surface_.glTexCoordY(1.0f);
  texcoords_ = {xmin , ymax,
                xmax , ymax,
                xmin , ymin,
                xmax , ymin};
  GlState::bindBuffer(GlState::ARRAY_BUFFER, textureBuffer_);
  GlUtil::fillVertexBuffer(texcoords_);
}


void ImageRender::updateQuad()
{
  prepareVertices();
  surface_.prepareForGl();
}

void ImageRender::updateTex()
{
  prepareTexcoords();
}

void ImageRender::updateShader()
{
  program_.compile();
  viewportParamId_ = glGetUniformLocation(program_.get(), "Viewport");
  texParamId_      = glGetUniformLocation(program_.get(), "Tex1");
}
