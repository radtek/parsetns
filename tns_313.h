//request:after header + data flag + ID SubID + extended
#define TNS_313_MAGICSHIFT_TO_USER_INFO (12+18)
#define TNS_313_MAGICSHIFT_TO_SQL_COMMAND1 (12+67)
#define TNS_313_MAGICSHIFT_TO_SQL_COMMAND2 (12+55)
//response:after header + data falg
#define TNS_312_MAGICSHIFT_TO_RESPONSE_USER_INFO (10+8)
#define TNS_313_MAGICSHIFT_TO_RESPONSE_SQL (10+70)
#define TNS_313_MAGICSHIFT_TO_RESPONSE_FETCH_MORE_valid (10+24)
#define TNS_313_MAGICSHIFT_TO_RESPONSE_FETCH_MORE_invalid 0
