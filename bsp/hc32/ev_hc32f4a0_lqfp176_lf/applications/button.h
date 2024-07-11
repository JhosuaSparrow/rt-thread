#include "rtthread.h"
#include "rtdevice.h"
#include "board.h"
#include "atcmd.h"
#include "drv_gpio.h"

#define RESET_BUTTON_PIN GET_PIN(D, 10)

void button_event_thread(void *args);
void irq_callback(void *args);
rt_err_t button_event_running(void);
rt_err_t button_event_mq_init(void);
void reset_button_init(void);
