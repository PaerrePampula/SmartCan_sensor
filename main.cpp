#include "mbed.h"
#include "DistanceSensor.h"
#include <cstdio>


 

DistanceSensor  disSensor(D9,D10);
Timer bounceBackTimer;
Timeout timerTimeOut;
InterruptIn magContact(D4);
EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread t;



unsigned int dist = 0;
void getDistanceFromSensor()
{
        dist=disSensor.getDistanceInCm();
        printf("cm:%u",dist );
}
void stopTimer()
{
    bounceBackTimer.stop();
    bounceBackTimer.reset();

}
void startDist(void)
{
    if (bounceBackTimer.elapsed_time().count() == 0)
    {
        bounceBackTimer.start();
        getDistanceFromSensor();
        timerTimeOut.attach(callback(&stopTimer), 500ms);
    }





}

int main()
{
        // Start the event queue
    t.start(callback(&queue, &EventQueue::dispatch_forever));
    printf("Starting in context %p\r\n", ThisThread::get_id());
    // The 'rise' handler will execute in IRQ context
    magContact.fall(queue.event( startDist));
    // The 'fall' handler will execute in the context of thread 't'

    while(1) {

    //
    //ThisThread::sleep_for(1s);
    }
}

 