/*
 * This thread will read any sensors attached to the Cypress board
 * and record the results in local memory.
 *
 */
#include "mbed.h"
#include "sensors.h"
#include "config.h"

void readSensorsTask() {

    DigitalOut vcc(VCC);
    DigitalOut gnd(GND);
    AnalogIn thermRead(THERM);

    vcc = 1;  // turn on the thermistor potential divider
    gnd = 0;

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
        printf("Temperature is %8.2f C\n", temperatureC );

        ThisThread::sleep_for(100);
    }
}