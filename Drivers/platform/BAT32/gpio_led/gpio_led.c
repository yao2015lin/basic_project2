#include "gpio_led.h"

#define  LED_INDEX_0     0
#define  LED_INDEX_1     1

#define  LED_ON     1
#define  LED_OFF    0

static struct led_type
{
    uint16_t  index;
    uint8_t   on_off;
    uint8_t   rev;
};

static int gpio_led_write(FIL_HAND *fd, const void *buf, uint32_t count)
{
    struct led_type led_type_t;
    if(NULL == buf)
    {
        printf("gpio_led buf is NULL\r\n");
        return 0;
    }
    memcpy(&led_type_t, buf, count);
    if(LED_INDEX_0 == led_type_t.index)
    {
        if(LED_ON == led_type_t.on_off)
        {
            PORT_ClrBit(PORTD, PIN8);
        }
        else
        {
            PORT_SetBit(PORTD, PIN8);
        }
    }
    else if(LED_INDEX_1 == led_type_t.index)
    {
        if(LED_ON == led_type_t.on_off)
        {
            PORT_ClrBit(PORTD, PIN9);
        }
        else
        {
            PORT_SetBit(PORTD, PIN9);
        }
    }
    return 0;
}

static int gpio_led_read(FIL_HAND *fd, void *data, uint32_t count)
{
    return 0;
}

static int gpio_led_open(FIL_HAND *fd)
{
    PORT_Init(PORTD, PIN8, OUTPUT);
    PORT_Init(PORTD, PIN9, OUTPUT);

    PORT_SetBit(PORTD, PIN8);
    PORT_SetBit(PORTD, PIN9);
    return 0;
}
struct file_operations gpio_led_fops =
{
    .onwer = 0,
    .open = gpio_led_open,
    .write = gpio_led_write,
    .read = gpio_led_read,
};

struct miscdevice gpio_led_device =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = "gpio-led",
    .fops = &gpio_led_fops,
    .nodename = "device",
};

int gpio_led_init(void)
{
    return (misc_register(&gpio_led_device));
}

device_init(gpio_led_init);