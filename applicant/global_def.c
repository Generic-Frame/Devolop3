#include "global_def.h"

app_gbl_t *app_global_def()
{
    app_gbl_t *ret = malloc(sizeof(app_gbl_t));
    ret->state_app = default_mode;

    for(int i = 0; i < sizeof(ret->buffer)/sizeof(ret->buffer[0]); i++)
    {
        mutex_init(&ret->locker[i]);
        ret->buffer[i] = list_init();
    }

    if(ret)
    {
        ret->ops = get_stream_capacity();
    }

    return ret;
}

