#ifndef __INIT_D_H
#define __INIT_D_H




#include <stdbool.h>
#include "BAT32G157.h"
#include "unit.h"
#if 0
#pragma arm section code ="fn1" 
#pragma arm section code ="fn2" 
#pragma arm section code ="fn3" 
#pragma arm section code ="fn4" 
#pragma arm section code ="fn5" 
#pragma arm section code ="fn6" 
#pragma arm section code ="fn7" 
#endif
#define FN10  "fn1.start"
#define FN1   "fn1"
#define FN11  "fn1.end"
#define FN20  "fn2.start"
#define FN2  "fn2"
#define FN21  "fn2.end"
#define FN30  "fn3.begain"
#define FN3   "fn3"
#define FN31  "fn3.end"
#define FN4  "fn4"
#define FN41  "fn4.end"
#define FN5   "fn5"
#define FN51  "fn5.end"
#define FN6   "fn6"
#define FN61  "fn6.end"
#define FN7   "fn7"
#define FN71  "fn7.end"

//typedef int (*initcall_t)(void);

typedef struct  _initcall_t
{
	int (*fun)( void);
}initcall_t;


#define __define_initcall(fn,id)  \
    static initcall_t  __initcall_##fn \
    __attribute__((__used__)) __attribute__((section(FN##id))) = { fn }


#define arch_init(fn)       __define_initcall( fn , 1)
#define device_init(fn)     __define_initcall( fn , 2)
#define modules_init(fn)     __define_initcall( fn , 3)
#define comp_init(fn)       __define_initcall( fn , 4)
#define task_init(fn)       __define_initcall( fn , 5)
#define resume_register(fn)     __define_initcall( fn , 6)
#define suspend_register(fn)    __define_initcall( fn , 7)

#define INIT_OK         0
#define INIT_FAIL       (!INIT_OK)
    
extern void driver_init(void);
extern void task_start( void );   
extern void task_resume( void );
extern void task_suspend( void ); 
 

#endif /* __DRIVER_REGISTER_H */
		