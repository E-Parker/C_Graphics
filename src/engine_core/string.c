#include "stdint.h"
#include "assert.h"

#include "engine_core/string.h"


void internal_String_clone_substring (String* source, String* destination, uint64_t start, uint64_t end) {
    assert(start < end);
    assert(end <= String_length(*destination) && start < String_length(*destination));

    char* dst = destination->start;
    char* src = source->start + start;

    for (; src < source->start + end; ++src, ++dst) {
        *dst = *src;
    }

    destination->end = dst;
}


void String_clone_to_String (String* source, String* destination) {
    if (source == destination) {
        return;
    }

    if (String_length(*source) > String_length(*destination)) {
        return;
    }

    char* dst = destination->start;
    char* src = source->start;

    for (; src < source->end; ++src, ++dst) {
        *dst = *src;
    }

    destination->end = dst;
}


void internal_String_clone_to_buffer (String* string, char* destination) {
    for (char* src = string->start; src <= string->end; ++src, ++destination) {
        *destination = *src;
    }
    *++destination = '\0';
}
