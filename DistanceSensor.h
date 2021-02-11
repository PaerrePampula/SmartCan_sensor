/*A distance sensor, that is using sonar to measure distances. The recommended and the originally used component is HC-SR04.
Maximum possible distance measured : 400cm
The connections are:
Connect VCC to a 5v power supply, do not use 3.3V! Its not enough for the sensor, and it only gives almost nonexistent results or nothing at all!
Connect Ground to Ground.
Connect ECHO and TRIGGER to any available digital pin on the MCU eg. D9 and D10.
*/
class DistanceSensor {
    public:
    /** Constructed with required pins..
    * @param echoP the pin, where the echo is connected (Output to MCU)
    * @param triggerP the pin, where the trigger is connected (Input from MCU)
    */
    DistanceSensor(PinName echoP, PinName triggerP);
    void startMeasuring();
    float getDistanceInCm();
    
    private:
    InterruptIn echo;
    DigitalOut trigger;
    Timer timer;
    float calculatedDistance;
    Timeout triggerTimeout, echoTimeout;
    float minClamp = 2; //The minimum distance possible
    float maxClamp = 400; //The max distance possible, the highest correctly measured distance.

    void initializeSensor();
    void stopTrigger();
    void stopTimerAndMeasure();
    void startTimer();


};