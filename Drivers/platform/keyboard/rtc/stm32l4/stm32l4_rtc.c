#include "stm32l4_rtc.h"

#define MODULE_NAME "stm32l4_rtc"

RTC_HandleTypeDef hrtc;

void stm32l4rtc_mspinit(RTC_HandleTypeDef *rtcHandle)
{
    __HAL_RCC_RTC_ENABLE();

    /* RTC interrupt Init */
    HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}

int stm32l4rtc_open(FIL_HAND *fd)
{
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};
    RTC_AlarmTypeDef sAlarm = {0};

    /**Initialize RTC Only
    */
    hrtc.Instance = RTC;
    hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
    hrtc.Init.AsynchPrediv = 127;
    hrtc.Init.SynchPrediv = 255;
    hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
    hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
    hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
    hrtc.MspInitCallback = stm32l4rtc_mspinit;
    if (HAL_RTC_Init(&hrtc) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }
    INIT_PRINT(INIT_OK,"stm32l4xx rtc init");
    if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0) != 0x1023)
    {
        sTime.Hours = 14;
        sTime.Minutes = 23;
        sTime.Seconds = 0;
        sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
        sTime.StoreOperation = RTC_STOREOPERATION_RESET;
        if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
        {
            Error_Handler(__FILE__, __LINE__);
        }

        sDate.WeekDay = RTC_WEEKDAY_MONDAY;
        sDate.Month = RTC_MONTH_NOVEMBER;
        sDate.Date = 23;
        sDate.Year = 17;

        if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
        {
            Error_Handler(__FILE__, __LINE__);
        }

        INIT_PRINT(INIT_OK,"stm32l4xx rtc set time&data");

        sAlarm.AlarmTime.Hours = 23;
        sAlarm.AlarmTime.Minutes = 30;
        sAlarm.AlarmTime.Seconds = 0;
        sAlarm.AlarmTime.SubSeconds = 0;
        sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
        sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
        sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
        sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
        sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
        sAlarm.AlarmDateWeekDay = 1;
        sAlarm.Alarm = RTC_ALARM_A;
        if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
        {
            Error_Handler(__FILE__, __LINE__);
        }
        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, 0x1023);
    }
    INIT_PRINT(INIT_OK,"stm32l4xx rtc set alarm");
    /**Enable the WakeUp
     */
    if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 0, RTC_WAKEUPCLOCK_RTCCLK_DIV16) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }
    return 0;
}

int stm32l4rtc_write(FIL_HAND *fd, const void *data, uint32_t count)
{
    RTC_DateTypeDef sDate;
    RTC_TimeTypeDef sTime;
    uint8_t *buf = (uint8_t *)data;

    if (count != 7)
    {
        return -ENOMEM;
    }
    sTime.Hours = buf[3];
    sTime.Minutes = buf[4];
    sTime.Seconds = buf[5];
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sTime.StoreOperation = RTC_STOREOPERATION_RESET;
    if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }

    sDate.WeekDay = buf[6];
    sDate.Month = buf[1];
    sDate.Date = buf[2];
    sDate.Year = buf[0];

    if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }

    return 0;
}

int stm32l4rtc_read(FIL_HAND *fd, void *buf, uint32_t count)
{
    RTC_DateTypeDef sTimeStampDateget;
    RTC_TimeTypeDef sTimeStampget;
    uint8_t *data = (uint8_t *)buf;

    if (count != 7)
    {
        return -ENOMEM;
    }
    /* Get the RTC current Time */
    HAL_RTC_GetTime(&hrtc, &sTimeStampget, RTC_FORMAT_BIN);
    /* Get the RTC current Date */
    HAL_RTC_GetDate(&hrtc, &sTimeStampDateget, RTC_FORMAT_BIN);

    data[0] = sTimeStampDateget.Year;
    data[1] = sTimeStampDateget.Month;
    data[2] = sTimeStampDateget.Date;
    data[3] = sTimeStampget.Hours;
    data[4] = sTimeStampget.Minutes;
    data[5] = sTimeStampget.Seconds;
    data[6] = sTimeStampDateget.WeekDay;
    return 0;
}

struct file_operations stm32l4rtc_fops =
{
    .onwer = 0,
    .open = stm32l4rtc_open,
    .write = stm32l4rtc_write,
    .read = stm32l4rtc_read,
};

struct miscdevice stm32l4_rtc =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = MODULE_NAME,
    .fops = &stm32l4rtc_fops,
    .nodename = "device",
};

int stm32l4rtc_init(void)
{
    return (misc_register(&stm32l4_rtc));
}

//device_init(stm32l4rtc_init);
