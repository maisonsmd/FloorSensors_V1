// 
// 
// 

#include "FloorSensors.h"

void FloorSensorsClass::readSensors()
{
	/*for (uint8_t i = 0; i < 3; i++)
	{
		Serial.print(front_prox_sensor[i].getState());
		Serial.print('\t');
		//Serial.println(side_analog_sensor[0].getState());
	}	
	for (uint8_t i = 0; i < 3; i++)
	{
		Serial.print(side_prox_sensor[i].getState());
		Serial.print('\t');
		//Serial.println(side_analog_sensor[0].getState());
	}*/
	for (uint8_t i = 0; i < 7; i++)
	{
		Serial.print(side_analog_sensor[i].getRawVal());
		Serial.print('\t');
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		Serial.print(back_analog_sensor[i].getRawVal());
		Serial.print('\t');
	}
	Serial.println();
}

FloorSensorsClass::FloorSensorsClass(uint8_t * _prox_val_array, uint16_t * _side_analog_val_array, uint16_t * _back_analog_val_array)
{
	for (uint8_t i = 0; i < 3; i++)
	{
		front_prox_sensor[i].setRawVal(_prox_val_array + i);
		side_prox_sensor[i].setRawVal(_prox_val_array + (i + 3));
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		side_analog_sensor[i].setRawVal(_side_analog_val_array + i);
		back_analog_sensor[i].setRawVal(_back_analog_val_array + i);
	}
}

void FloorSensorsClass::run()
{
	for (uint8_t i = 0; i < 3; i++)
	{
		front_prox_sensor[i].run();
		side_prox_sensor[i].run();
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		side_analog_sensor[i].run();
		back_analog_sensor[i].run();
	}
	//readSensors();
}

