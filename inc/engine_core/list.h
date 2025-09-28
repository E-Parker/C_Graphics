// List implementation by Ethan Parker.
//
// To use this library, add a source file with these two lines.
// #define LIST_IMPLEMENTATION
// #include "list.h"
//
// For documentation please see README.md  
//

#pragma once

#include "engine_core/engine_types.h"

//#define LIST_USE_MEMCPY

#ifdef LIST_USE_MEMCPY
#define internal_list_copy(dst, src, size) memcpy(dst, src, size)
#else
#define internal_list_copy(dst, src, size) for (u32 internal_list_copy_iterator = 0; internal_list_copy_iterator < (u32)size; ++internal_list_copy_iterator) { ((u8*)(dst))[internal_list_copy_iterator] = ((u8*)(src))[internal_list_copy_iterator]; }
#endif


// Type Definitions:
// 
//

typedef struct List {
    /* Simple list container, using a circular buffer. */
    u32 capacity;   // Current capacity of the list.
    u32 itemSize;   // Size in bytes of an element.
    u8* head;       // Pointer to the "head" of the list.
    u8* tail;       // Pointer to the "tail" of the list.
    u8* data;       // Pointer to the data block of "head" and "tail".
} List;

    // List Functions:
    // 
    //

#define List_create(T, capacity) internal_List_create(sizeof(T), capacity)
List* internal_List_create(const u32 ItemSize, const u32 Capacity);

#define List_initialize(T, list, capacity) internal_List_initialize(list, sizeof(T), capacity)
void internal_List_initialize(List* list, const u32 ItemSize, const u32 Capacity);

void List_destroy(List** list);
void List_deinitialize(List* list);

List* List_create_subset(List* list, u32 start, u32 end);

    // These macros are for manipulating the head and tail pointers, wrapping around the List's data region.
    // If getting the next slot would overrun the buffer, instead loop around to the start of the buffer. Otherwise, increment by ItemSize.
    //

#define List_isEmpty(list) ((list)->head == (list)->tail)
#define internal_List_start(T, list) ((T*)(list)->data)
#define internal_List_end(T, list) (T*)((internal_List_start(u8, (list))) + ((list)->capacity * (list)->itemSize))
#define internal_List_getNextPtr(T, list, ptr) ptr = (T*)(((u8*)ptr == internal_List_end(u8, list) - (list)->itemSize)? internal_List_start(u8, list) : (u8*)ptr + (list)->itemSize)
#define internal_List_getPrevPtr(T, list, ptr) ptr = (T*)(((u8*)ptr == internal_List_start(u8, list))? internal_List_end(u8, list) - (list)->itemSize : (u8*)ptr - (list)->itemSize)
#define internal_List_validate(list) assert((list)->head != (list)->tail)

// Creates T* variable, it, which is the current item in the loop. This will be a little faster than using List_at().
// If you are using function pointers, de-reference it before calling.
#define List_iterator(T, list) T* it = (T*)(list)->tail; (u8*)it != (list)->head; internal_List_getNextPtr(T, list, it)
//#define List_iterator_to(T, list, end) T* it = (T*)(list)->head; (void*)it != List_at(list, (u32)end); internal_List_getPrevPtr(T, list, it)
//#define List_iterator_from(T, list, start) T* it = (T*)List_at(list, start); (u8*)it != (list)->tail; internal_List_getPrevPtr(T, list, it)
//#define List_iterator_range(T, list, start, end) T* it = (T*)List_at(list, start); (void*)it != List_at(list, (u32)end); internal_List_getPrevPtr(T, list, it) 

#define List_push_front(list, Data) internal_List_push_front((list), (void*)&Data)
#define List_push_back(list, Data) internal_List_push_back((list), (void*)&Data)
#define List_peak_front(list, outVal) internal_List_peak_front((list), (void*)&outVal)
#define List_peak_back(list, outVal) internal_List_peak_back((list), (void*)&outVal)
#define List_pop_front(list, outVal) internal_List_pop_front((list), (void*)&outVal)
#define List_pop_back(list, outVal) internal_List_pop_back((list), (void*)&outVal)

#define List_set(list, template, count) internal_List_set(list, (void*)&template, count)

