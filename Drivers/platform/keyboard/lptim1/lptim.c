#include "lptim.h"
#include "pwr.h"


LPTIM_HandleTypeDef hlptim1;


#define MODULE_NAME "lptim1"


void LPTIM1_IRQHandler(void)
{
    HAL_LPTIM_IRQHandler(&hlptim1);

    pwr_set_wakeup_sur(WAKEUP_FLAG_SLEEP_TIME);
}

void HAL_LPTIM1_MspInit(LPTIM_HandleTypeDef* hlptim)
{
    __HAL_RCC_LPTIM1_CLK_ENABLE();

    HAL_NVIC_SetPriority(LPTIM1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(LPTIM1_IRQn);
}

int lptim_hal_init(void)
{
    hlptim1.Instance = LPTIM1;
    hlptim1.Init.Clock.Source = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
    hlptim1.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV32;
    hlptim1.Init.Trigger.Source = LPTIM_TRIGSOURCE_SOFTWARE;
    hlptim1.Init.OutputPolarity = LPTIM_OUTPUTPOLARITY_HIGH;
    hlptim1.Init.UpdateMode = LPTIM_UPDATE_IMMEDIATE;
    hlptim1.Init.CounterSource = LPTIM_COUNTERSOURCE_INTERNAL;
    hlptim1.Init.Input1Source = LPTIM_INPUT1SOURCE_GPIO;
    hlptim1.Init.Input2Source = LPTIM_INPUT2SOURCE_GPIO;
    hlptim1.MspInitCallback = HAL_LPTIM1_MspInit;
    if (HAL_LPTIM_Init(&hlptim1) != HAL_OK)
    {
        Error_Handler(__FILE__,__LINE__);
    }
    
    return 0;
}

int lptim1_open(FIL_HAND *fd)
{
    lptim_hal_init();
    INIT_PRINT(INIT_OK,"LPTIM1 init");
    return 0;
}


int lptim1_write(FIL_HAND *fd, const void *buf, uint32_t count)
{
    uint32_t sleep_ms = *(uint32_t *)buf;
    uint32_t cnt = 0;
    
    cnt = (sleep_ms*1000*32)/32768;
    
    //log(DEBUG,"sleep time=%u , cnt = %u\n" , sleep_ms , cnt);

    if (HAL_LPTIM_TimeOut_Start_IT(&hlptim1, 0, cnt) != HAL_OK)
    {
        Error_Handler(__FILE__,__LINE__);
    }

    return 0 ;
}

int lptim1_read(FIL_HAND *fd, void *data, uint32_t count)
{
    uint32_t c1 , c2;
    
    do{
        c1 = HAL_LPTIM_ReadCounter(&hlptim1); 
        c2 = HAL_LPTIM_ReadCounter(&hlptim1);
    }while(c1!=c2);
    
    *(uint32_t *)data = c1;
    
    //log(DEBUG,"read cnt =%d\n" , c1);

    return 0;
}

int lptim1_ioctl(FIL_HAND *fd, void *buf, uint32_t count, uint32_t flag)
{
    if (flag & IOCTL_FLAG_SET_STOP)
    {
        HAL_LPTIM_TimeOut_Stop_IT(&hlptim1);
    }
    return 0;
}


struct file_operations lptim1_fops =
{
    .onwer = 0,
    .open = lptim1_open,
    .write = lptim1_write,
    .read = lptim1_read,
    .ioctl = lptim1_ioctl,
};

struct miscdevice lptim1_device =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = MODULE_NAME,
    .fops = &lptim1_fops,
    .nodename = "device",
};

int lptim1_init(void)
{
    return (misc_register(&lptim1_device));
}

device_init(lptim1_init);


