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

  friend inline bool operator<(const Asset& lhs, const Asset& rhs);
  friend inline bool operator==(const Asset& lhs, const Asset& rhs);
  friend inline bool operator!=(const Asset& lhs, const Asset& rhs);


protected:
  Asset::Type type_;
  std::string path_;

private:
  static const std::string base_;

  // TODO swarminglogic, 2013-11-05: Remove
  static std::string typeToPath(Asset::Type type);

};



inline bool operator<(const Asset& lhs, const Asset& rhs) {
  return lhs.path() < rhs.path();
}


inline bool operator==(const Asset& lhs, const Asset& rhs) {
  return (lhs.type_ == rhs.type_) && (lhs.path_ == rhs.path_);
}
inline bool operator!=(const Asset& lhs, const Asset& rhs) {
  return !(lhs == rhs);
}



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
