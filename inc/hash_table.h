#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

typedef struct {
    /* An item in a hash table. */
    char* Key;
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
char* HashTable_insert(HashTable* table, const char* alias, void* value);
void HashTable_remove(HashTable* table, const char* alias);
void HashTable_resize(HashTable* table, const unsigned long long size);
#define HashTable_create(T, size) internal_HashTable_create(sizeof(T), (unsigned long long)size);

bool internal_HashTable_find(const HashTable* table, const char* alias, void** outValue);
#define HashTable_find(table, alias, outValue) (internal_HashTable_find(table, alias, (void**)outValue))
#define HashTable_array_iterator(table) unsigned long long i = 0; i < table->SlotsUsed; i++
#define HashTable_array_at(T, table, i) ((T*)table->Array[table->ActiveIndicies[i]].Value)
#define HashTable_array_key_at(T, table, i) ((T*)table->Array[table->ActiveIndicies[i]].Key)


#ifdef __cplusplus
}
#endif
