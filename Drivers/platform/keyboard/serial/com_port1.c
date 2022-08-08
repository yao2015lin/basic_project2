#include "com_port1.h"


int serial_1_open(FIL_HAND *fd)
{
    serial_open(fd);
    return 0;
}

int serial_1_write(FIL_HAND *fd, const void *buf, uint32_t count)
{
   
    serial_write(fd , buf , count);
    return 0;
}

int serial_1_read(FIL_HAND *fd, void *data, uint32_t count)
{
    serial_read(fd , data , count);
    
    return 0;
}

struct file_operations serial1_fops =
{
    .onwer = 0,
    .open = serial_1_open,
    .write = serial_1_write,
    .read = serial_1_read,
};

struct miscdevice serial1 =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = "com1",
    .fops = &serial1_fops,
    .nodename = "device",
};

int serial1_init(void)
{
    return (misc_register(&serial1));
}

device_init(serial1_init);
