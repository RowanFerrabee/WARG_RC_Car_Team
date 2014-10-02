/*
 * File:   main.c
 * Author: Chris Hajduk
 *
 * Created on August 21, 2014, 11:32 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "GPS.h"
#include "main.h"
#include "HunterTruckAPI.h"
#include "debug.h"

#include "OutputCompare.h"
#include "InterchipDMA.h"
#include "PWM.h"

_FOSCSEL(FNOSC_FRC); // Internal FRC oscillator
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_NONE);
_FWDT(FWDTEN_OFF & WDTPOST_PS2048 & WDTPRE_PR128); //32,128
/*
 *
 */

void delaySec(int);
long double distance(long double,long double,long double,long double);
int timeInSecs();
int newHeading(int);

int main(int argc, char** argv) {
    initTruck();
    int start_time = timeInSecs();
    long double start_lat = getLatitude();
    long double start_lon = getLongitude();
    int start_heading = getHeading();
    int phase = 0;

    setSteering(0);
    setThrottle(50);
    while(TRUE){

        int target_heading;
        if(phase == 0){
            if(distance(start_lat, getLatitude(), start_lon, getLongitude())>5){
                setThrottle(30);
                setSteering(60);
                phase = 1;
                target_heading = newHeading(90);
                }
        }
        else if (phase == 1){
            if(getHeading()==target_heading){
                setSteering(-60);
                target_heading=start_heading;
                phase = 2;

            }
        }
        else if (phase == 2){
            if(getHeading()==target_heading){
                setSteering(0);
                setThrottle(0);
                while(TRUE);
            }
        }

        background();
    }
    return (EXIT_SUCCESS);
}

long double distance(long double lat1, long double lat2, long double lon1, long double lon2){
    long double x1 = 111131.745*lat1;
    long double x2 = 111131.745*lat2;
    long double y1 = 111131.745*lon1*cos(PI/180*lat1);
    long double y2 = 111131.745*lon2*cos(PI/180*lat2);

    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

void delaySec(int secs){

    int start_time = 3600*getHour()+60*getMin()+getSec();

    int time2 = start_time;

    while(time2 <= start_time+secs){
        time2 = 3600*getHour()+60*getMin()+getSec();
    }

}

int timeInSecs(){
    return 3600*getHour() + 60*getMin() + getSec();
}

int newHeading(int a){ //(+) rep to the right, (-) rep to the left
    int new_heading = getHeading()+a;
    if(new_heading<0)
        new_heading+=360;
    else if(new_heading>360)
        new_heading-=360;
    return new_heading;
}



/*
 FIRST PROBLEM:
 int main(int argc, char** argv) {
    initTruck();
    int start_time = timeInSecs();
    setThrottle(50);

    while(TRUE){

        if(timeInSecs>start_time+10){
            setThrottle(0);
            while(TRUE);
        }

        background();
    }
    return (EXIT_SUCCESS);
}


 SECOND PROBLEM:
 int main(int argc, char** argv) {
    initTruck();
    int start_time = timeInSecs();
    long double start_lat = getLatitude();
    long double start_lon = getLongitude();

    setSteering(0);
    setThrottle(50);
    while(TRUE){

        if(distance(start_lat, getLatitude(), start_lon, getLongitude())>10.0){
            setThrottle(0);
            while(TRUE);
        }

        background();
    }
    return (EXIT_SUCCESS);
}

 PROBLEM 3:
 int main(int argc, char** argv) {
    initTruck();
    int start_time = timeInSecs();
    long double start_lat = getLatitude();
    long double start_lon = getLongitude();
    int start_heading = getHeading();
    int phase = 0;

    setSteering(0);
    setThrottle(50);
    while(TRUE){

        int target_heading;
        if(phase == 0){
            if(distance(start_lat, getLatitude(), start_lon, getLongitude())>5){
                setThrottle(30);
                setSteering(60);
                phase = 1;
                target_heading = newHeading(90);
                }
        }
        else if (phase == 1){
            if(getHeading()==target_heading){
                setSteering(-60);
                target_heading=start_heading;
                phase = 2;

            }
        }
        else if (phase == 2){
            if(getHeading()==target_heading){
                setSteering(0);
                setThrottle(0);
                while(TRUE);
            }
        }

        background();
    }
    return (EXIT_SUCCESS);
}
 */