#pragma once
// Disable struct padding
#pragma pack(push,1)

#include "IMRCore.h"
#include "Hdlc.h"

#define DLMS_GET_REQ_IDEN 0xC0
#define DLMS_GET_RES_IDEN 0xC4
#define DLMS_LOG_ADDR_LEN 6

typedef enum DLMS_OBJECT_TYPE
{
	DLMS_OBJECT_TYPE_ALL = -1,
	DLMS_OBJECT_TYPE_NONE = 0,
	DLMS_OBJECT_TYPE_DATA = 1,
	DLMS_OBJECT_TYPE_REGISTER = 3,
	DLMS_OBJECT_TYPE_EXTENDED_REGISTER = 4,
	DLMS_OBJECT_TYPE_DEMAND_REGISTER = 5,
	DLMS_OBJECT_TYPE_REGISTER_ACTIVATION = 6,
	DLMS_OBJECT_TYPE_PROFILE_GENERIC = 7,
	DLMS_OBJECT_TYPE_CLOCK = 8,
	DLMS_OBJECT_TYPE_SCRIPT_TABLE = 9,
	DLMS_OBJECT_TYPE_SCHEDULE = 10,
	DLMS_OBJECT_TYPE_SPECIAL_DAYS_TABLE = 11,
	DLMS_OBJECT_TYPE_ASSOCIATION_SHORT_NAME = 12,
	DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME = 15,
	DLMS_OBJECT_TYPE_SAP_ASSIGNMENT = 17,
	DLMS_OBJECT_TYPE_IMAGE_TRANSFER = 18,
	DLMS_OBJECT_TYPE_IEC_LOCAL_PORT_SETUP = 19,
	DLMS_OBJECT_TYPE_ACTIVITY_CALENDAR = 20,
	DLMS_OBJECT_TYPE_REGISTER_MONITOR = 21,
	DLMS_OBJECT_TYPE_ACTION_SCHEDULE = 22,
	DLMS_OBJECT_TYPE_IEC_HDLC_SETUP = 23,
	DLMS_OBJECT_TYPE_IEC_TWISTED_PAIR_SETUP = 24,
	DLMS_OBJECT_TYPE_MBUS_SLAVE_PORT_SETUP = 25,
	DLMS_OBJECT_TYPE_UTILITY_TABLES = 26,
	DLMS_OBJECT_TYPE_MODEM_CONFIGURATION = 27,
	DLMS_OBJECT_TYPE_AUTO_ANSWER = 28,
	DLMS_OBJECT_TYPE_AUTO_CONNECT = 29,
	DLMS_OBJECT_TYPE_TCP_UDP_SETUP = 41,
	DLMS_OBJECT_TYPE_IP4_SETUP = 42,
	DLMS_OBJECT_TYPE_MAC_ADDRESS_SETUP = 43,
	DLMS_OBJECT_TYPE_PPP_SETUP = 44,
	DLMS_OBJECT_TYPE_GPRS_SETUP = 45,
	DLMS_OBJECT_TYPE_SMTP_SETUP = 46,
	DLMS_OBJECT_TYPE_REGISTER_TABLE = 61,
	DLMS_OBJECT_TYPE_STATUS_MAPPING = 63,
	DLMS_OBJECT_TYPE_DLMS_SECURITY_SETUP = 64,
	DLMS_OBJECT_TYPE_DISCONNECT_CONTROL = 70,
	DLMS_OBJECT_TYPE_LIMITER = 71,
	DLMS_OBJECT_TYPE_MBUS_CLIENT = 72,
	DLMS_OBJECT_TYPE_PUSH_SETUP = 40,
	DLMS_OBJECT_TYPE_MESSAGE_HANDLER = 60,
	DLMS_OBJECT_TYPE_PARAMETER_MONITOR = 65,
	DLMS_OBJECT_TYPE_WIRELESS_MODEQ_CHANNEL = 73,
	DLMS_OBJECT_TYPE_MBUS_MASTER_PORT_SETUP = 74,

	/**
	 * configure a zigbee pro device with information necessary to create or
	 * join the network.
	 */
	DLMS_OBJECT_TYPE_ZIG_BEE_SAS_STARTUP = 101,

	/**
	 * configure the behavior of a zigbee pro device on joining or loss of
	 * connection to the network.
	 */
	DLMS_OBJECT_TYPE_ZIG_BEE_SAS_JOIN = 102,
	/**
	 * configure the fragmentation feature of zigbee pro transport layer.
	 */
	DLMS_OBJECT_TYPE_ZIG_BEE_SAS_APS_FRAGMENTATION = 103,
	DLMS_OBJECT_TYPE_ZIG_BEE_NETWORK_CONTROL = 104
} DLMS_OBJECT_TYPE;

typedef enum DLMS_CLASS_ID
{
	DLMS_CLASS_ID_DATA = 0x01,
	DLMS_CLASS_ID_REGISTER = 0x03,
	DLMS_CLASS_ID_PROFILE_GENERIC = 0x07
} DLMS_CLASS_ID;

