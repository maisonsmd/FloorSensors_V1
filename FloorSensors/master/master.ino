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
}

void loop()
{

	FloorSensors.run();

	//Chon mau cho LED
	FloorSensors.setColor(RED);
	//FloorSensors.setColor(BLUE);
	//FloorSensors.setColor(WHITE);  <=> RED


	//Doc cam bien tiem can
	Serial.print(FloorSensors.getProxityState(FL));	//FL = Front Left, FC = Front Center, FR = Front Right
										//SF = Side Front, SM = Side Midle, SB = Side Back


	//Doc cam bien bam vach phia truoc
	uint8_t index = 0;
	Serial.print(FloorSensors.getFrontAnalogState(index)); //index = 0...6, trai sang phai

	//Doc cam bien bam vach phia sau
	Serial.print(FloorSensors.getBackAnalogState(index)); // x = 0...6, trai sang phai
}
