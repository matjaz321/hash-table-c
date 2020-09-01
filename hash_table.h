//
// Created by matjaz on 01.09.20.
//

#ifndef C_HASH_TABLES_HASH_TABLE_H
#define C_HASH_TABLES_HASH_TABLE_H

typedef struct {
    char* key;
    char* value;
} hashTableItem;

typedef struct {
    int size;
    int count;
    hashTableItem** items;
} hashTable;

#endif //C_HASH_TABLES_HASH_TABLE_H
