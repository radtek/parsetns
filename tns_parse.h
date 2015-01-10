#define TNS_HEADER_AND_EXTENDED_LENGTH 14
#define TNS_HEADER_LENGTH 8
#define TNS_USER_NAME_LENGTH_MAX 100
#define TNE_HEADER_LENGTH 8
#define TNS_LOG_BUFFER_MAXE_LENGTH 180

#define BUFFER_NUM 11

#define LOG_LENGTH_MAX 


typedef struct 
{
	uint32_t packet_length;
	uint32_t packet_chksm;
	char type;
	char rsrvd;
	uint32_t header_chksm;
}idpi_tns_header_cache_t;

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
    TNS_VERSION_312 = 312,
	TNS_VERSION_313 = 313,
	TNS_VERSION_314 = 314,
	TNS_VERSION_315 = 315,
}idpi_tns_version_e;

typedef enum
{
	TNS_DATA_ALL_OTHER = 0,
    //client
	TNS_DATA_USER_INFO = 1,
	TNS_DATA_SQL_COMMAND1,
	TNS_DATA_SQL_COMMAND2,
	TNS_DATA_FETCH_MORE,
	TNS_DATA_CLOSE_SESSION,
    //server
    TNS_DATA_RESPONSE_SQL,
    TNS_DATA_RESPONSE_FETCH_MORE_WITHOUT_DATA,
    TNS_DATA_RESPONSE_FETCH_MORE_WITH_DATA,
    TNS_DATA_RESPONSE_CLOSE_SESSION,
}idpi_tns_payload_data_type_e;

typedef enum
{
	__IDPI_TNS_PARSE_STATE_INIT = 0, //
	__IDPI_TNS_PARSE_STATE_CONNECTING , //
	__IDPI_TNS_PARSE_STATE_CONNECTED , //
	__IDPI_TNS_PARSE_STATE_REDIRECTED , //
	__IDPI_TNS_PARSE_STATE_ERROR_MARKER , //
	__IDPI_TNS_PARSE_STATE_REQUESTING , //
	__IDPI_TNS_PARSE_STATE_REQUESTED , //
	__IDPI_TNS_PARSE_STATE_RESPONDING , //
	__IDPI_TNS_PARSE_STATE_RESPONDED , //
	//__IDPI_TNS_PARSE_STATE_SKIPING ,
}idpi_tns_state_e;

typedef struct
{
    idpi_tns_state_e parse_state;
    idpi_tns_version_e tns_version;
    idpi_tns_content_type_e content_type;

	char tns_user_name[TNS_USER_NAME_LENGTH_MAX-1];/*cache login user name*/

	uint32_t buf_num;
	uint8_t first_buf_direction;  
	uint8_t parse_start;
    uint8_t direction;//0:request 1:response
	
	uint16_t client_port;
	uint16_t server_port;
	uint32_t client_ip;
	uint32_t server_ip;

	uint16_t tns_pkt_length; /*bytes to read in pkt buffer*/
    uint8_t payload_data_type;

	uint8_t first_14_byte_cache_is_full;//1:used
	uint8_t first_14_byte_hava_cached_num;
	uint8_t first_14_byte_last_cached_num;
	unsigned char first_14_byte_cache_block[TNS_HEADER_AND_EXTENDED_LENGTH-1];//to cache firsr 14 byte 

    uint16_t data_flag;
    uint16_t id_subid;
    uint16_t id_subid_extended;
    uint16_t magicshift_to_skip;
    uint16_t magicshift;

    uint32_t pktbuf_left; /*bytes to read in pkt buffer*/
    unsigned char *pktbuf_curr; /*next addr to read in pkt buffer*/
	unsigned char *pktbuf_init; /*initial addr to read in pkt buffer*/

    uint8_t skip_flag; //1:skip
	uint32_t skip_left; /*skip pkt do not need to be parsed */

	uint8_t response_needed;//1:need to parse
	uint8_t wait_for_header;
	uint8_t wait_for_request;
	uint8_t wait_for_response;
	
    uint16_t data_to_buff_left;
    unsigned char *logbuf_start; /*start addr of log buffer*/
    unsigned char *logbuf_curr; /*next addr to write in log buffer*/ 
    uint8_t logging_flag;
    uint32_t segment_count;
}idpi_tns_parser_t;


int idpi_tns_print_pktbuf_curr_shift(idpi_tns_parser_t *psr);
int idpi_tns_parse_flow_init(idpi_tns_parser_t *psr);
int idpi_tns_print_header(idpi_tns_parser_t *psr);
uint8_t idpi_tns_parse_payload_data_type(uint16_t flag1, uint16_t flag2, uint16_t flag3);
int idpi_tns_parse_skip_unconcerned_packet_buf(idpi_tns_parser_t *psr);
int idpi_tns_parse_skip_unconcerned_configure(idpi_tns_parser_t *psr);
int idpi_tns_parse_payload_resend(idpi_tns_parser_t *psr);
int idpi_tns_parse_payload_connect(idpi_tns_parser_t *psr);
int idpi_tns_parse_payload_accept(idpi_tns_parser_t *psr);
int idpi_tns_cache_header_and_extend(idpi_tns_parser_t *psr, unsigned char *buf, uint32_t buf_len);
int idpi_tns_parse_payload_data(idpi_tns_parser_t *psr);
int idpi_tns_parse_header(idpi_tns_parser_t *psr);
int idpi_tns_parse_payload(idpi_tns_parser_t *psr);
int idpi_tns_parse_right_start(idpi_tns_parser_t* psr, uint8_t direction);

int idpi_tns_parse_processing(idpi_tns_parser_t* tns_flow_ptr, void* buf, uint32_t buf_len, uint8_t direction);

#define TNS_312_PARSE
#define TNS_313_PARSE
#define TNS_314_PARSE
#define TNS_315_PARSE

#ifdef TNS_312_PARSE
#include "tns_312.h"
#endif

#ifdef TNS_313_PARSE
#include "tns_313.h"
#endif

#ifdef TNS_314_PARSE
#include "tns_314.h"
#endif

#ifdef TNS_315_PARSE
#include "tns_315.h"
#endif
