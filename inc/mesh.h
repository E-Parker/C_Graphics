#pragma once

#ifdef __cplusplus
extern "C" {
#endif


#include "object.h"

//Forward Definitions:
struct Material;
struct Mesh;

struct StaticMesh {
    /* Data structure to store mesh data. */
    OBJECT_BODY();
    Mesh* meshRenders;
    Material** materials;
    uint32_t MaterialCount;

};

//StaticMesh* CreateStaticMeshFromRawData(const uint32_t* indeciesArray, const  Vector3* vertexBufferArray, const  Vector3* normalBufferArray, const  Vector2* tCoordArray, const  size_t indecies, const  size_t vertecies);
//StaticMesh* CreateStaticMeshFromGraphicsLibraryTransmissionFormat(const char* Path);
//StaticMesh* CreateStaticMeshFromGraphicsLibraryBinaryTransmissionFormat(const char* Path);
//StaticMesh* CreateStaticMeshFromWavefront(const char* path);
//
//// Par-Shapes wrapers:
//StaticMesh* CreateStaticMeshPrimativeCone(int slices, int stacks);
//StaticMesh* CreateStaticMeshPrimativeCylinder(int slices, int stacks);
//StaticMesh* CreateStaticMeshPrimativeTorus(int slices, int stacks, float radius);
//StaticMesh* CreateStaticMeshPrimativePlane(int slices, int stacks);
//StaticMesh* CreateStaticMeshPrimativeSphere(int subdivisions);

// String parsing:
//static void parseFace(std::vector<uint32_t>* vi, std::vector<uint32_t>* ti, std::vector<uint32_t>* ni, std::vector<std::string> segmentList);
//static int parseFaceIndicies(std::vector<uint32_t>* vi, std::vector<uint32_t>* ti, std::vector<uint32_t>* ni, const std::string data);
//static Vector2 Vector2FromString(const std::string data);
//static Vector3 Vector3FromString(const std::string data);


#ifdef __cplusplus
}
#endif

