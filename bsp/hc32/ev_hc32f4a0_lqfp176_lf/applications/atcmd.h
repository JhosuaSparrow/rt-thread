#include <rtdef.h>
#include <at.h>

rt_err_t atcmd_set_client_enable(rt_uint8_t client_id, rt_uint8_t enable);
rt_uint8_t atcmd_get_client_enable(rt_uint8_t client_id);
rt_err_t atcmd_client_init(void);
rt_err_t atcmd_client_send(char *cmd, rt_uint8_t client_id, rt_uint8_t recv_line_size, rt_uint8_t retry_count, at_response_t resp);
rt_err_t atcmd_reset(rt_uint8_t client_id, rt_uint8_t retry_count);
rt_err_t atcmd_query_imei(rt_uint8_t client_id, rt_uint8_t retry_count);
rt_err_t atcmd_query_vbat(rt_uint8_t client_id, rt_uint8_t retry_count);
void atcmd_clients_query_vbat(void);
at_result_t atcmd_server_find_setup(const char *args);
void atcmd_server_send_vbat(void *args);
rt_err_t atcmd_server_send_vbat_running(void);
