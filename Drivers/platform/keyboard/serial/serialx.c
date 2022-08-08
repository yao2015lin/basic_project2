#include "serialx.h"

static struct serial_dev *static_dev;

static struct serial_dev *serial1 = NULL;
static struct serial_dev *serial2 = NULL;
static struct serial_dev *serial3 = NULL;
static struct serial_dev *serial4 = NULL;
uint32_t temp;

void serial_irq_handle(  struct serial_dev *dev )
{
    if (((__HAL_UART_GET_FLAG(&dev->com_handle, UART_FLAG_IDLE)) != RESET))
    {
        __HAL_UART_CLEAR_IDLEFLAG(&dev->com_handle);
        temp = dev->com_handle.Instance->ISR;
        temp = dev->com_handle.Instance->RDR;

        if (dev->mutex != NULL)
        {
            send_semaphore(dev->mutex);
        }
    }
    HAL_UART_IRQHandler(&dev->com_handle);
}

void USART1_IRQHandler(void)
{
    if( serial1 != NULL )
    {
        serial_irq_handle(serial1);
    }
}

void USART2_IRQHandler(void)
{
    if( serial2 != NULL )
    {
        serial_irq_handle(serial2);
    }
}

void USART3_IRQHandler(void)
{
    if( serial3 != NULL )
    {
        serial_irq_handle(serial3);
    }
}

void USART4_IRQHandler(void)
{
    if( serial4 != NULL )
    {
        serial_irq_handle(serial4);
    }
}

int serial_clock_init(struct serial_dev *dev)
{
    if( dev->clock_init )
    {
        if( dev->clock_init() < 0)
        {
            log(ERR, "clock init is err\n");
            return -1;
        }
    }
    else
    {
        log(ERR,"clock is no init.\n");
    }

    return 0;
}

void serial_mspinit(UART_HandleTypeDef *handle)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    DMA_HandleTypeDef hdma;

    GPIO_InitStruct.Pin = static_dev->tx.pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = static_dev->tx.alternate;
    HAL_GPIO_Init(static_dev->tx.gpiox, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = static_dev->rx.pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = static_dev->rx.alternate;
    HAL_GPIO_Init(static_dev->rx.gpiox, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_RX Init */
    //static_dev->dma_handle.Instance = static_dev->dma_channel;
    static_dev->dma_handle.Init.Request = DMA_REQUEST_2;
    static_dev->dma_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;
    static_dev->dma_handle.Init.PeriphInc = DMA_PINC_DISABLE;
    static_dev->dma_handle.Init.MemInc = DMA_MINC_ENABLE;
    static_dev->dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    static_dev->dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    static_dev->dma_handle.Init.Mode = DMA_NORMAL;
    static_dev->dma_handle.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&static_dev->dma_handle) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }

    __HAL_LINKDMA(&static_dev->com_handle, hdmarx, static_dev->dma_handle);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(static_dev->irqn, 5, 0);
    HAL_NVIC_EnableIRQ(static_dev->irqn);
}


void serial_hal_init(struct serial_dev *dev)
{
    if( dev->com_handle.Instance == USART1 )
    {
        serial1 = dev;
    }
    else if( dev->com_handle.Instance == USART2 )
    {
        serial2 = dev;
    }
    else if( dev->com_handle.Instance == USART3 )
    {
        serial3 = dev;
    }
    else if( dev->com_handle.Instance == UART4 )
    {
        serial4 = dev;
    }

    //dev->com_handle.Instance = dev->com;
    dev->com_handle.Init.BaudRate = dev->band;
    dev->com_handle.Init.WordLength = UART_WORDLENGTH_8B;
    dev->com_handle.Init.StopBits = UART_STOPBITS_1;
    dev->com_handle.Init.Parity = UART_PARITY_NONE;
    dev->com_handle.Init.Mode = UART_MODE_TX_RX;
    dev->com_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    dev->com_handle.Init.OverSampling = UART_OVERSAMPLING_16;
    dev->com_handle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    dev->com_handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT | UART_ADVFEATURE_DMADISABLEONERROR_INIT;
    dev->com_handle.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
    dev->com_handle.AdvancedInit.DMADisableonRxError = UART_ADVFEATURE_DMA_DISABLEONRXERROR;
    dev->com_handle.MspInitCallback = serial_mspinit;
    if (HAL_UART_Init(&dev->com_handle) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }

}


