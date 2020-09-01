#include "hash_table.h"
#include "stdlib.h"
#include "string.h"

static hashTableItem* hashTableNewItem(const char* k, const char* v) {
    hashTableItem* i = malloc(sizeof(hashTableItem));
    i->key = strdup(k);
    i->value = strdup(v);

    return i;
}

hashTable* newHashTable() {
    hashTable* hashTable = malloc(sizeof(hashTable));

    hashTable->size = 53;
    hashTable->count = 0;
    hashTable->items = calloc((size_t)hashTable->size, sizeof(hashTableItem*));

    return hashTable;
}

static void deleteHashTableItem(hashTableItem* item) {
    free(item->key);
    free(item->value);
    free(item);
}

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