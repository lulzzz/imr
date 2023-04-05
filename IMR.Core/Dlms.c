#include "Dlms.h"

DLMS_ERROR_CODE DlmsRead(char** buffer, int* remLen, char** value, int readLen)
{
	return Read(buffer, remLen, value, readLen) == true ? DLMS_ERROR_CODE_NONE : DLMS_ERROR_CODE_PARSE_ERROR;
}

DLMS_ERROR_CODE DlmsReadChar(char** buffer, int* remLen, char* value)
{
	return ReadChar(buffer, remLen, value) == true ? DLMS_ERROR_CODE_NONE : DLMS_ERROR_CODE_PARSE_ERROR;
}

DLMS_ERROR_CODE DlmsReadInt16(char** buffer, int* remLen, short* value)
{
	return ReadInt16(buffer, remLen, value) == true ? DLMS_ERROR_CODE_NONE : DLMS_ERROR_CODE_PARSE_ERROR;
}

DLMS_ERROR_CODE DlmsReadInt32(char** buffer, int* remLen, int* value)
{
	return ReadInt32(buffer, remLen, value) == true ? DLMS_ERROR_CODE_NONE : DLMS_ERROR_CODE_PARSE_ERROR;
}

DLMS_ERROR_CODE DlmsReadInt64(char** buffer, int* remLen, long* value)
{
	return ReadInt64(buffer, remLen, value) == true ? DLMS_ERROR_CODE_NONE : DLMS_ERROR_CODE_PARSE_ERROR;
}

DLMS_ERROR_CODE DlmsReadFloat(char** buffer, int* remLen, float* value)
{
	return ReadFloat(buffer, remLen, value) == true ? DLMS_ERROR_CODE_NONE : DLMS_ERROR_CODE_PARSE_ERROR;
}

DLMS_ERROR_CODE DlmsReadDouble(char** buffer, int* remLen, double* value)
{
	return ReadDouble(buffer, remLen, value) == true ? DLMS_ERROR_CODE_NONE : DLMS_ERROR_CODE_PARSE_ERROR;
}

DLMS_ERROR_CODE DlmsReadString(char** buffer, int* remLen, char* value, int maxValueLen, int* readedLength)
{
	// Read 
	return ReadString(buffer, remLen, value, maxValueLen, readedLength) == true ? DLMS_ERROR_CODE_NONE : DLMS_ERROR_CODE_PARSE_ERROR;
}

DLMS_ERROR_CODE DlmsReadStruct(char** buffer, int* remLen, char** valuePtr)
{
	// Read Structure Enteries Count
	unsigned char count;
	if (!ReadChar(buffer, remLen, &count))
		return DLMS_ERROR_CODE_PARSE_ERROR;

	printf("Count:%d\t", count & 0xff);

	// Read Struture Length
	unsigned char length;
	if (!ReadChar(buffer, remLen, &length))
		return DLMS_ERROR_CODE_PARSE_ERROR;

	printf("Len:%d\t", length);

	// Calculate Rem Length
	int remLength = length - 3;
	printf("Len:%d\t", remLength);
	if (remLength < 0)
		return DLMS_ERROR_CODE_PARSE_ERROR;

	// Assign Data
	if (!Read(buffer, remLen, valuePtr, remLength))
		return DLMS_ERROR_CODE_PARSE_ERROR;

	char* t = *valuePtr;
	for (int i = 0; i < remLength; i++)
	{
		printf("%02x", *t & 0xff);
		t++;
	}

	return DLMS_ERROR_CODE_NONE;
}

void DlmsWriteDataTime(char** buffer, int* remLen, DLMS_DATE_TIME dateTime)
{
	WriteChar(buffer, remLen, 0x09);                                   // Data type: Octet string
	WriteChar(buffer, remLen, 0x0c);                                   // Lenght = 12 bytes

	if (dateTime.IsNull == 1)
	{
		WriteInt16(buffer, remLen, 0xFFFF);							   // Year
		WriteChar(buffer, remLen, 0xFF);                               // Month
		WriteChar(buffer, remLen, 0xFF);		                       // Day
	}
	else
	{
		WriteInt16(buffer, remLen, dateTime.Year);				       // Year
		WriteChar(buffer, remLen, dateTime.Month);                     // Month
		WriteChar(buffer, remLen, dateTime.Day);		               // Day
	}

	//Leave Others:  Day of week, Hour, Minute, Second, Hundreadths of second, Deviation highbyte, Deviation lowbyte, Clock status
	for (int i = 0; i < 8; i++)
		WriteChar(buffer, remLen, 0xFF);
}

