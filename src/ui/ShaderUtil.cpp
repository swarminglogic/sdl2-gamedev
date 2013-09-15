#include <ui/ShaderUtil.h>

#include <fstream>
#include <iostream>
#include <sstream>

#include <util/FileUtil.h>


GLuint ShaderUtil::createShaderProgram(
    const std::vector<std::pair<GLenum, std::string> >& shaders)
{
  GLuint programId = glCreateProgram();
  std::vector<GLuint> shaderIds;
  for (size_t shaderIndex = 0 ; shaderIndex < shaders.size() ; ++shaderIndex) {
    GLuint shaderId = prepareShader(shaders[shaderIndex].first,
                                    shaders[shaderIndex].second);
    glAttachShader(programId, shaderId);
    shaderIds.push_back(shaderId);
  }
  glLinkProgram(programId);
  std::string linkInfo = checkProgramInfo(programId, GL_LINK_STATUS);
  if (!linkInfo.empty())
    std::cerr << linkInfo << std::endl;

  // Delete shaders as they are no longer necessary after buing linked into
  // program.
  for (size_t i = 0 ; i < shaderIds.size() ; ++i)
    glDeleteShader(shaderIds[i]);

  return programId;
}


GLuint ShaderUtil::prepareShader(GLenum type, const std::string& sourceFile)
{
  std::string source = FileUtil::read(sourceFile);
  const GLchar* shader_source = source.c_str();

  GLuint shaderId = glCreateShader(type);
  glShaderSource(shaderId, 1, &shader_source, NULL);
  glCompileShader(shaderId);

  // check for errors, and throw exception if not ok.
  std::string compileInfo = checkShaderInfo(shaderId, GL_COMPILE_STATUS);
  if (!compileInfo.empty())
    std::cerr << compileInfo << std::cerr;
  return shaderId;
}


std::string ShaderUtil::checkShaderInfo(GLuint shaderId,
                                        GLenum statusType)
{
  GLint result = GL_TRUE;
  glGetShaderiv(shaderId, statusType, &result);
  if (GL_FALSE != result) {
    return std::string("");
  } else {
    std::stringstream ss;
    ss << "Shader error ocurred\n";
    GLint loglen;
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &loglen);
    if (loglen > 0) {
      char log[loglen];
      GLsizei written;
      glGetShaderInfoLog(shaderId, loglen, &written, log);
      ss << log << std::endl;
    }
    return ss.str();
  }
}


std::string ShaderUtil::checkProgramInfo(GLuint programId,
                                         GLenum statusType)
{
  GLint result = GL_TRUE;
  glGetProgramiv(programId, statusType, &result);
  if (GL_FALSE != result) {
    return std::string("");
  } else {
    std::stringstream ss;
    ss << "Program error ocurred\n";
    GLint loglen;
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &loglen);
    if (loglen > 0) {
      char log[loglen];
      GLsizei written;
      glGetProgramInfoLog(programId, loglen, &written, log);
      ss << log << std::endl;
    }
    return ss.str();
  }
}
