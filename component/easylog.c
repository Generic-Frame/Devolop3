#include "easylog.h"

const char *g_master_fun = NULL;
const char *g_master_lev = NULL;

static int g_master_fd = -1;
static FILE *g_master_fp = NULL;
static char g_logfile[128] = EASY_LOG_PATH;
static int g_master_flag = (EASYLOG_DATE | EASYLOG_TIME);
static mutex_t easylog_mutex = PTHREAD_MUTEX_INITIALIZER;


static int easylog_open_file()
{
    if(strlen(g_logfile) == 0)
    {
        return -1;
    }

    g_master_fp = fopen(g_logfile, "a");
    if(NULL == g_master_fp)
    {
        printf("g_master_fp fopen error\n");
        return -1;
    }
    g_master_fd = fileno(g_master_fp);

    return 0;
}

static int easylog_write_log(const char *fmt, va_list arg_list)
{
    if(easylog_open_file() == -1)
    {
        return -1;
    }

    struct timeval start;
    gettimeofday(&start, NULL);
    struct tm t;
    bzero(&t, sizeof(t));
    localtime_r(&start.tv_sec, &t);
    if (g_master_flag & EASYLOG_DATE)
    {
        fprintf(g_master_fp, "%04d-%02d-%02d ", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);
    }
    if (g_master_flag & EASYLOG_TIME)
    {
        fprintf(g_master_fp, "%02d:%02d:%02d.%03ld ", t.tm_hour, t.tm_min, t.tm_sec, start.tv_usec / 1000);
    }
    if (g_master_flag & EASYLOG_LEVEL && NULL != g_master_lev)
    {
        fprintf(g_master_fp, "[%s] ", g_master_lev);
    }

    if (g_master_flag & EASYLOG_FUNC && NULL != g_master_fun)
    {
        fprintf(g_master_fp, "%s() ", g_master_fun);
    }
    if(g_master_flag)
    {
        fprintf(g_master_fp, "%s", "-- ");
    }
    vfprintf(g_master_fp, fmt, arg_list);
    fprintf(g_master_fp, "\n");
    fflush(g_master_fp);

    fclose(g_master_fp);
    return 0;
}

int easylog_file(const char * logfile)
{
    snprintf(g_logfile, sizeof(g_logfile), "%s", logfile);
    return 0;
}

static void limitLogData()
{
#ifdef EASY_LOG_LEN

    FILE *fp = fopen(g_logfile, "r");
    if(NULL == fp)
    {
        return;
    }

    int sum = 0;
    char buf[1024] = {0};
    while(fgets(buf, 1024, fp) != NULL)
    {
        sum++;
    }

    fclose(fp);

    int cnt = sum - (EASY_LOG_LEN-1);

    if(cnt > 0)
    {
        for(int i = 0; i < cnt; i++)
        {
            char str[128] = {0};
            sprintf(str, "sed -i '1d' %s", g_logfile);
            system(str);
        }
    }

#endif
}

int easylog_write(const char * fmt, ...)
{
    mutex_lock(&easylog_mutex);
	int ret;
    limitLogData();
    va_list argptr;
    va_start(argptr, fmt);
    ret = easylog_write_log(fmt, argptr);
    va_end(argptr);
    mutex_unlock(&easylog_mutex);
    return ret;
}

int easylog_flag_add(int flag)
{
    g_master_flag |= flag;
    return 0;
}

int easylog_flag_rm(int flag)
{
    g_master_flag &= ~flag;
    return 0;
}

#if 0
void easylog_init(void)
{
	/* 1、设置日志名字 */
    easylog_file(EASY_LOG_PATH);

	/* 2、设置日志属性 */
	easylog_flag_add(EASYLOG_DATE | EASYLOG_TIME);
}
#endif