/*
 * This thread monitors stored inputs from sensors and controls outputs 
 * for heaters and lights, etc
 */

#include "mbed.h"
#include "display.h"
#include "actuators.h"
#include "config.h"
#include "wifi.h"

extern myD myData;

void setActuatorsTask() {
    message_t msg;
    DigitalOut lightLed(LIGHTS_LED); // status of light switch
    DigitalOut heaterLed(HEATER_LED); // is the heater on?
    DigitalOut fanLed(FAN_LED); // is fan on?
    heaterLed = false;
    lightLed = false;
    fanLed = false;
    int seconds10 = 0;

    while (true) {
        if ( myData.light > (myData.lightSet + LIGHT_DZ)) {
            // Turn the lights off it's too bright in here
            lightLed = false;
            myData.lightState = false;
        }
        else if (myData.light < (myData.lightSet - LIGHT_DZ)) {
            // Turn the lights on its dark in here
            lightLed = true;
            myData.lightState = true;
        }
        if ( myData.temp > (myData.tempSet + HEATER_DZ)) {
            // Turn the fire off it's too hot in here
            heaterLed = false;
            myData.heaterState = false;
        }
        else if (myData.temp < (myData.tempSet - HEATER_DZ)) {
            // Turn the fire on its cold in here
            heaterLed = true;
            myData.heaterState = true;
        }
        if (myData.humidity > (myData.humiditySet + FAN_DZ)) {
          // Turn the fan off it's too dry in here
          fanLed = false;
          myData.fanState = false;
        } else if (myData.humidity < (myData.humiditySet - FAN_DZ)) {
          // Turn the fan on its damp in here
          fanLed = true;
          myData.fanState = true;
        }
        sprintf(msg.buffer, " %s ", myData.lightState?" ON ":"OFF " );
        msg.displayType = LIGHT_STATE;
        displayMessage(msg);
        ThisThread::sleep_for(10);
        sprintf(msg.buffer, " %s ", myData.fanState?" ON ":"OFF " );
        msg.displayType = FAN_STATE;
        displayMessage(msg);
        if (seconds10++ == 100) { // ten second period
          seconds10 = 0;
          sendPub(HEATER_STATE_TOPIC, myData.heaterState?1:0);
          sendPub(LIGHT_STATE_TOPIC, myData.lightState?1:0);
          sendPub(FAN_STATE_TOPIC, myData.fanState?1:0);
        } 

        ThisThread::sleep_for(100);        

    }
    
}