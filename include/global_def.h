#ifndef GLOBAL_DEF_H
#define GLOBAL_DEF_H

#include "stream_ctrl.h"
#include "mutex_ctrl.h"
#include "type_def.h"

typedef enum gbl_event_t
{
    power_up,
	heartbeat,
}gbl_event_t;

typedef enum gbl_state_t
{
	default_mode,
	func_locked,
	menu_select,
	menu_start,
}gbl_state_t;

typedef enum gbl_index_t
{
    error_index		= -1,
	device_index,
	packet_index,
}gbl_index_t;

app_gbl_t *app_global_def();

#endif