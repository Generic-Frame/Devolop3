#include "message.h"

app_message_t *message_create(unsigned short cmd, const char *payload, unsigned int length)
{
    app_message_t *ret = malloc(sizeof(app_message_t) + length);

    if(ret)
    {
        ret->cmd = cmd;
        ret->length = length;

        if(payload)
        {
            memcpy(ret + 1, payload, length);
        }
    }

    return ret;
}

void message_schedule(app_message_t *msg, unsigned short index, unsigned short total)
{
    msg->index = index;
    msg->total = total;
}

int message_size(app_message_t *msg)
{
    int ret = 0;

    if(msg)
    {
        ret = sizeof(app_message_t) + msg->length;
    }

    return ret;
}
