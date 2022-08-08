#include "com_port2.h"

struct serial_dev com2;

int com2_clock_init( void )
{
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();
    
    return 0;
}

int serial_2_open(FIL_HAND *fd)
{
    return 0;
}

void com2_hal_init( void )
{
    com2.tx.gpiox = GPIOA;
    com2.tx.pin = GPIO_PIN_2;
    com2.tx.alternate = GPIO_AF7_USART2;

    com2.rx.gpiox = GPIOA;
    com2.rx.pin = GPIO_PIN_3;
    com2.rx.alternate = GPIO_AF7_USART2;

    com2.com_handle.Instance = USART2;
    com2.dma_handle.Instance = DMA1_Channel6;
    
    com2.clock_init = com2_clock_init;

    com2.irqn = USART2_IRQn;
    com2.band = 921600;
    com2.mode = WAIT_FOR_EVER;
    com2.max_size = 100;
    com2.size = 0;
    com2.buf = malloc(sizeof(uint8_t)*com2.max_size);
    if( com2.buf == NULL )
    {
        //log(ERR, "com2 buffer malloc fail\n");
    }

    serial_open(( void *)&com2);

}

int serial_2_write(FIL_HAND *fd, const void *buf, uint32_t count)
{
   
    serial_write(( void *)&com2 , buf , count);
    return 0;
}

int serial_2_read(FIL_HAND *fd, void *data, uint32_t count)
{
    serial_read(( void *)&com2 , data , count);
    
    return 0;
}

struct file_operations serial2_fops =
{
    .onwer = 0,
    .open = serial_2_open,
    .write = serial_2_write,
    .read = serial_2_read,
};

struct miscdevice serial2 =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = "com2",
    .fops = &serial2_fops,
    .nodename = "device",
    //.used = 1,
};

int serial2_init(void)
{
    return (misc_register(&serial2));
}

device_init(serial2_init);
