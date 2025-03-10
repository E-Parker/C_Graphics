#pragma once

#ifdef __cplusplus
extern "C" {
#endif


// Type Definitions:
// 
//

typedef struct {
    /* Simple list container, similar to a linked-list in functionality. */
    
    //                      Offset  | Alignment
    unsigned int capacity;  //  0   | 4
    unsigned int itemSize;  //  4   | 4
    char* head;             //  8   | 8
    char* tail;             //  16  | 8
    char* data;             //  24  | 8
} List;

// List Functions:
// 
//

#define List_create(T, ItemCapacity) internal_List_create(sizeof(T), ItemCapacity)
List* internal_List_create(const unsigned int ItemSize, const unsigned int Capacity);
void List_initialize(List*, const unsigned int ItemSize, const unsigned int Capacity);
void List_destroy(List** list);
void List_deinitialize(List* list);

// These macros are for manipulating the head and tail pointers, wrapping around the List's data region.
// If getting the next slot would overrun the buffer, instead loop around to the start of the buffer. Otherwise, increment by ItemSize.
//

#define List_isEmpty(List) (List->head == List->tail)
#define internal_List_start(T, List) ((T*)List->data)
#define internal_List_end(T, List) (T*)((internal_List_start(char, List)) + (List->capacity * List->itemSize))
#define internal_List_getNextPtr(T, List, ptr) ptr = (T*)(((char*)ptr == internal_List_end(char, List) - List->itemSize)? internal_List_start(char, List) : (char*)ptr + List->itemSize)
#define internal_List_getPrevPtr(T, List, ptr) ptr = (T*)(((char*)ptr == internal_List_start(char, List))? internal_List_end(char, List) - List->itemSize : (char*)ptr - List->itemSize)
#define internal_List_validate(List) assert(List->head != List->tail)

// Creates T* variable, it, which is the current item in the loop. This will be a little faster than using List_at().
// If you are using function pointers, dereference it before calling.
#define List_iterator(T, list) (T* it = (T*)list->head; (char*)it != list->tail; internal_List_getPrevPtr(T, list, it)) 

#define List_push_front(List, Data) internal_List_push_front(List, (void*)Data)
#define List_push_back(List, Data) internal_List_push_back(List, (void*)Data)
void internal_List_push_front(List* list, void* data);
void internal_List_push_back(List* list, void* data);
void* List_pop_front(List* list);
void* List_pop_back(List* list);

unsigned int List_size(const List* list);
void List_realloc(List* list, const unsigned int Capacity);
void List_remove_at(List* list, const unsigned int index);
void* List_at(const List* list, const unsigned int index);


#ifdef __cplusplus
}
#endif


