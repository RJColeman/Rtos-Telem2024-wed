/*
 * The status thread just flashes an LED to start with
 */
#include "mbed.h"
#include "status.h"
#define STATUS_PERIOD   500

void statusThread() {
    DigitalOut statusLed(LED1);
    while(true) {
        statusLed = !statusLed;
        ThisThread::sleep_for(STATUS_PERIOD);
    }

}