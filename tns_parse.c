#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "tns_parser.h"
#define IDPI_TNS_CORE_HELPER_BUFFER_LEN 4096
#define IDPI_TNS_DIRECTION_REQUEST (0)
#define IDPI_TNS_DIRECTION_RESPONSE (1)

#define IDPI_LOG_MAX_LENGTH (1456 - HILI_LOG_FIXED_LENGTH - USER_DEFINED_FIXED_LENGTH)

typedef struct
{
    idpi_tns_state_e parse_state;
    idpi_tns_version_e tns_version;
    idpi_tns_content_type_e content_type;
    uint8_t direction;
//    uint16_t client_port;
//    uint16_t server_port;
//  uint32_t segment_count; /*log segment number*/
 //   uint32_t client_ip;
 //   uint32_t server_ip;
 //   uint32_t pktbuf_left; /*bytes to read in pkt buffer*/
	uint32_t content_left; /*remaining bytes of message-body to read*/
    uint32_t content_current; /*remaining bytes of message-body to read*/
 //   hili_send_packet_conf_t send_conf;
    char *request_header[TNS_HEADER_LENGTH-1]; /*restore the current header */
    char *response_header[TNS_HEADER_LENGTH-1]; /*restore the current header */
    char *logbuf_start; /*start addr of log buffer*/
    char *logbuf_curr; /*next addr to write in log buffer*/
    char *pktbuf_curr; /*next addr to read in pkt buffer*/
//  char *notep; /*current pos in current note*/
    char tns_data_cache_block[IDPI_TNS_MEMO_LEN_LIMIT]; /*memo*/
}idpi_tns_parser_t;

#ifdef TNS_HILI_INTERFACE
	/* firstline_parser ??
	 *
	 * 
	 */
	typedef int (*pkt_parser_func)(idpi_tns_parser_t *psr);
	CVMX_SHARED static char *core_helper_buffer[16];
	CVMX_SHARED static pkt_parser_func firstline_parser[2];
	static int idpi_tns_parse_sql_line(idpi_tns_parser_t *psr);
	static int idpi_tns_parse_state_line(idpi_tns_parser_t *psr);

	/* cvmx_fpa_alloc(0);
	 *
	 * 
	 */
	static inline void *idpi_tns_alloc_parser()
	{
	    return cvmx_fpa_alloc(0);
	}

	/* Get the exclusive buffer of current core.
	 *
	 * Reducing the performance loss caused by synchronization among cores.
	 *
	 */
	static inline char *idpi_tns_get_core_helper_buffer() 
	{
	    return core_helper_buffer[cvmx_get_core_num()];
	}

	/* helper_buffer func?
	 *
	 * 
	 */
	int idpi_tns_parse_module_init() 
	{ 
	    char *helper_buffer = (char*)cvmx_bootmem_alloc(IDPI_TNS_CORE_HELPER_BUFFER_LEN*16, 128);
	    if (!helper_buffer)
	    {
	        printf("%s: tns module init failure. Out of mem\n", __FUNCTION__);
	    }

	    int i;
	    for (i = 0; i < 16; ++i)
	    {
	        core_helper_buffer[i] = helper_buffer + IDPI_TNS_CORE_HELPER_BUFFER_LEN*i;
	    }

	    firstline_parser[IDPI_HTTP_DIRECTION_REQUEST] = idpi_tns_parse_sql_line;
	    firstline_parser[IDPI_HTTP_DIRECTION_RESPONSE] = idpi_tns_parse_state_line;

	    printf("tns module initting(compile time %s-%s)..........\n", __DATE__, __TIME__);
	    assert(sizeof(idpi_tns_parser_t) < 2048);
	    return 0;
	}
	/* 
	 *
	 * 
	 */
	int idpi_tns_parse_module_finalize() 
	{ 
	    return 0;
	}
	/* 
	 *
	 * 
	 */
	void* idpi_tns_parse_flow_init(tns_parse_flow_conf_t *conf)
	{
	    idpi_tns_parser_t *psr = idpi_tns_alloc_parser();
	    if (!psr) 
	    {
	        return NULL;
	    }
	    
	    psr->logbuf_start = 0;
	    psr->logbuf_curr = 0;
	   // psr->parse_state = __IDPI_HTTP_PARSE_STATE_INIT;
	   
	    psr->client_ip = conf->cli_ip;
	    psr->server_ip = conf->srv_ip;
	    psr->client_port = conf->cli_port;
	    psr->server_port = conf->srv_port; 
	    psr->send_conf.src_port = conf->log_cli_port;
	    psr->send_conf.src_ip = conf->log_cli_ip;
	    psr->send_conf.dst_port = conf->log_srv_port;
	    psr->send_conf.dst_ip = conf->log_srv_ip;

	   // memset(psr->backup_notes, 0, sizeof(psr->backup_notes));
	   // __idpi_tns_note_init(psr);

	    return (void*)psr;
	}

	static int idpi_tns_parse_sql_line(idpi_tns_parser_t *psr)
	{
		return 0;
	}
	static int idpi_tns_parse_state_line(idpi_tns_parser_t *psr);
	{
		return 0;
	}
#else
	/**
	 * @brief Test if text starts with prefix
	 *
	 * @param text To be matched
	 * @param prefix Must be terminated by \0
	 *
	 * @return text if matched, 0 otherwise 
	 */
	static char *idpi_tns_match_prefix(char *text, char *prefix)
	{
	    char *p = text, *q = prefix;
	    while (*q != '\0')
	    {
	        if (*p != *q) 
	        {
	            return NULL;
	        }
	        p++, q++;
	    }
	    return text;
	}
	/* 
	 *
	 * 
	 */
	 __idpi_tns_cache_init(idpi_tns_parser_t *psr)
	{
	    
	}
	/* 
	 *
	 * 
	 */
	 __idpi_tns_xxx_init(idpi_tns_parser_t *psr)
	{
	    
	}
	/* 
	 *
	 * 
	 */
	int idpi_tns_parse_processing(void* tns_flow_ptr, void* buf, uint32_t buf_len, uint8_t direction)
	{
	    idpi_tns_parser_t *psr = (idpi_tns_parser_t *)tns_flow_ptr;
	    //assert(psr);
		if (direction != IDPI_TNS_DIRECTION_RESPONSE) /* flow direction change */
		    {
		        //__idpi_http_finish_log(psr); /* if unsent log buffer present, send it. if returns error, ignore */
		        if (IDPI_HTTP_DIRECTION_REQUEST == direction)
		        {
		            /* a new request arrives before the last response is finished */
		            /* We haven't supported HTTP pipelining yet, as for most browser disable this feature by default. */
		            __idpi_tns_note_init(psr);
		        }
		        psr->parse_state = __IDPI_HTTP_PARSE_STATE_INIT; /* if in error state, start over */
		        psr->direction = direction;
		    }

	}

#endif