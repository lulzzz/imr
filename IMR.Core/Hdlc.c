#include "Hdlc.h"

// Send:
//  1. Send Frame
//  2. Update S

//  Receive:
//  1. Receive Frame
//  2. CheckSR
//  3. Update R

unsigned char HdlcUpdateS(unsigned char s, unsigned char winSizeTransmit)
{
	return (s + 1) % HDLC_MODULUS;
}

unsigned char HdlcUpdateR(unsigned char r, unsigned char winSizeReceive)
{
	return (r + 1) % HDLC_MODULUS;
}

bool HdlcCheckSR(unsigned char s, unsigned char r, unsigned char meterS, unsigned char meterR)
{
	//printf("\n s:%d, r:%d, ms:%d, mr:%d\n", s, r, meterS, meterR);

	// Check S
	if (s != 255 && s != meterR)
		return false;

	// Check R
	if (r != 255 && r != meterS)
		return false;

	return true;
}

unsigned short HdlcCalculateCS(char* buffer, int startIndex, int count, bool applyTransformation)
{
	unsigned short fcs = HDLC_PPP_INIT_FCS16;
	for (int i = count; i > 0; i--)
		fcs = (unsigned short)(((fcs >> 8) & 0xFF) ^ HdlcFCSTable[(fcs ^ buffer[startIndex++]) & 0xFF]);

	if (!applyTransformation)
		return fcs;

	fcs = (unsigned short)(~fcs);
	unsigned short reverseByteOrder = (unsigned short)(((fcs >> 8) & 0xFF) | ((fcs << 8) & 0xFF00));
	return reverseByteOrder;
}

bool HdlcCheckHCS(char* buffer, int length)
{
	if (length < 11)
		return false;

	unsigned short calculatedCS = HdlcCalculateCS(buffer, 1, 10, false);
	if (calculatedCS == DHLC_PPP_GOOD_FCS16)
		return true;

	return false;
}

bool HdlcCheckFCS(char* buffer, int length)
{
	if (length < 2)
		return false;

	unsigned short calculatedCS = HdlcCalculateCS(buffer, 1, length - 2, false);
	if (calculatedCS == DHLC_PPP_GOOD_FCS16)
		return true;

	return false;
}

void HdlcWriteParam(char** packet, int* packetLenght, char paramIden, char paramLength, char* paramValue)
{
	WriteChar(packet, packetLenght, paramIden);				// Parameter Identifier
	WriteChar(packet, packetLenght, paramLength);			// Parameter Length
	Write(packet, packetLenght, paramValue, paramLength);	// Parameter Value
}

void HdlcWriteCharParam(char** packet, int* length, char paramIden, char paramValue)
{
	HdlcWriteParam(packet, length, paramIden, sizeof(char), &paramValue);
}

DLMS_ERROR_CODE HdlcReadParam(char** buffer, int* remLength, char paramIden, char* paramLength, char** paramValuePtr)
{
	// Read Parameter Identifier
	char iden;
	if (!ReadChar(buffer, remLength, &iden) | iden != paramIden)
		return DLMS_ERROR_CODE_PARSE_ERROR;

	// Read Parameter Length
	if (!ReadChar(buffer, remLength, paramLength) || *paramLength == 0)
		return DLMS_ERROR_CODE_PARSE_ERROR;

	// Read Parameter Value
	if (!Read(buffer, remLength, paramValuePtr, *paramLength))
		return DLMS_ERROR_CODE_PARSE_ERROR;

	return DLMS_ERROR_CODE_NONE;
}

DLMS_ERROR_CODE HdlcTryParseChar(char* buffer, int length, char* value)
{
	if (length > sizeof(char))
	{
		// Skip
		int skip = length - sizeof(char);
		buffer += skip;
		length -= skip;
	}

	if (!ReadChar(&buffer, &length, value))
		return DLMS_ERROR_CODE_PARSE_ERROR;
	return DLMS_ERROR_CODE_NONE;
}

DLMS_ERROR_CODE HdlcTryParseInt16(char* buffer, int length, short* value)
{
	if (length > sizeof(short))
	{
		// Skip
		int skip = length - sizeof(short);
		buffer += skip;
		length -= skip;
	}
	if (length < sizeof(short))
	{
		char lvalue;
		DLMS_ERROR_CODE errorCode = HdlcTryParseChar(buffer, length, &lvalue);
		if (errorCode != DLMS_ERROR_CODE_NONE)
			return errorCode;
		*value = lvalue;
		return DLMS_ERROR_CODE_NONE;
	}
	if (!ReadInt16(&buffer, &length, value))
		return DLMS_ERROR_CODE_PARSE_ERROR;
	return DLMS_ERROR_CODE_NONE;
}

