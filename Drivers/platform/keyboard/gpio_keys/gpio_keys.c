#include "gpio_keys.h"

static sc_semaphore_t gpio_keys_mutex;


#define  send_semap( ) \
    do                                                 \
    {                                                  \
        if (gpio_keys_mutex != NULL)                   \
        {                                              \
            send_semaphore(gpio_keys_mutex);           \
        }                                              \
    }while(0)

void EXTI0_IRQHandler(void)
{
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
        send_semap();
        //log(DEBUG, " COL0 , V=%d\n", HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0));
    }
}
void EXTI1_IRQHandler(void)
{
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_1) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
        send_semap();
        //log(DEBUG, " COL1 , V=%d\n", HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1));
    }
}
void EXTI2_IRQHandler(void)
{
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_2) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_2);
        send_semap();
        //log(DEBUG, " COL2 , V=%d\n", HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2));
    }
}
void EXTI3_IRQHandler(void)
{
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_3) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);
        send_semap();
        //log(DEBUG, " COL3 , V=%d\n", HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3));
    }
}
void EXTI4_IRQHandler(void)
{
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_4) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_4);
        send_semap();
        //log(DEBUG, " COL4 , V=%d\n", HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4));
    }
}
void EXTI9_5_IRQHandler(void)
{
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_5) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5);
        send_semap();
        //log(DEBUG, " COL5 , V=%d\n", HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5));
    }
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_6) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_6);
        send_semap();
        //log(DEBUG, " COL6 , V=%d\n", HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6));
    }
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_7) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_7);
        send_semap();
        //log(DEBUG, " COL7 , V=%d\n", HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7));
    }
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_8) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_8);
        send_semap();
        //log(DEBUG, " COL8 , V=%d\n", HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8));
    }
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_9) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_9);
        send_semap();
        //log(DEBUG, " COL9 , V=%d\n", HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9));
    }
}

void EXTI15_10_IRQHandler(void)
{

    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_10) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_10);
        send_semap();
        //log(DEBUG, " COL10 , V=%d\n", HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10));
    }
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_11) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_11);
        send_semap();
        //log(DEBUG, " COL11 , V=%d\n", HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11));
    }
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_12) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_12);
        send_semap();
        //log(DEBUG, " COL12 , V=%d\n", HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12));
    }
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_13) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);
        send_semap();
        //log(DEBUG, " COL13 , V=%d\n", HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13));
    }
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_14) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_14);
        send_semap();
        //log(DEBUG, " COL14 , V=%d\n", HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14));
    }
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_15) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_15);
        send_semap();
        //log(DEBUG, " COL15 , V=%d\n", HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15));
    }

}


struct gpio_keys
{
    GPIO_TypeDef  *gpiox;
    uint16_t pin;
};

static struct gpio_keys row[]=
{
    {GPIOC, GPIO_PIN_11},
    {GPIOC, GPIO_PIN_10},
    {GPIOC, GPIO_PIN_6},
    {GPIOC, GPIO_PIN_0},
    {GPIOC, GPIO_PIN_1},
    {GPIOC, GPIO_PIN_2},
};

const uint8_t row_size = sizeof (row) / sizeof (row[0]);

static struct gpio_keys col[]=
{
    {GPIOB, GPIO_PIN_0},
    {GPIOB, GPIO_PIN_1},
    {GPIOB, GPIO_PIN_2},
    {GPIOB, GPIO_PIN_3},
    {GPIOB, GPIO_PIN_4},
    {GPIOB, GPIO_PIN_5},
    {GPIOB, GPIO_PIN_6},
    {GPIOB, GPIO_PIN_7},
    {GPIOB, GPIO_PIN_8},
    {GPIOB, GPIO_PIN_9},
    {GPIOB, GPIO_PIN_10},
    {GPIOB, GPIO_PIN_11},
    {GPIOB, GPIO_PIN_12},
    {GPIOB, GPIO_PIN_13},
    {GPIOB, GPIO_PIN_14},
    {GPIOB, GPIO_PIN_15},
};

const uint8_t col_size = sizeof (col) / sizeof (col[0]);


void enable_irq( void )
{
    uint8_t i = 0;
    GPIO_InitTypeDef GPIO_InitStruct;


    for( i = 0 ; i < col_size ; i++ )
    {
        GPIO_InitStruct.Pin = col[i].pin;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
        HAL_GPIO_Init(col[i].gpiox, &GPIO_InitStruct);

        __HAL_GPIO_EXTI_CLEAR_IT(col[i].pin);
    }

    for( i = 0 ; i < row_size ; i++ )
    {
        HAL_GPIO_WritePin(row[i].gpiox, row[i].pin, GPIO_PIN_SET);
    }

    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}


