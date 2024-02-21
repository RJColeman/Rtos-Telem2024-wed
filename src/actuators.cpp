/*
 * This thread monitors stored inputs from sensors and controls outputs 
 * for heaters and lights, etc
 */

#include "mbed.h"
#include "display.h"
#include "actuators.h"
#include "config.h"

extern myD myData;

void setActuatorsTask() {
    message_t msg;
    DigitalOut lightLed(LIGHTS_LED); // status of light switch
    DigitalOut heaterLed(HEATER_LED); // is the heater on?
    heaterLed = false;
    lightLed = false;

    while (true) {
        if ( myData.light > myData.lightSet + LIGHT_DZ) {
            // Turn the lights off it's too bright in here
            lightLed = false;
            myData.lightState = false;
        }
        else if (myData.light < myData.lightSet - LIGHT_DZ) {
            // Turn the lights on its dark in here
            lightLed = true;
            myData.lightState = true;
        }
        if ( myData.temp > myData.tempSet + HEATER_DZ) {
            // Turn the fire off it's too hot in here
            heaterLed = false;
            myData.heaterState = false;
        }
        else if (myData.temp < myData.tempSet - HEATER_DZ) {
            // Turn the fire on its cold in here
            heaterLed = true;
            myData.heaterState = true;
        }
        sprintf(msg.buffer, " %s ", myData.lightState?" ON ":"OFF " );
        msg.displayType = LIGHT_STATE;
        displayMessage(msg);
        ThisThread::sleep_for(10);
        sprintf(msg.buffer, " %s ", myData.heaterState?" ON ":"OFF " );
        msg.displayType = HEATER_STATE;
        displayMessage(msg);
        ThisThread::sleep_for(100);        

    }
    
}