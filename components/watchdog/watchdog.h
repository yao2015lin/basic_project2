#ifndef __WATCHDOG_H
#define __WATCHDOG_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "cmsis_os.h"
#include "unit.h"
#include "miscdevice.h"
#include "priv_list.h"
#include "init_d.h"


extern void feed_dog( void );

#ifdef __cplusplus
}
#endif

#endif /* __WATCHDOG_H */