DATA_TYPE DlmsGetDataType(DLMS_TYPE type)
{
	switch (type)
	{
	case DLMS_TYPE_BOOLEAN:
		return DATA_TYPE_BOOLEAN;
	case DLMS_TYPE_DOUBLE_LONG:
		return DATA_TYPE_INT32;
	case DLMS_TYPE_DOUBLE_LONG_UNSIGNED:
		return DATA_TYPE_UINT32;
	case DLMS_TYPE_INTEGER:
		return DATA_TYPE_BYTE;
	case DLMS_TYPE_LONG:
		return DATA_TYPE_INT16;
	case DLMS_TYPE_UNSIGNED:
		return DATA_TYPE_BYTE;
	case DLMS_TYPE_LONG_UNSIGEND:
		return DATA_TYPE_UINT16;
	case DLMS_TYPE_LONG64:
		return DATA_TYPE_INT64;
	case DLMS_TYPE_LONG64_UNSIGNED:
		return DATA_TYPE_UINT64;
	case DLMS_TYPE_FLOAT32:
		return DATA_TYPE_FLOAT;
	case DLMS_TYPE_FLOAT64:
		return DATA_TYPE_DOUBLE;
	case DLMS_TYPE_DATE_TIME:
		return DATA_TYPE_DATE_TIME;
	case DLMS_TYPE_DATE:
		return DATA_TYPE_DATE;
	case DLMS_TYPE_TIME:
		return DATA_TYPE_TIME;
	default:
		return DATA_TYPE_UNKNOWN;
	}
}

DLMS_ERROR_CODE DlmsReadValue(char** payloadPtr, int* length, DLMS_TYPE type, char* valuePtr)
{
	//printf("\nType:%02x\n", type);
	// Special Types
	//if (type == DLMS_TYPE_STRUCTURE)
	//{
	//	// Read Count
	//}
	/*if (type == DLMS_TYPE_STRUCTURE)
		return DlmsReadStruct(payloadPtr, length, valuePtr);*/
		//if (type == DLMS_TYPE_OCTET_STRING)
		//{
		//	// Read Length
		//	unsigned char strLen;
		//	if (!ReadChar(payloadPtr, length, &strLen))
		//		return DLMS_ERROR_CODE_PARSE_ERROR;

		//	int readedLength = 0;
		//	DLMS_ERROR_CODE errorCode = DlmsReadString(payloadPtr, length, valuePtr, MAX_VALUE_LENGTH, &readedLength);
		//	if (errorCode != DLMS_ERROR_CODE_NONE)
		//		return errorCode;
		//	if (readedLength != strLen)
		//		return DLMS_ERROR_CODE_PARSE_ERROR;

		//	return DLMS_ERROR_CODE_NONE;
		//}

	switch (DlmsGetDataType(type))
	{
	case DATA_TYPE_BYTE:
	case DATA_TYPE_BOOLEAN:
		return DlmsReadChar(payloadPtr, length, valuePtr);
	case DATA_TYPE_STRING:
	{
		int readedLength = 0;
		return DlmsReadString(payloadPtr, length, valuePtr, MAX_VALUE_LENGTH, &readedLength);
	}
	case DATA_TYPE_INT16:
	case DATA_TYPE_UINT16:
		return DlmsReadInt16(payloadPtr, length, valuePtr);
	case DATA_TYPE_INT32:
	case DATA_TYPE_UINT32:
		return DlmsReadInt32(payloadPtr, length, valuePtr);
	case DATA_TYPE_INT64:
	case DATA_TYPE_UINT64:
		return DlmsReadInt64(payloadPtr, length, valuePtr);
	case DATA_TYPE_FLOAT:
		return DlmsReadFloat(payloadPtr, length, valuePtr);
	case DATA_TYPE_DOUBLE:
		return DlmsReadDouble(payloadPtr, length, valuePtr);
		/*case DATA_TYPE_DATE:
			return DlmsReadDate(payloadPtr, length, valuePtr);
		case DATA_TYPE_TIME:
			return DlmsReadTime(payloadPtr, length, valuePtr);
		case DATA_TYPE_DATE_TIME:
			return DlmsReadDateTime(payloadPtr, length, valuePtr);*/
	default:
		//printf("\nType:%02x\n", type);
		return DLMS_ERROR_CODE_NOT_IMPLEMENTED;
	}
}

DLMS_ERROR_CODE DlmsParseRegisterScalerUnit(char* payload, int length, DLMS_REGISTER* reg)
{
	// Read Structure Data Type
	char type;
	if (!ReadChar(&payload, &length, &type) || type != (char)DLMS_TYPE_STRUCTURE)
		return DLMS_ERROR_CODE_PARSE_ERROR;

	// Read Structure Entries Count
	unsigned char count;
	if (!ReadChar(&payload, &length, &count) || count != 0x02)
		return DLMS_ERROR_CODE_PARSE_ERROR;

	// Read Scaler Type
	if (!ReadChar(&payload, &length, &type) || type != (char)DLMS_TYPE_INTEGER)
		return DLMS_ERROR_CODE_PARSE_ERROR;

	// Read Scaler
	if (!ReadChar(&payload, &length, &reg->Scaler))
		return DLMS_ERROR_CODE_PARSE_ERROR;

	// Read Enum Data Type
	if (!ReadChar(&payload, &length, &type) || type != (char)DLMS_TYPE_ENUM)
		return DLMS_ERROR_CODE_PARSE_ERROR;

	// Read Unit Code
	if (!ReadChar(&payload, &length, &reg->UnitCode))
		return DLMS_ERROR_CODE_PARSE_ERROR;

	return DLMS_ERROR_CODE_NONE;
}

