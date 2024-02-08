/**
 *******************************************************************************
 * @file  usb/usb_dev_cdc/source/cdc_data_process.c
 * @brief Generic media access Layer.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2022-03-31       CDT             First version
   2022-10-31       CDT             Modify for MISRAC
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
// #include <string.h>
#include "rtthread.h"
// #include <rtdevice.h>
#include "cdc_data_process.h"

#define LOG_TAG "lf.cdc"
#include <lf_log.h>
/**
 * @addtogroup HC32F4A0_DDL_Applications
 * @{
 */

/**
 * @addtogroup USB_Dev_Cdc
 * @{
 */

uint16_t vcp_init(uint32_t u32CdcIndex);
uint16_t vcp_deinit(uint32_t u32CdcIndex);
uint16_t vcp_ctrlpare(uint32_t u32CdcIndex, uint32_t Cmd, uint8_t *Buf, uint32_t Len);
uint16_t vcp_txdata(uint32_t u32CdcIndex, uint8_t *Buf, uint32_t size);
uint16_t vcp_rxdata(uint32_t u32CdcIndex, uint8_t *Buf, uint32_t Len);
rt_ssize_t rt_usbd_cdc0_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size);
rt_ssize_t rt_usbd_cdc1_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size);
rt_ssize_t rt_usbd_cdc0_write(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size);
rt_ssize_t rt_usbd_cdc1_write(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size);
static rt_ssize_t _rt_usbd_read(uint32_t u32CdcIndex, uint8_t *buffer, rt_size_t size);

extern struct rt_device rt_usb_dev;

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

typedef struct cdc_cache {
    uint8_t buff[APP_RX_DATA_SIZE];
    uint8_t retag;
    uint32_t windex;
    uint32_t rindex;
    uint32_t size;
} cdc_cache_t;

CDC_IF_Prop_TypeDef VCP_fops = {
    &vcp_init,
    &vcp_deinit,
    &vcp_ctrlpare,
    &vcp_txdata,
    &vcp_rxdata
};

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

static cdc_cache_t cdc_rcache[CDC_NUM] = { 0 };

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Initializes the configuration of usart port
 * @param  None
 * @retval None
 */
uint16_t vcp_init(uint32_t u32CdcIndex) { return 0; }

/**
 * @brief  deInitializes the Media
 * @param  None
 * @retval None
 */
uint16_t vcp_deinit(uint32_t u32CdcIndex) { return 0; }

/**
 * @brief  Manage the CDC class requests
 * @param  [in] Cmd     Command code
 * @param  [in] Buf     data to be sent or received
 * @param  [in] Len     data length in bytes
 * @retval status
 */
uint16_t vcp_ctrlpare(uint32_t u32CdcIndex, uint32_t Cmd, uint8_t *Buf, uint32_t Len) {
    switch (Cmd) {
    case SEND_ENCAPSULATED_COMMAND:
      /* Not  needed for this driver */
        break;

    case GET_ENCAPSULATED_RESPONSE:
      /* Not  needed for this driver */
        break;

    case SET_COMM_FEATURE:
      /* Not  needed for this driver */
        break;

    case GET_COMM_FEATURE:
      /* Not  needed for this driver */
        break;

    case CLEAR_COMM_FEATURE:
      /* Not  needed for this driver */
        break;

    case SET_LINE_CODING:
        break;
    case GET_LINE_CODING:;
        break;
    case SET_CONTROL_LINE_STATE:
      /* Not  needed for this driver */
        break;
    case SEND_BREAK:
      /* Not  needed for this driver */
        break;
    default:
        break;
    }
    return 0;
}

/**
 * @brief  Receive data from USART and save in the buffer uart_rx_buffer, the
 *         received number is counted by APP_Rx_ptr_in
 * @param  None
 * @retval None
 */
