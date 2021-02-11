#include "mbed.h"
#include "DistanceSensor.h"


 

DistanceSensor  disSensor(D9,D10);

unsigned int dist;
int main()
{
 
    while(1) {


        dist=disSensor.getDistanceInCm();

        printf("cm:%u",dist );
 
    }
}
 