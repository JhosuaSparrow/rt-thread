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
// #include <rtthread.h>
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

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

typedef struct cdc_cache
{
    uint8_t buff[APP_RX_DATA_SIZE];
    uint8_t retag;
    uint32_t windex;
    uint32_t rindex;
    uint32_t size;
} cdc_cache_t;

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

static cdc_cache_t cdc_rcache = {
    {0}, 0, 0, 0, 0};

static rt_ssize_t _rt_usbd_read(uint8_t *buffer, rt_size_t size);
// static rt_sem_t cdc_sem;

// extern usb_core_instance usb_dev;

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

extern struct rt_device rt_usb_dev;

/**
 * @brief  Initializes the configuration of usart port
 * @param  None
 * @retval None
 */
void vcp_init(void)
{
}

/**
 * @brief  deInitializes the Media
 * @param  None
 * @retval None
 */
void vcp_deinit(void)
{
}

/**
 * @brief  Manage the CDC class requests
 * @param  [in] Cmd     Command code
 * @param  [in] Buf     data to be sent or received
 * @param  [in] Len     data length in bytes
 * @retval status
 */
void vcp_ctrlpare(uint32_t Cmd, uint8_t *Buf, uint32_t Len)
{
    switch (Cmd)
    {
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
}

/**
 * @brief  Receive data from USART and save in the buffer uart_rx_buffer, the
 *         received number is counted by APP_Rx_ptr_in
 * @param  None
 * @retval None
 */
void vcp_txdata(uint8_t *Buf, uint32_t size)
{
    // if (linecoding.datatype == 7U) {
    //     uart_rx_buffer[APP_Rx_ptr_in] = (uint8_t)USART_ReadData(CDC_COMM) & 0x7FU;
    // } else if (linecoding.datatype == 8U) {
    //     uart_rx_buffer[APP_Rx_ptr_in] = (uint8_t)USART_ReadData(CDC_COMM);
    // } else {
    //     ;
    // }

    // /* To avoid buffer overflow */
    // if (APP_Rx_ptr_in == APP_RX_DATA_SIZE) {
    //     APP_Rx_ptr_in = 0U;
    // }

    uint32_t write_size = 0U;
    while (write_size < size)
    {
        uart_rx_buffer[APP_Rx_ptr_in] = Buf[write_size];
        APP_Rx_ptr_in++;
        write_size++;
        /* To avoid buffer overflow */
        if (APP_Rx_ptr_in == APP_RX_DATA_SIZE)
        {
            APP_Rx_ptr_in = 0U;
        }
    }
    // LOG_D("vcp_txdata uart_rx_buffer %s", uart_rx_buffer);
    // LOG_D("vcp_txdata APP_Rx_ptr_in %d", APP_Rx_ptr_in);
}

/**
 * @brief  Data received from USB are sent to uart port
 * @param  [in] Buf     buffer to be sent to uart port
 * @param  [in] Len     data length in bytes
 * @retval None
 */
void vcp_rxdata(uint8_t *Buf, uint32_t Len)
{
    // uint32_t i;

    // for (i = 0UL; i < Len; i++) {
    //     while (SET != USART_GetStatus(CDC_COMM, USART_FLAG_TX_EMPTY)) {
    //         ;
    //     }
    //     USART_WriteData(CDC_COMM, (uint16_t)Buf[i]);
    // }

    // LOG_D("vcp_rxdata vcp_rxdata %s", Buf);
    rt_size_t write_size = 0;
    while (write_size < Len)
    {
        // memcpy(cdc_rcache.buff[cdc_rcache.windex], Buf[write_size], 1);
        cdc_rcache.buff[cdc_rcache.windex] = Buf[write_size];
        write_size++;
        cdc_rcache.windex++;
        if (cdc_rcache.windex >= APP_RX_DATA_SIZE)
        {
            cdc_rcache.retag = 1;
        }
        cdc_rcache.windex = cdc_rcache.windex >= APP_RX_DATA_SIZE ? 0 : cdc_rcache.windex;
    }

    cdc_rcache.size += write_size;
    // LOG_D("vcp_rxdata cdc_rcache.buff %s", cdc_rcache.buff);
    rt_usb_dev.rx_indicate(&rt_usb_dev, write_size);
}

rt_ssize_t _rt_usbd_read(uint8_t *buffer, rt_size_t size)
{
    rt_size_t read_size = 0;
    if (size <= 0 || size == RT_NULL)
    {
        size = cdc_rcache.size;
    }
    while (read_size < size)
    {
        if (cdc_rcache.rindex == 0 && cdc_rcache.retag == 1)
        {
            cdc_rcache.retag = 0;
        }
        if (cdc_rcache.retag == 0 && cdc_rcache.windex <= cdc_rcache.rindex)
        {
            break;
        }
        // memcpy(buffer + read_size, cdc_rcache.buff[cdc_rcache.rindex], 1);
        buffer[read_size] = cdc_rcache.buff[cdc_rcache.rindex];
        read_size++;
        cdc_rcache.rindex++;
        cdc_rcache.rindex = cdc_rcache.rindex >= APP_RX_DATA_SIZE ? 0 : cdc_rcache.rindex;
    }

    cdc_rcache.size -= read_size;
    return (rt_ssize_t)read_size;
}

rt_ssize_t rt_usbd_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    return _rt_usbd_read((uint8_t *)buffer, size);
}

rt_ssize_t rt_usbd_write(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    vcp_txdata((uint8_t *)buffer, size);
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