DLMS_ERROR_CODE HdlcTryParseInt32(char* buffer, int length, int* value)
{
	if (length > sizeof(int))
	{
		// Skip
		int skip = length - sizeof(int);
		buffer += skip;
		length -= skip;
	}
	if (length < sizeof(int))
	{
		short lvalue;
		DLMS_ERROR_CODE errorCode = HdlcTryParseInt16(buffer, length, &lvalue);
		if (errorCode != DLMS_ERROR_CODE_NONE)
			return errorCode;
		*value = lvalue;
		return DLMS_ERROR_CODE_NONE;
	}
	if (!ReadInt32(&buffer, &length, value))
		return DLMS_ERROR_CODE_PARSE_ERROR;
	return DLMS_ERROR_CODE_NONE;
}

DLMS_ERROR_CODE HdlcTryParseInt64(char* buffer, int length, long* value)
{
	if (length > sizeof(long))
	{
		// Skip
		int skip = length - sizeof(long);
		buffer += skip;
		length -= skip;
	}
	if (length < sizeof(long))
	{
		int lvalue;
		DLMS_ERROR_CODE errorCode = HdlcTryParseInt32(buffer, length, &lvalue);
		if (errorCode != DLMS_ERROR_CODE_NONE)
			return errorCode;
		*value = lvalue;
		return DLMS_ERROR_CODE_NONE;
	}
	if (!ReadInt64(&buffer, &length, value))
		return DLMS_ERROR_CODE_PARSE_ERROR;
	return DLMS_ERROR_CODE_NONE;
}

DLMS_ERROR_CODE HdlcReadCharParam(char** buffer, int* remLength, char paramIden, char* value)
{
	char* paramValuePtr;
	char paramLength;
	DLMS_ERROR_CODE errorCode = HdlcReadParam(buffer, remLength, paramIden, &paramLength, &paramValuePtr);
	if (errorCode != DLMS_ERROR_CODE_NONE)
		return errorCode;
	return HdlcTryParseChar(paramValuePtr, paramLength, value);
}

DLMS_ERROR_CODE HdlcReadInt16Param(char** buffer, int* remLength, char paramIden, short* value)
{
	char* paramValuePtr;
	char paramLength;
	DLMS_ERROR_CODE errorCode;
	errorCode = HdlcReadParam(buffer, remLength, paramIden, &paramLength, &paramValuePtr);
	if (errorCode != DLMS_ERROR_CODE_NONE)
		return errorCode;
	return HdlcTryParseInt16(paramValuePtr, paramLength, value);
}

DLMS_ERROR_CODE HdlcReadInt32Param(char** buffer, int* remLength, char paramIden, int* value)
{
	char* paramValuePtr;
	char paramLength;
	DLMS_ERROR_CODE errorCode;
	errorCode = HdlcReadParam(buffer, remLength, paramIden, &paramLength, &paramValuePtr);
	if (errorCode != DLMS_ERROR_CODE_NONE)
		return errorCode;
	return HdlcTryParseInt32(paramValuePtr, paramLength, value);
}

DLMS_ERROR_CODE HdlcReadInt64Param(char** buffer, int* remLength, char paramIden, long* value)
{
	char* paramValuePtr;
	char paramLength;
	DLMS_ERROR_CODE errorCode;
	errorCode = HdlcReadParam(buffer, remLength, paramIden, &paramLength, &paramValuePtr);
	if (errorCode != DLMS_ERROR_CODE_NONE)
		return errorCode;
	return HdlcTryParseInt64(paramValuePtr, paramLength, value);
}

void LLCEncapsulate(char** data, int* length)
{
	char* output = malloc(0);
	int outputLength = 0;
	WriteChar(&output, &outputLength, 0xE6); // Destination LSAP
	WriteChar(&output, &outputLength, 0xE6); // Source LSAP
	WriteChar(&output, &outputLength, 0x00); // Control
	Write(&output, &outputLength, *data, *length);

	// Cleanup
	free(*data);

	*data = output;
	*length = outputLength;
}

