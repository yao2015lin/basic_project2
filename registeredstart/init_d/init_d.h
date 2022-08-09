#ifndef __INIT_D_H
#define __INIT_D_H




#include <stdbool.h>
#include "BAT32G157.h"
#include "unit.h"

//#pragma section ="fn1" 
//#pragma section ="fn2" 
//#pragma section ="fn3" 
//#pragma section ="fn4" 
//#pragma section ="fn5" 
//#pragma section ="fn6" 
//#pragma section ="fn7" 

#define FN1  ".ARM.__AT_0x080000e0"
#define FN2  ".ARM.__AT_0x080000f0"
#define FN3  ".ARM.__AT_0x08000240"
#define FN4  ".ARM.__AT_0x08000260"
#define FN5  ".ARM.__AT_0x08000340"
#define FN6  ".ARM.__AT_0x08000380"
#define FN7  ".ARM.__AT_0x080003C0"

//typedef int (*initcall_t)(void);

typedef struct  _initcall_t
{
	int (*fun)( void);
}initcall_t;

#define __define_initcall(fn,id)  \
    static initcall_t  __initcall_##fn \
    __attribute__((used)) __attribute__((section(FN##id))) = { fn }


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