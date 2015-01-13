#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "tns_parse.h"

#ifdef HILI_DEBUG
int idpi_tns_parse_module_init() 
{ 
    char *helper_buffer = (char*)cvmx_bootmem_alloc(IDPI_tns_CORE_HELPER_BUFFER_LEN*16, 128);
    if (!helper_buffer)
    {
        /* char *syslog = "idpi_tns_parse_module_init(): tns module init failure. Out of mem\n"; */
        /* hili_send_module_send_syslog(syslog, sizeof(syslog)); */
        printf("%s(): tns module init failure. Out of mem.\n", __func__);
    }

    int i;
    for (i = 0; i < 16; ++i)
    {
        core_helper_buffer[i] = helper_buffer + IDPI_tns_CORE_HELPER_BUFFER_LEN*i;
    }

    firstline_parser[IDPI_tns_DIRECTION_REQUEST] = idpi_tns_parse_method_line;
    firstline_parser[IDPI_tns_DIRECTION_RESPONSE] = idpi_tns_parse_status_line;

     
    /* char syslog[100]; */
    /* sprintf(syslog, "tns module initting(compile time %s-%s)..........\n", __DATE__, __TIME__); */
    /* hili_send_module_send_syslog(syslog, sizeof(syslog)); */
    printf("tns module initting(compile time %s-%s)..........\n", __DATE__, __TIME__);
    assert(sizeof(idpi_tns_parser_t) < 2048);
    return 0;
}
#endif

void idpi_tns_context_init(idpi_tns_parser_t *psr)
{
    //psr->method = __IDPI_tns_METHOD_NULL;
    //psr->err_code = 0;
    //psr->header_remains[IDPI_tns_DIRECTION_REQUEST] = NULL;
    //psr->header_remains[IDPI_tns_DIRECTION_RESPONSE] = NULL;
    //psr->set_cookie = NULL;
    psr->segment_count = 0;
    //psr->cookie = NULL;
    //psr->url = NULL;
    //psr->content_type = NULL;
    //psr->content_encoding = NULL;
    //psr->chunked_encoding = 0;
    //psr->status_code = 0;

    int i;
    for (i = 1; i < IDPI_TNS_NUM_BACKUP_NOTES; ++i)
    {
        if (psr->backup_notes[i])
        {
            idpi_tns_free_memo(psr->backup_notes[i]);
            psr->backup_notes[i] = NULL;
        }
    }
    psr->backup_notes[0] = psr->main_notes;
    psr->notep = psr->main_notes;
    psr->curr_note = 0;
}

void* idpi_tns_parse_flow_init()//tns_parse_flow_conf_t *conf)
{
    idpi_tns_parser_t *psr = idpi_tns_alloc_parser();
    if (!psr) 
    {
        return NULL;
    }
    
    psr->logbuf_start = 0;
    psr->logbuf_curr = 0;
    //psr->parse_state = __IDPI_tns_PARSE_STATE_INIT;
    /*psr->client_ip = conf->cli_ip;
    psr->server_ip = conf->srv_ip;
    psr->client_port = conf->cli_port;
    psr->server_port = conf->srv_port;*/ 

    memset(psr->backup_notes, 0, sizeof(psr->backup_notes));
    idpi_tns_context_init(psr);

    return (void*)psr;
}

int idpi_tns_parse_kill_flow(void* tns_flow_ptr)
{
    idpi_tns_parser_t *psr = (idpi_tns_parser_t *)tns_flow_ptr;
    if (psr)
    {
        /*if (__IDPI_HTTP_PARSE_STATE_ERROR != psr->parse_state && psr->logbuf_start)
        {
            __idpi_http_finish_log(psr);
        }

        if (psr->logbuf_start) __idpi_http_free_logbuf(psr);*/
        idpi_tns_free_parser(tns_flow_ptr);
    }

    return 0;
}

int idpi_tns_parse_processing(idpi_tns_parser_t* tns_flow_ptr, void* buf, uint32_t buf_len, uint8_t direction)
{
    printf("##############################################\n");
    printf("%s(): buf_len is %u.\n", __func__, buf_len);
    
	idpi_tns_parser_t *psr = (idpi_tns_parser_t *)tns_flow_ptr;
    psr->direction = direction;
    idpi_tns_state_e tmp_parse_state = psr->parse_state;
    //assert(psr);

    psr->pktbuf_init = buf;
    psr->pktbuf_curr = buf;
    psr->pktbuf_left = buf_len;

    //printf("psr->pktbuf_curr1 is %p\n", psr->pktbuf_curr);
	//printf("psr->pktbuf_left1 is %u\n", psr->pktbuf_left);

    while(psr->pktbuf_left > 0)
    {
    	printf("##############################################\n");
    }
}

