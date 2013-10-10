#include <wip/ToTextureRenderer.h>

#include <glm/gtc/matrix_transform.hpp>
#include <ui/BaseCamera.h>
#include <ui/CameraFpv.h>
#include <ui/CameraSpherical.h>
#include <ui/SDL_opengl.h>
#include <util/Asset.h>


ToTextureRenderer::ToTextureRenderer()
  : log_("ToTextureRenderer"),
    vao_(0),
    frameBuffer_(0),
    camera_(nullptr),
    projectionMat_(glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f)),
    viewport_(),
    texture_(),
    mesh_(Asset::mesh("sss2.cobj")),
    shader_(),
    mvpID_(0),
    textureId_(0),
    diffuseColorId_(0),
  depthBuffer_(0),
  cameraMotion_{8, 8, 8, 8, 8, 8}
{
  camera_.reset(new CameraFpv(glm::vec3(3.3f), glm::vec3(0.0f)));
  // camera_.reset(new CameraSpherical(3.3f, Pointf(0.47f, 0.45f)));
  updateViewMatrix();
 }


ToTextureRenderer::~ToTextureRenderer()
{
}

void ToTextureRenderer::initialize()
{
  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);
  GlState::enable(GlState::DEPTH_TEST);
  GlState::enable(GlState::CULL_FACE);
  GlState::blendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  shader_.setShader(ShaderProgram::VERTEX, Asset::shader("objrender.vert"));
  shader_.setShader(ShaderProgram::FRAGMENT, Asset::shader("objrender.frag"));
  updateShader();
  updateModel();
  texture_.loadImage("uv_colorgrid.png");
  texture_.setIsMaxFiltering(true);
  texture_.prepareForGl();

  glClearColor( 0.1f, 0.2f, 0.3f, 1.0f );

  // TO TEXTURE RENDERING INITIALIZATION
  quadRender_.initialize();
  glGenFramebuffers(1, &fboHandle_);
  updateToTextureRenderFBO();
}


void ToTextureRenderer::updateToTextureRenderFBO()
{
  if (viewport_.w() == 0 || viewport_.h() == 0)
    return;

  GlState::bindFramebuffer(GL_FRAMEBUFFER, fboHandle_);

  // Create texture object
  renderTexture_ = 0;
  glGenTextures(1, &renderTexture_);
  GlState::activeTexture(GL_TEXTURE0);
  GlState::bindTexture(GL_TEXTURE_2D, renderTexture_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, viewport_.w(), viewport_.h(),
               0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // Bind texture to fbo
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                         GL_TEXTURE_2D, renderTexture_, 0);

  // Create depth buffer
  GLuint prevDepthBuffer_ = depthBuffer_;
  glGenRenderbuffers(1, &depthBuffer_);
  GlState::bindRenderbuffer(depthBuffer_);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
                        viewport_.w(), viewport_.h());

  // Bind depth buffer to FBO
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, depthBuffer_);

  // Set targets for grament shader output
  GLenum drawbufs[] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, drawbufs);

	// Check that framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    log_.exception("Framebuffer is not ok. What happened?");

  quadRender_.setGlTextureId(renderTexture_, viewport_);

  GlState::bindFramebuffer(GL_FRAMEBUFFER, 0);

  if (prevDepthBuffer_)
    glDeleteRenderbuffers(1, &prevDepthBuffer_);
}


