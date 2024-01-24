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
#include "usb_cdc.h"
#include <rtthread.h>

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
usb_core_instance  usb_dev;

/**
 * @brief  usb_cdc_init function
 * @param  None
 * @retval None
 */

void usb_cdc_init(void) {
    stc_usb_port_identify stcPortIdentify;
#if defined (USB_FS_MODE)
    stcPortIdentify.u8CoreID = USBFS_CORE_ID;
    stcPortIdentify.u8PhyType = USBHS_PHY_EMBED;
#elif defined (USB_HS_MODE) && (!defined (USB_HS_EXTERNAL_PHY))
    stcPortIdentify.u8CoreID = USBHS_CORE_ID;
    stcPortIdentify.u8PhyType = USBHS_PHY_EMBED;
#elif defined (USB_HS_MODE) && (defined (USB_HS_EXTERNAL_PHY))
    stcPortIdentify.u8CoreID = USBHS_CORE_ID;
    stcPortIdentify.u8PhyType = USBHS_PHY_EXT;
#endif
    usb_dev_init(&usb_dev, &stcPortIdentify, &user_desc, &class_cdc_cbk, &user_cb);
}

/**
 * @}
 */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
