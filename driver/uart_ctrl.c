#include "uart_ctrl.h"

int open_uart(unsigned char *path)
{
    return open(path, O_RDWR | O_NOCTTY | O_NDELAY);
}

void init_uart(int fd, int baud_rate)
{
    struct termios opt;
    tcgetattr(fd, &opt);
    cfsetispeed(&opt, baud_rate);
    cfsetospeed(&opt, baud_rate);
    tcsetattr(fd, TCSANOW, &opt);

    tcgetattr(fd, &opt);
    opt.c_cflag &= ~CSIZE;
    opt.c_cflag |= CS8;
    opt.c_cflag |= IXON | IXOFF | IXANY;
    opt.c_cflag &= ~PARENB;
    opt.c_cflag &= ~CSTOPB;
    opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    opt.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    opt.c_oflag &= ~OPOST;

    opt.c_cc[VTIME] = 150;
    opt.c_cc[VMIN] = 0;

    tcsetattr(fd, TCSANOW, &opt);
    tcflush(fd, TCIOFLUSH);
}

int close_uart(int fd)
{
    return close(fd);
}