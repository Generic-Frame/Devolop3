#ifndef LINKLIST_H
#define LINKLIST_H

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#define container_of(ptr, type, member) ({      \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - offsetof(type,member) );})


#define list_for_each(pos, head)   for(pos = (head)->next; pos != (NULL); pos = pos->next)


typedef struct node_t
{
	struct node_t *next;
}node_t;

typedef struct list_t
{
	node_t head;
	uint32_t count;
}list_t;


list_t *list_init(void);
int list_length(list_t *list);
node_t *list_remove(list_t *list);
void list_insert(list_t *list, node_t *node);

#endif