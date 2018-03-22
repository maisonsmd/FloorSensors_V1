//Mach Tiva
#include <SoftwareSerial.h>
#include "FloorSensorsMaster.h"
SoftwareSerial api(4, 3);

void setup()
{
	Serial.begin(115200);
	api.begin(57600);	//dung tam, sua sau
	FloorSensors.init(&api);
	//FloorSensors.init(&Serial);

	//Chon mau cho LED
	FloorSensors.setColor(RED);
	//FloorSensors.setColor(BLUE);
	//FloorSensors.setColor(WHITE);  <=> RED
}

void loop()
{

	FloorSensors.run();

	/*for(uint8_t i = 0; i < 7; i++)
	{
		Serial.print(String(FloorSensors.getSideAnalogState(i)) + ",");
	}*/
	Serial.println(FloorSensors.getSideOffset());
	delay(10);
	/*
	//Doc cam bien tiem can
	Serial.print(FloorSensors.getProxityState(FL));	//FL = Front Left, FC = Front Center, FR = Front Right
										//SF = Side Front, SM = Side Midle, SB = Side Back

	//Doc cam bien bam vach phia truoc
	uint8_t index = 0;
	Serial.print(FloorSensors.getSideAnalogState(index)); //index = 0...6, trai sang phai

	//Doc cam bien bam vach phia sau
	Serial.print(FloorSensors.getBackAnalogState(index)); // x = 0...6, trai sang phai
	*/
}
