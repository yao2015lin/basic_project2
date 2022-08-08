#include "lpuart1.h"
#include "stm32l4xx_ll_lpuart.h"

UART_HandleTypeDef hlpuart1;
DMA_HandleTypeDef hdma_lpuart_rx;

#define DEBUG_RX_MAX 64
#define HAL_RCC_DMA_CLOCK_ENABLE() __HAL_RCC_DMA2_CLK_ENABLE()
#define HAL_TX_PIN GPIO_PIN_11 //PB10     ------> LPUART1_RX
#define HAL_TX_PORT GPIOB
#define HAL_RX_PIN GPIO_PIN_10 //PB11     ------> LPUART1_TX
#define HAL_RX_PORT GPIOB
#define MODULE_NAME "lpuart1"

static sc_semaphore_t lpuart1_semap = NULL;
static uint8_t lpuart1_msg[DEBUG_RX_MAX];
static uint32_t ser_read_mode = WAIT_FOR_EVER;
static uint32_t temp;
static UART_WakeUpTypeDef WakeUpSelection;
static uint8_t lpuart_enable_rx = false;

void LPUART1_IRQHandler(void)
{
    if (((__HAL_UART_GET_FLAG(&hlpuart1, UART_FLAG_IDLE)) != RESET))
    {
        __HAL_UART_CLEAR_IDLEFLAG(&hlpuart1);
        temp = hlpuart1.Instance->ISR;
        temp = hlpuart1.Instance->RDR;
        if ((lpuart1_semap != NULL) && 
            (lpuart_enable_rx == true) &&
            (hlpuart1.hdmarx->Instance->CNDTR != DEBUG_RX_MAX))
        {
            send_semaphore(lpuart1_semap);
        }
    }
    if (LL_LPUART_IsActiveFlag_WKUP(LPUART1) && LL_LPUART_IsEnabledIT_WKUP(LPUART1))
    {
        LL_LPUART_DisableIT_WKUP(LPUART1);
        LL_LPUART_ClearFlag_WKUP(LPUART1);
    }

    HAL_UART_IRQHandler(&hlpuart1);
}

