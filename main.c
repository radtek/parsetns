#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "tns_parse.h"

int idpi_tns_parse_processing(idpi_tns_parser_t* tns_flow_ptr, unsigned char* buf, uint32_t buf_len, uint8_t direction);

int idpi_tns_parse_flow_init(idpi_tns_parser_t *psr)
{
	if(psr)
	{
		psr->parse_state        = __IDPI_TNS_PARSE_STATE_INIT;
		psr->first_14_byte_left = TNS_HEADER_AND_EXTENDED_LENGTH;
		psr->response_needed    = 0;
		psr->direction          = 1;
		psr->data_flag          = 0;
		psr->id_subid           = 0;
		psr->id_subid_extended  = 0;
	}
	else
		return -1;

	return 0;
}

int idpi_tns_print_header(idpi_tns_parser_t *psr)
{
        //printf("%d\n", psr->header_cache.type);

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

	return 0;
}
int idpi_tns_parse_skip_unconcerned_configure(idpi_tns_parser_t *psr)
{
	if(psr)
	{
		psr->skip_left = psr->tns_pkt_length - TNE_HEADER_LENGTH;

		if(psr->pktbuf_left >= psr->skip_left)
		{
			psr->pktbuf_left -= psr->skip_left;
			psr->pktbuf_curr += psr->skip_left;
			//printf("psr->pktbuf_left > psr->skip_left\n");
		}
		else
		{
			//printf("psr->pktbuf_left <= psr->skip_left\n");
			psr->skip_left -= psr->pktbuf_left;
			psr->pktbuf_left = 0;
			psr->skip_flag = 1;
		}
	}
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
int idpi_tns_parse_payload_data(idpi_tns_parser_t *psr)
{
	//TODO
	if(psr)
	{
		if((psr->direction == 1) && (psr->response_needed == 1))
		{
			unsigned char *parser_cursor = psr->pktbuf_curr + 2;
			
			int i = 0;
			printf("Payload is select response:\n");
			for(i = 0; i <= (psr->tns_pkt_length - 8 - 2 - 70); i++)
			{
				printf("%x", *(parser_cursor+70+i));
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
					printf("Payload is SQL COMMAND1:\n%s\n", (parser_cursor+67));
					idpi_tns_parse_skip_unconcerned_configure(psr);// this should be replaced by copy to log
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
int idpi_tns_parse_header(idpi_tns_parser_t *psr)
{
    //TODO
	if(psr)
	{	
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
	else
		return -1;

    return 0;
}

int idpi_tns_parse_processing(idpi_tns_parser_t* tns_flow_ptr, unsigned char* buf, uint32_t buf_len, uint8_t direction)
{
    
	uint8_t ret = -1;
	idpi_tns_parser_t *psr = (idpi_tns_parser_t *)tns_flow_ptr;
    psr->direction = direction;
    idpi_tns_state_e tmp_parse_state = psr->parse_state;
    //assert(psr);
    if((tmp_parse_state == __IDPI_TNS_PARSE_STATE_REQUESTING) || (tmp_parse_state == __IDPI_TNS_PARSE_STATE_RESPONDING))
    {
    	//TODO wait for next buffer data
    }
    else if(tmp_parse_state == __IDPI_TNS_PARSE_STATE_HEADERPARSING)
    {
    	//TODO wait for next buffer header
    }
    else
    {
    	psr->pktbuf_init = buf;
	    psr->pktbuf_curr = buf;
	    psr->pktbuf_left = buf_len;
	    //while(psr->pktbuf_left >= psr->first_14_byte_left)
	    while(psr->pktbuf_left > 0)
	    {
	    	if(psr->skip_flag == 1)
		    {
		    	idpi_tns_parse_skip_unconcerned_packet_buf(psr);//skip the whole remain of the packet
		    	psr->skip_flag = 0;
		    }
		    else
		    {
		    	if(psr->pktbuf_left >= psr->first_14_byte_left)
			    {
			        if(!idpi_tns_parse_header(psr))
			        {
			        	psr->pktbuf_curr += TNE_HEADER_LENGTH; /* skip header*/
			        	psr->pktbuf_left -= TNE_HEADER_LENGTH; /* skip header*/
			        	switch(psr->content_type)
			        	{
			        		case TNS_TYPE_CONNECT:
			        			printf("TNS_TYPE_CONNECT\n");
			        			ret = idpi_tns_parse_payload_connect(psr);
			        			if(ret != -1)
			        			{
			        				psr->parse_state = __IDPI_TNS_PARSE_STATE_CONNECTING;
			        				printf("psr->parse_state is %u\n", psr->parse_state);
			        			}
			        			break;

			        		case TNS_TYPE_ACCEPT:
			        			printf("TNS_TYPE_ACCEPT\n");
			        			ret = idpi_tns_parse_payload_accept(psr);
			        			if(ret != -1)
			        			{
			        				psr->parse_state = __IDPI_TNS_PARSE_STATE_CONNECTED;
			        				printf("psr->parse_state is %u\n", psr->parse_state);
			        			}
			        			break;
			        		case TNS_TYPE_DATA:
			        			printf("TNS_TYPE_DATA\n");
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
			    }
			    else
			    {	int ret = 1;
			    	idpi_tns_parse_header(psr);
			    	psr->pktbuf_curr += TNE_HEADER_LENGTH; /* skip header*/
			        psr->pktbuf_left -= TNE_HEADER_LENGTH;
			        if(psr->content_type == TNS_TYPE_RESEND)//parse resend
			        {
			        	printf("TNS_TYPE_RESEND\n");
			        	idpi_tns_parse_payload_resend(psr);
			        }
			        else if(psr->content_type == TNS_TYPE_DATA) //data type but length < 12
			        {
				        ;
			        }
			        else
			        {
			        	//TODO header or extended not complete
				        //printf("%s\n", psr->pktbuf_left<12);
			        }
			    }
		    }
	    }
    }
    printf("*************************\n");
    
    //idpi_tns_print_header(psr);
    return 0;
}

int main()
{
	idpi_tns_parser_t *ptr = (idpi_tns_parser_t*)malloc(sizeof(idpi_tns_parser_t));
	
	idpi_tns_parse_flow_init(ptr);

	int i =0;
	for(i = 0; i < BUFFER_NUM; i++)
	{
		if(i%2 == 0)
		{
			idpi_tns_parse_processing(ptr, buf[i], buf_len[i], 0);
		}
		else
		{
			idpi_tns_parse_processing(ptr, buf[i], buf_len[i], 1);
		}
		//buf_len = ((sizeof paste(buf, i)) / (sizeof (unsigned char)));
		//printf("buf_len is %u \n", buf_len[i]);
	}

	free(ptr);

	return 0;
}
