// Authored by: MohammadAmin Mohammadi (https://github.com/mamohammadi)

#pragma once
// Disable struct padding
#pragma pack(push,1)

#include "IMRCore.h"
#include "Media.h"

#define HDLC_STARTING_ENDING_FLAG 0x7e
#define HDLC_MAX_TRANSMIT_IDEN 0x05
#define HDLC_MAX_RECEIVE_IDEN 0x06
#define HDLC_TRANSMIT_WIN_SIZE_IDEN 0x07
#define HDLC_RECEIVE_WIN_SIZE_IDEN 0x08
#define HDLC_I_FRAME_IDEN 0x00
#define HDLC_I_FRAME_IDEN_MASK 0x01
#define HDLC_RR_FRAME_IDEN 0x01
#define HDLC_RR_FRAME_IDEN_MASK 0x0F
#define HDLC_RNR_FRAME_IDEN 0x05
#define HDLC_RNR_FRAME_IDEN_MASK 0X0F
#define HDLC_SNRM_FRAME_IDEN 0X83
#define HDLC_SNRM_FRAME_IDEN_MASK 0XEF
#define HDLC_DISC_FRAME_IDEN 0X43
#define HDLC_DISC_FRAME_IDEN_MASK 0XEF
#define HDLC_UA_FRAME_IDEN 0X63
#define HDLC_UA_FRAME_IDEN_MASK 0XEF
#define HDLC_DM_FRAME_IDEN 0X0F
#define HDLC_DM_FRAME_IDEN_MASK 0XEF
#define HDLC_FRMR_FRAME_IDEN 0X87
#define HDLC_FRMR_FRAME_IDEN_MASK 0XEF
#define HDLC_UI_FRAME_IDEN 0X03
#define HDLC_UI_FRAME_IDEN_MASK 0XEF
#define HDLC_MODULUS 8

static unsigned short HdlcFCSTable[] =
{
	0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
	0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
	0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
	0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
	0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
	0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
	0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
	0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
	0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
	0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
	0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
	0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
	0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
	0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
	0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
	0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
	0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
	0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
	0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
	0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
	0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
	0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
	0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
	0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
	0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
	0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
	0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
	0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
	0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
	0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
	0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
	0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};

#define HDLC_PPP_INIT_FCS16 0xFFFF      // Initial FCS Value
#define DHLC_PPP_GOOD_FCS16 0xF0B8      // Good fFinal FCS Value

typedef struct HDLC_COMM_PARAMS
{
	unsigned int ServerID;
	unsigned char ClientID;
	unsigned char MaxInfoLenTransmit;
	unsigned char MaxInfoLenReceive;
	unsigned char WinSizeTransmit; // Max S
	unsigned char WinSizeReceive; // Max R
	unsigned char S;
	unsigned char R;
} HDLC_COMM_PARAMS;

typedef enum DLMS_ERROR_CODE
{
	/* Data Access Result */
	DLMS_ERROR_CODE_NONE = 0,
	DLMS_ERROR_CODE_HARDWARE_FAULT = 1,
	DLMS_ERROR_CODE_TEMPORARY_FAILURE = 2,
	DLMS_ERROR_CODE_READ_WRITE_DENIED = 3,
	DLMS_ERROR_CODE_OBJECT_UNDEFINED = 4,
	DLMS_ERROR_CODE_OBJECT_CLASS_INCONSISTENT = 9,
	DLMS_ERROR_CODE_OBJECT_UNAVAILABLE = 11,
	DLMS_ERROR_CODE_TYPE_UNMATCHED = 12,
	DLMS_ERROR_CODE_SCOPE_OF_ACCESS_VIOLATED = 13,
	DLMS_ERROR_CODE_DATA_BLOCK_UNAVAILABLE = 14,
	DLMS_ERROR_CODE_LONG_GET_ABORTED = 15,
	DLMS_ERROR_CODE_NO_LONG_GET_IN_PROGRESS = 16,
	DLMS_ERROR_CODE_LONG_SET_ABORTED = 17,
	DLMS_ERROR_CODE_NO_LONG_SET_IN_PROGRESS = 18,
	DLMS_ERROR_CODE_DATA_BLOCK_NUMBER_INVALID = 19,
	DLMS_ERROR_CODE_OTHER_REASON = 250,

	DLMS_ERROR_CODE_PARSE_ERROR = 100,
	DLMS_ERROR_CODE_INVALID_FRAME = 101,
	DLMS_ERROR_CODE_HCS_ERROR = 102,
	DLMS_ERROR_CODE_FCS_ERROR = 103,
	DLMS_ERROR_CODE_AUTHENTICATION_FAILED = 104,
	DLMS_ERROR_CODE_NOT_IMPLEMENTED = 105,
	DLMS_ERROR_CODE_TYPE_ERROR = 106,
	DLMS_ERROR_CODE_S_R_ERROR = 107
} DLMS_ERROR_CODE;

typedef enum HDLC_REQUEST_FRAME_TYPE
{
	HDLC_REQUEST_FRAME_TYPE_INVALID,
	HDLC_REQUEST_FRAME_TYPE_I,
	HDLC_REQUEST_FRAME_TYPE_RR,
	HDLC_REQUEST_FRAME_TYPE_RNR,
	HDLC_REQUEST_FRAME_TYPE_SNRM,
	HDLC_REQUEST_FRAME_TYPE_DISC,
	HDLC_REQUEST_FRAME_TYPE_UI
} HDLC_REQUEST_FRAME_TYPE;

