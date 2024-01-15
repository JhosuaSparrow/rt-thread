/*
* @Author: Jack Sun
* @Date:   2024-01-08 14:07:06
* @Last Modified by:   jack.sun
* @Last Modified time: 2024-01-12 11:32:15
*/

void led_init(void);
int set_led_enable(int led_no, int enable);
int get_led_enable(int led_no);
void led_onoff(int led_no, int pin_level);
void led_show_thread(void *args);
int led_show_running(void);
