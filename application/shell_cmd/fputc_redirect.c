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
        (void)UART0_Send((uint8_t)"\r");
    }
    
    (void)UART0_Send(ch);
    
	return ch;
}

        