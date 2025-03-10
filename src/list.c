#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "list.h"


List* internal_List_create(const unsigned int ItemSize, const unsigned int Capacity) {
    List* newList = (List*)malloc(sizeof(List));
    assert(newList);

    List_initialize(newList, ItemSize, Capacity);
    return newList;
}


void List_initialize(List* list, const unsigned int ItemSize, const unsigned int Capacity) {
    // initialize default values of any fixedList.
    //
    //

    list->data = (char*)calloc(ItemSize, Capacity);

    // Set capacity and size.
    list->capacity = Capacity;
    list->itemSize = ItemSize;
    
    // Set the head and tail pointers to point into the data section
    list->head = list->data;
    list->tail = list->data;

}


void List_deinitialize(List* list) { 
    // De-initialize a List.
    free(list->data);
    list->data = NULL;
}


void List_destroy(List** list) {
    // De-initialize and free a List.
    
    free((*list)->data);
    (*list)->data = NULL;
    
    free((*list));
    (*list) = NULL;
}


void List_realloc(List* list, const unsigned int Capacity) {
    // Reallocates the data section of the list.
    // This function cannot shrink a list. 
    //
    
    unsigned int oldSize = List_size(list);

    if(Capacity < oldSize) {
        // tried to resize a list to a list that cannot fit the old list.
        return;
    }

    char* newData = (char*)calloc(list->itemSize, Capacity);
    
    // Early return if for some reason you're reallocating an empty array?? why are you doing that?
    if(list->head == list->tail) {
        list->head = newData;
        list->tail = newData;
        free(list->data);
        list->data = newData;
        return;
    }

    // The copy can be simplified since the list is either continuous, or split in two.
    //
    // if the head is behind the tail, list is split in two:
    if(list->head < list->tail) {
        // Copy first half.
        unsigned int bytesToCopyFromTail = internal_List_end(char, list) - list->tail;
        memcpy(list->tail, newData, bytesToCopyFromTail);

        // Copy the second half.
        unsigned int bytesToCopyFromHead = list->head - internal_List_start(char, list);
        memcpy(internal_List_start(char, list), newData + bytesToCopyFromTail, bytesToCopyFromHead);
    }
    // list is continuous, and so only one memcpy is needed:
    else {
        unsigned int bytesToCopy = (unsigned int)(list->head - list->tail);
        memcpy(list->head, newData, bytesToCopy);
    }
    
    // Reassign the head and tail to point into the new data section.
    list->head = newData + (oldSize * list->itemSize);
    list->tail = newData; 
    
    // free the old data, and set the pointer
    free(list->data);
    list->data = newData;

    // Update the capacity.
    list->capacity = Capacity;
}


unsigned int List_size(const List* list) {
    //  Returns the number of slots used.
    //
    //

    // If the head is in-front of the tail:
    if (list->head > list->tail) {
        return ((unsigned int)(list->head - list->tail) + list->itemSize) / list->itemSize;
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

    // Get the pointer to the tail, Index should count up from the tail towards the head.
    char* dataPointer = (char*)list->tail;
    
    // Add the offset to the data pointer,
    dataPointer += index * list->itemSize;
    
    if(dataPointer < internal_List_end(char, list)) {
        return dataPointer;
    };

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
    if(index >= List_size(list)) {
        return;
    }
    
    void* currentIndex = List_at(list, index);
    void* nextIndex = NULL;
    
    for(unsigned int i = index; i < list->capacity - 1; i++) {
        nextIndex = List_at(list, i + 1);              // Get the address of the next item.
        memcpy(currentIndex, nextIndex, list->itemSize);    // Using memcpy here since we don't know the type stored, only how many bytes it is. 
        currentIndex = nextIndex;
    }
}


void internal_List_push_front(List* list, void* data) {
    // push a new value onto the front of the list.
    //
    //

    // Early return if the list can still fit the next item. 
    if(List_size(list) < list->capacity) {
        internal_List_getNextPtr(char, list, list->head);
        memcpy(list->head, data, list->itemSize);
        return;
    }
    
    // Reallocate the array with a doubling factor of 1.5
    List_realloc(list, list->capacity + (list->capacity >> 1));
    internal_List_getNextPtr(char, list, list->head);
    memcpy(list->head, data, list->itemSize);
}


void internal_List_push_back(List* list, void* data) {
    // push a new value onto the back of the list.
    //
    //

    // Early return if the list can still fit the next item. 
    if(List_size(list) < list->capacity) { 
        internal_List_getPrevPtr(char, list, list->tail);
        memcpy(list->tail, data, list->itemSize);
        return;
    }
    
    // Reallocate the array with a doubling factor of 1.5
    List_realloc(list, list->capacity + (list->capacity >> 1));
    internal_List_getPrevPtr(char, list, list->tail);
    memcpy(list->tail, data, list->itemSize);    
}


void* List_pop_front(List* list) {
    // Returns next item from the front of the list. 
    // If this is a pointer type, you must now free it.
    //
    if (list->head == list->tail) return NULL;   // Leave early if there is no data.

    void* data = (void*)list->head;
    internal_List_getPrevPtr(char, list, list->head);
    return data;
}


void* List_pop_back(List* list) {
    // Returns next item from the back of the list. 
    // If this is a pointer type, you must now free it.
    //
    if (list->head == list->tail) return NULL;   // Leave early if there is no data.

    void* data = (void*)list->tail;
    internal_List_getNextPtr(char, list, list->tail);
    return data;
}