DLMS_ERROR_CODE LLCDencapsulate(char** data, int* length)
{
	char* dataPtr = *data;
	char destLSAP;
	if (!ReadChar(&dataPtr, length, &destLSAP) || destLSAP != (char)0xE6)
		return DLMS_ERROR_CODE_PARSE_ERROR;
	char sourceLSAP;
	if (!ReadChar(&dataPtr, length, &sourceLSAP) || sourceLSAP != (char)0xE7)
		return DLMS_ERROR_CODE_PARSE_ERROR;
	char control;
	if (!ReadChar(&dataPtr, length, &control))
		return DLMS_ERROR_CODE_PARSE_ERROR;

	char* output = malloc(0);
	int outputLength = 0;
	Write(&output, &outputLength, dataPtr, *length);

	// Cleanup
	free(*data);

	*data = output;
	*length = outputLength;

	return DLMS_ERROR_CODE_NONE;
}

bool HdlcGetSegment(HDLC_COMM_PARAMS comParams, char** data, int* dataLength, char** segmentPtr, int* segmentLength, bool* segmented)
{
	if (*dataLength <= 0)
		return false;

	*segmentLength = (*dataLength < comParams.MaxInfoLenTransmit) ? *dataLength : comParams.MaxInfoLenTransmit;
	Read(data, dataLength, segmentPtr, *segmentLength);
	*segmented = *dataLength == 0 ? false : true;
	return true;
}

/*
 * Control field format
 *  ----------------------------
 *  |      | MSB      | LSB    |
 *  ----------------------------
 *  | I    | R R R P/F S S S 0 |    Command, Response
 *  | RR   | R R R P/F 0 0 0 1 |    Command, Response
 *  | RNR  | R R R P/F 0 1 0 1 |    Command, Response
 *  | SNRM | 1 0 0 P   0 0 1 1 |    Command
 *  | DISC | 0 1 0 P   0 0 1 1 |    Command
 *  | UA   | 0 1 1 F   0 0 1 1 |    Response
 *  | DM   | 0 0 0 F   1 1 1 1 |    Response
 *  | FRMR | 1 0 0 F   0 1 1 1 |    Response
 *  | UI   | 0 0 0 P/F 0 0 1 1 |    Command
 *  ----------------------------
 */
unsigned char HdlcCreateControlField(HDLC_COMM_PARAMS comParams, HDLC_REQUEST_FRAME_TYPE frameType, bool poll)
{
	unsigned char controlField;
	unsigned char pollMask = (poll == true) ? 0x10 : 0x00;
	unsigned char s = comParams.S;
	unsigned char r = comParams.R;

	switch (frameType)
	{
	case HDLC_REQUEST_FRAME_TYPE_RR:
		s = 0x01;
		break;
	case HDLC_REQUEST_FRAME_TYPE_RNR:
		s = 0x05;
		break;
	case HDLC_REQUEST_FRAME_TYPE_SNRM:
		s = 0x03;
		r = 0x04;
		break;
	case HDLC_REQUEST_FRAME_TYPE_DISC:
		s = 0x03;
		r = 0x02;
		break;
	case HDLC_REQUEST_FRAME_TYPE_UI:
		s = 0x03;
		r = 0x00;
		break;
	}

	// Apply S And R
	controlField = r;
	controlField = (controlField << 5) & 0xE0;
	unsigned char helper = s & 0x0F;
	if (frameType == HDLC_REQUEST_FRAME_TYPE_I)
		helper = (s << 1) & 0x0E;
	controlField = (controlField | helper) & 0xFF;
	controlField = (controlField | pollMask) & 0xFF;
	return controlField;
}

char* HdlcBeginInitPacket(int* length, HDLC_COMM_PARAMS comParams, HDLC_REQUEST_FRAME_TYPE frameType, bool poll)
{
	char* packet = malloc(0);
	*length = 0;

	// Write Startin FLAG
	WriteChar(&packet, length, HDLC_STARTING_ENDING_FLAG);

	// Write Frame Format
	WriteInt16(&packet, length, 0x0000);

	// Write Destination Address
	WriteInt32(&packet, length, comParams.ServerID);

	// Write Source Address
	WriteChar(&packet, length, comParams.ClientID);

	// Write Control Field
	WriteChar(&packet, length, HdlcCreateControlField(comParams, frameType, poll));

	// Write HCS
	if (frameType == HDLC_REQUEST_FRAME_TYPE_SNRM || frameType == HDLC_REQUEST_FRAME_TYPE_I)
		WriteInt16(&packet, length, 0xFFFF);

	return packet;
}

