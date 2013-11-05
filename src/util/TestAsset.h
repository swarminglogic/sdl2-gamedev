#ifndef UTIL_TESTASSET_H
#define UTIL_TESTASSET_H

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

private:
};

#endif
