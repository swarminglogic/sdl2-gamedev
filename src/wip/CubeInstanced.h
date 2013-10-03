#ifndef WIP_CUBEINSTANCED_H
#define WIP_CUBEINSTANCED_H

#include <vector>

#include <ui/BasicRender.h>
#include <ui/SDL_opengl.h>
#include <ui/ShaderProgram.h>


/**
 * CubeInstanced class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class CubeInstanced : public BasicRender
{
public:
  CubeInstanced();
  virtual ~CubeInstanced();

  virtual void initialize() override;
  virtual void render(float currentTime) override;

  void setPvPointer(float* pv);

private:
  void updateShader();

  void prepareVertexColors();
  void prepareVertices();

  ShaderProgram program_;
  GLuint indexBuffer_;
  GLuint vertBuffer_;
  std::vector<GLushort> indices_;
  std::vector<GLfloat> vertices_;
  float* pv_;

  GLint pvID_;
  GLint posID_;

  // NonCopyable
  CubeInstanced(const CubeInstanced& c);
  CubeInstanced& operator=(const CubeInstanced& c);
};

#endif
