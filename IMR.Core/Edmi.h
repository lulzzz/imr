// Authored by: MohammadAmin Mohammadi (https://github.com/mamohammadi)

#pragma once
// Disable struct padding
#pragma pack(push,1)

#include "IMRCore.h"
#include "Media.h"

#define EDMI_STX_IDEN 0x02
#define EDMI_ETX_IDEN 0x03
#define EDMI_DLE_IDEN 0x10
#define EDMI_XON_IDEN 0x11
#define EDMI_XOFF_IDEN 0x13
#define EDMI_IDEN_CORRECTOR 0x40
#define EDMI_E_FRAME_IDEN 0x45
#define EDMI_MULTI_ERR_IDEN  0x0000FFF1

#define EDMI_CLIENT_SERIAL_LENGTH 0x06
static char EDMI_CLIENT_SERIAL[EDMI_CLIENT_SERIAL_LENGTH] = { 0x01, 0x2B, 0x16, 0x68, 0xFF, 0xFF };

typedef enum EDMI_TYPE
{
	EDMI_TYPE_STRING = 'A',							// Null terminated ASCII string (EXT: EDMI_TYPE_STRING)
	EDMI_TYPE_BOOLEAN = 'B',						// This is a byte that can be 0 meaning false, or none zero meaning true (EXT: EDMI_TYPE_BOOLEAN)
	EDMI_TYPE_BYTE = 'C',							// 8-bit unsigned integer (EXT: EDMI_TYPE_BYTE)
	EDMI_TYPE_DOUBLE = 'D',							// Double precision IEEE 64-bit floating point number (EXT: EDMI_TYPE_DOUBLE)
	EDMI_TYPE_EFA_STRING = 'E',						// External 16 character + null EFA string. Internal 16 - bit unsigned integer (EXT: EDMI_TYPE_STRING, INT: EDMI_TYPE_HEX_SHORT)
	EDMI_TYPE_FLOAT = 'F',							// Single precision IEEE 32-bit floating point number (EXT: EDMI_TYPE_FLOAT)
	EDMI_TYPE_STRING_LONG = 'G',					// External string representation of an integer. Internal 32 - bit signed long integer (EXT: EDMI_TYPE_STRING, INT: EDMI_TYPE_LONG)
	EDMI_TYPE_HEX_SHORT = 'H',						// 16-bit Hex unsigned short (EXT: EDMI_TYPE_HEX_SHORT)
	EDMI_TYPE_SHORT = 'I',							// 16-bit signed short integer (EXT: EDMI_TYPE_SHORT)
	EDMI_TYPE_VARIABLE_SPACE = 'J',					// If the written size is less than the maximum size it is padded with zeros. When read always returns the maximum size (EXT: EDMI_TYPE_VARIABLE_SPACE)
	EDMI_TYPE_LONG = 'L',							// 32-bit signed long integer (EXT: EDMI_TYPE_LONG)
	EDMI_TYPE_NONE = 'N',							// Used by the meter to indicate an invalid type (EXT: EDMI_TYPE_NONE)
	EDMI_TYPE_FLOAT_ENERGY = 'O',					// Internal 32-bit unsigned integer micropulses. External float(with transformer ratio applied) (EXT: EDMI_TYPE_FLOAT, INT: EDMI_TYPE_LONG)
	EDMI_TYPE_POWER_FACTOR = 'P',					// Internal signed short from –32767 to 32767. External float from –1.0 to + 1.0 (EXT: EDMI_TYPE_FLOAT, INT: EDMI_TYPE_SHORT)
	EDMI_TYPE_TIME = 'Q',							// Internal unsigned 32-bit seconds since midnight. External time as 3 bytes: {Hour} {Minute} {Second} Displays on an LCD formatted as HH:MM:SS (EXT: EDMI_TYPE_TIME, INT: EDMI_TYPE_HEX_LONG)
	EDMI_TYPE_DATE = 'R',							// Internal unsigned 32-bit seconds since 1/1/96. External date as 3 bytes: {Date} {Month} {Year} Displays on an LCD formatted as DD/MM/YY (EXT: EDMI_TYPE_DATE, INT:EDMI_TYPE_HEX_LONG)
	EDMI_TYPE_SPECIAL = 'S',						// Special (not a regular type) (EXT: EDMI_TYPE_SPECIAL)
	EDMI_TYPE_TIME_DATE = 'T',						// Internal unsigned 32-bit seconds since 1/1/96. External time / date as 6 bytes: {Date} {Month} {Year} {Hour} {Minute} {Second} Displays on an LCD formatted as HH MM:SS DD/MM/YY (EXT: EDMI_TYPE_TIME_DATE, INT:EDMI_TYPE_HEX_LONG)
	EDMI_TYPE_DOUBLE_ENERGY = 'U',					// Internal 64-bit integer micropulses. External double(with transformer ratio applied) (EXT: EDMI_TYPE_DOUBLE, INT: EDMI_TYPE_LONG_LONG)
	EDMI_TYPE_LONG_LONG = 'V',						// 64-bit signed integer generally expressed in hexadecimal for ease of display(16 digits) (EXT: EDMI_TYPE_LONG_LONG)
	EDMI_TYPE_WAVEFORM = 'W',						// Captured waveform, with status data. Format of the F402 register (EXT: EDMI_TYPE_WAVEFORM)
	EDMI_TYPE_HEX_LONG = 'X',						// Hex unsigned long (EXT: EDMI_TYPE_HEX_LONG)
	EDMI_TYPE_REGISTER_NUMBER_HEX_LONG = 'Z'		// Register number, type is generally equivalent to EDMI_TYPE_HEX_LONG (EXT: EDMI_TYPE_REGISTER_NUMBER_HEX_LONG)
} EDMI_TYPE;

