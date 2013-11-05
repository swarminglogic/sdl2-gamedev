#include <util/Asset.h>


const std::string Asset::base_("./assets/");

Asset::Asset()
  : type_(INVALID_TYPE),
    path_("")
{
}

Asset::~Asset()
{
}


Asset::Type Asset::type() const
{
  return type_;
}


std::string Asset::path() const
{
  return base_ + path_;
}


std::string Asset::getBasePath()
{
  return base_;
}


// Shader Asset
AssetShader::AssetShader(const std::string& resource)
{
  type_ = SHADER;
  path_ = std::string("shaders/") + resource;
}
AssetShader::~AssetShader(){}


// Image Asset
AssetImage::AssetImage(const std::string& resource)
{
  type_ = IMAGE;
  path_ = std::string("images/") + resource;
}
AssetImage::~AssetImage(){}


// Font Asset
AssetFont::AssetFont(const std::string& resource)
{
  type_ = FONT;
  path_ = std::string("fonts/") + resource;
}
AssetFont::~AssetFont(){}


// Mesh Asset
AssetMesh::AssetMesh(const std::string& resource)
{
  type_ = MESH;
  path_ = std::string("meshes/") + resource;
}
AssetMesh::~AssetMesh(){}
