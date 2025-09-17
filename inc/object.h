#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifndef OBJECT_INCLUDED
#define OBJECT_INCLUDED

// Object function return messages:

#define ERRORCODE_OBJECT_SUCCESS          0x00
#define ERRORCODE_OBJECT_MISSING_PARENT   0x01
#define ERRORCODE_OBJECT_SELF_PARENT      0x02
#define ERRORCODE_OBJECT_NULL_OBJECT      0x03

#include <glad/glad.h>
#include <stdbool.h>
#include <stdint.h>

#include "engine_core/list.h"
#include "engine/math.h"

// Standard Buffer Size is the maximum size any alias can be.
#define OBJECT_ALIAS_SIZE 28

// Type Definitions:
// 
//

// Define a function signature used to create and destroy objects.
typedef uint8_t (*Object_TickFunction)(void*, const double);
typedef void    (*Object_DestroyFunction)(void*);

typedef struct Object Object;

typedef struct Object {
    // Holds basic information that all objects in a scene will have. Instead of using inheritance & polymorphism, which has some 
    // overhead, pass objects as void* then static cast to the actual type. This has some advantages in that any generic operation, 
    // like moving, rotating, scaling, etc can be done without a class-specific override. This also allows for more frequent cache hits
    // because the data for an asset is always going to be exactly 128 bytes
    //
    // The union is used because by default, the c preprocessor will try to pack things into 4 bytes. 
    //
    //                                  Offset  | Size in bytes    
    #define OBJECT_BODY() \
    char Alias[OBJECT_ALIAS_SIZE];          /* 0    |   28      <----- These will be the same for any object.                       */  \
    union {uint8_t Type;                    /* 28   |   x           _- Only use the lower 24 bits, the first 8 represent type.      */  \
    uint32_t Flags;} Data;                  /* 28   |   4       <--+-- General purpose bit flags. useful for keeping object state.  */  \
    mat4 Transform;                         /* 32   |   64      <----- 4 * 4 matrix, represents the local position.                 */  \
    Object* Parent;                         /* 96   |   8       <----- pointer to the parent node. If NULL, assumed to be a root.   */  \
    List* Children;                         /* 104  |   8       <----- pointer to array of child nodes.                             */  \
    Object_TickFunction Tick;               /* 112  |   8       <----- function to update the object.                               */  \
    Object_DestroyFunction Destroy          /* 120  |   8       <----- function to destroy the object.                              */  \

    OBJECT_BODY();

} Object;

extern const uint8_t Object_TypeNone;
extern const uint8_t Object_TypeStaticMesh;
extern const uint8_t Object_TypeSkinnedMesh;
extern const uint8_t Object_TypeText;
extern const uint8_t Object_TypeCamera;

#define OBJECT_CREATE_BODY(object, parent, type) internal_Object_Initialize((void*)object, (void*)parent, type);
#define OBJECT_DESTROY_BODY(object) internal_Object_Deinitialize((Object*)object);

#define Object_IsType(object, Type) ((*((Object*)object)->Type) == Type)

uint8_t internal_Object_Initialize(void* objectPtr, void* parentPtr, const uint8_t type);
void internal_Object_Deinitialize(void* objectPtr);
void internal_Object_DestroyDefault(void* objectPtr);
uint8_t internal_Object_TickDefault(void* objectPtr, const double deltaTime);

void Object_SetAlias(void* objectPtr, const char* string);
void Object_GetGlobalTransform(void* objectPtr, mat4 out);

bool Object_IsChildOf(void* objectPtr, void* parentPtr, uint32_t* out);

bool Object_flag_compare(uint32_t data, uint32_t mask);
void Object_flag_set(uint32_t* data, uint32_t mask);
void Object_flag_unset(uint32_t* data, uint32_t mask);

void Object_set_parent(void* objectPtr, void* parentPtr);
#endif

#ifdef __cplusplus
}
#endif


