#include <wip/CubeInstanced.h>

#include <cassert>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ui/GlState.h>
#include <ui/GlUtil.h>
#include <util/Asset.h>


CubeInstanced::CubeInstanced()
  : program_(),
    indexBuffer_(),
    vertBuffer_(0),
    indices_(),
    vertices_(),
    pv_(nullptr),
    pvID_(0),
    posID_(0)
{
}


CubeInstanced::~CubeInstanced()
{}


void CubeInstanced::render(float time)
{
  if(program_.isModified()) {
    program_.compile();
    updateShader();
  }
  GlState::useProgram(program_.get());

  GlState::bindBuffer(GlState::ELEMENT_ARRAY_BUFFER, indexBuffer_);
  glUniformMatrix4fv(pvID_, 1, GL_FALSE, pv_);

  GlState::bindBuffer(GlState::ARRAY_BUFFER, vertBuffer_);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(0);


  const int n = 800;
  glm::vec3 pos[n];
  int i = 0;

  for (int x = -100 ; x < 100 ; ++x) {
    for (int y = -100 ; y < 100 ; ++y) {
      pos[i][0] = 0.1f * static_cast<float>(x) * 1.2f;
      pos[i][2] = 0.1f * static_cast<float>(y) * 1.2f;
      float r = std::sqrt(pos[i][0] * pos[i][0] +
                          pos[i][2] * pos[i][2]) * 0.5f;
      pos[i][1] = (2.0f *
                  std::sin(r + time * 0.3f) *
                  std::cos(r + time * 0.3f));

      if (i == n - 1) {
        glUniform3fv(posID_, n, &pos[0][0]);
        glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (GLvoid*)0,
                                n);
        i = 0;
        continue;
      }
      ++i;
    }
  }
  glDisableVertexAttribArray(0);
}


void CubeInstanced::setPvPointer(float* pv)
{
  pv_ = pv;
}


void CubeInstanced::initialize()
{
  program_.setShader(ShaderProgram::VERTEX, Asset::shader("cubeinstanced.vert"));
  program_.setShader(ShaderProgram::FRAGMENT, Asset::shader("passthru.frag"));
  updateShader();
  prepareVertices();

  vertBuffer_  = GlUtil::prepareVertexBuffer(vertices_);
  indexBuffer_ = GlUtil::prepareVertexBuffer(indices_,
                                             GlState::ELEMENT_ARRAY_BUFFER);
}

void CubeInstanced::prepareVertices()
{
  const GLfloat x = 0.5f;
  const GLfloat y = 0.5f;
  const GLfloat z = 0.5f;
  const GLfloat A[] = {-x,-y, z};
  const GLfloat B[] = { x,-y, z};
  const GLfloat C[] = {-x, y, z};
  const GLfloat D[] = { x, y, z};
  const GLfloat E[] = {-x,-y,-z};
  const GLfloat F[] = { x,-y,-z};
  const GLfloat G[] = { x, y,-z};
  const GLfloat H[] = {-x, y,-z};

  vertices_ = {
    A[0], A[1], A[2],
    B[0], B[1], B[2],
    C[0], C[1], C[2],
    D[0], D[1], D[2],
    E[0], E[1], E[2],
    F[0], F[1], F[2],
    G[0], G[1], G[2],
    H[0], H[1], H[2]
  };

  indices_ = {
    0, 1, 2, 2, 1, 3,
    5, 4, 6, 6, 4, 7,
    4, 0, 7, 7, 0, 2,
    1, 5, 3, 3, 5, 6,
    2, 3, 7, 7, 3, 6,
    4, 5, 0, 0, 5, 1
  };
}

void CubeInstanced::updateShader()
{
  program_.compile();
  pvID_ = glGetUniformLocation(program_.get(), "MVP");
  posID_ = glGetUniformLocation(program_.get(), "pos");
}
