#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#define Pow2Ceiling(T, num) ((T)internal_Pow2Ceiling(sizeof(T), num))

unsigned long long fnvHash64(const short* buffer, const short* const bufferEnd);
short* FindBufferEnd(const short* buffer);
unsigned long long internal_Pow2Ceiling(unsigned long long size, unsigned long long num);

typedef struct {
    /* An item in a hash table. */
    short* Key;
    void* Value;
} HashTableItem;

typedef struct HashTable {
    unsigned long long Size;
    unsigned long long SlotsUsed;
    unsigned long long* ActiveIndicies;
    HashTableItem* Array;
} HashTable;

HashTable* internal_HashTable_create(unsigned long long itemSize, unsigned long long size);
void HashTable_destroy(HashTable** table);
void HashTable_insert(HashTable* table, const short* alias, void* value);
void HashTable_remove(HashTable* table, const short* alias);
void HashTable_resize(HashTable* table, const unsigned long long size);
#define HashTable_create(T, size) internal_HashTable_create(sizeof(T), (unsigned long long)size);

bool internal_HashTable_find(const HashTable* table, const short* alias, void** outValue);
#define HashTable_find(table, alias, outValue) (internal_HashTable_find(table, alias, (void**)outValue))
#define HashTable_array_iterator(table) (unsigned long long i = 0; i < table->SlotsUsed; i++)
#define HashTable_array_at(T, table, i) ((T*)table->Array[table->ActiveIndicies[i]].Value)
#define HashTable_array_key_at(T, table, i) ((T*)table->Array[table->ActiveIndicies[i]].Key)


#ifdef __cplusplus
}
#endif