uint16_t vcp_txdata(uint32_t u32CdcIndex, uint8_t *Buf, uint32_t size) {
    uint32_t write_size = 0U;
    while (write_size < size) {
        uart_rx_buffer[u32CdcIndex][APP_Rx_ptr_in[u32CdcIndex]] = Buf[write_size];
        APP_Rx_ptr_in[u32CdcIndex]++;
        write_size++;
        /* To avoid buffer overflow */
        if (APP_Rx_ptr_in[u32CdcIndex] == APP_RX_DATA_SIZE) {
            APP_Rx_ptr_in[u32CdcIndex] = 0U;
        }
    }
    // LOG_D("vcp_txdata uart_rx_buffer[u32CdcIndex] %s", uart_rx_buffer[u32CdcIndex]);
    // LOG_D("vcp_txdata APP_Rx_ptr_in[u32CdcIndex] %d", APP_Rx_ptr_in[u32CdcIndex]);
    return 0;
}

/**
 * @brief  Data received from USB are sent to uart port
 * @param  [in] Buf     buffer to be sent to uart port
 * @param  [in] Len     data length in bytes
 * @retval None
 */
uint16_t vcp_rxdata(uint32_t u32CdcIndex, uint8_t *Buf, uint32_t Len) {
      // LOG_D("vcp_rxdata vcp_rxdata %s", Buf);
    rt_size_t write_size = 0;
    while (write_size < Len) {
        cdc_rcache[u32CdcIndex].buff[cdc_rcache[u32CdcIndex].windex] = Buf[write_size];
        write_size++;
        cdc_rcache[u32CdcIndex].windex++;
        if (cdc_rcache[u32CdcIndex].windex >= APP_RX_DATA_SIZE) {
            cdc_rcache[u32CdcIndex].retag = 1;
            cdc_rcache[u32CdcIndex].windex = 0;
        }
    }

    cdc_rcache[u32CdcIndex].size += write_size;
    // LOG_D("vcp_rxdata cdc_rcache[u32CdcIndex].buff %s", cdc_rcache[u32CdcIndex].buff);
    rt_usb_dev.rx_indicate(&rt_usb_dev, write_size);
    return 0;
}

rt_ssize_t _rt_usbd_read(uint32_t u32CdcIndex, uint8_t *buffer, rt_size_t size) {
    rt_size_t read_size = 0;
    if (size <= 0 || size == RT_NULL) {
        size = cdc_rcache[u32CdcIndex].size;
    }
    while (read_size < size) {
        if (cdc_rcache[u32CdcIndex].rindex == 0 && cdc_rcache[u32CdcIndex].retag == 1) {
            cdc_rcache[u32CdcIndex].retag = 0;
        }
        if (cdc_rcache[u32CdcIndex].retag == 0 && cdc_rcache[u32CdcIndex].windex <= cdc_rcache[u32CdcIndex].rindex) {
            break;
        }
        buffer[read_size] = cdc_rcache[u32CdcIndex].buff[cdc_rcache[u32CdcIndex].rindex];
        read_size++;
        cdc_rcache[u32CdcIndex].rindex++;
        cdc_rcache[u32CdcIndex].rindex = cdc_rcache[u32CdcIndex].rindex >= APP_RX_DATA_SIZE ? 0 : cdc_rcache[u32CdcIndex].rindex;
    }

    cdc_rcache[u32CdcIndex].size -= read_size;
    return (rt_ssize_t)read_size;
}

rt_ssize_t rt_usbd_cdc0_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size) {
    return _rt_usbd_read(0, (uint8_t *)buffer, size);
}

rt_ssize_t rt_usbd_cdc1_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size) {
    return _rt_usbd_read(1, (uint8_t *)buffer, size);
}

rt_ssize_t rt_usbd_cdc0_write(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size) {
    vcp_txdata(0, (uint8_t *)buffer, size);
    return (rt_ssize_t)size;
}

rt_ssize_t rt_usbd_cdc1_write(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size) {
    vcp_txdata(1, (uint8_t *)buffer, size);
    return (rt_ssize_t)size;
}

/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
