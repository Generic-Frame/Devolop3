#ifndef PARSER_H
#define PARSER_H

#include "error_def.h"
#include "type_def.h"
#include <unistd.h>
#include <malloc.h>
#include <string.h>

app_parser_t *parser_new();
app_message_t* parser_from_fd(app_parser_t *parser, app_com_ops_t *ops, int fd);
app_message_t *parser_from_mem(app_parser_t *parser, unsigned char *mem, unsigned int length);

#endif