#include <wip/DeferredRenderer.h>

#include <glm/gtc/matrix_transform.hpp>
#include <ui/BaseCamera.h>
#include <ui/CameraFpv.h>
#include <ui/CameraSpherical.h>
#include <ui/SDL_opengl.h>
#include <util/Asset.h>


DeferredRenderer::DeferredRenderer()
  : log_("DeferredRenderer"),
    vao_(0),
    frameBuffer_(0),
    camera_(nullptr),
    projectionMat_(glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f)),
    viewport_(),
    texture_(),
    mesh_("sss.cobj"),
    // mesh2_(new MeshRender("teapot.cobj")),
    // mesh3_(new MeshRender("head.cobj")),
    // mesh4_(new MeshRender("dragon.cobj")),
    // mesh5_(new MeshRender("buddah.cobj")),
    sceneBoxTexture_(),
    sceneBox_("scenebox.cobj"),
    shader_(),
    mvpID_(-1),
    modelViewMatID_(-1),
    projectionMatID_(-1),
    normalMatID_(-1),
    textureId_(-1),
    textureRepeatID_(-1),
    positionTexture_(0),
    normalTexture_(0),
    colorTexture_(0),
    depthBuffer_(0),
    cameraMotion_{8, 8, 8, 8, 8, 8}
{
  camera_.reset(new CameraFpv(glm::vec3(3.3f), glm::vec3(0.0f)));
  // camera_.reset(new CameraSpherical(3.3f, Pointf(0.47f, 0.45f)));
  quadRender_.setProjectionMat(projectionMat_);
  updateViewMatrix();
}


DeferredRenderer::~DeferredRenderer()
{
}

void DeferredRenderer::initialize()
{
  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);
  GlState::enable(GlState::DEPTH_TEST);
  GlState::enable(GlState::CULL_FACE);
  GlState::blendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  shader_.setShader(ShaderProgram::VERTEX,
                    Asset::shader("deferredobjrender.vert"));
  shader_.setShader(ShaderProgram::FRAGMENT,
                    Asset::shader("deferredobjrender.frag"));
  updateShader();
  // texture_.loadImage("rungholt-RGBA.png");
  texture_.loadImage("uv_colorgrid.png");
  texture_.setIsMaxFiltering(true);
  texture_.prepareForGl();

  sceneBoxTexture_.loadImage("uv_browngrid.png");
  sceneBoxTexture_.setIsMaxFiltering(true);
  sceneBoxTexture_.prepareForGl();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glClearColor( 0.1f, 0.2f, 0.3f, 1.0f );

  // TO TEXTURE RENDERING INITIALIZATION
  quadRender_.initialize();
  glGenFramebuffers(1, &fboHandle_);
  updateToTextureRenderFBO();
}


void DeferredRenderer::updateToTextureRenderFBO()
{
  if (viewport_.w() == 0 || viewport_.h() == 0)
    return;

  GlState::bindFramebuffer(GL_FRAMEBUFFER, fboHandle_);

  // Create depth buffer
  GLuint prevDepthBuffer_ = depthBuffer_;
  glGenRenderbuffers(1, &depthBuffer_);
  GlState::bindRenderbuffer(depthBuffer_);
  // TODO swarminglogic, 2013-10-11: Difference between GL_DEPTH_COMPONENT{,24}
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
                        viewport_.w(), viewport_.h());

  // POSITION texture object
  positionTexture_ = 0;
  glGenTextures(1, &positionTexture_);
  GlState::activeTexture(GL_TEXTURE0);
  GlState::bindTexture(GL_TEXTURE_2D, positionTexture_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, viewport_.w(), viewport_.h(),
               0, GL_RGB, GL_FLOAT, NULL);
  setTextureInterp();

  // NORMAL texture object
  normalTexture_ = 0;
  glGenTextures(1, &normalTexture_);
  GlState::activeTexture(GL_TEXTURE1);
  GlState::bindTexture(GL_TEXTURE_2D, normalTexture_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, viewport_.w(), viewport_.h(),
               0, GL_RGB, GL_FLOAT, NULL);
  setTextureInterp();

  // COLOR texture object
  colorTexture_ = 0;
  glGenTextures(1, &colorTexture_);
  GlState::activeTexture(GL_TEXTURE2);
  GlState::bindTexture(GL_TEXTURE_2D, colorTexture_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, viewport_.w(), viewport_.h(),
               0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  setTextureInterp();


  // Bind depth buffer to FBO
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, depthBuffer_);
  // Bind textures to fbo
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                         GL_TEXTURE_2D, positionTexture_, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
                         GL_TEXTURE_2D, normalTexture_, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2,
                         GL_TEXTURE_2D, colorTexture_, 0);


  // Set targets for grament shader output
  GLenum drawbufs[] = {GL_COLOR_ATTACHMENT0,
                       GL_COLOR_ATTACHMENT1,
                       GL_COLOR_ATTACHMENT2};

  glDrawBuffers(3, drawbufs);

  // Check that framebuffer is ok
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    log_.exception("Framebuffer is not ok. What happened?");

  // quadRender_.setSize(viewport_);
  quadRender_.setGlTextureId(DeferredQuadRender::GBUFFERTARGET_POSITION,
                             positionTexture_);
  quadRender_.setGlTextureId(DeferredQuadRender::GBUFFERTARGET_NORMAL,
                             normalTexture_);
  quadRender_.setGlTextureId(DeferredQuadRender::GBUFFERTARGET_COLOR,
                             colorTexture_);

  GlState::bindFramebuffer(GL_FRAMEBUFFER, 0);

  if (prevDepthBuffer_)
    glDeleteRenderbuffers(1, &prevDepthBuffer_);
}


