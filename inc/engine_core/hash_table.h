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
#include "engine_core/string.h"

typedef struct HashTable {
    u64 capacity;
    u64 itemSize;
    u64 slotsUsed;
    u64* activeIndicies;
    String* keys;
    u8* values;
} HashTable;

#define HashTable_create(T, capacity) internal_HashTable_create(sizeof(T), (u64)capacity);
#define HashTable_initialize(T, table, capacity) internal_HashTable_initialize(table, sizeof(T), (u64)capacity)

HashTable* internal_HashTable_create(u64 itemSize, u64 capacity);
void internal_HashTable_initialize(HashTable* table, u64 itemSize, u64 capacity);

void HashTable_destroy(HashTable** table);
void HashTable_deinitialize(HashTable* table);

// Insert a new value, or overwrite an existing one.
void HashTable_insert(HashTable* table, const String key, void* value);
bool HashTable_remove(HashTable* table, const String key);
void HashTable_resize(HashTable* table, const u64 capacity);

// Get the value of what's stored in the HashTable by value.
#define HashTable_find(table, key, out) (internal_HashTable_find(table, key, (void*)(&out)))
bool internal_HashTable_find(const HashTable* table, const String key, void* out);

// Get the value of what's stored in the HashTable by reference. These values should be only used temporarily, as the pointer will change if the table is reallocated.
bool HashTable_find_reference(const HashTable* table, const String key, void** outVal);

#define HashTable_array_iterator(table) u64 i = 0; i < (table)->slotsUsed; i++
#define HashTable_array_at(T, table, i) (T*)(&(table)->values[(table)->activeIndicies[i] * ((table)->itemSize)])
#define HashTable_array_key_at(table, i) (&(table)->keys[(table)->activeIndicies[i]])

//#define HASH_TABLE_IMPLEMENTATION
#ifdef HASH_TABLE_IMPLEMENTATION

#include "stdlib.h"
#include "string.h"

// Internal Functions
// 
//

#define Pow2Ceiling(T, num) ((T)internal_Pow2Ceiling(sizeof(T), num))

