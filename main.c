#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "tns_parse.h"
#include "test_buffer2.h"


int main()
{	
	idpi_tns_parser_t *ptr = (idpi_tns_parser_t *)idpi_tns_parse_flow_init();
	
	//idpi_tns_parse_processing(ptr, (void *)buf1, buf1_len, DIR_REQUEST);

    idpi_tns_parse_processing(ptr, (void *)smallbuf1, buf_smallbuf1_len, 0);
    idpi_tns_parse_processing(ptr, (void *)smallbuf2, buf_smallbuf2_len, 1);
    idpi_tns_parse_processing(ptr, (void *)smallbuf3, buf_smallbuf3_len, 0);
    idpi_tns_parse_processing(ptr, (void *)smallbuf4, buf_smallbuf4_len, 0);
    idpi_tns_parse_processing(ptr, (void *)smallbuf5, buf_smallbuf5_len, 0);
    idpi_tns_parse_processing(ptr, (void *)smallbuf6, buf_smallbuf6_len, 1);

    int i =0;
    for(i = 4; i < BUFFER_NUM; i++)
    {
        if(i%2 == 0)
        {
            idpi_tns_parse_processing(ptr, (void *)buf[i], buf_len[i], 0);
        }
        else
        {
            idpi_tns_parse_processing(ptr, (void *)buf[i], buf_len[i], 1);
        }
        //buf_len = ((sizeof paste(buf, i)) / (sizeof (uint8_t)));
        //printf("buf_len is %u \n", buf_len[i]);
    }

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
    ptr->content_type = 0;
    ptr->parse_state = __IDPI_TNS_PARSE_STATE_INIT;
    //ptr->content_encoding = NULL;
    //ptr->chunked_encoding = 0;
    ptr->logging_flag = 0;
    ptr->tns_version = 0;

    int i;
    for (i = 1; i < IDPI_TNS_NUM_BACKUP_CACHE_BLOCK; ++i)
    {
        if (ptr->backup_cache_block[i])
        {
            idpi_tns_free_memo(ptr->backup_cache_block[i]); 
            ptr->backup_cache_block[i] = NULL;
        }
    }
    ptr->backup_cache_block[0] = ptr->main_cache_block;
    ptr->cache_block_p = ptr->main_cache_block;
    ptr->curr_cache_block = 0;
    ptr->curr_cache_block_left = IDPI_TNS_MEMO_LEN_LIMIT;
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

    memset(ptr->backup_cache_block, 0, sizeof(ptr->backup_cache_block));
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

    return COMPLETE;
}

int idpi_tns_parse_free_backup_cache_block(idpi_tns_parser_t* tns_flow_ptr)
{
    idpi_tns_parser_t *ptr = tns_flow_ptr;

    if(ptr)
    {
        int i;
        for(i = 1; i < IDPI_TNS_NUM_BACKUP_CACHE_BLOCK; ++i)
        {
            if(ptr->backup_cache_block[i])
            {
                idpi_tns_free_memo(ptr->backup_cache_block[i]);
                ptr->backup_cache_block[i] = NULL;
            }
        }
        ptr->backup_cache_block[0] = ptr->main_cache_block;
        ptr->cache_block_p = ptr->main_cache_block;
        ptr->curr_cache_block = 0;
        ptr->cached_num = 0;
        ptr->curr_cache_block_left = IDPI_TNS_MEMO_LEN_LIMIT;
    }

    return COMPLETE;
}