typedef enum EDMI_UNIT_CODE
{
	EDMI_UNIT_CODE_AMPS = 'A',
	EDMI_UNIT_CODE_LITERS_PER_HOUR = 'B',
	EDMI_UNIT_CODE_ANGLE_IN_DEGREES = 'D',
	EDMI_UNIT_CODE_CUBIC_METERS_PER_HOUR = 'G',
	EDMI_UNIT_CODE_HERTZ = 'H',
	EDMI_UNIT_CODE_JOULES_PER_HOUR = 'I',
	EDMI_UNIT_CODE_JOULES = 'J',
	EDMI_UNIT_CODE_LITERS = 'L',
	EDMI_UNIT_CODE_MINUTES = 'M',
	EDMI_UNIT_CODE_NO_UNIT = 'N',
	EDMI_UNIT_CODE_CUBIC_METERS = 'O',
	EDMI_UNIT_CODE_PERCENT = 'P',
	EDMI_UNIT_CODE_POWER_FACTOR = 'Q',
	EDMI_UNIT_CODE_VARS = 'R',
	EDMI_UNIT_CODE_VA = 'S',
	EDMI_UNIT_CODE_SECONDS = 'T',
	EDMI_UNIT_CODE_UNKNOWN = 'U',
	EDMI_UNIT_CODE_VOLTES = 'V',
	EDMI_UNIT_CODE_WATTS = 'W',
	EDMI_UNIT_CODE_WATT_HOURS = 'X',
	EDMI_UNIT_CODE_VARH = 'Y',
	EDMI_UNIT_CODE_VAH = 'Z'
} EDMI_UNIT_CODE;

typedef enum EDMI_ERROR_CODE
{
	EDMI_ERROR_CODE_NONE = 0,
	EDMI_ERROR_CODE_CAN_NOT_WRITE = 1,
	EDMI_ERROR_CODE_UNIMPLEMENTED_OPERATION = 2,
	EDMI_ERROR_CODE_REGISTER_NOT_FOUND = 3,
	EDMI_ERROR_CODE_ACCESS_DENIED = 4,
	EDMI_ERROR_CODE_REQUEST_WRONG_LENGTH = 5,
	EDMI_ERROR_CODE_BAD_TYPE_CODE_INTERNAL_ERROR = 6,
	EDMI_ERROR_CODE_DATA_NOT_READY_YET = 7,
	EDMI_ERROR_CODE_OUT_OF_RANGE = 8,
	EDMI_ERROR_CODE_NOT_LOGGED_IN = 9,
	EDMI_ERROR_CODE_REQUEST_CRC_ERROR = 10,
	EDMI_ERROR_CODE_RESPONSE_CRC_ERROR = 11,
	EDMI_ERROR_CODE_REQUEST_RESPONSE_COMMAND_MISMATCH = 12,
	EDMI_ERROR_CODE_REQUEST_RESPONSE_REGISTER_MISMATCH = 13,
	EDMI_ERROR_CODE_LOGIN_FAILED = 14,
	EDMI_ERROR_CODE_LOGOUT_FAILED = 15,
	EDMI_ERROR_CODE_GET_METER_ATTENTION_FAILED = 16,
	EDMI_ERROR_CODE_RESPONSE_WRONG_LENGTH = 17,
	EDMI_ERROR_CODE_UNIMPLEMENTED_DATA_TYPE = 18
} EDMI_ERROR_CODE;