void ToTextureRenderer::render(float time)
{
  if(shader_.isModified())
    updateShader();
  if (mesh_.isUpdated())
    mesh_.load();
  camera_->update();
  updateViewMatrix();

  // Set viewport matching to-texture render.
  GlState::bindFramebuffer(GL_FRAMEBUFFER, fboHandle_);
  glClearColor( 0.1f, 0.2f, 0.3f, 1.0f );
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  GlState::useProgram(shader_.get());

  GlState::enable(GlState::BLEND);
  GlState::enable(GlState::DEPTH_TEST);

  // MVP
  mvpMat_ = projectionMat_ * viewMat_;
  glUniformMatrix4fv(mvpID_, 1, GL_FALSE, &mvpMat_[0][0]);

  // Texture1
  if (textureId_) {
    GlState::activeTexture(GL_TEXTURE0);
    texture_.glBind();
    glUniform1i(textureId_, 0);
  }

  const size_t shapeCount = mesh_.getShapeCount();
  for (size_t i = 0 ; i < shapeCount ; ++i) {
    GlState::bindBuffer(GlState::ELEMENT_ARRAY_BUFFER, mesh_.getIndexBuffer(i));

    // DiffuseColor
    if (diffuseColorId_) {
      glUniform3fv(mvpID_, 1, mesh_.getMaterial(i).diffuse);
    }

    // Verticies
    GlState::bindBuffer(GlState::ARRAY_BUFFER, mesh_.getVertexBuffer(i));
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    const bool hasUVdata = mesh_.hasUVdata(i);
    const bool hasNormalData = mesh_.hasNormalData(i);
    // UV data
    if (hasUVdata) {
      GlState::bindBuffer(GlState::ARRAY_BUFFER, mesh_.getUVBuffer(i));
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
      glEnableVertexAttribArray(1);
    }

    // Normal Data
    if (hasNormalData) {
      GlState::bindBuffer(GlState::ARRAY_BUFFER, mesh_.getNormalBuffer(i));
      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
      glEnableVertexAttribArray(2);
    }

    glDrawElements(GL_TRIANGLES,
                   mesh_.getIndicesCount(i),
                   GL_UNSIGNED_INT,
                   (GLvoid*)0);

    glDisableVertexAttribArray(0);
    // Indices
    if (hasUVdata) glDisableVertexAttribArray(1);
    if (hasNormalData) glDisableVertexAttribArray(2);
  }


  // Finished rendering scene to texture, reset viewport
  GlState::bindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor( 0.4f, 0.2f, 0.3f, 1.0f );
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  quadRender_.render(time);
}

bool ToTextureRenderer::handleEvent(const SDL_Event& event)
{
  const bool cameraChanged = camera_->handleEvent(event);
  return cameraChanged;
}


void ToTextureRenderer::handleResize(int width, int height)
{
  BasicRender::handleResize(width, height);
  float aspect = static_cast<float>(width) / static_cast<float>(height);
  projectionMat_ = glm::perspective(45.0f, aspect, 0.1f, 100.0f);
  viewport_ = Size(width, height);
  quadRender_.handleResize(width, height);
  updateToTextureRenderFBO();
}


bool ToTextureRenderer::prefersMouseGrab() const
{
  return camera_->prefersMouseGrab();
}



void ToTextureRenderer::updateViewMatrix()
{
  // Smooth out motion?
  const glm::vec3& pos    = camera_->getPosition();
  const glm::vec3& target = camera_->getTargetPosition();

  cameraMotion_[0].insertValue(pos.x);
  cameraMotion_[1].insertValue(pos.y);
  cameraMotion_[2].insertValue(pos.z);
  cameraMotion_[3].insertValue(target.x);
  cameraMotion_[4].insertValue(target.y);
  cameraMotion_[5].insertValue(target.z);

  viewMat_ = glm::lookAt(glm::vec3(cameraMotion_[0].getAverage(),
                                   cameraMotion_[1].getAverage(),
                                   cameraMotion_[2].getAverage()),
                         glm::vec3(cameraMotion_[3].getAverage(),
                                   cameraMotion_[4].getAverage(),
                                   cameraMotion_[5].getAverage()),
                         glm::vec3(0.0f, 1.0f, 0.0f));
}


void ToTextureRenderer::updateShader()
{
  shader_.compile();
  mvpID_          = glGetUniformLocation(shader_.get(), "MVP");
  textureId_      = glGetUniformLocation(shader_.get(), "Texture1");
  diffuseColorId_ = glGetUniformLocation(shader_.get(), "DiffuseColor");
  assert(mvpID_ >= 0);
}

void ToTextureRenderer::updateModel()
{
  mesh_.load();
}
