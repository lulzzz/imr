#include "Media.h"

MEDIA_ERROR WriteAllToMedia(WRITE_ALL_TO_MEDIA_DELEGATE callback, char* buffer, int length)
{
	MEDIA_ERROR ret = callback(buffer, length);
	free(buffer);
	return ret;
}

MEDIA_ERROR ReadFromMedia(READ_FROM_MEDIA_DELEGATE callback, char* byte)
{
	return callback(byte);
}