int idpi_tns_parse_cache_message(idpi_tns_parser_t* tns_flow_ptr)
{
	idpi_tns_parser_t *ptr = tns_flow_ptr;
   
	if(ptr)
	{
        uint32_t curr_cached_num = IDPI_TNS_MEMO_LEN_LIMIT - ptr->curr_cache_block_left;
		if(ptr->pktbuf_left >= ptr->curr_cache_block_left)
		{
			ptr->cached_num += ptr->pktbuf_left;
		
			memcpy(ptr->cache_block_p + curr_cached_num, ptr->pktbuf_curr, ptr->curr_cache_block_left);
			ptr->pktbuf_curr += IDPI_TNS_MEMO_LEN_LIMIT;
   			ptr->pktbuf_left -= IDPI_TNS_MEMO_LEN_LIMIT;

			while(ptr->pktbuf_left >= IDPI_TNS_MEMO_LEN_LIMIT)
			{
                ptr->curr_cache_block++;
                
                if(ptr->curr_cache_block == IDPI_TNS_NUM_BACKUP_CACHE_BLOCK)
                {
                    return ERROR;
                }
                
	            if(ptr->backup_cache_block[ptr->curr_cache_block] == NULL)
	            {
	                ptr->backup_cache_block[ptr->curr_cache_block] = idpi_tns_alloc_memo();
	                ptr->curr_cache_block_left = IDPI_TNS_MEMO_LEN_LIMIT;
	                if(ptr->backup_cache_block[ptr->curr_cache_block] == NULL)
	                {
                        printf("error\n");
	                    return ERROR;//add error code
	                }
	            }
	            ptr->cache_block_p = ptr->backup_cache_block[ptr->curr_cache_block];

	            memcpy(ptr->cache_block_p, ptr->pktbuf_curr, IDPI_TNS_MEMO_LEN_LIMIT);
				ptr->pktbuf_curr += IDPI_TNS_MEMO_LEN_LIMIT;
	   			ptr->pktbuf_left -= IDPI_TNS_MEMO_LEN_LIMIT;
			}

            ptr->curr_cache_block++;
            
            if(ptr->backup_cache_block[ptr->curr_cache_block] == NULL)
            {
                ptr->backup_cache_block[ptr->curr_cache_block] = idpi_tns_alloc_memo();
                ptr->curr_cache_block_left = IDPI_TNS_MEMO_LEN_LIMIT;
                if(ptr->backup_cache_block[ptr->curr_cache_block] == NULL)
                {
                    return ERROR;//add error code
                }
            }
            ptr->cache_block_p = ptr->backup_cache_block[ptr->curr_cache_block];

			memcpy(ptr->cache_block_p, ptr->pktbuf_curr, ptr->pktbuf_left);
            ptr->curr_cache_block_left -= ptr->pktbuf_left;
			ptr->pktbuf_left = 0;
		}
		else
		{
			memcpy(ptr->cache_block_p + curr_cached_num, ptr->pktbuf_curr, ptr->pktbuf_left);
			ptr->cached_num += ptr->pktbuf_left;
            ptr->curr_cache_block_left -= ptr->pktbuf_left;
			ptr->pktbuf_left = 0;
		}
        
        printf("ptr->curr_cache_block is %u\n", ptr->curr_cache_block);

		if(ptr->cached_num >= 8)
		{
			idpi_tns_parse_header(ptr);

			if(ptr->cached_num == ptr->tns_pkt_length)
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
	
	return COMPLETE;
}

int idpi_tns_parse_state_switch(idpi_tns_parser_t* tns_flow_ptr)
{
    idpi_tns_parser_t *ptr = tns_flow_ptr;

    if(ptr)
    {
        uint8_t state = ptr->parse_state;
        uint8_t content_type = ptr->content_type;

        if(ptr->direction == DIR_RESPONSE)
        {
            switch(content_type)
            {
                case TNS_TYPE_REFUSE:
                    state = __IDPI_TNS_PARSE_STATE_REFUSED;
                    break;

                case TNS_TYPE_REDIRECT:
                    state = __IDPI_TNS_PARSE_STATE_REDIRECTED;
                    break;

                case TNS_TYPE_ACCEPT:
                    state = __IDPI_TNS_PARSE_STATE_CONNECTED;
                    break;

                case TNS_TYPE_DATA:
                    state = __IDPI_TNS_PARSE_STATE_RESPONDED;
                    break;

                case TNS_TYPE_MARKER:
                    state = __IDPI_TNS_PARSE_STATE_ERROR_MARKER;
                    break;
            }
        }
        else
        {
            switch(content_type)
            {
                case TNS_TYPE_CONNECT:
                    state = __IDPI_TNS_PARSE_STATE_CONNECTING;
                    break;

                case TNS_TYPE_DATA:
                    state = __IDPI_TNS_PARSE_STATE_REQUESTED;
                    break;

                case TNS_TYPE_MARKER:
                    state = __IDPI_TNS_PARSE_STATE_ERROR_MARKER;
                    break;
            }
        }
        ptr->parse_state = state;
    }
    else
    {
        return ERROR;
    }

    return COMPLETE;
}

int idpi_tns_parse_header(idpi_tns_parser_t* tns_flow_ptr)
{
    idpi_tns_parser_t *ptr = tns_flow_ptr;

    ptr->tns_pkt_length = ptr->main_cache_block[0] << 8;
    ptr->tns_pkt_length += ptr->main_cache_block[1];

    ptr->content_type = (idpi_tns_content_type_e )ptr->main_cache_block[4];

    if(ptr->tns_pkt_length == 0) 
    {
        //tns 315
        ptr->tns_pkt_length = ptr->main_cache_block[2]<<8;
        ptr->tns_pkt_length += ptr->main_cache_block[3];
        
        return COMPLETE;
    }

    return COMPLETE;
}

int idpi_tns_parse_payload_connect(idpi_tns_parser_t* tns_flow_ptr)
{
    idpi_tns_parser_t *ptr = tns_flow_ptr;

    if(ptr != NULL && ptr->content_type == TNS_TYPE_CONNECT)
    {
        return COMPLETE;
    }
    else
    {
        return ERROR;
    }
}

int idpi_tns_parse_payload_accept(idpi_tns_parser_t* tns_flow_ptr)
{
    idpi_tns_parser_t *ptr = tns_flow_ptr;

    if(ptr != NULL && ptr->content_type == TNS_TYPE_ACCEPT)
    {
        uint8_t *parser_cursor = ptr->main_cache_block + 8;

        ptr->tns_version = parser_cursor[0] << 8;
        ptr->tns_version += parser_cursor[1];
        printf("ptr->tns_version is %u\n", ptr->tns_version);
    }
    else
    {
        return ERROR;
    }
        
    return COMPLETE;
}

uint8_t idpi_tns_parse_payload_data_type(uint16_t flag1, uint16_t flag2, uint16_t flag3)
{
    //TODO
    uint16_t data_flag = flag1;
    uint16_t id_subid  = flag2;
    uint16_t id_subid_extended = flag3;

    if((data_flag == 0) && (id_subid == 0x1169) && (id_subid_extended == 0x1201))
    {
        //TODO end fo connect client
        printf("Payload is TNS_DATA_RESPONSE_CLOSE_SESSION\n"); 
        return TNS_DATA_RESPONSE_CLOSE_SESSION;
    }
    else if((data_flag == 0) && (id_subid == 0x1169))
    {
        //TODO end of connect server
        printf("Payload is TNS_DATA_SQL_COMMAND1\n");   
        return TNS_DATA_SQL_COMMAND1;
    }
    else if((data_flag == 0) && (id_subid == 0x035e))
    {
        printf("Payload is TNS_DATA_SQL_COMMAND2\n");   
        return TNS_DATA_SQL_COMMAND2;
    }
    else if((data_flag == 0) && (id_subid == 0x0376))
    {
        printf("Payload is TNS_DATA_USER_INFO\n");
        return TNS_DATA_USER_INFO;
    }
    else if((data_flag == 0) && (id_subid == 0x0305))
    {
        printf("Payload is TNS_DATA_FETCH_MORE\n");
        return TNS_DATA_FETCH_MORE;
    }
    else
    {
        printf("Payload is TNS_DATA_ALL_OTHER\n");
        return TNS_DATA_ALL_OTHER;
    }
}

int idpi_tns_parse_payload_data(idpi_tns_parser_t* tns_flow_ptr)
{
    idpi_tns_parser_t *ptr = tns_flow_ptr;

    if(ptr != NULL && ptr->content_type == TNS_TYPE_DATA)
    {
        uint8_t *parser_cursor = ptr->main_cache_block + 8;

        uint16_t data_flag;
        uint16_t id_subid;
        uint16_t id_subid_extended;
        uint8_t payload_data_type;

        data_flag = parser_cursor[0] << 8;
        data_flag += parser_cursor[1];
        printf("data_flag is %x\n", data_flag);

        parser_cursor = parser_cursor + 2;
        id_subid = parser_cursor[0] << 8;
        id_subid += parser_cursor[1];
        printf("id_subid is %x\n", id_subid);

        parser_cursor = parser_cursor + 2;
        id_subid_extended = parser_cursor[0] << 8;
        id_subid_extended += parser_cursor[1];
        printf("id_subid_extended is %x\n", id_subid_extended);

        payload_data_type = idpi_tns_parse_payload_data_type(data_flag, id_subid, id_subid_extended);
    
        switch(payload_data_type)
        {
            case TNS_DATA_SQL_COMMAND1://need add decide whether is selcet
                //printf("Payload is SQL COMMAND1:\n%s\n", (parser_cursor+67));
                
                break;

            case TNS_DATA_SQL_COMMAND2://need add decide whether is selcet
                //printf("Payload is SQL COMMAND2:\n%s\n", (parser_cursor+55));
                
                break;

            case TNS_DATA_USER_INFO:
                //printf("Payload is USER INFO:\n%s\n", (parser_cursor+18));
                
                break;

            case TNS_DATA_ALL_OTHER:
                printf("Payload is TNS_DATA_ALL_OTHER to be ignore\n"); 
                break;
            //TODO other cases
        }

        data_flag = 0;
        id_subid = 0;
        id_subid_extended = 0;
    }
    else
    {
        return ERROR;
    }

    return COMPLETE;
}


int idpi_tns_print_header(idpi_tns_parser_t *ptr)
{
    printf("**********************************************\n");
    printf("idpi_tns_parser_t print:\n");
    printf("*   ptr->tns_version %u\n", ptr->tns_version);
    printf("*   ptr->content_type %s\n", content_type_array[ptr->content_type]);
    printf("*   ptr->direction %u\n", ptr->direction);
    printf("*   ptr->tns_pkt_length %u\n", ptr->tns_pkt_length);
    printf("*   ptr->cached_num %u\n", ptr->cached_num);
    printf("*   ptr->curr_cache_block %u\n", ptr->curr_cache_block);
    printf("*   ptr->parse_state %s\n", parse_state_array[ptr->parse_state]);

    printf("**********************************************\n");

    return COMPLETE;
}

int idpi_tns_parse_payload(idpi_tns_parser_t *ptr)
{
    if(ptr)
    {
        uint8_t message_type = ptr->content_type;
        switch(message_type)
        {
            case TNS_TYPE_CONNECT:
                printf("TNS_TYPE_CONNECT\n");
                idpi_tns_parse_payload_connect(ptr);
                break;

            case TNS_TYPE_ACCEPT:
                printf("TNS_TYPE_ACCEPT\n");
                idpi_tns_parse_payload_accept(ptr);
                break;

            case TNS_TYPE_DATA:
                printf("TNS_TYPE_DATA\n");
                idpi_tns_parse_payload_data(ptr);
                break;

            case TNS_TYPE_RESEND:
                printf("TNS_TYPE_RESEND\n");
                break;

            /*case TNS_TYPE_NULL:
                break;

            case TNS_TYPE_ABORT:
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
        return ERROR;

    return COMPLETE;
}

/*idpi_tns_parse_log_process(idpi_tns_parser_t* tns_flow_ptr)
{
    idpi_tns_parser_t *ptr = tns_flow_ptr;

    if(ptr)
    {
        if()
    }
    else
    {
        return ERROR;
    }

    return COMPLETE;
}*/

int idpi_tns_parse_processing(idpi_tns_parser_t* tns_flow_ptr, void* buf, uint32_t buf_len, uint8_t direction)
{
	idpi_tns_parser_t *ptr = tns_flow_ptr;

    printf("##############################################\n");
    printf("######## New Buffer, Parse Start Here ! ######\n");
    printf("##############################################\n");
    printf("%s(): buf_len is %u.\n", __func__, buf_len);
    
    ptr->direction = direction;

    ptr->pktbuf_init = buf;
    ptr->pktbuf_curr = buf;
    ptr->pktbuf_left = buf_len;

    if(ptr->direction == DIR_REQUEST)
    {
    	if(idpi_tns_parse_cache_message(ptr) == ERROR)
        {
            return ERROR;
        }
    }
    else
    {
        if(idpi_tns_parse_cache_message(ptr) == ERROR)
        {
            return ERROR;
        }
        //printf("message is response, just skip\n");
    }

    if(ptr->wait_flag == MESSAGE_IS_COMPLETE)
    {
        idpi_tns_parse_state_switch(ptr);

        idpi_tns_parse_payload(ptr);
        
        idpi_tns_parse_free_backup_cache_block(ptr);

        ptr->wait_flag == WAIT_FOR_BUFFER;
    }
    else
    {
        printf("wait for more buffer to handle the message\n");
        idpi_tns_print_header(ptr);
        return COMPLETE;
    }
    
    printf("wait for next message\n");
    idpi_tns_print_header(ptr);
}

