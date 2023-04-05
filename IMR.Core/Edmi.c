#include "Edmi.h"

EDMI_TYPE EdmiGetDataType(EDMI_TYPE type)
{
	switch (type)
	{
	case EDMI_TYPE_STRING:
		return DATA_TYPE_STRING;
	case EDMI_TYPE_BOOLEAN:
		return DATA_TYPE_BOOLEAN;
	case EDMI_TYPE_BYTE:
		return DATA_TYPE_BYTE;
	case EDMI_TYPE_DOUBLE:
		return DATA_TYPE_DOUBLE;
	case EDMI_TYPE_EFA_STRING:
		return DATA_TYPE_STRING;
	case EDMI_TYPE_FLOAT:
		return DATA_TYPE_FLOAT;
	case EDMI_TYPE_STRING_LONG:
		return DATA_TYPE_STRING;
	case EDMI_TYPE_HEX_SHORT:
		return DATA_TYPE_UINT16;
	case EDMI_TYPE_SHORT:
		return DATA_TYPE_INT16;
	case EDMI_TYPE_VARIABLE_SPACE:
		return DATA_TYPE_UNKNOWN; // DATA_TYPE_VARIABLE_SPACE;
	case EDMI_TYPE_LONG:
		return DATA_TYPE_INT32;
	case EDMI_TYPE_NONE:
		return DATA_TYPE_UNKNOWN; // DATA_TYPE_INVALID;
	case EDMI_TYPE_FLOAT_ENERGY:
		return DATA_TYPE_FLOAT;
	case EDMI_TYPE_POWER_FACTOR:
		return DATA_TYPE_FLOAT;
	case EDMI_TYPE_TIME:
		return DATA_TYPE_TIME;
	case EDMI_TYPE_DATE:
		return DATA_TYPE_DATE;
	case EDMI_TYPE_SPECIAL:
		return DATA_TYPE_UNKNOWN; // DATA_TYPE_SPECIAL;
	case EDMI_TYPE_TIME_DATE:
		return DATA_TYPE_DATE_TIME;
	case EDMI_TYPE_DOUBLE_ENERGY:
		return DATA_TYPE_DOUBLE;
	case EDMI_TYPE_LONG_LONG:
		return DATA_TYPE_INT64;
	case EDMI_TYPE_WAVEFORM:
		return DATA_TYPE_UNKNOWN; // DATA_TYPE_WAVEFORM;
	case EDMI_TYPE_HEX_LONG:
		return DATA_TYPE_UINT32;
	case EDMI_TYPE_REGISTER_NUMBER_HEX_LONG:
		return DATA_TYPE_UINT32;
	default:
		return DATA_TYPE_UNKNOWN;
	}
}

EDMI_ERROR_CODE EdmiReadValue(char** payloadPtr, int* length, EDMI_TYPE type, char* valuePtr)
{
	switch (EdmiGetDataType(type))
	{
	case DATA_TYPE_BYTE:
	case DATA_TYPE_BOOLEAN:
		return EdmiReadChar(payloadPtr, length, valuePtr);
	case DATA_TYPE_STRING:
		return EdmiReadString(payloadPtr, length, valuePtr, MAX_VALUE_LENGTH);
	case DATA_TYPE_INT16:
	case DATA_TYPE_UINT16:
		return EdmiReadInt16(payloadPtr, length, valuePtr);
	case DATA_TYPE_INT32:
	case DATA_TYPE_UINT32:
		return EdmiReadInt32(payloadPtr, length, valuePtr);
	case DATA_TYPE_INT64:
	case DATA_TYPE_UINT64:
		return EdmiReadInt64(payloadPtr, length, valuePtr);
	case DATA_TYPE_FLOAT:
		return EdmiReadFloat(payloadPtr, length, valuePtr);
	case DATA_TYPE_DOUBLE:
		return EdmiReadDouble(payloadPtr, length, valuePtr);
	case DATA_TYPE_DATE:
		return EdmiReadDate(payloadPtr, length, valuePtr);
	case DATA_TYPE_TIME:
		return EdmiReadTime(payloadPtr, length, valuePtr);
	case DATA_TYPE_DATE_TIME:
		return EdmiReadDateTime(payloadPtr, length, valuePtr);
	default:
		//printf("\nType:%02x\n", type);
		return EDMI_ERROR_CODE_UNIMPLEMENTED_DATA_TYPE;
	}
}

EDMI_ERROR_CODE EdmiReadRegisterValue(char** payloadPtr, int* length, EDMI_REGISTER* reg)
{
	return EdmiReadValue(payloadPtr, length, reg->Type, &reg->Value);
}

EDMI_REGISTER* EdmiGetFileInfoRegs(EDMI_SURVEY survey)
{
	EDMI_REGISTER* regs = malloc(EDMI_FILE_INFO_REGS_COUNT * sizeof(EDMI_REGISTER));
	EDMI_REGISTER* regsPtr = regs;
	EDMI_REGISTER* reg;

	int mask = survey << 16;

	// Interval Reg
	reg = regsPtr++;
	reg->Address = mask | 0xF014;
	reg->Type = EDMI_TYPE_LONG;
	reg->UnitCode = EDMI_UNIT_CODE_SECONDS;

	// Channels Count Reg
	reg = regsPtr;
	reg->Address = mask | 0xF012;
	reg->Type = EDMI_TYPE_BYTE;
	reg->UnitCode = EDMI_UNIT_CODE_NO_UNIT;

	return regs;
}

EDMI_ERROR_CODE EdmiSetProfileInfo(EDMI_FILE_INFO* info, EDMI_REGISTER* regs)
{
	EDMI_REGISTER* reg;

	// Interval Reg
	reg = regs++;
	if (reg->ErrorCode != EDMI_ERROR_CODE_NONE)
		return reg->ErrorCode;
	info->Interval = *(int*)& reg->Value;

	// Channels Count Reg
	reg = regs;
	if (reg->ErrorCode != EDMI_ERROR_CODE_NONE)
		return reg->ErrorCode;
	info->ChannelsCount = reg->Value[0] + 1; // Each load survey has an implied flags channel which is a word(2 bytes).This is channel 0.

	return EDMI_ERROR_CODE_NONE;
}

