#include "startup.h"
#include "stm32l4xx_hal.h"
#include "cmsis_os.h"
#include "init_d.h"
#include "unit.h"
#include "miscdevice.h"
#include "shell_cmd.h"
#include "time.h"
#include "pwr.h"
#include "sleep_time.h"

extern RTC_HandleTypeDef hrtc;

#define CHIP_SYSTICK_CTRL_REG (*((volatile uint32_t *)0xe000e010))
#define CHIP_SYSTICK_ENABLE_BIT (1UL << 0UL)

uint32_t wakeup_sur = 0;


/**
  * @brief set rtc wakeup time
  * @retval int
  */
#if USE_RTC_WAKEUP
void platform_wakeup_times(TickType_t times)
{
    uint32_t sleepTime = 0, rtcClock = RTC_WAKEUPCLOCK_RTCCLK_DIV16;

    //log(DEBUG, "I will sleep , time =%u\n", xModifiableIdleTime);
    if ((times > 29990) && (times < 30010))
    {
        wakeup_sur |= WAKEUP_FLAG_WATCHDOG;
    }

    if ((times / 1000) <= RTC_WUTR_WUT) //超出休眠最大时间
    {
        sleepTime = (times * 38) / 16;

        if (sleepTime > RTC_WUTR_WUT) //超出ms级别休眠最大时间，以秒为单位休眠
        {
            //log(DEBUG,"Wakeup use 1hz clock.\n");
            sleepTime = (times) / 1000;
            if ((times % 1000 > 500) && (sleepTime + 1 <= RTC_WUTR_WUT))
            {
                sleepTime += 1; //补偿
            }
            rtcClock = RTC_WAKEUPCLOCK_CK_SPRE_16BITS;
        }
        //log(DEBUG, "RTC wakeup time=%d\n" , sleepTime);
        HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
        HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, sleepTime, rtcClock);
    }
    else
    {
        log(DEBUG, "Outof wakeup time max count, sleep forever until IO interrupts.\n");
    }
}
#endif


void before_sleep(TickType_t xModifiableIdleTime)
{
    task_suspend();
}

/**
  * @brief resume all task
  * @retval null
  */

void after_sleep(TickType_t xExpectedIdleTime)
{
    
    task_resume( );
    //log(DEBUG,"wakeup ,pin5_count=%d , lpuart count=%d\n" , pin5_count , lpuartcount);

}

/**
  * @brief into stop2 mode
  * @retval null
  */
void _sleep(void)
{
    HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
}

/**
  * @brief freertos callback function
  * @retval null
  */
void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime)
{
#if (POWER_ON == 1)

   CHIP_SYSTICK_CTRL_REG &= ~CHIP_SYSTICK_ENABLE_BIT; //停止systick

    if (eTaskConfirmSleepModeStatus() == eAbortSleep)
    {
        CHIP_SYSTICK_CTRL_REG |= CHIP_SYSTICK_ENABLE_BIT;
    }
    else //没有任务需要运行，继续进入休眠
    {
        before_sleep(xExpectedIdleTime);
        set_time_sleep(xExpectedIdleTime);
        if (xExpectedIdleTime > 0)
        {
            __DSB();
            _sleep();
            __ISB();
        }
        after_sleep(xExpectedIdleTime);

        CHIP_SYSTICK_CTRL_REG |= CHIP_SYSTICK_ENABLE_BIT;
        if( wakeup_sur & WAKEUP_FLAG_SLEEP_TIME)
        {
            wakeup_sur &= ~WAKEUP_FLAG_SLEEP_TIME;
            vTaskStepTick(xExpectedIdleTime - 1);
        }
        else
        {
            uint32_t sleep_time;

            sleep_time = get_time_count();
            stop_time_sleep();
            if( sleep_time >= xExpectedIdleTime)
            {
                sleep_time = xExpectedIdleTime;
            }
            vTaskStepTick(sleep_time);
        }
        HAL_Delay(1);
    }
#endif
}

void pwr_set_wakeup_sur(uint32_t sur)
{
    wakeup_sur |= sur;
}

void HAL_Pwr_Init(void)
{
    wakeup_sur = 0;
    
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_MSI);
}
