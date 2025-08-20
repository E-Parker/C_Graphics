#pragma once

#include "stdint.h"

// Forward declariations:
typedef struct File File;

typedef struct Reader {
    // File parsing struct.
    //
    //
    
    File* fileDesc;     // File handler ascociated with this reader.
    uint32_t length;    // Length of the segment currently being parsed. May be larger than the buffer size.
    uint8_t segmentdlim;// delimiter signifying the end of a segment. for example, '\n' or '\0'. 
    uint8_t mode;       // mode the Reader should operate in.
    uint16_t padding;   //
    char buffer[0xff];  // raw data.  

} Reader;

// TODO: add some method to handle jumping around the file.
#define Reader_at(reader, index) ((reader->length > 0xff) ?  )

int Reader_Initialize(Reader* reader, const char* path, char mode, char segmentid);
int Reader_Deinitialize(Reader* reader);

int Reader_NextBlock(Reader* reader);
int Reader_PrevBlock(Reader* reader);