DLMS_ERROR_CODE DlmsReadRegisterValue(char* payload, int length, DLMS_REGISTER* reg)
{
	// Read Returned Type
	char type;
	if (!ReadChar(&payload, &length, &type))
		return DLMS_ERROR_CODE_PARSE_ERROR;
	reg->Type = type;
	if (type == (char)DLMS_TYPE_NULL)
	{
		reg->IsNull = true;
		return DLMS_ERROR_CODE_NONE;
	}
	reg->IsNull = false;
	return DlmsReadValue(&payload, &length, reg->Type, &reg->Value);
}

char* DlmsCreateGetRequestNormal(int* length, DLMS_CLASS_ID classID, char* insID, unsigned char attrId, char* accSelection, int accSelectionLen)
{
	char* cmd = malloc(0);
	*length = 0;

	WriteChar(&cmd, length, DLMS_GET_REQ_IDEN);
	WriteChar(&cmd, length, DLMS_GET_REQ_NORMAL);
	WriteChar(&cmd, length, 0x81); // Invoke id and priority (High Priority)
	WriteInt16(&cmd, length, classID);
	Write(&cmd, length, insID, 6);
	WriteChar(&cmd, length, attrId);
	if (accSelection == NULL)
		WriteChar(&cmd, length, DLMS_TYPE_NULL);
	else
		Write(&cmd, length, accSelection, accSelectionLen);
	return cmd;
}

char* DlmsCreateGetRequestNext(int* length, int blockNumber)
{
	char* cmd = malloc(0);
	*length = 0;

	WriteChar(&cmd, length, DLMS_GET_REQ_IDEN);
	WriteChar(&cmd, length, DLMS_GET_REQ_NEXT);
	WriteChar(&cmd, length, 0x81); // Invoke id and priority (High Priority)
	WriteInt32(&cmd, length, blockNumber);
	return cmd;
}

DLMS_ERROR_CODE DlmsParseNormal(char** normalResApdu, int* normalResApduLen, char** dataPtr, int* dataLength)
{
	// Read Invoke Id And Priority
	char inv;
	if (!ReadChar(normalResApdu, normalResApduLen, &inv))
		return DLMS_ERROR_CODE_PARSE_ERROR;

	// Read Get Data Result
	char result;
	if (!ReadChar(normalResApdu, normalResApduLen, &result))
		return DLMS_ERROR_CODE_PARSE_ERROR;
	if (result == (char)DLMS_GET_DATA_RESULT_DATA)
	{
		// Read Data
		*dataPtr = *normalResApdu;
		*dataLength = *normalResApduLen;
		return DLMS_ERROR_CODE_NONE;
	}
	else if (result == (char)DLMS_GET_DATA_RESULT_DATA_ACCESS)
	{
		// Data Access Result (Part of DLMS_ERROR_CODE)
		char acc;
		if (!ReadChar(normalResApdu, normalResApduLen, &acc))
			return DLMS_ERROR_CODE_PARSE_ERROR;
		return acc;
	}
	return DLMS_ERROR_CODE_PARSE_ERROR;
}

DLMS_ERROR_CODE DlmsParseWithDataBlock(char** apdu, int* length, char** dataPtr, int* dataLength, bool* lastBlock, int* nextBlockNumber)
{
	// Read Invoke Id And Priority
	char inv;
	if (!ReadChar(apdu, length, &inv))
		return DLMS_ERROR_CODE_PARSE_ERROR;

	// Read Is Last Block
	char last;
	if (!ReadChar(apdu, length, &last))
		return DLMS_ERROR_CODE_PARSE_ERROR;
	*lastBlock = last == 0 ? false : true;

	// Read Next Block Number
	if (!ReadInt32(apdu, length, nextBlockNumber))
		return DLMS_ERROR_CODE_PARSE_ERROR;

	// Read Result
	char result;
	if (!ReadChar(apdu, length, &result))
		return DLMS_ERROR_CODE_PARSE_ERROR;
	if (result == DLMS_GET_DATA_RESULT_DATA)
	{
		// Read Data
		*dataPtr = *apdu;
		*dataLength = *length;
		return DLMS_ERROR_CODE_NONE;
	}
	else if (result == DLMS_GET_DATA_RESULT_DATA_ACCESS)
	{
		// Read Data Access Result (Part of DLMS_ERROR_CODE)
		char acc;
		if (!ReadChar(apdu, length, &acc))
			return DLMS_ERROR_CODE_PARSE_ERROR;
		return acc;
	}
	return DLMS_ERROR_CODE_PARSE_ERROR;
}

