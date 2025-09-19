#pragma once

#include "stdint.h"

// simple string implementation using a buffer start and end method.

typedef struct String {
    char* bufferStart;
    char* bufferEnd;
} String;

// clears a buffer to length of string + 1, writes contents of string to buffer with a null terminator.
#define String_CloneToBuffer(string, destination) internal_String_clone_to_buffer(&string, &destination)

#define String_create(string, source) internal_String_create(&string, source) 
#define String_substring(source, destination, start, end) internal_String_substring(&source, &destination, (uint64_t)start, (uint64_t)end)
#define String_length(string) ((uint64_t)(string.bufferEnd - string.bufferStart))

void internal_String_cloneToBuffer(String* string, char* destination);
void internal_String_create(String* string, char* source);
void internal_String_substring(String* source, String* destination, uint64_t start, uint64_t end);

uint64_t fnvHash64(const char* buffer, const char* const bufferEnd);
char* FindBufferEnd(const char* buffer);

