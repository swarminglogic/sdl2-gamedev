#include <ui/ImageRender.h>

#include <ui/GlUtil.h>
#include <util/Asset.h>


ImageRender::ImageRender()
  :  log_("ImageRender"),
     program_(),
     viewport_(0, 0),
     position_(0,0),
     surface_(),
     vertexBuffer_(0),
     textureBuffer_(0),
     vertices_(),
     texcoords_()
{
}


ImageRender::~ImageRender()
{
}


void ImageRender::initialize()
{
  log_.d("Initializing");
  program_.setShader(ShaderProgram::VERTEX,
                     Asset::shader("2dscreentexture.vert"));
  program_.setShader(ShaderProgram::FRAGMENT,
                     Asset::shader("2dscreentexture.frag"));
  program_.compile();

  vertexBuffer_  = GlUtil::prepareVertexBuffer(3*4*sizeof(GLfloat));
  textureBuffer_ = GlUtil::prepareVertexBuffer(2*4*sizeof(GLfloat));
}


void ImageRender::render(float)
{
  if(program_.isModified()) program_.compile();
  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);

  // Uniform Viewport
  glUseProgram(program_.get());
  GLuint viewportParamId  = glGetUniformLocation(program_.get(), "Viewport");
  glUniform2iv(viewportParamId, 1, viewport_.getData());

  // Uniform Tex1
  glActiveTexture(GL_TEXTURE0);
  surface_.glBind();
  GLuint texParamId  = glGetUniformLocation(program_.get(), "Tex1");
  glUniform1i(texParamId, 0);

  // vec3 vpos
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  // vec2 tpos
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, textureBuffer_);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // Draw 0 vertices, starting from 0.
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
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
  updateQuad();
  updateTex();
}


void ImageRender::setSurface(SDL_Surface& surface)
{
  surface_.setSurface(surface);
  updateQuad();
  updateTex();
}


Point ImageRender::getPosition() const
{return position_;}
void ImageRender::setPosition(Point position)
{position_ = position;}
void ImageRender::setPosition(int x, int y)
{setPosition(Point(x, y));}



void ImageRender::prepareVertices()
{
  int xi = surface_.getClip().w();
  int yi = surface_.getClip().h();

  const GLfloat x = static_cast<float>(xi) * 2;
  const GLfloat y = static_cast<float>(yi) * 2;
  const GLfloat z = 0.0f;

  // Offset by 1 if not multiples of two.
  const float xoff = 0.375f + static_cast<float>(position_.x() - viewport_.w());
  const float yoff = 0.375f + static_cast<float>(viewport_.h() - position_.y());

  const GLfloat A[] = {0 + xoff, -y + yoff, z};
  const GLfloat B[] = {x + xoff, -y + yoff, z};
  const GLfloat C[] = {0 + xoff,  0 + yoff, z};
  const GLfloat D[] = {x + xoff,  0 + yoff, z};

  vertices_ = { A[0], A[1], A[2],
                B[0], B[1], B[2],
                C[0], C[1], C[2],
                D[0], D[1], D[2] };

  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
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
  glBindBuffer(GL_ARRAY_BUFFER, textureBuffer_);
  GlUtil::fillVertexBuffer(texcoords_);
}


void ImageRender::updateQuad()
{
  prepareVertices();
  surface_.prepareForGl();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void ImageRender::updateTex()
{
  prepareTexcoords();
}
