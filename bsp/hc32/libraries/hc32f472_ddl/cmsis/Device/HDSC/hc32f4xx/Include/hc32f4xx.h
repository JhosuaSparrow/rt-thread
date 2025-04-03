/**
 *******************************************************************************
 * @file  hc32f4xx.h
 * @brief This file contains the common part of the HC32 series.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2022-06-30       CDT             First version
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
#ifndef __HC32F4XX_H__
#define __HC32F4XX_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
/**
 * @brief HC32F4xx Device Include
 */
#if defined(HC32F472)
#include "hc32f472.h"
#include "system_hc32f472.h"
#else
#error "Please select first the target HC32xxxx device used in your application (in hc32xxxx.h file)"
#endif

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
 * Global function prototypes (definition in C source)
 ******************************************************************************/

#ifdef __cplusplus
}
#endif

<<<<<<< HEAD:bsp/hc32/libraries/hc32f4a0_ddl/midwares/hc32/iec60730_class_b_stl/stl_test_item/inc/stl_test_cpu.h
#endif /* __STL_TEST_CPU_H__ */
=======
#endif /* __HC32F4XX_H__ */
>>>>>>> master:bsp/hc32/libraries/hc32f472_ddl/cmsis/Device/HDSC/hc32f4xx/Include/hc32f4xx.h

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