EDMI_REGISTER* EdmiGetFileChannelRegs(EDMI_SURVEY survey, unsigned char channel)
{
	EDMI_REGISTER* regs = malloc(EDMI_FILE_CHANNEL_INFO_REGS_COUNT * sizeof(EDMI_REGISTER));
	EDMI_REGISTER* regsPtr = regs;
	EDMI_REGISTER* reg;
	int mask = survey << 16;

	// Type Reg
	reg = regsPtr++;
	reg->Address = mask | ((0xE2 << 8) | channel);
	reg->Type = EDMI_TYPE_BYTE;
	reg->UnitCode = EDMI_UNIT_CODE_NO_UNIT;

	// UnitCode Reg
	reg = regsPtr++;
	reg->Address = mask | ((0xE3 << 8) | channel);
	reg->Type = EDMI_TYPE_BYTE;
	reg->UnitCode = EDMI_UNIT_CODE_NO_UNIT;

	// ScalingCode Reg
	reg = regsPtr++;
	reg->Address = mask | ((0xE6 << 8) | channel);
	reg->Type = EDMI_TYPE_BYTE;
	reg->UnitCode = EDMI_UNIT_CODE_NO_UNIT;

	// ScalingFactor Reg
	reg = regsPtr++;
	reg->Address = mask | ((0xE8 << 8) | channel);
	reg->Type = EDMI_TYPE_FLOAT;
	reg->UnitCode = EDMI_UNIT_CODE_NO_UNIT;

	// Name Reg
	reg = regsPtr;
	reg->Address = mask | ((0xE4 << 8) | channel);
	reg->Type = EDMI_TYPE_STRING;
	reg->UnitCode = EDMI_UNIT_CODE_NO_UNIT;

	return regs;
}

EDMI_ERROR_CODE EdmiSetFileChannelInfo(EDMI_FILE_CHANNEL_INFO* info, EDMI_REGISTER* regs)
{
	EDMI_REGISTER* reg;

	// Type Reg
	reg = regs++;
	if (reg->ErrorCode != EDMI_ERROR_CODE_NONE)
		return reg->ErrorCode;
	info->Type = reg->Value[0];

	// UnitCode Reg
	reg = regs++;
	if (reg->ErrorCode != EDMI_ERROR_CODE_NONE)
		return reg->ErrorCode;
	info->UnitCode = reg->Value[0];

	// ScalingCode Reg
	reg = regs++;
	if (reg->ErrorCode != EDMI_ERROR_CODE_NONE)
		return reg->ErrorCode;
	info->ScalingCode = reg->Value[0];

	// ScalingFactor Reg
	reg = regs++;
	if (reg->ErrorCode != EDMI_ERROR_CODE_NONE)
		return reg->ErrorCode;
	info->ScalingFactor = *(float*)& reg->Value;

	// Name Reg
	reg = regs;
	if (reg->ErrorCode != EDMI_ERROR_CODE_NONE)
		return reg->ErrorCode;
	memcpy(&info->Name, &reg->Value, MAX_VALUE_LENGTH * sizeof(char));

	return EDMI_ERROR_CODE_NONE;
}

EDMI_ERROR_CODE EdmiRead(char** buffer, int* remLen, char** value, int readLen)
{
	return Read(buffer, remLen, value, readLen) == true ? EDMI_ERROR_CODE_NONE : EDMI_ERROR_CODE_RESPONSE_WRONG_LENGTH;
}

EDMI_ERROR_CODE EdmiReadChar(char** buffer, int* remLen, char* value)
{
	return ReadChar(buffer, remLen, value) == true ? EDMI_ERROR_CODE_NONE : EDMI_ERROR_CODE_RESPONSE_WRONG_LENGTH;
}

EDMI_ERROR_CODE EdmiReadInt16(char** buffer, int* remLen, short* value)
{
	return ReadInt16(buffer, remLen, value) == true ? EDMI_ERROR_CODE_NONE : EDMI_ERROR_CODE_RESPONSE_WRONG_LENGTH;
}

EDMI_ERROR_CODE EdmiReadInt32(char** buffer, int* remLen, int* value)
{
	return ReadInt32(buffer, remLen, value) == true ? EDMI_ERROR_CODE_NONE : EDMI_ERROR_CODE_RESPONSE_WRONG_LENGTH;
}

EDMI_ERROR_CODE EdmiReadInt64(char** buffer, int* remLen, long* value)
{
	return ReadInt64(buffer, remLen, value) == true ? EDMI_ERROR_CODE_NONE : EDMI_ERROR_CODE_RESPONSE_WRONG_LENGTH;
}

EDMI_ERROR_CODE EdmiReadFloat(char** buffer, int* remLen, float* value)
{
	return ReadFloat(buffer, remLen, value) == true ? EDMI_ERROR_CODE_NONE : EDMI_ERROR_CODE_RESPONSE_WRONG_LENGTH;
}

EDMI_ERROR_CODE EdmiReadDouble(char** buffer, int* remLen, double* value)
{
	return ReadDouble(buffer, remLen, value) == true ? EDMI_ERROR_CODE_NONE : EDMI_ERROR_CODE_RESPONSE_WRONG_LENGTH;
}

EDMI_ERROR_CODE EdmiReadString(char** buffer, int* remLen, char* value, int maxValueLen)
{
	int readedLength = 0;
	return ReadString(buffer, remLen, value, maxValueLen, &readedLength) == true ? EDMI_ERROR_CODE_NONE : EDMI_ERROR_CODE_RESPONSE_WRONG_LENGTH;
}

EDMI_ERROR_CODE EdmiReadDate(char** buffer, int* remLen, EDMI_DATE_TIME* dateTime)
{
	EDMI_ERROR_CODE errorCode = EdmiReadChar(buffer, remLen, &dateTime->Day);
	if (errorCode != EDMI_ERROR_CODE_NONE)
		return errorCode;
	errorCode = EdmiReadChar(buffer, remLen, &dateTime->Month);
	if (errorCode != EDMI_ERROR_CODE_NONE)
		return errorCode;
	return EdmiReadChar(buffer, remLen, &dateTime->Year);
}

