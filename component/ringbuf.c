#include "ringbuf.h"

int ringbuf_init(ringbuf_t *rbuf, int size)
{
	if (size <= 0)
	{
        return FUNC_ERR_PARAM;
	}

	rbuf->head = 0;
	rbuf->tail = 0;
	rbuf->size = size;
    rbuf->buf = malloc(size);

    return (rbuf->buf) ? FUNC_API_OK : FUNC_ERR_RESC;
}

int ringbuf_length(ringbuf_t *rbuf)
{
    return (rbuf->tail < rbuf->head) ? (rbuf->tail + rbuf->size - rbuf->head) : (rbuf->tail - rbuf->head);
}

int ringbuf_read(ringbuf_t *rbuf, char *buf, int nbytes)
{
    int i = 0;

    for( ; i < nbytes; i++)
    {
        if(RINGB_IS_EMPTY(rbuf))
        {
            break;
        }

        buf[i] = rbuf->buf[rbuf->head];
        rbuf->head = (rbuf->head + 1) % (rbuf->size);
    }

    return i;
}

int ringbuf_write(ringbuf_t *rbuf, char *buf, int nbytes)
{
    int i = 0;

    for( ; i < nbytes; i++)
    {
        if(RINGB_IS_FULL(rbuf))
        {
            break;
        }

        rbuf->buf[rbuf->tail] = buf[i];
        rbuf->tail = (rbuf->tail + 1) % (rbuf->size);
    }

    return i;
}
