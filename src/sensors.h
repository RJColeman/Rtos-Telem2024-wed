#ifndef SENSORS
#define SENSORS
#define VCC P10_3
#define GND P10_0
#define THERM P10_1
#define LDR P10_4
#define DHT P10_5
#define SENSOR_RATE 2000
void readSensorsTask();

#endif