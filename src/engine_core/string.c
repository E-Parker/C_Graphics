#include <stdint.h>
#include <assert.h>

#include "engine_core/string.h"

void internal_String_create(String* string, char* source) {
    string->bufferStart = source;
    string->bufferEnd = FindBufferEnd((const char*)source);
}


void internal_String_substring(String* source, String* destination, uint64_t start, uint64_t end) {
    assert(start < end);
    assert(end <= String_length((*destination)) && start < String_length((*destination)));
    destination->bufferStart = source->bufferStart + start;
    destination->bufferEnd = source->bufferStart + end;
}


void internal_String_cloneToBuffer(String* string, char* destination) {    
    for (char* src = string->bufferStart; src <= string->bufferEnd; ++src, ++destination) {
        *destination = *src;
    }
    *++destination = '\0';
}


