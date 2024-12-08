#ifndef CMD_MANAGER_H
#define CMD_MANAGER_H

#include "version_def.h"
#include "global_def.h"
#include "error_def.h"
#include "file_ctrl.h"
#include "message.h"
#include <stdio.h>

typedef enum bsp_cmd_t
{
    handshake   = 0xc0,
    password,
    version,
    easylog,
    update
}bsp_cmd_t;


void command_manager(app_gbl_t *resc);

#endif