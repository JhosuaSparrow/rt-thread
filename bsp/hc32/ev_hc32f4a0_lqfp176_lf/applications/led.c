/*
* @Author: Jack Sun
* @Date:   2024-01-08 14:07:06
* @Last Modified by:   JhosuaSparrow
* @Last Modified time: 2024-01-09 09:27:50
*/
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

/* defined the LED_GREEN pin: PC9 */
#define LED_0_PIN GET_PIN(C, 9)

static int led_enable = 0;

void led_init(void) {
    rt_pin_mode(LED_0_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_1_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_2_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_3_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_4_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_5_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_6_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_7_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_8_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_9_PIN, PIN_MODE_OUTPUT);
}

int set_led_enable(int led_no, int enable) {
    if (enable) {
        led_enable |= (1 << led_no);
    } else {
        led_enable &= (~(1 << led_no));
    }
}

int get_led_enable(int led_no) {
    int _enable_ = 0;
    _enable_ = led_enable & (1 << led_no);
    return _enable_
}

void led_show_thread(void) {
    led_init()
    while (1) {
        for(int i=0; i < 10; i++) {
            if (get_led_enable(i)) {
                rt_pin_write(LED_0_PIN, PIN_HIGH);
            }
        }
        rt_thread_mdelay(500);
        for(int i=0; i < 10; i++) {
            if (get_led_enable(i)) {
                rt_pin_write(LED_0_PIN, PIN_LOW);
            }
        }
        rt_thread_mdelay(500);
    }

}
