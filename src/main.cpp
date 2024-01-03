/*
 * City 1082 mqtt and telemetry project
 * main() launches all threads then loops
 */
 
#include "mbed.h"
#include "status.h"
#define MAIN_SLEEP 5000

// main() runs in its own thread in the OS
int main()
{
    Thread startStatus;
    printf(" System Initialisation in progress\n");
    while (true) {
        startStatus.start(callback(statusThread));
        printf("  ....\nStarted \n");
        {
            while(true) ThisThread::sleep_for(MAIN_SLEEP);
        }

    }
}

