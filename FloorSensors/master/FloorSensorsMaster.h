// FloorSensorsMaster.h

#ifndef _FLOORSENSORSMASTER_h
#define _FLOORSENSORSMASTER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "DataGroup.h"

//#define DEBUG	Serial

#ifdef DEBUG
#define LOG(x)		DEBUG.print(x)
#define LOGHEX(x)	DEBUG.print(x, HEX)
#define LOGLN(x)	DEBUG.println(x)
#define LOGF(x)		DEBUG.print(F(x))
#define LOGLNF(x)	DEBUG.println(F(x))
#else
#define LOG(x)		
#define LOGHEX(x)	
#define LOGLN(x)	
#define LOGF(x)		
#define LOGLNF(x)
#endif // DEBUG

#define TIME_OUT 100

#define API_BAUD	115200

#define FL	0
#define FC	1
#define FR	2
#define SF	3
#define SM	4
#define SB	5

#define REQUEST_BYTE	100

#define RED	0
#define BLUE 1
#define WHITE 2

class FloorSensorsMaster
{
protected:
	Stream * port;
	DataContainerClass api_container;
	uint8_t api_group_idx;
	//data index in data container
	uint8_t api_prox_data_idx[6];
	uint8_t api_analog_data_idx[14];	//7 front + 7 back

	//sensors value
	uint8_t api_prox_state[6];
	uint8_t api_analog_state[14]; 
	boolean api_is_busy = false;
	uint32_t api_request_tick = 0;

	uint8_t color = RED;

	void api_waitByte()
	{
		const uint32_t WAIT_BYTE_TIMEOUT = 1000;
		uint32_t timer_wait = micros();

		while (micros() - timer_wait < WAIT_BYTE_TIMEOUT)
		{
			if (port->available())
			{
				//LOG("TIME: ");
				//LOGLN(micros() - timer_wait);
				break;
			}
		}
	}

	void request(void);
	void waitForSlave(void);
public:
	void init(Stream * _port);
	void run();
	uint8_t getProxityState(uint8_t name);
	uint8_t getFrontAnalogState(uint8_t index);
	uint8_t getBackAnalogState(uint8_t index);
	//float getAngle();
	//float getOffset();
	void setColor(uint8_t _color);
};

extern FloorSensorsMaster FloorSensors;

#endif

