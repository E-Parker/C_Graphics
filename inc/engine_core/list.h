// List implementation by Ethan Parker.
//
// To use this library, add a source file with these two lines.
// #define LIST_IMPLEMENTATION
// #include "list.h"
//
// For documentation please see README.md  
//

#ifdef __cplusplus
extern "C" {
#endif

#pragma once

//#define LIST_USE_MEMCPY

#ifdef LIST_USE_MEMCPY
#define internal_list_copy(dst, src, size) memcpy(dst, src, size)
#else
#define internal_list_copy(dst, src, size) for (unsigned int internal_list_copy_iterator = 0; internal_list_copy_iterator < (unsigned int)size; ++internal_list_copy_iterator) { ((char*)(dst))[internal_list_copy_iterator] = ((char*)(src))[internal_list_copy_iterator]; }
#endif


// Type Definitions:
// 
//

typedef struct List{
    /* Simple list container, using a circular buffer. */
    
    unsigned int capacity;  // Current capacity of the list.
    unsigned int itemSize;  // Size in bytes of an element.
    char* head;             // Pointer to the "head" of the list.
    char* tail;             // Pointer to the "tail" of the list.
    char* data;             // Pointer to the data block of "head" and "tail".
} List;

// List Functions:
// 
//

#define List_create(T, capacity) internal_List_create(sizeof(T), capacity)
List* internal_List_create(const unsigned int ItemSize, const unsigned int Capacity);

#define List_initialize(T, list, capacity) internal_List_initialize(list, sizeof(T), capacity)
void internal_List_initialize(List* list, const unsigned int ItemSize, const unsigned int Capacity);

void List_destroy(List** list);
void List_deinitialize(List* list);

List* List_create_subset(List* list, unsigned int start, unsigned int end);

// These macros are for manipulating the head and tail pointers, wrapping around the List's data region.
// If getting the next slot would overrun the buffer, instead loop around to the start of the buffer. Otherwise, increment by ItemSize.
//

#define List_isEmpty(list) ((list)->head == (list)->tail)
#define internal_List_start(T, list) ((T*)(list)->data)
#define internal_List_end(T, list) (T*)((internal_List_start(char, (list))) + ((list)->capacity * (list)->itemSize))
#define internal_List_getNextPtr(T, list, ptr) ptr = (T*)(((char*)ptr == internal_List_end(char, list) - (list)->itemSize)? internal_List_start(char, list) : (char*)ptr + (list)->itemSize)
#define internal_List_getPrevPtr(T, list, ptr) ptr = (T*)(((char*)ptr == internal_List_start(char, list))? internal_List_end(char, list) - (list)->itemSize : (char*)ptr - (list)->itemSize)
#define internal_List_validate(list) assert((list)->head != (list)->tail)

// Creates T* variable, it, which is the current item in the loop. This will be a little faster than using List_at().
// If you are using function pointers, dereference it before calling.
#define List_iterator(T, list) (T* it = (T*)(list)->head; (char*)it != (list)->tail; internal_List_getPrevPtr(T, list, it)) 

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

void internal_List_set(List* list, void* template, const unsigned int count);

void* List_create_array(List* list);

unsigned int List_count(const List* list);
unsigned int List_byte_count(const List* list);

void List_realloc(List* list, unsigned int Capacity);
void List_reorder(List* list);

void List_remove_at(List* list, const unsigned int index);
void* List_at(const List* list, const unsigned int index);

void List_append(List* dst, List* src);

#ifdef __cplusplus
}
#endif

#ifdef LIST_IMPLEMENTATION
#include <stdlib.h>

#ifdef LIST_USE_MEMCPY
#include <string.h>
#endif

List* internal_List_create(const unsigned int ItemSize, const unsigned int Capacity) {
    List* newList = (List*)malloc(sizeof(List));

    internal_List_initialize(newList, ItemSize, Capacity);
    return newList;
}


