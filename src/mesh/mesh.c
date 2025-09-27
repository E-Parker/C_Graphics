#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"

#include "engine_core/string.h"
#include "engine_core/list.h"

#include "file_reader.h"
#include "renderable.h"
#include "mesh.h"

char segmentBuffer[0x100];

#define BUFFER_MAX_SIZE 0x1000

#define PackByte_uint16(a, b) ( ((uint16_t)b << 8) | (uint16_t)a )
#define PackByte_uint32(a, b, c, d) ( ((uint32_t)d << 24) | ((uint32_t)c << 16) | ((uint32_t)b << 8) | (uint32_t)a )
#define PackByte_uint64(a, b, c, d, e, f, g, h) ( ((uint64_t)h << 56) | ((uint64_t)g << 48) | ((uint64_t)f << 40) | ((uint64_t)e << 32) | ((uint64_t)d << 24) | ((uint64_t)c << 16) | ((uint64_t)b << 8) | (uint64_t)a )

// Supported File Formats:
#define ObjFile 0x6F002020206A626F      // .obj     - WaveFront
#define GlbFile 0x6700202020626C67      // .glb     - Graphics Library Transmission Format, as binary file
#define GltfFile 0x6700202066746C67     // .gltf    - Graphics Library Transmission Format
#define BinFile 0x000000006e69622e      // .bin     - Raw binary file
#define BlendFile 0x620020646E656C62    // .blend   - Blender project file
#define U3dFile 0x7500202020643375      // .u3d     - Universal 3D
#define UsdFile 0x7500202020647375      // .usd     - Universal Scene Descriptor
#define FbxFile 0x6600202020786266      // .fbx     - "Filmbox" format, Maya version.


void Object_StaticMesh_destroy(StaticMesh* mesh) {
    OBJECT_DESTROY_BODY(mesh);

    for (List_iterator(MeshRender, &mesh->meshRenders)) {
        FreeMesh(it);
    }

    List_deinitialize(&mesh->meshRenders);
    List_deinitialize(&mesh->materials);    // Materials are managed externally.
}


StaticMesh* Object_StaticMesh_create_empty(void* parent) {
    OBJECT_CREATE_BODY(StaticMesh, parent, Object_TypeStaticMesh);

    List_initialize(Material*, &object->materials, 1);
    List_initialize(MeshRender, &object->meshRenders, 1);

    Object_set_alias(object, "StaticMesh");
    object->Draw = Object_StaticMesh_Draw;
    object->Destroy = Object_StaticMesh_destroy;

    return object;
}


StaticMesh* Object_StaticMesh_create(const char* path, void* parent) {
    
    // Find the file extension.
    String pathString = String_from_ptr(path);
    char* ext = String_last(pathString, '.');

    if (!ext) {
        return NULL;
    }

    uint64_t fileExtentionPacked = 0;
    String fileExtentionPackedBytes = String_from_chars(fileExtentionPacked);
    String fileExtention = String_from_ptr(ext);
        
    String_clone_substring(fileExtention, fileExtentionPackedBytes, 0, String_length(fileExtention) - 1);
    String_as_lower(fileExtentionPackedBytes);
    
    switch (fileExtentionPacked) {
    case ObjFile:       return Object_StaticMesh_create_from_wave_front(path, parent);
    case GlbFile:       return Object_StaticMesh_create_from_graphics_library_binary_transmission_format(path, parent);
    case GltfFile:      return Object_StaticMesh_create_from_graphics_library_transmission_format(path, parent);
    case BinFile:       return Object_StaticMesh_create_from_raw_data(path, parent);
    case BlendFile:     return NULL;
    case U3dFile:       return NULL;
    case UsdFile:       return NULL;
    case FbxFile:       return NULL;
    default:            return Object_StaticMesh_create_empty(parent);
    };
    return NULL;
}



void Object_StaticMesh_Draw(void* object) {
    StaticMesh* staticMesh = (StaticMesh*)object;
    for (List_iterator(MeshRender, &staticMesh->meshRenders)) {
        DrawRenderable(it, *(Material**)List_at(&staticMesh->materials, it->materialIndex), staticMesh->Transform);
    }
}


void Object_StaticMesh_set_Material(StaticMesh* staticMesh, const uint32_t subMesh, const Material* material) {
    if (!staticMesh) {
        return;
    }

    MeshRender* mesh = (MeshRender*)List_at(&staticMesh->meshRenders, subMesh);

    if (!mesh) {
        return;
    }

    uint32_t materialCount = List_count(&staticMesh->materials);

    if (subMesh > materialCount) {
        return;
    }

    long int indexOf;
    if (List_contains_item(&staticMesh->materials, material, &indexOf)) {
        mesh->materialIndex = indexOf;
    }
    else {
        mesh->materialIndex = materialCount;
    }

    List_push_back(&staticMesh->materials, material);
}