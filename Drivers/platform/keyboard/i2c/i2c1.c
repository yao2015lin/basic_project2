#include "i2c1.h"

#define MODULE_NAME "i2c1"

i2c_stru_t i2c1_strc;

void i2c1_gpio_init( void )
{
    __HAL_RCC_GPIOB_CLK_ENABLE();

    i2c1_strc.sda.gpio = GPIOB;
    i2c1_strc.sda.pin = GPIO_PIN_9;

    i2c1_strc.scl.gpio = GPIOB;
    i2c1_strc.scl.pin = GPIO_PIN_8;
}

int i2c1_open(FIL_HAND *fd)
{

    GPIO_InitTypeDef GPIO_InitStruct;

    i2c1_gpio_init();

    GPIO_InitStruct.Pin = i2c1_strc.sda.pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(i2c1_strc.sda.gpio, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = i2c1_strc.scl.pin;
    HAL_GPIO_Init(i2c1_strc.scl.gpio, &GPIO_InitStruct);


    return 0;
}

int i2c1_write(FIL_HAND *fd, const void *buf, uint32_t count)
{
    i2c_data_t *i2c_data = (i2c_data_t *)buf;

    
    return 0;
}

int i2c1_read(FIL_HAND *fd, void *data, uint32_t count)
{
    i2c_data_t *i2c_data = (i2c_data_t *)data;
    
    i2c1_strc.dummy = i2c_data->timeout;
    
    if( i2cx_read(&i2c1_strc , 
              i2c_data->sal_addr , i2c_data->mem_addr , 
              i2c_data->pdata , i2c_data->size) == TRUE)
    {
        return 0;
    }
    
    return -EOS;
}

struct file_operations i2c1_fops =
{
    .onwer = 0,
    .open = i2c1_open,
    .write = i2c1_write,
    .read = i2c1_read,
};

struct miscdevice i2c1_device =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = MODULE_NAME,
    .fops = &i2c1_fops,
    .nodename = "device",
};

int i2c1_init(void)
{
    return (misc_register(&i2c1_device));
}

device_init(i2c1_init);