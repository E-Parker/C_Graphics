#include "assert.h"
#include "errno.h"

#include "engine_core/string.h"
#include "file_reader.h"


int Reader_Initialize(Reader* reader, const char* path) {
	reader->fileDesc = fopen(path, "r");
	return ferror(reader->fileDesc);
}


int Reader_Deinitialize(Reader* reader) {
	fclose(reader->fileDesc);
	return ferror(reader->fileDesc);
}


char internal_Reader_at(Reader* reader, u64 index) {
	u32 blockContainingIndex = index / READER_BUFFER_SIZE;
	
    // if the block containing the character we're after is before the current position, 
	// we need to reset to the start of the line.
	if (blockContainingIndex < reader->currentBlock) {
		fsetpos(reader->fileDesc, &reader->fileLinePos);
		reader->currentBlock = 0;
	}

	u32 blocksToContaining = blockContainingIndex - reader->currentBlock;
	u16 blockIndex = index % READER_BUFFER_SIZE;

	// request blocks until we're in the correct block.
	for (u32 i = 0; i < blocksToContaining; ++i) {
		fgets(internal_Reader_buffer(reader), READER_BUFFER_SIZE + 1, reader->fileDesc);
	}

	reader->currentBlock = blockContainingIndex;

	return reader->buffer[blockIndex];
}


bool internal_Reader_FindLineEnd(Reader* reader) {
	// Set buffer end to max u32 so when buffer overrun occurs, we can detect if the line is too big to fit in the buffer.
	reader->bufferEnd = ~0;
	
    // request size of buffer +1 so that the first byte of bufferEnd is overwriten.
    fgets(internal_Reader_buffer(reader), READER_BUFFER_SIZE + 1, reader->fileDesc);	
	
    // line is less than the length of the buffer:
	if (reader->bufferEnd == ~0) {
		for (u16 i = 0; i < READER_BUFFER_SIZE; ++i) {
			if (reader->buffer[i] == '\0') {
				reader->length += i;
				return true;
			}
		}
		// This shouldn't happen.
		assert(false);
	}
    
    // line is exactly the length of the buffer:
	if (reader->buffer[0xff] == '\0') {
		reader->length += READER_BUFFER_SIZE;
		return true;
	}
    
    // line must be longer than the buffer.
	reader->length += READER_BUFFER_SIZE;
	return false;
}


// this just looks horrid. please fix :(
int Reader_NextLine(Reader* reader) {
	fgetpos(reader->fileDesc, &reader->fileLinePos);
#ifndef READER_LIMIT_LINE_END_DEPTH
    while (!internal_Reader_FindLineEnd(reader)) {}
#else
    u64 iteration = 0;
    while (!internal_Reader_FindLineEnd(reader)) { if (iteration >= READER_LIMIT_LINE_END_DEPTH) return EFBIG; ++iteration; }
#endif
    fsetpos(reader->fileDesc, &reader->fileLinePos);
    return ferror(reader->fileDesc);
}


