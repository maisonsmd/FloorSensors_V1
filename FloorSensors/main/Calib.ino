//debug and calib
void HandleSerial_fail()
{
	if (DEBUG_PORT.available() <= 0 && is_debugger_conneced == false)
		return;

	is_debugger_conneced = true;

	if (is_debugger_conneced == false)
		return;

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
	/*
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
				LOG(String(Floor.front_analog_sensor[i].getState()) + ",");
			LOGLN("}");
			break;
		case 2:
			LOG("fv={");//front value
			for (uint8_t i = 0; i < 7; i++)
				LOG(String(Floor.front_analog_sensor[i].getAverageVal()) + ",");
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
				LOG(String(Floor.back_analog_sensor[i].getAverageVal()) + ",");
			LOGLN("}");
			break;
		}
		//next set
		value_set_to_post++;
		if (value_set_to_post == 5)
			value_set_to_post = 0;
	}
	*/
	//section: read command
	String indata;
	if (DEBUG_PORT.available() <= 0)
		return;

	indata = DEBUG_PORT.readString();
	if (!(indata.indexOf('(') >= 0 && indata.indexOf(')') > 0))
	{
		LOGLN("INFO: INVALID COMMAND");
		return;
	}

	String command = indata.substring(indata.indexOf('(') + 1, indata.indexOf('='));
	indata = indata.substring(indata.indexOf('<') + 1, indata.indexOf('>'));
	//LOGLN(indata);
	//ex: (F-R-L=<512,565,1245,34,65,122,45,>)

	if (command == "save")
	{
		saveEEPROM();
		LOGLN("INFO: SAVED");
		delay(500);
		return;
	}
	if (command == "savebackup")
	{
		saveEEPROM_backup();
		LOGLN("INFO: SAVED BACKUP");
		delay(500);
		return;
	}
	if (command == "read")
	{
		saveEEPROM_backup();
		LOGLN("INFO: READ");
		delay(500);
		return;
	}
	if (command == "read")
	{
		saveEEPROM_backup();
		LOGLN("INFO: READ BACKUP");
		delay(500);
		return;
	}
	if (command == "red")
	{
		setColor(RED);
		return;
	}
	if (command == "blue")
	{
		setColor(BLUE);
		return;
	}
	if (command == "white")
	{
		// there's no white LED
		setColor(WHITE);
		return;
	}

	//parse data
	uint16_t val[7];
	LOG(command + " => ");
	for (uint8_t i = 0; i < 7; i++)
	{
		val[i] = indata.substring(0, indata.indexOf(',')).toInt();
		//trim to next
		indata = indata.substring(indata.indexOf(',') + 1, indata.length());
		LOG("[" + String(i) + "]=" + String(val[i]) + "\t");
	}
	LOGLN();

	//Front-RED-Line
	if (command == "F-R-L")
		for (uint8_t i = 0; i < 7; i++)
			Floor.front_threshold[i].red.lower = val[i];
	//Front-RED-Background
	if (command == "F-R-B")
		for (uint8_t i = 0; i < 7; i++)
			Floor.front_threshold[i].red.upper = val[i];
	/*if (command == "F-G-L")
		for (uint8_t i = 0; i < 7; i++)
			Floor.front_threshold[i].green.lower = val[i];
	if (command == "F-G-B")
		for (uint8_t i = 0; i < 7; i++)
			Floor.front_threshold[i].green.upper = val[i];*/
	if (command == "F-B-L")
		for (uint8_t i = 0; i < 7; i++)
			Floor.front_threshold[i].blue.lower = val[i];
	if (command == "F-B-B")
		for (uint8_t i = 0; i < 7; i++)
			Floor.front_threshold[i].blue.upper = val[i];
	if (command == "F-W-L")
		for (uint8_t i = 0; i < 7; i++)
			Floor.front_threshold[i].white.lower = val[i];
	if (command == "F-W-B")
		for (uint8_t i = 0; i < 7; i++)
			Floor.front_threshold[i].white.upper = val[i];


	//Back-RED-Line
	if (command == "B-R-L")
		for (uint8_t i = 0; i < 7; i++)
			Floor.front_threshold[i].red.lower = val[i];
	//Back-RED-Background
	if (command == "B-R-B")
		for (uint8_t i = 0; i < 7; i++)
			Floor.front_threshold[i].red.upper = val[i];
	/*if (command == "B-G-L")
		for (uint8_t i = 0; i < 7; i++)
			Floor.front_threshold[i].green.lower = val[i];
	if (command == "B-G-B")
		for (uint8_t i = 0; i < 7; i++)
			Floor.front_threshold[i].green.upper = val[i];*/
	if (command == "B-B-L")
		for (uint8_t i = 0; i < 7; i++)
			Floor.front_threshold[i].blue.lower = val[i];
	if (command == "B-B-B")
		for (uint8_t i = 0; i < 7; i++)
			Floor.front_threshold[i].blue.upper = val[i];
	if (command == "B-W-L")
		for (uint8_t i = 0; i < 7; i++)
			Floor.front_threshold[i].white.lower = val[i];
	if (command == "B-W-B")
		for (uint8_t i = 0; i < 7; i++)
			Floor.front_threshold[i].white.upper = val[i];

	//update thresholds when click buttons on GUI
	if (command.charAt(2) == 'R')
		for (uint8_t i = 0; i < 7; i++)
		{
			Floor.front_analog_sensor[i].setThreshold(Floor.front_threshold[i].red);
			Floor.back_analog_sensor[i].setThreshold(Floor.back_threshold[i].red);
		}
	if (command.charAt(2) == 'B')
		for (uint8_t i = 0; i < 7; i++)
		{
			Floor.front_analog_sensor[i].setThreshold(Floor.front_threshold[i].blue);
			Floor.back_analog_sensor[i].setThreshold(Floor.back_threshold[i].blue);
		}
	if (command.charAt(2) == 'W')
		for (uint8_t i = 0; i < 7; i++)
		{
			Floor.front_analog_sensor[i].setThreshold(Floor.front_threshold[i].white);
			Floor.back_analog_sensor[i].setThreshold(Floor.back_threshold[i].white);
		}

}

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
				LOG(String(Floor.front_analog_sensor[i].getState()) + ",");
			LOGLN("}");
			break;
		case 2:
			LOG("fv={");//front value
			for (uint8_t i = 0; i < 7; i++)
				LOG(String(Floor.front_analog_sensor[i].getAverageVal()) + ",");
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
				LOG(String(Floor.back_analog_sensor[i].getAverageVal()) + ",");
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
		setColor(RED);
		break;
	case SET_BLUE:
		setColor(BLUE);
		break;
	case SET_WHITE:
		setColor(WHITE);
		break;
	case FRONT_RED:
		for (uint8_t i = 0; i < 7; i++)
		{
			Floor.front_threshold[i].red.upper = numbers[i];
			Floor.front_threshold[i].red.lower = numbers[7 + i];
		}
		for (uint8_t i = 0; i < 7; i++)
			Floor.front_analog_sensor[i].setThreshold(Floor.front_threshold[i].red);
		break;
	case FRONT_BLUE:
		for (uint8_t i = 0; i < 7; i++)
		{
			Floor.front_threshold[i].blue.upper = numbers[i];
			Floor.front_threshold[i].blue.lower = numbers[7 + i];
		}
		for (uint8_t i = 0; i < 7; i++)
			Floor.front_analog_sensor[i].setThreshold(Floor.front_threshold[i].blue);
		break;
	case FRONT_WHITE:
		for (uint8_t i = 0; i < 7; i++)
		{
			Floor.back_threshold[i].white.upper = numbers[i];
			Floor.back_threshold[i].white.lower = numbers[7 + i];
		}
		for (uint8_t i = 0; i < 7; i++)
			Floor.front_analog_sensor[i].setThreshold(Floor.front_threshold[i].white);
		break;
	case BACK_RED:
		for (uint8_t i = 0; i < 7; i++)
		{
			Floor.back_threshold[i].red.upper = numbers[i];
			Floor.back_threshold[i].red.lower = numbers[7 + i];
		}
		for (uint8_t i = 0; i < 7; i++)
			Floor.back_analog_sensor[i].setThreshold(Floor.back_threshold[i].red);
		break;
	case BACK_BLUE:
		for (uint8_t i = 0; i < 7; i++)
		{
			Floor.back_threshold[i].blue.upper = numbers[i];
			Floor.back_threshold[i].blue.lower = numbers[7 + i];
		}
		for (uint8_t i = 0; i < 7; i++)
			Floor.back_analog_sensor[i].setThreshold(Floor.back_threshold[i].blue);
		break;
	case BACK_WHITE:
		for (uint8_t i = 0; i < 7; i++)
		{
			Floor.back_threshold[i].white.upper = numbers[i];
			Floor.back_threshold[i].white.lower = numbers[7 + i];
		}
		for (uint8_t i = 0; i < 7; i++)
			Floor.back_analog_sensor[i].setThreshold(Floor.back_threshold[i].white);
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
	const uint32_t WAIT_BYTE_TIMEOUT = 100;
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