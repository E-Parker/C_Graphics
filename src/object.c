#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "gl_math.h"
#include "list.h"
#include "object.h"


const uint8_t Object_TypeNone          = 0x01;
const uint8_t Object_TypeStaticMesh    = 0x02;
const uint8_t Object_TypeSkinnedMesh   = 0x03;
const uint8_t Object_TypeText          = 0x04;
const uint8_t Object_TypeCamera        = 0x05;


uint8_t internal_Object_Initialize(void* objectPtr, void* parentPtr, const uint8_t type) {
    // Set default values of an object.
    // Should always be called at the start of any custom object factory function.
    //
    
    uint8_t errorCode = ERRORCODE_OBJECT_SUCCESS;


    // Perform blind cast to object. All object types have the same header alignment so this is safe, assuming an object is passed in here.
    Object* object = (Object*)objectPtr;
    Object* parent = (Object*)parentPtr;


    // Ensure that the parent is not itself to prevent circular references. This is considered non-fatal.
    if (objectPtr == parentPtr) {
        parent = NULL;
        errorCode = ERRORCODE_OBJECT_SELF_PARENT;
    }

    // Something has gone wrong, trying to initialize an object which is null.
    if (!objectPtr) {
        return ERRORCODE_OBJECT_NULL_OBJECT;
    }

    Object_SetAlias(object, "EmptyObject");
    mat4_copy(MAT4_IDENTITY, object->Transform);
    
    object->Data.Flags = 0;
    object->Data.Type = type;
    object->Parent = parent;
    
    if (parent) {
        List_push_front(parent->Children, object);
    }

    object->Children = List_create(Object*, 16);

    object->Tick = internal_Object_TickDefault;
    object->Destroy = internal_Object_DestroyDefault;

    return errorCode;
}


void internal_Object_Deinitialize(void* objectPtr) {
    // Destroys the chain of child objects attached to this object.
    //
    //

    // Get this object as an Object*
    Object* object = (Object*)objectPtr;
    
    // For every child of this object, call it's destroy function.
    for(uint64_t i = 0; i < List_count(object->Children); i++) {
        Object* childObject;
        List_pop_front(object->Children, childObject);
        childObject->Destroy(childObject);
    }
    
    // Now that all children objects have been destroyed, it is safe to free this object.
}

void internal_Object_DestroyDefault(void* objectPtr) {
    // Default destroy function for objects.
    Object* object = (Object*)objectPtr;
    internal_Object_Deinitialize(objectPtr);
    free(objectPtr);;
}

uint8_t internal_Object_TickDefault(void* ptr, const double deltaTime) {
    // Default tick function for objects.
    return ERRORCODE_OBJECT_SUCCESS;
}

void Object_SetAlias(void* objectPtr, const char* string) {
    /* copy the string into the gameObject alias buffer. */
    
    char* gameObjectAlias = (char*)objectPtr;
    
    // Clear the last alias.
    memset(gameObjectAlias, '\0', OBJECT_ALIAS_SIZE);
    
    // Set the characters in the alias up to the null terminator of the input string.
    // Will only copy up to STANDARD_BUFFER_SIZE characters.
    for(uint8_t i = 0; i < OBJECT_ALIAS_SIZE; i++) {
        if(string[i] == '\0') {
            return;
        }
        gameObjectAlias[i] = string[i];
    }
}


void Object_GetGlobalTransform(void* object, mat4 out) {
    /* This function returns the global transform of any asset. */
    
    Object* gameObject = (Object*)object;
    Object* parentObject;
    
    mat4 result;

    mat4_copy(result, gameObject->Transform);
    
    // accumulatively multiply the transform down the chain to the root node. 
    for(uint16_t i = 0; i < 512; i++) {
        parentObject = gameObject->Parent;
        
        if(parentObject == NULL) {
            break; 
        }
    
        mat4_multiply(result, parentObject->Transform, result);
    }
    mat4_copy(out, result);
}


bool Object_IsChildOf(void* objectPtr, void* parentPtr, uint32_t* out) {
    // Simple brute-force linear search. This fucking blows, but is probably better than maintaining a second array of references.
    Object* object = (Object*)objectPtr;
    Object* parent = (Object*)parentPtr;

    uint32_t indexOf = 0;

    // Iterate through children list, if a match is found copy index to out and return true.
    for List_iterator(Object*, parent->Children) {
        if (*it == object) {
            if (out) *out = indexOf;
            return true;
        }
        ++indexOf;
    }

    if (out) *out = 0;
    return false;
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


void Object_set_parent(void* objectPtr, void* parentPtr) {
    Object* object = (Object*)objectPtr;
    Object* objectNewParent = (Object*)parentPtr;
    Object* objectOldParent = object->Parent;

    // object already has a parent, remove from old parent child list.
    if (objectOldParent) {
        uint32_t indexOf;
        if (Object_IsChildOf(object, objectOldParent, &indexOf)) {
            List_remove_at(objectOldParent->Children, indexOf);
        }
    }
    
    // if there is a new parent, and the object is not already a child of that parent, append to the list.
    if (objectNewParent && !Object_IsChildOf(object, objectNewParent, NULL)) {
        List_push_front(objectNewParent->Children, object);
    }
    
    object->Parent = objectNewParent;
}

