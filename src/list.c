#include <stdlib.h>
//#include <string.h>

#include "list.h"


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
        return;
    }

    void* temp = List_create_array(list);
    internal_list_copy(list->data, temp, list->itemSize * List_count(list));
    free(temp);
}


void List_realloc(List* list, const unsigned int Capacity) {
    // Reallocates the data section of the list.
    // This function cannot shrink a list. 
    //
    
    unsigned int oldSize = List_count(list);

    if(Capacity < oldSize) {
        // tried to resize a list to a list that cannot fit the old list.
        return;
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
    list->head = newData + (oldSize * list->itemSize);
    list->tail = newData; 
    
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

    List_realloc(dst, combinedByteCount);       // realloc only does anything when the new capacity is greater than the existing one.
    List_reorder(dst);                          // reorder skips lists already in order, so if realloc is called this also doesn't waste time.
    internal_list_copy(dst->head, srcArray, srcByteCount);  // since the list has to be in order, and is big enough, just internal_list_copy the bytes from src.
    dst->head += srcByteCount;
    free(srcArray);
}

