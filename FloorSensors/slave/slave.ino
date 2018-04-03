//Nap cho mach STM phu tren board cam bien
#include "DataGroup.h"

#define MS_PORT		Serial3
#define DEBUG_PORT	Serial

#define LED			PC13
#define MY_ADDRESS	1

#define LOG(x)		DEBUG_PORT.print(x)
#define LOGHEX(x)	DEBUG_PORT.print(x, HEX)
#define LOGLN(x)	DEBUG_PORT.println(x)
#define LOGF(x)		DEBUG_PORT.print(F(x))
#define LOGLNF(x)	DEBUG_PORT.println(F(x))

//pin container
const uint8_t prox_pin[6] = { PB4, PB3, PA15, PB14, PB13, PB12 };
const uint8_t analog_pin[7] = { PA0, PA1, PA2, PA3, PA4, PA5, PA6 };
//data index in data container
uint8_t ms_prox_data_idx[6];
uint8_t ms_analog_data_idx[7];

DataContainerClass ms_container;
//group index
uint8_t ms_group_idx;
/*
boolean ms_is_busy = false;
uint32_t ms_request_tick = 0;*/

/*//sensors data index
uint8_t ms_analog_idx[7];
uint8_t ms_prox_idx[7];*/

//sensors value
uint16_t ms_analog_val[7];
uint8_t ms_prox_val[7];
void setup()
{
	DEBUG_PORT.begin(115200);
	MS_PORT.begin(250000);
	MS_PORT.setTimeout(5);
	pinMode(LED, OUTPUT);

	ms_group_idx = ms_container.AddGroup();

	for (uint8_t i = 0; i < 6; i++)
	{
		pinMode(prox_pin[i], INPUT);
		ms_prox_data_idx[i] = ms_container.GetDataGroup(ms_group_idx)->AddAddress<uint8_t>();
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		//pinMode(analog_pin[i], INPUT);
		ms_analog_data_idx[i] = ms_container.GetDataGroup(ms_group_idx)->AddAddress<uint16_t>();
	}
}

void loop()
{
	readSensors();
	waitCommand();
}

void readSensors()
{
	for (uint8_t i = 0; i < 6; i++)
		ms_container.GetDataGroup(ms_group_idx)->SetValue<uint8_t>(ms_prox_data_idx[i], !digitalRead(prox_pin[i]));
	for (uint8_t i = 0; i < 7; i++)
		ms_container.GetDataGroup(ms_group_idx)->SetValue<uint16_t>(ms_analog_data_idx[i], analogRead(analog_pin[i]));
}
void waitCommand()
{
	byte data[PACKAGE_SIZE];
	uint8_t count = 0;
	if (!MS_PORT.available())
		return;
	LOGLN("");
	/*while (MS_PORT.available())
	{
		//LOGHEX(MS_PORT.read());
		//LOG(",");
		//continue;
		if (count > PACKAGE_SIZE)
		{
			LOGLN("MS BUFFER OVERFLOWS");
			//flush buffer
			while (MS_PORT.available())
				MS_PORT.read();
		}
		data[count] = uint8_t(MS_PORT.read());
		//LOGHEX(data[count]);
		//LOG(',');
		count++;
		waitByte();
	}*/
	MS_PORT.readBytes(data, sizeof(data));
	//I'm not the one he's talking to
	if (data[0] != MY_ADDRESS)
		return;
	//requesting
	if (data[1] == 'R')
	{
		for (uint8_t i = 0; i < PACKAGE_SIZE; i++)
		{
			MS_PORT.write(ms_container.GetDataGroup(ms_group_idx)->GetBytes()[i]);
		}
		LOGLN("RESPONSED");
	}
	//this secsion just to blink LED, don't care
	static uint32_t timer_blink = 0;
	static uint8_t current_duration = 0;
	static uint32_t blink_duration = 0;
	if (millis() - timer_blink > blink_duration)
	{
		timer_blink = millis();
		switch (current_duration)
		{
		case 0:
		case 2:
			digitalWrite(LED, LOW);
			blink_duration = 7;
			current_duration++;
			break;
		case 1:
		case 3:
			digitalWrite(LED, HIGH);
			blink_duration = 150;
			current_duration++;
			break;
		case 4:
			digitalWrite(LED, HIGH);
			blink_duration = 400;
			current_duration = 0;
			break;
		}
	}
}

void waitByte()
{
	const uint32_t WAIT_BYTE_TIMEOUT = 100;
	uint32_t timer_wait = micros();

	while (micros() - timer_wait < WAIT_BYTE_TIMEOUT)
	{
		if (MS_PORT.available())
		{
			//LOG("TIME: ");
			//LOGLN(micros() - timer_wait);
			break;
		}
	}
}