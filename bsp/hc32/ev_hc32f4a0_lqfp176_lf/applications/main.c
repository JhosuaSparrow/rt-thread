/*
* @Author: Jack Sun
* @Date:   2024-01-08 14:07:06
* @Last Modified by:   JhosuaSparrow
* @Last Modified time: 2024-01-22 17:31:40
*/

#include "button.h"
#include "led.h"
#include "atcmd.h"
// #include "usb_cdc.h"


int main(void)
{
    // usb_cdc_init();

    // Init reset button
    reset_button_init();

    // Enable led flick
    for (int i = 1; i <= 10; i++)
    {
        set_led_enable(i, 1);
    }
    led_show_running();

    // Enable at client
    for (int i = 1; i <= 10; i++)
    {
        atcmd_set_client_enable(i, 1);
    }
    atcmd_client_init();

    // Server using UART2
    atcmd_server_send_vbat_running();

    return 0;
}
