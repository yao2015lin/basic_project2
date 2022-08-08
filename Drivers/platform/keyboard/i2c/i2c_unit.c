#include "i2c_type.h"

void i2c_delay_us(uint32_t us)
{
    while (us--)
    {
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
        __asm("NOP");
    }
}

void i2c_delay_bit(void)
{
    i2c_delay_us(50);
}

void i2c_delay_byte(void)
{
    i2c_delay_us(100);
}

void i2c_set_scl(i2c_stru_t *i2c_gpio)
{
    HAL_GPIO_WritePin(i2c_gpio->scl.gpio, i2c_gpio->scl.pin, GPIO_PIN_SET);
}

void i2c_clr_scl(i2c_stru_t *i2c_gpio)
{
    HAL_GPIO_WritePin(i2c_gpio->scl.gpio, i2c_gpio->scl.pin, GPIO_PIN_RESET);
}

void i2c_set_sda(i2c_stru_t *i2c_gpio)
{
    HAL_GPIO_WritePin(i2c_gpio->sda.gpio, i2c_gpio->sda.pin, GPIO_PIN_SET);
}

void i2c_clr_sda(i2c_stru_t *i2c_gpio)
{
    HAL_GPIO_WritePin(i2c_gpio->sda.gpio, i2c_gpio->sda.pin, GPIO_PIN_RESET);
}

void i2c_set_gpio_mode(platform_gpio_t *i2c_gpio_pin, uint8_t mode)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    if (mode == I2C_INPUT_MODE)
    {
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    }
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Pin = i2c_gpio_pin->pin;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(i2c_gpio_pin->gpio, &GPIO_InitStruct);
}

uint8_t i2c_get_sda(i2c_stru_t *i2c_gpio)
{
    uint8_t Ret;

    i2c_set_gpio_mode(&i2c_gpio->sda, I2C_INPUT_MODE);
    if (HAL_GPIO_ReadPin(i2c_gpio->sda.gpio, i2c_gpio->sda.pin) == GPIO_PIN_SET)
    {
        Ret = TRUE;
    }
    else
    {
        Ret = FALSE;
    }
    i2c_set_gpio_mode(&i2c_gpio->sda, I2C_OUTPUT_MODE);
    return Ret;
}

uint8_t i2c_get_scl(i2c_stru_t *i2c_gpio)
{
    uint8_t Ret;
    i2c_set_gpio_mode(&i2c_gpio->scl, I2C_INPUT_MODE);
    if (HAL_GPIO_ReadPin(i2c_gpio->scl.gpio, i2c_gpio->scl.pin) == GPIO_PIN_SET)
    {
        Ret = TRUE;
    }
    else
    {
        Ret = FALSE;
    }
    i2c_set_gpio_mode(&i2c_gpio->scl, I2C_OUTPUT_MODE);
    return Ret;
}

static void i2c_set_scl_check(i2c_stru_t *i2c_stp, uint8_t set)
{
    volatile uint16_t dummy;

    if (set == TRUE)
    {
        i2c_set_scl(i2c_stp);
    }
    else
    {
        i2c_clr_scl(i2c_stp);
    }

    if (set == TRUE)
    {
        dummy = i2c_stp->dummy;
        while ((i2c_get_scl(i2c_stp) == FALSE) && (dummy--))
            ;
    }
}

static void i2c_set_sda_check(i2c_stru_t *i2c_stp, uint8_t set)
{
    volatile uint16_t dummy;

    if (set == TRUE)
    {
        i2c_set_sda(i2c_stp);
    }
    else
    {
        i2c_clr_sda(i2c_stp);
    }

    if (set == TRUE)
    {
        dummy = i2c_stp->dummy;
        while ((i2c_get_sda(i2c_stp) == FALSE) && (dummy--))
            ;
    }
}

static BOOL i2c_start(i2c_stru_t *i2c_stp)
{
    volatile uint8_t status = TRUE;

    i2c_set_sda_check(i2c_stp, TRUE);

    i2c_delay_bit();

    i2c_set_scl_check(i2c_stp, TRUE);

    i2c_delay_bit();

    if ((i2c_get_sda(i2c_stp) == FALSE) || (i2c_get_scl(i2c_stp) == FALSE))
    {
        status = FALSE;
    }
    else
    {
        i2c_clr_sda(i2c_stp);
        i2c_delay_bit();
        i2c_clr_scl(i2c_stp);
        i2c_delay_bit();
    }
    return status;
}