DLMS_ERROR_CODE DlmsParseResponse(char** rawDlmsApdu, int* rawDlmsApduLen, char** dataPtr, int* dataLen, bool* lastBlock, int* nextBlockNumber)
{
	// Read Get Response Code
	char respIden;
	if (!ReadChar(rawDlmsApdu, rawDlmsApduLen, &respIden) || respIden != (char)DLMS_GET_RES_IDEN)
		return DLMS_ERROR_CODE_PARSE_ERROR;

	// Read Get Response Type
	char resType;
	if (!ReadChar(rawDlmsApdu, rawDlmsApduLen, &resType))
		return DLMS_ERROR_CODE_PARSE_ERROR;

	if (resType == (char)DLMS_GET_RES_NORMAL)
	{
		*lastBlock = true;
		*nextBlockNumber = 0;
		return DlmsParseNormal(rawDlmsApdu, rawDlmsApduLen, dataPtr, dataLen);
	}
	else if (resType == (char)DLMS_GET_RES_WITH_DATA_BLOCK)
		return DlmsParseWithDataBlock(rawDlmsApdu, rawDlmsApduLen, dataPtr, dataLen, lastBlock, nextBlockNumber);

	// With list is not implemented
	return DLMS_ERROR_CODE_NOT_IMPLEMENTED;

}

MEDIA_ERROR DlmsSendAARQ(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, HDLC_COMM_PARAMS* comParams, char* password, int passwordLength, char** response, int* length, DLMS_ERROR_CODE* errorCode)
{
	char* data = malloc(0);
	int dataLength = 0;

	char appContextName[] = { 0xa1, 0x09, 0x06, 0x07 };
	char appContextValue[] = { 0x60, 0x85, 0x74, 0x05, 0x08, 0x01, 0x01 };
	char lowLevelSec[] = { 0x60, 0x85, 0x74, 0x05, 0x08, 0x02, 0x01 };

	WriteChar(&data, &dataLength, 0x60); // Aarq tag
	WriteChar(&data, &dataLength, 0x01); // Packet len from here to the end must update later
	Write(&data, &dataLength, &appContextName, sizeof(appContextName) / sizeof(char));
	Write(&data, &dataLength, &appContextValue, sizeof(appContextValue) / sizeof(char));
	WriteChar(&data, &dataLength, 0x8a); // Value of the tag of access requirments field
	WriteChar(&data, &dataLength, 0x02); // Length of the tagged component’s value field
	WriteChar(&data, &dataLength, 0x07); // Number of unused bits in the last byte of the BIT
	WriteChar(&data, &dataLength, 0x80); // Authentication functional unit
	WriteChar(&data, &dataLength, 0x8b); // The tag ([11], IMPLICIT, Context-specific)
	WriteChar(&data, &dataLength, 0x07); // The length of the tagged component’s value field
	Write(&data, &dataLength, &lowLevelSec, sizeof(lowLevelSec) / sizeof(char));
	WriteChar(&data, &dataLength, 0xac); // The tag ([12], EXPLICIT, Context-specific)
	WriteChar(&data, &dataLength, passwordLength + 2);	// 09 The length of the tagged component’s value field
	WriteChar(&data, &dataLength, 0x80); // Authentication-value (char string [0])
	WriteChar(&data, &dataLength, passwordLength); // 0x07
	Write(&data, &dataLength, password, passwordLength);
	WriteChar(&data, &dataLength, 0xbe); // Tag and length for AARQ user field
	WriteChar(&data, &dataLength, 0x10);
	WriteChar(&data, &dataLength, 0x04); // Encoding the choice for user-information (OCTET STRING, universal) and length
	WriteChar(&data, &dataLength, 0x0e);
	WriteChar(&data, &dataLength, 0x01); // Tag for xDLMS-Initiate request
	WriteChar(&data, &dataLength, 0x00); // Usage field for dedicated-key component – not used
	WriteChar(&data, &dataLength, 0x00); // Usage field for the response allowed component – not used
	WriteChar(&data, &dataLength, 0x00); // Usage field of the proposed-quality-of-service component – not used
	WriteChar(&data, &dataLength, 0x06); // Proposed dlms version number 6
	WriteChar(&data, &dataLength, 0x5f); // Tag for conformance block
	WriteChar(&data, &dataLength, 0x1f);
	WriteChar(&data, &dataLength, 0x04);
	WriteChar(&data, &dataLength, 0x00);
	WriteChar(&data, &dataLength, 0x1c);
	WriteChar(&data, &dataLength, 0x1c);
	WriteChar(&data, &dataLength, 0x3d);
	WriteChar(&data, &dataLength, 0x00);
	WriteChar(&data, &dataLength, 0x00);

	//update packet len
	data[1] = dataLength - 2;

	// Send
	return HdlcSendInfo(readCallback, writeCallback, comParams, data, dataLength, response, length, errorCode);
}

