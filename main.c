#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "tns_parse.h"
#include "test_buffer2.h"

#ifdef LOG_LENGTH_MAX


#endif

int idpi_tns_parse_processing(idpi_tns_parser_t* tns_flow_ptr, void* buf, uint32_t buf_len, uint8_t direction)
{
	idpi_tns_parse_right_start(tns_flow_ptr, direction);
    printf("##############################################\n");
    
	idpi_tns_parser_t *psr = (idpi_tns_parser_t *)tns_flow_ptr;
    psr->direction = direction;
    idpi_tns_state_e tmp_parse_state = psr->parse_state;
    //assert(psr);
    psr->buf_num++;
    psr->pktbuf_init = buf;
    psr->pktbuf_curr = buf;
    psr->pktbuf_left = buf_len;

    //printf("psr->pktbuf_curr1 is %p\n", psr->pktbuf_curr);
	//printf("psr->pktbuf_left1 is %u\n", psr->pktbuf_left);

    while((psr->pktbuf_left > 0) && (psr->parse_start == 1))
    {
	    if((psr->logging_flag == 1))
	    {
	    	//TODO log valid data payload
	    	idpi_tns_parse_skip_unconcerned_configure(psr);
	    	psr->logging_flag = 0;
	    }
	    else if(psr->wait_for_header == 1)
	    {
	    	psr->first_14_byte_last_cached_num = TNS_HEADER_AND_EXTENDED_LENGTH - psr->first_14_byte_hava_cached_num;
	    	//TODO wait for next buffer header
	    	if(psr->pktbuf_left + psr->first_14_byte_hava_cached_num >= TNS_HEADER_AND_EXTENDED_LENGTH)
	    	{
	    		psr->wait_for_header =0;

	    		idpi_tns_cache_header_and_extend(psr, buf, psr->first_14_byte_last_cached_num);

	    		psr->first_14_byte_cache_is_full = 1;
	    		printf("wait_for_header == 1 & NOW IS header is full, psr->wait_for_header =0\n");
			}
			else
			{
				if(psr->pktbuf_left + psr->first_14_byte_hava_cached_num >= TNS_HEADER_LENGTH)
		    	{
		    		idpi_tns_cache_header_and_extend(psr, buf, buf_len);
			    	idpi_tns_parse_header(psr);
			    	//->pktbuf_curr += TNE_HEADER_LENGTH; /* skip header*/
			        psr->pktbuf_left -= buf_len;

			        if(psr->content_type == TNS_TYPE_RESEND)//parse resend
			        {
			        	printf("wait_for_header == 1 & TNS_TYPE_RESEND\n");
			        	idpi_tns_parse_payload_resend(psr);
			        }
			        else
			        {
			        	printf("wait_for_header == 1 & NOW IS __IDPI_TNS_PARSE_STATE_WAIT_FOR_HEADER1\n");
			        }
			    }
			    else
			    {	
			    	psr->pktbuf_left -= buf_len;
			    	idpi_tns_cache_header_and_extend(psr, buf, buf_len);
			    	//psr->parse_state = __IDPI_TNS_PARSE_STATE_WAIT_FOR_HEADER;
			    	printf("wait_for_header == 1 & NOW IS __IDPI_TNS_PARSE_STATE_WAIT_FOR_HEADER2\n");
			    }
			}
	    }
	    else
	    {
		    //while(psr->pktbuf_left >= psr->first_14_byte_left)
	    	if(psr->skip_flag == 1)
		    {	
		    	idpi_tns_parse_skip_unconcerned_packet_buf(psr);//skip the whole remain of the packet
		    	psr->skip_flag = 0;
		    }
		    else
		    {
		    	if((psr->pktbuf_left >= TNS_HEADER_AND_EXTENDED_LENGTH) || psr->first_14_byte_cache_is_full == 1)
			    {
			        if(!idpi_tns_parse_header(psr))
			        {	
			        	if(psr->first_14_byte_cache_is_full == 0)
			        	{
			        		psr->pktbuf_curr += TNE_HEADER_LENGTH; /* skip header*/
							psr->pktbuf_left -= TNE_HEADER_LENGTH; /* skip header*/
			        	}
			        	else
			        	{
			        		psr->pktbuf_left -= psr->first_14_byte_last_cached_num;
	   						psr->pktbuf_curr += psr->first_14_byte_last_cached_num;
	   						printf("psr->first_14_byte_last_cached_num is %u\n", psr->first_14_byte_last_cached_num);
			        	}

			        	idpi_tns_parse_payload(psr);
			        	
			        	if(psr->first_14_byte_cache_is_full == 1)
		        		{
		        			psr->first_14_byte_cache_is_full = 0;
		        			psr->first_14_byte_hava_cached_num = 0;
		        			memset(psr->first_14_byte_cache_block, '\0', TNS_HEADER_AND_EXTENDED_LENGTH - 1);
		        		}
			        }
			    }
			    else
			    {	//int ret = 1;
			    	
			    	if(psr->pktbuf_left >= TNS_HEADER_LENGTH)
			    	{
				    	idpi_tns_parse_header(psr);
				        if(psr->content_type == TNS_TYPE_RESEND)//parse resend
				        {
				        	printf("TNS_TYPE_RESEND\n");
				        	psr->pktbuf_curr += TNE_HEADER_LENGTH; /* skip header*/
				        	psr->pktbuf_left -= TNE_HEADER_LENGTH;
				        	idpi_tns_parse_payload_resend(psr);
				        }
				        else
				        {
				        	psr->wait_for_header = 1;
				        	psr->pktbuf_left -= buf_len;
				        	idpi_tns_cache_header_and_extend(psr, buf, buf_len);
				        	printf("NOW IS wait_for_header1\n");
				        }
				    }
				    else
				    {
				    	psr->wait_for_header = 1;
				    	psr->pktbuf_left -= buf_len;
				    	idpi_tns_cache_header_and_extend(psr, buf, buf_len);
				        printf("NOW IS wait_for_header2\n");
				    }
			    }
		    }   
	    }
	    
    }
    printf("Buffer is empty & wait for next buffer\n");
    
    idpi_tns_print_header(psr);

    return 0;
}


