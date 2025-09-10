#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "list.h"
#include "string_utilities.h"

#include "renderable.h"
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

int parseSplits(char* line, uint16_t MaxSegments, String* segments) {
    
    uint16_t splits = 0;
    char* currentChar = line;

    for (uint16_t i = 0; i < 0xffff; ++i, ++currentChar) {
        // Check the current character.
        switch ((int)(*currentChar)) {
        case '\0': return 1;// End of file reached early.
        case '\n':          // End of line reached. 
            segments[splits].bufferEnd = currentChar - 1;
            i = 0xffff;     // Set "i" to max short so for loop exits.
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
    return 0;
}


int parseFaceIndicies(List* vi, List* ti, List* ni, char* line) {
    // This function parses a wavefront file face data, adding it to the list of vertex indices, texture indicies, and normal indicies. 
    // Returns the number of vertices added. 

    if (line == NULL) return 0;
    
    String segments[0x6];
    
    parseSplits(line, 0x6, (String*)&segments);

    for (uint8_t i = 0; i < 0x6; ++i) {

    }


}

StaticMesh* CreateStaticMeshCube() {
    GLfloat vertices[] = {
        0, 0, 0,
        0, 1, 0,
        1, 1, 0,
        1, 0, 0,
        0, 0, 1,
        0, 1, 1,
        1, 1, 1,
        1, 0, 1,
    };

    GLfloat tchoord[] = {
        1, 1,
        1, 1,
        1, 1,
        1, 1,
        1, 1,
        1, 1,
        1, 1,
        1, 1,
    };

    uint32_t quads[] = {
        7,6,5,4, // front
        0,1,2,3, // back
        6,7,3,2, // right
        5,6,2,1, // top
        4,5,1,0, // left
        7,4,0,3, // bottom
    };
    
    uint32_t tris[6 * 6] = {0,};
    
    uint32_t* tri = (uint32_t*)&tris;
    uint32_t* quad = (uint32_t*)&quads;

    for (uint32_t i = 0; i < 6; ++i, quad += 4) {
        *tri++ = quad[0];
        *tri++ = quad[1];
        *tri++ = quad[2];
        *tri++ = quad[2];
        *tri++ = quad[3];
        *tri++ = quad[0];
    }

    

    StaticMesh* staticMesh = (StaticMesh*)malloc(sizeof(StaticMesh));
    staticMesh->meshRenders = (Mesh*)calloc(1, sizeof(Mesh));
    
    Mesh_Upload(staticMesh->meshRenders, (uint32_t*)vertices, (GLfloat*)tris, (GLfloat*)tris, (GLfloat*)tchoord, (sizeof(quads) << 1), 8);
    return staticMesh;
}


StaticMesh* CreateStaticMeshFromWavefront(const char* path) {
    
    StaticMesh* staticmesh = (StaticMesh*)malloc(sizeof(StaticMesh));
    staticmesh->meshRenders = (Mesh*)calloc(1, sizeof(Mesh));

    staticmesh->materials = NULL;
    staticmesh->MaterialCount = 0;

    uint32_t index[3] = {0, 1, 2};
    GLfloat verticies[9] = {1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f ,0.0f, 0.0f};
    GLfloat normals[9] = {1.0f, 0.0f, 0.0f,1.0f, 0.0f, 0.0f,1.0f, 0.0f, 0.0f};
    GLfloat tchoord[6] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    
    Mesh_Upload(staticmesh->meshRenders, (uint32_t*)index, (GLfloat*)verticies, (GLfloat*)normals, (GLfloat*)tchoord, 1, 3);

    return staticmesh;
    
}


void Object_StaticMesh_destroy(StaticMesh* mesh) {
    OBJECT_DESTROY_BODY(mesh);


    Mesh_Free(mesh->meshRenders);
    
    free(mesh->meshRenders);
    free(mesh);

}


