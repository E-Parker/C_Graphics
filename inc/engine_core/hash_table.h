// Hash table implementation by Ethan Parker.
// 
// To use this library, add a source file with these two lines.
// #define HASH_TABLE_IMPLEMENTATION
// #include "hash_table.h"
// 
// There are internal functions not defined in this header. you may link to them externally.
// See: fnvHash64, and FindBufferEnd.
//

#pragma once

#include "engine_core/engine_types.h"

// TODO: rework this to be able to store structs the same way List can.
typedef struct {
    char* Key;
    void* Value;
} HashTableItem;

typedef struct HashTable {
    u64 Size;
    u64 SlotsUsed;
    u64* ActiveIndicies;
    HashTableItem* Array;
} HashTable;

#define HashTable_create(T, size) internal_HashTable_create(sizeof(T), (u64)size);
#define HashTable_initialize(T, table, size) internal_HashTable_initialize(table, sizeof(T), (u64)size)

HashTable* internal_HashTable_create(u64 itemSize, u64 size);
HashTable* internal_HashTable_initialize(HashTable* table, i64 itemSize, u64 size);

void HashTable_destroy(HashTable** table);
void HashTable_deinitialize(HashTable* table);

char* HashTable_insert(HashTable* table, const char* alias, void* value);
void HashTable_remove(HashTable* table, const char* alias);
void HashTable_resize(HashTable* table, const u64 size);

bool internal_HashTable_find(const HashTable* table, const char* alias, void** outValue);
#define HashTable_find(table, alias, outValue) (internal_HashTable_find(table, alias, (void**)outValue))
#define HashTable_array_iterator(table) u64 i = 0; i < (table)->SlotsUsed; i++
#define HashTable_array_at(T, table, i) ((T*)(table)->Array[(table)->ActiveIndicies[i]].Value)
#define HashTable_array_key_at(T, table, i) ((T*)(table)->Array[(table)->ActiveIndicies[i]].Key)

#ifdef HASH_TABLE_IMPLEMENTATION

#include "stdlib.h"
#include "string.h"

// Internal Functions
// 
//

#define Pow2Ceiling(T, num) ((T)internal_Pow2Ceiling(sizeof(T), num))

u64 fnvHash64(const char* buffer, const char* const bufferEnd) {
    // implementation of the fnv64 hashing function, created by Glenn Fowler, Landon Curt Noll,
    // and Kiem-Phong Vo. I used fixed-width integers here for maximum portability.
    // 

    const u64 Prime = 0x00000100000001B3;
    u64 Hash = 0xCBF29CE484222325;

    char* bufferIter = (char*)buffer;

    // Iterate from the buffer start address to the buffer end address.
    for (; bufferIter < bufferEnd; bufferIter++) {
        //XOR the current hash with the current character, then multiply by an arbitrary prime number.
        Hash = (Hash ^ (*bufferIter)) * Prime;
    }

    return Hash;
}


char* FindBufferEnd(const char* buffer) {
    // Find the end of a buffer from a given c-string. 

    char* bufferEnd = (char*)buffer;

    // Assume the key is a c_string, iterate through to find the null terminator.
    for (u16 i = 0; i < 0xffff; i++) {
        if (*bufferEnd == '\0') {
            return bufferEnd;
        }
        bufferEnd++;
    }
    return NULL;
}


u64 internal_Pow2Ceiling(u64 size, u64 num) {
    /* This function returns the next nearest power of 2 from the input number. */

    num = num > 1 ? num - 1 : 1;

    u64 iterations = size * 8;

    for (int i = 1; i < iterations; i = i << 1) {
        num |= num >> i;
    }
    
    return ++num;
}


HashTable* internal_HashTable_create(u64 itemSize, u64 size) {
    u64 Size = (size < 16) ? 16 : Pow2Ceiling(u64, size);

    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    if (!table) goto TableMallocFailure;
    
    table->Array = (HashTableItem*)calloc(Size, sizeof(HashTableItem));
    if (!table->Array) goto TableArrayMallocFalure;
    
    table->ActiveIndicies = (u64*)calloc(Size, sizeof(u64));
    if (!table->ActiveIndicies) goto TableActiveIndiciesFalure;

    table->Size = Size;
    table->SlotsUsed = 0;
    return table;

    TableActiveIndiciesFalure:
        free(table->Array);

    TableArrayMallocFalure:
        free(table);

    TableMallocFailure:
        return NULL;
}

// TODO: consider consolidating internal_HashTable_create and internal_HashTable_initialize. Literally the same code twice.
HashTable* internal_HashTable_initialize(HashTable* table, i64 itemSize, u64 size) {
    table->Array = (HashTableItem*)calloc(size, sizeof(HashTableItem));
    if (!table->Array) goto TableArrayMallocFalure;

    table->ActiveIndicies = (u64*)calloc(size, sizeof(u64));
    if (!table->ActiveIndicies) goto TableActiveIndiciesFalure;

    table->Size = size;
    table->SlotsUsed = 0;
    return table;

    TableActiveIndiciesFalure:
        free(table->Array);

    TableArrayMallocFalure:
        free(table);
        return NULL;
}

