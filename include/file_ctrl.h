#ifndef FILE_CTRL_H
#define FILE_CTRL_H

#include <sys/stat.h>
#include <stdbool.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

int get_file_size(char *path);
int get_file_number(char *path);
int load_file_data(const char *name, unsigned char *buf, int length);
int save_file_data(const char *name, unsigned char *buf, int length);

#endif