EDMI_ERROR_CODE EdmiReadTime(char** buffer, int* remLen, EDMI_DATE_TIME* dateTime)
{
	EDMI_ERROR_CODE errorCode = EdmiReadChar(buffer, remLen, &dateTime->Hour);
	if (errorCode != EDMI_ERROR_CODE_NONE)
		return errorCode;
	errorCode = EdmiReadChar(buffer, remLen, &dateTime->Minute);
	if (errorCode != EDMI_ERROR_CODE_NONE)
		return errorCode;
	return EdmiReadChar(buffer, remLen, &dateTime->Second);
}

EDMI_ERROR_CODE EdmiReadDateTime(char** buffer, int* remLen, EDMI_DATE_TIME* dateTime)
{
	EDMI_ERROR_CODE errorCode = EdmiReadDate(buffer, remLen, dateTime);
	if (errorCode != EDMI_ERROR_CODE_NONE)
		return errorCode;
	return EdmiReadTime(buffer, remLen, dateTime);
}

void EdmiWriteDate(char** buffer, int* length, EDMI_DATE_TIME dateTime)
{
	WriteChar(buffer, length, dateTime.Day);
	WriteChar(buffer, length, dateTime.Month);
	WriteChar(buffer, length, dateTime.Year);
}

void EdmiWriteTime(char** buffer, int* length, EDMI_DATE_TIME dateTime)
{
	WriteChar(buffer, length, dateTime.Hour);
	WriteChar(buffer, length, dateTime.Minute);
	WriteChar(buffer, length, dateTime.Second);
}

void EdmiWriteDateTime(char** buffer, int* length, EDMI_DATE_TIME dateTime)
{
	EdmiWriteDate(buffer, length, dateTime);
	EdmiWriteTime(buffer, length, dateTime);
}

short EdmiGenerateCRC16(short i)
{
	short j;
	short k;
	short crc;
	k = (short)(i << 8);
	crc = 0;
	for (j = 0; j < 8; j++)
	{
		if (((crc ^ k) & 0x8000) != 0)
			crc = (short)((crc << 1) ^ 0x1021);
		else
			crc <<= 1;
		k <<= 1;
	}
	return crc;
}

short EdmiCalculateCharacterCRC16(short crc, char c)
{
	return (short)((crc << 8) ^ EdmiGenerateCRC16((short)((crc >> 8) ^ c)));
}

short EdmiCalculateCRC(char* packet, int length)
{
	short crc = 0;
	char* packetPtr = packet;
	for (int i = 0; i < length; i++)
		crc = EdmiCalculateCharacterCRC16(crc, *packetPtr++);
	return crc;
}

EDMI_ERROR_CODE EdmiValidateCRC(char* packet, int length)
{
	int len = length - 4;
	if (len <= 0)
		return EDMI_ERROR_CODE_RESPONSE_WRONG_LENGTH;

	short* mid = packet + len + 1;
	short midValue = L16toB16(*mid);
	short crc = EdmiCalculateCRC(packet + 1, len);
	if (crc == midValue)
		return EDMI_ERROR_CODE_NONE;

	return EDMI_ERROR_CODE_RESPONSE_CRC_ERROR;
}

void EdmiPreProccess(char** packet, int* length)
{
	int outputLength = 0;
	char* output = malloc(outputLength);

	char* packetPtr = *packet;
	for (int i = 0; i < *length; i++)
	{
		if (*packetPtr == EDMI_DLE_IDEN)
		{
			packetPtr++;
			i++;
			WriteChar(&output, &outputLength, *packetPtr - EDMI_IDEN_CORRECTOR);
		}
		else
			WriteChar(&output, &outputLength, *packetPtr);
		packetPtr++;
	}

	// Cleanup
	free(*packet);

	*length = outputLength;
	*packet = output;
}

void EdmiPostProccess(char** packet, int* length)
{
	int outputLength = 1;
	char* output = malloc(outputLength * sizeof(char));

	char* packetPtr = *packet;
	*output = *packetPtr++; // Append and Skip STX
	for (int i = 1; i < *length; i++) // Append frame
	{
		if (*packetPtr == EDMI_STX_IDEN || *packetPtr == EDMI_ETX_IDEN || *packetPtr == EDMI_XON_IDEN || *packetPtr == EDMI_XOFF_IDEN || *packetPtr == EDMI_DLE_IDEN)
		{
			WriteChar(&output, &outputLength, EDMI_DLE_IDEN);
			WriteChar(&output, &outputLength, *packetPtr + EDMI_IDEN_CORRECTOR);
		}
		else
			WriteChar(&output, &outputLength, *packetPtr);
		packetPtr++;
	}

	// Cleanup
	free(*packet);

	*length = outputLength;
	*packet = output;

}

void EdmiBeginInitPacket(char** packet, int* length, unsigned int serial, EDMI_COMMAND_TYPE commandType, EDMI_COMMAND_EXTENSION commandExtention)
{
	*packet = malloc(0);
	*length = 0;
	WriteChar(packet, length, EDMI_STX_IDEN);
	WriteChar(packet, length, EDMI_E_FRAME_IDEN);
	WriteInt32(packet, length, serial);
	Write(packet, length, EDMI_CLIENT_SERIAL, EDMI_CLIENT_SERIAL_LENGTH);
	WriteChar(packet, length, commandType);
	if (commandExtention != EDMI_COMMAND_EXTENSION_NO_EXTENSION)
		WriteChar(packet, length, commandExtention);
}

void EdmiEndInitPacket(char** packet, int* length)
{
	// Append CRC
	short crc = EdmiCalculateCRC(*packet, *length);
	WriteInt16(packet, length, crc);

	// Post Proccess
	EdmiPostProccess(packet, length);

	// Append ETX
	WriteChar(packet, length, EDMI_ETX_IDEN);
}

