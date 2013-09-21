#include <ui/ShaderProgram.h>

#include <functional>

#include <util/File.h>


ShaderProgram::ShaderProgram()
  : log_("ShaderProgram"),
    shaders_(),
    isShaderInformationModified_(true),
    programId_(0),
    cached_isShadersModified_(false),
    guard_isShaderModified_(std::bind(&ShaderProgram::guard_isShaderModified,
                                      this),
                            1000)
{
}


ShaderProgram::~ShaderProgram()
{
  deleteProgram();
}



GLuint ShaderProgram::get() const
{
  return programId_;
}


void ShaderProgram::clear()
{
  shaders_.clear();
  isShaderInformationModified_ = true;
}


void ShaderProgram::setShader(ShaderType type, const std::string& filename)
{
  shaders_[type] = File(filename);
  isShaderInformationModified_ = true;
}


void ShaderProgram::removeShader(ShaderType type)
{
  if (hasShader(type))
    shaders_.erase(type);
}


bool ShaderProgram::isModified()
{
  if (isShaderInformationModified_) return true;

  cached_isShadersModified_ = false;
  const bool wasExecuted = guard_isShaderModified_.exec();
  return wasExecuted && cached_isShadersModified_;
}


void ShaderProgram::guard_isShaderModified()
{
  bool isMod = false;
  for (auto& shader : shaders_)
    isMod |= shader.second.isUpdated();
  cached_isShadersModified_ = isMod;
}


void ShaderProgram::compile()
{
  GLuint newProgramId = glCreateProgram();
  log_.i("Creating shader program from shader files.");

  // Compile shaders.
  std::vector<GLuint> shaderIds;
  for (auto& shader : shaders_) {
    if (shader.second.isUpdated()) {
      log_.i() << "Loading shader file: " << shader.second.getFilename() << Log::end;
      shader.second.readToLocal();
    }

    GLuint shaderId = prepareShader(shader.first, shader.second.readCopy());

    if (shaderId != 0) {
      shaderIds.push_back(shaderId);
      glAttachShader(newProgramId, shaderId);
    // ERROR HANDLING
    } else {
      log_.e() << "Failed to compile shader: "
               << shader.second.getFilename() << Log::end;
      // Delete all shaders compiled up until this point.
      for (auto id : shaderIds)
        glDeleteShader(id);
      return;
    }
  }

  glLinkProgram(newProgramId);

  // ERROR HANDLING
  std::string linkInfo = ShaderUtil::checkProgramInfo(newProgramId);
  if (!linkInfo.empty()) {
    log_.e() << "Program Link Info: \n" << linkInfo << Log::end;
    for (auto id : shaderIds)
      glDeleteShader(id);
    glDeleteProgram(newProgramId);
    return;
  }


  // CLEANUP
  deleteProgram();
  programId_ = newProgramId;
  for (auto id : shaderIds) {
    glDetachShader(programId_, id);
    glDeleteShader(id);
  }

  isShaderInformationModified_ = false;
}


bool ShaderProgram::hasShader(ShaderType type) const
{
  return shaders_.find(type) != shaders_.end();
}


void ShaderProgram::deleteProgram()
{
  if (programId_ > 0)
    glDeleteProgram(programId_);
  programId_ = 0;
}


GLuint ShaderProgram::prepareShader(ShaderType type,
                                    const std::string& source) const
{
  GLenum glShaderType = shaderType2GLEnum(type);
  const GLchar* shader_source = source.c_str();
  GLuint shaderId = glCreateShader(glShaderType);
  glShaderSource(shaderId, 1, &shader_source, NULL);
  glCompileShader(shaderId);

  // Check for errors.
  std::string compileInfo =
    ShaderUtil::checkShaderInfo(shaderId);
  if (!compileInfo.empty()) {
    log_.e() << "Shader Compile Info: \n" << compileInfo << Log::end;
  }
  return shaderId;
}


GLenum ShaderProgram::shaderType2GLEnum(ShaderType type)
{
  switch (type) {
  case VERTEX:
    return GL_VERTEX_SHADER;
  case CONTROL:
    return GL_TESS_CONTROL_SHADER;
  case EVAL:
    return GL_TESS_EVALUATION_SHADER;
  case GEOMETRY:
    return GL_GEOMETRY_SHADER;
  case FRAGMENT:
    return GL_FRAGMENT_SHADER;
  default:
    return 0;
  }
}

