// FloorSensors.h

#ifndef _FLOORSENSORS_h
#define _FLOORSENSORS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Sensors.h"

#define LEFT	0
#define CENTER	1
#define RIGHT	2

#define FRONT	0
#define MIDDLE	1
#define BACK	2

//distance between 2 sensor array (mm)
#define SENSOR_ARRAY_DISTANCE	500

class FloorSensorsClass
{
private:
	//void readSensors();
public:
	//thresholds for colors
	ColorThresholds side_threshold[7];
	ColorThresholds back_threshold[7];

	ProximitySensor front_prox_sensor[3];
	ProximitySensor side_prox_sensor[3];

	AnalogSensor side_analog_sensor[7];
	AnalogSensor back_analog_sensor[7];

	FloorSensorsClass(uint8_t * _prox_val_array, 
		uint16_t * _side_analog_val_array,
		uint16_t * _back_analog_val_array);
	void run();
};

#endif

