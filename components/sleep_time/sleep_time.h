#ifndef __TIME_SLEEP_H
#define __TIME_SLEEP_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "cmsis_os.h"
#include "unit.h"
#include "miscdevice.h"
#include "priv_list.h"
#include "init_d.h"

extern void stop_time_sleep(void);
extern void set_time_sleep(uint32_t sleep_tims);
extern uint32_t get_time_count( void );

#ifdef __cplusplus
}
#endif

#endif /* __TIME_SLEEP_H */