int main()
{	
	printf("**********************************************************************************\n");
	printf("**********************************************************************************\n");
	printf("**********************************************************************************\n");
	printf("**********************************************************************************\n");
	printf("**********************************************************************************\n");
	printf("**********************************************************************************\n");
	printf("**********************************************************************************\n");
	printf("**********************************************************************************\n");
	idpi_tns_parser_t *ptr = (idpi_tns_parser_t*)malloc(sizeof(idpi_tns_parser_t));
	
	idpi_tns_parse_flow_init(ptr);

	//printf("buf_smallbuf2_len is %u\n", buf_smallbuf2_len);
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
		//buf_len = ((sizeof paste(buf, i)) / (sizeof (unsigned char)));
		//printf("buf_len is %u \n", buf_len[i]);
	}

	free(ptr);

	return 0;
}

int idpi_tns_print_pktbuf_curr_shift(idpi_tns_parser_t *psr)
{
	uint32_t shift;
	if(psr)
	{
		shift = psr->pktbuf_curr - psr->pktbuf_init;
	}
	else
		return -1;

	//printf("idpi_tns_print_pktbuf_curr_shift is %u\n", shift);

	return 0;
}

int idpi_tns_parse_flow_init(idpi_tns_parser_t *psr)
{
	if(psr)
	{
		psr->parse_state           = __IDPI_TNS_PARSE_STATE_INIT;
		psr->response_needed       = 0;
		psr->parse_state       	   = 0;
		psr->buf_num 			   = 0;
		psr->parse_start		   = 0; 
		psr->direction         	   = 3;
		psr->first_buf_direction   = 3;
		psr->data_flag         	   = 0;
		psr->id_subid          	   = 0;
		psr->id_subid_extended 	   = 0;
		psr->wait_for_header       = 0;
		psr->wait_for_request      = 0;
		psr->wait_for_response     = 0;
		psr->pktbuf_left           = 0;
		psr->skip_flag             = 0;
		psr->logging_flag          = 0;
		psr->logbuf_start          = 0;
		psr->logbuf_curr		   = 0;
		psr->first_14_byte_hava_cached_num = 0;
		psr->first_14_byte_cache_is_full   = 0;

		memset(psr->first_14_byte_cache_block, '\0', TNS_HEADER_AND_EXTENDED_LENGTH);
		int i = 0;
		for(i = 0; i < TNS_HEADER_AND_EXTENDED_LENGTH; i++)
		{
			printf("%x ", *(psr->first_14_byte_cache_block+i));
		}
		printf("\n");
	}
	else
		return -1;

	return 0;
}

