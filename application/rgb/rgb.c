#include "rgb.h"
#include "init_d.h"




struct rgb_type
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

static DEV_HAND fd;
static xTaskHandle rgb_handle;
static struct rgb_type rgb[PIXEL_MAX];

static void rgb_process(void const *parma)
{
 
    task_assert(parma);
    
    fd = c_open("ws2812b" , 0);
    INIT_PRINT( (fd==NULL)?INIT_FAIL:INIT_OK,"ws2812b");
    
    for( uint8_t i = 0 ; i < PIXEL_MAX ; i++ )
    {
        rgb[i].r = 255;
        rgb[i].g = 255;
        rgb[i].b = 255;
        
    }
    c_write(fd , ( uint8_t *)rgb , PIXEL_MAX);
    while (1)
    {
        sys_delay(30);
    }
}


int rgb_init(void)
{
    osThreadDef(rgb_task, rgb_process, osPriorityLow, 0, configMINIMAL_STACK_SIZE * 6);
    rgb_handle = osThreadCreate(osThread(rgb_task), NULL);
    configASSERT(rgb_handle)

    return 0;
}

int rgb_resume( void )
{
    c_ioctl(fd , NULL  , 0 , IOCTL_FLAG_RESUME);
    
    return 0;
}

int rgb_suspend( void )
{
    
    c_ioctl(fd , NULL  , 0 , IOCTL_FLAG_SUSPEND);
    
    return 0;
}

task_init(rgb_init);
resume_register(rgb_resume);
suspend_register(rgb_suspend);