void internal_List_push_front(List* list, void* data);
void internal_List_push_back(List* list, void* data);
void internal_List_pop_front(List* list, void* outVal);
void internal_List_pop_back(List* list, void* outVal);
void internal_List_peak_front(List* list, void* outVal);
void internal_List_peak_back(List* list, void* outVal);

void internal_List_set(List* list, void* template, const u32 count);

void* List_create_array(List* list);

u32 List_count(const List* list);
u32 List_byte_count(const List* list);
bool List_contains_item(const List* list, void* item, i64* out);

void List_realloc(List* list, u32 Capacity);
void List_reorder(List* list);

void List_remove_at(List* list, const u32 index);
void* List_at(const List* list, const u32 index);

void List_append(List* dst, List* src);


#ifdef LIST_IMPLEMENTATION

List* internal_List_create(const u32 ItemSize, const u32 Capacity) {
    List* newList = (List*)malloc(sizeof(List));

    internal_List_initialize(newList, ItemSize, Capacity);
    return newList;
}


List* List_create_subset(List* list, u32 start, u32 end) {
    // Creates a new list as a subset of another list.
    // Returns NULL if the subset cannot be created.
    //

    // cannot create subset with start "in-front of" end.
    if (start >= end) return NULL;

    u32 listCount = List_count(list);

    // cannot create subset which is outside of the bounds of the original list.
    if (end > listCount || start >= listCount) return NULL;

    u32 capacity = end - start;                    // capacity needed to store the number of items in the subset.
    u32 capacityBytes = capacity * list->itemSize; // capacity needed in bytes.

    void* subsetArray = malloc(capacityBytes);
    void* listArray = List_create_array(list);
    internal_list_copy(subsetArray, listArray, capacityBytes);

    List* subset = (List*)malloc(sizeof(List));

    subset->capacity = capacity;
    subset->itemSize = list->itemSize;
    subset->data = (u8*)subsetArray;
    subset->head = (u8*)subsetArray;
    subset->tail = (u8*)subsetArray;

    return subset;
}


void internal_List_initialize(List* list, const u32 ItemSize, const u32 Capacity) {
    // initialize default values of any fixedList.
    //
    //

    list->data = (u8*)calloc(ItemSize, Capacity);
    //list->data = (u8*)malloc(ItemSize * Capacity);

    // Set capacity and size.
    list->capacity = Capacity;
    list->itemSize = ItemSize;

    // Set the head and tail pointers to point into the data section
    list->head = list->data;
    list->tail = list->data;
}


void List_deinitialize(List* list) {
    // De-initialize a List.

    if (!list) return;

    free(list->data);
    list->data = NULL;
    list->head = NULL;  // points into list->data, no need to free.
    list->tail = NULL;  // points into list->data, no need to free.
}


void List_destroy(List** list) {
    // De-initialize and free a List.

    if (!(*list)) return;

    free((*list)->data);
    (*list)->data = NULL;
    (*list)->head = NULL;   // points into list->data, no need to free.
    (*list)->tail = NULL;   // points into list->data, no need to free.

    free((*list));
    (*list) = NULL;
}


void internal_List_set(List* list, void* template, const u32 count) {
    // Clears the List, resizes if needed, and fills it with the data stored in template.
    //
    //

    // If count is greater than the list capacity, reallocate with doubling factor of 1.5
    if (count >= list->capacity) {
        u8* newData = (u8*)malloc(list->itemSize * (count + (count >> 1)));
        free(list->data);
        list->head = newData;
        list->tail = newData;
        list->data = newData;
        list->capacity = count;
    }

    u8* buffer = list->data;
    for (u32 i = 0; i < count; i++, buffer += list->itemSize) {
        internal_list_copy(buffer, template, list->itemSize);
    }

    list->head = list->data + (count * list->itemSize);
}


