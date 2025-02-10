#pragma once

typedef struct FixedList {
    /* Simple fixed capacity container, similar to a linked-list in functionality. */
    
    //                      Offset  | Alignment
    unsigned int capacity;  //  0   | 4
    unsigned int itemSize;  //  4   | 4
    char* head;             //  8   | 8
    char* tail;             //  16  | 8
    char* data;             //  24  | 8
} FixedList;

#define FixedList_create(T, ItemCapacity) internal_FixedList_create(sizeof(T), ItemCapacity)
FixedList* internal_FixedList_create(const unsigned int ItemSize, const unsigned int Capacity);
void FixedList_initialize(FixedList*, const unsigned int ItemSize, const unsigned int Capacity);
void FixedList_destroy(FixedList** list);
void FixedList_deinitialize(FixedList* list);

// These macros are for manipulating the head and tail pointers, wrapping around the List's data region.
// If getting the next slot would overrun the buffer, instead loop around to the start of the buffer. Otherwise, increment by ItemSize.
//

#define FixedList_isEmpty(List) (List->head == List->tail)
#define FixedList_start(T, List) ((T*)List->data)
#define FixedList_end(List) ((FixedList_start(char, List)) + (List->capacity * List->itemSize))
#define internal_FixedList_getNextPtr(List,ptr) ptr = ((char*)ptr == FixedList_end(List) - List->itemSize)? FixedList_start(List) : (char*)ptr + List->itemSize
#define internal_FixedList_getPrevPtr(List,ptr) ptr = ((char*)ptr == FixedList_start(char, List))? FixedList_end(List) - List->itemSize : (char*)ptr - List->itemSize
#define internal_FixedList_validate(List) assert(List->head != List->tail)

// Custom for loop iterator. will be a little faster than FixedList_at().
#define FixedList_iterator(T, list) (T* it = (T*)list->head; (void*)it != (void*)list->tail; internal_FixedList_getprev_ptr(list, (void*)it);) 

#define FixedList_push_front(List, Data) internal_FixedList_push_front(List, (void*)Data)
#define FixedList_push_back(List, Data) internal_FixedList_push_back(List, (void*)Data)
void internal_FixedList_push_front(FixedList* list, void* data);
void internal_FixedList_push_back(FixedList* list, void* data);
void* FixedList_pop_front(FixedList* list);
void* FixedList_pop_back(FixedList* list);

unsigned int FixedList_size(const FixedList* list);
void FixedList_realloc(FixedList* list, const unsigned int Capacity);
void FixedList_remove_at(FixedList* list, const unsigned int index);
void* FixedList_at(const FixedList* list, const unsigned int index);

