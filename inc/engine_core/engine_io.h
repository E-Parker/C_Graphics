#pragma once

#include "stdio.h"
#include "engine_core/engine_types.h"

typedef struct IODescriptor {
	void* file;
	char* buffer;
	u64 bufferSize;
	u64 flags;
} IODescriptor;

#define ENGINE_IO_ADD_NULL_TERMINATOR 1
#define ENGINE_IO_ZERO_BUFFER 2

// Bytes the engine attempts to read before flushing the buffer.
#define ENGINE_BUFFER_READ_STEP 0x100

// Bytes the engine attempts to write before flushing the buffer.
#define ENGINE_BUFFER_WRITE_STEP 0x1000

// Read from file, ENGINE_BUFFER_READ_STEP bytes at a time, to a buffer.
ecode Engine_read(const IODescriptor iodesc);

// Write to file, ENGINE_BUFFER_WRITE_STEP bytes at a time, to a file.
ecode Engine_write(const IODescriptor iodesc);