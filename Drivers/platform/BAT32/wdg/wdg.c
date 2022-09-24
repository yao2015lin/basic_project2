#include "wdg.h"

#define MODULE_NAME "wdg"


int wdg_open(FIL_HAND *fd)
{
    return 0;
}

int wdg_write(FIL_HAND *fd, const void *data, uint32_t count)
{
    WDT_Restart();
    return 0;
}

struct file_operations wdg_fops =
{
    .onwer = 0,
    .open = wdg_open,
    .write = wdg_write,
};

struct miscdevice wdg_device =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = MODULE_NAME,
    .fops = &wdg_fops,
    .nodename = "device",
};

int wdg_init(void)
{
    return (misc_register(&wdg_device));
}

device_init(wdg_init);