void HdlcEndInitPacket(char** packet, int* length, HDLC_REQUEST_FRAME_TYPE frameType, bool segmented)
{
	int frameLength = *length + 1;

	// Update Frame Format Field
	short frameFormat = 0xA000;
	short segmentMask = 0x0800;
	if (segmented)
		frameFormat = (short)((frameFormat | segmentMask) & 0xFFFF);

	short frameLengthMask = 0x07FF;
	frameLength = (frameLength & frameLengthMask) & 0xFFFF;
	frameFormat = (short)((frameFormat | frameLength) & 0xFFFF);

	char* ptr = *packet;
	ptr[1] = (char)((frameFormat >> 8) & 0xFF);
	ptr[2] = (char)(frameFormat & 0xFF);

	// Update HCS
	if (frameType == HDLC_REQUEST_FRAME_TYPE_SNRM || frameType == HDLC_REQUEST_FRAME_TYPE_I)
	{
		short hcs = HdlcCalculateCS(*packet, 1, 8, true);
		ptr[9] = (char)((hcs >> 8) & 0xFF);
		ptr[10] = (char)(hcs & 0xFF);
	}

	// Write FCS
	short fcs = HdlcCalculateCS(*packet, 1, *length - 1, true);
	WriteInt16(packet, length, fcs);

	// Append Ending FLAG
	WriteChar(packet, length, HDLC_STARTING_ENDING_FLAG);
}

bool HdlcCheckFrameType(unsigned char controlField, unsigned char identifier, unsigned char identifierMask)
{
	controlField &= identifierMask;
	if (controlField == identifier)
		return true;
	return false;
}

HDLC_RESPONSE_FRAME_TYPE HdlcGetReponseFrameType(unsigned char controlField)
{
	if (HdlcCheckFrameType(controlField, HDLC_I_FRAME_IDEN, HDLC_I_FRAME_IDEN_MASK))
		return HDLC_RESPONSE_FRAME_TYPE_I;

	if (HdlcCheckFrameType(controlField, HDLC_RR_FRAME_IDEN, HDLC_RR_FRAME_IDEN_MASK))
		return HDLC_RESPONSE_FRAME_TYPE_RR;

	if (HdlcCheckFrameType(controlField, HDLC_RNR_FRAME_IDEN, HDLC_RNR_FRAME_IDEN_MASK))
		return HDLC_RESPONSE_FRAME_TYPE_RNR;

	if (HdlcCheckFrameType(controlField, HDLC_UA_FRAME_IDEN, HDLC_UA_FRAME_IDEN_MASK))
		return HDLC_RESPONSE_FRAME_TYPE_UA;

	if (HdlcCheckFrameType(controlField, HDLC_DM_FRAME_IDEN, HDLC_DM_FRAME_IDEN_MASK))
		return HDLC_RESPONSE_FRAME_TYPE_DM;

	if (HdlcCheckFrameType(controlField, HDLC_FRMR_FRAME_IDEN, HDLC_FRMR_FRAME_IDEN_MASK))
		return HDLC_RESPONSE_FRAME_TYPE_FRMR;

	return HDLC_RESPONSE_FRAME_TYPE_INVALID;
}

