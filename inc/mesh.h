#pragma once

#include "object.h"
#include "list.h"
#include "gl_math.h"
#include "file_reader.h"
#include "string_utilities.h"

//Forward Definitions:
typedef struct Material Material;
typedef struct Mesh Mesh;
typedef struct List List;

typedef struct StaticMesh {
    /* Data structure to store mesh data. */
    OBJECT_BODY();
    Mesh* meshRenders;
    Material** materials;
    uint32_t MaterialCount;

} StaticMesh;


//StaticMesh* CreateStaticMeshFromRawData(const uint32_t* indeciesArray, const  Vector3* vertexBufferArray, const  Vector3* normalBufferArray, const  Vector2* tCoordArray, const  size_t indecies, const  size_t vertecies);
//StaticMesh* CreateStaticMeshFromGraphicsLibraryTransmissionFormat(const char* Path);
//StaticMesh* CreateStaticMeshFromGraphicsLibraryBinaryTransmissionFormat(const char* Path);
StaticMesh* CreateStaticMeshFromWavefront(const char* path);
//
//// Par-Shapes wrapers:
//StaticMesh* CreateStaticMeshPrimativeCone(int slices, int stacks);
//StaticMesh* CreateStaticMeshPrimativeCylinder(int slices, int stacks);
//StaticMesh* CreateStaticMeshPrimativeTorus(int slices, int stacks, float radius);
//StaticMesh* CreateStaticMeshPrimativePlane(int slices, int stacks);
//StaticMesh* CreateStaticMeshPrimativeSphere(int subdivisions);

// String parsing:

// Split line into segments by space character. 
void parseSplits(char* line, uint16_t MaxSegments, String* segments);

//void Vector2FromString(const char* data, vec2 out);
//void Vector3FromString(const char* data, vec3 out);

int parseFaceIndicies(List* vi, List* ti, List* ni, char* line);

//static void parseFace(std::vector<uint32_t>* vi, std::vector<uint32_t>* ti, std::vector<uint32_t>* ni, std::vector<std::string> segmentList);
//static int parseFaceIndicies(std::vector<uint32_t>* vi, std::vector<uint32_t>* ti, std::vector<uint32_t>* ni, const std::string data);
//static Vector2 Vector2FromString(const std::string data);
//static Vector3 Vector3FromString(const std::string data);

void Object_StaticMesh_destroy(StaticMesh* mesh);


