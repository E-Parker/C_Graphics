#pragma once

#ifdef __cplusplus
extern "C" {
#endif

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
    char* data;             // Pointer to the data block of"head" and "tail".
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

void internal_List_push_front(List* list, void* data);
void internal_List_push_back(List* list, void* data);

void internal_List_pop_front(List* list, void* outVal);
void internal_List_pop_back(List* list, void* outVal);
void internal_List_peak_front(List* list, void* outVal);
void internal_List_peak_back(List* list, void* outVal);

void* List_create_array(List* list);

unsigned int List_count(const List* list);
unsigned int List_byte_count(const List* list);

void List_realloc(List* list, const unsigned int Capacity);
void List_reorder(List* list);
void List_remove_at(List* list, const unsigned int index);
void* List_at(const List* list, const unsigned int index);

void List_append(List* dst, List* src);

#ifdef __cplusplus
}
#endif