MEDIA_ERROR DlmsReadObject(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, HDLC_COMM_PARAMS* comParams, char* address, DLMS_CLASS_ID classID, unsigned char attrIndex, char* accessSelection, int accessSelectionLength, char** data, int* dataLength, DLMS_ERROR_CODE* errorCode)
{
	*errorCode = DLMS_ERROR_CODE_NONE;
	*data = malloc(0);
	*dataLength = 0;

	int cmdLength = 0;
	char* cmd = DlmsCreateGetRequestNormal(&cmdLength, classID, address, attrIndex, accessSelection, accessSelectionLength);
	char* response;
	int responseLength;
	MEDIA_ERROR ret = HdlcSendInfo(readCallback, writeCallback, comParams, cmd, cmdLength, &response, &responseLength, errorCode);
	if (ret != MEDIA_ERROR_NONE || *errorCode != DLMS_ERROR_CODE_NONE)
	{
		// Cleanup
		free(response);
		return ret;
	}

	// Until last block received
	int reqBlock = 0;
	char* responsePtr;
	char* responseDataPtr = NULL;
	int responseDataLength;
	for (;;)
	{
		bool lastBlock = false;
		int nextBlock = 0;
		responsePtr = response;

		*errorCode = DlmsParseResponse(&responsePtr, &responseLength, &responseDataPtr, &responseDataLength, &lastBlock, &nextBlock);
		if (*errorCode != DLMS_ERROR_CODE_NONE)
		{
			// Cleanup
			free(response);
			return ret;
		}

		// Copy Data
		Write(data, dataLength, responseDataPtr, responseDataLength);

		// Cleanup
		free(response);

		// All Data Blocks has been received
		if (lastBlock == true)
			return ret;

		reqBlock++;
		if (nextBlock != reqBlock)
			return DLMS_ERROR_CODE_DATA_BLOCK_NUMBER_INVALID;

		int nextCmdLen;
		char* nextCmd = DlmsCreateGetRequestNext(&nextCmdLen, reqBlock);
		ret = HdlcSendInfo(readCallback, writeCallback, comParams, nextCmd, nextCmdLen, &response, &responseLength, errorCode);
		if (ret != MEDIA_ERROR_NONE || *errorCode != DLMS_ERROR_CODE_NONE)
		{
			// Cleanup
			free(response);
			return ret;
		}
	}
}

MEDIA_ERROR DlmsReadRegisterScalerUnit(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, HDLC_COMM_PARAMS* comParams, DLMS_REGISTER* reg, DLMS_ERROR_CODE* errorCode)
{
	// Init
	*errorCode = DLMS_ERROR_CODE_NONE;

	// Read Register Scaler And Unit
	char* data;
	int dataLength;
	MEDIA_ERROR ret = DlmsReadObject(readCallback, writeCallback, comParams, &reg->Address, DLMS_CLASS_ID_REGISTER, DLMS_REGISTER_ATTRS_SCALER_UNIT, NULL, 0, &data, &dataLength, errorCode);
	if (ret != MEDIA_ERROR_NONE || *errorCode != DLMS_ERROR_CODE_NONE)
	{
		// Cleanup
		free(data);

		return ret;
	}
	*errorCode = DlmsParseRegisterScalerUnit(data, dataLength, reg);

	// Cleanup
	free(data);

	return ret;
}

DLMS_ERROR_CODE DlmsReadProfileLengthField(char** data, int* length, int* lengthValue)
{
	unsigned char value;
	if (!ReadChar(data, length, &value))
		return DLMS_ERROR_CODE_PARSE_ERROR;
	if (value == 0x82)
	{
		short len;
		if (!ReadInt16(data, length, &len))
			return DLMS_ERROR_CODE_PARSE_ERROR;
		*lengthValue = len;
	}
	else
		*lengthValue = value;

	return DLMS_ERROR_CODE_NONE;
}

