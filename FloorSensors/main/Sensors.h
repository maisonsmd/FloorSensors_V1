// Sensors.h

#ifndef _SENSORS_h
#define _SENSORS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define	PROX_DEBOUNCE_DURATION	5
#define COLOR_DEBOUNCE_DURATION	0

struct Threshold	//hold the analog threshold to decide which color is.
{
	uint16_t upper;
	uint16_t lower;
};

struct ColorThresholds
{
	Threshold red;
	Threshold blue;
	Threshold white;
};

class ProximitySensor
{
private:
	uint8_t * raw_val;
	uint8_t filted_state;
	uint32_t tmr_debounce;
	uint8_t current_state;
	uint8_t last_state;
public:
	void run(void);
	void setRawVal(uint8_t * _raw_val);
	uint8_t getState(void);
};

class AnalogSensor
{
private:
	uint16_t * raw_val;
	Threshold threshold;
	uint8_t filted_state;
	uint32_t tmr_debounce;
	uint8_t current_state;
	uint8_t last_state;
	uint16_t average_val;
public:
	void run();
	void setRawVal(uint16_t * _raw_val);
	void setThreshold(Threshold th);
	uint16_t getRawVal(void);
	uint16_t getAverageVal(void);
	uint8_t getState(void);
};


#endif

