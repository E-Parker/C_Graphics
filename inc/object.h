#pragma once

#include <glad/glad.h>
#include <stdint.h>

#include "fixed_list.h"
#include "gl_types.h"

// Standard Buffer Size is the maximum size any alias can be.
#define STANDARD_BUFFER_SIZE 20

#define Object_isType(object, Type) ((*((Object*)object)->Type) == Type)
#define Object_GetTransform(object) ((GLfloat*)(((Object*)object)->Transform))
#define Object_GetParent(object) (((Object*)object)->Parent)

// Define a function signature used to create and destroy objects.
typedef void (*Object_TickFunction)(void*, const double);
typedef void (*Object_DestroyFunction)(void*);


typedef struct Object {
    // Holds basic information that all objects in a scene will have. Instead of using inheritance & polymorphism, which has some 
    // overhead, pass objects as void* then static cast to the actual type. This has some advantages in that any generic operation, 
    // like moving, rotating, scaling, etc can be done without a class-specific override. This also allows for more frequent cache hits
    // because the data for an asset is always going to be exactly 128 bytes
    //
    // The union is used because by default, the c preprocessor will try to pack things into 4 bytes. 
    //
    //                                  Offset  | Size in bytes    
    #define OBJECT_BODY\
    char Alias[STANDARD_BUFFER_SIZE]{'\0'}; /* 0    |   20      <--+-- These will be the same for any object.                       */  \
    FixedList* References;                  /* 20   |   8       <-/ <- Array of other objects that reference this one.              */  \
    union Data {uint8_t Type;               /* 28   |   x           _- Only use the lower 24 bits, the first 8 represent type.      */  \
    uint32_t Flags;};                       /* 28   |   4       <--+-- General purpose bit flags. useful for keeping object state.  */  \
    mat4 Transform;                         /* 32   |   64      <----- 4 * 4 matrix, represents the local position.                 */  \
    Object* Parent;                         /* 96   |   8       <----- pointer to the parent node. If NULL, assumed to be a root.   */  \
    FixedList* Children;                    /* 104  |   8       <----- pointer to array of child nodes.                             */  \
    Object_TickFunction Tick;               /* 112  |   8       <----- function to update the object.                               */  \
    Object_DestroyFunction Destroy;         /* 120  |   8       <----- function to destroy the object.                              */  \

    OBJECT_BODY;

} Object;

const uint8_t Object_TypeNone          = 0x01;
const uint8_t Object_TypeStaticMesh    = 0x02;
const uint8_t Object_TypeSkinnedMesh   = 0x03;
const uint8_t Object_TypeText          = 0x04;
const uint8_t Object_TypeCamera        = 0x05;

#define OBJECT_CREATE_BODY(object, parent, type) internal_Object_initialize((void*)object, (void*)parent, type);
#define OBJECT_DESTROY_BODY(object) internal_Object_deinitialize((Object*)object);

void internal_Object_Initialize(void* ptr, void* parent, const uint8_t type);
void internal_Object_Deinitialize(void* ptr);

void internal_Object_DestroyDefault(void* ptr);
void internal_Object_TickDefault(void* ptr, const double deltaTime);

void Object_SetAlias(void* gameObject, const char* string);
void Object_GetGlobalTransform(void* gameObject, mat4 out); 

int Object_flag_compare(uint32_t data, uint32_t mask);
void Object_flag_set(uint32_t data, uint32_t mask);
void Object_flag_unset(uint32_t data, uint32_t mask);

