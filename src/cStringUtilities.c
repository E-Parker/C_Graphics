
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include "cStringUtilities.h"

uint64_t fnvHash64(const char* buffer, const char* const bufferEnd) {
    // implementation of the fnv64 hashing function, created by Glenn Fowler, Landon Curt Noll,
    // and Kiem-Phong Vo. I used fixed-width integers here for maximum portability.
    // 

    const uint64_t Prime = 0x00000100000001B3;
    uint64_t Hash = 0xCBF29CE484222325;

    char* bufferIter = (char*)buffer;

    // Iterate from the buffer start address to the buffer end address.
    for (; bufferIter < bufferEnd; bufferIter++) {
        //XOR the current hash with the current character, then multiply by an arbitrary prime number.
        Hash = (Hash ^ (*bufferIter)) * Prime;
    }

    return Hash;
}

char* FindBufferEnd(const char* buffer) {
    // Find the end of a buffer from a given c-string. 

    char* bufferEnd = (char*)buffer;

    // Assume the key is a c_string, iterate through to find the null terminator.
    for (uint16_t i = 0; i < 0xffff; i++) {
        if (*bufferEnd == '\0') {
            return bufferEnd;
        }
        bufferEnd++;
    }
    return NULL;
}




