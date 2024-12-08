#include "host_func.h"

app_message_t *bsp_host_command(bsp_cmd_t cmd, const char *payload, unsigned int length, app_parser_t *parser, app_com_ops_t *ops, int fd)
{
    app_message_t *msg = message_create(cmd, payload, length);

    ops->send(fd, msg, message_size(msg));

    free(msg);

    return parser_from_fd(parser, ops, fd);
}

//bsp_host_data_send(update, "/main.out", parser, ops, fd);
int bsp_host_data_send(bsp_cmd_t cmd, char *path, app_parser_t *parser, app_com_ops_t *ops, int fd)
{
    int file_size = get_file_size(path);
    int payload_size = MSG_SIZE - sizeof(app_message_t);

    int total = (file_size / payload_size) + (file_size % payload_size > 0 ? 1 : 0);

    app_message_t *msg = message_create(cmd, NULL, payload_size);

    FILE *fp = fopen(path, "rb");
    
    if(fp)
    {
        for(int i = 1; i <= total; i++)
        {
            msg->length = fread(msg->payload, 1, msg->length, fp);

            message_schedule(msg, i, total);

            ops->send(fd, msg, message_size(msg));

            app_message_t *ack = parser_from_fd(parser, ops, fd);

            free(ack);
        }
    }

    fclose(fp);

    free(msg);

    return FUNC_API_OK;
}

//bsp_host_data_recv(easylog, "./easylog.txt", parser, ops, fd);
int bsp_host_data_recv(bsp_cmd_t cmd, char *path, app_parser_t *parser, app_com_ops_t *ops, int fd)
{
    int file_size = 0;

    app_message_t *ack = NULL;

    app_message_t *msg = message_create(cmd, NULL, 0);

    FILE *fp = fopen(path, "wb");

    do
    {
        free(ack);

        ops->send(fd, msg, message_size(msg));

        ack = parser_from_fd(parser, ops, fd);
    
        file_size += ack->length;

        fwrite(ack->payload, 1, ack->length, fp);

    } while(ack->index != ack->total);

    fclose(fp);

    free(ack);
    free(msg);

    return FUNC_API_OK;
}

bool dev_online_check(dev_proc_t *dev_proc)
{
    int dev_fd = dev_proc->dev_fd;

    app_com_ops_t *ops = dev_proc->ops;

    app_parser_t *parser = dev_proc->parser;

    app_message_t *msg = bsp_host_command(handshake, NULL, 0, parser, ops, dev_fd);

    return (msg && msg->cmd == FUNC_API_OK) ? true : false;
}

bool try_open_uart(dev_proc_t *dev_proc, dev_online_check_t func)
{
    if( !dev_proc || !func )
        return false;

    for(int i = 0; i < 256; i++)
    {
        unsigned char path[32] = {0};

        sprintf(path, "/dev/ttyUSB%d",i);

        int fd = open_uart(path);

        if( fd < 0)
        {
            continue;
        }
        else
        {
            init_uart(fd, UART_RATE);
            dev_proc->dev_fd = fd;

            if(func(dev_proc))
            {
                dev_proc->dev_com = i;
                dev_proc->dev_is_online = true;
                return true;
            }

            close_uart(fd);
        }
    }

    return false;
}

static void *scan_loop(void *arg)
{
    dev_proc_t *dev_proc = (dev_proc_t *)arg;

    while(true)
    {
        if( dev_proc->dev_is_online == false )
        {
            while(true)
            {
                printf("尝试连接设备...\n");
                sleep(1);

                if(try_open_uart(dev_proc, dev_online_check))
                {
                    printf("com%d 已连接!\n",dev_proc->dev_com);
                    break;
                }
                else
                {
                    printf("未检索到有效设备！\n");
                    sleep(1);
                    continue;
                }
            }
        }

        if(dev_online_check(dev_proc) == false)
        {
            printf("设备已断开连接！\n");
            dev_proc->dev_is_online = false;
            close_uart(dev_proc->dev_fd);
        }

        sleep(3);
    }
}

void auto_scan_slave(dev_proc_t *dev_proc)
{
    work_thread_variable_init(dev_proc, scan_loop);
}

dev_proc_t *creat_device()
{
    dev_proc_t *dev_proc = malloc(sizeof(dev_proc_t));
    dev_proc->ops = get_stream_capacity();
    dev_proc->parser = parser_new();
    dev_proc->dev_is_online = false;
    return dev_proc;
}