#include <cstdlib>
#include <iostream>

#include <config/ConfigManager.h>
#include <core/MainManager.h>
#include <ui/GlState.h>
#include <util/Exception.h>
#include <util/Log.h>


int main(int argc, char *argv[])
{
  (void)argc; (void)argv;

  Log log("main");

  try {
    ConfigManager::instance();
    MainManager& mainManager = MainManager::instance();
    GlState::syncronize();
    mainManager.initialize();
    mainManager.run();
  }
  catch (Exception e) {
    log.e(std::string("Terminating early due to exception: ") + e.what());
    exit(EXIT_FAILURE);
  }

  MainManager::instance().finalize();
  log.i("Terminating successfully");
  return EXIT_SUCCESS;
}
