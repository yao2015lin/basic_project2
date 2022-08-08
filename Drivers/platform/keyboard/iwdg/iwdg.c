#include "iwdg.h"

#define MODULE_NAME "iwdg"

IWDG_HandleTypeDef hiwdg;

int iwdg_open(FIL_HAND *fd)
{
    hiwdg.Instance = IWDG;
    hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
    hiwdg.Init.Window = 4095;
    hiwdg.Init.Reload = 4095;
    if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }
    
    return 0;
}

int iwdg_write(FIL_HAND *fd, const void *data, uint32_t count)
{
    HAL_IWDG_Refresh(&hiwdg);
    
    return 0;
}

struct file_operations iwdg_fops =
{
    .onwer = 0,
    .open = iwdg_open,
    .write = iwdg_write,
};

struct miscdevice iwdg_device =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = MODULE_NAME,
    .fops = &iwdg_fops,
    .nodename = "device",
};

int iwdg_init(void)
{
    return (misc_register(&iwdg_device));
}

device_init(iwdg_init);