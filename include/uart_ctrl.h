#ifndef UART_CTRL_H
#define UART_CTRL_H

#include "error_def.h"
#include "type_def.h"
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int open_uart(unsigned char *path);
void init_uart(int fd, int baud_rate);
int close_uart(int fd);

#endif