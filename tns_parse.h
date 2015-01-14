#define IDPI_TNS_MEMO_LEN_LIMIT 60
#define IDPI_TNS_MAX_NUM_CACHE_BLOCK 8
#define IDPI_TNS_LOGBUF_LEN_MAX 1472
#define USERNAME_MAX_LENGTH 100

#define DIR_REQUEST 0
#define DIR_RESPONSE 1

#define WAIT_FOR_BUFFER 1
#define MESSAGE_IS_COMPLETE 0

//#define HILI_DEBUG

enum 
{
    ERROR = -1, 
    COMPLETE = 0,
};

enum 
{
    SET = 1, 
    UNSET = 0,
};

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

static char *content_type_array[] = 
{
    "",
    "TNS_TYPE_CONNECT ", 
    "TNS_TYPE_ACCEPT", 
    "TNS_TYPE_ACK", 
    "TNS_TYPE_REFUSE", 
    "TNS_TYPE_REDIRECT",
    "TNS_TYPE_DATA", 
    "TNS_TYPE_NULL",
    "",
    "TNS_TYPE_ABORT", 
    "",
    "TNS_TYPE_RESEND ", 
    "TNS_TYPE_MARKER", 
    "TNS_TYPE_ATTENTION ", 
    "TNS_TYPE_CONTROL", 
    "", "", "", "", 
    "TNS_TYPE_MAX "
};

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
    //__IDPI_TNS_PARSE_STATE_REQUESTING , //
    __IDPI_TNS_PARSE_STATE_REQUESTED , //
    //__IDPI_TNS_PARSE_STATE_RESPONDING , //
    __IDPI_TNS_PARSE_STATE_RESPONDED , //
    __IDPI_TNS_PARSE_STATE_REFUSED ,
}idpi_tns_state_e;

static char *parse_state_array[] = 
{
    "__IDPI_TNS_PARSE_STATE_INIT",
    "__IDPI_TNS_PARSE_STATE_CONNECTING ", 
    "__IDPI_TNS_PARSE_STATE_CONNECTED", 
    "__IDPI_TNS_PARSE_STATE_REDIRECTED", 
    "__IDPI_TNS_PARSE_STATE_ERROR_MARKER", 
    "__IDPI_TNS_PARSE_STATE_REQUESTED",
    "__IDPI_TNS_PARSE_STATE_RESPONDED", 
    "__IDPI_TNS_PARSE_STATE_REFUSED"
};

typedef struct
{
    idpi_tns_state_e parse_state;
    idpi_tns_version_e tns_version;
    idpi_tns_content_type_e content_type;
	
    uint8_t direction;
    uint8_t wait_flag;

	uint16_t client_port;
	uint16_t server_port;
	uint32_t client_ip;
	uint32_t server_ip;

	uint16_t tns_pkt_length; /*bytes to read in pkt buffer*/

    uint32_t pktbuf_left; /*bytes to read in pkt buffer*/
    uint8_t *pktbuf_curr; /*next addr to read in pkt buffer*/
	uint8_t *pktbuf_init; /*initial addr to read in pkt buffer*/

    uint8_t *logbuf_start; /*start addr of log buffer*/
    uint8_t *logbuf_curr; /*next addr to write in log buffer*/ 
    uint32_t segment_count;

    uint8_t *username[USERNAME_MAX_LENGTH];

    uint8_t request_curr_cache_block;
    uint8_t *request_cache_block_p[IDPI_TNS_MAX_NUM_CACHE_BLOCK]; /*start p of current block*/
    uint32_t request_cached_num;
    uint32_t request_curr_cache_block_left;

    uint8_t response_curr_cache_block;
    uint8_t *response_cache_block_p[IDPI_TNS_MAX_NUM_CACHE_BLOCK]; /*start p of current block*/
    uint32_t response_cached_num;
    uint32_t response_curr_cache_block_left;
}idpi_tns_parser_t;

typedef struct{
    uint16_t cli_port;
    uint16_t srv_port;
    uint32_t cli_ip;
    uint32_t srv_ip;
}tns_parse_flow_conf_t;

static inline void *idpi_tns_alloc_parser()
{
    //return cvmx_fpa_alloc(0);
    return (void *)malloc(2048);
}

static inline void idpi_tns_free_parser(void *ptr)
{
    //cvmx_fpa_free(ptr, 0, 2048/128);
    free(ptr);
}

static inline void *idpi_tns_alloc_memo()
{
    //return cvmx_fpa_alloc(0);
    return (void *)malloc(IDPI_TNS_MEMO_LEN_LIMIT);
}

static inline void idpi_tns_free_memo(void *ptr)
{
    //cvmx_fpa_free(ptr, 0, 2048/128);
    free(ptr);
}

extern void* idpi_tns_parse_flow_init();
extern int idpi_http_parse_kill_flow(void* tns_flow_ptr);
extern int idpi_tns_parse_processing(idpi_tns_parser_t* tns_flow_ptr, void* buf, uint32_t buf_len, uint8_t direction);