DLMS_ERROR_CODE DlmsParseProfile(char** data, int* length, DLMS_FILE_CHANNEL_INFO* channels, char channelsCount, DLMS_FILE_FIELD** fields, int* rowsCount)
{
	// Read 1ST Header
	int sectionLength = 0;
	char* sectionStartPtr = NULL;
	char* nextSectionStartPtr = NULL;
	DLMS_ERROR_CODE errorCode = DLMS_ERROR_CODE_NONE;

	char type;
	if (!ReadChar(data, length, &type))
		return DLMS_ERROR_CODE_PARSE_ERROR;

	// Check If Has Sections
	bool hasSection = false;
	if (type != (char)DLMS_TYPE_ARRAY)
	{
		// Rollback
		*data = *data - 1;
		*length = *length + 1;

		errorCode = DlmsReadProfileLengthField(data, length, &sectionLength);
		if (errorCode != DLMS_ERROR_CODE_NONE)
			return errorCode;
		sectionStartPtr = *data;
		nextSectionStartPtr = sectionStartPtr + sectionLength;
		hasSection = true;

		// Read Profile Array
		if (!ReadChar(data, length, &type) || type != (char)DLMS_TYPE_ARRAY)
			return DLMS_ERROR_CODE_PARSE_ERROR;
	}

	*rowsCount = 0;
	errorCode = DlmsReadProfileLengthField(data, length, rowsCount);
	if (errorCode != DLMS_ERROR_CODE_NONE)
		return errorCode;

	// Cleanup fields
	free(*fields);

	*fields = malloc((double)(*rowsCount) * channelsCount * sizeof(DLMS_FILE_FIELD));
	DLMS_FILE_FIELD* fieldPtr = *fields;

	for (int r = 0; r < *rowsCount; r++)
	{
		// Check If First Section Or Section Ends
		if (hasSection == true && (sectionStartPtr == NULL || *data == nextSectionStartPtr))
		{
			errorCode = DlmsReadProfileLengthField(data, length, &sectionLength);
			if (errorCode != DLMS_ERROR_CODE_NONE)
				return errorCode;
			sectionStartPtr = *data;
			nextSectionStartPtr = sectionStartPtr + sectionLength;
		}

		// Read Row Structure
		if (!ReadChar(data, length, &type) || type != (char)DLMS_TYPE_STRUCTURE)
			return DLMS_ERROR_CODE_PARSE_ERROR;

		char elementsCount = 0;
		if (!ReadChar(data, length, &elementsCount) || elementsCount != channelsCount)
			return DLMS_ERROR_CODE_PARSE_ERROR;

		// Row 0 May Contain From Column Description
		if (r == 0)
		{
			unsigned char t;
			if (!ReadChar(data, length, &t))
				return DLMS_ERROR_CODE_PARSE_ERROR;

			if (t == DLMS_TYPE_STRUCTURE)
			{
				unsigned char tCount;
				if (!ReadChar(data, length, &tCount) || tCount != 2)
					return DLMS_ERROR_CODE_PARSE_ERROR;

				// Read From
				if (!ReadChar(data, length, &t) || t != DLMS_TYPE_OCTET_STRING)
					return DLMS_ERROR_CODE_PARSE_ERROR;
				if (!ReadChar(data, length, &tCount) || tCount != 0x0C)
					return DLMS_ERROR_CODE_PARSE_ERROR;
				char v[0x0C];
				if (!Read(data, length, &v, 0x0C))
					return DLMS_ERROR_CODE_PARSE_ERROR;

				// Read Column Index
				unsigned char columnIndex;
				if (!ReadChar(data, length, &columnIndex))
					return DLMS_ERROR_CODE_PARSE_ERROR;
			}
			else
			{
				// Rollback
				*data = *data - 1;
				*length = *length + 1;
			}
		}

		// Read Columns
		for (int c = 0; c < channelsCount; c++)
		{
			// Read Field Type
			if (!ReadChar(data, length, &fieldPtr->Type))
				return DLMS_ERROR_CODE_PARSE_ERROR;
			if (fieldPtr->Type == DLMS_TYPE_NULL)
				fieldPtr->IsNull = true;
			else
			{
				// Read Field Value
				errorCode = DlmsReadValue(data, length, fieldPtr->Type, &fieldPtr->Value);
				fieldPtr->IsNull = false;
				if (errorCode != DLMS_ERROR_CODE_NONE)
					return errorCode;
			}

			fieldPtr++;
		}
	}

	return DLMS_ERROR_CODE_NONE;
}

EXPORT unsigned int DlmsGetServerID(unsigned int physicalAddress, unsigned int logicalAddress)
{
	return (((((((unsigned int)physicalAddress >> 7) & 127) << 8) | ((unsigned int)physicalAddress & 127)) << 1)) | (1 | logicalAddress << 17);
}

EXPORT MEDIA_ERROR DlmsLogin(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, HDLC_COMM_PARAMS* comParams, char* password, int passwordLength, char* errCode)
{
	DLMS_ERROR_CODE errorCode = DLMS_ERROR_CODE_NONE;

	MEDIA_ERROR ret = HdlcInitTransaction(readCallback, writeCallback, comParams, &errorCode);
	if (ret != MEDIA_ERROR_NONE || errorCode != DLMS_ERROR_CODE_NONE)
	{
		*errCode = errorCode;
		return ret;
	}

	// Send AARQ
	char* response;
	int responseLength;
	ret = DlmsSendAARQ(readCallback, writeCallback, comParams, password, passwordLength, &response, &responseLength, &errorCode);
	if (ret != MEDIA_ERROR_NONE || errorCode != DLMS_ERROR_CODE_NONE)
	{
		// Cleanup
		free(response);

		*errCode = errorCode;
		return ret;
	}

	if (responseLength >= 12 + passwordLength)
	{
		int passIndex = 0;
		for (int i = 12; i < responseLength; i++)
		{
			// Authentication Success
			if (passIndex == passwordLength)
			{
				free(response);
				*errCode = DLMS_ERROR_CODE_NONE;
				return MEDIA_ERROR_NONE;
			}

			if (response[i] == password[passIndex])
				passIndex++;
			else
				passIndex = 0;
		}
	}

	free(response);
	*errCode = DLMS_ERROR_CODE_AUTHENTICATION_FAILED;
	return MEDIA_ERROR_NONE;
}

EXPORT MEDIA_ERROR DlmsLogout(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, HDLC_COMM_PARAMS comParams, char* password, int passwordLength, char* errCode)
{
	DLMS_ERROR_CODE errorCode = DLMS_ERROR_CODE_NONE;

	MEDIA_ERROR ret = HDlcTerminateTransaction(readCallback, writeCallback, comParams, &errorCode);
	if (ret != MEDIA_ERROR_NONE || errorCode != DLMS_ERROR_CODE_NONE)
	{
		*errCode = errorCode;
		return ret;
	}

	*errCode = DLMS_ERROR_CODE_NONE;
	return MEDIA_ERROR_NONE;
}

