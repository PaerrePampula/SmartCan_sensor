#include "ScreenControl.h"
ScreenControl::ScreenControl(Adafruit_SSD1331& screen) : OLED(&screen)
{
    init();
}
void ScreenControl::init()
{
    OLED->begin(); // initialization of display object
    OLED->clearScreen();
    OLED->fillScreen(White);   // background screen in blue
    OLED->setTextColor(Black); // colour of text in cyan
    OLED->setCursor(0, 20);    // cursor is in x=0 and y=0
    OLED->setTextSize(1);
    OLED->printf("Hello from SmartCan!\n[Calibrating...]");

}
void ScreenControl::notifyInitialCalibration(int initialDepth)
{

    OLED->setCursor(0, 20); // cursor is in x=0 and y=0
    ThisThread::sleep_for(2s);
    OLED->clearScreen();
    OLED->fillScreen(White); // background screen in blue
    OLED->printf("[DONE]\nDepth:%dcm", initialDepth);
    ThisThread::sleep_for(2s);
    OLED->clearScreen();
}
void ScreenControl::notifyNewCalculation(int newCalculation)
{
    OLED->clearScreen();
    OLED->fillScreen(White); // background screen in blue
    OLED->setCursor(0, 15);  // cursor is in x=0 and y=15
    OLED->setTextSize(2);    // size of text
    OLED->setTextColor(Red); // text in red colour
    OLED->printf("%i%% Full", newCalculation);
    if (newCalculation >= 90)
    {
        OLED->printf("[EMPTY CAN!]");
    }
}