MEDIA_ERROR EdmiReceivePacket(READ_FROM_MEDIA_DELEGATE readCallback, char** packet, int* length, EDMI_ERROR_CODE* errorCode)
{
	// Init
	MEDIA_ERROR ret = MEDIA_ERROR_NONE;
	*errorCode = EDMI_ERROR_CODE_NONE;
	*packet = malloc(0);
	*length = 0;

	// Read Raw Packet
	bool hitStartingflag = false;
	for (; ; )
	{
		char byte;
		ret = ReadFromMedia(readCallback, &byte);
		if (ret != MEDIA_ERROR_NONE)
			break;
		if (byte == EDMI_STX_IDEN)
		{
			// Hit starting flag
			free(*packet);
			*packet = malloc(0);
			*length = 0;
			WriteChar(packet, length, 0xFF);
			WriteChar(packet, length, EDMI_STX_IDEN);
			hitStartingflag = true;
		}
		else if (hitStartingflag && byte == EDMI_ETX_IDEN)
		{
			// Hit ending falg
			WriteChar(packet, length, EDMI_ETX_IDEN);
			break;
		}
		else if (hitStartingflag)
			// Append packet data
			WriteChar(packet, length, byte);
	}

	// PreProccess And Unpack
	if (ret == MEDIA_ERROR_NONE)
	{
		EdmiPreProccess(packet, length);

		*errorCode = EdmiValidateCRC(*packet, *length);
		if (*errorCode == EDMI_ERROR_CODE_NONE)
		{
			int headerLen = 13;
			int headerLenPlusTrailerLen = 16;
			int payloadLen = *length - headerLenPlusTrailerLen;
			if (payloadLen >= 0)
			{
				char* payload = malloc(payloadLen);
				char* payloadPtr = *packet + headerLen;
				memcpy(payload, payloadPtr, payloadLen * sizeof(char));
				free(*packet);
				*packet = payload;
				*length = payloadLen;
				*errorCode = EDMI_ERROR_CODE_NONE;
			}
			else
				errorCode = EDMI_ERROR_CODE_RESPONSE_WRONG_LENGTH;
		}
	}

	return ret;
}

MEDIA_ERROR EdmiExecuteCommand(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, char** packet, int* length, EDMI_ERROR_CODE* errorCode)
{
	// Init
	*errorCode = EDMI_ERROR_CODE_NONE;
	MEDIA_ERROR ret = MEDIA_ERROR_NONE;

	// Send Packet
	ret = WriteAllToMedia(writeCallback, *packet, *length);
	if (ret == MEDIA_ERROR_NONE)
	{
		// Receive Packet
		ret = EdmiReceivePacket(readCallback, packet, length, errorCode);
	}
	else
	{
		*packet = malloc(0);
		*length = 0;
	}

	return ret;
}

void EdmiCreateLoginPacket(char** packet, int* length, unsigned int serial, char* username, int usernameLength, char* password, int passwordLength)
{
	EdmiBeginInitPacket(packet, length, serial, EDMI_COMMAND_TYPE_LOGIN, EDMI_COMMAND_EXTENSION_NO_EXTENSION);
	Write(packet, length, username, usernameLength);
	WriteChar(packet, length, ',');
	Write(packet, length, password, passwordLength);
	WriteChar(packet, length, 0x00);
	EdmiEndInitPacket(packet, length);
}

void EdmiCreateLogoutPacket(char** packet, int* length, unsigned int serial)
{
	EdmiBeginInitPacket(packet, length, serial, EDMI_COMMAND_TYPE_LOGOUT, EDMI_COMMAND_EXTENSION_NO_EXTENSION);
	EdmiEndInitPacket(packet, length);
}

void EdmiCreateReadRegisterPacket(char** packet, int* length, unsigned int serial, EDMI_REGISTER* reg)
{
	EdmiBeginInitPacket(packet, length, serial, EDMI_COMMAND_TYPE_READ_REGISTER_EXTENDED, EDMI_COMMAND_EXTENSION_NO_EXTENSION);
	WriteInt32(packet, length, reg->Address);
	EdmiEndInitPacket(packet, length);
}

void EdmiCreateReadRegistersPacket(char** packet, int* length, unsigned int serial, EDMI_REGISTER* regs, int regsCount)
{
	EdmiBeginInitPacket(packet, length, serial, EDMI_COMMAND_TYPE_READ_REGISTER_EXTENDED, EDMI_COMMAND_EXTENSION_NO_EXTENSION);

	// Use Multiple ErrorCodes
	WriteInt32(packet, length, EDMI_MULTI_ERR_IDEN);

	// Append Registers
	for (int i = 0; i < regsCount; i++)
		WriteInt32(packet, length, regs[i].Address);

	EdmiEndInitPacket(packet, length);
}

void EdmiCreateReadProfileInfoAccessPacket(char** packet, int* length, unsigned int serial, EDMI_SURVEY survey)
{
	EdmiBeginInitPacket(packet, length, serial, EDMI_COMMAND_TYPE_FILE_ACCESS, EDMI_COMMAND_EXTENSION_FILE_INFO);

	// Write Register Address
	WriteInt32(packet, length, (survey << 16) | 0xF008);

	EdmiEndInitPacket(packet, length);
}

void EdmiCreateSearchProfilePacket(char** packet, int* length, unsigned int serial, EDMI_SURVEY survey, EDMI_SEARCH_FILE* search)
{
	EdmiBeginInitPacket(packet, length, serial, EDMI_COMMAND_TYPE_FILE_ACCESS, EDMI_COMMAND_EXTENSION_FILE_SEARCH);

	// Write Register Address
	WriteInt32(packet, length, (survey << 16) | 0xF008);

	// Write search
	WriteInt32(packet, length, search->StartRecord);
	EdmiWriteDateTime(packet, length, search->DateTime);
	WriteChar(packet, length, search->DirOrResult);

	EdmiEndInitPacket(packet, length);
}

void EdmiCreateReadProfilePacket(char** packet, int* length, unsigned int serial, EDMI_SURVEY survey, EDMI_READ_FILE* read)
{
	EdmiBeginInitPacket(packet, length, serial, EDMI_COMMAND_TYPE_FILE_ACCESS, EDMI_COMMAND_EXTENSION_FILE_READ);

	// Write Register Address
	WriteInt32(packet, length, (survey << 16) | 0xF008);

	// Write read
	WriteInt32(packet, length, read->StartRecord);
	WriteInt16(packet, length, read->RecordsCount);
	WriteInt16(packet, length, read->RecordOffset);
	WriteInt16(packet, length, read->RecordSize);

	EdmiEndInitPacket(packet, length);
}

