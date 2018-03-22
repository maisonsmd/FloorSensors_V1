//debug and calib
void HandleSerial()
{
	const byte START_BYTE = 100;
	const byte SAVE = 1;
	const byte SAVE_BACKUP = 2;
	const byte READ = 3;
	const byte READ_BACKUP = 4;
	const byte SET_RED = 5;
	const byte SET_BLUE = 6;
	const byte SET_WHITE = 7;
	const byte FRONT_RED = 8;
	const byte FRONT_BLUE = 10;
	const byte FRONT_WHITE = 12;
	const byte BACK_RED = 13;
	const byte BACK_BLUE = 14;
	const byte BACK_WHITE = 15;

	const uint8_t MAX_BYTE = 30;

	if (DEBUG_PORT.available() <= 5 && is_debugger_conneced == false)
		return;

	is_debugger_conneced = true;

	//section: send values
	static uint32_t timer_post_val = 0;
	/*
	0 = prox sensors,
	1 = front color sensors state,
	2 = front color sensors value,
	3 = back color sensors state,
	4 = back color sensors value,
	*/
	static uint8_t value_set_to_post = 0;

	if (millis() - timer_post_val > 10)
	{
		timer_post_val = millis();
		switch (value_set_to_post)
		{
		case 0:
			LOG("prox={");
			for (uint8_t i = 0; i < 3; i++)
				LOG(String(Floor.front_prox_sensor[i].getState()) + ",");
			for (uint8_t i = 0; i < 3; i++)
				LOG(String(Floor.side_prox_sensor[i].getState()) + ",");
			LOGLN("}");
			break;
		case 1:
			LOG("fs={");//front state
			for (uint8_t i = 0; i < 7; i++)
				LOG(String(Floor.side_analog_sensor[i].getState()) + ",");
			LOGLN("}");
			break;
		case 2:
			LOG("fv={");//front value
			for (uint8_t i = 0; i < 7; i++)
				//LOG(String(Floor.side_analog_sensor[i].getAverageVal()) + ",");
				LOG(String(Floor.side_analog_sensor[i].getRawVal()) + ",");
			LOGLN("}");
			break;
		case 3:
			LOG("bs={");//back state
			for (uint8_t i = 0; i < 7; i++)
				LOG(String(Floor.back_analog_sensor[i].getState()) + ",");
			LOGLN("}");
			break;
		case 4:
			LOG("bv={");//back value
			for (uint8_t i = 0; i < 7; i++)
				//LOG(String(Floor.back_analog_sensor[i].getAverageVal()) + ",");
				LOG(String(Floor.back_analog_sensor[i].getRawVal()) + ",");
			LOGLN("}");
			break;
		}
		//next set
		value_set_to_post++;
		if (value_set_to_post == 5)
			value_set_to_post = 0;
	}

	if (DEBUG_PORT.available() <= 0)
		return;

	byte data[MAX_BYTE];
	uint8_t count = 0;

	while (DEBUG_PORT.available())
	{
		if (count >= MAX_BYTE)
			while (DEBUG_PORT.available())
				DEBUG_PORT.read();
		data[count] = DEBUG_PORT.read();
		count++;
		debug_waitByte();
	}
	/*for (uint8_t i = 0; i < MAX_BYTE; i++)
	{
		DEBUG_PORT.print(data[i]);
		DEBUG_PORT.print(',');
	}
	DEBUG_PORT.println();*/
	if (data[0] != START_BYTE)
	{
		LOGLN("INFO: INVALID COMMAND");
		return;
	}
	uint16_t numbers[14];
	for (uint8_t i = 0; i < 14; i++)
	{
		numbers[i] = (mergeBytes<uint16_t>(data + 2 + 2 * i));
	}
	switch (data[1])
	{
	case SET_RED:
		LOGLN("INFO: set COLOR_RED");
		delay(300);
		setColor(RED);
		break;
	case SET_BLUE:
		LOGLN("INFO: set COLOR_BLUE");
		delay(300);
		setColor(BLUE);
		break;
	case SET_WHITE:
		LOGLN("INFO: set COLOR_WHITE");
		delay(300);
		setColor(WHITE);
		break;
	case FRONT_RED:
		for (uint8_t i = 0; i < 7; i++)
		{
			Floor.side_threshold[i].red.background = numbers[i];
			Floor.side_threshold[i].red.line = numbers[7 + i];
		}
		for (uint8_t i = 0; i < 7; i++)
			Floor.side_analog_sensor[i].setThreshold(Floor.side_threshold[i].red);
		LOGLN("INFO: set FRONT_RED");
		delay(100);
		break;
	case FRONT_BLUE:
		for (uint8_t i = 0; i < 7; i++)
		{
			Floor.side_threshold[i].blue.background = numbers[i];
			Floor.side_threshold[i].blue.line = numbers[7 + i];
		}
		for (uint8_t i = 0; i < 7; i++)
			Floor.side_analog_sensor[i].setThreshold(Floor.side_threshold[i].blue);
		LOGLN("INFO: set FRONT_BLUE");
		delay(100);
		break;
	case FRONT_WHITE:
		for (uint8_t i = 0; i < 7; i++)
		{
			Floor.side_threshold[i].white.background = numbers[i];
			Floor.side_threshold[i].white.line = numbers[7 + i];
		}
		for (uint8_t i = 0; i < 7; i++)
			Floor.side_analog_sensor[i].setThreshold(Floor.side_threshold[i].white);
		LOGLN("INFO: set FRONT_WHITE");
		delay(100);
		break;
	case BACK_RED:
		for (uint8_t i = 0; i < 7; i++)
		{
			Floor.back_threshold[i].red.background = numbers[i];
			Floor.back_threshold[i].red.line = numbers[7 + i];
		}
		for (uint8_t i = 0; i < 7; i++)
			Floor.back_analog_sensor[i].setThreshold(Floor.back_threshold[i].red);
		LOGLN("INFO: set BACK_RED");
		delay(100);
		break;
	case BACK_BLUE:
		for (uint8_t i = 0; i < 7; i++)
		{
			Floor.back_threshold[i].blue.background = numbers[i];
			Floor.back_threshold[i].blue.line = numbers[7 + i];
		}
		for (uint8_t i = 0; i < 7; i++)
			Floor.back_analog_sensor[i].setThreshold(Floor.back_threshold[i].blue);
		LOGLN("INFO: set BACK_BLUE");
		delay(100);
		break;
	case BACK_WHITE:
		for (uint8_t i = 0; i < 7; i++)
		{
			Floor.back_threshold[i].white.background = numbers[i];
			Floor.back_threshold[i].white.line = numbers[7 + i];
		}
		for (uint8_t i = 0; i < 7; i++)
			Floor.back_analog_sensor[i].setThreshold(Floor.back_threshold[i].white);
		LOGLN("INFO: set BACK_WHITE");
		delay(100);
		break;
	case SAVE:
		saveEEPROM();
		delay(500);
		break;
	case SAVE_BACKUP:
		saveEEPROM_backup();
		delay(500);
		break;
	case READ:
		readEEPROM();
		delay(50);
		break;
	case READ_BACKUP:
		readEEPROM_backup();
		delay(500);
		break;
	}
}
void debug_waitByte()
{
	const uint32_t WAIT_BYTE_TIMEOUT = 200;
	uint32_t timer_wait = micros();

	while (micros() - timer_wait < WAIT_BYTE_TIMEOUT)
	{
		if (DEBUG_PORT.available())
		{
			//LOG("TIME: ");
			//LOGLN(micros() - timer_wait);
			break;
		}
	}
}