int serial_open(FIL_HAND *fd)
{
    struct serial_dev *dev = (struct serial_dev *)fd;

    if( dev == NULL )
    {
        log(ERR, "open serial is fail , handle is null\n");
        return -1;
    }

    static_dev = dev;

    serial_clock_init(dev);

    serial_hal_init(dev);

    dev->mutex = creat_semqphore();
    INIT_PRINT((dev->mutex == NULL) ? INIT_FAIL : INIT_OK, "\nserial debug semqphore creat");

    HAL_UART_Receive_DMA(&dev->com_handle, dev->buf, dev->max_size);
    __HAL_UART_CLEAR_IDLEFLAG(&dev->com_handle);
    __HAL_UART_ENABLE_IT(&dev->com_handle, UART_IT_IDLE);

    return 0;
}


int serial_write(FIL_HAND *fd, const void *buf, uint32_t count)
{
    struct serial_dev *dev = (struct serial_dev *)fd;

    if ((HAL_UART_Transmit(&dev->com_handle, (uint8_t *)buf, count, 500)) != HAL_OK)
    {
        return -EIO;
    }

    return 0;
}


int serial_read(FIL_HAND *fd, void *data, uint32_t count)
{
    struct serial_dev *dev = (struct serial_dev *)fd;
    uint32_t rxsize = 0;
    uint32_t rxlen = 0;
    uint8_t *temp;

    temp = (uint8_t *)malloc(sizeof(uint8_t)*dev->max_size);
    if( temp == NULL)
    {
        log(ERR,"serial read fail.\n");
        return -1;
    }
    if (dev->com_handle.hdmarx->Instance->CNDTR == dev->max_size) //缓存没有数据
    {
        //如果接收停止，开启接收
        if (dev->com_handle.RxState == HAL_UART_STATE_READY)
        {
            log(DEBUG, "rx state is ready , start receive\n");
            HAL_UART_Receive_DMA(&dev->com_handle, dev->buf, dev->max_size);
        }
        //等待接收完成信号
        read_semaphore(dev->mutex, dev->mode);
    }

    rxsize = dev->max_size - dev->com_handle.hdmarx->Instance->CNDTR;
    if (rxsize >= count) //接受到的总长度大于read希望获取的长度
    {
        memcpy(data, dev->buf, count); //提取数据
        rxsize -= count;

        if (rxsize > 0) //还有剩余的数据
        {
            memcpy(temp, dev->buf + count, rxsize); //其余数据放入临时缓存
            memset(dev->buf, 0x00, dev->max_size);   //清空接收缓存
            memcpy(dev->buf, temp, rxsize);
        }
        else
        {
            memset(dev->buf, 0x00, dev->max_size); //清空接收缓存
        }

        rxlen = count;
    }
    else //接收到的数据长度小于read希望获取的长度
    {
        memcpy(data, dev->buf, rxsize);       //提取数据
        memset(dev->buf, 0x00, dev->max_size); //清空接收缓存

        rxlen = rxsize;
        rxsize = 0;
    }

    if (dev->com_handle.RxState == HAL_UART_STATE_READY)
    {
        HAL_UART_Receive_DMA(&dev->com_handle, dev->buf + rxsize, dev->max_size - rxsize);
    }
    else
    {
        __HAL_DMA_DISABLE(dev->com_handle.hdmarx);
        dev->com_handle.hdmarx->Instance->CMAR = (uint32_t)(dev->buf + rxsize);
        dev->com_handle.hdmarx->Instance->CNDTR = dev->max_size - rxsize;
        __HAL_DMA_ENABLE(dev->com_handle.hdmarx);
    }
    free(temp);
    return rxlen;
}

