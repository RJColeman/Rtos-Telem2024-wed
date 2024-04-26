/*
 * This thread will read any sensors attached to the Cypress board
 * and record the results in local memory.
 *
 */
#include "mbed.h"
#include "sensors.h"
#include "config.h"
#include "display.h"
#include "wifi.h"
#include "DHT22.h"

#define timeout 200

myD myData;

void readSensorsTask() {
    message_t msg;
    int seconds10 = 0;

    DigitalOut vcc(VCC);
    DigitalOut gnd(GND);
    AnalogIn thermRead(THERM);
    AnalogIn lightRead(LDR);
    AnalogIn humidityRead(DHT);

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
    sprintf(msg.buffer, "%2.0f %c ", myData.humiditySet, '%');
    msg.displayType = HUMIDITY_SETTING;
    displayMessage(msg);
    sprintf(msg.buffer, "%s", (myData.fanState?" ON":"OFF"));
    msg.displayType = FAN_STATE;
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
        sprintf(msg.buffer, "%3.1f %c ", lightLevel, '%' );
        msg.displayType = LIGHT_READING;
        myData.light = lightLevel;
        displayMessage(msg);
        ThisThread::sleep_for(10);

// get humidity sensor information
  /*      DHT22::DHT22(PinName pin) {
    _data_pin = pin;
    DigitalInOut dht(_data_pin);
    dht.output();
    dht.write(1);
}

int DHT22::getTemperature() {
    return _temperature;
}

int DHT22::getHumidit() {
    return _humidity;
}

bool DHT22::sample() {
    __disable_irq();    // Disable Interrupts
    Timer t;
    DigitalInOut dht(_data_pin);
    int dht22_dat [5];
    dht.output();
    dht.write(0);
    wait_ms(1);
    dht.write(1);
    dht.input();
    wait_us(40);
    wait_us(80);
    int i,j,result=0;
    for (i=0; i<5; i++) {
        result=0; 
        for (j=0; j<8; j++) {
            while(dht.read());
            while(!dht.read());
            t.reset();
            t.start();
            while(dht.read());
            t.stop();
            int p;
            if(t.read_us()>50)
                p = 1;
            else
                p = 0;
            p=p <<(7-j);
            result=result|p;
        }
        dht22_dat[i] = result;
    }
    dht.output();
    dht.write(1);
    int dht22_check_sum;
    dht22_check_sum=dht22_dat[0]+dht22_dat[1]+dht22_dat[2]+dht22_dat[3];
    dht22_check_sum= dht22_check_sum%256;
    if (dht22_check_sum==dht22_dat[4]) {
        _humidity=dht22_dat[0]*256+dht22_dat[1];
        _temperature=dht22_dat[2]*256+dht22_dat[3];
        __enable_irq();
        return true;
    }
    __enable_irq();
    return false;
    humidityLevel = _humidity;
} */

        float humidityLevel = humidityRead.read() * 100;// Ambient humidity level measurement 0-100%
        sprintf(msg.buffer, "%3.1f %c ", humidityLevel, '%' );
        msg.displayType = HUMIDITY_READING;
        myData.humidity = humidityLevel;
        displayMessage(msg);

        ThisThread::sleep_for(90);
        if (seconds10++ == 100) { // ten second period
          seconds10 = 0;

          sendPub(TEMPERATURE_TOPIC, temperatureC);
          sendPub(LIGHT_LEVEL_TOPIC, lightLevel);
          sendPub(HUMIDITY_LEVEL_TOPIC, humidityLevel);
        } 

    }
}