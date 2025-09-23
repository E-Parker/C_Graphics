#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"

#include "engine_core/string.h"
#include "engine_core/list.h"

#include "file_reader.h"
#include "renderable.h"
#include "mesh.h"

#define BUFFER_SIZE 0x1000
#define SEGMENT_COUNT 16

#define EARLY_EOF -1
#define OUT_OF_SEGMENTS -2
#define LINE_TOO_LARGE -3
#define SEGMENT_TOO_LARGE -4
#define SEGMENT_MISSING_TAG -5


typedef struct ObjParser {
    List vi;
    List ti;
    List ni;
    List surfaceSplitIndecies;
    List vertexList;
    List normalList;
    List tCoordList;
    uint64_t materialCount;
    FILE* file;
    char buffer[BUFFER_SIZE];
} ObjParser;

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

int parseSplits(char* line, uint16_t segmentCount, String* segments) {

    uint16_t splits = 0;
    char* currentChar = line;

    for (uint16_t i = 0; i < 0xffff; ++i, ++currentChar) {
        // Check the current character.
        switch ((int)(*currentChar)) {
        case '\0': return EARLY_EOF;// End of file reached early.
        case '\n':                  // End of line reached. 
            segments[splits].end = currentChar - 1;
            return 0;
        case ' ':                   // Separator is found. Segment start and end must be set before incrementing splits.
            if (segments[splits].start) segments[splits].end = currentChar - 1; // SegmentStart cannot be set on iteration 0 so this is fine.
            else                        segments[splits].start = currentChar + 1;
            break;
        default: continue; break;
        }

        // Increment splits counter if this segment's start and end are set.
        if (segments[splits].start && segments[splits].end) {
            ++splits;
        }

        // break from loop if out of segments.
        if (splits >= segmentCount) {
            return OUT_OF_SEGMENTS;
        }
    }
    return -3;
}

int parseFaceIndicies(List* vi, List* ti, List* ni, uint16_t segmentCount, String* segments, uint16_t bufferSize, char* buffer) {
    // This function parses a wavefront file face data, adding it to the list of vertex indices, texture indicies, and normal indicies. 
    // Returns the number of vertices added. 

    for (uint8_t i = 0; i < segmentCount; ++i) {

        if (String_invalid(segments[i])) {
            break;
        }

        if (String_length(segments[i]) >= bufferSize) {
            return SEGMENT_TOO_LARGE;
        }

        String_clone_to_buffer(segments[i], buffer);




    }
}

int ParseLine (ObjParser* parser) {

    String segments[SEGMENT_COUNT];
    char buffer[BUFFER_SIZE];

    int errorcode = parseSplits(parser->buffer, SEGMENT_COUNT, (String*)&segments);

    if (errorcode) {
        return errorcode;
    }

    if (String_invalid(segments[0])) {
        return SEGMENT_MISSING_TAG;
    }

    // Figure out the tag, 
    // then do things depending on what the tag is.

    return 0;
}


StaticMesh* Object_StaticMesh_create_from_wave_front(const char* path, void* parent) {

    ObjParser parser = { 
        .materialCount = 0,
        .file = fopen(path, "r")
    };

    if (!parser.file) {
        return NULL;
    }

    StaticMesh* staticMesh = NULL;

    List_initialize(uint32_t, &parser.vi, 512);
    List_initialize(uint32_t, &parser.ni, 512);
    List_initialize(uint32_t, &parser.ti, 512);
    List_initialize(vec3, &parser.vertexList, 512);
    List_initialize(vec3, &parser.normalList, 512);
    List_initialize(vec2, &parser.tCoordList, 512);

    while (!feof(parser.file)) {
        fgets((char*)&parser.buffer, BUFFER_SIZE, parser.file);
        
        int errorcode = ParseLine(&parser);
        
        if (errorcode) {
            goto Deinitialize;
        }
    }

    // Upload the mesh and sub-meshes.
    // also handle correcting the indicies.

Deinitialize:
    List_deinitialize(&parser.vi);
    List_deinitialize(&parser.ni);
    List_deinitialize(&parser.ti);
    List_deinitialize(&parser.vertexList);
    List_deinitialize(&parser.normalList);
    List_deinitialize(&parser.tCoordList);
    return staticMesh;
}