List* List_create_subset(List* list, unsigned int start, unsigned int end) {
    // Creates a new list as a subset of another list.
    // Returns NULL if the subset cannot be created.
    //

    // cannot create subset with start "in-front of" end.
    if (start >= end) return NULL;

    unsigned int listCount = List_count(list);
    
    // cannot create subset which is outside of the bounds of the original list.
    if (end > listCount || start >= listCount) return NULL;

    unsigned int capacity = end - start;                    // capacity needed to store the number of items in the subset.
    unsigned int capacityBytes = capacity * list->itemSize; // capacity needed in bytes.

    void* subsetArray = malloc(capacityBytes);
    void* listArray = List_create_array(list);
    internal_list_copy(subsetArray, listArray, capacityBytes);

    List* subset = (List*)malloc(sizeof(List));

    subset->capacity = capacity;
    subset->itemSize = list->itemSize;
    subset->data = (char*)subsetArray;
    subset->head = (char*)subsetArray;
    subset->tail = (char*)subsetArray;

    return subset;
}


void internal_List_initialize(List* list, const unsigned int ItemSize, const unsigned int Capacity) {
    // initialize default values of any fixedList.
    //
    //

    list->data = (char*)calloc(ItemSize, Capacity);
    //list->data = (char*)malloc(ItemSize * Capacity);

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


void internal_List_set(List* list, void* template, const unsigned int count) {
    // Clears the List, resizes if needed, and fills it with the data stored in template.
    //
    //
    
    // If count is greater than the list capacity, reallocate with doubling factor of 1.5
    if (count >= list->capacity) {
        char* newData = (char*)malloc(list->itemSize * (count + (count >> 1)));    
        free(list->data);
        list->head = newData;
        list->tail = newData;
        list->data = newData;
        list->capacity = count;
    }

    char* buffer = list->data; 
    for (unsigned int i = 0; i < count; i++, buffer += list->itemSize) {
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
        unsigned int bytesToCopyFromTail = (unsigned int)(internal_List_end(char, list) - list->tail);
        internal_list_copy(array, list->tail, bytesToCopyFromTail);

        // Copy the second half.
        unsigned int bytesToCopyFromHead = list->head - internal_List_start(char, list);
        internal_list_copy((char*)array + bytesToCopyFromTail, internal_List_start(char, list), bytesToCopyFromHead);
    }
    // list is continuous, and so only one internal_list_copy is needed:
    else {
        unsigned int bytesToCopy = (unsigned int)(list->head - list->tail);
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

    unsigned int countBytes = List_byte_count(list);

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


void List_realloc(List* list, unsigned int Capacity) {
    // Reallocates the data section of the list.
    // This function cannot shrink a list. 
    //
    
    unsigned int oldCount = List_count(list);

    // Always try to reallocate the buffer. Simply clamp Capacity to always be at least list count.
    if(Capacity < oldCount) {
        Capacity = oldCount;
    }

    char* newData = (char*)malloc(list->itemSize * Capacity);
    
    // Early return if for some reason you're reallocating an empty array?? why are you doing that?
    if(list->head == list->tail) {
        free(list->data);
        list->head = newData;
        list->tail = newData;
        list->data = newData;
        return;
    }

    // The copy can be simplified since the list is either continuous, or split in two.
    //
    // if the head is behind the tail, list is split in two:
    if(list->head < list->tail) {
        // Copy first half.
        unsigned int bytesToCopyFromTail = internal_List_end(char, list) - list->tail;
        internal_list_copy(newData, list->tail, bytesToCopyFromTail);

        // Copy the second half.
        unsigned int bytesToCopyFromHead = list->head - internal_List_start(char, list);
        internal_list_copy(newData + bytesToCopyFromTail, internal_List_start(char, list), bytesToCopyFromHead);
    }
    // list is continuous, and so only one internal_list_copy is needed:
    else {
        unsigned int bytesToCopy = (unsigned int)(list->head - list->tail);
        internal_list_copy(newData, list->tail, bytesToCopy);
    }
    
    // Reassign the head and tail to point into the new data section.
    list->tail = newData;
    list->head = newData + (oldCount * list->itemSize);
    
    // free the old data, and set the pointer.
    free(list->data);
    list->data = newData;

    // Update the capacity.
    list->capacity = Capacity;
}


unsigned int List_byte_count(const List* list) {
    //  Returns the number of bytes used.
    //
    //

    // If the head is in-front of the tail:
    if (list->head > list->tail) {
        return (unsigned int)(list->head - list->tail);
    }
    // If the head is behind of the tail:
    else {
        return list->capacity - (unsigned int)(list->tail - list->head);
    }
}


unsigned int List_count(const List* list) {
    //  Returns the number of slots used.
    //
    //

    if (list->head == list->tail) {
        return 0;
    }
    // If the head is in-front of the tail:
    else if (list->head > list->tail) {
        return ((unsigned int)(list->head - list->tail)) / list->itemSize;
    }
    // If the head is behind of the tail:
    else {
        return (list->capacity - ((unsigned int)(list->tail - list->head) / list->itemSize));
    }
}


void* List_at(const List* list, const unsigned int index) {
    // Returns the item at a particular index.
    //
    //

    // Return NULL if index is out of range.
    if (index >= List_count(list)) {
        return NULL;
    } 

    // Get the pointer to the tail, Index should count up from the tail towards the head.
    char* dataPointer = (char*)list->tail;
    
    // Add the offset to the data pointer,
    dataPointer += index * list->itemSize;
    
    if(dataPointer < internal_List_end(char, list)) {
        return dataPointer;
    }

    // if its outside the bounds of the List, wrap around to the start. 
    // This is okay since i <= capacity so it could only ever wrap once.
    dataPointer -= list->capacity * list->itemSize;
    return dataPointer;
}


void List_remove_at(List* list, const unsigned int index) {
    // Remove item from a specific index.
    //
    //
    
    // Check that i is valid.
    if(index >= List_count(list)) {
        return;
    }
    
    void* currentIndex = List_at(list, index);
    void* nextIndex = NULL;
    
    for(unsigned int i = index; i < list->capacity - 1; i++) {
        nextIndex = List_at(list, i + 1);                   // Get the address of the next item.
        internal_list_copy(currentIndex, nextIndex, list->itemSize);    // Using internal_list_copy here since we don't know the type stored, only how many bytes it is. 
        currentIndex = nextIndex;
    }

    // Decrement the head since we shifted everything back. 
    internal_List_getPrevPtr(char, list, list->head);
}


void internal_List_push_front(List* list, void* data) {
    // push a new value onto the front of the list.
    //
    //

    // Early return if the list can still fit the next item. 
    if(List_count(list) < list->capacity - 1) {
        internal_List_getNextPtr(char, list, list->head);
        internal_list_copy(list->head, data, list->itemSize);
        return;
    }
    
    // Reallocate the array with a doubling factor of 1.5
    List_realloc(list, list->capacity + (list->capacity >> 1));
    internal_List_getNextPtr(char, list, list->head);
    internal_list_copy(list->head, data, list->itemSize);
}


void internal_List_push_back(List* list, void* data) {
    // push a new value onto the back of the list.
    //
    //

    // Early return if the list can still fit the next item. 
    if(List_count(list) < list->capacity) { 
        internal_list_copy(list->tail, data, list->itemSize);
        internal_List_getPrevPtr(char, list, list->tail);
        return;
    }
    
    // Reallocate the array with a doubling factor of 1.5.
    List_realloc(list, list->capacity + (list->capacity >> 1));
    internal_list_copy(list->tail, data, list->itemSize);
    internal_List_getPrevPtr(char, list, list->tail);
}


void internal_List_pop_front(List* list, void* outVal) {
    // Returns next item from the front of the list.
    // If this is a pointer type, you must now free it.
    //
    if (list->head == list->tail) return;   // Leave early if there is no data.

    internal_List_getPrevPtr(char, list, list->head);
    internal_list_copy(outVal, list->head, list->itemSize);
}


void internal_List_pop_back(List* list, void* outVal) {
    // Returns next item from the back of the list. 
    // If this is a pointer type, you must now free it.
    //
    if (list->head == list->tail) return;   // Leave early if there is no data.

    internal_list_copy(outVal, list->tail, list->itemSize);
    internal_List_getNextPtr(char, list, list->tail);
}


void internal_List_peak_front(List* list, void* outVal) {
    if (list->head == list->tail) return;   // Leave early if there is no data.

    char* temp = list->head;
    internal_List_getPrevPtr(char, list, temp);
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

    unsigned int srcByteCount = List_byte_count(src);
    unsigned int combinedByteCount = List_byte_count(dst) + srcByteCount;
    void* srcArray = List_create_array(src);
    
    // TODO: This is a bug.
    List_realloc(dst, combinedByteCount);       // realloc only does anything when the new capacity is greater than the existing one.
    List_reorder(dst);                          // reorder skips lists already in order, so if realloc is called this also doesn't waste time.
    internal_list_copy(dst->head, srcArray, srcByteCount);  // since the list has to be in order, and is big enough, just internal_list_copy the bytes from src.
    dst->head += srcByteCount;
    free(srcArray);
}
#endif