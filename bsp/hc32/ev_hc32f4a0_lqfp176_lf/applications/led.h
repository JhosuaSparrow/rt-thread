/*
 * @Author: Jack Sun
 * @Date:   2024-01-08 14:07:06
 * @Last Modified by:   jack.sun
 * @Last Modified time: 2024-01-12 11:32:15
 */

#include "rtthread.h"
#include "rtdevice.h"
#include "board.h"
#include "drv_gpio.h"

#define LED_DELAY_MS 500

rt_uint16_t get_led_enable(rt_uint16_t led_no);
rt_err_t set_led_enable(rt_uint16_t led_no, rt_uint16_t enable);
void led_init(void);
void led_on(rt_uint8_t led_id);
void led_off(rt_uint8_t led_id);
void led_toggle(rt_uint8_t led_id);
rt_uint8_t led_state(rt_uint8_t led_id);
void led_show_thread(void *args);
rt_err_t led_show_running(void);