DLMS_ERROR_CODE HdlcParseResponse(char* packet, int length, HDLC_RESPONSE_FRAME* response)
{
	char* packetStart = packet;
	int initLength = length;

	// Read Starting Flag
	char startingFlag;
	if (!ReadChar(&packet, &length, &startingFlag) || startingFlag != (char)HDLC_STARTING_ENDING_FLAG)
		return DLMS_ERROR_CODE_PARSE_ERROR;

	// Read Frame Format
	short frameFormat;
	if (!ReadInt16(&packet, &length, &frameFormat))
		return DLMS_ERROR_CODE_PARSE_ERROR;
	response->Segmented = ((frameFormat >> 11) & 0x01) == 0 ? false : true;
	int frameLength = frameFormat & 0x07FF;

	// Read Destination Address
	if (!ReadChar(&packet, &length, &response->ClientAddress))
		return DLMS_ERROR_CODE_PARSE_ERROR;

	// Read Source Address
	if (!ReadInt32(&packet, &length, &response->ServerAddress))
		return DLMS_ERROR_CODE_PARSE_ERROR;

	// Read Control Field
	unsigned char controlField;
	if (!ReadChar(&packet, &length, &controlField))
		return DLMS_ERROR_CODE_PARSE_ERROR;
	HDLC_RESPONSE_FRAME_TYPE type = HdlcGetReponseFrameType(controlField);
	if (type == HDLC_RESPONSE_FRAME_TYPE_INVALID)
		return DLMS_ERROR_CODE_INVALID_FRAME;
	response->Type = type;
	response->Final = ((controlField >> 4) & 0x01) == 0 ? false : true;
	response->R = (controlField >> 5) & 0x07;
	response->S = (controlField >> 1) & 0x07;

	// HCS Only On UA And IFrames
	if (type == HDLC_RESPONSE_FRAME_TYPE_UA || type == HDLC_RESPONSE_FRAME_TYPE_I)
	{
		// Read HCS
		short hcs;
		if (!ReadInt16(&packet, &length, &hcs))
			DLMS_ERROR_CODE_PARSE_ERROR;
		if (!HdlcCheckHCS(packetStart, initLength))
			return DLMS_ERROR_CODE_HCS_ERROR;

		// Read Data
		response->Data = packet;
		response->DataLength = length - 3;
		if (response->DataLength < 0)
			return DLMS_ERROR_CODE_PARSE_ERROR;
		packet += response->DataLength;
		length = 3;
	}

	// Read FCS
	short fcs;
	if (!ReadInt16(&packet, &length, &fcs))
		return DLMS_ERROR_CODE_PARSE_ERROR;
	if (!HdlcCheckFCS(packetStart, initLength))
		return DLMS_ERROR_CODE_FCS_ERROR;

	// Read Ending FLAG
	char endingFlag;
	if (!ReadChar(&packet, &length, &endingFlag) || endingFlag != (char)HDLC_STARTING_ENDING_FLAG)
		return DLMS_ERROR_CODE_PARSE_ERROR;

	return DLMS_ERROR_CODE_NONE;
}

bool HDlcGetFrameLength(char* packet, int length, int* frameLength)
{
	// Read Starting Flag
	char startingFlag;
	if (!ReadChar(&packet, &length, &startingFlag) || startingFlag != (char)HDLC_STARTING_ENDING_FLAG)
		return false;

	// Read Frame Format
	short frameFormat;
	if (!ReadInt16(&packet, &length, &frameFormat))
		return false;
	*frameLength = frameFormat & 0x07FF;
	return true;
}

MEDIA_ERROR HdlcReadPacket(READ_FROM_MEDIA_DELEGATE callback, char** packet, int* length)
{
	MEDIA_ERROR ret;
	char byte;
	bool hitStartingflag = false;

	*packet = malloc(0);
	*length = 0;
	for (; ; )
	{
		ret = ReadFromMedia(callback, &byte);
		if (ret != MEDIA_ERROR_NONE)
			return ret;

		if (byte == HDLC_STARTING_ENDING_FLAG)
		{
			if (!hitStartingflag)
			{
				// Found starting flag
				hitStartingflag = true;
				WriteChar(packet, length, byte);
			}
			else
			{
				WriteChar(packet, length, byte);

				// Actaris does not implemented 7e occurance in frame data solution. So we should read frame length to insure this flag is ending flag;
				int innerFrameLength = 0;
				if (HDlcGetFrameLength(*packet, *length, &innerFrameLength))
				{
					int totalLength = innerFrameLength + 2;
					if (*length >= totalLength)
					{
						// Riched end of frame
						return MEDIA_ERROR_NONE;
					}
				}
			}
		}
		else if (hitStartingflag)
			WriteChar(packet, length, byte);  // Append data
	}
}

