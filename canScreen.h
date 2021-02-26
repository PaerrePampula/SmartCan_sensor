#include "Adafruit_SSD1331.h"
class canScreen
{
    public:
    canScreen(Adafruit_SSD1331 OLEDScreen);
    void updateScreenInfo(Adafruit_SSD1331 OLEDScreen,int fillPercentage);
    private:
    void initialize(Adafruit_SSD1331 OLEDScreen);
};

