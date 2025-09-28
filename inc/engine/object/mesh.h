#pragma once

#include "engine_core/string.h"
#include "engine_core/list.h"
#include "engine/object.h"

//Forward Definitions:
typedef struct Material Material;
typedef struct MeshRender MeshRender;

typedef struct StaticMesh {
    OBJECT_BODY();
    List meshRenders;
    List materials;
} StaticMesh;


StaticMesh* Object_StaticMesh_create_empty(void* parent);
StaticMesh* Object_StaticMesh_create(const char* path, void* parent);
StaticMesh* Object_StaticMesh_create_from_raw_data(const char* path, void* parent);
StaticMesh* Object_StaticMesh_create_from_wave_front(const char* path, void* parent);
StaticMesh* Object_StaticMesh_create_from_graphics_library_transmission_format(const char* Path, void* parent);
StaticMesh* Object_StaticMesh_create_from_graphics_library_binary_transmission_format(const char* Path, void* parent);

void Object_StaticMesh_set_Material(StaticMesh* staticMesh, const uint32_t subMesh, const Material* material);
void Object_StaticMesh_Draw(void* object);
