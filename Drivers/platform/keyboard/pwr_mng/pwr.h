#ifndef __PWR_H
#define __PWR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "cmsis_os.h"

#define POWER_ON (0)

#define WAKEUP_FLAG_SLEEP_TIME 0x00000001
#define WAKEUP_FALG_BLE 0x00000002
#define WAKEUP_FALG_FINGER 0x00000004
#define WAKEUP_FALG_KEY 0x00000008
#define WAKEUP_FALG_IRQ 0x00000002


#define __HAL_RCC_XSE_CLK_PORT_ENABLE()   \
    do                                \
    {                                 \
        __HAL_RCC_GPIOC_CLK_ENABLE(); \
        __HAL_RCC_GPIOH_CLK_ENABLE(); \
    } while (0)    
    
#define __HAL_RCC_GPIO_CLK_ENABLE()   \
    do                                \
    {                                 \
        __HAL_RCC_GPIOA_CLK_ENABLE(); \
        __HAL_RCC_GPIOB_CLK_ENABLE(); \
        __HAL_RCC_GPIOC_CLK_ENABLE(); \
        __HAL_RCC_GPIOD_CLK_ENABLE(); \
        __HAL_RCC_GPIOE_CLK_ENABLE(); \
        __HAL_RCC_GPIOH_CLK_ENABLE(); \
    } while (0)

#define __HAL_RCC_GPIO_CLK_DISABLE()   \
    do                                 \
    {                                  \
        __HAL_RCC_GPIOA_CLK_DISABLE(); \
        __HAL_RCC_GPIOB_CLK_DISABLE(); \
        __HAL_RCC_GPIOC_CLK_DISABLE(); \
        __HAL_RCC_GPIOD_CLK_DISABLE(); \
        __HAL_RCC_GPIOE_CLK_DISABLE(); \
        __HAL_RCC_GPIOH_CLK_DISABLE(); \
    } while (0)
#define __HAL_RCC_GPIO_DEINIT()   \
    do                                 \
    {                                  \
        HAL_GPIO_DeInit(GPIOA , GPIO_PIN_All); \
        HAL_GPIO_DeInit(GPIOB , GPIO_PIN_All); \
        HAL_GPIO_DeInit(GPIOC , GPIO_PIN_All); \
        HAL_GPIO_DeInit(GPIOD , GPIO_PIN_All); \
        HAL_GPIO_DeInit(GPIOE , GPIO_PIN_All); \
        HAL_GPIO_DeInit(GPIOH , GPIO_PIN_All); \
    } while (0)

#define __HAL_GPIO_SET_ANALOG()                     \
    do                                              \
    {                                               \
        GPIO_InitTypeDef GPIO_InitStructure;        \
        GPIO_InitStructure.Pin = GPIO_PIN_All;      \
        GPIO_InitStructure.Mode = GPIO_MODE_ANALOG; \
        GPIO_InitStructure.Pull = GPIO_NOPULL;      \
        HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);  \
        HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);  \
        HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);  \
        HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);  \
        HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);  \
        HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);  \
    } while (0)

      
extern void pwr_set_wakeup_sur(uint32_t sur);

#ifdef __cplusplus
}
#endif

#endif /* __PWR_H */
