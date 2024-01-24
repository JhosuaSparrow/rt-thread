/*
* @Author: Jack Sun
* @Date:   2024-01-08 14:07:06
* @Last Modified by:   JhosuaSparrow
* @Last Modified time: 2024-01-22 17:27:44
*/
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "led.h"

#define LOG_TAG             "lf.led"
#include <lf_log.h>

#define LED_DELAY_MS        500

rt_uint16_t led_enable = 0;

static rt_base_t led_pins[] = {
    GET_PIN(A, 8),
    GET_PIN(C, 9),
    GET_PIN(C, 8),
    GET_PIN(C, 7),
    GET_PIN(C, 6),
    GET_PIN(D, 15),
    GET_PIN(D, 14),
    GET_PIN(D, 13),
    GET_PIN(D, 12),
    GET_PIN(D, 11)
};


rt_uint16_t get_led_enable(rt_uint16_t led_no)
{
    rt_uint16_t _enable_ = 0;
    _enable_ = led_enable & (1 << (led_no - 1));
    return _enable_;
}


rt_err_t set_led_enable(rt_uint16_t led_no, rt_uint16_t enable)
{
    if (enable)
    {
        led_enable |= (1 << (led_no - 1));
    }
    else
    {
        led_enable &= (~(1 << (led_no - 1)));
        led_off(led_no);
    }
    LOG_D("set_led_enable led_no %d, enable %d, get_led_enable %d", led_no, enable, get_led_enable(led_no));
    return RT_EOK;
}


void led_init(void)
{

    for (int i=0; i<10; i++)
    {
        rt_pin_mode(led_pins[i], PIN_MODE_OUTPUT);
        led_off(i);
    }
}


void led_on(rt_uint8_t led_id)
{
    rt_pin_write(led_pins[led_id], PIN_LOW);
}


void led_off(rt_uint8_t led_id)
{
    rt_pin_write(led_pins[led_id], PIN_HIGH);
}


void led_toggle(rt_uint8_t led_id)
{
    rt_pin_write(led_pins[led_id], !rt_pin_read(led_pins[led_id]));
}


rt_uint8_t led_state(rt_uint8_t led_id)
{
    rt_uint8_t res = rt_pin_read(led_pins[led_id]);
    return res;
}


void led_show_thread(void *args)
{
    while (1)
    {
        for(int i=0; i<10; i++)
        {
            if (get_led_enable(i + 1) != 0)
            {
                led_toggle(i);
            }
            else
            {
                if (led_state(i) == PIN_LOW)
                {
                    led_off(i);
                }
            }
        }
        rt_thread_mdelay(LED_DELAY_MS);
    }
}

rt_err_t led_show_running(void)
{
    rt_thread_t tid;
    rt_err_t ret = RT_EOK;

    led_init();
    tid = rt_thread_create("led", led_show_thread, RT_NULL, 0x1000, RT_THREAD_PRIORITY_MAX / 3, 20);
    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }
    else
    {
        ret = RT_ERROR;
    }

    return ret;
}
