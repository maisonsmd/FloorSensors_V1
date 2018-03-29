//Mach STM chinh tren board cam bien

#include "FloorSensors.h"
#include "DataGroup.h"
#include <EEPROM.h>

#define API_PORT	Serial1
#define MS_PORT		Serial3
#define DEBUG_PORT	Serial

#define TIME_OUT	100
#define LED			PC13

#define LOG(x)		DEBUG_PORT.print(x)
#define LOGHEX(x)	DEBUG_PORT.print(x, HEX)
#define LOGLN(x)	DEBUG_PORT.println(x)
#define LOGF(x)		DEBUG_PORT.print(F(x))
#define LOGLNF(x)	DEBUG_PORT.println(F(x))

#define COLOR_RED_PIN	PA7
#define COLOR_GREEN_PIN	PB1
#define COLOR_BLUE_PIN	PB0

#define	RED		0
#define GREEN	1
#define BLUE	2
#define WHITE	3
DataContainerClass ms_container;

uint8_t ms_group_idx;

//local sensor pin
uint8_t local_analog_pin[7] = { PA0,PA1,PA2, PA3, PA4, PA5, PA6 };

//data index in data container
uint8_t ms_prox_data_idx[6];
uint8_t ms_analog_data_idx[7];
uint8_t api_prox_values;
uint8_t api_side_analog_values;
uint8_t api_back_analog_values;

//sensors value
uint8_t ms_prox_val[7];
uint16_t ms_analog_val[7];
uint16_t local_analog_val[7];

boolean ms_is_busy = false;
uint32_t ms_request_tick = 0;

FloorSensorsClass Floor(ms_prox_val, ms_analog_val, local_analog_val);

boolean is_debugger_conneced = false;

void setup()
{
	API_PORT.begin(115200);
	MS_PORT.begin(250000);
	DEBUG_PORT.begin(115200);
	DEBUG_PORT.setTimeout(100);
	pinMode(LED, OUTPUT);
	pinMode(COLOR_RED_PIN, OUTPUT);
	pinMode(COLOR_GREEN_PIN, OUTPUT);
	pinMode(COLOR_BLUE_PIN, OUTPUT);

	for (uint8_t i = 0; i < 7; i++)
	{
		pinMode(local_analog_pin[i], INPUT);
	}

	ms_group_idx = ms_container.AddGroup();

	for (uint8_t i = 0; i < 6; i++)
	{
		ms_prox_data_idx[i] = ms_container.GetDataGroup(ms_group_idx)->AddAddress<uint8_t>();
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		ms_analog_data_idx[i] = ms_container.GetDataGroup(ms_group_idx)->AddAddress<uint16_t>();
	}

	setColor(BLUE);
	readEEPROM();
}

void loop()
{
	readLocalSensors();
	waitForSlave();
	Floor.run();
	//Serial.println(ms_prox_val[0]);

	//request from slave each 4ms
	static uint32_t timer = 0;
	if (millis() - timer > 4)
	{
		timer = millis();
		requestFromSlave();
		/*for (uint8_t i = 0; i < 7; i++)
		{
			LOG(ms_analog_val[i]);
			LOG('\t');
		}
		for (uint8_t i = 0; i < 7; i++)
		{
			LOG(local_analog_val[i]);
			LOG('\t');
		}
		LOGLN("");*/
	}
	HandleSerial();
	handleAPI();
}

void readLocalSensors()
{
	for (uint8_t i = 0; i < 7; i++)
		local_analog_val[i] = analogRead(local_analog_pin[i]);
}

void requestFromSlave()
{
	if (ms_is_busy == true)
	{
		LOGLN("SLAVE IS BUSY!");
		return;
	}
	ms_is_busy = true;
	ms_request_tick = millis();
	//slave's address
	MS_PORT.write(1);
	//'R' = Request
	MS_PORT.write('R');
	//data container index
	MS_PORT.write(ms_group_idx);
	//done
}

