#include "io_irq_line7.h"

#define MODULE_NAME "io_irq_line7"

static sc_semaphore_t io_irq_line7_semap = NULL;
static TimerHandle_t time_handle = NULL;
static uint8_t read_flag = true;

void line7_timer_callback( xTimerHandle pxTimer )
{
    read_flag = true;
    xTimerStop(time_handle , 0 );
}

void HAL_GPIO_PIN7_CALLBACK(void)
{
    if ((io_irq_line7_semap != NULL) && ( read_flag == true ))
    {
        read_flag = false;
        xTimerStartFromISR(time_handle , 0);
        send_semaphore(io_irq_line7_semap);
    }
}

int io_irq_line7_open(FIL_HAND *fd)
{
    GPIO_InitTypeDef gpio_init_structure;

    __HAL_RCC_GPIOB_CLK_ENABLE();

    gpio_init_structure.Pin = GPIO_PIN_7;
    gpio_init_structure.Pull = GPIO_NOPULL;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_init_structure.Mode = GPIO_MODE_IT_FALLING;
    HAL_GPIO_Init(GPIOB, &gpio_init_structure);

    /* Enable and set Button EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0x00);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    io_irq_line7_semap = creat_semqphore();
    INIT_PRINT((io_irq_line7_semap == NULL) ? INIT_FAIL : INIT_OK, "system io irq line7 semqphore creat");

    return 0;
}

int io_irq_line7_read(FIL_HAND *fd, void *data, uint32_t count)
{
    read_semaphore(io_irq_line7_semap, WAIT_FOR_EVER);

    return (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7));
}

struct file_operations io_irq_line7_fops =
{
    .onwer = 0,
    .open = io_irq_line7_open,
    .read = io_irq_line7_read,
};

struct miscdevice io_irq_line7_device =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = MODULE_NAME,
    .fops = &io_irq_line7_fops,
    .nodename = "device",
};

int io_irq_line7_init(void)
{
    time_handle = xTimerCreate("line7_timer",       /* 定时器名字 */
                                   200,              /* 定时器周期,单位时钟节拍 */
                                   pdTRUE,          /* 周期性 */
                                   (void *)0,      /* 定时器ID */
                                   line7_timer_callback); /* 定时器回调函数 */
    return (misc_register(&io_irq_line7_device));
}

device_init(io_irq_line7_init);
