/*
* @Author: Jack Sun
* @Date:   2024-01-12 14:11:40
* @Last Modified by:   jack.sun
* @Last Modified time: 2024-01-13 19:41:22
*/

#include <rtthread.h>
#include <uart.h>

#define UART1_NAME        "uart1"
#define UART2_NAME        "uart2"
#define UART3_NAME        "uart3"
#define UART4_NAME        "uart4"
#define UART5_NAME        "uart5"
#define UART6_NAME        "uart6"
#define UART7_NAME        "uart7"
#define UART8_NAME        "uart8"
#define UART9_NAME        "uart9"
#define UART10_NAME       "uart10"

static rt_device_t serial_1;
static rt_device_t serial_2;
static rt_device_t serial_3;
static rt_device_t serial_4;
static rt_device_t serial_5;
static rt_device_t serial_6;
static rt_device_t serial_7;
static rt_device_t serial_8;
static rt_device_t serial_9;
static rt_device_t serial_10;

typedef struct
{
    rt_device_t serial;
    rt_size_t size;
} serial_recv_event_t;

typedef struct
{
    char name[6];
    rt_device_t serial;
} serial_device_t;


static struct rt_messagequeue serial_recv_mq;
static rt_uint8_t msg_pool[4096];


/* 接收数据回调函数 */
rt_err_t uart_callback(rt_device_t dev, rt_size_t size)
{
    serial_recv_event_t buf = {dev, size};
    rt_err_t res = rt_mq_send(&serial_recv_mq, &buf, sizeof(buf));
    if (res != RT_EOK)
    {
        rt_kprintf("message queue send failed.\n");
    }
    return RT_EOK;
}


void serial_thread_entry(void *parameter)
{
    serial_recv_event_t buf;
    while (1)
    {
        if (rt_mq_recv(&serial_recv_mq, &buf, sizeof(buf), RT_WAITING_FOREVER) == RT_EOK)
        {
            char recv_buf[64];
            rt_device_read(buf.serial, -1, &recv_buf, 64);
            rt_device_write(buf.serial, 0, &recv_buf, sizeof(recv_buf));
        }
        rt_thread_mdelay(500);
    }
}


rt_err_t uart_init(char *uart_name, rt_device_t serial)
{
    serial = rt_device_find(uart_name);

    if (!serial)
    {
        rt_kprintf("find %s failed!\n", uart_name);
        return RT_ERROR;
    }
    /* 以中断接收及轮询发送方式打开串口设备 */
    rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);
    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(serial, uart_callback);

    return RT_EOK;
}


rt_err_t serial_recv_mq_init(void)
{
    rt_err_t res = rt_mq_init(&serial_recv_mq, "srcmq", &msg_pool[0], 8, sizeof(msg_pool), RT_IPC_FLAG_FIFO);
    if (res != RT_EOK) {
        rt_kprintf("init message queue failed.\n");
        return RT_ERROR;
    }
    return RT_EOK;
}


rt_err_t uart_start_running(void)
{
    rt_err_t ret = RT_EOK;

    serial_device_t sreial_devices[10] = {
        {UART1_NAME, serial_1},
        {UART2_NAME, serial_2},
        {UART3_NAME, serial_3},
        {UART4_NAME, serial_4},
        {UART5_NAME, serial_5},
        {UART6_NAME, serial_6},
        {UART7_NAME, serial_7},
        {UART8_NAME, serial_8},
        {UART9_NAME, serial_9},
        {UART10_NAME, serial_10}
    };

    for (int i=0; i<10; i++)
    {

        uart_init(sreial_devices[i].name, sreial_devices[i].serial);
    }

    // 初始化消息队列
    serial_recv_mq_init();

    /* 创建 serial 线程 */
    rt_thread_t tid = rt_thread_create("sreial", serial_thread_entry, RT_NULL, 0x4000, RT_THREAD_PRIORITY_MAX / 3, 10);
    /* 创建成功则启动线程 */
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
