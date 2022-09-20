
#include "time.h"

static DEV_HAND fd;

void setltime(time_t *tm)
{
    c_write(fd ,(void *)tm ,7);
}

void getltime(time_t *tm)
{
    c_read(fd ,(void *)tm ,7);
}

int time_init(void)
{   
    fd = c_open("bat32_157_rtc", 0);

    INIT_PRINT( (fd==NULL)?INIT_FAIL:INIT_OK,"Loading bat32 rtc component");

    return 0;
}

comp_init(time_init);

