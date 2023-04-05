// Authored by: MohammadAmin Mohammadi (https://github.com/mamohammadi)

#include "IMRCore.h"

EXPORT void Init()
{
	
}

EXPORT void Release(void* buffer)
{
	free(buffer);
}

char* Clone(char* buffer, int length)
{
	char* output = malloc(length * sizeof(char));
	memcpy(output, buffer, length);
	return output;
}

short L16toB16(short value)
{
	return ((value & 0xFF) << 8) | ((value >> 8) & 0xFF);
}

int L32toB32(int value)
{
	return
		((value & 0xFF) << 24) |
		(((value >> 8) & 0xFF) << 16) |
		(((value >> 16) & 0xFF) << 8) |
		((value >> 24) & 0xFF);
}

int L64toB64(long value)
{
	return
		((value & 0xFF) << 56) |
		(((value >> 8) & 0xFF) << 48) |
		(((value >> 16) & 0xFF) << 40) |
		(((value >> 24) & 0xFF) << 32) |
		(((value >> 32) & 0xFF) << 24) |
		(((value >> 40) & 0xFF) << 16) |
		(((value >> 48) & 0xFF) << 8) |
		((value >> 56) & 0xFF);
}

bool Read(char** buffer, int* remLen, char** value, int readLen)
{
	if (readLen > remLen)
		return false;

	*value = *buffer;
	*buffer += readLen;
	*remLen -= readLen;
	return true;
}

void Write(char** buffer, int* length, char* bytes, int bytesLength)
{
	int size = *length + bytesLength;
	char* r = malloc(size * sizeof(char));
	char* bufferPtr = *buffer;
	char* bytesPtr = bytes;
	char* rPtr = r;

	// Append buffer
	memcpy(rPtr, bufferPtr, *length);
	rPtr += *length;

	// Append bytes
	memcpy(rPtr, bytesPtr, bytesLength);

	// Cleanup
	free(*buffer);

	*length = size;
	*buffer = r;
}

bool ReadChar(char** buffer, int* remLen, char* value)
{
	char* startPtr;
	if (Read(buffer, remLen, &startPtr, sizeof(char)) == true)
	{
		*value = *startPtr;
		return true;
	}
	return false;
}

void WriteChar(char** buffer, int* length, char c)
{
	int size = *length + 1;
	char* r = malloc(size * sizeof(char));
	char* bufferPtr = *buffer;
	char* rPtr = r;

	// Append buffer
	memcpy(rPtr, bufferPtr, *length);
	rPtr += *length;

	// Append c
	*rPtr = c;

	// Cleanup
	free(*buffer);

	*length = size;
	*buffer = r;
}

bool ReadString(char** buffer, int* remLen, char* value, int maxValueLength, int* readedLength)
{
	char* startPtr = NULL;
	int counter = 0;
	*readedLength = 0;
	while (counter < maxValueLength)
	{
		if (Read(buffer, remLen, &startPtr, sizeof(char)) == true)
		{
			*readedLength = (*readedLength) + 1;
			char ch = *startPtr;
			*value++ = ch;
			if (ch == NULL)
				break;
		}
		else
			return false;
		counter++;
	}
	return true;
}

void WriteString(char** buffer, int* length, char* str, int strLength)
{
	Write(buffer, length, str, strLength);
	WriteChar(buffer, length, 0x00); // Write null
}

bool ReadInt16(char** buffer, int* remLen, short* value)
{
	char* startPtr = NULL;
	if (Read(buffer, remLen, &startPtr, sizeof(short)) == true)
	{
		*value = *(short*)startPtr;
		*value = L16toB16(*value);
		return true;
	}
	return false;
}

void WriteInt16(char** buffer, int* length, short value)
{
	value = L16toB16(value);
	char* bytes = (char*)(&value);
	Write(buffer, length, bytes, sizeof(short));
}

bool ReadInt32(char** buffer, int* remLen, int* value)
{
	char* startPtr = NULL;
	if (Read(buffer, remLen, &startPtr, sizeof(int)) == true)
	{
		*value = *(int*)startPtr;
		*value = L32toB32(*value);
		return true;
	}
	return false;
}

void WriteInt32(char** buffer, int* length, int value)
{
	value = L32toB32(value);
	char* bytes = (char*)(&value);
	Write(buffer, length, bytes, sizeof(int));
}

bool ReadInt64(char** buffer, int* remLen, long* value)
{
	char* startPtr = NULL;
	if (Read(buffer, remLen, &startPtr, sizeof(long)) == true)
	{
		*value = *(long*)startPtr;
		*value = L64toB64(*value);
		return true;
	}
	return false;
}

void WriteInt64(char** buffer, int* length, long value)
{
	value = L64toB64(value);
	char* bytes = (char*)(&value);
	Write(buffer, length, bytes, sizeof(long));
}

bool ReadFloat(char** buffer, int* remLen, float* value)
{
	int rawValue;
	if (ReadInt32(buffer, remLen, &rawValue) == true)
	{
		float* floatPtr = (float*)& rawValue;
		*value = *floatPtr;
		return true;
	}
	return false;
}

void WriteFloat(char** buffer, int* length, float value)
{
	WriteInt32(buffer, length, *(int*)& value);
}

bool ReadDouble(char** buffer, int* remLen, double* value)
{
	long rawValue;
	if (ReadInt64(buffer, remLen, &rawValue) == true)
	{
		double* doublePtr = (double*)& rawValue;
		*value = *doublePtr;
		return true;
	}
	return false;
}

void WriteDouble(char** buffer, int* length, double value)
{
	WriteInt64(buffer, length, *(long*)& value);
}