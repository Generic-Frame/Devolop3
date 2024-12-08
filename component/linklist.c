#include "linklist.h"

list_t *list_init(void)
{
    list_t *list = malloc(sizeof(list_t));

    if(list)
    {
        list->head.next = NULL;
        list->count = 0;
    }

	return list;
}

int list_length(list_t *list)
{
    return list->count;
}

node_t *list_remove(list_t *list)
{
    node_t *node = list->head.next;

    if(node)
    {
        list->head.next = node->next;
        list->count--;
    }

    return node;
}

void list_insert(list_t *list, node_t *node)
{
    node_t *last = &list->head;

    for(int i = 0; i < list->count; i++)
    {
        last = last->next;
    }

    last->next = node;
    node->next = NULL;
    list->count++;
}