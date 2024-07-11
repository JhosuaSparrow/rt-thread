#include <stdio.h>
#include "rtthread.h"
#include "rtdevice.h"
#include "board.h"
#include "at.h"
#include "led.h"
#include "hc32_ll_efm.h"

rt_uint16_t atcmd_get_client_enable(rt_uint16_t client_id);
rt_err_t atcmd_set_client_enable(rt_uint16_t client_id, rt_uint16_t enable);
rt_err_t atcmd_client_init(void);
rt_err_t atcmd_client_send(char *cmd, rt_uint8_t client_id, rt_uint8_t recv_line_size, rt_uint8_t retry_count, at_response_t resp);
rt_err_t atcmd_reset(rt_uint8_t client_id, rt_uint8_t retry_count);
rt_err_t atcmd_query_imei(rt_uint8_t client_id, rt_uint8_t retry_count);
rt_err_t atcmd_query_vbat(rt_uint8_t client_id, rt_uint8_t retry_count);
at_result_t atcmd_server_find_setup(const char *args);
void atcmd_server_send_vbat(void *args);
rt_err_t atcmd_server_send_vbat_running(void);
void init_group_id(void);
rt_uint8_t init_group_item(rt_uint32_t u32uid, rt_uint8_t item);
