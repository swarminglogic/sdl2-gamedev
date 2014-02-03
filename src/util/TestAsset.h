#ifndef UTIL_TESTASSET_H
#define UTIL_TESTASSET_H

#include <memory>
#include <util/Asset.h>

#include <cxxtest/TestSuite.h>

/**
 * Test for the Asset class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class TestAsset : public CxxTest::TestSuite
{
public:

  void testBasePath()
  {
    TS_ASSERT_EQUALS(Asset::getBasePath(), "./assets/");
  }

  void testAssetImage()
  {
    AssetImage image("test.png");
    TS_ASSERT_EQUALS(image.path(), "./assets/images/test.png");
    TS_ASSERT_EQUALS(image.type(), Asset::IMAGE);
  }


  void testAssetFont()
  {
    AssetFont font("test.ttf");
    TS_ASSERT_EQUALS(font.path(), "./assets/fonts/test.ttf");
    TS_ASSERT_EQUALS(font.type(), Asset::FONT);
  }

  void testAssetMesh()
  {
    AssetMesh mesh("test.cobj");
    TS_ASSERT_EQUALS(mesh.path(), "./assets/meshes/test.cobj");
    TS_ASSERT_EQUALS(mesh.type(), Asset::MESH);
  }

  void testAssetShader()
  {
    AssetShader shader("test.frag");
    TS_ASSERT_EQUALS(shader.path(), "./assets/shaders/test.frag");
    TS_ASSERT_EQUALS(shader.type(), Asset::SHADER);
  }

  void testVirtual() {
    std::unique_ptr<Asset> asset(new AssetShader("test.frag"));
    TS_ASSERT_EQUALS(asset->path(), "./assets/shaders/test.frag");
    TS_ASSERT_EQUALS(asset->type(), Asset::SHADER);

    asset.reset(new AssetImage("image.png"));
    TS_ASSERT_EQUALS(asset->path(), "./assets/images/image.png");
    TS_ASSERT_EQUALS(asset->type(), Asset::IMAGE);
  }

  void testOperators() {
    // Comparison
    AssetShader shader("test.frag");
    AssetShader shaderSame("test.frag");
    TS_ASSERT(shader == shaderSame);
    TS_ASSERT_EQUALS(shader, shaderSame);
    AssetShader shaderDifferentPath("other.frag");
    TS_ASSERT_DIFFERS(shader, shaderDifferentPath);
    AssetImage shaderDifferentType("test.frag");
    TS_ASSERT_DIFFERS(shader, shaderDifferentType);
  }

private:
};

#endif
