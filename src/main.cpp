/*
 * City 1082 mqtt and telemetry project
 * main() launches all threads then loops
 */
 
#include "mbed.h"
#include "status.h"
#include "sensors.h"
#include "display.h"
#define MAIN_SLEEP 5000

// main() runs in its own thread in the OS
int main()
{
    Thread startStatus;
    Thread startSensors;
    Thread startDisplay;
    printf(" System Initialisation in progress\n");
    while (true) {
        startStatus.start(callback(statusThread));
        startDisplay.start(callback(displayTask));
        startSensors.start(callback(readSensorsTask));
        printf("  ....\nStarted \n");
        {
            while(true) ThisThread::sleep_for(MAIN_SLEEP);
        }

    }
}

