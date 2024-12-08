#ifndef MESSAGE_H
#define MESSAGE_H

#include "type_def.h"
#include <stdlib.h>
#include <string.h>

#define MSG_SIZE    (512)

app_message_t *message_create(unsigned short cmd, const char *payload, unsigned int length);
void message_schedule(app_message_t *msg, unsigned short index, unsigned short total);
int message_size(app_message_t *msg);

#endif