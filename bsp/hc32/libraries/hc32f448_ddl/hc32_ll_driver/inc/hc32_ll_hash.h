/**
 *******************************************************************************
 * @file  hc32_ll_hash.h
 * @brief This file contains all the functions prototypes of the HASH driver
 *        library.
 @verbatim
   Change Logs:
   Date             Author          Notes
<<<<<<< HEAD:bsp/hc32/libraries/hc32f4a0_ddl/midwares/hc32/usb/usb_device_lib/device_core/usb_dev_core.h
   2022-03-31       CDT             First version
   2022-06-30       CDT             Add USB core ID select function
=======
   2023-05-31       CDT             First version
>>>>>>> master:bsp/hc32/libraries/hc32f448_ddl/hc32_ll_driver/inc/hc32_ll_hash.h
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
#ifndef __HC32_LL_HASH_H__
#define __HC32_LL_HASH_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ll_def.h"

#include "hc32f4xx.h"
#include "hc32f4xx_conf.h"
/**
 * @addtogroup LL_Driver
 * @{
 */

/**
 * @addtogroup LL_HASH
 * @{
 */

#if (LL_HASH_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
<<<<<<< HEAD:bsp/hc32/libraries/hc32f4a0_ddl/midwares/hc32/usb/usb_device_lib/device_core/usb_dev_core.h
extern void usb_dev_init(usb_core_instance *pdev,
                         stc_usb_port_identify *pstcPortIdentify,
                         usb_dev_desc_func *pdesc,
                         usb_dev_class_func *class_cb,
                         usb_dev_user_func *usr_cb);
extern void usb_dev_deinit(usb_core_instance *pdev);
=======
/**
 * @addtogroup HASH_Global_Functions
 * @{
 */

int32_t HASH_DeInit(void);
int32_t HASH_Calculate(const uint8_t *pu8SrcData, uint32_t u32SrcDataSize, uint8_t *pu8MsgDigest);

/**
 * @}
 */

#endif /* LL_HASH_ENABLE */
>>>>>>> master:bsp/hc32/libraries/hc32f448_ddl/hc32_ll_driver/inc/hc32_ll_hash.h

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32_LL_HASH_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
