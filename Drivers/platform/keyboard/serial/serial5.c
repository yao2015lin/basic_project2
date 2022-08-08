#include "serial5.h"

#define THIS_PORT huart5

#define MODULE_NAME "serial5"

UART_HandleTypeDef huart5;
DMA_HandleTypeDef hdma_uart5_rx;

static sc_semaphore_t usart5_semap = NULL;
static uint8_t usart5_msg[DEBUG_RX_MAX];
static uint32_t usart5_read_mode = WAIT_FOR_EVER;

void UART5_IRQHandler(void)
{
    /* USER CODE BEGIN USART1_IRQn 0 */
    uint32_t temp;

    if (((__HAL_UART_GET_FLAG(&THIS_PORT, UART_FLAG_IDLE)) != RESET))
    {
        __HAL_UART_CLEAR_IDLEFLAG(&THIS_PORT);
        temp = THIS_PORT.Instance->ISR;
        temp = THIS_PORT.Instance->RDR;
        if (usart5_semap != NULL)
        {
            send_semaphore(usart5_semap);
        }
    }

    /* USER CODE END USART1_IRQn 0 */
    HAL_UART_IRQHandler(&THIS_PORT);
    /* USER CODE BEGIN USART1_IRQn 1 */

    /* USER CODE END USART1_IRQn 1 */
}

void serial5_rst(void)
{
    HAL_UART_Receive_DMA(&THIS_PORT, usart5_msg, DEBUG_RX_MAX);

    __HAL_UART_CLEAR_IDLEFLAG(&THIS_PORT);
    __HAL_UART_ENABLE_IT(&THIS_PORT, UART_IT_IDLE);
}

void serial5_mspinit(UART_HandleTypeDef *uartHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_UART5_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**UART5 GPIO Configuration
    PC12     ------> UART5_TX
    PD2     ------> UART5_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* UART5 DMA Init */
    /* UART5_RX Init */
    hdma_uart5_rx.Instance = DMA2_Channel2;
    hdma_uart5_rx.Init.Request = DMA_REQUEST_2;
    hdma_uart5_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_uart5_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_uart5_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_uart5_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_uart5_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_uart5_rx.Init.Mode = DMA_NORMAL;
    hdma_uart5_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_uart5_rx) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }

    __HAL_LINKDMA(uartHandle, hdmarx, hdma_uart5_rx);

    /* UART5 interrupt Init */
    HAL_NVIC_SetPriority(UART5_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(UART5_IRQn);
}

void serial5_hal_init(void)
{
    THIS_PORT.Instance = UART5;
    THIS_PORT.Init.BaudRate = 921600;
    THIS_PORT.Init.WordLength = UART_WORDLENGTH_8B;
    THIS_PORT.Init.StopBits = UART_STOPBITS_1;
    THIS_PORT.Init.Parity = UART_PARITY_NONE;
    THIS_PORT.Init.Mode = UART_MODE_TX_RX;
    THIS_PORT.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    THIS_PORT.Init.OverSampling = UART_OVERSAMPLING_16;
    THIS_PORT.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    THIS_PORT.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT | UART_ADVFEATURE_DMADISABLEONERROR_INIT;
    THIS_PORT.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
    THIS_PORT.AdvancedInit.DMADisableonRxError = UART_ADVFEATURE_DMA_DISABLEONRXERROR;
    THIS_PORT.MspInitCallback = serial5_mspinit;
    if (HAL_UART_Init(&THIS_PORT) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }
}

int serial5_hal_open(void)
{
    usart5_semap = creat_semqphore();
    log(DEBUG, "serial debug semqphore creat ...... [OK]\n");

    HAL_UART_Receive_DMA(&THIS_PORT, usart5_msg, DEBUG_RX_MAX);
    __HAL_UART_CLEAR_IDLEFLAG(&THIS_PORT);
    __HAL_UART_ENABLE_IT(&THIS_PORT, UART_IT_IDLE);

    return 0;
}

int serial5_open(FIL_HAND *fd)
{
    serial5_hal_init();
    serial5_hal_open();
    return 0;
}

int serial5_write(FIL_HAND *fd, const void *buf, uint32_t count)
{
    if ((HAL_UART_Transmit(&THIS_PORT, (uint8_t *)buf, count, 500)) != HAL_OK)
    {
        return -EIO;
    }

    return 0;
}

int serial5_read(FIL_HAND *fd, void *data, uint32_t count)
{
    uint32_t rxsize = 0;
    uint32_t rxlen = 0;
    uint8_t temp[DEBUG_RX_MAX];

    if (THIS_PORT.hdmarx->Instance->CNDTR == DEBUG_RX_MAX) //缓存没有数据
    {
        //如果接收停止，开启接收
        if (THIS_PORT.RxState == HAL_UART_STATE_READY)
        {
            log(DEBUG, "rx state is ready , start receive\n");
            HAL_UART_Receive_DMA(&THIS_PORT, usart5_msg, DEBUG_RX_MAX);
        }
        //等待接收完成信号
        read_semaphore(usart5_semap, usart5_read_mode);
    }

    rxsize = DEBUG_RX_MAX - THIS_PORT.hdmarx->Instance->CNDTR;
    if (rxsize >= count) //接受到的总长度大于read希望获取的长度
    {
        memcpy(data, usart5_msg, count); //提取数据
        rxsize -= count;

        if (rxsize > 0) //还有剩余的数据
        {
            memcpy(temp, usart5_msg + count, rxsize); //其余数据放入临时缓存
            memset(usart5_msg, 0x00, DEBUG_RX_MAX);   //清空接收缓存
            memcpy(usart5_msg, temp, rxsize);
        }
        else
        {
            memset(usart5_msg, 0x00, DEBUG_RX_MAX); //清空接收缓存
        }

        rxlen = count;
    }
    else //接收到的数据长度小于read希望获取的长度
    {
        memcpy(data, usart5_msg, rxsize);       //提取数据
        memset(usart5_msg, 0x00, DEBUG_RX_MAX); //清空接收缓存

        rxlen = rxsize;
        rxsize = 0;
    }

    if (THIS_PORT.RxState == HAL_UART_STATE_READY)
    {
        HAL_UART_Receive_DMA(&THIS_PORT, usart5_msg + rxsize, DEBUG_RX_MAX - rxsize);
    }
    else
    {
        __HAL_DMA_DISABLE(THIS_PORT.hdmarx);
        THIS_PORT.hdmarx->Instance->CMAR = (uint32_t)(usart5_msg + rxsize);
        THIS_PORT.hdmarx->Instance->CNDTR = DEBUG_RX_MAX - rxsize;
        __HAL_DMA_ENABLE(THIS_PORT.hdmarx);
    }
    return rxlen;
}

struct file_operations serial5_fops =
{
    .onwer = 0,
    .open = serial5_open,
    .write = serial5_write,
    .read = serial5_read,
};

struct miscdevice serial5_device =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = MODULE_NAME,
    .fops = &serial5_fops,
    .nodename = "device",
};

int serial5_init(void)
{
    return (misc_register(&serial5_device));
}

device_init(serial5_init);
