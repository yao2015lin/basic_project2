#include <stdio.h>
#include <string.h>
#include "BAT32G157.h"
#include "sci.h"
#include "serialx.h"

extern struct serial_dev    com2;

int fputc(int ch, FILE *f)
{

    if(ch == '\n')
    {
        HAL_UART_Transmit(&(com2.com_handle) , "\r" , 1 , 500);
    }
    
	HAL_UART_Transmit(&(com2.com_handle) , (uint8_t *)&ch , 1 , 500);
    
	return ch;
}

        