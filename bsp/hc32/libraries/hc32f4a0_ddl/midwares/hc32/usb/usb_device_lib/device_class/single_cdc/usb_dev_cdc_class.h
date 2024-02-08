/**
 *******************************************************************************
 * @file  usb_dev_cdc_class.h
 * @brief Head file for usb_dev_cdc_class.c
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
#ifndef __USB_DEV_CDC_CLASS_H__
#define __USB_DEV_CDC_CLASS_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_dev_def.h"

/**
 * @addtogroup LL_USB_LIB
 * @{
 */

/**
 * @addtogroup LL_USB_DEV_CLASS
 * @{
 */

/**
 * @addtogroup LL_USB_SINGLE_CDC
 * @{
 */

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
#define CDC_NUM 0x02u
#define CDC_INTERFACE_NUM CDC_NUM * 2u
/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
// #define USB_CDC_CONFIG_DESC_SIZ (75U)
// #define USB_CDC_DESC_SIZ (75U - 9U)
#define USB_CDC_CONFIG_DESC_SIZ (9u + (58u + 8u) * CDC_NUM)
#define USB_CDC_DESC_SIZ (67u - 9u)

#define CDC_DESCRIPTOR_TYPE (0x21U)

#define DEVICE_CLASS_CDC (0x02U)
#define DEVICE_SUBCLASS_CDC (0x00U)

#define STANDARD_ENDPOINT_DESC_SIZE (0x09U)

#define MAX_CDC_IN_PACKET_SIZE (MAX_CDC_PACKET_SIZE)

#define MAX_CDC_OUT_PACKET_SIZE (MAX_CDC_PACKET_SIZE)

/**************************************************/
/* CDC Requests                                   */
/**************************************************/
#define SEND_ENCAPSULATED_COMMAND (0x00U)
#define GET_ENCAPSULATED_RESPONSE (0x01U)
#define SET_COMM_FEATURE (0x02U)
#define GET_COMM_FEATURE (0x03U)
#define CLEAR_COMM_FEATURE (0x04U)
#define SET_LINE_CODING (0x20U)
#define GET_LINE_CODING (0x21U)
#define SET_CONTROL_LINE_STATE (0x22U)
#define SEND_BREAK (0x23U)
#define NO_CMD (0xFFU)

#define APP_FOPS                          (VCP_fops)

  typedef struct _CDC_IF_PROP {
    uint16_t(*pIf_Init)(uint32_t u32CdcIndex);
    uint16_t(*pIf_DeInit)(uint32_t u32CdcIndex);
    uint16_t(*pIf_Ctrl)(uint32_t u32CdcIndex, uint32_t Cmd, uint8_t *Buf, uint32_t Len);
    uint16_t(*pIf_DataTx)(uint32_t u32CdcIndex, uint8_t *Buf, uint32_t Len);
    uint16_t(*pIf_DataRx)(uint32_t u32CdcIndex, uint8_t *Buf, uint32_t Len);
  } CDC_IF_Prop_TypeDef;

  typedef struct stc_cdc_ch_res {
    uint8_t Instance;
    uint8_t data_in_ep;
    uint8_t data_out_ep;
    uint8_t command_ep;
    uint8_t command_itf;
  } stc_cdc_ch_res_t;

  extern const stc_cdc_ch_res_t stcCDC_ChPara[CDC_NUM];

  extern uint8_t uart_rx_buffer[CDC_NUM][APP_RX_DATA_SIZE];
  extern uint32_t APP_Rx_ptr_in[CDC_NUM];
  extern uint32_t APP_Rx_ptr_out[CDC_NUM];
  /*******************************************************************************
   * Global variable definitions ('extern')
   ******************************************************************************/
  extern usb_dev_class_func class_cdc_cbk;

  /* CDC Device library callbacks */
  extern void usb_dev_cdc_init(void *pdev);
  extern void usb_dev_cdc_deinit(void *pdev);
  extern uint8_t usb_dev_cdc_setup(void *pdev, USB_SETUP_REQ *req);
  extern void usb_dev_cdc_datain(void *pdev, uint8_t epnum);
  extern void usb_dev_cdc_dataout(void *pdev, uint8_t epnum);
  extern void usb_dev_cdc_ctrlep_rxready(void *pdev);
  extern uint8_t usb_dev_cdc_sof(void *pdev);

  /**
   * @}
   */

  /**
   * @}
   */

  /**
   * @}
   */

#ifdef __cplusplus
}
#endif

#endif /* __USB_DEV_CDC_CLASS_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
