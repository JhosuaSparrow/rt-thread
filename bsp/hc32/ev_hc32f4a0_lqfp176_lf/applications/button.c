/*
* @Author: Jack Sun
* @Date:   2024-01-16 14:36:02
* @Last Modified by:   jack.sun
* @Last Modified time: 2024-01-18 10:18:07
*/

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <button.h>
#include <atcmd.h>

#define LOG_TAG             "lf.button"
#include <lf_log.h>

#define RESET_BUTTON_NUM          GET_PIN(D, 10)

static struct rt_semaphore btnrst_sem;
// static rt_sem_t btnrst_sem;
static rt_uint8_t reset_running = 0;


void button_event_thread(void *args)
{
    rt_err_t recv_res, reset_res;
    rt_uint8_t retry = 3;
    while (1)
    {
        recv_res = rt_sem_take(&btnrst_sem, RT_WAITING_FOREVER);
        LOG_D("recev btnrst_sem");
        if (recv_res == RT_EOK)
        {
            for (rt_uint8_t i = 0; i < 10; i++)
            {
                for (rt_uint8_t j = 0; j < retry; j++)
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
            reset_running = 0;
        }
    }
}


void irq_callback(void *args)
{
    LOG_D("enter gpio irq.");
    if(rt_pin_read(RESET_BUTTON_NUM) == 0)
    {
        rt_pin_irq_enable(RESET_BUTTON_NUM, PIN_IRQ_DISABLE);
        rt_hw_ms_delay(20);
        if(rt_pin_read(RESET_BUTTON_NUM) == 0) {
            LOG_D("button down irq.");
            if (reset_running == 0)
            {
                reset_running = 1;
                rt_err_t res = rt_sem_release(&btnrst_sem);
                if (res == RT_EOK)
                {
                    LOG_D("rt_sem_release success.");
                }
                else
                {
                    LOG_E("rt_sem_release faild.");
                    reset_running = 0;
                }
            }
            else
            {
                LOG_D("atcmd reset is running.");
            }
        }
        rt_pin_irq_enable(RESET_BUTTON_NUM, PIN_IRQ_ENABLE);
    }
}


rt_err_t button_event_running(void)
{
    rt_thread_t tid;
    rt_err_t ret = RT_EOK;

    tid = rt_thread_create("btn", button_event_thread, RT_NULL, 0x2000, RT_THREAD_PRIORITY_MAX / 3, 20);
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


void reset_button_init(void)
{
    rt_sem_init(&btnrst_sem, "btnrst", 0, RT_IPC_FLAG_FIFO);
    // btnrst_sem = rt_sem_create("btnrst", 0, RT_IPC_FLAG_FIFO);
    button_event_running();
    rt_pin_mode(RESET_BUTTON_NUM, PIN_MODE_INPUT_PULLUP);
    rt_pin_attach_irq(RESET_BUTTON_NUM, PIN_IRQ_MODE_FALLING, irq_callback, RT_NULL);
    rt_pin_irq_enable(RESET_BUTTON_NUM, PIN_IRQ_ENABLE);
}
