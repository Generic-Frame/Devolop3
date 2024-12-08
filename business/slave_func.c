#include "slave_func.h"

static ringbuf_t ringbuf = { NULL };

static void *msg_parser_loop(void *arg)
{
    app_gbl_t *resc = (app_gbl_t *)arg;

    list_t *packet_list = resc->buffer[packet_index];

    app_parser_t *parser = parser_new();

    while( true )
    {
        unsigned char buf[1] = {0};

        int len = ringbuf_read(&ringbuf, buf, sizeof(buf));

        if(len > 0)
        {
            app_message_t *msg = parser_from_mem(parser, buf ,sizeof(buf));

            if(msg && packet_list)
            {
                app_packet_t *packet = malloc(sizeof(app_packet_t));
                packet->app_msg = msg;

                mutex_lock(&resc->locker[packet_index]);
                list_insert(packet_list, &packet->link_node);
                mutex_unlock(&resc->locker[packet_index]);
            }
        }
    }
}

static void *date_recv_loop(void *arg)
{
    app_gbl_t *resc = (app_gbl_t *)arg;
    app_com_ops_t *ops = resc->ops;

	unsigned char buf[MSG_SIZE] = {0};

    while( true )
    {
        memset(buf, 0, sizeof(buf));
        int len = ops->recv(resc->slave_fd, buf, sizeof(buf));

        if(len > 0)
        {
            ringbuf_write(&ringbuf, buf, len);
        }
    }
}

void bsp_slave_init(app_gbl_t *resc)
{
    int fd = open_uart(UART_PATH);

    if(fd > 0)
    {
        resc->slave_fd = fd;
        init_uart(fd, UART_RATE);
        
        ringbuf_init(&ringbuf, RINGB_LEN);

        work_thread_variable_init(resc, date_recv_loop);
        work_thread_variable_init(resc, msg_parser_loop);
    }
}