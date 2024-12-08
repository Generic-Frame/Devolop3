#include "cmd_manager.h"

static int handshake_respond(app_message_t *msg, app_com_ops_t *ops, int fd)
{
    int ret = FUNC_API_OK;
    app_message_t *ack = message_create(ret, NULL, 0);
    ops->send(fd, ack, message_size(ack));
    free(ack);
    return ret;
}

static int password_respond(app_message_t *msg, app_com_ops_t *ops, int fd)
{
    int ret = FUNC_API_OK;
    for(int i = 0; i < msg->length; i++)
    {
        printf("%x ",msg->payload[i]);
    }

    printf("\n");
    app_message_t *ack = message_create(ret, NULL, 0);
    ops->send(fd, ack, message_size(ack));
    free(ack);
    return ret;
}

static int version_respond(app_message_t *msg, app_com_ops_t *ops, int fd)
{
    int ret = FUNC_API_OK;
    app_message_t *ack = message_create(ret, FIRMWARE_VERSION, sizeof(FIRMWARE_VERSION));
    ops->send(fd, ack, message_size(ack));
    free(ack);
    return ret;
}

static int easylog_respond(app_message_t *msg, app_com_ops_t *ops, int fd)
{
    int ret = FUNC_API_OK;

    static int index = 0;
    static int total = 0;
    static FILE *fp = NULL;
    static app_message_t *ack = NULL;

    int payload_size = MSG_SIZE - sizeof(app_message_t);

    if(!index)
    {
        ack = message_create(ret, NULL, payload_size);

        int file_size = get_file_size("/easylog.txt");

        total = (file_size / payload_size) + (file_size % payload_size > 0 ? 1 : 0);

        fp = fopen("/easylog.txt", "rb");

        index++;
    }

    ack->length = fread(ack->payload, 1, payload_size, fp);

    message_schedule(ack, index, total);

    ops->send(fd, ack, message_size(ack));

    if(ack->index >= ack->total)
    {
        fclose(fp);
        free(ack);
        ack = NULL;
        index = 0;
    }
    else
    {
        index++;
    }

    return ret;
}

static int update_respond(app_message_t *msg, app_com_ops_t *ops, int fd)
{
    int ret = FUNC_API_OK;
    static FILE *fp = NULL;

    if(msg->index == 1)
    {
        fp = fopen("/main.out", "wb");
    }

    if(msg->payload)
    {
        fwrite(msg->payload, 1, msg->length, fp);
    }

    if(msg->index >= msg->total)
    {
        fclose(fp);
        system("sync");
        system("reboot");
    }

    app_message_t *ack = message_create(ret, NULL, 0);
    ops->send(fd, ack, message_size(ack));
    free(ack);
    return ret;
}

static void command_process(app_message_t *msg, app_com_ops_t *ops, int fd)
{
    unsigned short cmd = msg->cmd;

    printf("cmd = %x\n",cmd);

    switch(cmd)
    {
        case handshake:
            handshake_respond(msg, ops, fd);
            break;
        case password:
            password_respond(msg, ops, fd);
            break;
        case version:
            version_respond(msg, ops, fd);
            break;
        case easylog:
            easylog_respond(msg, ops, fd);
            break;
        case update:
            update_respond(msg, ops, fd);
            break;
        default:
            break;
    }
}

void command_manager(app_gbl_t *resc)
{
    list_t *packet_list = resc->buffer[packet_index];
    app_com_ops_t *ops = resc->ops;

    mutex_lock(&resc->locker[packet_index]);
    node_t *node = list_remove(packet_list);
    mutex_unlock(&resc->locker[packet_index]);

    app_packet_t *packet = container_of(node, app_packet_t, link_node);
    app_message_t *msg = packet->app_msg;

    command_process(msg, ops, resc->slave_fd);

    free(msg);
    free(packet);
}