EDMI_ERROR_CODE EdmiParseLoginPayload(char* payload, int length)
{
	// Read Result Code
	char resultCode;
	EDMI_ERROR_CODE errorCode = EdmiReadChar(&payload, &length, &resultCode);
	if (errorCode == EDMI_ERROR_CODE_NONE)
		errorCode = (resultCode == EDMI_RESPONSE_CODE_ACK) ? EDMI_ERROR_CODE_NONE : EDMI_ERROR_CODE_LOGIN_FAILED;
	return errorCode;
}

EDMI_ERROR_CODE EdmiParseLogoutPayload(char* payload, int length)
{
	// Read Result Code
	char resultCode;
	EDMI_ERROR_CODE errorCode = EdmiReadChar(&payload, &length, &resultCode);
	if (errorCode == EDMI_ERROR_CODE_NONE)
		errorCode = (resultCode == EDMI_RESPONSE_CODE_ACK) ? EDMI_ERROR_CODE_NONE : EDMI_ERROR_CODE_LOGIN_FAILED;
	return errorCode;
}

EDMI_ERROR_CODE EdmiParseReadRegisterPayload(char* payload, int length, EDMI_REGISTER* reg)
{
	// Read Result Code
	char resultCode;
	EDMI_ERROR_CODE errorCode = EdmiReadChar(&payload, &length, &resultCode);
	if (errorCode == EDMI_ERROR_CODE_NONE)
	{
		if (resultCode == EDMI_RESPONSE_CODE_CAN)
		{
			// Read Error Code
			EDMI_ERROR_CODE errCode;
			errorCode = EdmiReadChar(&payload, &length, &errCode);
			errorCode = (errorCode == EDMI_ERROR_CODE_NONE) ? errCode : errorCode;
		}
		else
		{
			if (resultCode == EDMI_COMMAND_TYPE_READ_REGISTER_EXTENDED)
			{
				// Read Register Address
				int addr;
				errorCode = EdmiReadInt32(&payload, &length, &addr);

				// Read Register Value
				if (errorCode == EDMI_ERROR_CODE_NONE)
					errorCode = EdmiReadRegisterValue(&payload, &length, reg);
			}
			else
				errorCode = EDMI_ERROR_CODE_REQUEST_RESPONSE_COMMAND_MISMATCH;
		}
	}

	reg->ErrorCode = errorCode;
	return errorCode;
}

EDMI_ERROR_CODE EdmiParseReadRegistersPayload(char* payload, int length, EDMI_REGISTER* regs, int regsCount)
{
	// Read Result Code
	char resultCode;
	EDMI_ERROR_CODE errorCode = EdmiReadChar(&payload, &length, &resultCode);
	if (errorCode == EDMI_ERROR_CODE_NONE)
	{
		if (resultCode == EDMI_COMMAND_TYPE_READ_REGISTER_EXTENDED)
		{
			// Read Multiple Error Codes IDEN
			int mIden;
			errorCode = EdmiReadInt32(&payload, &length, &mIden);
			if (errorCode == EDMI_ERROR_CODE_NONE)
			{
				if (mIden == EDMI_MULTI_ERR_IDEN)
				{
					EDMI_REGISTER* ptr;
					for (int i = 0; i < regsCount; i++)
					{
						ptr = regs++;

						// Read Register Error Code
						errorCode = EdmiReadChar(&payload, &length, &ptr->ErrorCode);
						if (errorCode != EDMI_ERROR_CODE_NONE)
							break;

						// Read Register Value
						errorCode = EdmiReadRegisterValue(&payload, &length, ptr);
						if (errorCode != EDMI_ERROR_CODE_NONE)
							break;
					}
				}
				else
					errorCode = EDMI_ERROR_CODE_REQUEST_RESPONSE_COMMAND_MISMATCH;
			}
		}
		else
			errorCode = EDMI_ERROR_CODE_REQUEST_RESPONSE_COMMAND_MISMATCH;
	}

	return errorCode;
}

EDMI_ERROR_CODE EdmiParseReadProfileInfoAccessPayload(char* payload, int length, EDMI_FILE_INFO* info)
{
	// Read Result Code
	unsigned char resultCode;
	EDMI_ERROR_CODE errorCode = EdmiReadChar(&payload, &length, &resultCode);
	if (errorCode != EDMI_ERROR_CODE_NONE)
		return errorCode;

	if (resultCode == EDMI_RESPONSE_CODE_CAN)
	{
		// Read Error Code
		EDMI_ERROR_CODE errCode;
		errorCode = EdmiReadChar(&payload, &length, &errCode);
		return (errorCode == EDMI_ERROR_CODE_NONE) ? errCode : errorCode;
	}

	if (resultCode != EDMI_COMMAND_TYPE_FILE_ACCESS)
		return EDMI_ERROR_CODE_REQUEST_RESPONSE_COMMAND_MISMATCH;

	// Read Extention
	errorCode = EdmiReadChar(&payload, &length, &resultCode);
	if (errorCode != EDMI_ERROR_CODE_NONE)
		return errorCode;

	if (resultCode != EDMI_COMMAND_EXTENSION_FILE_INFO)
		return EDMI_ERROR_CODE_REQUEST_RESPONSE_COMMAND_MISMATCH;

	// Read Register Address
	int regAddr;
	errorCode = EdmiReadInt32(&payload, &length, &regAddr);
	if (errorCode != EDMI_ERROR_CODE_NONE)
		return errorCode;

	// Read Start Record
	errorCode = EdmiReadInt32(&payload, &length, &info->StartRecord);
	if (errorCode != EDMI_ERROR_CODE_NONE)
		return errorCode;

	// Read Records Count
	errorCode = EdmiReadInt32(&payload, &length, &info->RecordsCount);
	if (errorCode != EDMI_ERROR_CODE_NONE)
		return errorCode;

	// Read Record Size
	errorCode = EdmiReadInt16(&payload, &length, &info->RecordSize);
	if (errorCode != EDMI_ERROR_CODE_NONE)
		return errorCode;

	// Read Type
	errorCode = EdmiReadChar(&payload, &length, &info->Type);
	if (errorCode != EDMI_ERROR_CODE_NONE)
		return errorCode;

	// Read Name
	return EdmiReadString(&payload, &length, &info->Name, MAX_VALUE_LENGTH);
}

