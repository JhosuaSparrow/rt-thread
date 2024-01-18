/*
* @Author: Jack Sun
* @Date:   2024-01-16 19:47:45
* @Last Modified by:   jack.sun
* @Last Modified time: 2024-01-18 10:13:14
*/

#include <string.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <at.h>
#include <led.h>

#define LOG_TAG             "lf.atcmd"
#include <lf_log.h>

#define AT_CLIENT_BUFSZ     512
#define AT_CLIENT_TIMEOUT   3000

#define GROUP_ID          "UNIQUE_ID"

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
    char *name;
    rt_uint8_t id;
    char imei[16];
    char vbat[16];
    char level[8];
    at_client_t client;
} at_usart_client_t;


static rt_uint8_t CLINET_ENABLE = 0;

static at_usart_client_t USART_CLIENTS[10] = {
    {UART1_NAME, 1, RT_NULL, RT_NULL, RT_NULL},
    {UART2_NAME, 2, RT_NULL, RT_NULL, RT_NULL},
    {UART3_NAME, 3, RT_NULL, RT_NULL, RT_NULL},
    {UART4_NAME, 4, RT_NULL, RT_NULL, RT_NULL},
    {UART5_NAME, 5, RT_NULL, RT_NULL, RT_NULL},
    {UART6_NAME, 6, RT_NULL, RT_NULL, RT_NULL},
    {UART7_NAME, 7, RT_NULL, RT_NULL, RT_NULL},
    {UART8_NAME, 8, RT_NULL, RT_NULL, RT_NULL},
    {UART9_NAME, 9, RT_NULL, RT_NULL, RT_NULL},
    {UART10_NAME, 10, RT_NULL, RT_NULL, RT_NULL}
};


rt_err_t atcmd_set_client_enable(rt_uint8_t client_id, rt_uint8_t enable)
{
    if (enable)
    {
        CLINET_ENABLE |= (1 << (client_id - 1));
    }
    else
    {
        CLINET_ENABLE &= (~(1 << (client_id - 1)));
    }
    return RT_EOK;
}


rt_uint8_t atcmd_get_client_enable(rt_uint8_t client_id)
{
    rt_uint8_t _enable_ = 0;
    _enable_ = CLINET_ENABLE & (1 << (client_id - 1));
    return _enable_;
}


rt_err_t atcmd_client_init(void)
{
    for (rt_uint8_t i=0; i<10; i++)
    {
        if (atcmd_get_client_enable(i + 1))
        {
            LOG_D("USART_CLIENTS[i].name %s", USART_CLIENTS[i].name);
            rt_err_t res = at_client_init(USART_CLIENTS[i].name, AT_CLIENT_BUFSZ, AT_CLIENT_BUFSZ);
            if (res == RT_EOK)
            {
                USART_CLIENTS[i].client = at_client_get(USART_CLIENTS[i].name);
                at_obj_set_end_sign(USART_CLIENTS[i].client, '\n');
            }
        }
    }
}


rt_err_t atcmd_client_send(char *cmd, rt_uint8_t client_id, rt_uint8_t recv_line_size, rt_uint8_t retry_count, at_response_t resp)
{
    rt_err_t res = RT_ERROR;
    if (!atcmd_get_client_enable(client_id + 1)) {
        return RT_ERROR;
    }
    if (retry_count == RT_NULL)
    {
        retry_count = 3;
    }
    at_resp_set_info(resp, AT_CLIENT_BUFSZ, 0, AT_CLIENT_TIMEOUT);
    for (rt_uint8_t i=0; i<retry_count; i++)
    {
        res = at_obj_exec_cmd(USART_CLIENTS[client_id].client, resp, cmd);
        LOG_D("client cmd %s, at_obj_exec_cmd res %d, resp->buf %s, retry_count %d", cmd, res, resp->buf, i);
        if (res == RT_EOK)
        {
            break;
        }

    }
    return res;
}