void handleAPI()
{
	api_prox_values = 0;
	api_back_analog_values = 0;
	api_side_analog_values = 0;
	for(uint8_t i = 0; i < 3; i++)
	{
		if (Floor.front_prox_sensor[i].getState() == 1)
			bitSet(api_prox_values, i);
		if (Floor.side_prox_sensor[i].getState() == 1)
			bitSet(api_prox_values, i + 3);
	}
	for(uint8_t i = 0; i < 7; i++)
	{
		if (Floor.side_analog_sensor[i].getState() == 1)
			bitSet(api_side_analog_values, i);
		if (Floor.back_analog_sensor[i].getState() == 1)
			bitSet(api_back_analog_values, i);
	}

	if (!API_PORT.available())
		return;
	String indata = "";
	while (API_PORT.available())
	{
		indata += char(API_PORT.read());
		api_waitByte();
	}
	//LOG(indata);
	//requesting
	if (indata.indexOf("RQ") >= 0)
	{
		API_PORT.write(api_prox_values);
		API_PORT.write(api_side_analog_values);
		API_PORT.write(api_back_analog_values);
	}
	if (indata.indexOf("SR") >= 0)
	{
		setColor(RED);
	}
	if (indata.indexOf("SB") >= 0)
	{
		setColor(BLUE);
	}
	if (indata.indexOf("SW") >= 0)
	{
		setColor(WHITE);
	}
	//this secsion just to blink the LED, don't care
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
			blink_duration = 15;
			current_duration++;
			break;
		case 1:
		case 3:
			digitalWrite(LED, HIGH);
			blink_duration = 200;
			current_duration++;
			break;
		case 4:
			digitalWrite(LED, HIGH);
			blink_duration = 500;
			current_duration = 0;
			break;
		}
	}
}

void waitForSlave()
{
	if (!MS_PORT.available())
	{
		if (millis() - ms_request_tick > TIME_OUT && ms_is_busy)
		{
			LOGLN("SLAVE TIME OUT");
			ms_is_busy = false;
		}
		return;
	}
	//new data comes
	ms_is_busy = false;
	byte data[PACKAGE_SIZE];
	uint8_t count = 0;
	//read data from slave
	//LOG("[");
	while (MS_PORT.available())
	{
		if (count > PACKAGE_SIZE)
		{
			LOGLN("MS BUFFER OVERFLOWS");
			//flush buffer
			while (MS_PORT.available())
				MS_PORT.read();
		}
		data[count] = MS_PORT.read();
		//LOGHEX(data[count]);
		//LOG(',');
		count++;
		ms_waitByte();
	}
	//LOGLN("]");
	ms_container.GetDataGroup(ms_group_idx)->SetBytes(data);
	//parse data
	for (uint8_t i = 0; i < 6; i++)
	{
		ms_prox_val[i] = ms_container.GetDataGroup(ms_group_idx)->GetValue<uint8_t>(ms_prox_data_idx[i]);
		//LOG(ms_prox_val[i]);
		//LOG(',');
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		ms_analog_val[i] = ms_container.GetDataGroup(ms_group_idx)->GetValue<uint16_t>(ms_analog_data_idx[i]);
		//LOG(ms_analog_val[i]);
		//LOG('\t');
	}
	//LOGLN("");
}

void ms_waitByte()
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

void api_waitByte()
{
	const uint32_t WAIT_BYTE_TIMEOUT = 1000;
	uint32_t timer_wait = micros();

	while (micros() - timer_wait < WAIT_BYTE_TIMEOUT)
	{
		if (API_PORT.available())
		{
			//LOG("TIME: ");
			//LOGLN(micros() - timer_wait);
			break;
		}
	}
}

void setColor(uint8_t c)
{
	static uint8_t lastColor = 255;
	switch (c)
	{
	case RED:
		digitalWrite(COLOR_RED_PIN, LOW);
		digitalWrite(COLOR_GREEN_PIN, HIGH);
		digitalWrite(COLOR_BLUE_PIN, LOW);
		for (uint8_t i = 0; i < 7; i++)
		{
			Floor.side_analog_sensor[i].setThreshold(Floor.side_threshold[i].red);
			Floor.back_analog_sensor[i].setThreshold(Floor.back_threshold[i].red);
		}
		if (lastColor != RED)
			LOGLN("COL: RED");
		break;
	case WHITE:
		digitalWrite(COLOR_RED_PIN, HIGH);
		digitalWrite(COLOR_GREEN_PIN, LOW);
		digitalWrite(COLOR_BLUE_PIN, LOW);
		for (uint8_t i = 0; i < 7; i++)
		{
			Floor.side_analog_sensor[i].setThreshold(Floor.side_threshold[i].white);
			Floor.back_analog_sensor[i].setThreshold(Floor.back_threshold[i].white);
		}
		if (lastColor != WHITE)
			LOGLN("COL: WHITE");
		break;
	case BLUE:
		digitalWrite(COLOR_RED_PIN, LOW);
		digitalWrite(COLOR_GREEN_PIN, HIGH);
		digitalWrite(COLOR_BLUE_PIN, LOW);
		for (uint8_t i = 0; i < 7; i++)
		{
			Floor.side_analog_sensor[i].setThreshold(Floor.side_threshold[i].blue);
			Floor.back_analog_sensor[i].setThreshold(Floor.back_threshold[i].blue);
		}
		if (lastColor != BLUE)
			LOGLN("COL: BLUE");
		break;
	}
	lastColor = c;
}
