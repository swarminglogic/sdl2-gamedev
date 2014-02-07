#include <ui/DeferredRenderer.h>

#include <core/MainManager.h>
#include <glm/gtc/matrix_transform.hpp>
#include <ui/GlState.h>
#include <ui/RenderableI.h>
#include <ui/SceneI.h>


DeferredRenderer::DeferredRenderer()
  : log_("DeferredRenderer"),
    vertexArrayObject_(0),
    frameBuffer_(0),
    fboHandle_(0),
    mvpID_(-1),
    modelViewMatID_(-1),
    normalMatID_(-1),
    textureId_(-1),
    textureRepeatID_(-1),
    positionTexture_(0),
    normalTexture_(0),
    colorTexture_(0),
    depthBuffer_(0)
{
  quadRender_.setProjectionMat(projectionMat_);
}


DeferredRenderer::~DeferredRenderer()
{
}


void DeferredRenderer::initialize()
{
  glGenVertexArrays(1, &vertexArrayObject_);
  glBindVertexArray(vertexArrayObject_);
  GlState::enable(GlState::DEPTH_TEST);
  GlState::enable(GlState::CULL_FACE);
  GlState::blendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  shader_ = MainManager::resources().loadShader(
    {{ShaderProgram::VERTEX,   "deferredobjrender.vert"},
     {ShaderProgram::FRAGMENT, "deferredobjrender.frag"}});

  glClearColor( 0.1f, 0.2f, 0.3f, 1.0f );
  quadRender_.initialize();
  glGenFramebuffers(1, &fboHandle_);
  updateDeferredRendererFBO();
}

void DeferredRenderer::updateDeferredRendererFBO()
{
  if (viewport_.w() == 0 || viewport_.h() == 0)
    return;

  GlState::bindFramebuffer(GL_FRAMEBUFFER, fboHandle_);

  GLuint prevDepthBuffer = depthBuffer_;
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
    log_.exception("Framebuffer is not ok.");

  quadRender_.setGlTextureId(DeferredQuadRender::GBUFFERTARGET_POSITION,
                             positionTexture_);
  quadRender_.setGlTextureId(DeferredQuadRender::GBUFFERTARGET_NORMAL,
                             normalTexture_);
  quadRender_.setGlTextureId(DeferredQuadRender::GBUFFERTARGET_COLOR,
                             colorTexture_);

  GlState::bindFramebuffer(GL_FRAMEBUFFER, 0);

  if (prevDepthBuffer)
    glDeleteRenderbuffers(1, &prevDepthBuffer);
}



void DeferredRenderer::render(float time) const
{
  static glm::mat4 mvpMat_, viewMat_;
  static glm::mat4 projectionViewMat_;
  static glm::mat3 normalMat_;

  // Bind FBO, clear buffers
  GlState::bindFramebuffer(GL_DRAW_FRAMEBUFFER, fboHandle_);
  glClearColor( 0.1f, 0.2f, 0.3f, 1.0f );
  glDepthMask(GL_TRUE);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set GL State
  GlState::enable(GlState::DEPTH_TEST);
  GlState::disable(GlState::BLEND);
  GlState::useProgram(shader_->get());
  GlState::enable(GlState::DEPTH_CLAMP);
  GlState::enable(GlState::CULL_FACE);

  // Render scene
  if (scene_) {
    viewMat_ = scene_->getViewMatrix();
    normalMat_ = scene_->getNormalMatrix();
    projectionViewMat_ = projectionMat_ * viewMat_;

    if (modelViewMatID_ >= 0)
      glUniformMatrix4fv(modelViewMatID_, 1, GL_FALSE, &viewMat_[0][0]);
    if (normalMatID_ >= 0)
      glUniformMatrix3fv(normalMatID_, 1, GL_FALSE, &normalMat_[0][0]);;

    for (auto& renderable : scene_->getSceneObjects()) {
      // mvp matrix
      mvpMat_ = projectionViewMat_ * renderable->getModelTransform();
      glUniformMatrix4fv(mvpID_, 1, GL_FALSE, &mvpMat_[0][0]);

      SurfaceShPtr decal = renderable->getDecalTexture();
      if (decal && textureId_ >= 0) {
        GlState::activeTexture(GL_TEXTURE0);
        decal->glBind();
        glUniform1i(textureId_, 0);
        if (textureRepeatID_ >= 0)
          glUniform1f(textureRepeatID_, decal->getRepeatFactorHint());
      }
      renderable->getMeshRender().render(time);
    }
  }

  // --------------------
  glDepthMask(GL_FALSE);
  GlState::disable(GlState::DEPTH_TEST);

  // Finished rendering scene to texture, reset viewport
  GlState::bindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  quadRender_.render(time);
}

void DeferredRenderer::refresh() {
  if(shader_->isModified())
    updateShader();
  quadRender_.refresh();
  quadRender_.setModelViewMat(scene_->getViewMatrix());
}


bool DeferredRenderer::handleEvent(const SDL_Event& event)
{
  (void)event;

  return false;
}

void DeferredRenderer::handleResize(int width, int height)
{
  BasicRender::handleResize(width, height);
  float aspect = static_cast<float>(width) / static_cast<float>(height);
  projectionMat_ = glm::perspective(45.0f, aspect, 0.1f, 100.0f);
  viewport_ = Size(width, height);
  quadRender_.handleResize(width, height);
  quadRender_.setProjectionMat(projectionMat_);
  updateDeferredRendererFBO();
}

bool DeferredRenderer::prefersMouseGrab() const
{
  return false;
}


void DeferredRenderer::setScene(std::shared_ptr<const SceneI> scene)
{
  scene_ = scene;
}


void DeferredRenderer::updateShader()
{
  shader_->compile();
  mvpID_          = glGetUniformLocation(shader_->get(), "MVP");
  modelViewMatID_ = glGetUniformLocation(shader_->get(), "ModelViewMat");
  normalMatID_    = glGetUniformLocation(shader_->get(), "NormalMat");
  textureId_      = glGetUniformLocation(shader_->get(), "Texture1");
  textureRepeatID_= glGetUniformLocation(shader_->get(), "TextureRepeat");

  if(mvpID_          < 0) log_.w("MVP uniform not found");
  if(modelViewMatID_ < 0) log_.w("ModelViewMat uniform not found");
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
