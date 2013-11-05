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
    FONT,
    MESH,
    INVALID_TYPE
  };

  Asset();
  virtual ~Asset();

  Asset::Type type() const;
  std::string path() const;

  static std::string getBasePath();

protected:
  Asset::Type type_;
  std::string path_;

private:
  static const std::string base_;

  // TODO swarminglogic, 2013-11-05: Remove
  static std::string typeToPath(Asset::Type type);

};



class AssetShader : public Asset
{
public:
  AssetShader(const std::string& resource);
  virtual ~AssetShader();
};

class AssetImage : public Asset
{
public:
  AssetImage(const std::string& resource);
  virtual ~AssetImage();
};

class AssetFont : public Asset
{
public:
  AssetFont(const std::string& resource);
  virtual ~AssetFont();
};

class AssetMesh : public Asset
{
public:
  AssetMesh(const std::string& resource);
  virtual ~AssetMesh();
};





#endif
