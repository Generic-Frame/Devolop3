#include "dev_manager.h"

static int default_handle(int object, void *param)
{
    Debug("default_handle\n");
}

static int default_handle2(int object, void *param)
{
    Debug("default_handle2\n");
}

static void device_process(int object, void *param, int *state)
{
    Debug("object=%d, param=%s, state = %d\n", object, (char *)param, *state);

	static const FsmTable_t table[] =
	{
		{ power_up  , default_mode , func_locked , default_handle },
        { heartbeat , func_locked  , menu_select , default_handle2 },

	};

    for(int i = 0; i < sizeof(table)/sizeof(table[0]); i++)
    {
        if(object == table[i].event && *state == table[i].cur_state)
        {
            table[i].handle(object, param);
            *state = table[i].next_state;
        }
    }
}

void device_manager(app_gbl_t *resc)
{
    list_t *device_list = resc->buffer[device_index];

    mutex_lock(&resc->locker[device_index]);
    node_t *node = list_remove(device_list);
    mutex_unlock(&resc->locker[device_index]);

    app_event_t *event = container_of(node, app_event_t, link_node);

    device_process(event->object, event->param, &resc->state_app);

    free(event);
}

void device_listener(app_gbl_t *resc, int object, void *param)
{
    app_event_t *event = malloc(sizeof(app_event_t));

    event->object = object;
    event->param = param;

    mutex_lock(&resc->locker[device_index]);
    list_insert(resc->buffer[device_index], &event->link_node);
    mutex_unlock(&resc->locker[device_index]);
}