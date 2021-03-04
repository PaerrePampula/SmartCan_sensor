#include "ScreenControl.h"
#include "MQTTNetworkingControl.h"
#include "DistanceSensor.h"
#include "mbed.h"
#include "ESP8266Interface.h"

//Electrical components
ESP8266Interface esp(MBED_CONF_APP_ESP_TX_PIN, MBED_CONF_APP_ESP_RX_PIN);
DistanceSensor disSensor(D0, D1);
Adafruit_SSD1331 OLED(D9, D6, D10, D11, NC,D13); // cs, res, dc, mosi, (nc), sck
InterruptIn magContact(D12);

//Other mbed c++ objects
Timer bounceBackTimer;
Timeout timerTimeOut;
EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread t;



//Fields
bool newEvent = false;
bool canMeasure = true;

unsigned short foundDistance = 0;
unsigned short foundDifference;
unsigned short calibratedDistance = 0;

void calculateNewDifferenceToCalibratedDepth() 
{
    
    float depthDifference = ((float)foundDistance / (float)calibratedDistance) * 100;

    foundDifference = 100 - (int)depthDifference;
    printf("New calculation:%hu%% difference\n", foundDifference);

}
void distanceCalculationHandle() 
{
    ThisThread::sleep_for(100ms);
    foundDistance = disSensor.getDistanceInCm();
    if (foundDistance > calibratedDistance) 
    {
    foundDistance = 0;
    }
}
void getDistanceFromSensor() 
{
    queue.call(&distanceCalculationHandle);

    newEvent = true;
}
void enableMeasure() 
{ 
    canMeasure = !canMeasure; 
}
void startDist(void) 
{
    if (canMeasure) 
    {
    // Disable possible measuring
    enableMeasure();
    // Get the distance from sensor
    getDistanceFromSensor();
    // Wait for 500ms, then re-enable measuring
    timerTimeOut.attach(callback(&enableMeasure), 1500ms);
    }
}

int main() 
{
    ScreenControl sControl(OLED);
    MQTTNetworkingControl mqttControl(esp);
    //sControl + mControl self-initializes
    calibratedDistance = disSensor.getDistanceInCm();
    sControl.notifyInitialCalibration(calibratedDistance);
    // Start the event queue to stop on can close-events
    t.start(callback(&queue, &EventQueue::dispatch_forever));
    magContact.fall(queue.event(&startDist));
    while (1) 
    {
        if (newEvent) 
        {   
            printf("%i", foundDistance);
            //Prevent any new firings of this event
            newEvent = !newEvent;
            //Sleep for a while, to wait to make sure that the can is indeed closed
            ThisThread::sleep_for(3s);
            //Do the calculation between the calibrated depth and the currently measured depth (which is calculated as (newDepth/CalibDepth)*100)
            calculateNewDifferenceToCalibratedDepth();
            //Notify the screen of this new calculation
            sControl.notifyNewCalculation(foundDifference);
            //Fire off the message publish in the mqtt control class
            mqttControl.createAndSendMQTTMessage(foundDifference);
        }
    }
}
