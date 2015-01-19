#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/*******************************************************
*tns_parse_module init.call by app 
*******************************************************/
#ifdef HILI_DEBUG
int idpi_tns_parse_module_init() 
{ 
    return 0;
}
#endif
/*******************************************************
*init the context struct for every pair of request & response
*******************************************************/
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
    ptr->need_to_log = UNSET;
    ptr->client_ip = 0xc0a8be0a;
    ptr->server_ip = 0xc0a8be0a;
    ptr->client_port = 0xead5;
    ptr->server_port = 0xead5;

    int i;
    for(i = 0; i < IDPI_TNS_MAX_NUM_CACHE_BLOCK; ++i)
    {
        if(ptr->request_cache_block_p[i])
        {
            idpi_tns_free_memo(ptr->request_cache_block_p[i]); 
            ptr->request_cache_block_p[i] = NULL;
        }

        if(ptr->response_cache_block_p[i])
        {
            idpi_tns_free_memo(ptr->response_cache_block_p[i]); 
            ptr->response_cache_block_p[i] = NULL;
        }
    }

    ptr->request_cache_block_p[ptr->request_curr_cache_block] = idpi_tns_alloc_memo();
    ptr->request_curr_cache_block = 0;
    ptr->request_cached_num = 0;
    ptr->request_curr_cache_block_left = IDPI_TNS_MEMO_LEN_LIMIT;

    ptr->response_cache_block_p[ptr->response_curr_cache_block] = idpi_tns_alloc_memo();
    ptr->response_curr_cache_block = 0;
    ptr->response_cached_num = 0;    
    ptr->response_curr_cache_block_left = IDPI_TNS_MEMO_LEN_LIMIT;
}
/*******************************************************
*create the context struct for each flow, call by app
*******************************************************/
void* idpi_tns_parse_flow_init()//tns_parse_flow_conf_t *conf)
{
    idpi_tns_parser_t *ptr = idpi_tns_alloc_parser();

    if (!ptr) 
    {
        printf("idpi_tns_alloc_parser error\n");
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

    memset(ptr->request_cache_block_p, 0, sizeof(ptr->request_cache_block_p));
    memset(ptr->response_cache_block_p, 0, sizeof(ptr->response_cache_block_p));
    idpi_tns_context_init(ptr);

    return (void*)ptr;
}
/*******************************************************
*call by app for each end of flow
*******************************************************/
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
/*******************************************************
*cache message body from buffer until message is complete
*******************************************************/
int idpi_tns_parse_cache_message(idpi_tns_parser_t* tns_flow_ptr)
{
    idpi_tns_parser_t *ptr = tns_flow_ptr;
   
    if(ptr)
    {
        uint32_t cached_num;
        uint32_t curr_cached_num;
        uint32_t curr_cache_block_left;
        uint8_t **cache_block_p;
        uint8_t curr_cache_block;
        
        if(ptr->direction == DIR_REQUEST)
        {
            curr_cache_block_left = ptr->request_curr_cache_block_left;
            cached_num            = ptr->request_cached_num;
            cache_block_p         = ptr->request_cache_block_p;
            curr_cache_block      = ptr->request_curr_cache_block;
        }
        else
        {
            curr_cache_block_left = ptr->response_curr_cache_block_left;
            cached_num            = ptr->response_cached_num;
            cache_block_p         = ptr->response_cache_block_p;
            curr_cache_block      = ptr->response_curr_cache_block;
        }

        curr_cached_num = IDPI_TNS_MEMO_LEN_LIMIT - curr_cache_block_left;
        if(ptr->pktbuf_left >= curr_cache_block_left)
        {
            cached_num += ptr->pktbuf_left;
        
            memcpy(cache_block_p[curr_cache_block] + curr_cached_num, ptr->pktbuf_curr, curr_cache_block_left);
            ptr->pktbuf_curr += IDPI_TNS_MEMO_LEN_LIMIT;
            ptr->pktbuf_left -= IDPI_TNS_MEMO_LEN_LIMIT;

            while(ptr->pktbuf_left >= IDPI_TNS_MEMO_LEN_LIMIT)
            {
                curr_cache_block++;
                
                if(curr_cache_block == IDPI_TNS_MAX_NUM_CACHE_BLOCK)
                {
                    return ERROR;
                }
                
                if(cache_block_p[curr_cache_block] == NULL)
                {
                    cache_block_p[curr_cache_block] = idpi_tns_alloc_memo();
                    curr_cache_block_left = IDPI_TNS_MEMO_LEN_LIMIT;
                    if(cache_block_p[curr_cache_block] == NULL)
                    {
                        printf("error\n");
                        return ERROR;//add error code               
                    }
                }

                memcpy(cache_block_p[curr_cache_block], ptr->pktbuf_curr, IDPI_TNS_MEMO_LEN_LIMIT);
                ptr->pktbuf_curr += IDPI_TNS_MEMO_LEN_LIMIT;
                ptr->pktbuf_left -= IDPI_TNS_MEMO_LEN_LIMIT;
            }

            curr_cache_block++;
            
            if(cache_block_p[curr_cache_block] == NULL)
            {
                cache_block_p[curr_cache_block] = idpi_tns_alloc_memo();
                curr_cache_block_left = IDPI_TNS_MEMO_LEN_LIMIT;
                if(cache_block_p[curr_cache_block] == NULL)
                {
                    return ERROR;//add error code
                }
            }

            memcpy(cache_block_p[curr_cache_block], ptr->pktbuf_curr, ptr->pktbuf_left);
            curr_cache_block_left -= ptr->pktbuf_left;
            ptr->pktbuf_left = 0;
        }
        else
        {
            memcpy(cache_block_p[curr_cache_block] + curr_cached_num, ptr->pktbuf_curr, ptr->pktbuf_left);
            cached_num += ptr->pktbuf_left;
            curr_cache_block_left -= ptr->pktbuf_left;
            ptr->pktbuf_left = 0;
        }
        
        printf("curr_cache_block is %u\n", curr_cache_block);

        if(cached_num >= 8)
        {
            idpi_tns_parse_header(ptr);

            if(cached_num == ptr->tns_pkt_length)
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

        if(ptr->direction == DIR_REQUEST)
        {
            ptr->request_curr_cache_block_left = curr_cache_block_left;
            ptr->request_cached_num            = cached_num;
            ptr->request_curr_cache_block      = curr_cache_block;
            int i;
            for(i = 1; i <= curr_cache_block; i++)
            {
                ptr->request_cache_block_p[i] = cache_block_p[i];
            }
        }
        else
        {
            ptr->response_curr_cache_block_left = curr_cache_block_left;
            ptr->response_cached_num            = cached_num;
            ptr->response_curr_cache_block      = curr_cache_block;
            int i;
            for(i = 1; i <= curr_cache_block; i++)
            {
                ptr->response_cache_block_p[i] = cache_block_p[i];
            }
        }
    }
    
    return COMPLETE;
}
/*******************************************************
*switch parse state of context according to content type
*******************************************************/
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
/*******************************************************
*iparse tns message header(first 8 bytes)
*******************************************************/
int idpi_tns_parse_header(idpi_tns_parser_t* tns_flow_ptr)
{
    uint8_t *main_cache_block;
    idpi_tns_parser_t *ptr = tns_flow_ptr;

    if(ptr->direction == DIR_REQUEST)
    {
        main_cache_block = ptr->request_cache_block_p[0];
    }
    else
    {
        main_cache_block = ptr->response_cache_block_p[0];
    }

    ptr->tns_pkt_length = main_cache_block[0] << 8;
    ptr->tns_pkt_length += main_cache_block[1];

    ptr->content_type = (idpi_tns_content_type_e )main_cache_block[4];

    if(ptr->tns_pkt_length == 0) 
    {
        //tns 315
        ptr->tns_pkt_length = main_cache_block[2]<<8;
        ptr->tns_pkt_length += main_cache_block[3];
        
        return COMPLETE;
    }

    return COMPLETE;
}
/*******************************************************
*parse payload of accept type. Record the tns version
*******************************************************/
int idpi_tns_parse_payload_accept(idpi_tns_parser_t* tns_flow_ptr)
{
    uint8_t *main_cache_block;
    idpi_tns_parser_t *ptr = tns_flow_ptr;

    if(ptr->direction == DIR_REQUEST)
    {
        main_cache_block = ptr->request_cache_block_p[0];
    }
    else
    {
        main_cache_block = ptr->response_cache_block_p[0];
    }

    if(ptr != NULL && ptr->content_type == TNS_TYPE_ACCEPT)
    {
        uint8_t *parser_cursor = main_cache_block + 8;

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
/*******************************************************
*parse payload of connect type. Record the connection info
*******************************************************/
int idpi_tns_parse_payload_connect(idpi_tns_parser_t* tns_flow_ptr)
{
    idpi_tns_parser_t *ptr = tns_flow_ptr;
    uint32_t copy_length;
    uint8_t *start_pos = ptr->request_cache_block_p[0] + 58;
    copy_length = ptr->tns_pkt_length - 58;

    if(copy_length >= CONNECTION_INFO_MAX_LENGTH)
    {
        copy_length = CONNECTION_INFO_MAX_LENGTH;
    }

    memcpy(ptr->connection_info, start_pos, copy_length);
    printf("connection_info is ");
    int j;
    for(j = 0; j < copy_length; j++)
        printf("%c", ptr->connection_info[j]);
    printf("\n");
}
/*******************************************************
*parse payload of data type. find valid data message to log
*******************************************************/
uint8_t idpi_tns_parse_payload_data_type(idpi_tns_parser_t* tns_flow_ptr)
{
    //TODO
    uint8_t *main_cache_block;
    idpi_tns_parser_t *ptr = tns_flow_ptr;

    if(ptr->direction == DIR_REQUEST)
    {
        main_cache_block = ptr->request_cache_block_p[0];
    }
    else
    {
        main_cache_block = ptr->response_cache_block_p[0];
    }

    if(ptr != NULL && ptr->content_type == TNS_TYPE_DATA)
    {
        uint8_t *parser_cursor = main_cache_block + 8;

        uint16_t data_flag;
        uint16_t id_subid;
        uint16_t id_subid_extended;

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
            ptr->need_to_log = SET;  
            return TNS_DATA_SQL_COMMAND1;
        }
        else if((data_flag == 0) && (id_subid == 0x035e))
        {
            printf("Payload is TNS_DATA_SQL_COMMAND2\n");
            ptr->need_to_log = SET;   
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

        data_flag = 0;
        id_subid = 0;
        id_subid_extended = 0;
    }
    else
    {
        return ERROR;
    }
}
/*******************************************************************************
*return the magicshift to valid data in data payload according to tns version
*******************************************************************************/
uint8_t idpi_tns_parse_locate_valid_payload(idpi_tns_parser_t *ptr, uint8_t payload_data_type)
{   //TODO add response
    if(ptr)
    {   
        uint8_t magicshift = 0;
        if(ptr->content_type == TNS_TYPE_DATA)
        {
            if(ptr->tns_version == TNS_VERSION_312)
            {
                switch(payload_data_type)
                {
                    case TNS_DATA_SQL_COMMAND1:
                        magicshift = TNS_312_MAGICSHIFT_TO_SQL_COMMAND1;
                        break;
                    case TNS_DATA_SQL_COMMAND2:
                        magicshift = TNS_312_MAGICSHIFT_TO_SQL_COMMAND2;
                        break;
                    case TNS_DATA_USER_INFO:
                        magicshift = TNS_312_MAGICSHIFT_TO_USER_INFO;
                        break;
                }
            }
            else if(ptr->tns_version == TNS_VERSION_313)
            {
                switch(payload_data_type)
                {
                    case TNS_DATA_SQL_COMMAND1:
                        magicshift = TNS_313_MAGICSHIFT_TO_SQL_COMMAND1;
                        break;
                    case TNS_DATA_SQL_COMMAND2:
                        magicshift = TNS_313_MAGICSHIFT_TO_SQL_COMMAND2;
                        break;
                    case TNS_DATA_USER_INFO:
                        magicshift = TNS_313_MAGICSHIFT_TO_USER_INFO;
                        break;
                }
            }
            else if(ptr->tns_version == TNS_VERSION_314)
            {
                switch(payload_data_type)
                {
                    case TNS_DATA_SQL_COMMAND1:
                        magicshift = TNS_314_MAGICSHIFT_TO_SQL_COMMAND1;
                        break;
                    case TNS_DATA_SQL_COMMAND2:
                        magicshift = TNS_314_MAGICSHIFT_TO_SQL_COMMAND2;
                        break;
                    case TNS_DATA_USER_INFO:
                        magicshift = TNS_314_MAGICSHIFT_TO_USER_INFO;
                        break;
                }
            }
            else
            {
                switch(payload_data_type)
                {
                    case TNS_DATA_SQL_COMMAND1:
                        magicshift = TNS_315_MAGICSHIFT_TO_SQL_COMMAND1;
                        break;
                    case TNS_DATA_SQL_COMMAND2:
                        magicshift = TNS_315_MAGICSHIFT_TO_SQL_COMMAND2;
                        break;
                    case TNS_DATA_USER_INFO:
                        magicshift = TNS_315_MAGICSHIFT_TO_USER_INFO;
                        break;
                }
            }
        }
        else
        {
            return ERROR;
        }

        return magicshift;
    }
    else
        return ERROR;
}
/****************************************************
*print some info of context for each parse_process
*****************************************************/
int idpi_tns_print_header(idpi_tns_parser_t *ptr)
{
    printf("**********************************************\n");
    printf("idpi_tns_parser_t print:\n");
    printf("*   ptr->tns_version %u\n", ptr->tns_version);
    printf("*   ptr->content_type %s\n", content_type_array[ptr->content_type]);
    printf("*   ptr->direction %u\n", ptr->direction);
    printf("*   ptr->tns_pkt_length %u\n", ptr->tns_pkt_length);
    printf("*   ptr->request_cached_num %u\n", ptr->request_cached_num);
    printf("*   ptr->response_cached_num %u\n", ptr->response_cached_num);
    printf("*   ptr->request_curr_cache_block %u\n", ptr->request_curr_cache_block);
    printf("*   ptr->response_curr_cache_block %u\n", ptr->response_curr_cache_block);
    printf("*   ptr->parse_state %s\n", parse_state_array[ptr->parse_state]);

    printf("**********************************************\n");

    return COMPLETE;
}
/****************************************************
*alloc log buffer
*****************************************************/
int idpi_tns_parse_alloc_logbuf(idpi_tns_parser_t *ptr)
{
    //ptr->logbuf_start = hili_send_module_fpa_alloc(); 
    ptr->logbuf_start = (uint8_t *)malloc(IDPI_TNS_LOGBUF_LEN_MAX); 
    if (!ptr->logbuf_start)
    {
        //return ptr->err_code = __IDPI_HTTP_PARSE_OUT_OF_MEM;
        return -1;
    }

    //memset(ptr->logbuf_start, 0, 16);
    ptr->logbuf_curr = ptr->logbuf_start + 16;

    *((uint16_t*)(ptr->logbuf_start + 8)) = 0x1;/*type tns*/
    ptr->logbuf_start[11] = 0x01;/* last segment default */
    *((uint16_t*)(ptr->logbuf_start + 12)) = 0; /* segment count */

    return 0;
}
/****************************************************
*show the content in log buffer
*****************************************************/
int idpi_tns_parse_print_logbuf(idpi_tns_parser_t *ptr)
{
    int i = 0;
    printf("logbuffer is:\n");
    for(i = 0; i < (ptr->logbuf_curr - ptr->logbuf_start); i++)
    {
        printf(" %x", *(ptr->logbuf_start + i));
    }
    printf("\n");
}
/****************************************************
*send log :
*1)a pair of request & response is finished
*2)the log is full to send segment of it
*****************************************************/
int idpi_tns_parse_send_logbuf(idpi_tns_parser_t *ptr, unsigned char is_segmented, unsigned char is_last_segment)
{
    ptr->logbuf_start[10] = is_segmented;
    ptr->logbuf_start[11] = is_last_segment;
    *(uint16_t*)(ptr->logbuf_start + 12) = (is_segmented?(++ptr->segment_count):0); /* segment count */
    *(uint16_t*)(ptr->logbuf_start + 14) = (ptr->logbuf_curr - ptr->logbuf_start - 16); /* log's variable content size */
    //hili_send_module_send_udp_packet(ptr->logbuf_start, ptr->logbuf_curr - ptr->logbuf_start, &ptr->send_conf);
    idpi_tns_parse_print_logbuf(ptr);
    //memset(ptr->logbuf_start, '\0', ptr->logbuf_curr - ptr->logbuf_start);

    printf("idpi_tns_parse_send_logbuf\n");
    
    idpi_tns_parse_free_logbuf(ptr);//need this?????

    return 0;
}
/****************************************************
*create a log for each pair of request & response
*****************************************************/
int idpi_tns_parse_create_log(idpi_tns_parser_t *ptr)
{
    if (idpi_tns_parse_alloc_logbuf(ptr))
    {
        printf("idpi_tns_parse_create_log failure\n");
        return ERROR;
    }
    
    strcpy(ptr->logbuf_curr, "2015-01-09-00-00-00");
    ptr->logbuf_curr += 30; /* ignore login time field */ 

    uint16_t *var16p = (uint16_t*)ptr->logbuf_curr;
    *var16p++ = ptr->tns_version;
    *var16p++ = ptr->client_port;
    *var16p++ = ptr->server_port;

    uint32_t *var32p = (uint32_t*)var16p;
    *var32p++ = ptr->client_ip;
    *var32p++ = ptr->server_ip;

    ptr->logbuf_curr = (unsigned char*)var32p;
    strcpy(ptr->logbuf_curr, "2015-01-09-00-00-00");
    ptr->logbuf_curr += 30; /* ignore login time field */ 
    strcpy(ptr->logbuf_curr, "2015-01-09-00-00-00");
    ptr->logbuf_curr += 30; /* ignore login time field */ 

    var32p = (uint32_t*)ptr->logbuf_curr;
    *var32p++ = strlen(ptr->username);
    ptr->logbuf_curr = (unsigned char*)var32p;
    memcpy(ptr->logbuf_curr, ptr->username, strlen(ptr->username));
    ptr->logbuf_curr += strlen(ptr->username);

    printf("idpi_tns_parse_create_log\n");
    /* next thing to log is content data */ 

    return 0;
}
/****************************************************
*free the log buffer ptr
*****************************************************/
int idpi_tns_parse_free_logbuf(idpi_tns_parser_t *ptr)
{
    free(ptr->logbuf_start);
    ptr->logbuf_start = ptr->logbuf_curr = 0;
    return 0;
}
/****************************************************
*write valid data to log for each cach_block
*****************************************************/
int idpi_tns_parse_log_valid_message(idpi_tns_parser_t* tns_flow_ptr, uint8_t *copy_start_p, uint32_t copy_length, uint8_t cache_block_num)
{
    idpi_tns_parser_t *ptr = tns_flow_ptr;

    if(ptr != NULL)
    {    
        if(copy_length < ptr->log_buffer_left)
        {
            memcpy(ptr->logbuf_curr, copy_start_p, copy_length);
            ptr->logbuf_curr += copy_length;
            if(cache_block_num == 0)
            {
                idpi_tns_parse_send_logbuf(ptr, 0, 1);
            }
            else
            {
                idpi_tns_parse_send_logbuf(ptr, 1, 1);
            }
        }
        else
        {
            memcpy(ptr->logbuf_curr, copy_start_p, ptr->log_buffer_left);
            ptr->logbuf_curr += ptr->log_buffer_left;
            idpi_tns_parse_send_logbuf(ptr, 1, 0);
            
            copy_length -= ptr->log_buffer_left;
            copy_start_p += ptr->log_buffer_left;  

            while(copy_length >= IDPI_TNS_LOGBUF_LEN_MAX)
            {
                if(idpi_tns_parse_alloc_logbuf(ptr))
                {
                    printf("idpi_tns_parse_create_log failure\n");
                    return ERROR;
                }

                ptr->log_buffer_left = IDPI_TNS_LOGBUF_LEN_MAX - (ptr->logbuf_curr - ptr->logbuf_start);
                memcpy(ptr->logbuf_curr, copy_start_p, ptr->log_buffer_left);
                ptr->logbuf_curr += ptr->log_buffer_left;
                idpi_tns_parse_send_logbuf(ptr, 1, 0);
                copy_length -= ptr->log_buffer_left;
                copy_start_p += ptr->log_buffer_left;    
            }

            if(idpi_tns_parse_alloc_logbuf(ptr))
            {
                printf("idpi_tns_parse_create_log failure\n");
                return ERROR;
            }
            ptr->log_buffer_left = IDPI_TNS_LOGBUF_LEN_MAX - (ptr->logbuf_curr - ptr->logbuf_start);

            memcpy(ptr->logbuf_curr, copy_start_p, copy_length);
            ptr->log_buffer_left -= copy_length;
            if(cache_block_num == ptr->request_curr_cache_block)
            {
                idpi_tns_parse_send_logbuf(ptr, 1, 1);
            }
            ptr->logbuf_curr += copy_length;
        }
    }
    else
    {
        return ERROR;
    }
}
/****************************************************
*main process of write log
*****************************************************/
int idpi_tns_parse_log_process(idpi_tns_parser_t* tns_flow_ptr)
{
    idpi_tns_parser_t *ptr = tns_flow_ptr;
    
    if(ptr)
    {
        int i = 0;
        uint8_t magicshift = ptr->magicshift;
        //TODO add response
        if(ptr->content_type == TNS_TYPE_DATA && ptr->direction == DIR_REQUEST)
        {
            if(ERROR != idpi_tns_parse_create_log(ptr))
            {
                uint32_t copy_left = ptr->tns_pkt_length - magicshift;
                uint32_t copy_length;
                uint8_t *copy_start_p = ptr->request_cache_block_p[0] + magicshift;
                ptr->log_buffer_left = IDPI_TNS_LOGBUF_LEN_MAX - (ptr->logbuf_curr - ptr->logbuf_start);
                
                uint32_t *var32p = (uint32_t*)ptr->logbuf_curr;
                *var32p++ = ptr->tns_pkt_length - magicshift;
                ptr->logbuf_curr = (unsigned char*)var32p;
                ptr->log_buffer_left -= 4;

                if(ptr->tns_pkt_length > IDPI_TNS_MEMO_LEN_LIMIT)
                {
                    copy_length = IDPI_TNS_MEMO_LEN_LIMIT - magicshift;
                    copy_left -= copy_length; 
                    idpi_tns_parse_log_valid_message(ptr, copy_start_p, copy_length, i);

                    while(copy_left >= IDPI_TNS_MEMO_LEN_LIMIT)
                    {
                        i++;
                        copy_start_p = ptr->request_cache_block_p[i];
                        copy_length = IDPI_TNS_MEMO_LEN_LIMIT;
                        idpi_tns_parse_log_valid_message(ptr, copy_start_p, copy_length, i);
                        
                        copy_left -= copy_length;
                    }
                    i++;
                    copy_start_p = ptr->request_cache_block_p[i];   
                    copy_length = copy_left;

                    idpi_tns_parse_log_valid_message(ptr, copy_start_p, copy_length, i);
                }
                else
                {
                    copy_length = copy_left;
                    copy_left = 0;
                    idpi_tns_parse_log_valid_message(ptr, copy_start_p, copy_length, 0);
                }
            }
        }
        else if(ptr->content_type == TNS_TYPE_DATA && ptr->direction == DIR_RESPONSE)
        {
            return COMPLETE;
        }
    }
    else
    {
        return ERROR;
    }

    return COMPLETE;
}
/****************************************************
*tns parse main process call by app
*****************************************************/
int idpi_tns_parse_processing(idpi_tns_parser_t* tns_flow_ptr, void* buf, uint32_t buf_len, uint8_t direction)
{
    idpi_tns_parser_t *ptr = tns_flow_ptr;

    if (direction != ptr->direction) /* flow direction change */
    {
        if (DIR_REQUEST == direction)
        {
            /* a new request arrives before the last response is finished */
            /* We haven't supported HTTP pipelining yet, as for most browser disable this feature by default. */
            idpi_tns_context_init(ptr);
        }
        ptr->direction = direction;
    }

    uint8_t payload_data_type;
    uint8_t magicshift_to_data;

    printf("##############################################\n");
    printf("####### New Buffer, Parse Start Here ! #######\n");
    printf("##############################################\n");
    printf("%s(): buf_len is %u.\n", __func__, buf_len);

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
        
        if(ptr->content_type == TNS_TYPE_DATA)
        {
            payload_data_type = idpi_tns_parse_payload_data_type(ptr);

            ptr->magicshift = idpi_tns_parse_locate_valid_payload(ptr, payload_data_type);

            if(payload_data_type == TNS_DATA_USER_INFO)
            {
                uint8_t i, copy_length;
                uint8_t *start_pos = ptr->request_cache_block_p[0] + ptr->magicshift;
                for(i = 0; *(start_pos +i) != 0x0d; i++)
                    ;
                if(i <= USERNAME_MAX_LENGTH)
                {
                    copy_length = i;
                }
                else
                {
                    copy_length = USERNAME_MAX_LENGTH;
                }
                memcpy(ptr->username, start_pos, copy_length);
                printf("length of username is %d\n", strlen(ptr->username));
                int j;
                for(j = 0; j < copy_length; j++)
                    printf("%c", ptr->username[j]);
                printf("\n");
            }
            else if(ptr->need_to_log == SET)
            {
                idpi_tns_parse_log_process(ptr);
                ptr->need_to_log = UNSET;
            }
        }
        else if(ptr->content_type == TNS_TYPE_CONNECT)
        {
            //idpi_tns_parse_payload_connect(ptr);
            idpi_tns_parse_payload_connect(ptr);
        }
        else if(ptr->content_type == TNS_TYPE_ACCEPT)
        {
            idpi_tns_parse_payload_accept(ptr);
        }
        
        idpi_tns_parse_state_switch(ptr);

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
