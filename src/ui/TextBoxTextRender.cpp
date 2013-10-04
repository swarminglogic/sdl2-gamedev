#include <ui/TextBoxTextRender.h>

#include <glm/glm.hpp>
#include <io/TextBoxText.h>
#include <math/Pointf.h>
#include <ui/GlState.h>
#include <ui/GlUtil.h>
#include <ui/Surface.h>
#include <util/Asset.h>
#include <util/CharMap.h>


TextBoxTextRender::TextBoxTextRender()
  : log_("TextBoxTextRender"),
    charmap_(nullptr),
    surface_(new Surface),
    textBoxText_(nullptr),
    viewport_(0, 0),
    position_(),
    program_(),
    vertexBuffer_(),
    vertices_(),
    zoomFactor_(1),
    paramId_Tex1_(-1),
    paramId_Viewport_(-1),
    paramId_charBoxDimensions_(-1),
    paramId_offset_(-1)
{
}


TextBoxTextRender::~TextBoxTextRender()
{
}


void TextBoxTextRender::initialize()
{
  log_.d("Initializing");
  assert(charmap_ && surface_ && textBoxText_);
  assert(viewport_ != Size(0, 0));

  program_.setShader(ShaderProgram::VERTEX,
                     Asset::shader("2dscreentext.vert"));
  program_.setShader(ShaderProgram::FRAGMENT,
                     Asset::shader("2dscreentext.frag"));
  updateShader();

  vertexBuffer_  = GlUtil::allocateVertexBuffer(2*4*sizeof(GLfloat));
}


void TextBoxTextRender::postConfigureInitialize()
{
  updateQuad();
}


void TextBoxTextRender::render(float )
{
  assert(charmap_ && surface_ && textBoxText_);
  assert(!vertices_.empty() && "Forget postConfigureInitialize() ?");
  if(program_.isModified()) updateShader();
  GlState::useProgram(program_.get());

  // Uniform Tex1
  GlState::activeTexture(GL_TEXTURE0);
  surface_->glBind();
  glUniform1i(paramId_Tex1_, 0);

  // Uniform Viewport
  glUniform2iv(paramId_Viewport_, 1, viewport_.getData());

  // Uniform Charbox dimensions
  const int charWidth = charmap_->getCharSize().w();
  const float charWidthf = (float)charWidth / (float)surface_->getWidth();
  const int charHeight = charmap_->getCharSize().h();
  const float charHeightf = (float)charHeight / (float)surface_->getHeight();

  const Pointf charTexDim(surface_->glTexCoordX(charWidthf),
                          surface_->glTexCoordY(charHeightf));
  glUniform2fv(paramId_charBoxDimensions_, 1, charTexDim.getData());

  // Uniform quadVertices
  glEnableVertexAttribArray(0);
  GlState::bindBuffer(GlState::ARRAY_BUFFER, vertexBuffer_);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

  // Prepare Quad and Texture offset
  std::vector<glm::vec4> offset;
  const uint8_t cHeight = textBoxText_->getHeight();
  const std::array<uint8_t, 256>& cWidths = textBoxText_->getWidths();
  const std::vector<std::string>& lines = textBoxText_->getFormatted();
  float xpos = 0.0f;
  float hpos = 0.0f;
  for (const auto& line : lines) {
    for (const auto& ch : line) {
      // TODO swarminglogic, 2013-10-02:
      // The variant should be provided by TextBoxText
      // TextBoxTextChar 'getChar', 'getVariant'
      const Point charIndexOffset =
        charmap_->getCharIndexOffset(CharMap::C_GOLDEN, ch);
      const float tpos = (float)charIndexOffset.x() * charTexDim.x();
      const float upos = (float)charIndexOffset.y() * charTexDim.y();
      offset.push_back(glm::vec4(xpos, hpos, tpos, upos));
      xpos += (float)cWidths[ch] * 2 * zoomFactor_;
    }
    hpos -= (float)cHeight * 2 * zoomFactor_; // 3: line spacing
    xpos = 0;
  }

  // Render instanced quads in batches of 500
  int remaining = (int)offset.size();
  int rendered = 0;
  while (remaining > 0) {
    int toRender = std::min(remaining, 500);
      // Transfer data to uniform.
    glUniform4fv(paramId_offset_, toRender, &offset[0][0] + rendered);

    // render toRender quads, with information
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, toRender);

    // Increment, decrement counters
    rendered += toRender;
    remaining -= toRender;
  }
  assert(rendered == (int)offset.size());
  glDisableVertexAttribArray(0);
}

void TextBoxTextRender::handleResize(int width, int height)
{
  if (viewport_.w() == width && viewport_.h() == height)
    return;

  viewport_ = Size(width, height);
  if(!vertices_.empty())
    updateQuad();
}


void TextBoxTextRender::setTextBoxText(const TextBoxText& textBoxText)
{ textBoxText_.reset(new TextBoxText(textBoxText));}
void TextBoxTextRender::setCharMap(const CharMap& charmap)
{charmap_.reset(new CharMap(charmap));}


void TextBoxTextRender::loadImage(const std::string& filename)
{surface_->loadImage(filename);}

void TextBoxTextRender::setText(const std::string& text)
{textBoxText_->setText(text);}


void TextBoxTextRender::setPosition(Point position)
{position_ = position;}

Point TextBoxTextRender::getPosition() const
{return position_;}

void TextBoxTextRender::setZoomFactor(unsigned char zoomFactor)
{zoomFactor_ = zoomFactor;}


void TextBoxTextRender::prepareVertices()
{
  int cw = charmap_->getCharSize().w();
  int ch = charmap_->getCharSize().h();

  const GLfloat x = static_cast<float>(cw * 2 * zoomFactor_);
  const GLfloat y = static_cast<float>(ch * 2 * zoomFactor_);

  const float xoff = (0.375f/(float)(zoomFactor_ + 1) +
                      static_cast<float>(position_.x() - viewport_.w()));
  const float yoff = (0.375f/(float)(zoomFactor_ + 1) +
                      static_cast<float>(viewport_.h() - position_.y()));

  const GLfloat A[] = {0 + xoff, -y + yoff};
  const GLfloat B[] = {x + xoff, -y + yoff};
  const GLfloat C[] = {0 + xoff,  0 + yoff};
  const GLfloat D[] = {x + xoff,  0 + yoff};

  vertices_ = { A[0], A[1],
                B[0], B[1],
                C[0], C[1],
                D[0], D[1] };

  GlState::bindBuffer(GlState::ARRAY_BUFFER, vertexBuffer_);
  GlUtil::fillVertexBuffer(vertices_);
}

void TextBoxTextRender::updateQuad()
{
  prepareVertices();
  surface_->prepareForGl();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

bool TextBoxTextRender::isReady() const
{
  return charmap_ && surface_ && textBoxText_;
}

void TextBoxTextRender::updateShader()
{
  program_.compile();
  paramId_charBoxDimensions_ =
    glGetUniformLocation(program_.get(), "charBoxDimensions");
  paramId_Viewport_ = glGetUniformLocation(program_.get(), "Viewport");
  paramId_offset_   = glGetUniformLocation(program_.get(), "offset");
  paramId_Tex1_     = glGetUniformLocation(program_.get(), "Tex1");
}
