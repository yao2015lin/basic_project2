#ifndef __SERIAL0_H
#define __SERIAL0_H

#ifdef __cplusplus
extern "C"
{
#endif

  /**
  * @}
  */
#include <stdbool.h>
#include "BAT32G157.h"
#include "major.h"
#include "unit.h"
#include "init_d.h"
#include "miscdevice.h"
#include "priv_os.h"
#include "startup.h"
#include "userdefine.h"
#include "sci.h"

  /**
  * @}
  */
 
/**
  * @}
  */

/**
  * @}
  */


  /**
  * @}
  */
 void com0_hal_init( void );

#ifdef __cplusplus
}
#endif

#endif /* __SERIAL0_H */