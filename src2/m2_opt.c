/*
  
  mnuopt.c
  
  A parser for the string OPTions

  m2tklib = Mini Interative Interface Toolkit Library
  

  Syntax:
  options := {<cmd>}
  cmd:= <char> <arg>
  char:= single character
  arg:= <value>

  value
    a value from 0 to 255
    value 255 has a special meaning: it means: argument is not present, so "w255" is the same as ""
    

  known option for m2tklib version 2
  all values default to 0
    t		touchable
    o		focusable	0: no focus, 1: focus (block group if child cnt > 0)  (version 1: read only flag "r")

*/

#include "m2.h"

static const char *m2_parser_str;

//static void m2_parser_inc_str(void) M2_NOINLINE;
//static char m2_parser_get_c(void) M2_NOINLINE;

static void m2_parser_inc_str(void)
{
  m2_parser_str++;
}

static char m2_parser_get_c(void)
{
  return *m2_parser_str;
}

static uint8_t m2_parser_is_digit(void)
{
  register char c;
  c = m2_parser_get_c();
  if ( c >= '0' && c <= '9' )
    return 1;
  return 0;
}

static void m2_parser_skip_space(void)
{
  register char c;
  for(;;)
  {
    c = m2_parser_get_c();
    if ( c == '\0' )
      break;
    if ( c > ' ')
      break;
    m2_parser_inc_str();
  }
}

static uint8_t m2_parser_get_val(void)
{
  register uint8_t val = 0;
  register int8_t c;
  for(;;)
  {
    c = m2_parser_get_c();
    if ( c >= '0' && c <= '9' )
    {
      val *= 10;
      val += c;
      val -= '0';
    }
    else
      break;
    m2_parser_inc_str();
  }
  m2_parser_skip_space();
  return val;
}

static void m2_parser_skip_cmd(void)
{
  /* skip command byte */
  m2_parser_inc_str();
  m2_parser_skip_space();
  if ( m2_parser_is_digit() )
    m2_parser_get_val();
}

static uint8_t m2_parser_find_cmd(char cmd)
{
  register char c;
  m2_parser_skip_space();  
  for(;;)
  {
    c = m2_parser_get_c();
    if ( c == '\0' )
      return 0;
    if ( c == cmd )
      return 1;
    m2_parser_skip_cmd();
  }
}


/*==============================================================*/

void m2_parser_set_str(const char *str)
{
  m2_parser_str = str;
}

uint8_t m2_parser_get_cmd_val(char cmd, uint8_t not_found_val)
{
  if ( m2_parser_find_cmd(cmd) != 0 )
  {
    m2_parser_inc_str();
    m2_parser_skip_space();
    return m2_parser_get_val();
  }
  return not_found_val;
}


/*==============================================================*/

uint8_t m2_opt_get_val_any_default(const char *str, char cmd, uint8_t default_value)
{
  if ( str == NULL )
    return default_value;
  m2_parser_set_str(str);
  return m2_parser_get_cmd_val(cmd, default_value);
}

uint8_t m2_opt_get_val_zero_default(const char *str, char cmd)
{
  return m2_opt_get_val_any_default(str, cmd, 0);
}


/*==============================================================*/

