#pragma once

#include "../engine_core/engine_types.h"

#define char_as_lower(c) (c > 0x40 && c < 0x5b) ? c | 0x60 : c
#define char_as_upper(c) (c > 0x60 && c < 0x7b) ? c & !0x40 : c

// simple string implementation using a buffer start and end method.

typedef struct String {
    char* start;
    char* end;
} String;

// clears a buffer to length of string + 1, writes contents of string to buffer with a null terminator.
#define String_clone_to_buffer(string, destination) internal_String_clone_to_buffer(&string, &destination)

// initialize a String from a char[]. buffer must be on the stack.
#define String_from_chars(buffer) { .start = (char*)(&buffer), .end = (char*)(&buffer) + sizeof(buffer) }
#define String_from_ptr(buffer) {.start = (char*)buffer, .end = FindBufferEnd((const char*)buffer) }
#define String_length(string) ((u64)((string).end - (string).start) + 1)
#define String_invalid(string) (!(string).start || !(string).end || (string).start == (string).end)

#define String_as_lower(string) for (char* c = (string).start; c < (string).end; ++c) { *c = char_as_lower(*c); }
#define String_as_upper(string) for (char* c = (string).start; c < (string).end; ++c) { *c = char_as_upper(*c); }

#define String_first(string, pattern) internal_String_first(&(string), pattern);
#define String_last(string, pattern) internal_String_last(&(string), pattern);

#define String_clone(source, destination) destination.end = destination.start; for (char* c = (source).start; c < (source).end; ++c, ++destination.end) { *destination.end = *c; }  
#define String_substring(source, start, end) { .start = (source).start + (u64)start, .end = (source).start + (uint64_t)end }
#define String_clone_substring(source, destination, start, end) internal_String_clone_substring (&source, &destination, (u64)start, (u64)end)

void internal_String_clone_substring (String* source, String* destination, u64 start, u64 end);
void internal_String_clone_to_buffer (String* string, char* destination);

char* internal_String_first (String* string, char pattern);
char* internal_String_last (String* string, char pattern);

u64 fnvHash64(const char* buffer, const char* const bufferEnd);
char* FindBufferEnd(const char* buffer);
