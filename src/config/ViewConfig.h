#ifndef CONFIG_VIEWCONFIG_H
#define CONFIG_VIEWCONFIG_H

#include <string>

#include <math/Size.h>


/**
 * ViewConfig class. Represents view-specific settings.
 *
 * @author SwarmingLogic
 */
class ViewConfig
{
public:
  ViewConfig();
  ViewConfig(Size screenSize,
             const std::string& windowTitle,
             bool isFullScreen);
  virtual ~ViewConfig();

  const Size& getScreenSize() const;
  void setScreenSize(Size screenSize);
  int getScreenWidth() const;
  int getScreenHeight() const;

  const std::string& getWindowTitle() const;
  void setWindowTitle(const std::string& windowTitle);

  bool isFullScreen() const;
  void setIsFullScreen(bool isFullScreenSet);

private:
  Size screenSize_;
  std::string windowTitle_;
  bool isFullScreen_;
};

#endif
