#ifndef __I2C_TYPE_H
#define __I2C_TYPE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include "BAT32G157.h"
#include "major.h"
#include "unit.h"
#include "init_d.h"
#include "miscdevice.h"
#include "priv_os.h"
#include "startup.h"

#define I2C_ACKNOWLEDGE        	FALSE
#define I2C_NON_ACKNOWLEDGE     TRUE

typedef enum _I2cIoTransType{
    I2C_TRANS_READ,
    I2C_TRANS_WRITE
}I2cIoTransType;

typedef enum
{
    I2C_INPUT_MODE , 
    I2C_OUTPUT_MODE,
}i2cgpio_mode_n;

typedef struct 
{
    /* data */
    uint16_t sal_addr;
    uint16_t mem_addr;
    uint8_t  mem_size;
    uint8_t *pdata;
    uint16_t size;
    uint32_t timeout; 
}i2c_data_t;

typedef struct 
{
    GPIO_TypeDef *gpio;
    uint16_t pin;
}platform_gpio_t;

typedef struct 
{
    platform_gpio_t sda;
    platform_gpio_t scl;
    uint32_t dummy;
}i2c_stru_t;

extern uint8_t i2cx_read(i2c_stru_t *i2c_stp ,uint8_t slaveAdr,uint16_t subAdr,uint8_t *buff,uint16_t bufLen);


#ifdef __cplusplus
}
#endif

#endif /* __I2C_TYPE_H */