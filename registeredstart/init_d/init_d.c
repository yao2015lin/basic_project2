#include "init_d.h"

#if 0
#define FN1  ".ARM.__AT_0x080000e0"
#define FN2  ".ARM.__AT_0x080000f0"
#define FN3  ".ARM.__AT_0x08000240"
#define FN4  ".ARM.__AT_0x08000260"
#define FN5  ".ARM.__AT_0x08000340"
#define FN6  ".ARM.__AT_0x08000380"
#define FN7  ".ARM.__AT_0x080003C0"


#define  $$BaseSectionfn1   "0x080000e0"
#define  $$LimitSectionfn1  "0x080000ec"

#define  $$BaseSectionfn2  "0x080000f0"
#define  $$LimitSectionfn2  "0x0800023c"

#define  $$BaseSectionfn3   "0x08000240"
#define  $$LimitSectionfn3  "0x0800025c"

#define $$BaseSectionfn4   "0x08000260"
#define  $$LimitSectionfn4  "0x0800033c"

#define  $$BaseSectionfn5  "0x08000340"
#define  $$LimitSectionfn5 "0x0800037c"

#define  $$BaseSectionfn6  "0x08000380"
#define  $$LimitSectionfn6 "0x080003bc"

#define $$BaseSectionfn7  "0x080003C0"
#define  $$LimitSectionfn7 "0x080003fc"

static initcall_t *fn1_start __attribute__((__used__)) __attribute__((section($$BaseSectionfn1)));
static initcall_t *fn1_end  __attribute__((__used__)) __attribute__((section($$LimitSectionfn1)));

static initcall_t *fn2_start __attribute__((__used__)) __attribute__((section($$BaseSectionfn2)));
static initcall_t *fn2_end __attribute__((__used__)) __attribute__((section($$LimitSectionfn2)));

static initcall_t *fn3_start __attribute__((__used__)) __attribute__((section($$BaseSectionfn3)));
static initcall_t *fn3_end __attribute__((__used__)) __attribute__((section($$LimitSectionfn3)));

static initcall_t *fn4_start __attribute__((__used__)) __attribute__((section($$BaseSectionfn4)));
static initcall_t *fn4_end __attribute__((__used__)) __attribute__((section($$LimitSectionfn4)));

static initcall_t *fn5_start __attribute__((__used__)) __attribute__((section($$BaseSectionfn5)));
static initcall_t *fn5_end __attribute__((__used__)) __attribute__((section($$LimitSectionfn5)));

static initcall_t *fn6_start __attribute__((__used__)) __attribute__((section($$BaseSectionfn6)));
static initcall_t *fn6_end __attribute__((__used__)) __attribute__((section($$LimitSectionfn6)));

static initcall_t *fn7_start __attribute__((__used__)) __attribute__((section($$BaseSectionfn7)));
static initcall_t *fn7_end __attribute__((__used__)) __attribute__((section($$LimitSectionfn7)));
#endif
static int arch_init_start( void)
{
    return 0;
}
arch_init(arch_init_start);
static int arch_init_end( void)
{
    return 0;
}
__define_initcall(arch_init_end, 11);

static int device_init_start( void)
{
    return 0;
}
device_init(device_init_start);
int device_init_end( void)
{
    return 0;
}
__define_initcall(device_init_end, 21);

static int modules_init_start( void)
{
    return 0;
}
modules_init(modules_init_start);
int modules_init_end( void)
{
    return 0;
}
__define_initcall(modules_init_end, 31);

static int comp_init_start( void)
{
    return 0;
}
comp_init(comp_init_start);
int comp_init_end( void)
{
    return 0;
}
__define_initcall(comp_init_end, 41);

void driver_init(void)
{
    initcall_t *tp;

    for (tp = &__initcall_arch_init_start; tp < &__initcall_arch_init_end; tp++)
    {
        if (tp != NULL)
        {
            tp->fun();
        }
        else
        {
            log(ERR, "fn init is error , start=%x , end = %x\n", __initcall_arch_init_start, __initcall_arch_init_end);
        }
    }

    for (tp = &__initcall_device_init_start; tp < &__initcall_device_init_end; tp++)
    {
        if (tp != NULL)
        {
            tp->fun();
        }
        else
        {
            log(ERR, "fn init is error , start=%x , end = %x\n", __initcall_device_init_start, __initcall_device_init_end);
        }
    }

    for (tp = &__initcall_modules_init_start; tp < &__initcall_modules_init_end; tp++)
    {
        if (tp != NULL)
        {
            tp->fun();
        }
        else
        {
            log(ERR, "fn init is error , start=%x , end = %x\n", __initcall_modules_init_start, __initcall_modules_init_end);
        }
    }

    for (tp = &__initcall_comp_init_start; tp < &__initcall_comp_init_end; tp++)
    {
        if (tp != NULL)
        {
            tp->fun();
        }
        else
        {
            log(ERR, "fn init is error , start=%x , end = %x\n", __initcall_comp_init_start, __initcall_comp_init_end);
        }
    }

    
}

static int task_init_start( void)
{
    return 0;
}
task_init(task_init_start);
int task_init_end( void)
{
    return 0;
}
__define_initcall(task_init_end, 51);

void task_start( void )
{
    initcall_t *tp;

    for (tp = &__initcall_task_init_start; tp < &__initcall_task_init_end; tp++)
    {
        if (tp != NULL)
        {
            tp->fun();
        }
        else
        {
            log(ERR, "fn init is error , start=%x , end = %x\n", __initcall_task_init_start, __initcall_task_init_end);
        }
    }
}

static int resume_register_start( void)
{
    return 0;
}
resume_register(resume_register_start);
int resume_register_end( void)
{
    return 0;
}
__define_initcall(resume_register_end, 61);

void task_resume( void )
{
    initcall_t *tp;
    for (tp = &__initcall_resume_register_start; tp < &__initcall_resume_register_end; tp++)
    {
        if (tp != NULL)
        {
            tp->fun();
        }
        else
        {
            log(ERR, "fn init is error , start=%x , end = %x\n", __initcall_resume_register_start, __initcall_resume_register_end);
        }
    }    
}
static int suspend_register_start( void)
{
    return 0;
}
suspend_register(suspend_register_start);
int suspend_register_end( void)
{
    return 0;
}
__define_initcall(suspend_register_end, 71);
void task_suspend( void )
{
    initcall_t *tp;

    for (tp = &__initcall_suspend_register_start; tp < &__initcall_suspend_register_end; tp++)
    {
        if (tp != NULL)
        {
            tp->fun();
        }
        else
        {
            log(ERR, "fn init is error , start=%x , end = %x\n", __initcall_suspend_register_start, __initcall_suspend_register_end);
        }
    }    
}
