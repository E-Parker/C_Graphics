#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define LIST_IMPLEMENTATION
#include "engine_core/list.h"

#define HASH_TABLE_IMPLEMENTATION
#include "engine_core/string.h"
#include "engine_core/hash_table.h"

#include "engine_core/engine_types.h"

const u8 Object_TypeNone = 0x01;
const u8 Object_TypeCamera = 0x02;
const u8 Object_TypeDrawable = 0xF0;
const u8 Object_TypeStaticMesh = 0xF1;
const u8 Object_TypeSkinnedMesh = 0xF2;
const u8 Object_TypeText = 0xF3;