EDMI_ERROR_CODE EdmiParseSearchProfilePayload(char* payload, int length, EDMI_SEARCH_FILE* search)
{
	// Read Result Code
	char resultCode;
	EDMI_ERROR_CODE errorCode = EdmiReadChar(&payload, &length, &resultCode);
	if (errorCode == EDMI_ERROR_CODE_NONE)
	{
		if (resultCode == EDMI_RESPONSE_CODE_CAN)
		{
			// Read Error Code
			EDMI_ERROR_CODE errCode;
			errorCode = EdmiReadChar(&payload, &length, &errCode);
			errorCode = (errorCode == EDMI_ERROR_CODE_NONE) ? errCode : errorCode;
		}
		else
		{
			if (resultCode == EDMI_COMMAND_TYPE_FILE_ACCESS)
			{
				// Read Extention
				errorCode = EdmiReadChar(&payload, &length, &resultCode);
				if (errorCode == EDMI_ERROR_CODE_NONE)
				{
					if (resultCode == EDMI_COMMAND_EXTENSION_FILE_SEARCH)
					{
						// Read Register Address
						int regAddr;
						errorCode = EdmiReadInt32(&payload, &length, &regAddr);
						if (errorCode == EDMI_ERROR_CODE_NONE)
						{
							// Read Start Record
							errorCode = EdmiReadInt32(&payload, &length, &search->StartRecord);
							if (errorCode == EDMI_ERROR_CODE_NONE)
							{
								// Read DateTime
								errorCode = EdmiReadDateTime(&payload, &length, &search->DateTime);

								// Read Result
								if (errorCode == EDMI_ERROR_CODE_NONE)
									errorCode = EdmiReadChar(&payload, &length, &search->DirOrResult);
							}
						}
					}
					else
						errorCode = EDMI_ERROR_CODE_REQUEST_RESPONSE_COMMAND_MISMATCH;
				}
			}
			else
				errorCode = EDMI_ERROR_CODE_REQUEST_RESPONSE_COMMAND_MISMATCH;
		}
	}

	return errorCode;
}

EDMI_ERROR_CODE EdmiParseReadProfilePayload(char* payload, int length, EDMI_READ_FILE* read, EDMI_PROFILE_SPEC* profileSpec, EDMI_FILE_FIELD** fields)
{
	// Init
	*fields = malloc(0);

	// Read Result Code
	unsigned char resultCode;
	EDMI_ERROR_CODE errorCode = EdmiReadChar(&payload, &length, &resultCode);
	if (errorCode != EDMI_ERROR_CODE_NONE)
		return errorCode;

	if (resultCode == EDMI_RESPONSE_CODE_CAN)
	{
		// Read Error Code
		EDMI_ERROR_CODE errCode;
		errorCode = EdmiReadChar(&payload, &length, &errCode);
		return (errorCode == EDMI_ERROR_CODE_NONE) ? errCode : errorCode;
	}

	if (resultCode != EDMI_COMMAND_TYPE_FILE_ACCESS)
		return EDMI_ERROR_CODE_REQUEST_RESPONSE_COMMAND_MISMATCH;

	// Read Extention
	errorCode = EdmiReadChar(&payload, &length, &resultCode);
	if (errorCode != EDMI_ERROR_CODE_NONE)
		return errorCode;

	if (resultCode != EDMI_COMMAND_EXTENSION_FILE_READ)
		return EDMI_ERROR_CODE_REQUEST_RESPONSE_COMMAND_MISMATCH;

	// Read Register Address
	int regAddr;
	errorCode = EdmiReadInt32(&payload, &length, &regAddr);
	if (errorCode != EDMI_ERROR_CODE_NONE)
		return errorCode;

	// Read Start Record
	errorCode = EdmiReadInt32(&payload, &length, &read->StartRecord);
	if (errorCode != EDMI_ERROR_CODE_NONE)
		return errorCode;

	// Read Records Count
	errorCode = EdmiReadInt16(&payload, &length, &read->RecordsCount);
	if (errorCode != EDMI_ERROR_CODE_NONE)
		return errorCode;

	// Read Record Offset
	errorCode = EdmiReadInt16(&payload, &length, &read->RecordOffset);
	if (errorCode != EDMI_ERROR_CODE_NONE)
		return errorCode;

	// Read Record Size
	errorCode = EdmiReadInt16(&payload, &length, &read->RecordSize);
	if (errorCode != EDMI_ERROR_CODE_NONE)
		return errorCode;
						
	// Set Records Count
	profileSpec->RecordsCount = read->RecordsCount;

	// Cleanup And Allocation
	free(*fields);
	*fields = malloc(read->RecordsCount * profileSpec->ChannelsCount * sizeof(EDMI_FILE_FIELD));
	EDMI_FILE_FIELD* fieldPtr = *fields;

	// Read Record
	for (short record = 0; record < read->RecordsCount; record++)
	{
		// Read Channel
		for (unsigned char ch = 0; ch < profileSpec->ChannelsCount; ch++)
		{
			// Read Field
			errorCode = EdmiReadValue(&payload, &length, profileSpec->ChannelsInfo[ch].Type, &fieldPtr->Value);
			if (errorCode != EDMI_ERROR_CODE_NONE)
				return errorCode;
			fieldPtr++;
		}
	}

	return errorCode;
}

MEDIA_ERROR EdmiReadProfileInfoAccess(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, unsigned int serial, EDMI_SURVEY survey, EDMI_FILE_INFO* info, EDMI_ERROR_CODE* errorCode)
{
	// Init
	MEDIA_ERROR ret = MEDIA_ERROR_NONE;
	*errorCode = EDMI_ERROR_CODE_NONE;

	// Create Read Profile Info Access Packet
	char* packet = NULL;
	int length;
	EdmiCreateReadProfileInfoAccessPacket(&packet, &length, serial, survey);

	// Execute Command
	ret = EdmiExecuteCommand(readCallback, writeCallback, &packet, &length, errorCode);

	// Parse Read File Info Access Payload
	if (ret == MEDIA_ERROR_NONE && *errorCode == EDMI_ERROR_CODE_NONE)
		*errorCode = EdmiParseReadProfileInfoAccessPayload(packet, length, info);

	// Cleanup
	free(packet);

	return ret;
}