rt_err_t atcmd_reset(rt_uint8_t client_id, rt_uint8_t retry_count)
{
    char cmd[] = "AT+RESET";
    at_response_t resp = at_create_resp(AT_CLIENT_BUFSZ, 0, AT_CLIENT_TIMEOUT);
    rt_err_t res = atcmd_client_send(cmd, client_id, 0, 3, resp);
    at_delete_resp(resp);
    return res;
}


rt_err_t atcmd_query_imei(rt_uint8_t client_id, rt_uint8_t retry_count)
{

    char cmd[] = "AT+IMEI?";
    at_response_t resp = at_create_resp(AT_CLIENT_BUFSZ, 0, AT_CLIENT_TIMEOUT);
    rt_err_t res = atcmd_client_send(cmd, client_id, 3, 3, resp);
    if (res == RT_EOK)
    {
        at_resp_parse_line_args(resp, 1,"+IMEI:%s", USART_CLIENTS[client_id].imei);
        LOG_D("USART_CLIENTS[client_id].imei %s", USART_CLIENTS[client_id].imei);
    }
    at_delete_resp(resp);
    return res;
}


rt_err_t atcmd_query_vbat(rt_uint8_t client_id, rt_uint8_t retry_count)
{

    char cmd[] = "AT+VBAT?";
    at_response_t resp = at_create_resp(AT_CLIENT_BUFSZ, 0, AT_CLIENT_TIMEOUT);
    rt_err_t res = atcmd_client_send(cmd, client_id, 3, 3, resp);
    if (res == RT_EOK)
    {
        at_resp_parse_line_args(resp, 1, "%*[^:]:%[^,],%[^,],%s", USART_CLIENTS[client_id].imei, USART_CLIENTS[client_id].vbat, USART_CLIENTS[client_id].level);
        LOG_D("client_id %d, imei %s, vbat %s, level %s", client_id, USART_CLIENTS[client_id].imei, USART_CLIENTS[client_id].vbat, USART_CLIENTS[client_id].level);
    }
    at_delete_resp(resp);
    return res;
}


void atcmd_clients_query_vbat(void)
{
    for (rt_uint8_t i=0; i<10; i++)
    {
        atcmd_query_vbat(i, 3);
    }
}

at_result_t atcmd_server_find_setup(const char *args)
{
    rt_err_t res = AT_RESULT_FAILE;
    char group_id[16];
    char imei[16];
    char chr_enable[1];
    rt_uint8_t enable;
    if (at_req_parse_args(args, "=%[^,],%[^,],%s", group_id, imei, chr_enable) > 0)
    {
        enable = (chr_enable[0] == '1');

        LOG_D("find args %s group_id %s, imei %s, enable %d", args, group_id, imei, enable);
        if (strcmp(group_id, GROUP_ID) == 0)
        {
            for (rt_uint8_t i=0; i<10; i++)
            {
                if (strcmp(USART_CLIENTS[i].imei, imei) == 0 || i == 1)
                {
                    res = set_led_enable(USART_CLIENTS[i].id, enable);
                    LOG_D("set_led_enable led no %d, enable %d, res %d", USART_CLIENTS[i].id, enable, res);
                }
            }
        }
    }
    if (res == RT_EOK)
    {
        res = AT_RESULT_OK;
    }
    return res;
}

AT_CMD_EXPORT("AT+FIND", "=<group_id>,<imei>,<enable>", RT_NULL, RT_NULL, atcmd_server_find_setup, RT_NULL);


void atcmd_server_send_vbat(void *args)
{
    while (1)
    {
        for (rt_uint8_t i=0; i<10; i++)
        {
            at_server_printfln("+VBAT:%s,%d,%s,%s,%s", GROUP_ID, USART_CLIENTS[i].id, USART_CLIENTS[i].imei, USART_CLIENTS[i].vbat, USART_CLIENTS[i].level);
            rt_thread_mdelay(100);
        }
        rt_thread_mdelay(60 * 1000);
    }
}


rt_err_t atcmd_server_send_vbat_running(void)
{
    rt_thread_t tid;
    rt_err_t ret = RT_EOK;

    tid = rt_thread_create("sendvbat", atcmd_server_send_vbat, RT_NULL, 0x2000, RT_THREAD_PRIORITY_MAX / 3, 20);
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
