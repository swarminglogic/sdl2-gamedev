#include <util/Asset.h>



std::string Asset::path(Type type, const std::string& filename)
{
  return typeToPath(type) + filename;
}

std::string Asset::image(const std::string& filename)
{
  return path(IMAGE, filename);
}

std::string Asset::shader(const std::string& filename)
{
  return path(SHADER, filename);
}

std::string Asset::font(const std::string& filename)
{
  return path(FONT, filename);
}

std::string Asset::mesh(const std::string& filename)
{
  return path(MESH, filename);
}


std::string Asset::typeToPath(Asset::Type type)
{
  // TODO swarminglogic, 2013-09-25: Differentiate with subfolders
  switch (type) {
  case SHADER:
    return std::string("./assets/shaders/");
  case IMAGE:
    return std::string("./assets/images/");
  case FONT:
    return std::string("./assets/fonts/");
  case MESH:
    return std::string("./assets/meshes/");
  default:
    return std::string("");
  }
}


