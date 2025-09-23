#pragma once

#include "engine_core/string.h"
#include "engine_core/list.h"
#include "object.h"

//Forward Definitions:
typedef struct Material Material;
typedef struct MeshRender MeshRender;

typedef struct StaticMesh {
    OBJECT_BODY();
    List meshRenders;
    List materials;
} StaticMesh;


<<<<<<< HEAD

StaticMesh* CreateStaticMeshCube();

//StaticMesh* CreateStaticMeshFromRawData(const uint32_t* indeciesArray, const  Vector3* vertexBufferArray, const  Vector3* normalBufferArray, const  Vector2* tCoordArray, const  size_t indecies, const  size_t vertecies);
=======
StaticMesh* Object_StaticMesh_create_empty(void* parent);
StaticMesh* Object_StaticMesh_create(const char* path, void* parent);
StaticMesh* Object_StaticMesh_create_from_raw_data(const char* path, void* parent);
StaticMesh* Object_StaticMesh_create_from_wave_front(const char* path, void* parent);
>>>>>>> rewrite
//StaticMesh* CreateStaticMeshFromGraphicsLibraryTransmissionFormat(const char* Path);
//StaticMesh* CreateStaticMeshFromGraphicsLibraryBinaryTransmissionFormat(const char* Path);

void Object_StaticMesh_set_Material(StaticMesh* staticMesh, const uint32_t subMesh, const Material* material);
void Object_StaticMesh_Draw(void* object);

// Split line into segments by space character. 
int parseSplits(char* line, uint16_t MaxSegments, String* segments);

//void Vector2FromString(const char* data, vec2 out);
//void Vector3FromString(const char* data, vec3 out);

int parseFaceIndicies(List* vi, List* ti, List* ni, char* line);




