#include <stdint.h>
#include <string.h>
#include "asset.h"
#include "gl_types.h"
#include "fixed_list.h"


void internal_Object_Initialize(void* ptr, void* parent, const uint8_t type) {
    // Set default values of an object.
    // Should always be called at the start of any custom object factory function.
    //
    
    // Ensure that the parent is not itself to prevent circular references.
    if (ptr == parent) {
        parent = NULL;
    }

    Object* object = (Object*)ptr;

    Object_SetAlias(object, "EmptyObject");
    memcpy(object->Transform, MAT4_IDENTITY, 64);   
    
    object->Data.Flags = 0;
    object->Data.Type = type;

    object->Parent = (Object*)parent;
    object->Children = FixedList_create(Object*, maximumChildren);
    object->Destory = internal_Object_Destroy; 
}


void internal_Object_Deinitialize(void* ptr) {
    // Destroys the chain of child objects attached to this object.
    //
    //
    
    // Get this object as an Object*
    Object* object = (Object*)ptr;
    
    // For every child of this object, call its destroy function.
    for(uint64_t i = 0; i < FixedList_size(object->Children); i++) {
        Object* childObject = (Object*)FixedList_pop_front(object->Children);
        childObject->Destory(childObject);
    }
    
    // Now that all children objects have been destroyed, it is safe to free this object.
}

void internal_Object_DestroyDefault(void* ptr) {
    // Default destroy function for objects.
    
    Object* object = (Object*)ptr;
    internal_Object_Deinitialize(ptr);
    free(ptr);
    
    
}

void internal_Object_TickDefault(void* ptr, const double deltaTime) {
    // Default tick function for objects.
}

void Object_SetAlias(void* gameObject, const char* string) {
    /* copy the string into the gameObject alias buffer. */
    
    char* gameObjectAlias = (char*)gameObject;
    
    // Clear the last alias.
    memset(gameObjectAlias, '\0', STANDARD_BUFFER_SIZE);
    
    // Set the characters in the alias up to the null terminator of the input string.
    // Will only copy up to STANDARD_BUFFER_SIZE characters.
    for(uint8_t i = 0; i < STANDARD_BUFFER_SIZE; i++) {
        if(string[i] == '\0') {
            return;
        }
        gameObjectAlias[i] = string[i];
    }
}


Matrix GetGlobalTransform(void* gameObject) {
    /* This function returns the global transform of any asset. */ 
    mat4 result = *Object_GetTransform(gameObject);
    void* parentObject = NULL;
    
    for(uint16_t i = 0; i < 512; i++) {
        parentObject = Object_GetParent(gameObject);
        
        if(parentObject == NULL) {
            return result;
        }

        mat4mul(result, *Object_GetTransform(parentObject), result);
         
    }
    return result;
}


