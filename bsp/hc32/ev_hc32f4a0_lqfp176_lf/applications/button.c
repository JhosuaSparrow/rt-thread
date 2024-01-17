/*
* @Author: Jack Sun
* @Date:   2024-01-16 14:36:02
* @Last Modified by:   jack.sun
* @Last Modified time: 2024-01-17 17:32:14
*/

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <button.h>
#include <atcmd.h>

#define LOG_TAG             "lf.button"
#include <lf_log.h>

#define RESET_BUTTON_NUM          GET_PIN(D, 10)

static struct rt_messagequeue button_event_mq;
static rt_uint8_t btnmq_msg_pool[16];
static rt_uint8_t reset_running = 0;


void button_event_thread(void *args)
{
    rt_uint8_t tag;
    rt_err_t recv_res, reset_res;
    rt_uint8_t retry = 3;
    while (1)
    {
        tag = 0;
        recv_res = rt_mq_recv(&button_event_mq, &tag, 1, RT_WAITING_FOREVER);
        LOG_D("rt_mq_recv tag %d", tag);
        if (recv_res == 1)
        {
            for (rt_uint8_t i=0; i<10; i++)
            {
                for (rt_uint8_t j=0; j<retry; j++)
                {
                    reset_res = atcmd_reset(i, 3);
                    LOG_D("atcmd_reset client_id %d reset_res %d", i, reset_res);
                    if (reset_res == RT_EOK)
                    {
                        break;
                    }
                    rt_thread_mdelay(100);
                }
            }
        }
        reset_running = 0;
    }
}


void irq_callback(void *args)
{
    LOG_D("button down irq.");
    if (reset_running == 0)
    {
        rt_uint8_t tag = 1;
        reset_running = 1;
        rt_err_t res = rt_mq_send(&button_event_mq, &tag, sizeof(tag));
        if (res != RT_EOK)
        {
            LOG_E("button_event_mq send faild.");
            reset_running = 0;
        }
        else
        {
            LOG_D("button_event_mq send success.");
        }
    }
    else
    {
        LOG_D("reset is running.");
    }
}


rt_err_t button_event_running(void)
{
    rt_thread_t tid;
    rt_err_t ret = RT_EOK;

    tid = rt_thread_create("btn", button_event_thread, RT_NULL, 2048, RT_THREAD_PRIORITY_MAX / 3, 20);
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


rt_err_t button_event_mq_init(void)
{
    rt_err_t res = rt_mq_init(&button_event_mq, "btnmq", &btnmq_msg_pool[0], 8, sizeof(btnmq_msg_pool), RT_IPC_FLAG_FIFO);
    if (res != RT_EOK)
    {
        LOG_E("init button_event_mq failed. res %d", res);
    }
    else
    {
        LOG_D("init button_event_mq success. res %d", res);
    }
    return res;
}


void reset_button_init(void)
{
    button_event_mq_init();
    button_event_running();
    rt_pin_mode(RESET_BUTTON_NUM, PIN_MODE_INPUT_PULLUP);
    rt_pin_attach_irq(RESET_BUTTON_NUM, PIN_IRQ_MODE_FALLING, irq_callback, RT_NULL);
    rt_pin_irq_enable(RESET_BUTTON_NUM, PIN_IRQ_ENABLE);
}
