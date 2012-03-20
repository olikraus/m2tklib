

#ifndef _UTL_H
#define _UTL_H

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

#define UTL_STRING_CONVERSION_DATA_LEN 6

extern char m2_utl_string_conversion_data[UTL_STRING_CONVERSION_DATA_LEN];


const char *m2_utl_u8dp(char *dest, uint8_t v);
const char *m2_utl_u8d(uint8_t v, uint8_t d);

#ifdef __cplusplus
}
#endif


#endif
