#include "hash_table.c"

int main() {
    hashTable* hashTable = newHashTable();
    deleteHashTable(hashTable);
}