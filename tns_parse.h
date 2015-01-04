#ifndef HILI_IPC_HTTP_PARSER_H
#define HILI_IPC_HTTP_PARSER_H

#include <stddef.h> 
#ifdef LINUX_SIM
#include "arpa/inet.h"
#include "HiliLinuxHelper.h"
#include "tnmc-hili-infsim.h"
#else
#include "cvmx.h"
#include "cvmx-packet.h"
#include "cvmx-pko.h"
#include "cvmx-fpa.h"
#include "cvmx-fau.h"
#include "cvmx-wqe.h"
#include "cvmx-spinlock.h"
#include "cvmx-malloc.h"
#include "cvmx-bootmem.h"
#include "cvmx-coremask.h"
#include "cvmx-sysinfo.h"
#include "hili_send_module.h"
#endif

#include "hili_send_module.h"
#include "hili_http_parse_module.h"

#define TNS_HILI_INTERFACE
#ifdef TNS_HILI_INTERFACE

typedef struct{
    uint16_t cli_port;
    uint16_t srv_port;
    uint32_t cli_ip;
    uint32_t srv_ip;

    uint16_t log_cli_port;
    uint32_t log_cli_ip;
    uint16_t log_srv_port;
    uint32_t log_srv_ip;
}tns_parse_flow_conf_t;

typedef struct
{
    idpi_tns_state_e parse_state;
    idpi_tns_version_e tns_version;
    idpi_tns_content_type_e content_type;
    uint8_t direction;
    uint16_t client_port;
    uint16_t server_port;
//  uint32_t segment_count; /*log segment number*/
    uint32_t client_ip;
    uint32_t server_ip;
    uint32_t pktbuf_left; /*bytes to read in pkt buffer*/
	uint32_t content_left; /*remaining bytes of message-body to read*/
    uint32_t content_current; /*remaining bytes of message-body to read*/
    hili_send_packet_conf_t send_conf;
    char *request_header[TNS_HEADER_LENGTH-1]; /*restore the current header */
    char *response_header[TNS_HEADER_LENGTH-1]; /*restore the current header */
    char *logbuf_start; /*start addr of log buffer*/
    char *logbuf_curr; /*next addr to write in log buffer*/
    char *pktbuf_curr; /*next addr to read in pkt buffer*/
//  char *notep; /*current pos in current note*/
    char tns_data_cache_block[IDPI_TNS_MEMO_LEN_LIMIT]; /*memo*/
}idpi_tns_parser_t;


typedef enum
{
	IDPI_TNS_VERSION_312,
    IDPI_TNS_VERSION_313,
    IDPI_TNS_VERSION_314,
    IDPI_TNS_VERSION_315,
}idpi_tns_version_e;

#else

typedef enum
{
	__IDPI_TNS_PARSE_STATE_INIT = 0, 或会话结束后
	__IDPI_TNS_PARSE_STATE_CONNECTING, 
	__IDPI_TNS_PARSE_STATE_CONNECTED, 
	__IDPI_TNS_PARSE_STATE_REDIRECTED, 
	__IDPI_TNS_PARSE_STATE_ERROR_MARKER, 整
	__IDPI_TNS_PARSE_STATE_REQUESTING, 
	__IDPI_TNS_PARSE_STATE_REQUESTED, 
	__IDPI_TNS_PARSE_STATE_RESPONDING, 不完整
	__IDPI_TNS_PARSE_STATE_RESPONDED, 
}idpi_tns_state_e;

int idpi_tns_parse_module_init();
void *idpi_tns_parse_flow_init(tns_parse_flow_conf_t *tns_parse_flow_conf);
int idpi_tns_parse_processing(void* tns_flow_ptr, void* buf, uint32_t buf_len, uint8_t direction);

#endif



































































































































































































