void* List_create_array(List* list) {
    // Creates a C-Style pointer array from the list.
    // you must free this list yourself!
    //

    void* array = malloc(List_byte_count(list));

    // The copy can be simplified since the list is either continuous, or split in two.
    //
    // if the head is behind the tail, list is split in two:
    if (list->head < list->tail) {
        // Copy first half.
        u32 bytesToCopyFromTail = (u32)(internal_List_end(u8, list) - list->tail);
        internal_list_copy(array, list->tail, bytesToCopyFromTail);

        // Copy the second half.
        u32 bytesToCopyFromHead = list->head - internal_List_start(u8, list);
        internal_list_copy((u8*)array + bytesToCopyFromTail, internal_List_start(u8, list), bytesToCopyFromHead);
    }
    // list is continuous, and so only one internal_list_copy is needed:
    else {
        u32 bytesToCopy = (u32)(list->head - list->tail);
        internal_list_copy(array, list->head, bytesToCopy);
    }

    return array;
}


void List_reorder(List* list) {
    // Reorders the data section of the list so that it isn't split.
    //
    //

    // do nothing if the list is empty.
    if (list->head == list->tail) return;

    // tail = data means the list is already ordered, so do nothing.
    if (list->tail == list->data) return;

    u32 countBytes = List_byte_count(list);

    // if the list is continuous, and there is enough room at the start of the list's data block to store the whole list:
    if ((list->tail - list->data) >= countBytes && list->head > list->tail) {
        internal_list_copy(list->data, list->tail, countBytes);
    }
    else {
        void* temp = List_create_array(list);
        internal_list_copy(list->data, temp, list->itemSize * List_count(list));
        free(temp);
    }

    list->tail = list->data;
    list->head = list->data + countBytes;
}


void List_realloc(List* list, u32 Capacity) {
    // Reallocates the data section of the list.
    // This function cannot shrink a list. 
    //

    u32 oldCount = List_count(list);

    // Always try to reallocate the buffer. Simply clamp Capacity to always be at least list count.
    if (Capacity < oldCount) {
        Capacity = oldCount;
    }

    u8* newData = (u8*)malloc(list->itemSize * Capacity);

    // Jump to end if for some reason you're reallocating an empty array?? why are you doing that?
    if (list->head == list->tail) {
        goto ReasignPointers;
    }

    // The copy can be simplified since the list is either continuous, or split in two.
    //
    // if the head is behind the tail, list is split in two:
    if (list->head < list->tail) {
        // Copy first half.
        u32 bytesToCopyFromTail = internal_List_end(u8, list) - list->tail;
        internal_list_copy(newData, list->tail, bytesToCopyFromTail);

        // Copy the second half.
        u32 bytesToCopyFromHead = list->head - internal_List_start(u8, list);
        internal_list_copy(newData + bytesToCopyFromTail, internal_List_start(u8, list), bytesToCopyFromHead);
    }
    // list is continuous, and so only one internal_list_copy is needed:
    else {
        u32 bytesToCopy = (u32)(list->head - list->tail);
        internal_list_copy(newData, list->tail, bytesToCopy);
    }

    ReasignPointers:
    // Reassign the head and tail to point into the new data section.
    list->tail = newData;
    list->head = newData + (oldCount * list->itemSize);

    // free the old data, and set the pointer.
    free(list->data);
    list->data = newData;

    // Update the capacity.
    list->capacity = Capacity;
}


u32 List_byte_count(const List* list) {
    //  Returns the number of bytes used.
    //
    //

    // If the head is in-front of the tail:
    if (list->head > list->tail) {
        return (u32)(list->head - list->tail);
    }
    // If the head is behind of the tail:
    else {
        return list->capacity - (u32)(list->tail - list->head);
    }
}


u32 List_count(const List* list) {
    //  Returns the number of slots used.
    //
    //

    if (list->head == list->tail) {
        return 0;
    }
    // If the head is in-front of the tail:
    else if (list->head > list->tail) {
        return ((u32)(list->head - list->tail)) / list->itemSize;
    }
    // If the head is behind of the tail:
    else {
        return (list->capacity - ((u32)(list->tail - list->head) / list->itemSize));
    }
}


bool List_contains_item(List* list, void* item, i64* out) {
    u8* itemAsBytes = (u8*)item;
    i64 itemIndex = 0;
    for (List_iterator(u8, list), ++itemIndex) {
        for (u32 i = 0; i < list->itemSize; ++i) {
            if (it[i] != itemAsBytes[i]) {
                goto NotMatching;
            }
        }
        if (out) *out = itemIndex;
        return true;
        NotMatching:
        continue;
    }
    if (out) *out = -1;
    return false;
}


