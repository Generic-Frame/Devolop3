#ifndef EVENT_FUNC_H
#define EVENT_FUNC_H

#include "thread_ctrl.h"
#include "cmd_manager.h"
#include "dev_manager.h"
#include "global_def.h"

#define EVENT_NOT_trigger   (error_index)
#define EVENT_DEV_trigger   (device_index)
#define EVENT_DAT_trigger   (packet_index)


void bsp_event_init(app_gbl_t *resc);

#endif