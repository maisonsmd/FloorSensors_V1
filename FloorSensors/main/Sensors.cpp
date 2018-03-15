// 
// 
// 

#include "Sensors.h"


void ProximitySensor::run(void)
{
	current_state = * raw_val;
	//check if variables overflowed
	if (tmr_debounce > millis())
		tmr_debounce = millis();

	//reset timer when changed state
	if (!(current_state == last_state))
		tmr_debounce = millis();

	//stayed stable in a predefined duration
	if (millis() - tmr_debounce > PROX_DEBOUNCE_DURATION)
		filted_state = current_state;
	//update last value
	last_state = current_state;
}

void ProximitySensor::setRawVal(uint8_t * _raw_val)
{
	raw_val = _raw_val;
}

uint8_t ProximitySensor::getState(void)
{
	return filted_state;
}

void AnalogSensor::run()
{
	if (*raw_val > threshold.upper)
		current_state = 0;
	if (*raw_val < threshold.lower)
		current_state = 1;

	if (current_state != last_state)
		tmr_debounce = millis();
	
	if (millis() - tmr_debounce > COLOR_DEBOUNCE_DURATION)
		filted_state = current_state;

	last_state = current_state;
	average_val = (uint16_t(float(average_val) * 0.9 + float(*raw_val) * 0.1));
}

void AnalogSensor::setRawVal(uint16_t * _raw_val)
{
	raw_val = _raw_val;
}

void AnalogSensor::setThreshold(Threshold th)
{
	threshold = th;
}

uint16_t AnalogSensor::getRawVal(void)
{
	return * raw_val;
}

uint16_t AnalogSensor::getAverageVal(void)
{
	return average_val;
}

uint8_t AnalogSensor::getState(void)
{
	return filted_state;
}
