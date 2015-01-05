#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "tns_parse.h"

#ifdef TNS_CODE_DEVELOPING
	/**
	 * @brief Test if text starts with prefix
	 *
	 * @param text To be matched
	 * @param prefix Must be terminated by \0
	 *
	 * @return text if matched, 0 otherwise 
	 */
	static char *idpi_tns_match_prefix(char *text, char *prefix)
	{
	    char *p = text, *q = prefix;
	    while (*q != '\0')
	    {
	        if (*p != *q) 
	        {
	            return NULL;
	        }
	        p++, q++;
	    }
	    return text;
	}

	int idpi_tns_print_header(idpi_tns_parser_t *psr)
	{
		printf("%d\n", psr->header_cache.type);

		return 0;
	}

	int idpi_tns_parse_header(idpi_tns_parser_t *psr)
	{
		//TODO
		char parser_cursor = psr->pktbuf_curr;
		char *cache_tmp = null;

		memset(cache_tmp,'\0',sizeof(cache_tmp));
		memcpy(cache_tmp, parser_cursor, 2);
		if(strcmp(buff,"") != 0) //tns 312\313\314
		{
			memcpy(psr->header_cache.packet_length, parser_cursor, 2);
			memcpy(psr->header_cache.type, parser_cursor+4, 1);
		}
		else
		{
			//tns 315
			return 0;
		}

		return 0;
	}

	int idpi_tns_parse_processing(void* tns_flow_ptr, void* buf, uint32_t buf_len, uint8_t direction)
	{
	    idpi_tns_parser_t *psr = (idpi_tns_parser_t *)tns_flow_ptr;
	    uint8_t tmp_direction = direction;
	    //assert(psr);
	    psr->pktbuf_curr = (char *)buf;
		psr->pktbuf_left = buf_len;
		if(psr->pktbuf_left >= 12)
		{
			idpi_tns_parse_header(idpi_tns_parser_t *psr);
		}
		else
		{
			//TODO
			printf("%s\n", psr->pktbuf_left<12);
			return 0;
		}
		idpi_tns_print_header(idpi_tns_parser_t *psr);

		return 0;
	}

#endif