typedef struct EDMI_DATE_TIME
{
	unsigned char Year;
	unsigned char Month;
	unsigned char Day;
	unsigned char Hour;
	unsigned char Minute;
	unsigned char Second;
	bool IsNull;
} EDMI_DATE_TIME;

typedef struct  EDMI_REGISTER
{
	int Address;
	char Type;
	char UnitCode;
	char ErrorCode;
	char Value[MAX_VALUE_LENGTH];
} EDMI_REGISTER;

typedef enum EDMI_SURVEY
{
	LS01 = 0x0305,
	LS02 = 0x0325,
	LS03 = 0x0345,
	LS04 = 0x0365,
	LS05 = 0x0385,
	LS06 = 0x0395,
	LS07 = 0x03A5,
	LS08 = 0x03B5,
	LS09 = 0x03C5,
	LS10 = 0x03D5
} EDMI_SURVEY;

typedef enum EDMI_COMMAND_TYPE
{
	EDMI_COMMAND_TYPE_GET_METER_ATTENTION = 0x1B,
	EDMI_COMMAND_TYPE_INFO = 'I',
	EDMI_COMMAND_TYPE_READ_REGISTER = 'R',
	EDMI_COMMAND_TYPE_WRITE_REGISTER = 'W',
	EDMI_COMMAND_TYPE_READ_REGISTER_EXTENDED = 'M',
	EDMI_COMMAND_TYPE_WRITE_REGISTER_EXTENDED = 'N',
	EDMI_COMMAND_TYPE_INFO_EXTENDED = 'O',
	EDMI_COMMAND_TYPE_READ_MULTIPLE_REGISTER_EXTENDED = 'A',
	EDMI_COMMAND_TYPE_WRITE_MULTIPLE_REGISTER_EXTENDED = 'B',
	EDMI_COMMAND_TYPE_LOGIN = 'L',
	EDMI_COMMAND_TYPE_LOGOUT = 'X',
	EDMI_COMMAND_TYPE_FILE_ACCESS = 'F'
} EDMI_COMMAND_TYPE;

typedef enum EDMI_COMMAND_EXTENSION
{
	EDMI_COMMAND_EXTENSION_NO_EXTENSION = 'N',
	EDMI_COMMAND_EXTENSION_FILE_READ = 'R',
	EDMI_COMMAND_EXTENSION_FILE_WRITE = 'W',
	EDMI_COMMAND_EXTENSION_FILE_INFO = 'I',
	EDMI_COMMAND_EXTENSION_FILE_SEARCH = 'S'
} EDMI_COMMAND_EXTENSION;

typedef enum EDMI_RESPONSE_CODE
{
	EDMI_RESPONSE_CODE_ACK = 0x06,
	EDMI_RESPONSE_CODE_CAN = 0x18
} EDMI_RESPONSE_CODE;

#define EDMI_FILE_INFO_REGS_COUNT 2
typedef struct EDMI_FILE_INFO
{
	// Regs
	int Interval;
	unsigned char ChannelsCount;

	// Access
	int StartRecord;
	int RecordsCount;
	short RecordSize;
	char Type;
	char Name[MAX_VALUE_LENGTH];
}EDMI_FILE_INFO;

#define EDMI_FILE_CHANNEL_INFO_REGS_COUNT 5
typedef struct EDMI_FILE_CHANNEL_INFO
{
	// Regs
	char Type;
	char UnitCode;
	char ScalingCode;
	float ScalingFactor;
	char Name[MAX_VALUE_LENGTH];
} EDMI_FILE_CHANNEL_INFO;

typedef enum EDMI_SEARCH_FILE_DIR
{
	EDMI_SEARCH_FILE_DIR_FORM_START_RECORD_BACKWARD = 0,
	EDMI_SEARCH_FILE_DIR_FORM_START_RECORD_FORWARD = 1
} EDMI_SEARCH_FILE_DIR;

