#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define READER_BUFFER_SIZE 0x100

typedef struct Reader {
    char buffer[READER_BUFFER_SIZE];    // Internal use only, raw data. 
    uint32_t bufferEnd;                 // Internal use only, padding for the buffer.
    uint32_t currentBlock;              // Internal use only, current "block" of READER_BUFFER_SIZE stored in buffer.
    fpos_t fileLinePos;                 // Internal use only, Position of current line in file.
    uint64_t length;                    // Length of the segment currently in the buffer. May be larger than the size of the buffer.
    FILE* fileDesc;                     // File handler associated with this reader.
} Reader;

// Crude pointer cast to avoid warnings about intentional buffer overrun.
#define internal_Reader_buffer(reader) ((char*)reader)

// Get a char from it's index in the line. Use the length property for loops and what not.
#define Reader_at(reader, index) ((reader->length > READER_BUFFER_SIZE) ? internal_Reader_at(reader, (uint64_t)index) : reader->buffer[index])

int Reader_Initialize(Reader* reader, const char* path);
int Reader_Deinitialize(Reader* reader);

char internal_Reader_at(Reader* reader, uint64_t index);

// Finds the length of the current line. Returns true when end of line is found, or end of file.
bool internal_Reader_FindLineEnd(Reader* reader);

int Reader_NextLine(Reader* reader);


