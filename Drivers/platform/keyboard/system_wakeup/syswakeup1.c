#include "syswakeup1.h"

#define MODULE_NAME "syswakeup1"

static sc_semaphore_t syswk1_semap = NULL;


void HAL_GPIO_PIN0_CALLBACK(void)
{

    if (syswk1_semap != NULL)
    {
        send_semaphore(syswk1_semap);
    }
}

int syswakeup1_open(FIL_HAND *fd)
{
    GPIO_InitTypeDef gpio_init_structure;

    __HAL_RCC_GPIOA_CLK_ENABLE();

    gpio_init_structure.Pin = GPIO_PIN_0;
    gpio_init_structure.Pull = GPIO_NOPULL;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_init_structure.Mode = GPIO_MODE_IT_RISING;
    HAL_GPIO_Init(GPIOA, &gpio_init_structure);

    /* Enable and set Button EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority(EXTI0_IRQn, 5, 0x00);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    syswk1_semap = creat_semqphore();
    INIT_PRINT((syswk1_semap == NULL) ? INIT_FAIL : INIT_OK, "system wakeup1 semqphore creat");

    return 0;
}

int syswakeup1_write(FIL_HAND *fd, const void *buf, uint32_t count)
{

    return 0;
}

int syswakeup1_read(FIL_HAND *fd, void *data, uint32_t count)
{
    read_semaphore(syswk1_semap, WAIT_FOR_EVER);

    return ( HAL_GPIO_ReadPin(GPIOA , GPIO_PIN_0));
}

struct file_operations syswakeup1_fops =
    {
        .onwer = 0,
        .open = syswakeup1_open,
        .write = syswakeup1_write,
        .read = syswakeup1_read,
};

struct miscdevice syswakeup1_device =
    {
        .minor = MISC_DYNAMIC_MINOR,
        .name = MODULE_NAME,
        .fops = &syswakeup1_fops,
        .nodename = "device",
};

int syswakeup1_init(void)
{
    return (misc_register(&syswakeup1_device));
}

device_init(syswakeup1_init);
