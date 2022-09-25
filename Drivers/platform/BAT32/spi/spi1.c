#include "spi1.h"



int spi1_open(FIL_HAND *fd)
{
    SPI11_MasterInit(SPI_MODE_0);
    return 0;
}


int spi1_write(FIL_HAND *fd, const void *buf, uint32_t count)
{
    (void)SPI11_MasterSend((uint8_t *)buf, (uint16_t) count);
    return 0;
}

int spi1_read(FIL_HAND *fd, void *data, uint32_t count)
{
    (void)SPI11_MasterReceive((uint8_t *)data, (uint16_t) count);
    return 0;
}

struct file_operations spi1_fops =
{
    .onwer = 0,
    .open = spi1_open,
    .write = spi1_write,
    .read = spi1_read,
};

struct miscdevice spi1 =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = "spi1",
    .fops = &spi1_fops,
    .nodename = "device",
    //.used = 1,
};

int spi1_init(void)
{
    return (misc_register(&spi1));
}

device_init(spi1_init);
