#include "led.h"
#include "BAT32G_157_it.h"

#define LED_INDEX_LED_0    0
#define LED_INDEX_LED_1    1

#define LED_EVENT_OFF 0 
#define LED_EVENT_ON 1
#define LED_EVENT_FLASH  2
#define LED_EVENT_QUICKLY 3
#define LED_EVENT_BREATH  4


#define LED_GPIO_LED_OFF   0
#define LED_GPIO_LED_ON    1

static struct led_type
{
    uint16_t  index;
    uint8_t   event;
    uint8_t   rev;
};

static struct led_gpio_type
{
    uint16_t  index;
    uint8_t   on_off;
    uint8_t   rev;
};
static DEV_HAND ledfd;


static int led_open(FIL_HAND *fd)
{
    ledfd = c_open("gpio-led", 0);
    INIT_PRINT((ledfd == NULL) ? INIT_FAIL : INIT_OK, "open gpio-led");
    return 0;
}


int led_write(FIL_HAND *fd, const void *buf, uint32_t count)
{
    struct led_type led_type_t;
    if(NULL == buf)
    {
        printf("led buf is NULL\r\n");
        return 0;
    }
    memcpy(&led_type_t, buf, count);
    struct led_gpio_type gpio_led;
    if(LED_INDEX_LED_0 == led_type_t.index)
    {
       switch(led_type_t.event)
       {
        case LED_EVENT_OFF:
           gpio_led.index = LED_INDEX_LED_0;
           gpio_led.on_off = LED_GPIO_LED_OFF;
           c_write(ledfd, ( uint8_t *)&gpio_led, sizeof(gpio_led)); 
        break;
        case LED_EVENT_ON:
           gpio_led.index = LED_INDEX_LED_0;
           gpio_led.on_off = LED_GPIO_LED_ON;
           c_write(ledfd, ( uint8_t *)&gpio_led, sizeof(gpio_led)); 
        break;
        case LED_EVENT_FLASH:
        break;
        default:break;

       }
    }
    else if(LED_INDEX_LED_1 == led_type_t.index)
    {
       switch(led_type_t.event)
       {
        case LED_EVENT_OFF:
           gpio_led.index = LED_INDEX_LED_1;
           gpio_led.on_off = LED_GPIO_LED_OFF;
           c_write(ledfd, ( uint8_t *)&gpio_led, sizeof(gpio_led)); 
        break;
        case LED_EVENT_ON:
           gpio_led.index = LED_INDEX_LED_1;
           gpio_led.on_off = LED_GPIO_LED_ON;
           c_write(ledfd, ( uint8_t *)&gpio_led, sizeof(gpio_led)); 
        break;
        case LED_EVENT_FLASH:
        break;
        default:break;

       }
    }

     
    return 0;
}


int led_ioctl(FIL_HAND *fd, void *buf, uint32_t count, uint32_t flag)
{
    
    return 0;
}


static int led_resume(FIL_HAND *fd)
{
    c_ioctl(fd, NULL, 0, IOCTL_FLAG_RESUME);
    return 0;
}

static int led_suspend(FIL_HAND *fd)
{
    c_ioctl(fd, NULL, 0, IOCTL_FLAG_SUSPEND);
    return 0;
}

struct file_operations led_fops =
{
    .onwer = 0,
    .open = led_open,
    .write = led_write,
    .ioctl = led_ioctl,
    .resume = led_resume,
    .suspend = led_suspend,
};

struct miscdevice led_device =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = "led",
    .fops = &led_fops,
    .nodename = "modules",
};

static int led_init(void)
{
    return (misc_register(&led_device));
}

modules_init(led_init);
