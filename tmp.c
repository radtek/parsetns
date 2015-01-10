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