#ifndef __SERIALX_H
#define __SERIALX_H

#ifdef __cplusplus
extern "C"
{
#endif

  /**
  * @}
  */
#include <stdbool.h>
#include <stdlib.h>
  
#include "BAT32G157.h"
#include "major.h"
#include "unit.h"
#include "init_d.h"
#include "miscdevice.h"
#include "priv_os.h"
#include "startup.h"
/**
  * @}
  */
struct serial_gpio
{
    GPIO_TypeDef  *gpiox;
    uint16_t pin;
    uint32_t alternate;
};

struct serial_dev
{
    uint8_t *name;
    struct serial_gpio tx;
    struct serial_gpio rx;
    UART_HandleTypeDef  com_handle;
    DMA_HandleTypeDef   dma_handle;
    sc_semaphore_t  mutex;
    IRQn_Type irqn;
    uint32_t band;
    uint32_t mode;
    
    uint8_t *buf;
    uint32_t size;
    uint32_t max_size;

    int (*clock_init)( void );
};
/**
  * @}
  */
int serial_open(FIL_HAND *fd);

int serial_write(FIL_HAND *fd, const void *buf, uint32_t count);

int serial_read(FIL_HAND *fd, void *data, uint32_t count);

  /**
  * @}
  */


#ifdef __cplusplus
}
#endif

#endif /* __SERIAL1_H */