void* List_at(const List* list, const u32 index) {
    // Returns the item at a particular index.
    //
    //

    // Return NULL if index is out of range.
    if (index > List_count(list)) {
        return NULL;
    }

    // Get the pointer to the tail, Index should count up from the tail towards the head.
    u8* dataPointer = (u8*)list->tail;

    // Add the offset to the data pointer,
    dataPointer += index * list->itemSize;

    if (dataPointer < internal_List_end(u8, list)) {
        return dataPointer;
    }

    // if its outside the bounds of the List, wrap around to the start. 
    // This is okay since i <= capacity so it could only ever wrap once.
    dataPointer -= list->capacity * list->itemSize;
    return dataPointer;
}


void List_remove_at(List* list, const u32 index) {
    // Remove item from a specific index.
    //
    //

    // Check that i is valid.
    if (index > List_count(list)) {
        return;
    }

    void* currentIndex = List_at(list, index);
    void* nextIndex = NULL;

    for (u32 i = index; i < list->capacity - 1; i++) {
        nextIndex = List_at(list, i + 1);                               // Get the address of the next item.
        internal_list_copy(currentIndex, nextIndex, list->itemSize);    // Using internal_list_copy here since we don't know the type stored, only how many bytes it is. 
        currentIndex = nextIndex;
    }

    // Decrement the head since we shifted everything back. 
    internal_List_getPrevPtr(u8, list, list->head);
}


void internal_List_push_back(List* list, void* data) {
    // Reallocate the array with a doubling factor of 1.5
    if (List_count(list) >= list->capacity - 1) {
        List_realloc(list, list->capacity + (list->capacity << 1));
    }
    internal_list_copy(list->head, data, list->itemSize);
    internal_List_getNextPtr(u8, list, list->head);
}


void internal_List_push_front(List* list, void* data) {
    // Reallocate the array with a doubling factor of 1.5
    if (List_count(list) >= list->capacity - 1) {
        List_realloc(list, list->capacity + (list->capacity << 1));
    }
    internal_list_copy(list->tail, data, list->itemSize);
    internal_List_getPrevPtr(u8, list, list->tail);
}


void internal_List_pop_front(List* list, void* outVal) {
    // Returns next item from the front of the list.
    // If this is a pointer type, you must now free it.
    //
    if (list->head == list->tail) return;   // Leave early if there is no data.

    internal_List_getPrevPtr(u8, list, list->head);
    internal_list_copy(outVal, list->head, list->itemSize);
}


void internal_List_pop_back(List* list, void* outVal) {
    // Returns next item from the back of the list. 
    // If this is a pointer type, you must now free it.
    //
    if (list->head == list->tail) return;   // Leave early if there is no data.

    internal_list_copy(outVal, list->tail, list->itemSize);
    internal_List_getNextPtr(u8, list, list->tail);
}


void internal_List_peak_front(List* list, void* outVal) {
    if (list->head == list->tail) return;   // Leave early if there is no data.

    u8* temp = list->head;
    internal_List_getPrevPtr(u8, list, temp);
    internal_list_copy(outVal, temp, list->itemSize);
}


void internal_List_peak_back(List* list, void* outVal) {
    if (list->head == list->tail) return;   // Leave early if there is no data.
    internal_list_copy(outVal, list->tail, list->itemSize);
}


void List_append(List* dst, List* src) {
    // Appends one list to another.
    //
    //

    if (!dst || !src) return;

    // leave early if the list contain different data "types".
    if (dst->itemSize != src->itemSize) return;

    u32 sourceByteCount = List_byte_count(src);
    u32 sourceCount = List_count(src);
    u32 combinedCount = List_count(dst) + sourceCount;
    void* srcArray = List_create_array(src);

    if (combinedCount >= dst->capacity) {
        List_realloc(dst, combinedCount);
    }

    else {
        List_reorder(dst);
    }

    // At this point, the list must be in order since both realloc and reorder cause the array to not be split.
    internal_list_copy(dst->head, srcArray, sourceByteCount);
    dst->head += sourceByteCount;
    free(srcArray);
}

#endif
