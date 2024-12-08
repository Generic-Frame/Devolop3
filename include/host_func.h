#ifndef HOST_FUNC_H
#define HOST_FUNC_H

#include "cmd_manager.h"
#include "slave_func.h"
#include "file_ctrl.h"
#include "error_def.h"
#include "type_def.h"
#include "message.h"
#include "parser.h"

typedef struct dev_proc_t
{
	int dev_fd;
	int dev_com;
	bool dev_is_online;
    app_com_ops_t *ops;
    app_parser_t *parser;
}dev_proc_t;


typedef bool(*dev_online_check_t)(dev_proc_t *dev_proc);

app_message_t *bsp_host_command(bsp_cmd_t cmd, const char* payload, unsigned int length, app_parser_t *parser, app_com_ops_t *ops, int fd);
int bsp_host_data_send(bsp_cmd_t cmd, char *path, app_parser_t *parser, app_com_ops_t *ops, int fd);
int bsp_host_data_recv(bsp_cmd_t cmd, char *path, app_parser_t *parser, app_com_ops_t *ops, int fd);
void auto_scan_slave(dev_proc_t *dev_proc);
dev_proc_t *creat_device();

#endif