u64 fnvHash64 (const char* buffer, const char* const bufferEnd) {
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


char* FindBufferEnd (const char* buffer) {
    // Find the end of a buffer from a given c-string. 

    char* bufferEnd = (char*)buffer;

    // Assume the key is a c_string, iterate through to find the null terminator.
    for (u16 i = 0; i < 0xffff; ++i, ++bufferEnd) {
        if (*bufferEnd == '\0') {
            return bufferEnd;
        }
    }
    return NULL;
}

u64 internal_Pow2Ceiling (u64 size, u64 num) {
    /* This function returns the next nearest power of 2 from the input number. */

    num = num > 1 ? num - 1 : 1;

    u64 iterations = size * 8;

    for (int i = 1; i < iterations; i = i << 1) {
        num |= num >> i;
    }
    
    return ++num;
}


HashTable* internal_HashTable_create (u64 itemSize, u64 capacity) {
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    internal_HashTable_initialize(table, itemSize, capacity);
    
    if (!table->activeIndicies) {
        free(table);
        table = NULL;
    }

    return table;
}

void internal_HashTable_initialize (HashTable* table, u64 itemSize, u64 capacity) {
    u64 Capacity = (capacity < 16) ? 16 : Pow2Ceiling(u64, capacity);

    if (!table) goto TableMallocFailure;

    table->keys = (String*)calloc(Capacity, sizeof(String));
    if (!table->keys) goto TableKeyArrayMallocFalure;

    table->values = calloc(Capacity, itemSize);
    if (!table->values) goto TableValueArrayMallocFailure;

    table->activeIndicies = (u64*)calloc(Capacity, 8);
    if (!table->activeIndicies) goto TableActiveIndiciesFalure;

    table->capacity = Capacity;
    table->itemSize = itemSize;
    table->slotsUsed = 0;
    return;

TableActiveIndiciesFalure:
    free(table->values);

TableValueArrayMallocFailure:
    free(table->keys);

TableKeyArrayMallocFalure:
    free(table);

TableMallocFailure:
    return;
}

// Public Functions:
//
//

void HashTable_destroy (HashTable** table) {
    HashTable_deinitialize(*table);
    free((*table));
    *table = NULL;
}

void HashTable_deinitialize (HashTable* table) {
    
    for (HashTable_array_iterator(table)) {
        String* key = HashTable_array_key_at(table, i);
        String_free_dirty(key);
    }

    free(table->activeIndicies);
    free(table->keys);
    free(table->values);
}

//#include "stdio.h"

void HashTable_insert (HashTable* table, const String key, void* value) {
    // if out of space, increase size by 1.5.
    if(table->slotsUsed == table->capacity) {
        HashTable_resize(table, (table->capacity + (table->capacity >> 1)));
    }

    u64 hash = fnvHash64(key.start, key.end) % table->capacity;
    u64 originalHash = hash;

    //printf("insert\t c %u h %u, l %u, %s \n", table->capacity, hash, String_length(key), key.start);

    // Check for collisions. Insert at the next free location.
    while (table->keys[hash].start) {

        // index already exists. Overwrite.
        if (String_equal(key, table->keys[hash])) {
            goto WriteValue;
        }

        hash++;
        hash %= table->capacity;

        if (hash == originalHash) {
            Engine_exit_forced(-1);
        }
    }
    
    // Allocate and copy the string across.
    String_create_dirty(&key, &(table->keys[hash]));

WriteValue:
    for (u64 i = 0; i < table->itemSize; ++i) {
        table->values[(hash * table->itemSize) + i] = ((u8*)value)[i];
    }

    table->activeIndicies[table->slotsUsed] = hash;
    table->slotsUsed++;
}


bool HashTable_remove (HashTable* table, const String key) {
    u64 hash = fnvHash64(key.start, key.end) % table->capacity;
    u64 originalHash = hash;

    while (table->keys[hash].start) {
        if (String_equal(key, table->keys[hash])) {
            goto ItemFound;
        }

        hash++;
        hash %= table->capacity;

        if (originalHash == hash) {
            goto ItemNotFound;
        }
    }

ItemNotFound:
    return false;
    
    // TODO: implement some system to handle correcting other items after one is removed since all colliding items have to move.

ItemFound:
    String_free_dirty(&table->keys[hash]);

    u64 i = 0;
    for (; i < table->slotsUsed; i++) {
        if (table->activeIndicies[i] == hash) break;
    }

    for (u64 k = i + 1; k < table->slotsUsed; k++) {
        table->activeIndicies[k - 1] = table->activeIndicies[k];
    }

    table->slotsUsed--;
    return true;
}


void HashTable_resize (HashTable* table, const u64 size) {
    // Resize a hash table to the nearest power of 2 to the size provided. (values less than 16 will be rounded up to 16).
    u64 capacity = (size <= 16) ? 16 : Pow2Ceiling(u64, size);

    // if the table is already the size provided, skip resizing.
    if (table->capacity == capacity) {
        return;
    }

    u64* activeIndicies = (u64*)calloc(capacity, sizeof(u64));
    Engine_validate(activeIndicies, ENOMEM);

    String* keys = (String*)calloc(capacity, sizeof(String));
    Engine_validate(keys, ENOMEM);

    u8* values = (u8*)calloc(capacity, table->itemSize);
    Engine_validate(values, ENOMEM);

    u64 slotsUsed = 0;

    for (HashTable_array_iterator(table)) {
        void* value = HashTable_array_at(void, table, i);
        String* key = HashTable_array_key_at(table, i);

        u64 hash = fnvHash64(key->start, key->end) % capacity;
        u64 oldHash = fnvHash64(key->start, key->end) % table->capacity;
        u64 originalHash = hash;

        while (keys[hash].start) {
            hash++;
            hash %= capacity;

            if (hash == originalHash) {
                Engine_exit_forced(-1);
            }
        }

        for (u64 i = 0; i < table->itemSize; ++i) {
            values[(hash * table->itemSize) + i] = table->values[(oldHash * table->itemSize) + i];
        }

        keys[hash].start = key->start;
        keys[hash].end = key->end;

        activeIndicies[slotsUsed] = hash;
        slotsUsed++;
    }

    free(table->activeIndicies);
    free(table->keys);
    free(table->values);

    table->capacity = capacity;
    table->activeIndicies = activeIndicies;
    table->keys = keys;
    table->values = values;
}


bool internal_HashTable_find (const HashTable* table, const String key, void* out) {
    void* outref;
    if (!HashTable_find_reference(table, key, &outref)) {
        return false;
    }

    u64 hash = fnvHash64(key.start, key.end) % table->capacity;
    
    for (u64 i = 0; i < table->itemSize; ++i) {
        ((u8*)out)[i] = table->values[(hash * table->itemSize) + i];
    }

    return true;
}

bool HashTable_find_reference (const HashTable* table, const String key, void** outVal) {
    u64 hash = fnvHash64(key.start, key.end) % table->capacity;
    u64 originalHash = hash;

    //printf("find\t c %u h %u, l %u, %s \n", table->capacity, hash, String_length(key), key.start);

    if (String_invalid(table->keys[hash])) {
        goto ItemNotFound;
    }

    while (table->keys[hash].start) {
        if (String_equal(key, table->keys[hash])) {
            goto ItemFound;
        }

        hash++;
        hash %= table->capacity;

        if (hash == originalHash) {
            goto ItemNotFound;
        }
    }

ItemNotFound:
    if (outVal) *outVal = NULL;
    return false;

ItemFound:
    if (outVal) *outVal = (void*)(&table->values[hash * table->itemSize]);
    return true;
}

#endif