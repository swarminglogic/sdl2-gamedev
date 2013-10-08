#include <wip/ObjRenderer.h>

#include <glm/gtc/matrix_transform.hpp>
#include <ui/BaseCamera.h>
#include <ui/CameraFpv.h>
#include <ui/CameraSpherical.h>
#include <ui/SDL_opengl.h>
#include <util/Asset.h>
#include <util/Log.h>


ObjRenderer::ObjRenderer()
  : vao_(0),
    frameBuffer_(0),
    camera_(nullptr),
    projectionMat_(glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f)),
    viewport_(),
    texture_(),
    mesh_(Asset::mesh("sss.cobj")),
    shader_(),
    mvpID_(0),
    textureId_(0),
    diffuseColorId_(0)
{
  camera_.reset(new CameraFpv(glm::vec3(3.3f), glm::vec3(0.0f)));
  // camera_.reset(new CameraSpherical(3.3f, Pointf(0.47f, 0.45f)));
  updateViewMatrix();
 }


ObjRenderer::~ObjRenderer()
{
}

void ObjRenderer::initialize()
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
  texture_.prepareForGl();
}

void ObjRenderer::render(float)
{
  if(shader_.isModified())
    updateShader();
  if (camera_->update())
    updateViewMatrix();
  if (mesh_.isUpdated())
    mesh_.load();

  GlState::useProgram(shader_.get());

  const GLfloat color[] = { 0.1f, 0.2f, 0.3f, 1.0f };
  glClearBufferfv(GL_COLOR, 0, color);
  glClear(GL_DEPTH_BUFFER_BIT);

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
  Log log("ObjRender");
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
}

bool ObjRenderer::handleEvent(const SDL_Event& event)
{
  const bool cameraChanged = camera_->handleEvent(event);
  if (cameraChanged)
    updateViewMatrix();
  return cameraChanged;
}


void ObjRenderer::handleResize(int width, int height)
{
  BasicRender::handleResize(width, height);
  float aspect = static_cast<float>(width) / static_cast<float>(height);
  projectionMat_ = glm::perspective(45.0f, aspect, 0.1f, 100.0f);
  viewport_ = Size(width, height);
}


bool ObjRenderer::prefersMouseGrab() const
{
  return camera_->prefersMouseGrab();
}



void ObjRenderer::updateViewMatrix()
{
  viewMat_ = glm::lookAt(camera_->getPosition(),
                         camera_->getTargetPosition(),
                         glm::vec3(0.0f, 1.0f, 0.0f));
}


void ObjRenderer::updateShader()
{
  shader_.compile();
  mvpID_          = glGetUniformLocation(shader_.get(), "MVP");
  textureId_      = glGetUniformLocation(shader_.get(), "Texture1");
  diffuseColorId_ = glGetUniformLocation(shader_.get(), "DiffuseColor");
  assert(mvpID_ >= 0);
}

void ObjRenderer::updateModel()
{
  mesh_.load();
}