MEDIA_ERROR EdmiReadProfileInfo(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, unsigned int serial, EDMI_SURVEY survey, EDMI_FILE_INFO* info, EDMI_ERROR_CODE* errorCode)
{
	// Read Profile Info Regs
	EDMI_REGISTER* regs = EdmiGetFileInfoRegs(survey);
	unsigned char errCode;
	MEDIA_ERROR ret = EdmiReadRegisters(readCallback, writeCallback, serial, regs, EDMI_FILE_INFO_REGS_COUNT, &errCode);
	*errorCode = errCode;
	if (ret == MEDIA_ERROR_NONE && *errorCode == EDMI_ERROR_CODE_NONE)
	{
		*errorCode = EdmiSetProfileInfo(info, regs);

		// Read File Info Access
		if (*errorCode == EDMI_ERROR_CODE_NONE)
			ret = EdmiReadProfileInfoAccess(readCallback, writeCallback, serial, survey, info, errorCode);
	}

	// Cleanup
	free(regs);

	return ret;
}

MEDIA_ERROR EdmiReadProfileChannelInfo(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, unsigned int serial, EDMI_SURVEY survey, EDMI_REGISTER* regs, int regsCount, EDMI_FILE_CHANNEL_INFO* info, EDMI_ERROR_CODE* errorCode)
{
	// Init
	MEDIA_ERROR ret = MEDIA_ERROR_NONE;
	*errorCode = EDMI_ERROR_CODE_NONE;

	// Read Profile Channel Regs
	unsigned char errCode;
	ret = EdmiReadRegisters(readCallback, writeCallback, serial, regs, regsCount, &errCode);
	*errorCode = errCode;
	if (ret == MEDIA_ERROR_NONE && *errorCode == EDMI_ERROR_CODE_NONE)
		* errorCode = EdmiSetFileChannelInfo(info, regs);
	return ret;
}

MEDIA_ERROR EdmiReadProfileChannelsInfo(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, unsigned int serial, EDMI_SURVEY survey, EDMI_FILE_CHANNEL_INFO* infoes, unsigned char channelsCount, EDMI_ERROR_CODE* errorCode)
{
	// Init
	MEDIA_ERROR ret = MEDIA_ERROR_NONE;
	*errorCode = EDMI_ERROR_CODE_NONE;

	EDMI_FILE_CHANNEL_INFO* infoPtr = infoes;
	for (unsigned char channel = 0; channel < channelsCount; channel++)
	{
		// Read Profile Channel Info
		EDMI_REGISTER* regs = EdmiGetFileChannelRegs(survey, channel);

		ret = EdmiReadProfileChannelInfo(readCallback, writeCallback, serial, survey, regs, EDMI_FILE_CHANNEL_INFO_REGS_COUNT, infoPtr, errorCode);

		// Cleanup
		free(regs);

		if (ret != MEDIA_ERROR_NONE || *errorCode != EDMI_ERROR_CODE_NONE)
			break;

		infoPtr++;
	}
	return ret;
}

MEDIA_ERROR EdmiSearchProfile(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, unsigned int serial, EDMI_SURVEY survey, EDMI_SEARCH_FILE* search, EDMI_ERROR_CODE* errorCode)
{
	// Init
	MEDIA_ERROR ret = MEDIA_ERROR_NONE;
	*errorCode = EDMI_ERROR_CODE_NONE;

	// Create Search Profile Packet
	char* packet = NULL;
	int length;
	EdmiCreateSearchProfilePacket(&packet, &length, serial, survey, search);

	// Execute Command
	ret = EdmiExecuteCommand(readCallback, writeCallback, &packet, &length, errorCode);

	// Parse Search File Payload
	if (ret == MEDIA_ERROR_NONE && *errorCode == EDMI_ERROR_CODE_NONE)
		* errorCode = EdmiParseSearchProfilePayload(packet, length, search);

	// Cleanup
	free(packet);

	return ret;

}

EXPORT MEDIA_ERROR EdmiLogin(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, unsigned int serial, char* username, int usernameLength, char* password, int passwordLength, unsigned char* errCode)
{
	// Init
	EDMI_ERROR_CODE errorCode = EDMI_ERROR_CODE_NONE;
	MEDIA_ERROR ret = MEDIA_ERROR_NONE;

	// Create Login Packet
	char* packet = NULL;
	int length;
	EdmiCreateLoginPacket(&packet, &length, serial, username, usernameLength, password, passwordLength);

	// Execute Command
	ret = EdmiExecuteCommand(readCallback, writeCallback, &packet, &length, &errorCode);

	// Parse Login Payload
	if (ret == MEDIA_ERROR_NONE && errorCode == EDMI_ERROR_CODE_NONE)
		errorCode = EdmiParseLoginPayload(packet, length);

	// Cleanup
	free(packet);

	*errCode = errorCode;
	return ret;
}

EXPORT MEDIA_ERROR EdmiLogout(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, unsigned int serial, unsigned char* errCode)
{
	// Init
	EDMI_ERROR_CODE errorCode = EDMI_ERROR_CODE_NONE;
	MEDIA_ERROR ret = MEDIA_ERROR_NONE;

	// Create Logout Packet
	char* packet = NULL;
	int length;
	EdmiCreateLogoutPacket(&packet, &length, serial);

	// Execute Command
	ret = EdmiExecuteCommand(readCallback, writeCallback, &packet, &length, &errorCode);

	// Parse Logout Payload
	if (ret == MEDIA_ERROR_NONE && errorCode == EDMI_ERROR_CODE_NONE)
		errorCode = EdmiParseLogoutPayload(packet, length);

	// Cleanup
	free(packet);

	*errCode = errorCode;
	return ret;
}

