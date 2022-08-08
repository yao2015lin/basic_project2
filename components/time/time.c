
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
    fd = c_open("stm32l4_rtc", 0);

    INIT_PRINT( (fd==NULL)?INIT_FAIL:INIT_OK,"Loading stm32l4xx rtc component");

    return 0;
}

comp_init(time_init);