void i2c_stop(i2c_stru_t *i2c_stp)
{
    i2c_clr_scl(i2c_stp);
    i2c_delay_bit();
    i2c_clr_sda(i2c_stp);
    i2c_delay_bit();
    i2c_set_scl_check(i2c_stp, TRUE);
    i2c_delay_bit();
    i2c_set_sda_check(i2c_stp, TRUE);
    i2c_delay_bit();
}

uint8_t i2c_receive_byte(i2c_stru_t *i2c_stp, BOOL ack)
{
    volatile uint8_t receive = 0;
    uint8_t mask = BIT7;
    while (mask)
    {
        i2c_set_sda(i2c_stp);
        i2c_set_scl_check(i2c_stp, TRUE);
        i2c_delay_bit();
        if (i2c_get_sda(i2c_stp) == TRUE)
            receive |= mask;
        i2c_clr_scl(i2c_stp);
        i2c_delay_bit();
        mask >>= 1;
    }
    if (ack == TRUE)
    {
        i2c_set_sda_check(i2c_stp, I2C_ACKNOWLEDGE);
    }
    else
    {
        i2c_set_sda_check(i2c_stp, I2C_NON_ACKNOWLEDGE);
    }
    i2c_delay_bit();
    i2c_set_scl_check(i2c_stp, TRUE);
    i2c_delay_bit();
    i2c_clr_scl(i2c_stp);
    i2c_delay_bit();
    return receive;
}

uint8_t i2c_send_byte(i2c_stru_t *i2c_stp, uint8_t val)
{
    volatile uint8_t mark = BIT7;
    volatile uint8_t ack = I2C_NON_ACKNOWLEDGE;

    i2c_clr_scl(i2c_stp);
    i2c_delay_bit();
    while (mark)
    {
        if (val & mark)
            i2c_set_sda_check(i2c_stp, TRUE);
        else
            i2c_set_sda_check(i2c_stp, FALSE);

        i2c_delay_bit();
        i2c_set_scl_check(i2c_stp, TRUE);
        i2c_delay_bit();
        i2c_clr_scl(i2c_stp);
        i2c_delay_bit();
        mark >>= 1;
    }
    // recieve acknowledge
    i2c_set_sda(i2c_stp);
    i2c_delay_bit();
    i2c_set_scl_check(i2c_stp, TRUE);
    i2c_delay_bit();
    ack = i2c_get_sda(i2c_stp);
    i2c_clr_scl(i2c_stp);
    i2c_delay_bit();
    return (ack);
}

uint8_t i2c_access_start(i2c_stru_t *i2c_stp, uint8_t ucSlaveAdr, I2cIoTransType Trans)
{
    volatile uint8_t dummy;

    if (Trans == I2C_TRANS_READ)
    {
        ucSlaveAdr |= BIT0;
    }
    else
    {
        ucSlaveAdr &= ~BIT0;
    }
    dummy = i2c_stp->dummy;
    while (dummy--)
    {
        if (i2c_start(i2c_stp) == FALSE)
        {
            continue;
        }
        if (i2c_send_byte(i2c_stp, ucSlaveAdr) == I2C_ACKNOWLEDGE)
        { // check acknowledge

            return TRUE;
        }
        i2c_delay_byte();
        i2c_stop(i2c_stp);
    }
    return FALSE;
}



uint8_t i2cx_read(i2c_stru_t *i2c_stp ,uint8_t slaveAdr,uint16_t subAdr,uint8_t *buff,uint16_t bufLen)
{

    volatile uint8_t dummy = 20;

	while(dummy--){
		if (i2c_access_start(i2c_stp, slaveAdr, I2C_TRANS_WRITE) == FALSE)
        {
			continue;
		}
		if (i2c_send_byte(i2c_stp, subAdr) == I2C_NON_ACKNOWLEDGE)
        {
			continue;
		}
		if (i2c_access_start(i2c_stp, slaveAdr, I2C_TRANS_READ) == FALSE)
        {
			continue;
		}
		while(bufLen--)
        {
			*buff = i2c_receive_byte(i2c_stp, (bufLen>0)?TRUE:FALSE);
			buff++;
		}
		break;
	}
    
	i2c_stop(i2c_stp);  
    
    if( dummy == 0)
    {
        return FALSE;
    }
    
    return TRUE;
}
