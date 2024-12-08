#ifndef EASYLOG_H
#define EASYLOG_H

#include "mutex_ctrl.h"
#include <sys/time.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

extern const char *g_master_fun;
extern const char *g_master_lev;

#define EASYLOG_DATE    (1 << 0)
#define EASYLOG_TIME    (1 << 1)
#define EASYLOG_FUNC    (1 << 2)
#define EASYLOG_LEVEL   (1 << 3)

#define EASY_LOG_PATH	"main.log"
#define EASY_LOG_LEN	1000
#define DEBUG

int easylog_flag_add(int flag);
int easylog_flag_rm(int flag);
int easylog_file(const char * logfile);
int easylog_write(const char * fmt, ...);

#define easylog(fmt, arg...) \
    do{\
        g_master_fun  = __func__;\
        g_master_lev  = "INFO";\
        easylog_write(fmt, ##arg);\
    }while(0)

#define Debug( fmt,  arg  ... )  \
	do{printf("[%s-%s-%d]: "fmt , __FILE__, __FUNCTION__, __LINE__, ##arg);  }while(0);

#endif