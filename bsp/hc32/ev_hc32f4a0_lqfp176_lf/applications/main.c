/*
* @Author: Jack Sun
* @Date:   2024-01-08 14:07:06
* @Last Modified by:   jack.sun
* @Last Modified time: 2024-01-18 10:41:55
*/

#include <button.h>
#include <led.h>
#include <atcmd.h>


void main(void)
{
    reset_button_init();
    set_led_enable(2, 1);
    led_show_running();
    atcmd_set_client_enable(6, 1);
    atcmd_client_init();
    // Server using UART2
    atcmd_server_send_vbat_running();
}
