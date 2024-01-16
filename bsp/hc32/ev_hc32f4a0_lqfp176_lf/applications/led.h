/*
* @Author: Jack Sun
* @Date:   2024-01-08 14:07:06
* @Last Modified by:   jack.sun
* @Last Modified time: 2024-01-12 11:32:15
*/

#include <rtdef.h>

rt_err_t set_led_enable(rt_uint8_t led_no, rt_uint8_t enable);
rt_uint8_t get_led_enable(rt_uint8_t led_no);
void led_init(void);
void led_show_thread(void *args);
rt_err_t led_show_running(void);
