/*
* @Author: Jack Sun
* @Date:   2024-01-08 14:07:06
* @Last Modified by:   jack.sun
* @Last Modified time: 2024-01-12 11:32:15
*/

#include <rtthread.h>

rt_err_t uart_callback(rt_device_t dev, rt_size_t size);
void serial_thread_entry(void *parameter);
rt_err_t uart_init(char *uart_name, rt_device_t serial);
rt_err_t serial_recv_mq_init(void);
rt_err_t uart_start_running(void);
