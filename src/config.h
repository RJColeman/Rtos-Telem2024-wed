#ifndef CONFIG
#define CONFIG

#define VCC P10_3     // Potential divider voltage for thermistor
#define GND P10_0     // Potential divider ground for thermistor
#define THERM P10_1   // Thermistor connected to this port
#define LDR P10_4     // Light dependent resistor connection
#define DHT P10_5      // humidity sensor connection

#define R_REF 10000   // 10K resistor in potential divider

// thermistor characteristic Steinhart Hart Coefficients

/* A constant of NCP18XH103F03RB thermistor is */
#define A_COEFF                          (float)(0.0009032679f)
/* B constant of NCP18XH103F03RB thermistor is */
#define B_COEFF                          (float)(0.000248772f)
/* C constant of NCP18XH103F03RB thermistor is */
#define C_COEFF                          (float)(0.0000002041094f)

/* Zero Kelvin in degree C */
#define ABSOLUTE_ZERO                    (float)(-273.15)

#define TEMPERATURE_READING (uint8_t)(0)
#define TEMPERATURE_SETTING (uint8_t)(1)
#define HEATER_STATE (uint8_t)(2)
#define LIGHT_READING (uint8_t)(3)
#define LIGHT_SETTING (uint8_t)(4)
#define LIGHT_STATE (uint8_t)(5)
#define HUMIDITY_READING (uint8_t)(6)
#define HUMIDITY_SETTING (uint8_t)(7)
#define FAN_STATE (uint8_t)(8)
#define STATUS_DISPLAY (uint8_t)(9)

struct myD {
    float temp;
    float tempSet = 20.0; // default set temp is 20C
    int heaterState = 0; // heater is on or off initially off
    float light;
    float lightSet = 50.0; // default light level 50%
    int lightState = 0; // lights are off
    float humidity;
    float humiditySet = 60.0; // default humidity level 60%
    int fanState = 0; // fan is initially off
} ;

#define BROKER          "192.168.1.176"
#define PORT            1883

#define THING_NAME      "rjcThingy"

//#define TEMPERATURE_TOPIC   "/temperatureC"
//#define LIGHT_LEVEL_TOPIC   "/lightlevel"
#define ANNOUNCE_TOPIC      "/announce"
#define LIGHT_SET_TOPIC "rjcThingy/lightSet"
#define TEMP_SET_TOPIC "rjcThingy/tempSet"
#define HUMIDITY_SET_TOPIC "rjcThingy/humiditySet"

#endif