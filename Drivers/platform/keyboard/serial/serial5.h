#ifndef __SERIAL5_H
#define __SERIAL5_H

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
/**
  * @}
  */

/**
  * @}
  */
#define DEBUG_RX_MAX 64

  /**
  * @}
  */

  extern void serial5_rst(void);

#ifdef __cplusplus
}
#endif

#endif /* __SERIAL1_H */
