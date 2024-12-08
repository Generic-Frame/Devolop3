#ifndef DEV_MANAGER_H
#define DEV_MANAGER_H

#include "global_def.h"
#include "easylog.h"
#include <stdio.h>

typedef struct FsmTable_t
{
    int event;
    int cur_state;
    int next_state;
    int (*handle)(int,void*);
}FsmTable_t;

void device_manager(app_gbl_t *resc);
void device_listener(app_gbl_t *resc, int object, void *param);

#endif