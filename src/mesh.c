#include <stdint.h>
#include <stddef.h>

#include "list.h"
#include "string_utilities.h"

#include "mesh.h"


static char segmentBuffer[0xff];


/*
void vec2FromString(String data) {

    std::stringstream dataStream(data);
    std::string segment;
    std::vector<std::string> segmentList;

    while (std::getline(dataStream, segment, ' ')) {
        if (!segment.empty()) {
            segmentList.push_back(segment);
        }
    }

    Vector2 vector{ 0.0f, 0.0f };

    vector.x = std::stof(segmentList[0]);
    vector.y = std::stof(segmentList[1]);

    return vector;

}

Vector3 Vector3FromString(const std::string data) {

    std::stringstream dataStream(data);
    std::string segment;
    std::vector<std::string> segmentList;

    while (std::getline(dataStream, segment, ' ')) {
        if (!segment.empty()) {
            segmentList.push_back(segment);
        }
    }

    Vector3 vector{ 0.0f, 0.0f, 0.0f };

    vector.x = std::stof(segmentList[0]);
    vector.y = std::stof(segmentList[1]);
    vector.z = std::stof(segmentList[2]);

    return vector;
}
*/

void parseSplits(char* line, uint16_t MaxSegments, String* segments) {
    
    uint16_t splits = 0;
    char* currentChar = line;

    for (uint16_t i = 0; i < 0xffff; ++i, ++currentChar) {
        // Check the current character.
        switch ((int)(*currentChar)) {
        case '\0': return 1;// End of file reached early.
        case '\n':          // End of line reached. 
            segments[splits].bufferEnd = currentChar - 1;
            i = 0xffff;
            break;
        case ' ':           // Separator is found. Segment start and end must be set before incrementing splits.
            if (segments[splits].bufferStart)   segments[splits].bufferEnd = currentChar - 1;   // SegmentStart cannot be set on iteration 0 so this is fine.
            else                                segments[splits].bufferStart = currentChar + 1;
            break;
        default: continue; break;
        }

        // Increment splits counter if this segment's start and end are set.
        if (segments[splits].bufferStart && segments[splits].bufferEnd) {
            ++splits;
        }

        // break from loop if out of segments.
        if (splits >= MaxSegments) {
            break;
        }
    }
}

int parseFaceIndicies(List* vi, List* ti, List* ni, char* line) {
    // This function parses a wavefront file face data, adding it to the list of vertex indices, texture indicies, and normal indicies. 
    // Returns the number of vertices added. 

    if (line == NULL) return 0;
    
    String segments[0x6];
    
    parseSplits(line, 0x6, &segments);

    for (uint8_t i = 0; i < 0x6; ++i) {

    }


}


StaticMesh* CreateStaticMeshFromWavefront(const char* path) {
    return NULL;



}


void Object_StaticMesh_destroy(StaticMesh* mesh) {
    OBJECT_DESTROY_BODY(mesh);
}


