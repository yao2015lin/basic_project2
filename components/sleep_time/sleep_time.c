
#include "sleep_time.h"

static DEV_HAND fd;

uint32_t get_time_count( void )
{
    uint32_t count = 0;

    c_read(fd ,(void *)&count ,4);

    return count;
}

void set_time_sleep(uint32_t sleep_tims)
{
    if( sleep_tims < 0xFFFF)
    {
        c_write(fd ,(void *)&sleep_tims ,4);
    }
}

void stop_time_sleep(void)
{
    c_ioctl(fd , NULL , 0 , IOCTL_FLAG_SET_STOP);
}

int time_sleep_init(void)
{   
    fd = c_open("lptim1", 0);

    INIT_PRINT( (fd==NULL)?INIT_FAIL:INIT_OK,"Loading lptime component");

    return 0;
}

comp_init(time_sleep_init);