typedef enum EDMI_SEARCH_FILE_RESULT
{
	EDMI_SEARCH_FILE_RESULT_FOUND_EXACT_MATCH = 0,
	EDMI_SEARCH_FILE_RESULT_HIT_END_OF_FILE = 1,
	EDMI_SEARCH_FILE_RESULT_FOUND_THE_CLOSEST_MATCH = 2,
	EDMI_SEARCH_FILE_RESULT_NO_TIME_STAMP_IN_SURVEY = 3,
	EDMI_SEARCH_FILE_RESULT_NO_DATA_RECORDED_IN_SURVEY = 4
} EDMI_SEARCH_FILE_RESULT;

typedef struct EDMI_SEARCH_FILE
{
	int StartRecord;
	EDMI_DATE_TIME DateTime;
	unsigned char DirOrResult;
} EDMI_SEARCH_FILE;

typedef struct EDMI_READ_FILE
{
	int StartRecord;
	short RecordsCount;
	short RecordOffset;
	short RecordSize;
} EDMI_READ_FILE;

typedef struct EDMI_FILE_FIELD
{
	char Value[MAX_VALUE_LENGTH];
} EDMI_FILE_FIELD;

#define EDMI_MAX_CHANNELS_COUNT 16
typedef struct EDMI_PROFILE_SPEC
{
	short Survey;
	int Interval;
	EDMI_DATE_TIME FromDateTime;
	EDMI_DATE_TIME ToDateTime;
	int RecordsCount;
	unsigned char ChannelsCount;
	EDMI_FILE_CHANNEL_INFO ChannelsInfo[EDMI_MAX_CHANNELS_COUNT];
	char Name[MAX_VALUE_LENGTH];
} EDMI_PROFILE_SPEC;

EDMI_TYPE EdmiGetDataType(EDMI_TYPE type);

EDMI_ERROR_CODE EdmiReadValue(char** payloadPtr, int* length, EDMI_TYPE type, char* valuePtr);

EDMI_ERROR_CODE EdmiReadRegisterValue(char** payloadPtr, int* length, EDMI_REGISTER* reg);

EDMI_REGISTER* EdmiGetFileInfoRegs(EDMI_SURVEY survey);

EDMI_ERROR_CODE EdmiSetProfileInfo(EDMI_FILE_INFO* info, EDMI_REGISTER* regs);

EDMI_REGISTER* EdmiGetFileChannelRegs(EDMI_SURVEY survey, unsigned char channel);

EDMI_ERROR_CODE EdmiSetFileChannelInfo(EDMI_FILE_CHANNEL_INFO* info, EDMI_REGISTER* regs);

EDMI_ERROR_CODE EdmiRead(char** buffer, int* remLen, char** value, int readLen);

EDMI_ERROR_CODE EdmiReadChar(char** buffer, int* remLen, char* value);

EDMI_ERROR_CODE EdmiReadInt16(char** buffer, int* remLen, short* value);

EDMI_ERROR_CODE EdmiReadInt32(char** buffer, int* remLen, int* value);

EDMI_ERROR_CODE EdmiReadInt64(char** buffer, int* remLen, long* value);

EDMI_ERROR_CODE EdmiReadFloat(char** buffer, int* remLen, float* value);

EDMI_ERROR_CODE EdmiReadDouble(char** buffer, int* remLen, double* value);

EDMI_ERROR_CODE EdmiReadString(char** buffer, int* remLen, char* value, int maxValueLen);

EDMI_ERROR_CODE EdmiReadDate(char** buffer, int* remLen, EDMI_DATE_TIME* dateTime);

EDMI_ERROR_CODE EdmiReadTime(char** buffer, int* remLen, EDMI_DATE_TIME* dateTime);

EDMI_ERROR_CODE EdmiReadDateTime(char** buffer, int* remLen, EDMI_DATE_TIME* dateTime);

void EdmiWriteDate(char** buffer, int* length, EDMI_DATE_TIME dateTime);

void EdmiWriteTime(char** buffer, int* length, EDMI_DATE_TIME dateTime);

void EdmiWriteDateTime(char** buffer, int* length, EDMI_DATE_TIME dateTime);

short EdmiGenerateCRC16(short i);

short EdmiCalculateCharacterCRC16(short crc, char c);

short EdmiCalculateCRC(char* packet, int length);

EDMI_ERROR_CODE EdmiValidateCRC(char* packet, int length);

void EdmiPreProccess(char** packet, int* length);

void EdmiPostProccess(char** packet, int* length);

void EdmiBeginInitPacket(char** packet, int* length, unsigned int serial, EDMI_COMMAND_TYPE commandType, EDMI_COMMAND_EXTENSION commandExtention);

void EdmiEndInitPacket(char** packet, int* length);

