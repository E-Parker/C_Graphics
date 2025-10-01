#pragma once

#include "../engine_core/engine_types.h"

#define char_as_lower(c) (c > 0x40 && c < 0x5b) ? c | 0x60 : c
#define char_as_upper(c) (c > 0x60 && c < 0x7b) ? c & !0x40 : c

// simple string implementation using a buffer start and end method.

typedef struct String {
    char* start;
    char* end;
} String;

// Create a copy of a string on the heap. This is outside the normal use-cases for this string implementation. Use at your own risk!
void String_create_dirty(String* source, String* destination);

// Only use to free strings created on the heap with the String_create_dirty method.
void String_free_dirty(String* str);

// writes the contents of a String to a char*, with a null terminator. Ensure the destination is at least String length + 1. 
#define String_clone_to_ptr(string, destination) internal_String_clone_to_chars(&string, (char*)destination)

// writes contents of string to a stack buffer with a null terminator. Ensure the destination is at least String length + 1. 
#define String_clone_to_chars(string, destination) internal_String_clone_to_chars(&string, &destination)

// initialize a String from a char[]. buffer must be on the stack.
#define String_from_chars(buffer) { .start = (char*)(&buffer), .end = (char*)(&buffer) + sizeof(buffer) }

// initialize a String from a char*. Uses the more expensive FindBufferEnd function to create a string from a c_str.
#define String_from_ptr(buffer) {.start = (char*)buffer, .end = FindBufferEnd((const char*)buffer) }

#define String_from_ptr_size(buffer, buffersize) { .start = (char*)buffer, .end = (char*)buffer + (u64)buffersize }

#define String_equal(left, right) internal_String_equal(&left, &right)
#define String_invalid(string) (!(string).start || !(string).end || (string).start == (string).end)

#define String_length(string) ((u64)((string).end - (string).start))
#define String_as_lower(string) for (char* c = (string).start; c <= (string).end; ++c) { *c = char_as_lower(*c); }
#define String_as_upper(string) for (char* c = (string).start; c <= (string).end; ++c) { *c = char_as_upper(*c); }

#define String_first(string, pattern) internal_String_first(&(string), pattern);
#define String_last(string, pattern) internal_String_last(&(string), pattern);

#define String_clone(source, destination) (destination).end = (destination).start; for (char* c = (source).start; c <= (source).end; ++c, ++(destination).end) { *((destination).end) = *c; }  
#define String_substring(source, start, end) { .start = (source).start + (u64)start, .end = (source).start + (u64)end }
#define String_clone_substring(source, destination, start, end) internal_String_clone_substring (&source, &destination, (u64)start, (u64)end)

bool internal_String_equal(String* left, String* right);

void internal_String_clone_substring (String* source, String* destination, u64 start, u64 end);
void internal_String_clone_to_chars (String* string, char* destination);

char* internal_String_first (String* string, char pattern);
char* internal_String_last (String* string, char pattern);

u64 fnvHash64 (const char* buffer, const char* const bufferEnd);
char* FindBufferEnd (const char* buffer);