void DeferredRenderer::render(float time)
{
  if(shader_.isModified())
    updateShader();

  mesh_.refresh();
  if (mesh2_) mesh2_->refresh();
  if (mesh3_) mesh3_->refresh();
  if (mesh4_) mesh4_->refresh();
  if (mesh5_) mesh5_->refresh();
  sceneBox_.refresh();
  camera_->update();
  updateViewMatrix();

  // Set viewport matching to-texture render.
  GlState::bindFramebuffer(GL_DRAW_FRAMEBUFFER, fboHandle_);
  glClearColor( 0.1f, 0.2f, 0.3f, 1.0f );
  glDepthMask(GL_TRUE);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  GlState::enable(GlState::DEPTH_TEST);
  GlState::disable(GlState::BLEND);
  GlState::useProgram(shader_.get());
  GlState::enable(GlState::DEPTH_CLAMP);
  GlState::enable(GlState::CULL_FACE);
  // Only the geometry pass updates the depth buffer

  // MVP
  mvpMat_ = projectionMat_ * viewMat_;
  glUniformMatrix4fv(mvpID_, 1, GL_FALSE, &mvpMat_[0][0]);

  if (modelViewMatID_ >= 0)
    glUniformMatrix4fv(modelViewMatID_, 1, GL_FALSE, &viewMat_[0][0]);
  if (normalMatID_ >= 0)
    glUniformMatrix3fv(normalMatID_, 1, GL_FALSE, &normalMat_[0][0]);;

  // MESH RENDER START
  // Texture1
  if (textureId_ >= 0) {
    GlState::activeTexture(GL_TEXTURE0);
    texture_.glBind();
    glUniform1i(textureId_, 0);
    if (textureRepeatID_ >= 0)
      glUniform1f(textureRepeatID_, 1.0f);
  }
  mesh_.render(0.0f);
  if (mesh2_) mesh2_->render(0.0f);
  if (mesh3_) mesh3_->render(0.0f);
  if (mesh4_) mesh4_->render(0.0f);
  if (mesh5_) mesh5_->render(0.0f);

  if (textureId_ >= 0) {
    sceneBoxTexture_.glBind();
    if (textureRepeatID_ >= 0)
      glUniform1f(textureRepeatID_, 20.0f);
  }
  sceneBox_.render(0.0f);


  // GlState::disable(GlState::DEPTH_CLAMP);
  glDepthMask(GL_FALSE);
  GlState::disable(GlState::DEPTH_TEST);

  // Finished rendering scene to texture, reset viewport
  GlState::bindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  quadRender_.render(time);
}

bool DeferredRenderer::handleEvent(const SDL_Event& event)
{
  const bool cameraChanged = camera_->handleEvent(event);
  return cameraChanged;
}


void DeferredRenderer::handleResize(int width, int height)
{
  BasicRender::handleResize(width, height);
  float aspect = static_cast<float>(width) / static_cast<float>(height);
  projectionMat_ = glm::perspective(45.0f, aspect, 0.1f, 100.0f);
  viewport_ = Size(width, height);
  quadRender_.handleResize(width, height);
  quadRender_.setProjectionMat(projectionMat_);
  updateToTextureRenderFBO();
}


bool DeferredRenderer::prefersMouseGrab() const
{
  return camera_->prefersMouseGrab();
}


void DeferredRenderer::updateViewMatrix()
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

  normalMat_ = glm::mat3(glm::vec3(viewMat_[0]),
                         glm::vec3(viewMat_[1]),
                         glm::vec3(viewMat_[2]));

  quadRender_.setModelViewMat(viewMat_);
}


void DeferredRenderer::updateShader()
{
  shader_.compile();
  mvpID_          = glGetUniformLocation(shader_.get(), "MVP");
  modelViewMatID_ = glGetUniformLocation(shader_.get(), "ModelViewMat");
  // projectionMatID_= glGetUniformLocation(shader_.get(), "ProjectionMat");
  normalMatID_    = glGetUniformLocation(shader_.get(), "NormalMat");
  textureId_      = glGetUniformLocation(shader_.get(), "Texture1");
  textureRepeatID_= glGetUniformLocation(shader_.get(), "TextureRepeat");

  if(mvpID_          < 0) log_.w("MVP uniform not found");
  if(modelViewMatID_ < 0) log_.w("ModelViewMat uniform not found");
  // if(projectionMatID_< 0) log_.w("ProjectionMat uniform not found");
  if(normalMatID_    < 0) log_.w("NormalMat uniform not found");
  if(textureId_      < 0) log_.w("Texture1 uniform not found");
  if(textureRepeatID_< 0) log_.w("TextureRepeat uniform not found");
}

void DeferredRenderer::setTextureInterp()
{
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
