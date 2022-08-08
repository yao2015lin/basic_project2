#ifndef __TIME_H
#define __TIME_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "cmsis_os.h"
#include "unit.h"
#include "miscdevice.h"
#include "priv_list.h"
#include "init_d.h"

typedef struct
{
    uint8_t year;
    uint8_t mon;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    uint8_t week;
}time_t;


extern void setltime(time_t *tm);
extern void getltime(time_t *tm);

#ifdef __cplusplus
}
#endif

#endif /* __TIME_H */