MEDIA_ERROR EdmiReceivePacket(READ_FROM_MEDIA_DELEGATE readCallback, char** packet, int* length, EDMI_ERROR_CODE* errorCode);

MEDIA_ERROR EdmiExecuteCommand(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, char** packet, int* length, EDMI_ERROR_CODE* errorCode);

void EdmiCreateLoginPacket(char** packet, int* length, unsigned int serial, char* username, int usernameLength, char* password, int passwordLength);

void EdmiCreateLogoutPacket(char** packet, int* length, unsigned int serial);

void EdmiCreateReadRegisterPacket(char** packet, int* length, unsigned int serial, EDMI_REGISTER* reg);

void EdmiCreateReadRegistersPacket(char** packet, int* length, unsigned int serial, EDMI_REGISTER* regs, int regsCount);

void EdmiCreateReadProfileInfoAccessPacket(char** packet, int* length, unsigned int serial, EDMI_SURVEY survey);

void EdmiCreateSearchProfilePacket(char** packet, int* length, unsigned int serial, EDMI_SURVEY survey, EDMI_SEARCH_FILE* search);

void EdmiCreateReadProfilePacket(char** packet, int* length, unsigned int serial, EDMI_SURVEY survey, EDMI_READ_FILE* read);

EDMI_ERROR_CODE EdmiParseLoginPayload(char* payload, int length);

EDMI_ERROR_CODE EdmiParseLogoutPayload(char* payload, int length);

EDMI_ERROR_CODE EdmiParseReadRegisterPayload(char* payload, int length, EDMI_REGISTER* reg);

EDMI_ERROR_CODE EdmiParseReadRegistersPayload(char* payload, int length, EDMI_REGISTER* regs, int regsCount);

EDMI_ERROR_CODE EdmiParseReadProfileInfoAccessPayload(char* payload, int length, EDMI_FILE_INFO* info);

EDMI_ERROR_CODE EdmiParseSearchProfilePayload(char* payload, int length, EDMI_SEARCH_FILE* search);

EDMI_ERROR_CODE EdmiParseReadProfilePayload(char* payload, int length, EDMI_READ_FILE* read, EDMI_PROFILE_SPEC* profileSpec, EDMI_FILE_FIELD** fields);

MEDIA_ERROR EdmiReadProfileInfoAccess(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, unsigned int serial, EDMI_SURVEY survey, EDMI_FILE_INFO* info, EDMI_ERROR_CODE* errorCode);

MEDIA_ERROR EdmiReadProfileInfo(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, unsigned int serial, EDMI_SURVEY survey, EDMI_FILE_INFO* info, EDMI_ERROR_CODE* errorCode);

MEDIA_ERROR EdmiReadProfileChannelInfo(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, unsigned int serial, EDMI_SURVEY survey, EDMI_REGISTER* regs, int regsCount, EDMI_FILE_CHANNEL_INFO* info, EDMI_ERROR_CODE* errorCode);

MEDIA_ERROR EdmiReadProfileChannelsInfo(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, unsigned int serial, EDMI_SURVEY survey, EDMI_FILE_CHANNEL_INFO* infoes, unsigned char channelsCount, EDMI_ERROR_CODE* errorCode);

MEDIA_ERROR EdmiSearchProfile(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, unsigned int serial, EDMI_SURVEY survey, EDMI_SEARCH_FILE* search, EDMI_ERROR_CODE* errorCode);

EXPORT MEDIA_ERROR EdmiLogin(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, unsigned int serial, char* username, int usernameLength, char* password, int passwordLength, unsigned char* errCode);

EXPORT MEDIA_ERROR EdmiLogout(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, unsigned int serial, unsigned char* errCode);

EXPORT MEDIA_ERROR EdmiReadRegister(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, unsigned int serial, EDMI_REGISTER* reg, unsigned char* errCode);

EXPORT MEDIA_ERROR EdmiReadRegisters(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, unsigned int serial, EDMI_REGISTER* regs, int regsCount, unsigned char* errCode);

EXPORT MEDIA_ERROR EdmiReadProfile(READ_FROM_MEDIA_DELEGATE readCallback, WRITE_ALL_TO_MEDIA_DELEGATE writeCallback, unsigned int serial, short survey, EDMI_DATE_TIME fromDateTime, EDMI_DATE_TIME toDateTime, EDMI_PROFILE_SPEC* profileSpec, EDMI_FILE_FIELD** fields, unsigned char* errCode);