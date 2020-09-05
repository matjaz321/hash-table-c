#include "hash_table.h"
#include "stdlib.h"
#include "string.h"
#include <math.h>
#include "prime.h"

int HT_PRIME_1 = 53;
int HT_PRIME_2 = 61;
int HASH_TABLE_INITIAL_BASE_SIZE = 53;

static hashTableItem  HASH_TABLE_DELETED_ITEM = {NULL, NULL};

// Assign memory to the new bucket and add the properties (key, value)
static hashTableItem* hashTableNewItem(const char* k, const char* v) {
    hashTableItem* i = malloc(sizeof(hashTableItem));
    i->key = strdup(k);
    i->value = strdup(v);

    return i;
}

static hashTable* hashTableNewSized(const int baseSize) {
    hashTable* hashTable1 =  malloc(sizeof(hashTable));
    hashTable1->baseSize = baseSize;

    hashTable1->size = nextPrime(hashTable1->baseSize);
    hashTable1->count = 0;
    hashTable1->items = calloc((size_t)hashTable1->size, sizeof(hashTableItem*));

    return hashTable1;
}

static void resize(hashTable* hashTable1, const int baseSize) {
    if (baseSize < HASH_TABLE_INITIAL_BASE_SIZE) {
        return;
    }

    hashTable* newHashTable = hashTableNewSized(baseSize);

    for (int i = 0; i < hashTable1->size; i++) {
        hashTableItem* item = hashTable1->items[i];
        if (item != NULL && item != &HASH_TABLE_DELETED_ITEM) {
            insert(newHashTable, item->key, item->value);
        }
    }

    hashTable1->baseSize = newHashTable->baseSize;
    hashTable1->count = newHashTable->count;

    const int tmpSize = hashTable1->size;
    hashTable1->size = newHashTable->size;
    newHashTable->size = tmpSize;

    hashTableItem** tmpItems = hashTable1->items;
    hashTable1->items = newHashTable->items;
    newHashTable->items = tmpItems;

    deleteHashTable(newHashTable);
}

// Table is resized when the limit load is 0.7
static void resizeUp(hashTable* hashTable1) {
    resize(hashTable1, hashTable1->baseSize * 2);
}


// Table is resized down when the limit is bellow 0.1
static void resizeDown(hashTable* hashTable1) {
    resize(hashTable1, hashTable1->baseSize / 2);
}

// Assign memory to the new hash table
hashTable* newHashTable() {
    return hashTableNewSized(HASH_TABLE_INITIAL_BASE_SIZE);
}

// Free up the memory which will result into deleting a bucket
static void deleteHashTableItem(hashTableItem* item) {
    free(item->key);
    free(item->value);
    free(item);
}

// So before deleting a hash table we need to remove every bucket: Time complexity(O(n)
void deleteHashTable(hashTable* hashTable) {
    for (int i = 0; i < hashTable->size; i++) {
        hashTableItem* item = hashTable->items[i];
        if (item != NULL) {
            deleteHashTableItem(item);
        }
    }

    free(hashTable->items);
    free(hashTable);
}

/**
 * This function basically does the following: (btw any CHF could be used)
 * - It will generate a very large number
 * - It will reduce it by the number of buckets (or size of array)
 *
 * Time complexity: O(n) n = string length
 *
 *
 * Critical stat for hash table is load factor @see https://en.wikipedia.org/wiki/Hash_table#Key_statistics
 *  LOAD FACTOR = n/k
 *  n = the number of entries occupied in hash table
 *  k = the number of buckets
 *
 *  Bscly as load factor grows larger the hash table becomes slower.
 *  The solution is to double the size of the table when the load factor bound is reached
 */
static int generateHash(const char* s, const int primeNumber, const int numberOfBuckets) {
    long hash = 0;
    const int sLen = strlen(s);
    for (int i = 0; i < sLen; ++i) {
        hash += (long)pow(primeNumber, sLen - (i + 1)) * s[i];
        hash = hash % numberOfBuckets;
    }

    return (int)hash;
}

/* Collision resolution.
 * Technique used for handling collisions: Open addressing with double hashing @see https://en.wikipedia.org/wiki/Open_addressing
 */
static int getHash(const char* s, const int numOfBuckets, const int attempt) {
    const int hash1 = generateHash(s, HT_PRIME_1, numOfBuckets);
    const int hash2 = generateHash(s, HT_PRIME_2, numOfBuckets);

    return (hash1 + (attempt * (hash2 + 1)) % numOfBuckets);
}

void insert(hashTable* hashTable, const char* key, const char* value) {
    // Just for simpler math multiple by 1100
    if (hashTable->count * 100 / hashTable->size > 70) {
        resizeUp(hashTable);
    }

    hashTableItem* item = hashTableNewItem(key, value);
    int hash = getHash(item->key, hashTable->size, 0);
    hashTableItem* currentItem = hashTable->items[hash];
    int i = 1;
    while (currentItem != NULL) {
        if (currentItem != &HASH_TABLE_DELETED_ITEM) {
            if (strcmp(currentItem->key, key) == 0) {
                deleteHashTableItem(currentItem);
                hashTable->items[hash] = item;
                return;
            }
        }
        hash = getHash(item->key, hashTable->size, i);
        currentItem = hashTable->items[hash];
        i++;
    }
    hashTable->items[hash] = item;
    hashTable->count++;
}

char* search(hashTable* hashTable, const char* key) {
    int hash = getHash(key, hashTable->size, 0);
    hashTableItem* item = hashTable->items[hash];
    int i = 1;
    while (item != NULL) {
        if (item != &HASH_TABLE_DELETED_ITEM) {
            if (strcmp(item->key, key) == 0) {
                return item->value;
            }
        }

        hash = getHash(key, hashTable->size, i);
        item = hashTable->items[hash];
        i++;
    }

    return NULL;
}

void delete(hashTable* hashTable, const char* key) {
    // For simpler math multiply by 100
    if (hashTable->count * 100 / hashTable->size < 10) {
        resizeDown(hashTable);
    }


    int hash = getHash(key, hashTable->size, 0);
    hashTableItem* item = hashTable->items[hash];
    int i = 1;

    while (item != NULL) {
        if (item != &HASH_TABLE_DELETED_ITEM) {
            if (strcmp(item->key, key) == 0 ) {
                deleteHashTableItem(item);
                hashTable->items[hash] = &HASH_TABLE_DELETED_ITEM;
            }
        }

        hash = getHash(key, hashTable->size, i);
        item = hashTable->items[hash];
        i++;
    }
    hashTable->count++;
}