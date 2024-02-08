/**
 *******************************************************************************
 * @file  usb/usb_dev_cdc/source/main.h
 * @brief This file contains the including files of main routine.
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
#ifndef __MAIN_H__
#define __MAIN_H__

#include <rtdef.h>
#include "usb_dev_user.h"
#include "usb_dev_desc.h"
#include "usb_dev_cdc_class.h"
#include "cdc_data_process.h"

#endif /* __MAIN_H__ */

void usb_cdc_init(void);
rt_err_t rt_hc32_cdc_register(rt_device_t rt_usb_dev, const char *name, rt_size_t dev_id, rt_uint16_t flags);
void rt_hc32_cdcs_register(void);
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