typedef enum DLMS_GET_REQ
{
	DLMS_GET_REQ_NORMAL = 0x01,
	DLMS_GET_REQ_NEXT = 0x02,
	DLMS_GET_REQ_WITH_LIST = 0x03
} DLMS_GET_REQ;

typedef enum DLMS_GET_RES
{
	DLMS_GET_RES_NORMAL = 0x01,
	DLMS_GET_RES_WITH_DATA_BLOCK = 0x02,
	DLMS_GET_RES_WITH_LIST = 0x03,
	DLMS_GET_RES_INVALID = 0xFF
} DLMS_GET_RES;

typedef enum DLMS_GET_DATA_RESULT
{
	DLMS_GET_DATA_RESULT_DATA = 0x00,
	DLMS_GET_DATA_RESULT_DATA_ACCESS = 0x01
};

typedef enum DLMS_TYPE
{
	DLMS_TYPE_NULL = 0x00,					// Null data
	DLMS_TYPE_BOOLEAN = 0x03,				// Boolean
	DLMS_TYPE_BIT_STRING = 0x04,			// An ordered sequence of boolean values
	DLMS_TYPE_DOUBLE_LONG = 0x05,			// Integer32
	DLMS_TYPE_DOUBLE_LONG_UNSIGNED = 0x06,  // Unsigned32
	DLMS_TYPE_OCTET_STRING = 0x09,			// An ordered sequence of octets (8 bit bytes)
	DLMS_TYPE_VISIBLE_STRING = 0x0a,		// An ordered sequence of asci characters
	DLMS_TYPE_UTF8_STRING = 0x0c,			// An ordered sequence of characters encoded utf-8
	DLMS_TYPE_BCD = 0x0d,					// Binary coded decimal
	DLMS_TYPE_INTEGER = 0x0f,				// Integer8
	DLMS_TYPE_LONG = 0x10,					// Integer16
	DLMS_TYPE_UNSIGNED = 0x11,				// Unsinged8
	DLMS_TYPE_LONG_UNSIGEND = 0x12,			// Unsigned16
	DLMS_TYPE_LONG64 = 0x14,				// Integer64
	DLMS_TYPE_LONG64_UNSIGNED = 0x15,		// Unsigned64
	DLMS_TYPE_ENUM = 0x16,					// Enum
	DLMS_TYPE_FLOAT32 = 0x17,				// Octet string size = 4
	DLMS_TYPE_FLOAT64 = 0x18,				// Octet string size = 8
	DLMS_TYPE_DATE_TIME = 0x19,				// Octet string size = 12
	DLMS_TYPE_DATE = 0x1a,					// Octet string size = 5
	DLMS_TYPE_TIME = 0x1b,					// Octet string size = 4
	DLMS_TYPE_ARRAY = 0x01,					// Array
	DLMS_TYPE_STRUCTURE = 0x02,				// Structure
	DLMS_TYPE_COMPACT_ARRAY = 0x13,			// Compact array
} DLMS_TYPE;

typedef enum DLMS_ACCESS_SELECTOR
{
	DLMS_ACCESS_SELECTOR_RANGE_DESCRIPTOR = 0x01,
	DLMS_ACCESS_SELECTOR_ENTITY_DESCRIPTOR = 0x02
} DLMS_ACCESS_SELECTOR;

typedef struct DLMS_DATE_TIME
{
	bool IsNull;
	short Year;
	char Month;
	char Day;
	char Hour;
	char Minute;
	char Second;
} DLMS_DATE_TIME;

typedef struct DLMS_REGISTER
{
	char Address[DLMS_LOG_ADDR_LEN];
	unsigned char Type;
	unsigned char ErrorCode;
	signed char Scaler;
	unsigned char UnitCode;
	bool IsNull;
	char Value[MAX_VALUE_LENGTH];
} DLMS_REGISTER;

typedef enum DLMS_REGISTER_ATTRS
{
	DLMS_REGISTER_ATTRS_LOGICAL_NAME = 0x01,
	DLMS_REGISTER_ATTRS_VALUE = 0x02,
	DLMS_REGISTER_ATTRS_SCALER_UNIT = 0x03
};

typedef struct DLMS_FILE_INFO
{
	char Address[DLMS_LOG_ADDR_LEN];
} DLMS_FILE_INFO;

typedef enum DLMS_FILE_ATTRS
{
	DLMS_FILE_ATTRS_LOGICL_NAME = 0x01,
	DLMS_FILE_ATTRS_BUFFER = 0x02,
	DLMS_FILE_ATTRS_CAPTURE_OBJECTS = 0x03,
	DLMS_FILE_ATTRS_CAPTURE_PERIOD = 0x04
} DLMS_FILE_ATTRS;

typedef struct DLMS_FILE_CHANNEL_INFO
{
	unsigned short ClassID;
	char Address[DLMS_LOG_ADDR_LEN];
	unsigned char AttributeIndex;
	unsigned short DataIndex;
	signed char Scaler;
	unsigned char UnitCode;
} DLMS_FILE_CHANNEL_INFO;

