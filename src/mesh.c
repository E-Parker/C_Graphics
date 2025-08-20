#include <stdint.h>
#include <stddef.h>

#include "list.h"
#include "string_utilities.h"


int parseFaceIndicies(List* vi, List* ti, List* ni, char* line) {
    // This function parses a wavefront file face data, adding it to the list of vertex indices, texture indicies, and normal indeicies. 
    // Returns the number of vertices added. 

    if (line == NULL) return 0;
    
    String segments[0x6];
    char buffer[0xff];
    uint8_t splits;
    char* currentChar = line;

    for (uint8_t i = 0;  i < 0xff; ++i, ++currentChar) {
        // Check the current character.
        switch ((int)(*currentChar)) {
            case '\0': return 1;// End of file reached early.
            case '\n':          // End of line reached. 
                segments[splits].bufferEnd = currentChar - 1;
                i = 0xff;
                break;
            case ' ':           // Seperator is found. Segment start and end must be set before incrementing splits.
                if (segments[splits].bufferStart)   segments[splits].bufferEnd = currentChar - 1;   // SegmentStart cannot be set on interation 0 so this is fine.
                else                                segments[splits].bufferStart = currentChar + 1;
                break;
            default: continue; break;
        }
        
        // Increment splits counter if this segment's start and end are set.
        if (segments[splits].bufferStart && segments[splits].bufferEnd) {
            splits++;
        }
        
        // break from loop if out of segments.
        if (splits >= 0x6) {
            break;
        }
    }
    return 0;
}