MEDIA_ERROR HdlcSendSNRM(WRITE_ALL_TO_MEDIA_DELEGATE callback, HDLC_COMM_PARAMS comParams)
{
	// Propose Our Default Params. Note: SNRM Should Init S, R to Zero
	comParams.MaxInfoLenTransmit = 0x80;
	comParams.MaxInfoLenReceive = 0x80;
	comParams.WinSizeTransmit = 0x01;
	comParams.WinSizeReceive = 0x07;
	comParams.S = 0;
	comParams.R = 0;

	// Create SNRM Packet
	int length = 0;
	char* packet = HdlcBeginInitPacket(&length, comParams, HDLC_REQUEST_FRAME_TYPE_SNRM, true);
	WriteChar(&packet, &length, 0x81);							// format identifier
	WriteChar(&packet, &length, 0x80);							// group identifier
	WriteChar(&packet, &length, 0x12);							// group length
	HdlcWriteCharParam(&packet, &length, HDLC_MAX_TRANSMIT_IDEN, comParams.MaxInfoLenTransmit);
	HdlcWriteCharParam(&packet, &length, HDLC_MAX_RECEIVE_IDEN, comParams.MaxInfoLenReceive);
	HdlcWriteCharParam(&packet, &length, HDLC_TRANSMIT_WIN_SIZE_IDEN, comParams.WinSizeTransmit);
	HdlcWriteCharParam(&packet, &length, HDLC_RECEIVE_WIN_SIZE_IDEN, comParams.WinSizeReceive);
	HdlcEndInitPacket(&packet, &length, HDLC_REQUEST_FRAME_TYPE_SNRM, false);

	// Send SNRM Packet
	return WriteAllToMedia(callback, packet, length);
}

MEDIA_ERROR HdlcSendRR(WRITE_ALL_TO_MEDIA_DELEGATE callback, HDLC_COMM_PARAMS comParams)
{
	// Create RR Packet
	int length;
	char* packet = HdlcBeginInitPacket(&length, comParams, HDLC_REQUEST_FRAME_TYPE_RR, true);
	HdlcEndInitPacket(&packet, &length, HDLC_REQUEST_FRAME_TYPE_RR, false);
	//printf("SendRR: r:%d", comParams.R);
	// Send RR Packet
	return WriteAllToMedia(callback, packet, length);
}

MEDIA_ERROR HdlcSendDISC(WRITE_ALL_TO_MEDIA_DELEGATE callback, HDLC_COMM_PARAMS comParams)
{
	// Create DISC Packet
	int length;
	char* packet = HdlcBeginInitPacket(&length, comParams, HDLC_REQUEST_FRAME_TYPE_DISC, true);
	HdlcEndInitPacket(&packet, &length, HDLC_REQUEST_FRAME_TYPE_DISC, false);

	// Send DISC Packet
	return WriteAllToMedia(callback, packet, length);
}

DLMS_ERROR_CODE HdlcParseUAFrameData(HDLC_COMM_PARAMS* comParams, char* data, int length)
{
	// Read Format Identifier
	char formatIden;
	if (!ReadChar(&data, &length, &formatIden) || formatIden != (char)0x81)
		return DLMS_ERROR_CODE_PARSE_ERROR;

	// Read Group Identifier
	char groupIden;
	if (!ReadChar(&data, &length, &groupIden) || groupIden != (char)0x80)
		return DLMS_ERROR_CODE_PARSE_ERROR;

	unsigned char len;
	if (!ReadChar(&data, &length, &len))
		return DLMS_ERROR_CODE_PARSE_ERROR;

	// Read Params
	unsigned char value;
	DLMS_ERROR_CODE errorCode = HdlcReadCharParam(&data, &length, HDLC_MAX_TRANSMIT_IDEN, &value);
	if (errorCode != DLMS_ERROR_CODE_NONE)
		return errorCode;
	comParams->MaxInfoLenReceive = value;

	errorCode = HdlcReadCharParam(&data, &length, HDLC_MAX_RECEIVE_IDEN, &value);
	if (errorCode != DLMS_ERROR_CODE_NONE)
		return errorCode;
	comParams->MaxInfoLenTransmit = value;

	errorCode = HdlcReadCharParam(&data, &length, HDLC_TRANSMIT_WIN_SIZE_IDEN, &value);
	if (errorCode != DLMS_ERROR_CODE_NONE)
		return errorCode;
	comParams->WinSizeReceive = value;

	errorCode = HdlcReadCharParam(&data, &length, HDLC_RECEIVE_WIN_SIZE_IDEN, &value);
	if (errorCode != DLMS_ERROR_CODE_NONE)
		return errorCode;
	comParams->WinSizeTransmit = value;

	// Init S, R Into Zero
	comParams->S = 0;
	comParams->R = 0;

	// Win Sizes Are Not As Implemented By ISO So We Just Traced Meters And Found This
	/*comParams->WinSizeTransmit = 0x07;
	comParams->WinSizeReceive = 0x07;*/

	//printf("\nMax_T: %d, Max_R: %d, T:%d, R:%d\n", comParams->MaxInfoLenTransmit, comParams->MaxInfoLenReceive, comParams->WinSizeTransmit, comParams->WinSizeReceive);

	return errorCode;
}