typedef struct DLMS_FILE_FIELD
{
	bool IsNull;
	unsigned char Type;
	char Value[MAX_VALUE_LENGTH];
} DLMS_FILE_FIELD;

DLMS_ERROR_CODE DlmsRead(char** buffer, int* remLen, char** value, int readLen);

DLMS_ERROR_CODE DlmsReadChar(char** buffer, int* remLen, char* value);

DLMS_ERROR_CODE DlmsReadInt16(char** buffer, int* remLen, short* value);

DLMS_ERROR_CODE DlmsReadInt32(char** buffer, int* remLen, int* value);

DLMS_ERROR_CODE DlmsReadInt64(char** buffer, int* remLen, long* value);

DLMS_ERROR_CODE DlmsReadFloat(char** buffer, int* remLen, float* value);

DLMS_ERROR_CODE DlmsReadDouble(char** buffer, int* remLen, double* value);

DLMS_ERROR_CODE DlmsReadString(char** buffer, int* remLen, char* value, int maxValueLen, int* readedLength);

DLMS_ERROR_CODE DlmsReadStruct(char** buffer, int* remLen, char** valuePtr);

void DlmsWriteDataTime(char** buffer, int* remLen, DLMS_DATE_TIME dateTime);

DATA_TYPE DlmsGetDataType(DLMS_TYPE type);

DLMS_ERROR_CODE DlmsReadValue(char** payloadPtr, int* length, DLMS_TYPE type, char* valuePtr);

DLMS_ERROR_CODE DlmsParseRegisterScalerUnit(char* payload, int length, DLMS_REGISTER* reg);

DLMS_ERROR_CODE DlmsReadRegisterValue(char* payload, int length, DLMS_REGISTER* reg);

char* DlmsCreateGetRequestNormal(int* length, DLMS_CLASS_ID classID, char* insID, unsigned char attrId, char* accSelection, int accSelectionLen);

char* DlmsCreateGetRequestNext(int* length, int blockNumber);

DLMS_ERROR_CODE DlmsParseNormal(char** normalResApdu, int* normalResApduLen, char** dataPtr, int* dataLength);

DLMS_ERROR_CODE DlmsParseWithDataBlock(char** apdu, int* length, char** dataPtr, int* dataLength, bool* lastBlock, int* nextBlockNumber);

DLMS_ERROR_CODE DlmsParseResponse(char** rawDlmsApdu, int* rawDlmsApduLen, char** dataPtr, int* dataLen, bool* lastBlock, int* nextBlockNumber);

MEDIA_ERROR DlmsSendAARQ(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, HDLC_COMM_PARAMS* comParams, char* password, int passwordLength, char** response, int* length, DLMS_ERROR_CODE* errorCode);

MEDIA_ERROR DlmsReadObject(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, HDLC_COMM_PARAMS* comParams, char* address, DLMS_CLASS_ID classID, unsigned char attrIndex, char* accessSelection, int accessSelectionLength, char** data, int* dataLength, DLMS_ERROR_CODE* errorCode);

MEDIA_ERROR DlmsReadRegisterScalerUnit(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, HDLC_COMM_PARAMS* comParams, DLMS_REGISTER* reg, DLMS_ERROR_CODE* errorCode);

DLMS_ERROR_CODE DlmsReadProfileLengthField(char** data, int* length, int* lengthValue);

DLMS_ERROR_CODE DlmsParseProfile(char** data, int* length, DLMS_FILE_CHANNEL_INFO* channels, char channelsCount, DLMS_FILE_FIELD** fields, int* rowsCount);

EXPORT unsigned int DlmsGetServerID(unsigned int physicalAddress, unsigned int logicalAddress);

EXPORT MEDIA_ERROR DlmsLogin(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, HDLC_COMM_PARAMS* comParams, char* password, int passwordLength, char* errCode);

EXPORT MEDIA_ERROR DlmsLogout(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, HDLC_COMM_PARAMS comParams, char* password, int passwordLength, char* errCode);

EXPORT MEDIA_ERROR DlmsReadRegister(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, HDLC_COMM_PARAMS* comParams, DLMS_REGISTER* reg, char* errCode);

EXPORT MEDIA_ERROR DlmsReadProfileChannels(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, HDLC_COMM_PARAMS* comParams, DLMS_FILE_INFO fileInfo, DLMS_FILE_CHANNEL_INFO** channels, unsigned char* channelsCount, char* errCode);

EXPORT MEDIA_ERROR DlmsReadProfile(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, HDLC_COMM_PARAMS* comParams, DLMS_DATE_TIME* from, DLMS_DATE_TIME* to, DLMS_FILE_INFO fileInfo, DLMS_FILE_CHANNEL_INFO* channels, unsigned char channelsCount, DLMS_FILE_FIELD** fields, int* rowsCount, char* errCode);

