#ifndef RINGBUF_H
#define RINGBUF_H

#include "error_def.h"
#include <stdbool.h>
#include <stdlib.h>

#define RINGB_IS_EMPTY(x)	({	\
		typeof(x) _x = (x);		\
		_x->head == _x->tail ? true : false; })


#define RINGB_IS_FULL(x)	({	\
		typeof(x) _x = (x);		\
		_x->head == (_x->tail + 1) % _x->size ? true : false; })


typedef struct ringbuf_t
{
     char *buf;
     int head;
     int tail;
     int size;
}ringbuf_t;


int ringbuf_init(ringbuf_t *rbuf, int size);
int ringbuf_length(ringbuf_t *rbuf);
int ringbuf_read(ringbuf_t *rbuf, char *buf, int nbytes);
int ringbuf_write(ringbuf_t *rbuf, char *buf, int nbytes);

#endif