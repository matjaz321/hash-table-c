#ifndef C_HASH_TABLES_HASH_TABLE_H
#define C_HASH_TABLES_HASH_TABLE_H

typedef struct {
    char* key;
    char* value;
} hashTableItem;

typedef struct {
    int size;
    int count;
    int baseSize;
    hashTableItem** items;
} hashTable;

hashTable* newHashTable();
void deleteHashTable(hashTable* hashTable);
void insert(hashTable* hashTable, const char* key, const char* value);
char* search(hashTable* hashTable, const char* key);
#endif //C_HASH_TABLES_HASH_TABLE_H
