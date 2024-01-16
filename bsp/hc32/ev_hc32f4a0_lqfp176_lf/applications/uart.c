/*
* @Author: Jack Sun
* @Date:   2024-01-12 14:11:40
* @Last Modified by:   jack.sun
* @Last Modified time: 2024-01-16 14:15:20
*/

#include <stdio.h>
#include <string.h>
#include <rtthread.h>
#include <uart.h>

#define UART_NUM          10
#define MQ_ITEM_SIZE      8

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

typedef struct
{
    rt_device_t serial;
    rt_size_t size;
} serial_recv_event_t;

typedef struct
{
    char name[8];
    rt_device_t serial;
} serial_device_t;

static struct rt_messagequeue serial_recv_mq;
static rt_uint8_t msg_pool[4096];
static serial_device_t serial_devices[UART_NUM] = {
    {UART1_NAME, RT_NULL},
    {UART2_NAME, RT_NULL},
    {UART3_NAME, RT_NULL},
    {UART4_NAME, RT_NULL},
    {UART5_NAME, RT_NULL},
    {UART6_NAME, RT_NULL},
    {UART7_NAME, RT_NULL},
    {UART8_NAME, RT_NULL},
    {UART9_NAME, RT_NULL},
    {UART10_NAME, RT_NULL}
};
static rt_uint8_t UART_ENAMBE=0;


rt_err_t set_uart_enable(rt_uint8_t uart_no, rt_uint8_t enable)
{
    if (enable)
    {
        UART_ENAMBE |= (1 << (uart_no - 1));
    }
    else
    {
        UART_ENAMBE &= (~(1 << (uart_no - 1)));
    }
    return RT_EOK;
}


rt_uint8_t get_uart_enable(rt_uint8_t uart_no)
{
    rt_uint8_t _enable_ = 0;
    _enable_ = UART_ENAMBE & (1 << (uart_no - 1));
    return _enable_;
}


/* 接收数据回调函数 */
rt_err_t uart_callback(rt_device_t dev, rt_size_t size)
{
    char msg[64];
    sprintf(msg, "uart_callback size %d\r\n", size);
    uart_write(msg, RT_NULL, dev);
    serial_recv_event_t buf = {dev, size};
    rt_err_t res = rt_mq_send(&serial_recv_mq, &buf, sizeof(buf));
    if (res != RT_EOK)
    {
        sprintf(msg, "rt_mq_send error %d\r\n", res);
        uart_write(msg, RT_NULL, dev);
    }
    else
    {
        sprintf(msg, "rt_mq_send ok %d\r\n", res);
        uart_write(msg, RT_NULL, dev);
    }
    return res;
}


void clean_str(char *data)
{
    for (rt_uint32_t i=0; i<sizeof(data); i++)
    {
        data[i] = '\0';
    }
}


void serial_thread_entry(void *parameter)
{
    serial_recv_event_t buf = {RT_NULL, RT_NULL};
    char msg[64];
    char recv_buf[64] = {0};
    rt_uint32_t count = 0;
    char char_code = '\0';
    rt_uint32_t recv_size = 0;
    while (1)
    {
        clean_str(recv_buf);
        rt_err_t res = rt_mq_recv(&serial_recv_mq, &buf, MQ_ITEM_SIZE, RT_WAITING_FOREVER);
        if (res == MQ_ITEM_SIZE)
        {
            sprintf(msg, "rt_mq_recv data res %d\r\n", res);
            uart_write(msg, RT_NULL, buf.serial);

            recv_size = rt_device_read(buf.serial, 0, &recv_buf, 64);
            sprintf(msg, "rt_mq_recv recv_size %d, recv_buf size %d, length %d\r\n", recv_size, sizeof(recv_buf), strlen(recv_buf));
            uart_write(msg, RT_NULL, buf.serial);

            // count = 0;
            // while (1)
            // {
            //     char_code = '\0';
            //     recv_size = rt_device_read(buf.serial, -1, &char_code, 1);
            //     sprintf(msg, "rt_mq_recv recv_size %d, char_code %c\r\n", recv_size, char_code);
            //     uart_write(msg, RT_NULL, buf.serial);
            //     if (recv_size == 1)
            //     {
            //         recv_buf[count] = char_code;
            //         count ++;
            //     }
            //     else
            //     {
            //         sprintf(msg, "rt_mq_recv over count %d, recv_buf %s, recv_buf length %d\r\n", count, recv_buf, strlen(recv_buf));
            //         uart_write(msg, RT_NULL, buf.serial);
            //         break;
            //     }
            // }
            uart_write(recv_buf, RT_NULL, buf.serial);
            sprintf(msg, "\r\nrt_mq_recv data over\r\n");
            uart_write(msg, RT_NULL, buf.serial);
        }
        else
        {
            sprintf(msg, "rt_mq_recv data failed %d serial %s\r\n", res, buf.serial);
            uart_write(msg, 5, RT_NULL);
        }
        rt_thread_mdelay(500);
    }
}


rt_err_t uart_init(char *uart_name)
{
    rt_device_t serial = rt_device_find(uart_name);

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
    rt_err_t res = rt_mq_init(&serial_recv_mq, "srcmq", &msg_pool[0], 32, sizeof(msg_pool), RT_IPC_FLAG_FIFO);
    if (res != RT_EOK) {
        char msg[] = "init message queue failed.\r\n";
        uart_write(msg, 5, RT_NULL);
        return RT_ERROR;
    }
    return RT_EOK;
}


rt_err_t uart_start_running(void)
{
    rt_err_t ret = RT_EOK;

    for (int i=0; i<UART_NUM; i++)
    {
        if (get_uart_enable(i + 1))
        {
            rt_err_t res = uart_init(serial_devices[i].name);
            if (res == RT_EOK) {
                serial_devices[i].serial = rt_device_find(serial_devices[i].name);
            }
        }
    }

    // 初始化消息队列
    serial_recv_mq_init();

    /* 创建 serial 线程 */
    rt_thread_t tid = rt_thread_create("serial", serial_thread_entry, RT_NULL, 0x4000, RT_THREAD_PRIORITY_MAX / 3, 10);
    /* 创建成功则启动线程 */
    if (tid != RT_NULL)
    {
        rt_err_t res = rt_thread_startup(tid);
        char data[32];
        sprintf(data, "rt_thread_startup %d\r\n", res);
        uart_write(data, 5, RT_NULL);
    }
    else
    {
        ret = RT_ERROR;
        char data[32];
        sprintf(data, "rt_thread_startup failed\r\n");
        uart_write(data, 5, RT_NULL);
    }

    return ret;
}


rt_err_t uart_write(char *msg, rt_uint8_t num, rt_device_t serial)
{
    if (num != RT_NULL && get_uart_enable(num + 1))
    {
        rt_device_write(serial_devices[num].serial, 0, msg, strlen(msg));
    }
    else if (serial != RT_NULL)
    {
        rt_device_write(serial, 0, msg, strlen(msg));
    }
}

void test_uart(void) {
    rt_uint32_t count=0;
    char data[14];
    while (count < 5) {
        for (rt_uint8_t i=0; i<10; i++)
        {
            if (get_uart_enable(i + 1) && serial_devices[i].serial != RT_NULL)
            {
                sprintf(data, "%s %d \r\n", "test uart", i);
                uart_write(data, i, RT_NULL);
            }
        }
        rt_thread_mdelay(300);
        count ++;
    }
}
