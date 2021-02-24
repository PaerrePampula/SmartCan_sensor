#include "mbed.h"
#include "DistanceSensor.h"
#include <cstdio>


 

DistanceSensor  disSensor(D0,D1);
Timer bounceBackTimer;
Timeout timerTimeOut;
InterruptIn magContact(D12);
EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread t;

//Fields
bool canMeasure = true;


unsigned int dist = 0;
void getDistanceFromSensor()
{
    dist=disSensor.getDistanceInCm();
    printf("cm:%u",dist );
    
}
void enableMeasure()
{
    canMeasure = !canMeasure;
}
void startDist(void)
{
    if (canMeasure)
    {
        //Disable possible measuring
        enableMeasure();
        //Get the distance from sensor
        getDistanceFromSensor();
        //Wait for 500ms, then re-enable measuring
        timerTimeOut.attach(callback(&enableMeasure), 1500ms);
    }

}

int main()
{
    // Start the event queue
    t.start(callback(&queue, &EventQueue::dispatch_forever));

    magContact.fall(queue.event( startDist));

    while(1) {
    }
}

 