#include "Adafruit_SSD1331.h"
#include "Adafruit_GFX.h"
#include "canScreen.h"
Adafruit_SSD1331 OLED(A7, D6, D10, D11, NC, D13); // cs, res, dc, mosi, (nc), sck  
DigitalOut VCCEN(D3);
DigitalOut PMODEN(D5);

// Definition of colours on the OLED display
#define Black 0x0000
#define Blue 0x001F
#define Red 0xF800
#define Green 0x07E0
#define Cyan 0x07FF
#define Magenta 0xF81F
#define Yellow 0xFFE0
#define White 0xFFFF
canScreen::canScreen()
{
    initialize();
}
void initialize()
{
        VCCEN = 1;
        PMODEN = 1;
        //Need to set to High, if these two are not connected to the 3.3V power supply directly.
        OLED.begin(); // initialization of display object
        OLED.clearScreen();   

}
void updateScreenInfo(int fillPercentage)
{
        OLED.clearScreen();   
        OLED.fillScreen(Blue); // background screen in blue
        OLED.setTextColor(Cyan); // colour of text in cyan
        OLED.setCursor(0,0); // cursor is in x=0 and y=0
        OLED.printf("%i%%", fillPercentage);

}