#include "rgb.h"
#include "init_d.h"

struct rgb_type
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};
struct led_type
{
    uint16_t  index;
    uint8_t   event;
    uint8_t   rev;
};

static DEV_HAND fd;
static xTaskHandle rgb_handle;
static struct rgb_type rgb[PIXEL_MAX];
static DEV_HAND led_fd;
static void rgb_process(void const *parma)
{
 
    task_assert(parma);
    struct led_type led;
    fd = c_open("ws2812b" , 0);
    INIT_PRINT( (fd==NULL)?INIT_FAIL:INIT_OK,"ws2812b");
    led_fd = c_open("led" , 0);
    INIT_PRINT( (fd==NULL)?INIT_FAIL:INIT_OK,"led");
    for( uint8_t i = 0 ; i < PIXEL_MAX ; i++ )
    {
        rgb[i].r = 255;
        rgb[i].g = 255;
        rgb[i].b = 255;
        
    }
    c_write(fd , ( uint8_t *)rgb , PIXEL_MAX);
    while (1)
    {
        led.index = 0;
        led.event = 0;
        c_write(led_fd, ( uint8_t *)&led, sizeof(led));
        led.index = 1;
        led.event = 1;
        c_write(led_fd, ( uint8_t *)&led, sizeof(led));
        sys_delay(500);
        led.index = 0;
        led.event = 1;
        c_write(led_fd, ( uint8_t *)&led, sizeof(led));
        led.index = 1;
        led.event = 0;
        c_write(led_fd, ( uint8_t *)&led, sizeof(led));
		sys_delay(500);

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