MEDIA_ERROR HDlcReceiveUA(READ_FROM_MEDIA_DELEGATE callback, HDLC_COMM_PARAMS* comParams, DLMS_ERROR_CODE* errorCode)
{
	char* packet;
	int length;
	*errorCode = DLMS_ERROR_CODE_NONE;
	MEDIA_ERROR ret = HdlcReadPacket(callback, &packet, &length);
	if (ret == MEDIA_ERROR_NONE)
	{
		HDLC_RESPONSE_FRAME resFrame;
		*errorCode = HdlcParseResponse(packet, length, &resFrame);
		if (*errorCode == DLMS_ERROR_CODE_NONE)
		{
			if (resFrame.Type == HDLC_RESPONSE_FRAME_TYPE_UA)
				* errorCode = HdlcParseUAFrameData(comParams, resFrame.Data, resFrame.DataLength);
			else
				*errorCode = DLMS_ERROR_CODE_INVALID_FRAME;
		}
	}

	// Cleanup
	free(packet);
	return ret;
}

MEDIA_ERROR HdlcReceiveRR(READ_FROM_MEDIA_DELEGATE callback, HDLC_COMM_PARAMS comParams, DLMS_ERROR_CODE* errorCode)
{
	char* packet;
	int length;
	*errorCode = DLMS_ERROR_CODE_NONE;
	MEDIA_ERROR ret = HdlcReadPacket(callback, &packet, &length);
	if (ret == MEDIA_ERROR_NONE)
	{
		HDLC_RESPONSE_FRAME resFrame;
		*errorCode = HdlcParseResponse(packet, length, &resFrame);
		if (*errorCode == DLMS_ERROR_CODE_NONE)
		{
			//printf("ReceiveRR: mr:%d", resFrame.R);
			if (resFrame.Type != HDLC_RESPONSE_FRAME_TYPE_RR)
				* errorCode = DLMS_ERROR_CODE_INVALID_FRAME;
			else if (!HdlcCheckSR(comParams.S, 255, 255, resFrame.R))
				* errorCode = DLMS_ERROR_CODE_S_R_ERROR;
			else
				*errorCode = DLMS_ERROR_CODE_NONE;
		}
	}

	// Cleanup
	free(packet);
	return ret;
}

MEDIA_ERROR HdlcReceiveI(READ_FROM_MEDIA_DELEGATE callback, HDLC_COMM_PARAMS* comParams, bool* segmented, bool* isFinal, char** response, int* responseLength, DLMS_ERROR_CODE* errorCode)
{
	char* packet;
	int length;
	*errorCode = DLMS_ERROR_CODE_NONE;
	MEDIA_ERROR ret = HdlcReadPacket(callback, &packet, &length);
	if (ret == MEDIA_ERROR_NONE)
	{
		HDLC_RESPONSE_FRAME resFrame;
		*errorCode = HdlcParseResponse(packet, length, &resFrame);
		if (*errorCode == DLMS_ERROR_CODE_NONE)
		{
			if (resFrame.Type != HDLC_RESPONSE_FRAME_TYPE_I)
				* errorCode = DLMS_ERROR_CODE_INVALID_FRAME;
			else
			{
				if (HdlcCheckSR(comParams->S, comParams->R, resFrame.S, resFrame.R) || true)
				{
					comParams->R = HdlcUpdateR(comParams->R, comParams->WinSizeReceive);
					*segmented = resFrame.Segmented;
					*isFinal = resFrame.Final;
					if (resFrame.DataLength > 0)
						Write(response, responseLength, resFrame.Data, resFrame.DataLength);
					*errorCode = DLMS_ERROR_CODE_NONE;
					//printf("Meter: seg:%d final:%d ms:%d, mr:%d\n", *segmented, *isFinal, resFrame.S, resFrame.R);
				}
				else
					*errorCode = DLMS_ERROR_CODE_S_R_ERROR;
			}
		}
	}

	// Cleanup
	free(packet);
	return ret;
}

