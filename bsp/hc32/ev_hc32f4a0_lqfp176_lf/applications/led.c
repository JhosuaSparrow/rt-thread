/*
* @Author: Jack Sun
* @Date:   2024-01-08 14:07:06
* @Last Modified by:   jack.sun
* @Last Modified time: 2024-01-15 13:57:34
*/
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <led.h>

int led_enable = 0;

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


rt_err_t set_led_enable(rt_uint8_t led_no, rt_uint8_t enable)
{
    if (enable)
    {
        led_enable |= (1 << (led_no - 1));
    }
    else
    {
        led_enable &= (~(1 << (led_no - 1)));
        rt_pin_write(led_pins[led_no - 1], PIN_LOW);
    }
    return RT_EOK;
}


rt_uint8_t get_led_enable(rt_uint8_t led_no)
{
    rt_uint8_t _enable_ = 0;
    _enable_ = led_enable & (1 << (led_no - 1));
    return _enable_;
}


void led_init(void)
{

    for (int i=0; i<10; i++)
    {
        rt_pin_mode(led_pins[i], PIN_MODE_OUTPUT);
        rt_pin_write(led_pins[i], PIN_LOW);
    }
}


void led_show_thread(void *args)
{
    led_init();
    while (1)
    {
        for(int i=0; i<10; i++)
        {
            if (get_led_enable(i + 1) != 0)
            {
                rt_pin_write(led_pins[i], !rt_pin_read(led_pins[i]));
            }
            else
            {
                if (rt_pin_read(led_pins[i]) == PIN_HIGH)
                {
                    rt_pin_write(led_pins[i], PIN_LOW);
                }
            }
        }
        rt_thread_mdelay(500);
    }
}

rt_err_t led_show_running(void)
{
    rt_thread_t tid;
    rt_err_t ret = RT_EOK;

    tid = rt_thread_create("led", led_show_thread, RT_NULL, 1024, RT_THREAD_PRIORITY_MAX / 3, 20);
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
