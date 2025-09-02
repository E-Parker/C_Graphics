	#include <stdio.h>
#include <assert.h>

#include "string_utilities.h"
#include "file_reader.h"


int Reader_Initialize(Reader* reader, const char* path) {

	reader->fileDesc = fopen(path, "r");
	return ferror(reader->fileDesc);
}


int Reader_Deinitialize(Reader* reader) {
	fclose(reader->fileDesc);
	return ferror(reader->fileDesc);
}

char internal_Reader_at(Reader* reader, uint64_t index) {
	
	uint32_t blockContainingIndex = index / 0x100;

	// if the block containing the character we're after is before the current position, 
	// we need to reset to the start of the line.
	if (blockContainingIndex < reader->currentBlock) {
		fsetpos(reader->fileDesc, reader->fileLinePos);
		reader->currentBlock = 0;
	}

	uint32_t blocksToContaining = blockContainingIndex - reader->currentBlock;
	uint16_t blockIndex = index % 0x100;

	// request blocks until we're in the correct block.
	for (uint32_t i = 0; i < blocksToContaining; ++i) {
		fgets(reader->buffer, 0x101, reader->fileDesc);
	}

	reader->currentBlock = blockContainingIndex;

	return reader->buffer[blockIndex];
}

bool internal_Reader_FindLineEnd(Reader* reader) {
	
	// Set buffer end to max uint32_t so when buffer overrun occurs, we can detect if the line is too big to fit in the buffer.
	reader->bufferEnd = 0xFFFFFFFF;
	fgets(reader->buffer, 0x101, reader->fileDesc);	// request size of buffer +1 so that the first byte of bufferEnd is overwriten.

	// return early if end of file reached.
	if (feof(reader->fileDesc)) {
		clearerr(reader->fileDesc);
		return true;
	}

	// line is less than the length of the buffer:
	if (reader->bufferEnd == 0xFFFFFFFF) {
		// Find the end of line (first instance of '\0').
		for (uint16_t i = 0; i < 0x100; ++i) {
			if (reader->buffer[i] == '\0') {
				reader->length += i;
				return true;
			}
		}
		// This shouldn't happen.
		assert(false);
	}

	// buffer end was overwritten, so line is exactly 256 characters:
	if (reader->buffer[0xff] == '\n') {
		reader->length += 0xff;
		return true;
	}

	// buffer end was overwritten but there is more characters left.
	reader->length += 0x100;
	return false;
}


int Reader_NextLine(Reader* reader){
	fgetpos(reader->fileDesc, &reader->fileLinePos);
	while (!internal_Reader_FindLineEnd(reader)) {}
	fsetpos(reader->fileDesc, reader->fileLinePos);
}

