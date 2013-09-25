#include <util/Asset.h>



std::string Asset::path(Type type, const std::string& filename)
{
  return typeToPath(type) + filename;
}


std::string Asset::typeToPath(Asset::Type type)
{
  // TODO swarminglogic, 2013-09-25: Differentiate with subfolders
  switch (type) {
  case SHADER:
    return std::string("./assets/");
  case IMAGE:
    return std::string("./assets/");
  case FONT:
    return std::string("./assets/");
  default:
    return std::string("");
  }
}