EXPORT MEDIA_ERROR EdmiReadRegister(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, unsigned int serial, EDMI_REGISTER* reg, unsigned char* errCode)
{
	// Init
	EDMI_ERROR_CODE errorCode = EDMI_ERROR_CODE_NONE;
	MEDIA_ERROR ret = MEDIA_ERROR_NONE;

	// Create Read Register Packet
	char* packet = NULL;
	int length;
	EdmiCreateReadRegisterPacket(&packet, &length, serial, reg);

	// Execute Command
	ret = EdmiExecuteCommand(readCallback, writeCallback, &packet, &length, &errorCode);

	// Parse Read Register Payload
	if (ret == MEDIA_ERROR_NONE && errorCode == EDMI_ERROR_CODE_NONE)
		errorCode = EdmiParseReadRegisterPayload(packet, length, reg);

	// Cleanup
	free(packet);

	*errCode = errorCode;
	return ret;
}

EXPORT MEDIA_ERROR EdmiReadRegisters(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, unsigned int serial, EDMI_REGISTER* regs, int regsCount, unsigned char* errCode)
{
	// Init
	EDMI_ERROR_CODE errorCode = EDMI_ERROR_CODE_NONE;
	MEDIA_ERROR ret = MEDIA_ERROR_NONE;

	// Create Read Registers Packet
	char* packet = NULL;
	int length;
	EdmiCreateReadRegistersPacket(&packet, &length, serial, regs, regsCount);

	// Execute Command
	ret = EdmiExecuteCommand(readCallback, writeCallback, &packet, &length, &errorCode);

	// Parse Read Registers Payload
	if (ret == MEDIA_ERROR_NONE && errorCode == EDMI_ERROR_CODE_NONE)
		errorCode = EdmiParseReadRegistersPayload(packet, length, regs, regsCount);

	// Cleanup
	free(packet);

	*errCode = errorCode;
	return ret;
}

EXPORT MEDIA_ERROR EdmiReadProfile(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, unsigned int serial, short survey, EDMI_DATE_TIME fromDateTime, EDMI_DATE_TIME toDateTime, EDMI_PROFILE_SPEC* profileSpec, EDMI_FILE_FIELD** fields, unsigned char* errCode)
{
	// Init
	EDMI_ERROR_CODE errorCode;
	MEDIA_ERROR ret;

	// Read Profile Info
	EDMI_FILE_INFO fileInfo;
	ret = EdmiReadProfileInfo(readCallback, writeCallback, serial, survey, &fileInfo, &errorCode);
	if (ret != MEDIA_ERROR_NONE && errorCode != EDMI_ERROR_CODE_NONE)
	{
		*errCode = errorCode;
		return ret;
	}

	// Read Profile Channels Info
	EDMI_FILE_CHANNEL_INFO* channelsInfo = malloc(fileInfo.ChannelsCount * sizeof(EDMI_FILE_INFO));
	ret = EdmiReadProfileChannelsInfo(readCallback, writeCallback, serial, survey, channelsInfo, fileInfo.ChannelsCount, &errorCode);
	if (ret != MEDIA_ERROR_NONE || errorCode != EDMI_ERROR_CODE_NONE)
	{
		// Cleanup
		free(channelsInfo);

		*errCode = errorCode;
		return ret;
	}

	// Search From Date
	EDMI_SEARCH_FILE fromSearch;
	fromSearch.StartRecord = fileInfo.StartRecord;
	fromSearch.DateTime = fromDateTime;
	fromSearch.DirOrResult = EDMI_SEARCH_FILE_DIR_FORM_START_RECORD_BACKWARD;
	ret = EdmiSearchProfile(readCallback, writeCallback, serial, survey, &fromSearch, &errorCode);
	if (ret != MEDIA_ERROR_NONE || errorCode != EDMI_ERROR_CODE_NONE)
	{
		// Cleanup
		free(channelsInfo);

		*errCode = errorCode;
		return ret;
	}

	// Search To Date
	EDMI_SEARCH_FILE toSearch;
	toSearch.StartRecord = MAXINT16;
	if (toDateTime.IsNull == false)
	{
		toSearch.StartRecord = fileInfo.StartRecord;
		toSearch.DateTime = toDateTime;
		toSearch.DirOrResult = EDMI_SEARCH_FILE_DIR_FORM_START_RECORD_BACKWARD;
		ret = EdmiSearchProfile(readCallback, writeCallback, serial, survey, &toSearch, &errorCode);
		if (ret != MEDIA_ERROR_NONE || errorCode != EDMI_ERROR_CODE_NONE)
		{
			// Cleanup
			free(channelsInfo);

			*errCode = errorCode;
			return ret;
		}
	}

	// Read Profile
	EDMI_READ_FILE read;
	read.StartRecord = fromSearch.StartRecord;
	read.RecordsCount = (toDateTime.IsNull == true) ? MAXINT16 : (toSearch.StartRecord - fromSearch.StartRecord + 1);
	read.RecordSize = fileInfo.RecordSize;
	read.RecordOffset = 0;

	// Set Profile Spec
	profileSpec->Survey = survey;
	profileSpec->Interval = fileInfo.Interval;
	profileSpec->FromDateTime = fromSearch.DateTime;
	profileSpec->ToDateTime = (toDateTime.IsNull == true) ? toDateTime : toSearch.DateTime;
	profileSpec->ChannelsCount = fileInfo.ChannelsCount;
	EDMI_FILE_CHANNEL_INFO* chInfoPtr = channelsInfo;
	EDMI_FILE_CHANNEL_INFO* specChInfoPtr = &profileSpec->ChannelsInfo;
	for (unsigned char ch = 0; ch < fileInfo.ChannelsCount; ch++)
		* specChInfoPtr++ = *chInfoPtr++;
	memcpy(&profileSpec->Name, &fileInfo.Name, MAX_VALUE_LENGTH * sizeof(char));
	
	// Create Read Profile Packet
	char* packet = NULL;
	int length;
	EdmiCreateReadProfilePacket(&packet, &length, serial, survey, &read);

	// Execute Command
	EdmiExecuteCommand(readCallback, writeCallback, &packet, &length, &errorCode);
	if (ret == MEDIA_ERROR_NONE && errorCode == EDMI_ERROR_CODE_NONE)
		errorCode = EdmiParseReadProfilePayload(packet, length, &read, profileSpec, fields);

	// Cleanup
	free(channelsInfo);
	free(packet);

	*errCode = errorCode;
	return ret;
}