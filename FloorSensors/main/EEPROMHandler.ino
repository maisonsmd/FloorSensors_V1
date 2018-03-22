#pragma once

void readEEPROM()
{
	uint16_t currentIdx = 0;
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.read(currentIdx, &Floor.side_threshold[i].red.background);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.read(currentIdx, &Floor.side_threshold[i].red.line);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.read(currentIdx, &Floor.side_threshold[i].blue.background);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.read(currentIdx, &Floor.side_threshold[i].blue.line);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.read(currentIdx, &Floor.side_threshold[i].white.background);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.read(currentIdx, &Floor.side_threshold[i].white.line);
		currentIdx += sizeof(uint16_t);
	}
	///////////////////

	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.read(currentIdx, &Floor.back_threshold[i].red.background);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.read(currentIdx, &Floor.back_threshold[i].red.line);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.read(currentIdx, &Floor.back_threshold[i].blue.background);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.read(currentIdx, &Floor.back_threshold[i].blue.line);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.read(currentIdx, &Floor.back_threshold[i].white.background);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.read(currentIdx, &Floor.back_threshold[i].white.line);
		currentIdx += sizeof(uint16_t);
	}
	setColor(RED);
	LOG("READ EPPROM DONE, TOTAL ");
	LOG(currentIdx);
	LOGLN(" WORDS");
}
void readEEPROM_backup()
{
	uint16_t currentIdx = 200;
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.read(currentIdx, &Floor.side_threshold[i].red.background);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.read(currentIdx, &Floor.side_threshold[i].red.line);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.read(currentIdx, &Floor.side_threshold[i].blue.background);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.read(currentIdx, &Floor.side_threshold[i].blue.line);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.read(currentIdx, &Floor.side_threshold[i].white.background);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.read(currentIdx, &Floor.side_threshold[i].white.line);
		currentIdx += sizeof(uint16_t);
	}
	///////////////////

	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.read(currentIdx, &Floor.back_threshold[i].red.background);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.read(currentIdx, &Floor.back_threshold[i].red.line);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.read(currentIdx, &Floor.back_threshold[i].blue.background);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.read(currentIdx, &Floor.back_threshold[i].blue.line);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.read(currentIdx, &Floor.back_threshold[i].white.background);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.read(currentIdx, &Floor.back_threshold[i].white.line);
		currentIdx += sizeof(uint16_t);
	}
	setColor(BLUE);
	LOG("READ BACKUP DONE, TOTAL ");
	LOG(currentIdx - 200);
	LOGLN(" WORDS");
}
void saveEEPROM()
{
	uint16_t currentIdx = 0;
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.write(currentIdx, Floor.side_threshold[i].red.background);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.write(currentIdx, Floor.side_threshold[i].red.line);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.write(currentIdx, Floor.side_threshold[i].blue.background);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.write(currentIdx, Floor.side_threshold[i].blue.line);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.write(currentIdx, Floor.side_threshold[i].white.background);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.write(currentIdx, Floor.side_threshold[i].white.line);
		currentIdx += sizeof(uint16_t);
	}
	///////////////////

	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.write(currentIdx, Floor.back_threshold[i].red.background);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.write(currentIdx, Floor.back_threshold[i].red.line);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.write(currentIdx, Floor.back_threshold[i].blue.background);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.write(currentIdx, Floor.back_threshold[i].blue.line);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.write(currentIdx, Floor.back_threshold[i].white.background);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.write(currentIdx, Floor.back_threshold[i].white.line);
		currentIdx += sizeof(uint16_t);
	}
	LOG("SAVE EPPROM DONE, TOTAL ");
	LOG(currentIdx);
	LOGLN(" WORDS");
}
void saveEEPROM_backup()
{
	uint16_t currentIdx = 200;
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.write(currentIdx, Floor.side_threshold[i].red.background);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.write(currentIdx, Floor.side_threshold[i].red.line);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.write(currentIdx, Floor.side_threshold[i].blue.background);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.write(currentIdx, Floor.side_threshold[i].blue.line);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.write(currentIdx, Floor.side_threshold[i].white.background);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.write(currentIdx, Floor.side_threshold[i].white.line);
		currentIdx += sizeof(uint16_t);
	}
	///////////////////

	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.write(currentIdx, Floor.back_threshold[i].red.background);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.write(currentIdx, Floor.back_threshold[i].red.line);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.write(currentIdx, Floor.back_threshold[i].blue.background);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.write(currentIdx, Floor.back_threshold[i].blue.line);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.write(currentIdx, Floor.back_threshold[i].white.background);
		currentIdx += sizeof(uint16_t);
	}
	for (uint8_t i = 0; i < 7; i++)
	{
		EEPROM.write(currentIdx, Floor.back_threshold[i].white.line);
		currentIdx += sizeof(uint16_t);
	}
	LOG("SAVE BACKUP DONE, TOTAL ");
	LOG(currentIdx - 200);
	LOGLN(" WORDS");
}