void disable_irq (void)
{
    uint8_t i = 0;
    GPIO_InitTypeDef GPIO_InitStruct;

    for( i = 0 ; i < col_size ; i++ )
    {
        GPIO_InitStruct.Pin = col[i].pin;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        HAL_GPIO_Init(col[i].gpiox, &GPIO_InitStruct);
    }

    HAL_NVIC_DisableIRQ(EXTI0_IRQn);
    HAL_NVIC_DisableIRQ(EXTI1_IRQn);
    HAL_NVIC_DisableIRQ(EXTI2_IRQn);
    HAL_NVIC_DisableIRQ(EXTI3_IRQn);
    HAL_NVIC_DisableIRQ(EXTI4_IRQn);
    HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
    HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);

}
int gpio_keys_open(FIL_HAND *fd)
{
    uint8_t i = 0;
    GPIO_InitTypeDef GPIO_InitStruct;
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    for( i = 0 ; i < col_size ; i++ )
    {
        GPIO_InitStruct.Pin = col[i].pin;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
        HAL_GPIO_Init(col[i].gpiox, &GPIO_InitStruct);
    }

    for( i = 0 ; i < row_size ; i++ )
    {
        GPIO_InitStruct.Pin = row[i].pin;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        HAL_GPIO_Init(row[i].gpiox, &GPIO_InitStruct);
    }

    for( i = 0 ; i < row_size ; i++ )
    {
        HAL_GPIO_WritePin(row[i].gpiox, row[i].pin, GPIO_PIN_SET);
    }

    /* Enable and set Button EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority(EXTI0_IRQn, 5, 0x00);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    HAL_NVIC_SetPriority(EXTI1_IRQn, 5, 0x00);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);

    HAL_NVIC_SetPriority(EXTI2_IRQn, 5, 0x00);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);

    HAL_NVIC_SetPriority(EXTI3_IRQn, 5, 0x00);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);

    HAL_NVIC_SetPriority(EXTI4_IRQn, 5, 0x00);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0x00);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0x00);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

    gpio_keys_mutex = creat_semqphore();
    INIT_PRINT((gpio_keys_mutex == NULL) ? INIT_FAIL : INIT_OK, "serial debug semqphore creat");


    return 0;
}

int gpio_keys_write(FIL_HAND *fd, const void *buf, uint32_t count)
{


    return 0;
}

uint8_t mode = 0;
uint8_t key_temp[128];

int gpio_keys_read(FIL_HAND *fd, void *data, uint32_t count)
{
    uint8_t i = 0, j = 0, m = 0;
    uint32_t val = 0;
    uint32_t sn = 0;
    uint8_t *keybuf= (uint8_t *)data;
 
    if( mode == 0 )
    {
        read_semaphore(gpio_keys_mutex, WAIT_FOR_EVER);
    }
    if( count > sizeof(key_temp))
    {
        log(ERR , " read length > fifo\n");
        return 0;
    }
    mode = 1;
    disable_irq();

    //log(DEBUG,"read value\n");
    do
    {   
        sn = 0;
        memset(keybuf , 0 , count);
        for( i = 0 ; i < row_size ; i++ )
        {
            for( m = 0 ; m < row_size ; m++ )
            {
                HAL_GPIO_WritePin(row[m].gpiox, row[m].pin, GPIO_PIN_RESET);
            }
            HAL_GPIO_WritePin(row[i].gpiox, row[i].pin, GPIO_PIN_SET);

            for( j = 0 ; j < col_size ; j++ )
            {
                if( HAL_GPIO_ReadPin( col[j].gpiox, col[j].pin) == 1)
                {

                    //log(DEBUG," i = %d , j=%d\n", i, j);
                    val = ((i+1)<<4)|(j+1);
                    keybuf[sn++] = val;
                    if(sn > count)
                    {
                        log(ERR,"read leng is to big\n");
                        while(1);   //最大按键数量大于缓存，错误死循环
                    }
                }
            }
        }
        //如果按键键值改变，则返回按键键值
        if( strncmp(( char *)key_temp , ( char *)keybuf , count) !=0 )
        {
            memcpy( key_temp , keybuf , count );
            goto back_to_home;
        }
         //如果键值不变，则无新按键按下，polling go on
        else   
        {
            sys_delay(30);
        }
        
    } while (1);

back_to_home:
    if( sn == 0 )
    {
        mode = 0;
        enable_irq();
    }
    //log_arry(DEBUG,"READ" , keybuf , sn);
    return sn;

}

struct file_operations gpio_keys_fops =
{
    .onwer = 0,
    .open = gpio_keys_open,
    .write = gpio_keys_write,
    .read = gpio_keys_read,
};

struct miscdevice gpio_keys_device =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = "gpio-keys",
    .fops = &gpio_keys_fops,
    .nodename = "device",
};

int gpio_keys_init(void)
{
    return (misc_register(&gpio_keys_device));
}

device_init(gpio_keys_init);