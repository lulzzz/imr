// Authored by: MohammadAmin Mohammadi (https://github.com/mamohammadi)

#pragma once
// Disable struct padding
#pragma pack(push,1)

#include <stdbool.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <Windows.h>

#define EXPORT __declspec(dllexport)
#define CALLBACK __stdcall

#define MAX_VALUE_LENGTH 25

typedef enum DATA_TYPE
{
	DATA_TYPE_BYTE,
	DATA_TYPE_BOOLEAN,
	DATA_TYPE_STRING,
	DATA_TYPE_INT16,
	DATA_TYPE_UINT16,
	DATA_TYPE_INT32,
	DATA_TYPE_UINT32,
	DATA_TYPE_INT64,
	DATA_TYPE_UINT64,
	DATA_TYPE_FLOAT,
	DATA_TYPE_DOUBLE,
	/*DATA_TYPE_VARIABLE_SPACE,*/
	DATA_TYPE_DATE,
	DATA_TYPE_TIME,
	DATA_TYPE_DATE_TIME,
	/*DATA_TYPE_INVALID,
	DATA_TYPE_SPECIAL,
	DATA_TYPE_WAVEFORM,*/
	DATA_TYPE_UNKNOWN
} DATA_TYPE;

EXPORT void Init();

EXPORT void Release(void* buffer);

char* Clone(char* buffer, int length);

short L16toB16(short value);

int L32toB32(int value);

int L64toB64(long value);

bool Read(char** buffer, int* remLen, char** value, int readLen);

void Write(char** buffer, int* length, char* bytes, int bytesLength);

bool ReadChar(char** buffer, int* remLen, char* value);

void WriteChar(char** buffer, int* length, char c);

bool ReadString(char** buffer, int* remLen, char* value, int maxValueLength, int* readedLength);

void WriteString(char** buffer, int* length, char* str, int strLength);

bool ReadInt16(char** buffer, int* remLen, short* value);

void WriteInt16(char** buffer, int* length, short value);

bool ReadInt32(char** buffer, int* remLen, int* value);

void WriteInt32(char** buffer, int* length, int value);

bool ReadInt64(char** buffer, int* remLen, long* value);

void WriteInt64(char** buffer, int* length, long value);

bool ReadFloat(char** buffer, int* remLen, float* value);

void WriteFloat(char** buffer, int* length, float value);

bool ReadDouble(char** buffer, int* remLen, double* value);

void WriteDouble(char** buffer, int* length, double value);