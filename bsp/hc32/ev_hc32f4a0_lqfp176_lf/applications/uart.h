/*
* @Author: Jack Sun
* @Date:   2024-01-08 14:07:06
* @Last Modified by:   jack.sun
* @Last Modified time: 2024-01-12 11:32:15
*/

#include <rtdef.h>

rt_err_t set_uart_enable(rt_uint8_t uart_no, rt_uint8_t enable);
rt_uint8_t get_uart_enable(rt_uint8_t uart_no);
rt_err_t uart_callback(rt_device_t dev, rt_size_t size);
void serial_thread_entry(void *parameter);
rt_err_t uart_init(char *uart_name);
void clean_str(char *data);
rt_err_t serial_recv_mq_init(void);
rt_err_t uart_start_running(void);
rt_err_t uart_write(char *msg, rt_uint8_t num, rt_device_t sreial);
void test_uart(void);
