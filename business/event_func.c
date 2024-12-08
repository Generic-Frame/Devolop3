#include "event_func.h"

static int bsp_scan_trigger(void *arg)
{
    app_gbl_t *resc = (app_gbl_t *)arg;

    int count = 0, ret = error_index;

    bool trigger = false;

    for( ; count < sizeof(resc->buffer)/sizeof(resc->buffer[0]); count++)
    {
        int len = list_length(resc->buffer[count]);

        if(len > 0)
        {
            trigger = true;
            break;
        }
    }

    if(trigger)
    {
        ret += count;
        ret += 1;
    }

    return ret;
}

static void *bsp_listener_loop(void *arg)
{
    app_gbl_t *resc = (app_gbl_t *)arg;

    while( true )
    {
        int ret = bsp_scan_trigger(resc);

        switch(ret)
        {
            case EVENT_NOT_trigger:
                break;
            case EVENT_DEV_trigger:
                device_manager(resc);
                break;
            case EVENT_DAT_trigger:
                command_manager(resc);
                break;
            default:
                break;
        }
    }
}

void bsp_event_init(app_gbl_t *resc)
{
    work_thread_variable_init(resc, bsp_listener_loop);
}