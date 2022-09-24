#ifndef __STARTUP_H
#define __STARTUP_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "cmsis_os.h"

#include "uart0.h"
extern void com0_hal_init( void );  
#define HAL_Debug_init  com0_hal_init

  
extern uint8_t chipUid[12];

//extern void HAL_Pwr_Init( void );
//extern void HAL_Sysclk_Init(void);
//extern void HAL_Sysclk_resume(void);
extern void Error_Handler(char * file, int line);
extern void assert_failed(char *file, uint32_t line);
extern void vApplicationIdleHook(void);
extern void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);
extern void vApplicationMallocFailedHook(void);


#ifdef __cplusplus
}
#endif

#endif /* __STARTUP_H */