EXPORT MEDIA_ERROR DlmsReadRegister(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, HDLC_COMM_PARAMS* comParams, DLMS_REGISTER* reg, char* errCode)
{
	// Init;
	DLMS_ERROR_CODE errorCode = DLMS_ERROR_CODE_NONE;

	// Read Register Scaler And Unit
	MEDIA_ERROR ret = DlmsReadRegisterScalerUnit(readCallback, writeCallback, comParams, reg, &errorCode);

	if (ret != MEDIA_ERROR_NONE || errorCode != DLMS_ERROR_CODE_NONE)
	{
		*errCode = errorCode;
		return ret;
	}

	// Read Register Value
	char* data;
	int dataLength;
	ret = DlmsReadObject(readCallback, writeCallback, comParams, &reg->Address, DLMS_CLASS_ID_REGISTER, DLMS_REGISTER_ATTRS_VALUE, NULL, 0, &data, &dataLength, &errorCode);
	if (ret != MEDIA_ERROR_NONE || errorCode != DLMS_ERROR_CODE_NONE)
	{
		// cleanup
		free(data);

		*errCode = errorCode;
		return ret;
	}
	errorCode = DlmsReadRegisterValue(data, dataLength, reg);

	// Cleanup
	free(data);

	*errCode = errorCode;
	return ret;
}

EXPORT MEDIA_ERROR DlmsReadProfileChannels(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, HDLC_COMM_PARAMS* comParams, DLMS_FILE_INFO fileInfo, DLMS_FILE_CHANNEL_INFO** channels, unsigned char* channelsCount, char* errCode)
{
	// Init
	*channels = malloc(0);
	*channelsCount = 0;
	DLMS_ERROR_CODE errorCode = DLMS_ERROR_CODE_NONE;

	char* data;
	int dataLength;
	MEDIA_ERROR ret = DlmsReadObject(readCallback, writeCallback, comParams, &fileInfo.Address, DLMS_CLASS_ID_PROFILE_GENERIC, DLMS_FILE_ATTRS_CAPTURE_OBJECTS, NULL, 0, &data, &dataLength, &errorCode);
	if (ret == MEDIA_ERROR_NONE && errorCode == DLMS_ERROR_CODE_NONE)
	{
		char* dataPtr = data;

		// Read Array Type
		unsigned char type;
		if (ReadChar(&dataPtr, &dataLength, &type) && type == DLMS_TYPE_ARRAY)
		{
			// Read Array Element Count
			unsigned char arrCount;
			if (ReadChar(&dataPtr, &dataLength, &arrCount))
			{
				free(*channels);
				*channels = malloc(arrCount * sizeof(DLMS_FILE_CHANNEL_INFO));
				*channelsCount = arrCount;

				DLMS_FILE_CHANNEL_INFO* cInfoPtr = *channels;
				for (unsigned char c = 0; c < *channelsCount; c++)
				{
					// Read Structure Type
					if (!ReadChar(&dataPtr, &dataLength, &type) || type != DLMS_TYPE_STRUCTURE)
					{
						errorCode = DLMS_ERROR_CODE_PARSE_ERROR;
						break;
					}

					// Read Structure Count
					unsigned char structCount;
					if (!ReadChar(&dataPtr, &dataLength, &structCount))
					{
						errorCode = DLMS_ERROR_CODE_PARSE_ERROR;
						break;
					}

					// Read Structure Length
					unsigned char structLength;
					if (!ReadChar(&dataPtr, &dataLength, &structLength))
					{
						errorCode = DLMS_ERROR_CODE_PARSE_ERROR;
						break;
					}

					// Read ClassID
					if (!ReadInt16(&dataPtr, &dataLength, &cInfoPtr->ClassID))
					{
						errorCode = DLMS_ERROR_CODE_PARSE_ERROR;
						break;
					}

					// Read Address
					unsigned char addrType;
					if (!ReadChar(&dataPtr, &dataLength, &addrType) || addrType != DLMS_TYPE_OCTET_STRING)
					{
						errorCode = DLMS_ERROR_CODE_PARSE_ERROR;
						break;
					}
					unsigned char addrLength;
					if (!ReadChar(&dataPtr, &dataLength, &addrLength) || addrLength != DLMS_LOG_ADDR_LEN)
					{
						errorCode = DLMS_ERROR_CODE_PARSE_ERROR;
						break;
					}
					bool addrReadSuccess = true;
					for (unsigned char i = 0; i < DLMS_LOG_ADDR_LEN; i++)
					{
						unsigned char addrI;
						if (!ReadChar(&dataPtr, &dataLength, &addrI))
						{
							addrReadSuccess = false;
							break;
						}
						cInfoPtr->Address[i] = addrI;
					}
					if (!addrReadSuccess)
					{
						errorCode = DLMS_ERROR_CODE_PARSE_ERROR;
						break;
					}

					// Read Attribute Index
					unsigned char attrType;
					if (!ReadChar(&dataPtr, &dataLength, &attrType) || attrType != DLMS_TYPE_INTEGER)
					{
						errorCode = DLMS_ERROR_CODE_PARSE_ERROR;
						break;
					}
					if (!ReadChar(&dataPtr, &dataLength, &cInfoPtr->AttributeIndex))
					{
						errorCode = DLMS_ERROR_CODE_PARSE_ERROR;
						break;
					}

					// Read Data Index
					unsigned char dataType;
					if (!ReadChar(&dataPtr, &dataLength, &dataType) || dataType != DLMS_TYPE_LONG_UNSIGEND)
					{
						errorCode = DLMS_ERROR_CODE_PARSE_ERROR;
						break;
					}
					if (!ReadInt16(&dataPtr, &dataLength, &cInfoPtr->DataIndex))
					{
						errorCode = DLMS_ERROR_CODE_PARSE_ERROR;
						break;
					}

					// Next Column
					cInfoPtr++;
				}
			}
			else
				errorCode = DLMS_ERROR_CODE_PARSE_ERROR;
		}
		else
			errorCode = DLMS_ERROR_CODE_PARSE_ERROR;
	}

	// Cleanup
	free(data);

	// Read Channels Scaler And UnitCode
	//DLMS_FILE_CHANNEL_INFO* channelPtr = *channels;
	//for (int c = 0; c < *channelsCount; c++)
	//{
	//	if (channelPtr->ClassID == DLMS_CLASS_ID_REGISTER)
	//	{
	//		DLMS_REGISTER reg;
	//		for (int i = 0; i < DLMS_LOG_ADDR_LEN; i++)
	//			reg.Address[i] = channelPtr->Address[i];
	//		DLMS_ERROR_CODE e;
	//		ret = DlmsReadRegisterScalerUnit(readCallback, writeCallback, comParams, &reg, &errorCode);
	//		/*if (ret != MEDIA_ERROR_NONE || errorCode != DLMS_ERROR_CODE_NONE)
	//			continue;*/
	//	}
	//	channelPtr++;
	//}

	* errCode = errorCode;
	return ret;
}

