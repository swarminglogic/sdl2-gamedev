#ifndef IO_TESTRESOURCEMANAGER_H
#define IO_TESTRESOURCEMANAGER_H

#include <config/ConfigManager.h>
#include <core/MainManager.h>
#include <io/ResourceManager.h>
#include <ui/GlState.h>
#include <ui/Mesh.h>
#include <ui/Surface.h>
#include <util/Asset.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the ResourceManager class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class TestResourceManager : public CxxTest::TestSuite
{
public:
  void setUp() {
    ConfigManager::instance().getLogManager().setStreamLogLevel(LogManager::LEVEL_NONE);
    MainManager& mainManager = MainManager::instance();
  }


  void testInitialzation() {}

  void testImageLoading()
  {
    ResourceManager rm;
    SurfaceShPtr ptr = rm.load(AssetImage("doesntexist.png"));
    TS_ASSERT(!ptr);
    SurfaceShPtr ptr2 = rm.load(AssetImage("uv_browngrid.png"));
    TS_ASSERT(ptr2);
    SurfaceShPtr ptr3 = rm.load(AssetImage("uv_browngrid.png"));
    TS_ASSERT_EQUALS(ptr2, ptr3);

    SurfaceShPtr ptr4 = rm.load(AssetImage("gnsh-bitmapfont.png"));
    TS_ASSERT_DIFFERS(ptr3, ptr4);
    SurfaceShPtr ptr5 = rm.load(AssetImage("gnsh-bitmapfont.png"));
    SurfaceShPtr ptr6 = rm.load(AssetImage("gnsh-bitmapfont.png"));
    TS_ASSERT_EQUALS(ptr4, ptr5);
    TS_ASSERT_EQUALS(ptr4, ptr6);
  }


  void testMeshLoading()
  {
    ResourceManager rm;
    MeshShPtr ptr = rm.load(AssetMesh("doesntexist.cobj"));
    TS_ASSERT(!ptr);

    MeshShPtr ptr2 = rm.load(AssetMesh("sss.cobj"));
    TS_ASSERT(ptr2);
    MeshShPtr ptr3 = rm.load(AssetMesh("sss.cobj"));
    TS_ASSERT_EQUALS(ptr2, ptr3);

    MeshShPtr ptr4 = rm.load(AssetMesh("scenebox.cobj"));
    MeshShPtr ptr5 = rm.load(AssetMesh("scenebox.cobj"));
    TS_ASSERT_EQUALS(ptr4, ptr5);
    TS_ASSERT_DIFFERS(ptr3, ptr4);
    TS_ASSERT_DIFFERS(ptr3, ptr5);
  }


  void testShaderLoading()
  {
    ResourceManager rm;
    ShaderProgramShPtr ptr = rm.load(
      {{ShaderProgram::VERTEX,   AssetShader("doesntexist.vert")},
       {ShaderProgram::FRAGMENT, AssetShader("doesntexist.frag")}});
    TS_ASSERT(!ptr);

    ShaderProgramShPtr ptr2 = rm.load(
      {{ShaderProgram::VERTEX,   AssetShader("deferredobjrender.vert")},
       {ShaderProgram::FRAGMENT, AssetShader("deferredobjrender.frag")}});
    TS_ASSERT(ptr2);
    ShaderProgramShPtr ptr3 = rm.load(
      {{ShaderProgram::VERTEX,   AssetShader("deferredobjrender.vert")},
       {ShaderProgram::FRAGMENT, AssetShader("deferredobjrender.frag")}});
    TS_ASSERT_EQUALS(ptr2, ptr3);
  }


  void testShortHand() {
    ResourceManager rm;
    MeshShPtr ptr = rm.loadMesh("doesntexist.cobj");
    TS_ASSERT(!ptr);

    MeshShPtr ptr2 = rm.loadMesh("sss.cobj");
    TS_ASSERT(ptr2);
    MeshShPtr ptr3 = rm.loadMesh("sss.cobj");
    TS_ASSERT_EQUALS(ptr2, ptr3);

    MeshShPtr ptr4 = rm.loadMesh("scenebox.cobj");
    MeshShPtr ptr5 = rm.loadMesh("scenebox.cobj");
    TS_ASSERT_EQUALS(ptr4, ptr5);
    TS_ASSERT_DIFFERS(ptr3, ptr4);
    TS_ASSERT_DIFFERS(ptr3, ptr5);
  }


private:
};

#endif
