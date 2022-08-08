#ifndef __UINT_H
#define __UINT_H


#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "errno.h"
#include "BAT32G157.h"
#include "cmsis_os.h"


#ifndef NULL
#define NULL				(void*)(0)
#endif

#ifndef FALSE
#define FALSE				0
#endif
#ifndef TRUE
#define TRUE				!FALSE
#endif

#ifndef BOOL
#define BOOL				bool
#endif
#ifndef false
#define false				FALSE
#endif
#ifndef true
#define true				TRUE
#endif

#define BIT0		(0x00000001ul)
#define BIT1		(0x00000002ul)
#define BIT2		(0x00000004ul)
#define BIT3		(0x00000008ul)
#define BIT4		(0x00000010ul)
#define BIT5		(0x00000020ul)
#define BIT6		(0x00000040ul)
#define BIT7		(0x00000080ul)
#define BIT8		(0x00000100ul)
#define BIT9		(0x00000200ul)
#define BIT10	(0x00000400ul)
#define BIT11	(0x00000800ul)
#define BIT12	(0x00001000ul)
#define BIT13	(0x00002000ul)
#define BIT14	(0x00004000ul)
#define BIT15	(0x00008000ul)
#define BIT16	(0x00010000ul)
#define BIT17	(0x00020000ul)
#define BIT18	(0x00040000ul)
#define BIT19	(0x00080000ul)
#define BIT20	(0x00100000ul)
#define BIT21	(0x00200000ul)
#define BIT22	(0x00400000ul)
#define BIT23	(0x00800000ul)
#define BIT24	(0x01000000ul)
#define BIT25	(0x02000000ul)
#define BIT26	(0x04000000ul)
#define BIT27	(0x08000000ul)
#define BIT28	(0x10000000ul)
#define BIT29	(0x20000000ul)
#define BIT30	(0x40000000ul)
#define BIT31	(0x80000000ul)

#ifndef U8_MAX
#define U8_MAX     (255)
#endif
#ifndef S8_MAX
#define S8_MAX     (127)
#endif
#ifndef S8_MIN
#define S8_MIN     (-128)
#endif
#ifndef U16_MAX
#define U16_MAX    (65535u)
#endif
#ifndef S16_MAX
#define S16_MAX    (32767)
#endif
#ifndef S16_MIN
#define S16_MIN    (-32768)
#endif
#ifndef U32_MAX
#define U32_MAX    (4294967295uL)
#endif
#ifndef S32_MAX
#define S32_MAX    (2147483647)
#endif
#ifndef S32_MIN
#define S32_MIN    (-2147483648uL)
#endif
/**
  * @brief  This macro returns the maximum value between two inputs,
  *                    the evaluation is done with the ">" operator
  * @param  a  First input
  * @param  b  Second input  
  * @retval Max(a,b)
  */
#ifndef MAX
#define MAX(a,b)        ((((uint32_t)(a)) > ((uint32_t)(b)))  ? (a)  : (b))
#endif
/**
  * @brief  This macro returns the minumum value between two inputs,
  *                     the evaluation is done with the ">" operator
  * @param  a  First input
  * @param  b  Second input  
  * @retval Min(a,b)
  */
#ifndef MIN
#define MIN(a,b)        ((((uint32_t)(a)) > ((uint32_t)(b)))  ? (b)  : (a))
#endif

#define task_assert( x )    configASSERT(((unsigned long)x) == 0)


extern uint32_t    sysDebugFlag;
                                      

#define ERR                 1
#define WARN                2
#define INFO                3
#define DEBUG               4

#define LOG_PRESENT_BIT(x)        (((uint32_t)((uint32_t)1<<(x))))
#define LOG_BIT_ON(m, b)          (((m) & LOG_PRESENT_BIT(b)) != 0)
#define LOG_SET_BIT(m, b)         ((m) |= LOG_PRESENT_BIT(b))
#define LOG_CLEAR_BIT(m, b)       ((m) &= ~LOG_PRESENT_BIT(b))

void printf_time( void );

void log_err( char* fmt, ...);

#define log(level, fmt, args...)  do{\
    if(LOG_BIT_ON(sysDebugFlag,level)){\
		if(level == ERR){\
		printf("\033[0;31m");}\
        else if(level == WARN)\
        printf("\033[0;32m");\
        else if(level == INFO)\
        printf("\033[0;33m");\
        else if(level == DEBUG)\
        printf("\033[0;37m");\
        printf("[#]");\
        printf_time();\
        printf(fmt, ##args);\
        printf("\033[0;39m");\
    }\
}while(0)


#define PRINTK(level, fmt, args...)  do{\
    if(LOG_BIT_ON(sysDebugFlag,level)){\
		if(level == ERR){\
		printf("\033[0;31m");}\
        else if(level == WARN)\
        printf("\033[0;32m");\
        else if(level == INFO)\
        printf("\033[0;33m");\
        else if(level == DEBUG)\
        printf("\033[0;37m");\
        printf(fmt, ##args);\
        printf("\033[0;39m");\
    }\
}while(0)

extern char printf_buffer[256];

#define INIT_PRINT(level, fmt, args...)  do{\
    if(LOG_BIT_ON(sysDebugFlag,level)){\
        sprintf(printf_buffer,fmt, ##args);\
        printf("%-64s",printf_buffer);\
        if(level != 0){\
        printf("\033[0;31m");\
        printf("[ FAIL ]\n");}\
        else{\
        printf("\033[0;32m");\
        printf("[  OK  ]\n");}\
        printf("\033[0;39m");\
    }\
}while(0)



void read_task_stack( char const *name , xTaskHandle taskHandle);

void sys_delay(uint32_t ms);

int string_to_hex( char *data , int length , char *respone);

unsigned char str_to_int(unsigned char dData);

unsigned char str_to_hex(unsigned char data);

uint8_t  hex_to_char(uint8_t ucData);

uint64_t atol64( char *str);

void log_arry(uint32_t level ,unsigned char *pst , unsigned char *arry , unsigned int leng);

unsigned char aiot_strcmp( unsigned char *pst , unsigned char *str , unsigned char len);

void soft_system_resert( const char *funs );

extern uint8_t sysclock_is_pll( void );
extern void sysclock_resurm( void );

#endif
