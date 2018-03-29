#include <SoftwareSerial.h>
#include "FloorSensorsMaster.h"

#define BAUD		115200
#define RED			0
#define	BLUE		1
#define WHITE		2

#define RED_PIN		A3
#define GREEN_PIN	A2
#define BLUE_PIN	13

#define LAT_PIN		11
#define CLK_PIN		12
#define DAT_PIN		10

#define FL_PIN		9
#define FC_PIN		8
#define FR_PIN		7
#define SF_PIN		6
#define SM_PIN		5
#define SB_PIN		4

uint8_t prox_pin[6] = { FL_PIN, FC_PIN, FR_PIN, SF_PIN, SM_PIN, SB_PIN };

SoftwareSerial requester(A0, 2);
//SoftwareSerial responser(A1, 3);
#define responser	Serial

void setColor(uint8_t color)
{
	switch (color)
	{
	case RED:
		digitalWrite(RED_PIN, HIGH);
		digitalWrite(GREEN_PIN, LOW);
		digitalWrite(BLUE_PIN, LOW);
		break;
	case BLUE:
		digitalWrite(RED_PIN, LOW);
		digitalWrite(GREEN_PIN, LOW);
		digitalWrite(BLUE_PIN, HIGH);
		break;
	case WHITE:
		digitalWrite(RED_PIN, HIGH);
		digitalWrite(GREEN_PIN, HIGH);
		digitalWrite(BLUE_PIN, HIGH);
		break;
	}
}

void setup()
{
	pinMode(RED_PIN, OUTPUT);
	pinMode(GREEN_PIN, OUTPUT);
	pinMode(BLUE_PIN, OUTPUT);
	pinMode(CLK_PIN, OUTPUT);
	pinMode(DAT_PIN, OUTPUT);
	pinMode(LAT_PIN, OUTPUT);
	for (uint8_t i = 0; i < 6; i++)
	{
		pinMode(prox_pin[i], OUTPUT);
		digitalWrite(prox_pin[i], HIGH);
		if(i > 0)
			digitalWrite(prox_pin[i-1], LOW);
		delay(100);
	}
	digitalWrite(prox_pin[5], LOW);
	for (uint8_t i = 0; i < 8; i++)
	{
		digitalWrite(LAT_PIN, LOW);
		shiftOut(DAT_PIN, CLK_PIN, LSBFIRST, 0x01 << i);
		shiftOut(DAT_PIN, CLK_PIN, MSBFIRST, 0);
		digitalWrite(LAT_PIN, HIGH);
		delay(50);
	}
	for (uint8_t i = 0; i < 8; i++)
	{
		digitalWrite(LAT_PIN, LOW);
		shiftOut(DAT_PIN, CLK_PIN, LSBFIRST, 0);
		shiftOut(DAT_PIN, CLK_PIN, MSBFIRST, 0x01 << i);
		digitalWrite(LAT_PIN, HIGH);
		delay(50);
	}
	setColor(WHITE);
	delay(300);
	setColor(RED);
	delay(300);
	setColor(BLUE);
	delay(300);
	requester.begin(115200);
	responser.begin(115200);
	//Serial.begin(115200);
	FloorSensors.init(&responser);
}

void loop()
{
	if (requester.available())
	{
		String s = "";
		while (requester.available())
		{
			s += char(requester.read());
		}
		if (s.indexOf("SR") >= 0)
			setColor(RED);
		if (s.indexOf("SB") >= 0)
			setColor(BLUE);
		if (s.indexOf("SW") >= 0)
			setColor(WHITE);
	}
	//FloorSensors.run();	//we dont want to send any data
	FloorSensors.waitForSlave();
	/*static uint32_t timer = 0;
	if (millis() - timer < 5)
		return;
	timer = millis();*/
	digitalWrite(LAT_PIN, LOW);
	shiftOut(DAT_PIN, CLK_PIN, LSBFIRST, FloorSensors.api_back_analog_values << 1);
	shiftOut(DAT_PIN, CLK_PIN, MSBFIRST, FloorSensors.api_side_analog_values << 1);
	digitalWrite(LAT_PIN, HIGH);
	for (uint8_t i = FL; i <= SB; i++)
		digitalWrite(prox_pin[i], FloorSensors.getProximityState(i));
}