int idpi_tns_print_header(idpi_tns_parser_t *psr)
{
    printf("**********************************************\n");
    printf("idpi_tns_parser_t print:\n");
    printf("*	psr->buf_num %u\n", psr->buf_num);
    printf("*	psr->parse_state %u\n", psr->parse_state);
    printf("*	psr->tns_version %u\n", psr->tns_version);
    printf("*	psr->content_type %u\n", psr->content_type);
    printf("*	psr->direction %u\n", psr->direction);
    printf("*	psr->tns_pkt_length %u\n", psr->tns_pkt_length);
    printf("*	psr->pktbuf_left %u\n", psr->pktbuf_left);
    printf("*	psr->skip_flag %u\n", psr->skip_flag);
    printf("*	psr->response_needed %u\n", psr->response_needed);
    printf("*	psr->wait_for_request %u\n", psr->wait_for_request);
    printf("*	psr->wait_for_response %u\n", psr->wait_for_response);
    printf("*	psr->wait_for_header %u\n", psr->wait_for_header);
    printf("*	psr->first_14_byte_cache_is_full %u\n", psr->first_14_byte_cache_is_full);
    printf("*	psr->first_14_byte_hava_cached_num %u\n", psr->first_14_byte_hava_cached_num);
    printf("**********************************************\n");

    return 0;
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

int idpi_tns_parse_skip_unconcerned_packet_buf(idpi_tns_parser_t *psr)
{
	if(psr)
	{
		printf("skipping ignore packer\n");
		if(psr->skip_flag == 1)
		{
			if(psr->pktbuf_left > psr->skip_left)
			{
				psr->pktbuf_left -= psr->skip_left;
				psr->pktbuf_curr += psr->skip_left;
				psr->skip_flag = 0;
			} 
			else
			{	
				psr->skip_left -= psr->pktbuf_left;
				psr->pktbuf_left = 0;
				psr->skip_flag = 1;
			}
		}
	}
	else
		return -1;
	idpi_tns_print_pktbuf_curr_shift(psr);
	return 0;
}

/*after parse header to skip the whole unconerned packet*/
int idpi_tns_parse_skip_unconcerned_configure(idpi_tns_parser_t *psr)
{
	if(psr)
	{
		
		if(psr->first_14_byte_cache_is_full == 1)
		{
			psr->skip_left = psr->tns_pkt_length - TNS_HEADER_AND_EXTENDED_LENGTH;
		}
		else
			psr->skip_left = psr->tns_pkt_length - TNE_HEADER_LENGTH;

		//printf("psr->skip_left is %u\n", psr->skip_left);
		//printf("psr->pktbuf_left is %u\n", psr->pktbuf_left);
		if(psr->pktbuf_left >= psr->skip_left)
		{
			psr->pktbuf_left -= psr->skip_left;
			psr->pktbuf_curr += psr->skip_left;
			psr->skip_left = 0;
			//printf("psr->pktbuf_left > psr->skip_left\n");
		}
		else
		{
			//printf("psr->pktbuf_left <= psr->skip_left\n");
			psr->skip_left -= psr->pktbuf_left;
			psr->pktbuf_left = 0;
			psr->skip_flag = 1;
		}
		//printf("psr->skip_left is %u\n", psr->skip_left);
	}

	idpi_tns_print_pktbuf_curr_shift(psr);

	return 0;
}

int idpi_tns_parse_payload_resend(idpi_tns_parser_t *psr)
{
	if(psr)
	{
		idpi_tns_parse_skip_unconcerned_configure(psr);
	}
	else
		return -1;

	return 0;
}

int idpi_tns_parse_payload_connect(idpi_tns_parser_t *psr)
{
	if(psr)
	{
		printf("idpi_tns_parse_payload_connect\n");
		idpi_tns_parse_skip_unconcerned_configure(psr);
	}
	else
		return -1;

	return 0;
}

int idpi_tns_parse_payload_accept(idpi_tns_parser_t *psr)
{
	if(psr)
	{
		unsigned char *parser_cursor = psr->pktbuf_curr;
		psr->tns_version = parser_cursor[0] << 8;
		psr->tns_version += parser_cursor[1];
		printf("psr->tns_version is %u\n", psr->tns_version);

		idpi_tns_parse_skip_unconcerned_configure(psr);
	}
	else
		return -1;

	return 0;
}

int idpi_tns_cache_header_and_extend(idpi_tns_parser_t *psr, unsigned char *buf, uint32_t buf_len)
{
	if(psr)
	{
		uint32_t cached_num = psr->first_14_byte_hava_cached_num;
		unsigned char *copyinit = psr->first_14_byte_cache_block + cached_num;

		memcpy(copyinit, buf, buf_len);
		psr->first_14_byte_hava_cached_num += buf_len;
		cached_num += buf_len;
		//psr->pktbuf_left -= buf_len;
		printf("cached_num is %u\n", cached_num);
		int i = 0;
		for(i = 0; i < cached_num; i++)
		{
			printf("%x ", *(psr->first_14_byte_cache_block+i));
		}
		printf("\n");

	}
	else
		return -1;

	return 0;
}

//return magicshift defined by <<TNS-differ-version>>
int idpi_tns_parse_locate_valid_payload(idpi_tns_parser_t *psr)
{	//TODO add response
	if(psr)
	{	
		int magicshift = 0;
		if(psr->content_type == TNS_TYPE_CONNECT)
		{
			magicshift = 0;
		}
		else if(psr->content_type == TNS_TYPE_DATA)
		{
			if(psr->tns_version == TNS_VERSION_312)
			{
				switch(psr->payload_data_type)
				{
					case TNS_DATA_SQL_COMMAND1:
						magicshift = TNS_312_MAGICSHIFT_TO_SQL_COMMAND1;
					case TNS_DATA_SQL_COMMAND2:
						magicshift = TNS_312_MAGICSHIFT_TO_SQL_COMMAND2;
					case TNS_DATA_USER_INFO:
						magicshift = TNS_312_MAGICSHIFT_TO_USER_INFO;
				}
			}
			else if(psr->tns_version == TNS_VERSION_312)
			{
				switch(psr->payload_data_type)
				{
					case TNS_DATA_SQL_COMMAND1:
						magicshift = TNS_313_MAGICSHIFT_TO_SQL_COMMAND1;
					case TNS_DATA_SQL_COMMAND2:
						magicshift = TNS_313_MAGICSHIFT_TO_SQL_COMMAND2;
					case TNS_DATA_USER_INFO:
						magicshift = TNS_313_MAGICSHIFT_TO_USER_INFO;
				}
			}
			else if(psr->tns_version == TNS_VERSION_312)
			{
				switch(psr->payload_data_type)
				{
					case TNS_DATA_SQL_COMMAND1:
						magicshift = TNS_314_MAGICSHIFT_TO_SQL_COMMAND1;
					case TNS_DATA_SQL_COMMAND2:
						magicshift = TNS_314_MAGICSHIFT_TO_SQL_COMMAND2;
					case TNS_DATA_USER_INFO:
						magicshift = TNS_314_MAGICSHIFT_TO_USER_INFO;
				}
			}
			else
			{
				switch(psr->payload_data_type)
				{
					case TNS_DATA_SQL_COMMAND1:
						magicshift = TNS_315_MAGICSHIFT_TO_SQL_COMMAND1;
					case TNS_DATA_SQL_COMMAND2:
						magicshift = TNS_315_MAGICSHIFT_TO_SQL_COMMAND2;
					case TNS_DATA_USER_INFO:
						magicshift = TNS_315_MAGICSHIFT_TO_USER_INFO;
				}
			}
		}
		psr->magicshift = magicshift;
		
		if(psr->first_14_byte_cache_is_full == 1)
			psr->magicshift_to_skip = magicshift - 6;
		else
			psr->magicshift_to_skip = magicshift;		
	}
	else
		return -1;

	return 0;
}
int idpi_tns_parse_alloc_logbuf(idpi_tns_parser_t *psr)
{
	if(psr->logging_flag == 1)
		return -1;

	//psr->logbuf_start = hili_send_module_fpa_alloc(); 
	psr->logbuf_start = malloc(TNS_LOG_BUFFER_MAXE_LENGTH); 
    if (!psr->logbuf_start)
    {
        //return psr->err_code = __IDPI_HTTP_PARSE_OUT_OF_MEM;
        return -1;
    }

    memset(psr->logbuf_start, 0, 16);
    psr->logbuf_curr = psr->logbuf_start + 16;

    *((uint16_t*)(psr->logbuf_start + 8)) = 0x1;/*type tns*/
    psr->logbuf_start[11] = 0x01;/* last segment default */
    *((uint16_t*)(psr->logbuf_start + 12)) = 0; /* segment count */

    return 0;

}

int idpi_tns_parse_send_logbuf(idpi_tns_parser_t *psr, unsigned char is_segmented, unsigned char is_last_segment)
{
    psr->logbuf_start[10] = is_segmented;
    psr->logbuf_start[11] = is_last_segment;
    *(uint16_t*)(psr->logbuf_start + 12) = (is_segmented?(++psr->segment_count):0); /* segment count */
    *(uint16_t*)(psr->logbuf_start + 14) = (psr->logbuf_curr - psr->logbuf_start - 16); /* log's variable content size */
    //hili_send_module_send_udp_packet(psr->logbuf_start, psr->logbuf_curr - psr->logbuf_start, &psr->send_conf);
    psr->logbuf_start = psr->logbuf_curr = 0;

    return 0;
}

int idpi_tns_parse_finish_log(idpi_tns_parser_t *psr)
{
    if (!psr->logbuf_start) 
    	return -1;
    return idpi_tns_parse_send_logbuf(psr, psr->segment_count, 1);
}

int idpi_tns_parse_create_log(idpi_tns_parser_t *psr)
{
    if (idpi_tns_parse_alloc_logbuf(psr))
    {
        return -1;
    }

    strcpy(psr->logbuf_curr, "2015-01-09-00-00-00");
    psr->logbuf_curr += 30; /* ignore login time field */ 

    uint16_t *var16p = (uint16_t*)psr->logbuf_curr;
    *var16p++ = psr->tns_version;
    *var16p++ = psr->client_port;
    *var16p++ = psr->server_port;

    uint32_t *var32p = (uint32_t*)var16p;
    *var32p++ = psr->client_ip;
    *var32p++ = psr->server_ip;

    psr->logbuf_curr = (unsigned char*)var32p;
    strcpy(psr->logbuf_curr, "2015-01-09-00-00-00");
    psr->logbuf_curr += 30; /* ignore login time field */ 
    strcpy(psr->logbuf_curr, "2015-01-09-00-00-00");
    psr->logbuf_curr += 30; /* ignore login time field */ 

    /* next thing to log is content data */ 

    return 0;
}

int idpi_tns_parse_free_logbuf(idpi_tns_parser_t *psr)
{
    free(psr->logbuf_start);
    psr->logbuf_start = psr->logbuf_curr = 0;
    return 0;
}

int idpi_tns_parse_log_process(idpi_tns_parser_t *psr)
{
	if(psr == NULL)
		return -1;
	if((psr->logbuf_curr == 0) && (psr->logbuf_start == 0))
		idpi_tns_parse_alloc_logbuf(psr);

	if((psr->pktbuf_left <= psr->magicshift_to_skip)
	{
		psr->magicshift_to_skip -= psr->pktbuf_left;
		psr->pktbuf_left = 0;
	}
	else
	{
		psr->pktbuf_left -= psr-> magicshift_to_skip;
		psr-> magicshift_to_skip = 0;
		//printf("psr->skip_left is %u\n", psr->skip_left);
		//printf("psr->pktbuf_left is %u\n", psr->pktbuf_left);
		uint16_t log_buffer_left = TNS_LOG_BUFFER_MAXE_LENGTH - (psr->logbuf_curr - psr->logbuf_start);
		if(psr->pktbuf_left >= psr->data_to_buff_left)
		{
			if(psr->data_to_buff_left < log_buffer_left)
			{
				memcpy(psr->logbuf_curr, psr->pktbuf_curr, psr->data_to_buff_left);
				psr->pktbuf_left -= psr->data_to_buff_left;
				psr->pktbuf_curr += psr->data_to_buff_left;
				psr->logbuf_curr += psr->data_to_buff_left;

				psr->data_to_buff_left = 0;
				psr->logging_flag = 0; //finish log?direction, 
			}
			else
			{
				memcpy(psr->logbuf_curr, psr->pktbuf_curr, log_buffer_left);
				psr->pktbuf_left -= log_buffer_left;
				psr->pktbuf_curr += log_buffer_left;
				psr->data_to_buff_left -= log_buffer_left;

				idpi_tns_parse_send_logbuf(psr, 1, 0);
			}
		}
		else
		{
			if(psr->pktbuf_left < log_buffer_left)
			{
				memcpy(psr->logbuf_curr, psr->pktbuf_curr, psr->pktbuf_left);
				psr->logbuf_curr += psr->pktbuf_left;
				psr->data_to_buff_left -= psr->pktbuf_left;

				psr->pktbuf_left = 0;	
			}
			else
			{
				memcpy(psr->logbuf_curr, psr->pktbuf_curr, log_buffer_left);
				psr->pktbuf_left -= log_buffer_left;
				psr->pktbuf_curr += log_buffer_left;
				psr->data_to_buff_left -= log_buffer_left;

				idpi_tns_parse_send_logbuf(psr, 1, 0);
			}
		}

		if((psr->data_to_buff_left == 0) && (psr->direction == 1))
		{
			if(psr->segment_count == 0)
				idpi_tns_parse_send_logbuf(psr, 0, 1);
			else
				idpi_tns_parse_send_logbuf(psr, 1, 1);
		}
	}
}

int idpi_tns_parse_payload_data(idpi_tns_parser_t *psr)
{//TODO add response
	//TODO
	if(psr)
	{//entry psr psr->pktbuf_curr should point to payload
		if((psr->direction == 1) && (psr->response_needed == 1))
		{
			unsigned char *parser_cursor = psr->pktbuf_curr + 2;
			
			int i = 0;
			printf("Payload is select response:\n");
			for(i = 0; i <= (psr->tns_pkt_length - 8 - 2 - 70); i++)
			{
				printf("%c", *(parser_cursor+70+i));
			}
			printf("\n");

			psr->response_needed = 0; 
			idpi_tns_parse_skip_unconcerned_configure(psr);// this should be replaced by copy to log
		}
		else if(psr->direction == 1)
		{
			printf("Payload is other content to be ignore\n");
			idpi_tns_parse_skip_unconcerned_configure(psr);
		}
		else// if(psr->direction == 1)
		{
			//TODO add Direction
			unsigned char *parser_cursor = psr->pktbuf_curr;

			psr->data_flag = parser_cursor[0] << 8;
			psr->data_flag += parser_cursor[1];
			//printf("psr->data_flag is %u\n", psr->data_flag);

			parser_cursor = parser_cursor + 2;
			psr->id_subid += parser_cursor[0] << 8;
			psr->id_subid += parser_cursor[1];
			//printf("psr->id_subid is %x\n", psr->id_subid);

			parser_cursor = parser_cursor + 2;
			psr->id_subid_extended += parser_cursor[0] << 8;
			psr->id_subid_extended += parser_cursor[1];
			//printf("psr->id_subid_extended is %x\n", psr->id_subid_extended);

			psr->payload_data_type = idpi_tns_parse_payload_data_type(psr->data_flag, psr->id_subid, psr->id_subid_extended);
		
			switch(psr->payload_data_type)
			{
				case TNS_DATA_SQL_COMMAND1://need add decide whether is selcet
					psr->response_needed = 1; //next response need to be logged
					
					psr->logging_flag = 1;
					idpi_tns_parse_create_log(psr);

					idpi_tns_parse_locate_valid_payload(psr);

					psr->data_to_buff_left = psr->tns_pkt_length - psr->magicshift;
					if((psr->pktbuf_left <= psr->magicshift_to_skip)
					{
						psr->magicshift_to_skip -= psr->pktbuf_left;
						psr->pktbuf_left = 0;
					}
					printf("psr->data_to_buff_left is %u\n", psr->data_to_buff_left);

					printf("Payload is SQL COMMAND1:\n%s\n", (parser_cursor+67));
					
					//idpi_tns_parse_skip_unconcerned_configure(psr);// this should be replaced by copy to log
					
					break;

				case TNS_DATA_SQL_COMMAND2://need add decide whether is selcet
					psr->response_needed = 1; //next response need to be logged
					printf("Payload is SQL COMMAND2:\n%s\n", (parser_cursor+55));
					idpi_tns_parse_skip_unconcerned_configure(psr);// this should be replaced by copy to log
					break;

				case TNS_DATA_USER_INFO:
					printf("Payload is USER INFO:\n%s\n", (parser_cursor+18));
					idpi_tns_parse_skip_unconcerned_configure(psr);// this should be replaced by copy to log			
					break;

				case TNS_DATA_ALL_OTHER:
					printf("Payload is TNS_DATA_ALL_OTHER to be ignore\n");	
					idpi_tns_parse_skip_unconcerned_configure(psr);
					break;
				//TODO other cases
			}

			psr->data_flag = 0;
			psr->id_subid = 0;
			psr->id_subid_extended = 0;

			return 0;
		}
	}
	else
		return -1;
	

	return 0;
}

int idpi_tns_parse_payload(idpi_tns_parser_t *psr)
{
	int ret = -1;
	if(psr)
	{
		switch(psr->content_type)
    	{
    		case TNS_TYPE_CONNECT:
    			printf("TNS_TYPE_CONNECT\n");
    			idpi_tns_print_pktbuf_curr_shift(psr);
    			ret = idpi_tns_parse_payload_connect(psr);
    			if(ret != -1)
    			{
    				psr->parse_state = __IDPI_TNS_PARSE_STATE_CONNECTING;
    				printf("psr->parse_state is %u\n", psr->parse_state);
    			}
    			break;

    		case TNS_TYPE_ACCEPT:
    			printf("TNS_TYPE_ACCEPT\n");
    			idpi_tns_print_pktbuf_curr_shift(psr);
    			ret = idpi_tns_parse_payload_accept(psr);
    			if(ret != -1)
    			{
    				psr->parse_state = __IDPI_TNS_PARSE_STATE_CONNECTED;
    				printf("psr->parse_state is %u\n", psr->parse_state);
    			}
    			break;
    		case TNS_TYPE_DATA:
    			printf("TNS_TYPE_DATA\n");
    			idpi_tns_print_pktbuf_curr_shift(psr);
    			idpi_tns_parse_payload_data(psr);
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

int idpi_tns_parse_header(idpi_tns_parser_t *psr)
{//do not change psr->pktbuf_curr
    //TODO
	if(psr)
	{
		if(psr->first_14_byte_hava_cached_num >= TNS_HEADER_LENGTH)
		{
			//printf("idpi_tns_parse_header in cached_block\n");
			unsigned char *tmp_cache_block = psr->first_14_byte_cache_block;
			psr->tns_pkt_length = tmp_cache_block[0] << 8;
		    psr->tns_pkt_length += tmp_cache_block[1];
		    psr->content_type = (idpi_tns_content_type_e )tmp_cache_block[4];
		    if(psr->tns_pkt_length != 0) //tns 312\313\314
		    {
				printf("psr->tns_pkt_length is %u\n", psr->tns_pkt_length);
				printf("psr->content_type is %u\n", psr->content_type);
			}
		    else
		    {
		        //tns 315
		    	psr->tns_pkt_length = tmp_cache_block[2]<<8;
		    	psr->tns_pkt_length += tmp_cache_block[3];
		        
		        return 0;
		    }
		}
		else
		{
			//printf("idpi_tns_parse_header in buffer\n");

			unsigned char *parser_cursor = psr->pktbuf_curr;
	    
		    psr->tns_pkt_length = parser_cursor[0] << 8;
		    psr->tns_pkt_length += parser_cursor[1];
		    psr->content_type = (idpi_tns_content_type_e )parser_cursor[4];
		    if(psr->tns_pkt_length != 0) //tns 312\313\314
		    {
				printf("psr->tns_pkt_length is %u\n", psr->tns_pkt_length);
				printf("psr->content_type is %u\n", psr->content_type);
			}
		    else
		    {
		        //tns 315
		    	psr->tns_pkt_length = parser_cursor[2]<<8;
		    	psr->tns_pkt_length += parser_cursor[3];
		        
		        return 0;
		    }
		}	
	}
	else
		return -1;

    return 0;
}

int idpi_tns_parse_right_start(idpi_tns_parser_t* psr, uint8_t direction)
{
	if(psr)
	{
		if(psr->buf_num == 0)
			psr->first_buf_direction = direction;
		else if((psr->first_buf_direction != direction) && (psr->parse_start == 0))
			psr->parse_start = 1;
		else
			return 0;
	}
	else
		return -1;

	return 0;
}


