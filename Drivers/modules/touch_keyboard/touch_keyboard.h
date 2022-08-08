#ifndef __TOUCH_KEYBOARD_H
#define __TOUCH_KEYBOARD_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include "BAT32G157.h"
#include "major.h"
#include "unit.h"
#include "init_d.h"
#include "miscdevice.h"
#include "priv_os.h"
#include "startup.h"


typedef enum
{
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_DEL,    // *
    KEY_ENT,    // #
}keyvalue_enum;
      
#ifdef __cplusplus
}
#endif

#endif /* __TOUCH_KEYBOARD_H */
