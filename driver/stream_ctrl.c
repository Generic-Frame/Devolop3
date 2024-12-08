#include "stream_ctrl.h"

static int stream_input(int fd, void *data,int length)
{
    return read(fd, data, length);
}

static int stream_output(int fd, void *data,int length)
{
    return write(fd, data, length);
}

static app_com_ops_t this_manager =
{
    .recv             		    = stream_input,
	.send						= stream_output
};

app_com_ops_t *get_stream_capacity(void)
{
    return &this_manager;
}