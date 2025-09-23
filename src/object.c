#include "stdlib.h"

#include "engine_core/list.h"
#include "engine/math.h"
#include "object.h"


const uint8_t Object_TypeNone          = 0x01;
const uint8_t Object_TypeStaticMesh    = 0x02;
const uint8_t Object_TypeSkinnedMesh   = 0x03;
const uint8_t Object_TypeText          = 0x04;
const uint8_t Object_TypeCamera        = 0x05;


uint8_t internal_Object_Initialize(void* objectPtr, void* parentPtr, const uint8_t type) {

    // Perform blind cast to object. All object types have the same header alignment so this is safe, assuming an object is passed in here.
    Object* object = (Object*)objectPtr;
    Object* parent = (Object*)parentPtr;
    uint8_t errorCode = ERRORCODE_OBJECT_SUCCESS;

    if (objectPtr == parentPtr) {
        errorCode = ERRORCODE_OBJECT_SELF_PARENT;
        goto ObjectInitFail;
    }

    if (!objectPtr) {
        errorCode = ERRORCODE_OBJECT_NULL_OBJECT;
        goto ObjectInitFail;
    }

<<<<<<< HEAD
    Object_SetAlias(object, "Empty");
=======
    for (uint8_t i = 0; i < OBJECT_ALIAS_SIZE; i++) {
        object->Alias[i] = '\0';
    };

>>>>>>> rewrite
    mat4_copy(MAT4_IDENTITY, object->Transform);
    
    object->Data.Flags = 0;
    object->Data.Type = type;
    object->Parent = parent;
    
    if (parent) {
        List_push_back(&parent->Children, object);
    }

    List_initialize(Object*, &object->Children, 16);

    object->Tick = internal_Object_TickDefault;
    object->Destroy = internal_Object_DestroyDefault;

ObjectInitFail:
    return errorCode;
}


void internal_Object_Deinitialize(void* objectPtr) {
    Object* object = (Object*)objectPtr;
    
    // TODO: come up with a better solution.
    // This is okay, but maybe sort of bad because recursion. 
    // Could potentially blow up the stack.
    for(uint64_t i = 0; i < List_count(&object->Children); i++) {
        Object* childObject;
        List_pop_front(&object->Children, childObject);
        childObject->Destroy(childObject);
    }
}


void internal_Object_DestroyDefault(void* objectPtr) {
    Object* object = (Object*)objectPtr;
    internal_Object_Deinitialize(objectPtr);
    free(objectPtr);
}


uint8_t internal_Object_TickDefault(void* ptr, const double deltaTime) {
    OBJECT_TICK_BODY(ptr)
    return ERRORCODE_OBJECT_SUCCESS;
}


void Object_set_alias(void* objectPtr, const char* string) {
    char* gameObjectAlias = (char*)objectPtr;
    char* currentCharacter = (char*)string;
    
    for(uint8_t i = 0; i < OBJECT_ALIAS_SIZE; i++) {
        if(currentCharacter == '\0') {
            gameObjectAlias[i] = '\0';
        }

        else {
            gameObjectAlias[i] = string[i];
            currentCharacter++;
        }
    }
}


void Object_get_world_space_transform(void* objectPtr, mat4 out) {
    Object* object = (Object*)objectPtr;
    Object* currentNode = object;

    List hiarachy;
    mat4 result;

    List_initialize(GLfloat*, &hiarachy, 128);
    mat4_copy(MAT4_IDENTITY, result);

    while (currentNode->Parent) {
        List_push_front(&hiarachy, currentNode->Transform);
    }

    for (List_iterator(GLfloat*, &hiarachy)) {
        mat4_multiply(result, *it, result);
    }

    mat4_copy(result, out);
}


void Object_set_parent(void* objectPtr, void* parentPtr) {
    Object* object = (Object*)objectPtr;
    Object* newParent = (Object*)parentPtr;
    Object* parent = object->Parent;

    if (object->Parent == newParent) {
        return;
    }

    if (parent) {
        List_remove_at(&parent->Children, object->internal_IndexOf);
        // TODO: look into maybe changing this to a less brute force method.
        uint64_t newChildIndex = 0;
        for (List_iterator(Object*, &parent->Children), newChildIndex++) {
            (*it)->internal_IndexOf = newChildIndex;
        }
    }
    
    if (newParent) {
        object->internal_IndexOf = List_count(&newParent->Children);
        List_push_back(&newParent->Children, object);
    }

    object->Parent = newParent;
}


bool Object_flag_compare(uint32_t data, uint32_t mask) {
    data |= 0xff000000;
    mask |= 0xff000000;
    return ((data & mask) == mask) & 1;
}

void Object_flag_set(uint32_t* data, uint32_t mask) {
    *data |= mask;
}

void Object_flag_unset(uint32_t* data, uint32_t mask) {
    mask &= 0x00ffffff;
    *data &= ~mask;
}
