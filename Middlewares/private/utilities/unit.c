#include "unit.h"
#include "time.h"
#include "startup.h"

char printf_buffer[256];
uint32_t    sysDebugFlag = 0xFF;



unsigned char aiot_strcmp( unsigned char *pst , unsigned char *str , unsigned char len)
{
	if((len == 0)||(pst == NULL)||(str == NULL))
	{
		return FALSE;
	}
		
	while( len-- )
	{
		if(*str++ != *pst++ )
			return FALSE;
	}
	return TRUE;
}


void printf_time( void )
{
    time_t tim;
    
    getltime(&tim);
    
    printf("[%02d-%02d-%02d %02d:%02d:%02d]",tim.year, tim.mon, tim.day, tim.hour, tim.min, tim.sec);
      
}



void log_err( char* fmt, ...)
{
	va_list args;
	int fmt_result;
	char log_buf[512];

	memset(log_buf , 0x00 , 512);
	
	va_start(args, fmt);
	
	fmt_result = vsprintf(log_buf , fmt, args);
	if (( fmt_result > 0 ) && ( fmt_result < 512))
	{
#ifndef BOOTLOADER
		//err_log(DEBUG_MESSAGE , (uint8_t *)log_buf , fmt_result);
#endif
		printf("\033[0;31m");
        printf("[#]");
        printf_time();\
		printf("%.*s" , fmt_result , log_buf);
		printf("\033[0;39m");
	}
	
	va_end(args);
	 	
}

void log_arry(uint32_t level ,unsigned char *pst , unsigned char *arry , unsigned int leng)
{
    if(LOG_BIT_ON(sysDebugFlag,level))
    {
        if(level == ERR)
        printf("\033[0;31m");
        else if(level == WARN)
        printf("\033[0;32m");
        else if(level == INFO)
        printf("\033[0;33m");
        else if(level == DEBUG)
        printf("\033[0;37m");
        printf("[#]");
        printf_time();
        printf("%s :  [" , pst);
        for(unsigned int i = 0 ; i < leng ; i++)
        {
            printf("%02X " , arry[i]);
        }
        printf("]\r\n");
        printf("\033[0;39m");
    }
}



uint64_t atol64( char *str)
{
    uint64_t t[16]={0}; 
    uint8_t i=0 , cnt =0 , j =0;
    uint64_t temp = 0;
    
    while( *str != '\0')
    {
        t[cnt++] = str_to_hex(*str);
        str++;
        if( cnt > 16)
        {
            return 0;
        }
    }
    
    j = cnt -1;
    
    for( i = 0 ; i < cnt ; i++)
    {
        temp |= t[j--]<<(i*4);
    }
    
    return temp;
    
}

uint8_t  hex_to_char(uint8_t ucData)
{
	if(ucData < 10){
		return ucData+'0';
	}
	else{
		return ucData-10+'A';
	}
}

unsigned char str_to_hex(unsigned char data)
{
    if(data <= '9')
    {
        return data-'0';
    }
	else if((data >= 'A')&&(data <= 'F'))
    {
        return data+10-'A';
    }
    else
    {
        return data+10-'a';
    }
}


unsigned char str_to_int(unsigned char dData)
{
	if(dData <= '9')
          return dData-'0';
	else if((dData >= 'A')&&(dData <= 'F'))
          return dData+10-'A';
        else
          return dData+10-'a';
}

int string_to_hex( char *data , int length , char *respone)
{

	if(length%2 != 0)	return -1;

	for( int i = 0 ; i < length/2 ;i++)
	{
		*respone++ = (((str_to_int(data[i*2])<<4)&0xf0)|(str_to_int(data[i*2+1])&0x0f)); 
	}

	return 0;
	
}


void sys_delay(uint32_t ms)
{
    osDelay(ms);
}


void read_task_stack(char const *name , xTaskHandle taskHandle)
{
    unsigned portBASE_TYPE uxHighWaterMark; 
    
    uxHighWaterMark = uxTaskGetStackHighWaterMark(taskHandle);

    log(WARN,"[%s]stack surplus = %d.\r\n" ,name,uxHighWaterMark);
    
}


uint8_t sysclock_is_pll( void )
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    uint32_t FLatency;
    
    HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &FLatency);

    if( RCC_ClkInitStruct.SYSCLKSource != RCC_SYSCLKSOURCE_PLLCLK)
    {
        return FALSE;
    }
    
    return TRUE;

}


void sysclock_resurm( void )
{
    if( sysclock_is_pll() == FALSE )
    {
        HAL_Sysclk_Init();
    }
}

void soft_system_resert( const char *funs )
{
    log(WARN,"System has been resert form %s\n" , funs);
    NVIC_SystemReset();
}




