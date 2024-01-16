/*
* @Author: Jack Sun
* @Date:   2024-01-08 14:07:06
* @Last Modified by:   jack.sun
* @Last Modified time: 2024-01-16 10:19:01
*/

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <led.h>
#include <uart.h>


void main(void)
{
    set_led_enable(2, 1);
    led_show_running();
    set_uart_enable(6, 1);
    uart_start_running();
    test_uart();
}
