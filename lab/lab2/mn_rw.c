/*

  mn_rw.c
  
*/

#include <stdio.h>
#include "mn.h"

#define MN_FILE_DOWN_START 10
#define MN_FILE_DOWN_END 11

#define MN_N_FN_CODE 0
#define MN_N_USER_VAL 1

int mn_write_marker(FILE *fp, int m)
{
  if ( fputc(m, fp) == EOF )
    return 0;
  return 1;
}

int mn_write_int16(FILE *fp, int16_t x)
{
  if ( fputc((x&255), fp) == EOF )
    return 0;
  if ( fputc((x>>8)&255, fp) == EOF )
    return 0;
  return 1;
}

int mn_write_int32(FILE *fp, int16_t x)
{
  if ( fputc((x&255), fp) == EOF )
    return 0;
  if ( fputc((x>>8)&255, fp) == EOF )
    return 0;
  if ( fputc((x>>16)&255, fp) == EOF )
    return 0;
  if ( fputc((x>>24)&255, fp) == EOF )
    return 0;
  return 1;
}


int mn_write_node_member(FILE *fp, mn_type n)
{
  int16_t code;
  code = mn_GetCodeByFn(n->fn); 
  if ( code < 0 )
    return 0; 	/* internal error */
  
  if ( mn_write_marker(fp, MN_N_FN_CODE) == 0 )
    return 0;
  if ( mn_write_int16(fp, code) == 0 )
    return 0;
  
  return 1; 
}


int mn_write_node(FILE *fp, mn_type n)
{
  if ( mn_write_marker(fp, MN_FILE_DOWN_START) == 0 )
    return 0;
  if ( mn_write_node(fp, n->d) == 0 )
    return 0;
  if ( mn_write_marker(fp, MN_FILE_DOWN_END) == 0 )
    return 0;
  if ( mn_write_node_member(fp, n->d) == 0 )
    return 0;
  if ( mn_write_node(fp, n->n) == 0 )
    return 0;
  return 1;
}


