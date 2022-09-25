#include "cmsis_os.h"
#include "BAT32G157.h"
static volatile uint32_t uwTick;
void SysTick_Handler(void)
{
  uwTick++;
  osSystickHandler();
}

uint32_t HAL_GetTick(void)
{
  return uwTick;
}
