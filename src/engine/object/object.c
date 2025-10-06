#include "engine_core/engine_types.h"
#include "engine_core/engine_error.h"
#include "engine_core/list.h"
#include "engine_core/string.h"

#include "engine/math.h"
#include "engine/object.h"

List RootObjectList;

ecode InitObjects() {
    List_initialize(Object*, &RootObjectList, 0x40);
    return 0;
}

ecode internal_Object_Initialize(void* objectPtr, void* parentPtr, const u8 type) {

    // Perform blind cast to object. All object types have the same header alignment so this is safe, assuming an object is passed in here.
    Object* object = (Object*)objectPtr;
    Object* parent = (Object*)parentPtr;
    ecode errorCode = 0;

    if (objectPtr == parentPtr) {
        errorCode = ERROR_OBJECT_SELF_PARENT;
        goto ObjectInitFail;
    }

    if (!objectPtr) {
        errorCode = ERROR_OBJECT_NULL_OBJECT;
        goto ObjectInitFail;
    }

    for (u8 i = 0; i < OBJECT_ALIAS_SIZE; i++) {
        object->Alias[i] = '\0';
    };

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
    object->Draw = NULL;

    Engine_log_errorcode(errorCode);
    
ObjectInitFail:
    return errorCode;
}


void internal_Object_Deinitialize(void* objectPtr) {
    Object* object = (Object*)objectPtr;
    
    // TODO: come up with a better solution.
    // This is okay, but maybe sort of bad because recursion. 
    // Could potentially blow up the stack.
    for(u64 i = 0; i < List_count(&object->Children); i++) {
        Object* childObject;
        List_pop_front(&object->Children, childObject);
        childObject->Destroy(childObject);
    }
}


void internal_Object_DestroyDefault (void* objectPtr) {
    Object* object = (Object*)objectPtr;
    internal_Object_Deinitialize(objectPtr);
    free(objectPtr);
}


ecode internal_Object_TickDefault (void* ptr, const double deltaTime) {
    OBJECT_TICK_BODY(ptr)
    return 0;
}


void Object_set_alias (void* objectPtr, const char* string) {
    char* gameObjectAlias = (char*)objectPtr;
   
    String aliasString = String_from_ptr_size(gameObjectAlias, OBJECT_ALIAS_SIZE);
    String newAliasString = String_from_ptr(string);
    
    if (String_length(newAliasString) >= OBJECT_ALIAS_SIZE) {
        Engine_log_errorcode(ERROR_OBJECT_NAMETOBIG);
        return;
    }

    String_clone(newAliasString, aliasString);
}


void Object_get_world_space_transform (void* objectPtr, mat4 out) {
    Object* object = (Object*)objectPtr;
    Object* currentNode = object;

    List hiarachy;
    mat4 result;

    List_initialize(GLfloat*, &hiarachy, 0x40);
    mat4_copy(MAT4_IDENTITY, result);

    while (currentNode->Parent) {
        List_push_front(&hiarachy, currentNode->Transform);
    }

    for (List_iterator(GLfloat*, &hiarachy)) {
        mat4_multiply(result, *it, result);
    }

    List_deinitialize(&hiarachy);

    mat4_copy(result, out);
}


void Object_set_parent (void* objectPtr, void* parentPtr) {
    Object* object = (Object*)objectPtr;
    Object* newParent = (Object*)parentPtr;
    Object* parent = object->Parent;

    if (object->Parent == newParent) {
        return;
    }

    if (parent) {
        List_remove_at(&parent->Children, object->internal_IndexOf);
        // TODO: look into maybe changing this to a less brute force method.
        u64 newChildIndex = 0;
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


bool Object_flag_compare (u32 data, u32 mask) {
    data |= 0xff000000;
    mask |= 0xff000000;
    return ((data & mask) == mask) & 1;
}

void Object_flag_set (u32* data, u32 mask) {
    *data |= mask;
}

void Object_flag_unset (u32* data, u32 mask) {
    mask &= 0x00ffffff;
    *data &= ~mask;
}
