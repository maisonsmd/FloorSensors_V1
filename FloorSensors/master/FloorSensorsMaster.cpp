// 
// 
// 

#include "FloorSensorsMaster.h"

void FloorSensorsMaster::request(void)
{
	if (api_is_busy == true)
	{
		LOGLN("SLAVE IS BUSY!");
		return;
	}
	api_is_busy = true;
	api_request_tick = millis();
	//Request
	port->println("RQ");
	switch (color)
	{
	case RED:
		port->println("SR");
		break;
	case BLUE:
		port->println("SB");
		break;
	case WHITE:
		port->println("SW");
		break;
	}
	//done
}

void FloorSensorsMaster::waitForSlave(void)
{
	if (!port->available())
	{
		if (millis() - api_request_tick > TIME_OUT && api_is_busy)
		{
			LOGLN("SLAVE TIME OUT");
			api_is_busy = false;
		}
		return;
	}
	//new data comes
	api_is_busy = false;
	byte data[PACKAGE_SIZE];
	uint8_t count = 0;
	//read data from slave
	//LOG("[");
	while (port->available())
	{
		if (count > PACKAGE_SIZE)
		{
			LOGLN("MS BUFFER OVERFLOWS");
			//flush buffer
			while (port->available())
				port->read();
		}
		data[count] = port->read();
		//LOGHEX(data[count]);
		//LOG(',');
		count++;
		api_waitByte();
	}
	//LOGLN("]");
	api_container.GetDataGroup(api_group_idx)->SetBytes(data);
	//parse data
	for (uint8_t i = 0; i < 6; i++)
	{
		api_prox_state[i] = api_container.GetDataGroup(api_group_idx)->GetValue<uint8_t>(api_prox_data_idx[i]);
		//LOG(api_prox_state[i]);
		//LOG(',');
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		api_analog_state[i] = api_container.GetDataGroup(api_group_idx)->GetValue<uint16_t>(api_analog_data_idx[i]);
		api_analog_state[7+i] = api_container.GetDataGroup(api_group_idx)->GetValue<uint16_t>(api_analog_data_idx[7+i]);
		//LOG(api_analog_state[i]);
		//LOG('\t');
	}
	//LOGLN("");
}

void FloorSensorsMaster::init(Stream * _port)
{
	port = _port;
	//port->begin(API_BAUD);
	api_group_idx = api_container.AddGroup();
	for (uint8_t i = 0; i < 6; i++)
	{
		api_prox_data_idx[i] = api_container.GetDataGroup(api_group_idx)->AddAddress<uint8_t>();
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		api_analog_data_idx[i] = api_container.GetDataGroup(api_group_idx)->AddAddress<uint8_t>();
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		api_analog_data_idx[i + 7] = api_container.GetDataGroup(api_group_idx)->AddAddress<uint8_t>();
	}
}

void FloorSensorsMaster::run()
{
	//request from slave each 4ms
	static uint32_t timer = 0;
	if (millis() - timer > 5)
	{
		timer = millis();
		request();
	}
	waitForSlave();
}

uint8_t FloorSensorsMaster::getProxityState(uint8_t name)
{
	return api_prox_state[name];
}

uint8_t FloorSensorsMaster::getFrontAnalogState(uint8_t index)
{
	return api_analog_state[index];
}

uint8_t FloorSensorsMaster::getBackAnalogState(uint8_t index)
{
	return (api_analog_state + 7)[index];
}

void FloorSensorsMaster::setColor(uint8_t _color)
{
	color = _color;
}


FloorSensorsMaster FloorSensors;

