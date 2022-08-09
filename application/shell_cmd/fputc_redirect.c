#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "BAT32G157.h"
#include "userdefine.h"
#include "sci.h"

//extern struct      com2;

int fputc(int ch, FILE *f)
{

    if(ch == '\n')
    {
        //HAL_UART_Transmit(&(com2.com_handle) , "\r" , 1 , 500);
        (void)UART0_Send((uint8_t)"\r");
    }
    
	//HAL_UART_Transmit(&(com2.com_handle) , (uint8_t *)&ch , 1 , 500);
    (void)UART0_Send(ch);
    
	return ch;
}

        