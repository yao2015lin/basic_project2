/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usb_device.c
  * @version        : v2.0_Cube
  * @brief          : This file implements the USB Device
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/

#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_hid.h"

/* USER CODE BEGIN Includes */
#include "cmsis_os.h"
#include "unit.h"
#include "miscdevice.h"
#include "priv_list.h"
#include "init_d.h"
#include "startup.h"

USBD_HandleTypeDef hUsbDeviceFS;
extern USBD_DescriptorsTypeDef FS_Desc;


static void MX_USB_DEVICE_Init(void)
{
    if (USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS) != USBD_OK)
    {
        Error_Handler(__FILE__,__LINE__);
    }
    if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_HID) != USBD_OK)
    {
        Error_Handler(__FILE__,__LINE__);
    }
    if (USBD_Start(&hUsbDeviceFS) != USBD_OK)
    {
        Error_Handler(__FILE__,__LINE__);
    }
}

static int usb_write(FIL_HAND *fd, const void *buf, uint32_t count)
{
	uint8_t *data = ( uint8_t *)buf;

	if( count > 8 )
	{
		log(ERR ,"leng is over , len=%d\n" , count);
		return -1;
	}

	USBD_HID_SendReport(&hUsbDeviceFS , data , count);
	
	return 0;
}

static int usb_open(FIL_HAND *fd)
{
  	MX_USB_DEVICE_Init();
    return 0;
}

static int usb_resume(FIL_HAND *fd)
{
  
    return 0;
}

static int usb_suspend(FIL_HAND *fd)
{

    return 0;
}


struct file_operations usb_fops =
{
    .onwer = 0,
    .open = usb_open,
    .write = usb_write,
    .resume = usb_resume,
    .suspend = usb_suspend,
};

struct miscdevice usb_device =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = "usb-hid",
    .fops = &usb_fops,
    .nodename = "modules",
};

static int usb_hid_init(void)
{
    return (misc_register(&usb_device));
}

modules_init(usb_hid_init);




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
