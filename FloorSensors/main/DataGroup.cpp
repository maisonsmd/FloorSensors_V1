// 
// 
// 

#include "DataGroup.h"

DataGroupClass::DataGroupClass()
{
	memset(_bytes, 0, MAX_DATA_SIZE);
}

void DataGroupClass::SetGroupID(uint8_t id)
{
	//Serial.println("Set GRID: " + String(id));
	_groupID = id;
}

uint8_t DataGroupClass::GetGroupID()
{
	//Serial.println("Get GRID: " + String(_groupID));
	return _groupID;
}

uint8_t * DataGroupClass::GetGroupMap(uint8_t * size)
{
	if (size != nullptr)
		*size = _groupMembersCount;
	return _groupMap;
}

uint8_t DataGroupClass::GetGroupSize()
{
	return _groupSize;
}

uint8_t DataGroupClass::GetGroupMembersCount()
{
	return _groupMembersCount;
}

uint8_t * DataGroupClass::GetBytes()
{
	return _bytes;
}

void DataGroupClass::SetBytes(uint8_t arr[MAX_DATA_SIZE])
{
	for (uint8_t i = 0; i < MAX_DATA_SIZE; i++)
		_bytes[i] = arr[i];
}

//////////////////////////////////////////////////////////////////////////////////////
uint8_t DataContainerClass::AddGroup(const DataGroupClass & dataGr)
{
	DataGroupClass * tempDataGroup = new DataGroupClass[_groupsCount + 1];
	//copy all data to new array
	for (uint8_t i = 0; i < _groupsCount; i++)
		tempDataGroup[i] = _dataGroup[i];
	//add last data
	tempDataGroup[_groupsCount] = dataGr;
	tempDataGroup[_groupsCount].SetGroupID(_groupsCount);
	//delete previous array
	if (_dataGroup != nullptr)
		delete[] _dataGroup;
	//point to new array
	_dataGroup = tempDataGroup;
	_groupsCount++;
	return _groupsCount - 1; //- 1 for index in array
}

uint8_t DataContainerClass::AddGroup()
{
	DataGroupClass * tempDataGroup = new DataGroupClass[_groupsCount + 1];
	//copy all data to new array
	for (uint8_t i = 0; i < _groupsCount; i++)
		tempDataGroup[i] = _dataGroup[i];

	tempDataGroup[_groupsCount].SetGroupID(_groupsCount);
	//delete previous array
	if (_dataGroup != nullptr)
		delete[] _dataGroup;
	//point to new array
	_dataGroup = tempDataGroup;
	_groupsCount++;
	return _groupsCount - 1; //- 1 for index in array
}

DataGroupClass * DataContainerClass::GetDataGroup(uint8_t index)
{
	if (index < _groupsCount)
		return &_dataGroup[index];
	return nullptr;
}
/*
uint8_t DataContainerClass::GetGroupsCount()
{
return _groupsCount;
}

uint8_t * DataContainerClass::GetBytes(uint8_t groupIndex)
{
return _dataGroup[groupIndex].GetBytes();
}
*/