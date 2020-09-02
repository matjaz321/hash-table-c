#include "hash_table.h"

int main() {
    hashTable* hashTable = newHashTable();
    insert(hashTable, (const char*)'d', (const char*)'a');
}
