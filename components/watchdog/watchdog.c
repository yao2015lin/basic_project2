#include "watchdog.h"

static DEV_HAND fd;

void feed_dog( void )
{
    c_write(fd , NULL , 0 );
}

int watchdog_init(void)
{   
    fd = c_open("iwdg", 0);

    INIT_PRINT( (fd==NULL)?INIT_FAIL:INIT_OK,"Loading stm32l4xx iwdg component");

    return 0;
}

comp_init(watchdog_init);