// Public Functions:
//
//

void HashTable_destroy(HashTable** table) {

    for (u64 i = 0; i < (*table)->Size; i++) {
         
        HashTableItem item = (*table)->Array[i];

        // If there is no value to delete, continue.
        if (item.Value == NULL) {
            continue;
        }

        free(item.Value);
    }

    free((*table)->Array);
    free(*table);
    *table = NULL;

}


void HashTable_deinitialize(HashTable* table) {
    for (u64 i = 0; i < table->Size; i++) {

        HashTableItem item = table->Array[i];

        // If there is no value to delete, continue.
        if (item.Value == NULL) {
            continue;
        }

        free(item.Value);
    }

    free(table->Array);
    table->Size = 0;
    table->Array = NULL;
}

char* HashTable_insert(HashTable* table, const char* key, void* value) {

    // if out of space, double the size.
    if(++table->SlotsUsed == table->Size) {
        HashTable_resize(table, table->Size << 1);
    }

    char* keyEnd = FindBufferEnd(key);
    u64 hash = fnvHash64(key, keyEnd) % table->Size;
    u64 originalHash = hash;

    while (table->Array[hash].Key != NULL) {

        if (strcmp(key, table->Array[hash].Key) == 0) {
            break;
        }

        hash++;
        hash %= table->Size;

        if (originalHash == hash) {
            // Something has gone wrong and there is no space.
            assert(false);
        }
    }
    
    // Copy the key across
    table->Array[hash].Key = (char*)malloc((keyEnd - key + 1) * sizeof(char));
    assert(table->Array[hash].Key != NULL);

    memcpy(table->Array[hash].Key, key, (keyEnd - key + 1) * sizeof(char));
    table->Array[hash].Value = value;
    table->ActiveIndicies[table->SlotsUsed - 1] = hash;

    return table->Array[hash].Key;
}


void HashTable_remove(HashTable* table, const char* key) {

    char* keyEnd = FindBufferEnd(key);
    u64 hash = fnvHash64(key, keyEnd) % table->Size;
    u64 originalHash = hash;

    while (table->Array[hash].Key != NULL) {

        if (strcmp(key, table->Array[hash].Key) == 0) {
            free(table->Array[hash].Key);
            free(table->Array[hash].Value);
            table->Array[hash].Key = NULL;
            table->Array[hash].Value = NULL;
            
            u64 i = 0;
            for (; i < table->SlotsUsed; i++) {
                if (table->ActiveIndicies[i] == hash) break;
            }

            for (u64 k = i + 1; k < table->SlotsUsed; k++) {
                table->ActiveIndicies[k - 1] = table->ActiveIndicies[k];
            }

            table->SlotsUsed--;
            return;
        }

        hash++;
        hash %= table->Size;

        if (originalHash == hash) {
            return;
        }
    }
}


void HashTable_resize(HashTable* table, const u64 size) {
    // Resize a hash table to the nearest power of 2 to the size provided. (values less than 16 will be rounded up to 16).


    u64 newSize = (size <= 16) ? 16 : Pow2Ceiling(u64, size);

    // if the table is already the size provided, skip resizing.
    if (table->Size == newSize) {
        return;
    }

    HashTableItem* Temp = (HashTableItem*)calloc(newSize, sizeof(HashTableItem));
    u64* TempIndicies = (u64*)calloc(newSize, sizeof(u64));
    assert(Temp != NULL);
    assert(TempIndicies != NULL);

    for (u64 i = 0; i < table->Size; i++) {

        // if this slot was unused, skip it.
        if (table->Array[i].Key == NULL) {
            continue;
        }

        // Generate the hash for the item.
        u64 hash = fnvHash64(table->Array[i].Key, FindBufferEnd(table->Array[i].Key)) % newSize;
        u64 originalHash = hash;

        // Check for collisions, linearly probe for a free slot.
        while (Temp[hash].Key != NULL) {
            hash++;
            hash %= newSize;

            if (originalHash == hash) {
                // something went really wrong and an open slot could not be found.
                assert(false);
            }
        }
        // Store the item from the old array in the temp one.
        table->ActiveIndicies[i] = hash;
        Temp[hash] = table->Array[i];
    }

    free(table->ActiveIndicies);
    table->ActiveIndicies = TempIndicies;
    
    free(table->Array);
    table->Array = Temp;
    
    Temp = NULL;
    TempIndicies = NULL;
    table->Size = newSize;
}


bool internal_HashTable_find(const HashTable* table, const char* key, void** outValue) {

    u64 hash = fnvHash64(key, FindBufferEnd(key)) % table->Size;
    u64 originalHash = hash;

    while (table->Array[hash].Key != NULL) {

        if (strcmp(key, table->Array[hash].Key) == 0) {
            break;
        }

        hash++;
        hash %= table->Size;

        if (originalHash == hash) {
            *outValue = NULL;
            return false;
        }
    }
    *outValue = table->Array[hash].Value;
    return true;
}
#endif