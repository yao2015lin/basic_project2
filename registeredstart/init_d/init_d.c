#include "init_d.h"

#define FN1  ".ARM.__AT_0x080000e0"
#define FN2  ".ARM.__AT_0x080000f0"
#define FN3  ".ARM.__AT_0x08000240"
#define FN4  ".ARM.__AT_0x08000260"
#define FN5  ".ARM.__AT_0x08000340"
#define FN6  ".ARM.__AT_0x08000380"
#define FN7  ".ARM.__AT_0x080003C0"

#define  $$BaseSectionfn1   ".ARM.__AT_0x080000e0"
#define  $$LimitSectionfn1  ".ARM.__AT_0x080000f0"

#define  $$BaseSectionfn2  ".ARM.__AT_0x080000f0"
#define  $$LimitSectionfn2  ".ARM.__AT_0x08000240"

#define  $$BaseSectionfn3   ".ARM.__AT_0x08000240"
#define  $$LimitSectionfn3  ".ARM.__AT_0x08000260"

#define $$BaseSectionfn4   ".ARM.__AT_0x08000260"
#define  $$LimitSectionfn4  ".ARM.__AT_0x08000340"

#define  $$BaseSectionfn5  ".ARM.__AT_0x08000340"
#define  $$LimitSectionfn5 ".ARM.__AT_0x08000380"

#define  $$BaseSectionfn6  ".ARM.__AT_0x08000380"
#define  $$LimitSectionfn6 ".ARM.__AT_0x080003C0"

#define $$BaseSectionfn7  ".ARM.__AT_0x080003C0"
#define  $$LimitSectionfn7 ".ARM.__AT_0x08000400"

static initcall_t *fn1_start __attribute__((used)) __attribute__((section($$BaseSectionfn1)));
static initcall_t *fn1_end  __attribute__((used)) __attribute__((section($$LimitSectionfn1)));

static initcall_t *fn2_start __attribute__((used)) __attribute__((section($$BaseSectionfn2)));
static initcall_t *fn2_end __attribute__((used)) __attribute__((section($$LimitSectionfn2)));

static initcall_t *fn3_start __attribute__((used)) __attribute__((section($$BaseSectionfn3)));
static initcall_t *fn3_end __attribute__((used)) __attribute__((section($$LimitSectionfn3)));

static initcall_t *fn4_start __attribute__((used)) __attribute__((section($$BaseSectionfn4)));
static initcall_t *fn4_end __attribute__((used)) __attribute__((section($$LimitSectionfn4)));

static initcall_t *fn5_start __attribute__((used)) __attribute__((section($$BaseSectionfn5)));
static initcall_t *fn5_end __attribute__((used)) __attribute__((section($$LimitSectionfn5)));

static initcall_t *fn6_start __attribute__((used)) __attribute__((section($$BaseSectionfn6)));
static initcall_t *fn6_end __attribute__((used)) __attribute__((section($$LimitSectionfn6)));

static initcall_t *fn7_start __attribute__((used)) __attribute__((section($$BaseSectionfn7)));
static initcall_t *fn7_end __attribute__((used)) __attribute__((section($$LimitSectionfn7)));

void driver_init(void)
{
    initcall_t *tp;

    for (tp = fn1_start; tp < fn1_end; tp++)
    {
        if (tp != NULL)
        {
            tp->fun();
        }
        else
        {
            log(ERR, "fn init is error , start=%x , end = %x\n", fn1_start, fn1_end);
        }
    }

    for (tp = fn2_start; tp < fn2_end; tp++)
    {
        if (tp != NULL)
        {
            tp->fun();
        }
        else
        {
            log(ERR, "fn init is error , start=%x , end = %x\n", fn2_start, fn2_end);
        }
    }

    for (tp = fn3_start; tp < fn3_end; tp++)
    {
        if (tp != NULL)
        {
            tp->fun();
        }
        else
        {
            log(ERR, "fn init is error , start=%x , end = %x\n", fn3_start, fn3_end);
        }
    }

    for (tp = fn4_start; tp < fn4_end; tp++)
    {
        if (tp != NULL)
        {
            tp->fun();
        }
        else
        {
            log(ERR, "fn init is error , start=%x , end = %x\n", fn4_start, fn4_end);
        }
    }

    
}


void task_start( void )
{
    initcall_t *tp;

    for (tp = fn5_start; tp < fn5_end; tp++)
    {
        if (tp != NULL)
        {
            tp->fun();
        }
        else
        {
            log(ERR, "fn init is error , start=%x , end = %x\n", fn5_start, fn5_end);
        }
    }
}

void task_resume( void )
{
initcall_t *tp;

    for (tp = fn6_start; tp < fn6_end; tp++)
    {
        if (tp != NULL)
        {
            tp->fun();
        }
        else
        {
            log(ERR, "fn init is error , start=%x , end = %x\n", fn6_start, fn6_end);
        }
    }    
}

void task_suspend( void )
{
    initcall_t *tp;

    for (tp = fn7_start; tp < fn7_end; tp++)
    {
        if (tp != NULL)
        {
            tp->fun();
        }
        else
        {
            log(ERR, "fn init is error , start=%x , end = %x\n", fn7_start, fn7_end);
        }
    }    
}