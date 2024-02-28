/*
 * City 1082 mqtt and telemetry project
 * main() launches all threads then loops
 */
 
#include "mbed.h"
#include "status.h"
#include "sensors.h"
#include "display.h"
#include "actuators.h"
#include "wifi.h"
#define MAIN_SLEEP 5000

// main() runs in its own thread in the OS
int main()
{
    Thread startStatus;
    Thread startSensors;
    Thread startDisplay;
    Thread startActuators;
    Thread startWifi;
    printf(" System Initialisation in progress\n");
    while (true) {
        startStatus.start(callback(statusThread));
        startWifi.start(callback(wifiTask));
        startDisplay.start(callback(displayTask));
        startSensors.start(callback(readSensorsTask));
        startActuators.start(callback(setActuatorsTask));
        printf("  ....\nStarted \n");
        {
            while(true) ThisThread::sleep_for(MAIN_SLEEP);
        }

    }
}

