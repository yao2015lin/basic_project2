#include "bat32_rtc.h"

#define MODULE_NAME "bat32_157_rtc"

int bat32rtc_open(FIL_HAND *fd)
{
  rtc_counter_value_t date_time;
  CLK_Osc_Setting(OSC_PORT, OSC_OSCILLATOR); /* SubOSC enable */
  RTC_Init(RTC_FSUB);
  RTC_Start();
  date_time.year  = 0x22;
  date_time.month = 0x08;
  date_time.week  = SATURDAY; 
  date_time.day   = 0x24;
  date_time.hour  = 0x23;
  date_time.min   = 0x30;
  date_time.sec   = 0x00;
  RTC_Set_CounterValue(&date_time);
  RTC_Set_ConstPeriodInterruptOn(ONESEC);
  return 0;
}

int bat32rtc_write(FIL_HAND *fd, const void *data, uint32_t count)
{

    rtc_counter_value_t date_time;
    uint8_t *buf = (uint8_t *)data;

    if (count != 7)
    {
        return -1;
    }
    date_time.hour = buf[3];
    date_time.min = buf[4];
    date_time.sec = buf[5];
    date_time.week = buf[6];
    date_time.month = buf[1];
    date_time.day = buf[2];
    date_time.year = buf[0];
    RTC_Set_CounterValue(&date_time);
    RTC_Set_ConstPeriodInterruptOn(ONESEC);
    return 0;
}

int bat32rtc_read(FIL_HAND *fd, void *buf, uint32_t count)
{
    uint8_t *data = (uint8_t *)buf;
    rtc_counter_value_t date_time;
    if (count != 7)
    {
        return -1;
    }
    RTC_Get_CounterValue(&date_time);
    data[0] = date_time.year;
    data[1] = date_time.month;
    data[2] = date_time.day;
    data[3] = date_time.hour;
    data[4] = date_time.min;
    data[5] = date_time.sec;
    data[6] = date_time.week;
    return 0;
}

struct file_operations bat32rtc_fops =
{
    .onwer = 0,
    .open = bat32rtc_open,
    .write = bat32rtc_write,
    .read = bat32rtc_read,
};

struct miscdevice bat32_rtc =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = MODULE_NAME,
    .fops = &bat32rtc_fops,
    .nodename = "device",
};

int bat32rtc_init(void)
{
    return (misc_register(&bat32_rtc));
}

device_init(bat32rtc_init);