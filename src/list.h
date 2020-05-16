#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct list_t {
    void** data;
    int data_length;
    int entries;
    int max_entries;
    int next_size;
} list_t;

list_t* list_create(int);
void list_push(list_t*, void*);
void list_append(list_t*, void*);
void list_remove(list_t*, int, bool);
void list_empty(list_t*, bool);

#endif