typedef enum HDLC_RESPONSE_FRAME_TYPE
{
	HDLC_RESPONSE_FRAME_TYPE_INVALID,
	HDLC_RESPONSE_FRAME_TYPE_I,
	HDLC_RESPONSE_FRAME_TYPE_RR,
	HDLC_RESPONSE_FRAME_TYPE_RNR,
	HDLC_RESPONSE_FRAME_TYPE_UA,
	HDLC_RESPONSE_FRAME_TYPE_DM,
	HDLC_RESPONSE_FRAME_TYPE_FRMR
} HDLC_RESPONSE_FRAME_TYPE;

typedef struct HDLC_RESPONSE_FRAME
{
	bool Segmented;
	bool Final;
	int Length;
	int ServerAddress;
	int ClientAddress;
	HDLC_RESPONSE_FRAME_TYPE Type;
	char R;
	char S;
	char* Data;
	int DataLength;
} HDLC_RESPONSE_FRAME;

unsigned char HdlcUpdateS(unsigned char s, unsigned char winSizeTransmit);

unsigned char HdlcUpdateR(unsigned char r, unsigned char winSizeReceive);

bool HdlcCheckSR(unsigned char s, unsigned char r, unsigned char meterS, unsigned char meterR);

unsigned short HdlcCalculateCS(char* buffer, int startIndex, int count, bool applyTransformation);

bool HdlcCheckHCS(char* buffer, int length);

bool HdlcCheckFCS(char* buffer, int length);

void HdlcWriteParam(char** packet, int* packetLenght, char paramIden, char paramLength, char* paramValue);

void HdlcWriteCharParam(char** packet, int* length, char paramIden, char paramValue);

DLMS_ERROR_CODE HdlcReadParam(char** buffer, int* remLength, char paramIden, char* paramLength, char** paramValuePtr);

DLMS_ERROR_CODE HdlcTryParseChar(char* buffer, int length, char* value);

DLMS_ERROR_CODE HdlcTryParseInt16(char* buffer, int length, short* value);

DLMS_ERROR_CODE HdlcTryParseInt32(char* buffer, int length, int* value);

DLMS_ERROR_CODE HdlcTryParseInt64(char* buffer, int length, long* value);

DLMS_ERROR_CODE HdlcReadCharParam(char** buffer, int* remLength, char paramIden, char* value);

DLMS_ERROR_CODE HdlcReadInt16Param(char** buffer, int* remLength, char paramIden, short* value);

DLMS_ERROR_CODE HdlcReadInt32Param(char** buffer, int* remLength, char paramIden, int* value);

DLMS_ERROR_CODE HdlcReadInt64Param(char** buffer, int* remLength, char paramIden, long* value);

void LLCEncapsulate(char** data, int* length);

DLMS_ERROR_CODE LLCDencapsulate(char** data, int* length);

bool HdlcGetSegment(HDLC_COMM_PARAMS comParams, char** data, int* dataLength, char** segmentPtr, int* segmentLength, bool* segmented);

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
unsigned char HdlcCreateControlField(HDLC_COMM_PARAMS comParams, HDLC_REQUEST_FRAME_TYPE frameType, bool poll);

char* HdlcBeginInitPacket(int* length, HDLC_COMM_PARAMS comParams, HDLC_REQUEST_FRAME_TYPE frameType, bool poll);

void HdlcEndInitPacket(char** packet, int* length, HDLC_REQUEST_FRAME_TYPE frameType, bool segmented);

bool HdlcCheckFrameType(unsigned char controlField, unsigned char identifier, unsigned char identifierMask);

HDLC_RESPONSE_FRAME_TYPE HdlcGetReponseFrameType(unsigned char controlField);

DLMS_ERROR_CODE HdlcParseResponse(char* packet, int length, HDLC_RESPONSE_FRAME* response);

bool HDlcGetFrameLength(char* packet, int length, int* frameLength);

MEDIA_ERROR HdlcReadPacket(READ_FROM_MEDIA_DELEGATE callback, char** packet, int* length);

MEDIA_ERROR HdlcSendSNRM(WRITE_ALL_TO_MEDIA_DELEGATE callback, HDLC_COMM_PARAMS comParams);

MEDIA_ERROR HdlcSendRR(WRITE_ALL_TO_MEDIA_DELEGATE callback, HDLC_COMM_PARAMS comParams);

MEDIA_ERROR HdlcSendDISC(WRITE_ALL_TO_MEDIA_DELEGATE callback, HDLC_COMM_PARAMS comParams);

DLMS_ERROR_CODE HdlcParseUAFrameData(HDLC_COMM_PARAMS* comParams, char* data, int length);

MEDIA_ERROR HDlcReceiveUA(READ_FROM_MEDIA_DELEGATE callback, HDLC_COMM_PARAMS* comParams, DLMS_ERROR_CODE* errorCode);

MEDIA_ERROR HdlcReceiveRR(READ_FROM_MEDIA_DELEGATE callback, HDLC_COMM_PARAMS comParams, DLMS_ERROR_CODE* errorCode);

MEDIA_ERROR HdlcReceiveI(READ_FROM_MEDIA_DELEGATE callback, HDLC_COMM_PARAMS* comParams, bool* segmented, bool* isFinal, char** response, int* responseLength, DLMS_ERROR_CODE* errorCode);

MEDIA_ERROR HdlcInitTransaction(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, HDLC_COMM_PARAMS* comPrams, DLMS_ERROR_CODE* errorCode);

MEDIA_ERROR HDlcTerminateTransaction(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, HDLC_COMM_PARAMS comParams, DLMS_ERROR_CODE* errorCode);

MEDIA_ERROR HdlcSendInfo(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, HDLC_COMM_PARAMS* comParams, char* info, int dataLength, char** response, int* responseLength, DLMS_ERROR_CODE* errorCode);