MEDIA_ERROR HdlcInitTransaction(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, HDLC_COMM_PARAMS* comPrams, DLMS_ERROR_CODE* errorCode)
{
	*errorCode = DLMS_ERROR_CODE_NONE;

	// Send SNRM
	MEDIA_ERROR ret = HdlcSendSNRM(writeCallback, *comPrams);
	if (ret != MEDIA_ERROR_NONE)
		return ret;

	// Receive UA (Set Communication Params)
	ret = HDlcReceiveUA(readCallback, comPrams, errorCode);
	if (ret != MEDIA_ERROR_NONE || *errorCode != DLMS_ERROR_CODE_NONE)
		return ret;

	return ret;
}

MEDIA_ERROR HDlcTerminateTransaction(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, HDLC_COMM_PARAMS comParams, DLMS_ERROR_CODE* errorCode)
{

	// Send DISC
	MEDIA_ERROR ret = HdlcSendDISC(writeCallback, comParams);
	if (ret != MEDIA_ERROR_NONE)
		return ret;

	// Receive UA Or DM (We do not check validity)
	HDlcReceiveUA(readCallback, &comParams, errorCode);

	*errorCode = DLMS_ERROR_CODE_NONE;
	return ret;
}

MEDIA_ERROR HdlcSendInfo(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, HDLC_COMM_PARAMS* comParams, char* info, int dataLength, char** response, int* responseLength, DLMS_ERROR_CODE* errorCode)
{
	*errorCode = DLMS_ERROR_CODE_NONE;
	*response = malloc(0);
	*responseLength = 0;

	// LLC
	LLCEncapsulate(&info, &dataLength);

	// Send Info
	bool poll;
	bool segmented;
	char* segmentPtr;
	int segmentLength;
	char* dataPtr = info;
	MEDIA_ERROR ret = MEDIA_ERROR_NONE;
	for (;;)
	{
		poll = false;
		HdlcGetSegment(*comParams, &dataPtr, &dataLength, &segmentPtr, &segmentLength, &segmented);
		//if (segmented == false || comParams->S == comParams->WinSizeTransmit)
		// We Use Win Transmit Size = 1 So Every Time Poll Is True
		poll = true;

		//printf("We: seg:%d poll:%d s:%d, r:%d\n", segmented, poll, comParams->S, comParams->R);

		// Create I Frame Packet
		int packetLength;
		char* packet = HdlcBeginInitPacket(&packetLength, *comParams, HDLC_REQUEST_FRAME_TYPE_I, poll);
		Write(&packet, &packetLength, segmentPtr, segmentLength);
		HdlcEndInitPacket(&packet, &packetLength, HDLC_REQUEST_FRAME_TYPE_I, segmented);

		// Send Packet
		ret = WriteAllToMedia(writeCallback, packet, packetLength);
		if (ret != MEDIA_ERROR_NONE)
		{
			// Cleanup
			free(info);
			return ret;
		}
		comParams->S = HdlcUpdateS(comParams->S, comParams->WinSizeTransmit);

		if (segmented == false)
		{
			// It was the final segment (Cleanup) and (Exit) to receive response
			free(info);
			break;
		}

		if (poll == false)
		{
			// Don't Wait For Meter Response (Continue Sending Packets)
			continue;
		}

		// Wait For RR Frame
		ret = HdlcReceiveRR(readCallback, *comParams, errorCode);
		if (ret != MEDIA_ERROR_NONE || *errorCode != DLMS_ERROR_CODE_NONE)
		{
			// Cleanup
			free(info);
			return ret;
		}
	}

	// Receive Response Frames
	segmented == false;
	bool isFinal = false;
	for (;;)
	{
		// Receive I Frame
		ret = HdlcReceiveI(readCallback, comParams, &segmented, &isFinal, response, responseLength, errorCode);
		if (ret != MEDIA_ERROR_NONE || *errorCode != DLMS_ERROR_CODE_NONE)
			return ret;

		if (segmented == false)
		{
			// We Are Done
			*errorCode = LLCDencapsulate(response, responseLength);
			return MEDIA_ERROR_NONE;
		}

		if (isFinal == false)
		{
			// Receive Next Frame
			continue;
		}

		// Send RR to receive remaining segments
		ret = HdlcSendRR(writeCallback, *comParams);
		if (ret != MEDIA_ERROR_NONE)
			return ret;
	}
}
