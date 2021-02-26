#include "Adafruit_GFX.h"
#include "Adafruit_SSD1331.h"
  // Define colors
#define Black 0x0000
#define Blue 0x001F
#define Red 0xF800
#define Green 0x07E0
#define Cyan 0x07FF
#define Magenta 0xF81F
#define Yellow 0xFFE0
#define White 0xFFFF
class ScreenControl 
{
public:
  ScreenControl(Adafruit_SSD1331 &screen);
  void notifyInitialCalibration(int initialDepth);
private:
  Adafruit_SSD1331 *OLED;
  void init();
  void updateScreen();



};