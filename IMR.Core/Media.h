// Authored by: MohammadAmin Mohammadi (https://github.com/mamohammadi)

#pragma once
// Disable struct padding
#pragma pack(push,1)

#include "IMRCore.h"

typedef enum MEDIA_ERROR
{
	MEDIA_ERROR_NONE,
	MEDIA_ERROR_NOT_INITIATED,
	MEDIA_ERROR_CAN_NOT_OPEN_DEVICE,
	MEDIA_ERROR_NOT_CONNECTED,
	MEDIA_ERROR_WRITE_ERROR,
	MEDIA_ERROR_READ_ERROR,
	MEDIA_ERROR_RESPONSE_TIMEOUT,
	MEDIA_ERROR_EXCEPTION
} MEDIA_ERROR;

typedef MEDIA_ERROR(CALLBACK* WRITE_ALL_TO_MEDIA_DELEGATE)(char* buffer, int length);

typedef MEDIA_ERROR(CALLBACK* READ_FROM_MEDIA_DELEGATE)(char* byte);

MEDIA_ERROR WriteAllToMedia(WRITE_ALL_TO_MEDIA_DELEGATE callback, char* buffer, int length);

MEDIA_ERROR ReadFromMedia(READ_FROM_MEDIA_DELEGATE callback, char* byte);