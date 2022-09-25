#include "uart0.h"




int serial_0_open(FIL_HAND *fd)
{
    return 0;
}

void com0_hal_init( void )
{
    MD_STATUS status;
    SystemCoreClockUpdate();
	status = UART0_Init(SystemCoreClock, 115200);
	if(status == MD_ERROR)
    {
        while(1);
    }
}

int serial_0_write(FIL_HAND *fd, const void *buf, uint32_t count)
{
    UART0_Send_data((uint8_t *)buf, (uint16_t)count);
    return 0;
}

int serial_0_read(FIL_HAND *fd, void *data, uint32_t count)
{
    UART0_Receive_data((uint8_t *)data, count);
    return 0;
}

struct file_operations serial0_fops =
{
    .onwer = 0,
    .open = serial_0_open,
    .write = serial_0_write,
    .read = serial_0_read,
};

struct miscdevice serial0 =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = "com0",
    .fops = &serial0_fops,
    .nodename = "device",
    //.used = 1,
};

int serial0_init(void)
{
    return (misc_register(&serial0));
}

device_init(serial0_init);