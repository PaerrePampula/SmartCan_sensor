#include "mbed.h"
#include "DistanceSensor.h"


DistanceSensor::DistanceSensor(PinName echoP, PinName triggerP): echo(echoP), trigger(triggerP)
{
    initializeSensor();
}
void DistanceSensor::initializeSensor()
{
    calculatedDistance = -1;

}
void DistanceSensor::stopTrigger()
{
    trigger = 0;
    
}
void DistanceSensor::stopTimerAndMeasure()
{
    timer.stop(); //Stop the timer used in the measuring
    /*How is it calculated?
    The calculation is done by using the basic physics calculation
    S = V*T, or S = T*V
    Where S = Distance, V = Speed (In our case the speed of sound) and T = time.
    The temparature in the soundbeams way has an effect on speed of sound, but we will round our values to a nicer 340, with a slight cost to accuracy
    Our measured distances are not going to be that big and the sensor wont measure anything over 4m, so the difference this makes is very slight
    Now, for a proper conversion, the m/sec is converted to a cm/uSec value = 0.0343cm/uSec
    The value we really get from doing this calculation is doubled,
    because the echo captures the sound wave when it leaves to bounce,
    meaning that we have to divide the resulting value with 2!
    */
    calculatedDistance = timer.elapsed_time().count() * 0.0343f/2 ;
    //The <Algorithm> include would have a handy std::clamp to clamp our values
    //But this can also be done with some simple if-statements!
    //calculatedDistance = (calculatedDistance > maxClamp) ? minClamp : calculatedDistance;
    //calculatedDistance = (calculatedDistance < minClamp) ? minClamp : calculatedDistance; 
    timer.reset();
    echoTimeout.detach(); //Dont look for the missing fall anymore.
    echo.fall(NULL); //Also null the look for the fall for measuring


}
void DistanceSensor::startTimer()
{

        echo.rise(NULL);
        timer.start();
        //Stop the timer in the case if echo doesnt go LOW
        echoTimeout.attach(callback(this, &DistanceSensor::stopTimerAndMeasure), 5s);
        //Once the echo sensor goes LOW, start the actual measurement.
        echo.fall(callback(this, &DistanceSensor::stopTimerAndMeasure)) ;


}
void DistanceSensor::startMeasuring()
{
    //Turn on the trigger to HIGH to attempt measurement.
    trigger = 1;
    //The trigger sends 8 pulses now, this is later captured in the echo
    //Once the trigger pin is set to HIGH, wait for 10 uS and turn it off
    triggerTimeout.attach(callback(this, &DistanceSensor::stopTrigger), 10us);
    //Once this is done, have the rising edge of the ECHO pin be detected, this is used to measure distance in the calculation using time
    echo.rise(callback(this,&DistanceSensor::startTimer));


}
float DistanceSensor::getDistanceInCm()
{
    startMeasuring();
    ThisThread::sleep_for(200ms);
    return calculatedDistance;
}
