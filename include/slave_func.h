#ifndef SLAVE_FUNC_H
#define SLAVE_FUNC_H

#include "thread_ctrl.h"
#include "global_def.h"
#include "uart_ctrl.h"
#include "message.h"
#include "ringbuf.h"
#include "parser.h"

#define UART_PATH   ("/dev/ttyS0")
#define UART_RATE  (B115200)
#define RINGB_LEN   (1024)

void bsp_slave_init(app_gbl_t *resc);

#endif