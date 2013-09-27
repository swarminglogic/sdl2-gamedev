#ifndef UTIL_ASSET_H
#define UTIL_ASSET_H

#include <string>


/**
 * Asset class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class Asset
{
public:
  enum Type{
    SHADER,
    IMAGE,
    FONT
  };

  static std::string path(Type type, const std::string& filename);
  static std::string image(const std::string& filename);
  static std::string shader(const std::string& filename);
  static std::string font(const std::string& filename);


private:
  static std::string typeToPath(Type type);

  Asset();
  virtual ~Asset();

  // NonCopyable
  Asset(const Asset& c);
  Asset& operator=(const Asset& c);
};

#endif
