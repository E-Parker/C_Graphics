#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

uint64_t fnvHash64(const char* buffer, const char* const bufferEnd);
char* FindBufferEnd(const char* buffer);

#ifdef __cplusplus
}
#endif
