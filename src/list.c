#include "list.h"

list_t* list_create(int start_size)
{
    list_t* list = malloc(sizeof(list_t));
    list->data = malloc(sizeof(void*) * start_size);
    list->count = 0;
    list->max_entries = start_size;

    return list;
}

void list_push(list_t* list, void* data)
{
    if (list->count == list->max_entries) {
        list->data = realloc(list->data, sizeof(void*) * list->max_entries * 2);
    }
    
    memcpy(&list->data[1], list->data, sizeof(void*)*list->max_entries);
    list->data[0] = data;

    if (list->count == list->max_entries) {
        list->max_entries *= 2;
    }
    list->count++;
}

void list_append(list_t* list, void* data)
{
    if (list->count == list->max_entries) {
        list->data = realloc(list->data, sizeof(void*) * list->max_entries * 2);
    }

    list->data[list->count++] = data;

    if (list->count-1 == list->max_entries) {
        list->max_entries *= 2;
    }
}

void list_remove(list_t* list, int index, bool f)
{
    if (f)
        free(list->data[index]);

    list->data[index] = NULL;

    if (list->count < list->max_entries) {
        memcpy(&list->data[index], &list->data[index+1], sizeof(void*) * (list->max_entries-(index+1)));
    }

    list->count--;
}

void list_empty(list_t* list, bool f)
{
    if (f)
        for (int i = 0; i < list->count; i++)
            free(list->data[i]);
    
    list->count = 0;
}
