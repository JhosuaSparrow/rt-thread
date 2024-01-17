#include <rtdef.h>

void button_event_thread(void *args);
void irq_callback(void *args);
rt_err_t button_event_running(void);
rt_err_t button_event_mq_init(void);
void reset_button_init(void);