void lpuart1_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /**LPUART1 GPIO Configuration
    PB10     ------> LPUART1_RX
    PB11     ------> LPUART1_TX
    */
    GPIO_InitStruct.Pin = HAL_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF8_LPUART1;
    HAL_GPIO_Init(HAL_TX_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = HAL_RX_PIN;
    HAL_GPIO_Init(HAL_RX_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = GPIO_PIN_All;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void lpuart1_mspinit(UART_HandleTypeDef *uartHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_LPUART1_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = HAL_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF8_LPUART1;
    HAL_GPIO_Init(HAL_TX_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = HAL_RX_PIN;
    HAL_GPIO_Init(HAL_RX_PORT, &GPIO_InitStruct);

    hdma_lpuart_rx.Instance = DMA2_Channel7;
    hdma_lpuart_rx.Init.Request = DMA_REQUEST_4;
    hdma_lpuart_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_lpuart_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_lpuart_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_lpuart_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_lpuart_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_lpuart_rx.Init.Mode = DMA_NORMAL;
    hdma_lpuart_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_lpuart_rx) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }

    __HAL_LINKDMA(uartHandle, hdmarx, hdma_lpuart_rx);

    HAL_NVIC_SetPriority(LPUART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(LPUART1_IRQn);

    /* USART1 interrupt Init */
    LL_LPUART_SetWKUPType(LPUART1, LL_LPUART_WAKEUP_ON_RXNE);
    LL_LPUART_EnableIT_RXNE(LPUART1);
    LL_LPUART_EnableIT_WKUP(LPUART1);
}

void lpuart1_hal_init(void)
{
    HAL_RCC_DMA_CLOCK_ENABLE();

    hlpuart1.Instance = LPUART1;
    hlpuart1.Init.BaudRate = 4800;
    hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
    hlpuart1.Init.StopBits = UART_STOPBITS_1;
    hlpuart1.Init.Parity = UART_PARITY_NONE;
    hlpuart1.Init.Mode = UART_MODE_TX_RX;
    hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    hlpuart1.Init.OverSampling = UART_OVERSAMPLING_16;
    hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT | UART_ADVFEATURE_DMADISABLEONERROR_INIT;
    hlpuart1.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
    hlpuart1.AdvancedInit.DMADisableonRxError = UART_ADVFEATURE_DMA_DISABLEONRXERROR;
    hlpuart1.MspInitCallback = lpuart1_mspinit;
    if (HAL_UART_Init(&hlpuart1) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }
    printf("\n");
}

int lpuart1_open(FIL_HAND *fd)
{
    lpuart1_semap = creat_semqphore();
    INIT_PRINT((lpuart1_semap == NULL) ? INIT_FAIL : INIT_OK, "serial debug semqphore creat");

    lpuart1_hal_init();
    
    HAL_UART_Receive_DMA(&hlpuart1, lpuart1_msg, DEBUG_RX_MAX);
    __HAL_UART_CLEAR_IDLEFLAG(&hlpuart1);
    __HAL_UART_ENABLE_IT(&hlpuart1, UART_IT_IDLE);

    /* set the wake-up event:
    * specify wake-up on RXNE flag */
    WakeUpSelection.WakeUpEvent = UART_WAKEUP_ON_READDATA_NONEMPTY;

    return 0;
}

int lpuart1_write(FIL_HAND *fd, const void *buf, uint32_t count)
{
    if ((HAL_UART_Transmit(&hlpuart1, (uint8_t *)buf, count, 500)) != HAL_OK)
    {
        return -EIO;
    }

    return 0;
}

int lpuart1_read(FIL_HAND *fd, void *data, uint32_t count)
{
    uint32_t rxsize = 0;
    uint32_t rxlen = 0;
    uint8_t temp[DEBUG_RX_MAX];

    if (hlpuart1.hdmarx->Instance->CNDTR == DEBUG_RX_MAX) //缓存没有数据
    {
        //如果接收停止，开启接收
        if (hlpuart1.RxState == HAL_UART_STATE_READY)
        {
            log(DEBUG, "rx state is ready , start receive\n");
            HAL_UART_Receive_DMA(&hlpuart1, lpuart1_msg, DEBUG_RX_MAX);
        }
        //等待接收完成信号
        lpuart_enable_rx =  true;
        read_semaphore(lpuart1_semap, ser_read_mode);
    }

    rxsize = DEBUG_RX_MAX - hlpuart1.hdmarx->Instance->CNDTR;
     if (rxsize >= count) //接受到的总长度大于read希望获取的长度
    {
        memcpy(data, lpuart1_msg, count); //提取数据
        rxsize -= count;

        if (rxsize > 0) //还有剩余的数据
        {
            memcpy(temp, lpuart1_msg + count, rxsize); //其余数据放入临时缓存
            memset(lpuart1_msg, 0x00, DEBUG_RX_MAX);   //清空接收缓存
            memcpy(lpuart1_msg, temp, rxsize);
        }
        else
        {
            memset(lpuart1_msg, 0x00, DEBUG_RX_MAX); //清空接收缓存
        }

        rxlen = count;
    }
    else //接收到的数据长度小于read希望获取的长度
    {
        memcpy(data, lpuart1_msg, rxsize);       //提取数据
        memset(lpuart1_msg, 0x00, DEBUG_RX_MAX); //清空接收缓存

        rxlen = rxsize;
        rxsize = 0;
    }

    if (hlpuart1.RxState == HAL_UART_STATE_READY)
    {
        HAL_UART_Receive_DMA(&hlpuart1, lpuart1_msg + rxsize, DEBUG_RX_MAX - rxsize);
    }
    else
    {
        __HAL_DMA_DISABLE(hlpuart1.hdmarx);
        hlpuart1.hdmarx->Instance->CMAR = (uint32_t)(lpuart1_msg + rxsize);
        hlpuart1.hdmarx->Instance->CNDTR = DEBUG_RX_MAX - rxsize;
        __HAL_DMA_ENABLE(hlpuart1.hdmarx);
    }
    lpuart_enable_rx = false;
    return rxlen;
}

int lpuart1_resume(FIL_HAND *fd)
{

    return 0;
}



int lpuart1_suspend(FIL_HAND *fd)
{
  
    while (__HAL_UART_GET_FLAG(&hlpuart1, USART_ISR_BUSY) == SET);

    while (__HAL_UART_GET_FLAG(&hlpuart1, USART_ISR_REACK) == RESET);

    if (HAL_UARTEx_StopModeWakeUpSourceConfig(&hlpuart1, WakeUpSelection) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }
    
    __HAL_UART_ENABLE_IT(&hlpuart1, UART_IT_WUF);

    HAL_UARTEx_EnableStopMode(&hlpuart1);

    return 0;
}

int lpuart1_ioctl(FIL_HAND *fd, void *buf, uint32_t count, uint32_t flag)
{
    if (flag & IOCTL_FLAG_RESUME)
    {
        lpuart1_resume(fd);
    }
    else if (flag & IOCTL_FLAG_SUSPEND)
    {
        lpuart1_suspend(fd);
    }
    return 0;
}

struct file_operations lpuart1_fops =
{
    .onwer = 0,
    .open = lpuart1_open,
    .write = lpuart1_write,
    .read = lpuart1_read,
    .ioctl = lpuart1_ioctl,
    .resume = lpuart1_resume,
    .suspend = lpuart1_suspend,
};

struct miscdevice lpuart1_device =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = MODULE_NAME,
    .fops = &lpuart1_fops,
    .nodename = "device",
};

int lpuart1_init(void)
{
    return (misc_register(&lpuart1_device));
}

device_init(lpuart1_init);