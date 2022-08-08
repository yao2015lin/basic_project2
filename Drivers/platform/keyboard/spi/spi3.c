#include "spi3.h"

SPI_HandleTypeDef hspi;
DMA_HandleTypeDef hdma_spi_tx;


void DMA2_Channel2_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_spi_tx);
}

static void spi_mspdeinit(SPI_HandleTypeDef *spiHandle)
{
    /* Peripheral clock disable */
    __HAL_RCC_SPI3_CLK_DISABLE();

    /**SPI1 GPIO Configuration    
    PC12     ------> SPI3_MOSI 
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_12);

    /* Peripheral DMA DeInit*/
    HAL_DMA_DeInit(spiHandle->hdmatx);
}

static void spi_mspinit(SPI_HandleTypeDef *spiHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    /* Peripheral clock enable */
    __HAL_RCC_SPI3_CLK_ENABLE();
    __HAL_RCC_DMA2_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**SPI3 GPIO Configuration    
    	PC12     ------> SPI1_MOSI 
    	*/
    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* SPI3 DMA Init */
    /* SPI3_TX Init */
    hdma_spi_tx.Instance = DMA2_Channel2;
    hdma_spi_tx.Init.Request = DMA_REQUEST_3;
    hdma_spi_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_spi_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spi_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_spi_tx.Init.Mode = DMA_NORMAL;
    hdma_spi_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_spi_tx) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }
    
    HAL_NVIC_SetPriority(DMA2_Channel2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA2_Channel2_IRQn);
  
    __HAL_LINKDMA(spiHandle, hdmatx, hdma_spi_tx);
    
}

static int spi_open(FIL_HAND *fd)
{
    hspi.Instance = SPI3;
	hspi.Init.Mode = SPI_MODE_MASTER;
	hspi.Init.Direction = SPI_DIRECTION_2LINES;
	hspi.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi.Init.NSS = SPI_NSS_SOFT;
	hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	hspi.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi.Init.CRCPolynomial = 7;
	hspi.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	hspi.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
    hspi.MspInitCallback = spi_mspinit;
    hspi.MspDeInitCallback = spi_mspdeinit;
	if (HAL_SPI_Init(&hspi) != HAL_OK)
	{
		Error_Handler(__FILE__, __LINE__);
	}
    return 0;
}

static int spi_write(FIL_HAND *fd, const void *buf, uint32_t count)
{
    uint8_t *sdata = (uint8_t *)buf;

    if (HAL_SPI_Transmit_DMA(&hspi, sdata, count) != HAL_OK)
    {
        return -EBUSY;
    }

    return 0;
}


static int spi_resume(FIL_HAND *fd)
{
    return 0;
}

static int spi_suspend(FIL_HAND *fd)
{
    return 0;
}

static int spi_ioctl(FIL_HAND *fd, void *buf, uint32_t count, uint32_t flag)
{
    if (flag & IOCTL_FLAG_RESUME)
    {
        spi_resume(fd);
    }
    else if (flag & IOCTL_FLAG_SUSPEND)
    {
        spi_suspend(fd);
    }
    

    return 0;
}

static struct file_operations spi_fops =
{
    .onwer = 0,
    .open = spi_open,
    .write = spi_write,
    .ioctl = spi_ioctl,
    .resume = spi_resume,
    .suspend = spi_suspend,
};

static struct miscdevice spi_device =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = "spi3",
    .fops = &spi_fops,
    .nodename = "device",
};

static int spi_init(void)
{
    return (misc_register(&spi_device));
}

device_init(spi_init);
