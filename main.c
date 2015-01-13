#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "tns_parse.h"
#include "test_buffer.h"


int main()
{	

	idpi_tns_parser_t *ptr = (idpi_tns_parser_t *)idpi_tns_parse_flow_init();
	
	idpi_tns_parse_processing(ptr, (void *)buf1, buf1_len, DIR_REQUEST);

	idpi_tns_parse_kill_flow(ptr);

	return 0;
}

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

void idpi_tns_context_init(idpi_tns_parser_t *tns_flow_ptr)
{
	idpi_tns_parser_t *ptr = tns_flow_ptr;
    //ptr->method = __IDPI_tns_METHOD_NULL;
    //ptr->err_code = 0;
    //ptr->header_remains[IDPI_tns_DIRECTION_REQUEST] = NULL;
    //ptr->header_remains[IDPI_tns_DIRECTION_RESPONSE] = NULL;
    //ptr->set_cookie = NULL;
    ptr->segment_count = 0;
    //ptr->cookie = NULL;
    //ptr->url = NULL;
    //ptr->content_type = NULL;
    //ptr->content_encoding = NULL;
    //ptr->chunked_encoding = 0;
    //ptr->status_code = 0;

    int i;
    for (i = 1; i < IDPI_TNS_NUM_BACKUP_NOTES; ++i)
    {
        if (ptr->backup_notes[i])
        {
            idpi_tns_free_memo(ptr->backup_notes[i]);
            ptr->backup_notes[i] = NULL;
        }
    }
    ptr->backup_notes[0] = ptr->main_notes;
    ptr->notep = ptr->main_notes;
    ptr->curr_note = 0;
}

void* idpi_tns_parse_flow_init()//tns_parse_flow_conf_t *conf)
{
    idpi_tns_parser_t *ptr = idpi_tns_alloc_parser();

    if (!ptr) 
    {
        printf("error\n");
        return NULL;
    }
    
    ptr->logbuf_start = 0;
    ptr->logbuf_curr = 0;
    ptr->wait_flag = WAIT_FOR_BUFFER;
    //ptr->parse_state = __IDPI_tns_PARSE_STATE_INIT;
    /*ptr->client_ip = conf->cli_ip;
    ptr->server_ip = conf->srv_ip;
    ptr->client_port = conf->cli_port;
    ptr->server_port = conf->srv_port;*/ 

    memset(ptr->backup_notes, 0, sizeof(ptr->backup_notes));
    idpi_tns_context_init(ptr);

    return (void*)ptr;
}

int idpi_tns_parse_kill_flow(void* tns_flow_ptr)
{
    idpi_tns_parser_t *ptr = (idpi_tns_parser_t *)tns_flow_ptr;
    if (ptr)
    {
        /*if (__IDPI_HTTP_PARSE_STATE_ERROR != ptr->parse_state && ptr->logbuf_start)
        {
            __idpi_http_finish_log(ptr);
        }

        if (ptr->logbuf_start) __idpi_http_free_logbuf(ptr);*/
        idpi_tns_free_parser(ptr);
    }

    return 0;
}

int idpi_tns_parse_pkt_length(idpi_tns_parser_t* tns_flow_ptr)
{
	idpi_tns_parser_t *ptr = tns_flow_ptr;

	ptr->tns_pkt_length = ptr->main_notes[0] << 8;
    ptr->tns_pkt_length += ptr->main_notes[1];

    if(ptr->tns_pkt_length == 0) 
    {
        //tns 315
        ptr->tns_pkt_length = ptr->main_notes[2]<<8;
        ptr->tns_pkt_length += ptr->main_notes[3];
        
        return 0;
    }

    return 0;
}

