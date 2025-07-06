#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "cStringUtilities.h"
#include "hash_table.h"

// Internal Functions
// 
//

#define Pow2Ceiling(T, num) ((T)internal_Pow2Ceiling(sizeof(T), num))

unsigned long long internal_Pow2Ceiling(unsigned long long size, unsigned long long num) {
    /* This function returns the next nearest power of 2 from the input number. */

    assert(num > 1);

    unsigned long long iterations = size * 8;
    num--;

    for (int i = 1; i < iterations; i = i << 1) {
        num |= num >> i;
    }
    
    return ++num;
}

// Public Functions:
//
//

HashTable* internal_HashTable_create(unsigned long long itemSize, unsigned long long size) {

    unsigned long long Size = (size < 16) ? 16 : Pow2Ceiling(unsigned long long, size);

    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    assert(table != NULL);

    table->Array = (HashTableItem*)calloc(Size, sizeof(HashTableItem));
    assert(table->Array != NULL);

    table->ActiveIndicies = (unsigned long long*)calloc(Size, sizeof(unsigned long long));
    assert(table->ActiveIndicies != NULL);

    table->Size = Size;
    table->SlotsUsed = 0;
    return table;
}


void HashTable_destroy(HashTable** table) {

    for (unsigned long long i = 0; i < (*table)->Size; i++) {
         
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

char* HashTable_insert(HashTable* table, const char* key, void* value) {

    // if out of space, double the size.
    if(++table->SlotsUsed == table->Size) {
        HashTable_resize(table, table->Size << 1);
    }

    char* keyEnd = FindBufferEnd(key);
    unsigned long long hash = fnvHash64(key, keyEnd) % table->Size;
    unsigned long long originalHash = hash;

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
    unsigned long long hash = fnvHash64(key, keyEnd) % table->Size;
    unsigned long long originalHash = hash;

    while (table->Array[hash].Key != NULL) {

        if (strcmp(key, table->Array[hash].Key) == 0) {
            free(table->Array[hash].Key);
            free(table->Array[hash].Value);
            table->Array[hash].Key = NULL;
            table->Array[hash].Value = NULL;
            
            unsigned long long i = 0;
            for (; i < table->SlotsUsed; i++) {
                if (table->ActiveIndicies[i] == hash) break;
            }

            for (unsigned long long k = i + 1; k < table->SlotsUsed; k++) {
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


void HashTable_resize(HashTable* table, const unsigned long long size) {
    // Resize a hash table to the nearest power of 2 to the size provided. (values less than 16 will be rounded up to 16).


    unsigned long long newSize = (size <= 16) ? 16 : Pow2Ceiling(unsigned long long, size);

    // if the table is already the size provided, skip resizing.
    if (table->Size == newSize) {
        return;
    }

    HashTableItem* Temp = (HashTableItem*)calloc(newSize, sizeof(HashTableItem));
    unsigned long long* TempIndicies = (unsigned long long*)calloc(newSize, sizeof(unsigned long long));
    assert(Temp != NULL);
    assert(TempIndicies != NULL);

    for (unsigned long long i = 0; i < table->Size; i++) {

        // if this slot was unused, skip it.
        if (table->Array[i].Key == NULL) {
            continue;
        }

        // Generate the hash for the item.
        unsigned long long hash = fnvHash64(table->Array[i].Key, FindBufferEnd(table->Array[i].Key)) % newSize;
        unsigned long long originalHash = hash;

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

    unsigned long long hash = fnvHash64(key, FindBufferEnd(key)) % table->Size;
    unsigned long long originalHash = hash;

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

