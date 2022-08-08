#include "syswakeup5.h"

#define MODULE_NAME "syswakeup5"

static sc_semaphore_t syswk5_semap = NULL;


void HAL_GPIO_PIN5_CALLBACK(void)
{
    if (syswk5_semap != NULL)
    {
        send_semaphore(syswk5_semap);
    }
}

int syswakeup5_open(FIL_HAND *fd)
{
    GPIO_InitTypeDef gpio_init_structure;

    __HAL_RCC_GPIOC_CLK_ENABLE();

    gpio_init_structure.Pin = GPIO_PIN_5;
    gpio_init_structure.Pull = GPIO_NOPULL;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_init_structure.Mode = GPIO_MODE_IT_RISING;
    HAL_GPIO_Init(GPIOC, &gpio_init_structure);

    /* Enable and set Button EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0x00);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    syswk5_semap = creat_semqphore();
    INIT_PRINT((syswk5_semap == NULL) ? INIT_FAIL : INIT_OK, "system wakeup5 semqphore creat");

    return 0;
}

int syswakeup5_write(FIL_HAND *fd, const void *buf, uint32_t count)
{

    return 0;
}

int syswakeup5_read(FIL_HAND *fd, void *data, uint32_t count)
{
    read_semaphore(syswk5_semap, WAIT_FOR_EVER);

    return ( HAL_GPIO_ReadPin(GPIOC , GPIO_PIN_5));
}

struct file_operations syswakeup5_fops =
    {
        .onwer = 0,
        .open = syswakeup5_open,
        .write = syswakeup5_write,
        .read = syswakeup5_read,
};

struct miscdevice syswakeup5_device =
    {
        .minor = MISC_DYNAMIC_MINOR,
        .name = MODULE_NAME,
        .fops = &syswakeup5_fops,
        .nodename = "device",
};

int syswakeup5_init(void)
{
    return (misc_register(&syswakeup5_device));
}

device_init(syswakeup5_init);
