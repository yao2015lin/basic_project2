
/* Includes ------------------------------------------------------------------*/
//#include "startup.h"
#include "BAT32G157.h"
#include "cmsis_os.h"
#include "init_d.h"
#include "unit.h"
#include "miscdevice.h"
//#include "serial1.h"
#include "time.h"
#include "cmb_def.h"
#include "version.h"
#include "cm_backtrace.h"
#include "watchdog.h"

osThreadId startup_handle;

uint8_t chipUid[12];


void HAL_GetUID(void)
{
    uint8_t i;
    uint8_t *pUID = (uint8_t *)(UID_BASE);

    for (i = 0; i != 12; i++)
    {
        chipUid[i] = *pUID++;
    }
}
/**
  * @brief  show system id ,and clock
  * @retval int
  */

void prtsrce_sysinf(void)
{
    uint8_t i = 0;
    
    printf("*****************************************************\n");
    printf(" *\tTerminus lock use freertos ,version:V10.0.1\n");
    printf(" *\tBy terminus hwteam(BeiJing), project started in December 2018.\n");
    printf(" *\tBulid data=%s %s\n" ,__DATE__, __TIME__);
    printf(" *\tCodeing version =%s\n" ,PRG_VERSION);
    printf(" *\tSoft version =%s\n" ,SOFT_VERSION);
    printf(" *\tHardware version =%s\n" ,HARD_VERSION);
    printf(" *\tHardfault debug tool version =%s\n" ,CMB_SW_VERSION);
    printf(" *\tCPU:BAT32G157\n");
    //printf(" *\tHCLK:%dHz\n", HAL_RCC_GetHCLKFreq());
    //printf(" *\tPCLK1:%dHz\n", HAL_RCC_GetPCLK1Freq());
   // printf(" *\tPCLK2:%dHz\n", HAL_RCC_GetPCLK2Freq());
    //printf(" *\tHAL version:0x%X\n", HAL_GetHalVersion());
   // printf(" *\tDevice revision ID:0x%X\n", HAL_GetREVID());
   // printf(" *\tDevice ID:0x%X\n", HAL_GetDEVID());
    printf(" *\tUnique device ID:0x");
    for (i = 0; i < 12; i++)
    {
        printf("%X", chipUid[i]);
    }
    printf("\n");
    printf("*****************************************************");
    printf("\n\n");
}
DEV_HAND nvfd;
void startup_task(void const *argument)
{
    /* memory , plarform , components ...init*/
    driver_init();
    
    /* task init*/
    task_start();

   // HAL_Delay(2000);

    while (1)
    {
        //feed_dog();
        osDelay(20); 
    }
}

/**
  * @brief  creat start task, module init must into task
  * @retval int
  */
void startup(void)
{
    osThreadDef(stup_task, startup_task, osPriorityNormal,    0, 256);
    startup_handle = osThreadCreate(osThread(stup_task), NULL);
}

/**
  * @brief  The application entry point.
  * @retval int
  */
 

int main(void) 
{
    uint32_t msCnt;
    /* Output error message for HARDFAULT*/
    //HAL_HardfaultDebug_init();
    
    /* reset of all peripherals, Initializes the Flash interface and the Systick. */
    //HAL_Init();

    /* Get chip uid*/
    HAL_GetUID();

    /* Configure the system clock */
    //HAL_Sysclk_Init();
    SystemCoreClockUpdate();
	  msCnt = SystemCoreClock / 1000;
	  SysTick_Config(msCnt); 

    /* Config pwr clock*/
    //HAL_Pwr_Init();
    
    /* Init serial for printf port*/
    //HAL_Debug_init();

    /* Printf system info ,clock device version...*/
    prtsrce_sysinf();
    
    /* Call init function for freertos objects (in freertos.c) */
    startup();

    /* Start scheduler */
    osKernelStart();

    /* We should never get here as control is now taken by the scheduler */

    /* Infinite loop */
    while (1);
}

/**
  * @brief  idle hook
  * @retval None
  */
void vApplicationIdleHook(void)
{

}
/**
  * @brief  Overflow stack
  * @retval None
  */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
    log(ERR, "Into vApplicationStackOverflowHook , task name = %s .\n", pcTaskName);
}

/**
  * @brief  malloc fail 
  * @retval None
  */
void vApplicationMallocFailedHook(void)
{
    log(ERR, "Into vApplicationMallocFailedHook\n");
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(char *file, int line)
{
    /* User can add his own implementation to report the HAL error return state */
    log(ERR, "Into Error_Handler , file=%s ,line=%d.\n", file ,line);
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
extern void after_sleep(void);
void assert_failed(char *file, uint32_t line)
{
    after_sleep();
    printf("Wrong parameters value: file %s on line %d\r\n", file, line);
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


/***********************************************************************************************************************
* Function Name: HardFault_Handler
* Description  : Hard Fault handler to report stacked register values
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
// Hard Fault handler in C, with stack frame location and LR value
// extracted from the assembly wrapper as input parameters
void hard_fault_handler_c(unsigned int * hardfault_args, unsigned lr_value)
{
	unsigned int stacked_r0;
	unsigned int stacked_r1;
	unsigned int stacked_r2;
	unsigned int stacked_r3;
	unsigned int stacked_r12;
	unsigned int stacked_lr;
	unsigned int stacked_pc;
	unsigned int stacked_psr;
	
	stacked_r0 = ((unsigned long) hardfault_args[0]);
	stacked_r1 = ((unsigned long) hardfault_args[1]);
	stacked_r2 = ((unsigned long) hardfault_args[2]);
	stacked_r3 = ((unsigned long) hardfault_args[3]);
	stacked_r12 = ((unsigned long) hardfault_args[4]);
	stacked_lr = ((unsigned long) hardfault_args[5]);
	stacked_pc = ((unsigned long) hardfault_args[6]);
	stacked_psr = ((unsigned long) hardfault_args[7]);
	
	printf ("[Hard fault handler]\r\n");
	printf ("R0 = %x\r\n", stacked_r0);
	printf ("R1 = %x\r\n", stacked_r1);
	printf ("R2 = %x\r\n", stacked_r2);
	printf ("R3 = %x\r\n", stacked_r3);
	printf ("R12 = %x\r\n", stacked_r12);
	printf ("Stacked LR = %x\r\n", stacked_lr);
	printf ("Stacked PC = %x\r\n", stacked_pc);
	printf ("Stacked PSR = %x\r\n", stacked_psr);
	printf ("Current LR = %x\r\n", lr_value);
	
	while(1); // endless loop
}

/***********************************************************************************************************************
* Function Name: HardFault_Handler
* Description  : Assembly wrapper using Embedded Assembler in Keil MDK
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
// Hard Fault handler wrapper in assembly
// It extracts the location of stack frame and passes it to handler
// in C as a pointer. We also extract the LR value as second
// parameter.
__asm void HardFault_Handler(void)
{
		MOVS 	r0, #4
		MOV 	r1, LR
		TST 	r0, r1
		BEQ 	stacking_used_MSP
		MRS 	R0, PSP ; first parameter - stacking was using PSP
		B 		get_LR_and_branch
stacking_used_MSP
		MRS 	R0, MSP ; first parameter - stacking was using MSP
get_LR_and_branch
		MOV 	R1, LR ; second parameter is LR current value
		LDR 	R2,=__cpp(hard_fault_handler_c)
		BX 		R2
}