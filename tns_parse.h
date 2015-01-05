#define TNS_HEADER_LENGTH_FIELD_NUM 5;

typedef struct 
{
	uint32_t packet_length;
	uint32_t packet_chksm;
	char type;
	char rsrvd;
	uint32_t header_chksm;
}idpi_tns_header_t;

typedef enum
{
    TNS_TYPE_CONNECT = 1,
	TNS_TYPE_ACCEPT = 2,
	TNS_TYPE_ACK = 3,
	TNS_TYPE_REFUSE = 4,
	TNS_TYPE_REDIRECT = 5,
	TNS_TYPE_DATA = 6,
	TNS_TYPE_NULL = 7,
	TNS_TYPE_ABORT = 9,
	TNS_TYPE_RESEND = 11,
	TNS_TYPE_MARKER = 12,
	TNS_TYPE_ATTENTION = 13,
	TNS_TYPE_CONTROL = 14,
	TNS_TYPE_MAX = 19,
}idpi_tns_content_type_e;

typedef enum
{
    TNS_VERSION_312 = 1,
	TNS_VERSION_313 ,
	TNS_VERSION_314 ,
	TNS_VERSION_315 ,
}idpi_tns_version_e;

typedef enum
{
	TNS_DATA_ALL_OTHER = 0,
    //client
	TNS_DATA_USER_INFO = 1,
	TNS_DATA_SQL_COMMAND,
	TNS_DATA_FETCH_MORE,
	TNS_DATA_CLOSE_SESSION,
    //server
    TNS_DATA_RESPONSE_SQL,
    TNS_DATA_RESPONSE_FETCH_MORE_WITHOUT_DATA,
    TNS_DATA_RESPONSE_FETCH_MORE_WITH_DATA,
    TNS_DATA_RESPONSE_CLOSE_SESSION,
}idpi_tns_payload_data_type_e;

typedef struct
{
    //idpi_tns_state_e parse_state;
    idpi_tns_version_e tns_version;
    idpi_tns_content_type_e content_type;
    //uint8_t direction;
//    uint16_t client_port;
//    uint16_t server_port;
//  uint32_t segment_count; /*log segment number*/
 //   uint32_t client_ip;
 //   uint32_t server_ip;
	uint32_t pktbuf_left; /*bytes to read in pkt buffer*/
	uint16_t tns_pkt_length; /*bytes to read in pkt buffer*/
	//uint32_t content_left; /*remaining bytes of message-body to read*/
    //uint32_t content_current; /*remaining bytes of message-body to read*/
 //   hili_send_packet_conf_t send_conf;
    //char *response_header[TNS_HEADER_LENGTH-1]; /*restore the current header */
    //char *logbuf_start; /*start addr of log buffer*/
    //char *logbuf_curr; /*next addr to write in log buffer*/
    uint8_t payload_data_type;
    uint16_t data_flag;
    uint32_t id_subid;
    uint16_t id_subid_extended;
    unsigned char *pktbuf_curr; /*next addr to read in pkt buffer*/
//  char *notep; /*current pos in current note*/
    //char tns_data_cache_block[IDPI_TNS_MEMO_LEN_LIMIT]; /*memo*/
    idpi_tns_header_t header_cache;
}idpi_tns_parser_t;







