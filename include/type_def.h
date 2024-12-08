#ifndef TYPE_DEFINE_H
#define TYPE_DEFINE_H

#include "mutex_ctrl.h"
#include "linklist.h"
#include <stdbool.h>

typedef struct app_com_ops_t
{
	int (*recv)(int fd, void *data, int length);
	int (*send)(int fd, void *data, int length);
}app_com_ops_t;

typedef struct app_message_t
{
	unsigned short type;
	unsigned short cmd;
	unsigned short index;
	unsigned short total;
	unsigned int length;
	unsigned char payload[];
}app_message_t;

typedef struct app_parser_t
{
	bool headFlag;
	int needLength;
	app_message_t cache;
	app_message_t *message;
}app_parser_t;

typedef struct app_event_t
{
    int object;
    void *param;
    node_t link_node;
}app_event_t;

typedef struct app_packet_t
{
    app_message_t *app_msg;
    node_t link_node;
}app_packet_t;

typedef struct app_gbl_t
{
	int slave_fd;
	int state_app;
	mutex_t locker[2];
	list_t *buffer[2];
	app_com_ops_t *ops;
}app_gbl_t;

#endif