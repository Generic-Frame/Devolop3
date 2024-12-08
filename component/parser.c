#include "parser.h"

static void init_state(app_parser_t *parser)
{
    parser->headFlag = false;
    parser->needLength = sizeof(parser->cache);
    free(parser->message);
    parser->message = NULL;
}

static int middle_state(app_parser_t *parser)
{
    parser->headFlag = true;
    parser->needLength = parser->cache.length;
    parser->message = malloc(sizeof(parser->cache) + parser->needLength);

    if(parser->message)
    {
        *parser->message = parser->cache;
    }

    return !!parser->message;
}

static app_message_t *last_state(app_parser_t *parser)
{
    app_message_t *ret = NULL;

    if(parser->headFlag && !parser->needLength)
    {
        ret = parser->message;
        parser->message = NULL;
    }

    return ret;
}

app_parser_t *parser_new()
{
    app_parser_t *ret = calloc(1,sizeof(app_parser_t));

    if(ret)
        init_state(ret);

    return ret;
}

static int try_recv(void *buf, int length, app_com_ops_t *ops, int fd)
{
    int count = 0;
    int readLen = 0;

    while(readLen < length)
    {
        int len = ops->recv(fd, buf + readLen, length - readLen);

        if(len > 0)
        {
            readLen += len;
        }
        else
        {
            usleep(20000);

            if(count++ > 5)
                break;
        }
    }

    return readLen;
}

app_message_t* parser_from_fd(app_parser_t *parser, app_com_ops_t *ops, int fd)
{
    app_message_t *ret = NULL;
    app_parser_t *ptr = parser;

    int offset, len = 0;

    if(ptr && ops && fd > 0)
    {
        if(ptr->headFlag == false)
        {
            offset = sizeof(ptr->cache) - ptr->needLength;
            len = try_recv((unsigned char *)&ptr->cache + offset, ptr->needLength, ops, fd);

            if(len == ptr->needLength)
            {
                if(middle_state(ptr))
                {
                    ret = parser_from_fd(ptr, ops, fd);
                }
            }
            else
            {
                ptr->needLength -= len;
            }
        }
        else
        {
            if(ptr->message)
            {
                offset = ptr->message->length - ptr->needLength;
                len = try_recv(ptr->message->payload + offset, ptr->needLength, ops, fd);
                ptr->needLength -= len;
            }

            if(ret = last_state(ptr))
                init_state(ptr);
        }
    }

    return ret;
}

app_message_t *parser_from_mem(app_parser_t *parser, unsigned char *mem, unsigned int length)
{
    app_message_t *ret = NULL;
    app_parser_t *ptr = parser;

    int offset, len = (ptr->needLength < length) ? ptr->needLength : length;

    if(ptr && mem && length)
    {
        if(ptr->headFlag == false)
        {
            offset = sizeof(ptr->cache) - ptr->needLength;
            memcpy((unsigned char *)&ptr->cache + offset, mem, len);

            if(ptr->needLength == len)
            {
                middle_state(ptr);
            }
            else
            {
                ptr->needLength -= len;
            }
        }
        else
        {
            if(ptr->message)
            {
                offset = ptr->message->length - ptr->needLength;
                memcpy(ptr->message->payload + offset, mem, len);
                ptr->needLength -= len;
            }
        }

        if(ret = last_state(ptr))
        {
            init_state(ptr);
        }
    }

    return ret;
}