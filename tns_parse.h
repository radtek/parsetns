#define TNS_CODE_DEVELOPING
#ifdef TNS_CODE_DEVELOPING
	#define TNS_HEADER_LENGTH_FIELD_NUM 5;
	typedef struct 
	{
		uint32_t packet_length;
		uint32_t packet_chksm;
		char type;
		char rsrvd;
		uint32_t header_chksm;
	}idpi_tns_header_t;

	typedef struct
	{
	    //idpi_tns_state_e parse_state;
	    //idpi_tns_version_e tns_version;
	    //idpi_tns_content_type_e content_type;
	    //uint8_t direction;
	//    uint16_t client_port;
	//    uint16_t server_port;
	//  uint32_t segment_count; /*log segment number*/
	 //   uint32_t client_ip;
	 //   uint32_t server_ip;
		uint32_t pktbuf_left; /*bytes to read in pkt buffer*/
		//uint32_t content_left; /*remaining bytes of message-body to read*/
	    //uint32_t content_current; /*remaining bytes of message-body to read*/
	 //   hili_send_packet_conf_t send_conf;
	    //char *response_header[TNS_HEADER_LENGTH-1]; /*restore the current header */
	    //char *logbuf_start; /*start addr of log buffer*/
	    //char *logbuf_curr; /*next addr to write in log buffer*/
	    char *pktbuf_curr; /*next addr to read in pkt buffer*/
	//  char *notep; /*current pos in current note*/
	    //char tns_data_cache_block[IDPI_TNS_MEMO_LEN_LIMIT]; /*memo*/
	    idpi_tns_header_t header_cache;
	}idpi_tns_parser_t;

	int idpi_tns_parse_processing(void* tns_flow_ptr, void* buf, uint32_t buf_len, uint8_t direction);
#endif






























































































































































































































