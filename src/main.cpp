#include <cstdlib>
#include <iostream>

#include <config/ConfigManager.h>
#include <core/MainManager.h>
#include <util/Exception.h>
#include <util/Log.h>


int main(int argc, char *argv[])
{
  Log log("main");
  try {
    ConfigManager::instance();
    MainManager& mainManager = MainManager::instance();
    mainManager.initialize();
    mainManager.run();
  }
  catch (Exception e) {
    log.error(std::string("Terminating early due to exception: ") + e.what());
    exit(EXIT_FAILURE);
  }

  MainManager::instance().finalize();
  log.info("Terminating successfully");
  return EXIT_SUCCESS;
}
