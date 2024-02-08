/**
 *******************************************************************************
 * @file  usb/usb_dev_cdc/source/cdc_data_process.h
 * @brief Header file for cdc_data_process.c
 @verbatim
   Change Logs:
   Date             Author          Notes
   2022-03-31       CDT             First version
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
#ifndef __CDC_DATA_PROCESS_H__
#define __CDC_DATA_PROCESS_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include <rtdef.h>
#include "usb_dev_cdc_class.h"

/**
 * @addtogroup HC32F4A0_DDL_Applications
 * @{
 */

/**
 * @addtogroup USB_Dev_Cdc
 * @{
 */

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/**
 *  all needed parameters to be configured for the ComPort.
 * These parameters can modified on the fly by the host through CDC class
 * command class requests.
 */

    typedef struct {
        uint32_t bitrate;
        uint8_t format;
        uint8_t paritytype;
        uint8_t datatype;
    } LINE_CODING;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

#define DEFAULT_CONFIG (0U)
#define OTHER_CONFIG (1U)

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/

    extern uint16_t vcp_init(uint32_t u32CdcIndex);
    extern uint16_t vcp_deinit(uint32_t u32CdcIndex);
    extern uint16_t vcp_ctrlpare(uint32_t u32CdcIndex, uint32_t Cmd, uint8_t *Buf, uint32_t Len);
    extern uint16_t vcp_txdata(uint32_t u32CdcIndex, uint8_t *Buf, uint32_t size);
    extern uint16_t vcp_rxdata(uint32_t u32CdcIndex, uint8_t *Buf, uint32_t Len);
    extern rt_ssize_t rt_usbd_cdc0_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size);
    extern rt_ssize_t rt_usbd_cdc1_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size);
    extern rt_ssize_t rt_usbd_cdc0_write(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size);
    extern rt_ssize_t rt_usbd_cdc1_write(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size);

#ifdef __cplusplus
}
#endif

#endif /* __CDC_DATA_PROCESS_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
