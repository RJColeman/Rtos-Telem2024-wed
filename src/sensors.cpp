/*
 * This thread will read any sensors attached to the Cypress board
 * and record the results in local memory.
 *
 */
#include "mbed.h"
#include "sensors.h"
#include "config.h"
#include "display.h"

myD myData;

void readSensorsTask() {
    message_t msg;

    DigitalOut vcc(VCC);
    DigitalOut gnd(GND);
    AnalogIn thermRead(THERM);
    AnalogIn lightRead(LDR);

    vcc = 1;  // turn on the thermistor potential divider
    gnd = 0;
    sprintf(msg.buffer, "%2.1f C ", myData.tempSet);
    msg.displayType = TEMPERATURE_SETTING;
    displayMessage(msg);
    sprintf(msg.buffer, "%s", (myData.heaterState?" ON":"OFF"));
    msg.displayType = HEATER_STATE;
    displayMessage(msg);
    sprintf(msg.buffer, "%2.0f %c ", myData.lightSet, '%');
    msg.displayType = LIGHT_SETTING;
    displayMessage(msg);
    sprintf(msg.buffer, "%s", (myData.lightState?" ON":"OFF"));
    msg.displayType = LIGHT_STATE;
    displayMessage(msg);
    while (true) {
        float thermVolts = thermRead.read() * 2.4; // convert adc reading to volts
        float current = (3.3 - thermVolts) / R_REF;
        float thermResistance = thermVolts / current;
        float logrT = (float32_t)log((float64_t)thermResistance);
        /* Calculate temperature from the resistance of thermistor using
        * Steinhart-Hart Equation */
        float stEqn = (float32_t)((A_COEFF) + ((B_COEFF)*logrT) +
                                  ((C_COEFF)*pow((float64)logrT, (float32)3)));
        float temperatureC = (float32_t)(((1.0 / stEqn) + ABSOLUTE_ZERO) + 0.05);
        sprintf(msg.buffer, "%2.1f C ", temperatureC );
        msg.displayType = TEMPERATURE_READING;
        myData.temp = temperatureC;
        displayMessage(msg);
        ThisThread::sleep_for(10);
        float lightLevel = lightRead.read() * 100;// Ambient light level measurement 0-100%
        myData.light = lightLevel;
        sprintf(msg.buffer, "%3.1f %c ", lightLevel, '%' );
        msg.displayType = LIGHT_READING;
        displayMessage(msg);

        ThisThread::sleep_for(100);
    }
}