EXPORT MEDIA_ERROR DlmsReadProfile(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, HDLC_COMM_PARAMS* comParams, DLMS_DATE_TIME* from, DLMS_DATE_TIME* to, DLMS_FILE_INFO fileInfo, DLMS_FILE_CHANNEL_INFO* channels, unsigned char channelsCount, DLMS_FILE_FIELD** fields, int* rowsCount, char* errCode)
{
	// Init
	*fields = malloc(0);
	*rowsCount = 0;
	DLMS_ERROR_CODE errorCode = DLMS_ERROR_CODE_NONE;

	char* accSelection = malloc(0);
	int accSelectionLength = 0;

	WriteChar(&accSelection, &accSelectionLength, DLMS_ACCESS_SELECTOR_RANGE_DESCRIPTOR);

	// Range Descriptor Clock
	WriteChar(&accSelection, &accSelectionLength, 0x01); // Array
	WriteChar(&accSelection, &accSelectionLength, 0x02); // Count
	WriteChar(&accSelection, &accSelectionLength, 0x04); // From Column Index
	WriteChar(&accSelection, &accSelectionLength, 0x00); // To Column Index = NULL (Means Same As From Index)

	// From Value
	DlmsWriteDataTime(&accSelection, &accSelectionLength, *from);

	// To Value
	DlmsWriteDataTime(&accSelection, &accSelectionLength, *to);

	// Array Of Columns To Retrive
	WriteChar(&accSelection, &accSelectionLength, DLMS_TYPE_ARRAY);
	WriteChar(&accSelection, &accSelectionLength, channelsCount);

	for (int c = 0; c < channelsCount; c++)
	{
		// Data Structure Of Column c
		WriteChar(&accSelection, &accSelectionLength, DLMS_TYPE_STRUCTURE);
		WriteChar(&accSelection, &accSelectionLength, 0x04); // Structure Enteries Count
		WriteChar(&accSelection, &accSelectionLength, 0x12); // Struture Length

		// ClassID
		WriteInt16(&accSelection, &accSelectionLength, channels[c].ClassID);

		// Write Logical Address
		WriteChar(&accSelection, &accSelectionLength, DLMS_TYPE_OCTET_STRING);
		WriteChar(&accSelection, &accSelectionLength, 0x06);
		Write(&accSelection, &accSelectionLength, &channels[c].Address, DLMS_LOG_ADDR_LEN);

		// Write Attribute Index
		WriteChar(&accSelection, &accSelectionLength, DLMS_TYPE_INTEGER);
		WriteChar(&accSelection, &accSelectionLength, channels[c].AttributeIndex); // Value

		// Write Data Index
		WriteChar(&accSelection, &accSelectionLength, DLMS_TYPE_LONG_UNSIGEND);
		WriteInt16(&accSelection, &accSelectionLength, channels[c].DataIndex);
	}

	char* data;
	int dataLength;
	MEDIA_ERROR ret = DlmsReadObject(readCallback, writeCallback, comParams, &fileInfo.Address,
		DLMS_CLASS_ID_PROFILE_GENERIC, DLMS_FILE_ATTRS_BUFFER, accSelection, accSelectionLength, &data, &dataLength, &errorCode);
	if (ret != MEDIA_ERROR_NONE || errorCode != DLMS_ERROR_CODE_NONE)
	{
		// Cleanup
		free(data);

		*errCode = errorCode;
		return ret;
	}

	// Parse Profile
	char* dataPtr = data;
	*errCode = DlmsParseProfile(&dataPtr, &dataLength, channels, channelsCount, fields, rowsCount);
	// Cleanup
	free(data);
	return ret;
}
