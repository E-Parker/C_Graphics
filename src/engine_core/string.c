
#include "assert.h"

#include "engine_core/engine_types.h"
#include "engine_core/string.h"

#define USE_CSTR_REDUNDANCY

void String_create_dirty(String* source, String* destination) {

    if (!source->end || !source->start) {
        return;
    }

#ifdef USE_CSTR_REDUNDANCY
    destination->start = (char*)calloc(String_length(*source) + 1, sizeof(char));
#else
    destination->start = (char*)malloc(String_length(*source));
#endif
    String_clone(*source, *destination);
    --destination->end;
}

void String_free_dirty(String* str) {
    if (!str->start) return;
    free(str->start);
    str->start = NULL;
    str->end = NULL;
}

#include "stdio.h"

bool internal_String_equal(String* left, String* right) {
    if (!left || !right) {
        return false;
    }

    if (String_length(*left) != String_length(*right)) {
        return false;
    }
    
    char* l = left->start;
    char* r = right->start;

    for (; l < left->end; ++l, ++r) {
        if ((*l - *r)) {
            return false;
        }
    }

    return true;
}

void internal_String_clone_substring (String* source, String* destination, u64 start, u64 end) {
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


void internal_String_clone_to_chars (String* string, char* destination) {
    char* dst = destination;
    char* src = string->start;
    for (; src <= string->end; ++src, ++dst) {
        *dst = *src;
    }
    ++dst;
    *dst = '\0';
}

char* internal_String_first (String* string, char pattern) {
    char* buffer = string->start;
    while (buffer != string->end) {
        if (*buffer == pattern) {
            return buffer;
        }
        buffer++;
    }
    return NULL;
}


char* internal_String_last (String* string, char pattern) {
    char* buffer = string->start;
    char* match = NULL;
    while (buffer != string->end) {
        if (*buffer == pattern) {
            match = buffer;
        }
        buffer++;
    }
    return match;

}
