#include "spi1.h"

#define MODULE_NAME "spi1"

SPI_HandleTypeDef hspi1;

void spi1_mspinit(SPI_HandleTypeDef *spiHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    __HAL_RCC_SPI1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE(); 

    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
}

int spi1_open(FIL_HAND *fd)
{
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 7;
    hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
    hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
    hspi1.MspInitCallback = spi1_mspinit;
    if (HAL_SPI_Init(&hspi1) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }
    return 0;
}

int spi1_write(FIL_HAND *fd, const void *buf, uint32_t count)
{
    uint8_t *sdata = (uint8_t *)buf;

    if( HAL_SPI_Transmit(&hspi1, sdata,count, 1000) != HAL_OK)
    {
        return -EBUSY;
    }

    return 0;
}

int spi1_read(FIL_HAND *fd, void *data, uint32_t count)
{
    uint8_t *rdata = (uint8_t *)data;

    if( HAL_SPI_Receive(&hspi1, rdata,count, 1000) != HAL_OK)
    {
        return -EBUSY;
    }

    return 0;
}

int spi1_write_read( uint8_t *w_data , uint8_t *r_data , uint16_t cnt )
{

    if( HAL_SPI_TransmitReceive(&hspi1, w_data, r_data , cnt, 1000) != HAL_OK)
    {
        return -EBUSY;
    }

    return 0;
}

int spi1_resume(FIL_HAND *fd)
{
    return 0;
}

int spi1_suspend(FIL_HAND *fd)
{
    return 0;
}

int spi1_ioctl(FIL_HAND *fd, void *buf, uint32_t count, uint32_t flag)
{
    if (flag & IOCTL_FLAG_RESUME)
    {
        spi1_resume(fd);
    }
    else if (flag & IOCTL_FLAG_SUSPEND)
    {
        spi1_suspend(fd);
    }
    else if( flag & IOCTL_SET_WR_FLAG )
    {
        spi_wr_t *wrt = (spi_wr_t *)buf;

        spi1_write_read(wrt->w_d , wrt->r_d , wrt->cnt);
    }
    
    
    return 0;
}

struct file_operations spi1_fops =
{
    .onwer = 0,
    .open = spi1_open,
    .write = spi1_write,
    .read = spi1_read,
    .ioctl = spi1_ioctl,
    .resume = spi1_resume,
    .suspend = spi1_suspend,
};

struct miscdevice spi1_device =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = MODULE_NAME,
    .fops = &spi1_fops,
    .nodename = "device",
};

int spi1_init(void)
{
    return (misc_register(&spi1_device));
}

device_init(spi1_init);

