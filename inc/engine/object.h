#pragma once

#include "glad/glad.h"

#include "engine_core/engine_types.h"
#include "engine_core/list.h"
#include "engine/math.h"

// Standard Buffer Size is the maximum size any alias can be.
#define OBJECT_ALIAS_SIZE 116


// Type Definitions:
// 
//

// TODO: Implemented "GetComponent" with ObjectType as input. This will be the basis for polymorphic data.


typedef struct Object {
    // Holds basic information that all objects in a scene will have. Instead of using inheritance & polymorphism, which has some 
    // overhead, pass objects as void* then static cast to the actual type. This has some advantages in that any generic operation, 
    // like moving, rotating, scaling, etc can be done without a class-specific override.
    //
    // The union is used because by default, the c preprocessed will try to pack things into 4 bytes. 
    //  
    #define OBJECT_BODY() \
    char Alias[OBJECT_ALIAS_SIZE];          /* <----- These will be the same for any object.                        */ \
    union {u8 Type;                         /*     _- Only use the lower 8 bits, the first 8 represent type.        */ \
    u32 Flags;} Data;                       /* <--+-- General purpose bit flags. useful for keeping object state.   */ \
    mat4 Transform;                         /* <----- 4 * 4 matrix, represents the position & rotation.             */ \
    List Children;                          /* <----- list of child objects.                                        */ \
    u64 internal_IndexOf;                   /* <----- Index of the object in it's parent's children list.           */ \
    Object* Parent;                         /* <----- pointer to the parent node. If NULL, assumed to be a root.    */ \
    Function_Tick Tick;                     /* <----- function to update the object.                                */ \
    Function_Void_OneParam Draw;            /* <----- function to draw the object.                                  */ \
    Function_Void_OneParam Destroy          /* <----- function to destroy the object.                               */ \

    OBJECT_BODY();

} Object;

ecode InitObjects();

#define OBJECT_CREATE_BODY(T, parent, type) T* object = (T*)malloc(sizeof(T)); if(!object) { return NULL; } internal_Object_Initialize((void*)object, (void*)parent, type);
#define OBJECT_DESTROY_BODY(object) internal_Object_Deinitialize((void*)object);
#define OBJECT_TICK_BODY(object) 

#define Object_IsType(object, Type) ((*((Object*)object)->Type) == Type)

Object* Object_create(void* parent);

ecode   internal_Object_Initialize (void* objectPtr, void* parentPtr, const u8 type);
void    internal_Object_Deinitialize (void* objectPtr);
void    internal_Object_DestroyDefault (void* objectPtr);
ecode   internal_Object_TickDefault (void* objectPtr, const double deltaTime);

void Object_get_world_space_transform (void* objectPtr, mat4 out);
void Object_set_parent (void* objectPtr, void* parentPtr);
void Object_set_alias (void* objectPtr, const char* string);

bool Object_flag_compare (u32 data, u32 mask);
void Object_flag_set (u32* data, u32 mask);
void Object_flag_unset (u32* data, u32 mask);