int idpi_tns_parse_cache_message(idpi_tns_parser_t* tns_flow_ptr)
{
	idpi_tns_parser_t *ptr = tns_flow_ptr;

	if(ptr)
	{
		if(ptr->pktbuf_left >= IDPI_TNS_MEMO_LEN_LIMIT)
		{
			ptr->cached_num += ptr->pktbuf_left;
		
			memcpy(ptr->notep, ptr->pktbuf_curr, IDPI_TNS_MEMO_LEN_LIMIT);
			ptr->pktbuf_curr += IDPI_TNS_MEMO_LEN_LIMIT;
   			ptr->pktbuf_left -= IDPI_TNS_MEMO_LEN_LIMIT;
   			
			while(ptr->pktbuf_left >= IDPI_TNS_MEMO_LEN_LIMIT)
			{
                ptr->curr_note++;
                
                if(ptr->curr_note == IDPI_TNS_NUM_BACKUP_NOTES )
                {
                    return -1;
                }
                
	            if(ptr->backup_notes[ptr->curr_note] == NULL)
	            {
	                ptr->backup_notes[ptr->curr_note] = idpi_tns_alloc_memo();
	                
	                if(ptr->backup_notes[ptr->curr_note] == NULL)
	                {
                        printf("error\n");
	                    return -1;//add error code
	                }
	            }
	            ptr->notep = ptr->backup_notes[ptr->curr_note];

	            memcpy(ptr->notep, ptr->pktbuf_curr, IDPI_TNS_MEMO_LEN_LIMIT);
				ptr->pktbuf_curr += IDPI_TNS_MEMO_LEN_LIMIT;
	   			ptr->pktbuf_left -= IDPI_TNS_MEMO_LEN_LIMIT;
                
			}

            ptr->curr_note++;
            
            if(ptr->backup_notes[ptr->curr_note] == NULL)
                {
                    ptr->backup_notes[ptr->curr_note] = idpi_tns_alloc_memo();
                    
                    if(ptr->backup_notes[ptr->curr_note] == NULL)
                    {
                        return -1;//add error code
                    }
                }
            ptr->notep = ptr->backup_notes[ptr->curr_note];

			memcpy(ptr->notep, ptr->pktbuf_curr, ptr->pktbuf_left);
			ptr->pktbuf_left = 0;
		}
		else
		{
			memcpy(ptr->notep, ptr->pktbuf_curr, ptr->pktbuf_left);
			ptr->cached_num += ptr->pktbuf_left;
			ptr->pktbuf_left = 0;
		}
		
		if(ptr->cached_num >= 8)
		{
			idpi_tns_parse_pkt_length(ptr);

			if(ptr->cached_num = ptr->tns_pkt_length)
			{
				ptr->wait_flag = MESSAGE_IS_COMPLETE;
			}
			else
			{
				ptr->wait_flag = WAIT_FOR_BUFFER;
			}	
		}
		else
		{
			ptr->wait_flag = WAIT_FOR_BUFFER;
		}

	}
	
	return 0;
}

int idpi_tns_parse_message_type(idpi_tns_parser_t* tns_flow_ptr)
{
    idpi_tns_parser_t *ptr = tns_flow_ptr;

    ptr->content_type = (idpi_tns_content_type_e )ptr->main_notes[4];

    return ptr->content_type;
}

int idpi_tns_print_header(idpi_tns_parser_t *ptr)
{
    printf("**********************************************\n");
    printf("idpi_tns_parser_t print:\n");
    printf("*	ptr->tns_version %s\n", version_array[ptr->content_type]);
    printf("*	ptr->content_type %s\n", content_type_array[ptr->content_type]);
    printf("*	ptr->direction %u\n", ptr->direction);
    printf("*	ptr->tns_pkt_length %u\n", ptr->tns_pkt_length);

    printf("**********************************************\n");

    return 0;
}

int idpi_tns_parse_payload(idpi_tns_parser_t *ptr)
{
    if(ptr)
    {
        uint8_t message_type = idpi_tns_parse_message_type(ptr);
        switch(message_type)
        {
            case TNS_TYPE_CONNECT:
                printf("TNS_TYPE_CONNECT\n");
                break;

            case TNS_TYPE_ACCEPT:
                break;

            case TNS_TYPE_DATA:
               
                break;

            /*case TNS_TYPE_NULL:
                break;

            case TNS_TYPE_ABORT:
                break;

            case TNS_TYPE_RESEND:
                break;

            case TNS_TYPE_MARKER:
                break;

            case TNS_TYPE_ATTENTION:
                break;

            case TNS_TYPE_CONTROL:
                break;

            case TNS_TYPE_MAX:
                break;

            case TNS_TYPE_ACK:
                break;

            case TNS_TYPE_REFUSE:
                break;

            case TNS_TYPE_REDIRECT:
                break;*/
        }

    }
    else
        return -1;

    return 0;
}

int idpi_tns_parse_processing(idpi_tns_parser_t* tns_flow_ptr, void* buf, uint32_t buf_len, uint8_t direction)
{
	idpi_tns_parser_t *ptr = tns_flow_ptr;
    printf("##############################################\n");
    printf("%s(): buf_len is %u.\n", __func__, buf_len);
    
    ptr->direction = direction;
    if(ptr->direction == DIR_RESPONSE)
    {
    	return 0;
    }

    ptr->pktbuf_init = buf;
    ptr->pktbuf_curr = buf;
    ptr->pktbuf_left = buf_len;

    if(ptr->direction == DIR_REQUEST)
    {
    	if(idpi_tns_parse_cache_message(ptr) == error)
        {
            return error;
        }
    }

    if(ptr->wait_flag == MESSAGE_IS_COMPLETE)
    {
        idpi_tns_parse_payload(ptr);

        ptr->wait_flag == WAIT_FOR_BUFFER;
    }
    else
    {
        printf("wait for more buffer to handle the message\n");
        return 0;
    }

    idpi_tns_print_header(ptr);
    printf("wait for next message\n");
    //printf("ptr->pktbuf_curr1 is %p\n", ptr->pktbuf_curr);
	//printf("ptr->pktbuf_left1 is %u\n", ptr->pktbuf_left);
}

