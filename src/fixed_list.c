#include "fixed_list.h"
#include <string.h>


FixedList* internal_FixedList_create(const unsigned int ItemSize, const unsigned int Capacity) {
    FixedList* newList = (FixedList*)malloc(sizeof(FixedList))
    assert(newList);

    FixeList_initialize(newList, ItemSize, Capacity);
    return newList;
}


void FixedList_initialize(FixedList* list, const unsigned int ItemSize, const unsigned int Capacity) {
    // initialize default values of any fixedList.
    //
    //

    list->data = (char*)calloc(ItemSize, Capacity);

    // Set capacity and size.
    list->capacity = Capacity;
    list->itemSize = ItemSize;
    
    // Set the head and tail pointers to point into the data section
    newList->head = list->data;
    newList->tail = list->data;

}


void FixedList_deinitialize(FixedList* list) { 
    // Deinitialize a FixedList.
    free(list->data);
    list->data = NULL;
}


void FixedList_destroy(FixedList** list) {
    // Deinitialize and free a FixedList.
    
    free((*list)->data);
    (*list)->data = NULL;
    
    free((*list));
    (*list) = NULL;
}


void FixedList_realloc(FixedList* list, const unsigned int Capacity) {
    // Realocates the data section of the List.
    //
    //
    
    unsigned int oldSize = FixedList_size(list);

    if(Capacity < oldSize) {
#ifdef DEBUG
        // tried to resize a list to a list that cannot fit the old list.
        assert(0);
#endif
        return;
    }

    char* newData = (char*)calloc(list->itemSize, Capacity);
    
    // Early return if for some reason you're realocating an empty array?? why are you doing that?
    if(list->head == list->tail) {
        list->head = newData;
        list->tail = newData;
        free(list->data);
        list->data = newData;
        return;
    }

    // The copy can be simplified since the list is is either continuous, or split in two.
    //
    // if the head is behind the tail, list is split in two:
    if(list->head < list->tail) {
        // Copy first half.
        unsigned int bytesToCopyFromTail = FixedList_end(list) - list->tail;
        memcpy(list->tail, newData, bytesToCopyFromTail);

        // Copy the second half.
        unsigned int bytesToCopyFromHead = list->head - FixedList_start(char, list);
        memcpy(FixedList_start(char, list), newData + bytesToCopyFromTail, bytesToCopyFromHead);
    }
    // list is is continuous and so only one memcpy is needed:
    else {
        unsigned int bytesToCopy = ((unsigned int)(list->head - list->tail);
        memcpy(list->head, newdata, bytesToCopy);
    }
    
    // Reassign the head and tail to point into the new data section.
    list->head = newData + (oldSize * list->itemSize);
    list->tail = newData; 
    
    // free the old data, and set the pointer
    free(list->data)
    list->data = newData;

    // Update the capacity.
    list->capacity = Capacity;
}


unsigned int FixedList_size(const FixedList* list) {
    //  Returns the number of slots used.
    //
    //

    // If the head is infront of the tail:
    if (list->head > list->tail) {
        return ((unsigned int)(list->head - list->tail) + list->itemSize) / list->itemSize;
    }
    // If the head is behind of the tail:
    else {
        return (list->capacity - ((unsigned int)(list->tail - list->head) / list->itemSize));
    }
}


void* FixedList_at(const FixedList* list, const unsigned int index) {
    // Returns the item at a partiular index.
    //
    //

    // Get the pointer to the tail, Index should count up from the tail towards the head.
    char* dataPointer = (char*)list->tail;
    
    // Add the offset to the data pointer,
    dataPointer += index * list->itemSize;
    
    if(dataPointer < FixedList_end(list)) {
        return dataPointer
    };

    // if its outside the bounds of the FixedList, wrap around to the start. 
    // This is okay since i <= capacity so it could only ever wrap once.
    dataPointer -= list->capacity * list->itemSize;
    return dataPointer
}


void FixedList_remove_at(FixedList* list, const unsigned int index) {
    // Remove item from a specific index.
    //
    //
    
    // Check that i is valid.
    if(index >= FixedList_size(list)) {
        return;
    }
    
    void* currentIndex = FixedList_at(list, index);
    void* nextIndex = NULL;
    
    for(unsigned int i = index; i < list->capacity - 1; i++) {
        nextIndex = FixedList_at(list, i + 1);          // Get the adress of the next item.
        memcpy(currentIndex, nextIndex, list->itemSize);// Using memcpy here since we don't know the type stored, only how many bytes it is. 
        currentIndex = nextIndex;
    }
}


void internal_FixedList_push_front(FixedList* list, void* data) {
    // push a new value onto the front of the list.
    //
    //

    // Early return if the list can still fit the next item. 
    if(FixedList_size(list) < fixedList->capacity) {
        internal_FixedList_GetNextPtr(list, list->Head);
        memcpy(list->head, data, list->itemSize);
        return;
    }
    
    // Realocate the array with a doubling factor of 1.5
    FixedList_Realloc(list, list->capacity + (list->capacity >> 1));
    internal_FixedList_GetNextPtr(list, list->Head);
    memcpy(list->head, data, list->itemSize);
}


void internal_FixedList_push_back(FixedList* list, void* data) {
    // push a new value onto the back of the list.
    //
    //

    // Early return if the list can still fit the next item. 
    if(FixedList_size(list) < fixedList->capacity) { 
        internal_FixedList_GetPrevPtr(list, list->tail);
        memcpy(list->tail, data, list->itemSize);
        return;
    }
    
    // Realocate the array with a doubling factor of 1.5
    FixedList_Realloc(list, list->capacity + (list->capacity >> 1));
    internal_FixedList_GetPrevPtr(list, list->tail);
    memcpy(list->tail, data, list->itemSize);    
}


void* FixedList_pop_front(FixedList* list) {
    
    if (list->head == list->tail) return;   // Leave early if there is no data.

    void* data = (void*)list->head;
    internal_FixedList_GetPrevPtr(list, list->head);
    return data;
}


void* FixedList_pop_back(FixedList* List) {
    
    if (list->head == list->tail) return;   // Leave early if there is no data.

    void* data = (void*)List->Tail;
    internal_FixedList_GetNextPtr(List, List->Tail);
    return data;
}



