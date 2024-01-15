/*
* @Author: Jack Sun
* @Date:   2024-01-08 14:07:06
* @Last Modified by:   jack.sun
* @Last Modified time: 2024-01-13 17:04:35
*/
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <led.h>

#define LED_1_PIN GET_PIN(A, 8)
#define LED_2_PIN GET_PIN(C, 9)
#define LED_3_PIN GET_PIN(C, 8)
#define LED_4_PIN GET_PIN(C, 7)
#define LED_5_PIN GET_PIN(C, 6)
#define LED_6_PIN GET_PIN(D, 15)
#define LED_7_PIN GET_PIN(D, 14)
#define LED_8_PIN GET_PIN(D, 13)
#define LED_9_PIN GET_PIN(D, 12)
#define LED_10_PIN GET_PIN(D, 11)

int led_enable = 0;


void led_init(void)
{
    rt_pin_mode(LED_1_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_2_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_3_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_4_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_5_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_6_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_7_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_8_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_9_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_10_PIN, PIN_MODE_OUTPUT);

    for (int i=1; i<=10; i++)
    {
        led_onoff(i, PIN_LOW);
    }
}


int set_led_enable(int led_no, int enable)
{
    if (enable)
    {
        led_enable |= (1 << (led_no - 1));
    }
    else
    {
        led_enable &= (~(1 << (led_no - 1)));
        led_onoff(led_no, PIN_LOW);
    }
}


int get_led_enable(int led_no)
{
    int _enable_ = 0;
    _enable_ = led_enable & (1 << (led_no - 1));
    return _enable_;
}


void led_onoff(int led_no, int pin_level)
{
    switch (led_no)
    {
        case 1:
            rt_pin_write(LED_1_PIN, pin_level);
            break;
        case 2:
            rt_pin_write(LED_2_PIN, pin_level);
            break;
        case 3:
            rt_pin_write(LED_3_PIN, pin_level);
            break;
        case 4:
            rt_pin_write(LED_4_PIN, pin_level);
            break;
        case 5:
            rt_pin_write(LED_5_PIN, pin_level);
            break;
        case 6:
            rt_pin_write(LED_6_PIN, pin_level);
            break;
        case 7:
            rt_pin_write(LED_7_PIN, pin_level);
            break;
        case 8:
            rt_pin_write(LED_8_PIN, pin_level);
            break;
        case 9:
            rt_pin_write(LED_9_PIN, pin_level);
            break;
        case 10:
            rt_pin_write(LED_10_PIN, pin_level);
            break;
    }
}


void led_show_thread(void *args)
{
    int level = PIN_LOW;
    led_init();
    while (1)
    {
        level = !level;
        for(int i=1; i <= 10; i++)
        {
            if (get_led_enable(i))
            {
                led_onoff(i, level);
            }
        }
        rt_thread_mdelay(500);
    }
}

int led_show_running(void)
{
    rt_thread_t tid;
    rt_err_t ret = RT_EOK;

    tid = rt_thread_create("led", led_show_thread, RT_NULL, 512, RT_THREAD_PRIORITY_MAX / 3, 20);
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
