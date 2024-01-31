/**
 *******************************************************************************
 * @file  usb/usb_dev_cdc/source/main.c
 * @brief Example CDC device.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2022-03-31       CDT             First version
   2022-10-31       CDT             Add USB core ID select function
 @endverbatim
 *******************************************************************************
 * Copyright (C) 2022-2023, Xiaohua Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by XHSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 *
 *******************************************************************************
 */

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include <rtthread.h>
#include <rtdevice.h>
#include "usb_cdc.h"

#define LOG_TAG "lf.usbcdc"
#include "lf_log.h"

/**
 * @addtogroup HC32F4A0_DDL_Applications
 * @{
 */

/**
 * @addtogroup USB_Dev_Cdc
 * @{
 */

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

usb_core_instance usb_dev;
struct rt_device rt_usb_dev = {0};

// static rt_err_t rt_hc32_cdc_register(rt_device_t rt_usb_dev, const char *name, rt_uint16_t flags);

/**
 * @brief  usb_cdc_init function
 * @param  None
 * @retval None
 */
void usb_cdc_init(void)
{
    stc_usb_port_identify stcPortIdentify;
#if defined(USB_FS_MODE)
    stcPortIdentify.u8CoreID = USBFS_CORE_ID;
    stcPortIdentify.u8PhyType = USBHS_PHY_EMBED;
#elif defined(USB_HS_MODE) && (!defined(USB_HS_EXTERNAL_PHY))
    stcPortIdentify.u8CoreID = USBHS_CORE_ID;
    stcPortIdentify.u8PhyType = USBHS_PHY_EMBED;
#elif defined(USB_HS_MODE) && (defined(USB_HS_EXTERNAL_PHY))
    stcPortIdentify.u8CoreID = USBHS_CORE_ID;
    stcPortIdentify.u8PhyType = USBHS_PHY_EXT;
#endif
    usb_dev_init(&usb_dev, &stcPortIdentify, &user_desc, &class_cdc_cbk, &user_cb);
}

rt_err_t rt_hc32_cdc_register(rt_device_t rt_usb_dev, const char *name, rt_uint16_t flags)
{
    rt_err_t ret;

    rt_usb_dev->type = RT_Device_Class_Char;
    rt_usb_dev->rx_indicate = RT_NULL;
    rt_usb_dev->tx_complete = RT_NULL;

    rt_usb_dev->init = RT_NULL;
    rt_usb_dev->open = RT_NULL;
    rt_usb_dev->close = RT_NULL;
    rt_usb_dev->read = rt_usbd_read;
    rt_usb_dev->write = rt_usbd_write;
    rt_usb_dev->control = RT_NULL;

    /* register a character device */
    ret = rt_device_register(rt_usb_dev, name, flags);
    LOG_D("rt_device_register %s ret %d", name, ret);

    return ret;
}

/**
 * @}
 */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
