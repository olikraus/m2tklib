/*

  mn_arg.c
  
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mn.h"

void mn_arg_init(mn_arg_struct *a)
{
  a->name[0] = '\0';
  a->user_val = 0;
  a->user_str = NULL;
  a->default_val = 0;
  a->is_fmt = 0;
  a->is_enable = 0;
  a->t = MN_ARG_T_BOOL;
}

int mn_AddArgPlain(mn_type n)
{
  void *ptr;
  if ( n->arg_cnt == 0 )
  {
    if ( n->arg_list != NULL )
      free(n->arg_list);
    n->arg_list = (mn_arg_struct *)malloc(sizeof(mn_arg_struct));
    if ( n->arg_list == NULL )
      return -1;
    mn_arg_init(n->arg_list);
    n->arg_cnt = 1;
  }
  else
  {
    if ( n->arg_list == NULL )
    {
      ptr = malloc(sizeof(mn_arg_struct));
      if ( ptr == NULL )
	return -1;
      n->arg_cnt = 0;
    }
    else
    {
      ptr = realloc(n->arg_list, sizeof(mn_arg_struct)*(n->arg_cnt+1));
      if ( ptr == NULL )
	return -1;
    }
    n->arg_list = (mn_arg_struct *)ptr;
    mn_arg_init(n->arg_list+n->arg_cnt);
    n->arg_cnt++;
  }
  return n->arg_cnt - 1;
}

int mn_FindArgByName(mn_type n, const char *name)
{
  int pos;
  for( pos = 0; pos < n->arg_cnt; pos++ )
  {
    if ( strcmp(n->arg_list[pos].name, name) == 0 )
      return pos;
  }
  return -1;
}

const char *mn_GetArgStrByName(mn_type n, const char *name)
{
  int pos = mn_FindArgByName(n, name);
  if ( pos < 0 )
    return NULL;
  if ( n->arg_list[pos].t != MN_ARG_T_STR )
    return NULL;
  return n->arg_list[pos].user_str;
}


void mn_SetArg(mn_type n, int pos, int t, const char *name, unsigned long default_val, int is_fmt)
{
  strncpy(n->arg_list[pos].name, name, MN_ARG_NAME_LEN);
  n->arg_list[pos].name[MN_ARG_NAME_LEN-1] = '\0';	
  n->arg_list[pos].user_val = default_val;
  n->arg_list[pos].user_str = NULL;
  n->arg_list[pos].default_val = default_val;
  n->arg_list[pos].is_enable = 0;
  n->arg_list[pos].is_fmt = is_fmt;
  n->arg_list[pos].t = t;
}

/*
  name: The short name for the argument, a signle char for options of the format string
*/
int mn_AddArg(mn_type n, int t, const char *name, unsigned long default_val, int is_fmt)
{
  int pos;
  pos = mn_AddArgPlain(n);
  if ( pos < 0 )
    return pos;
  mn_SetArg(n, pos, t, name, default_val, is_fmt);
  return pos;
}

const char *mn_GetFmtStr(mn_type n)
{
  static char buf[1024];
  int i;
  strcpy(buf, "\"");
  for( i = 0; i < n->arg_cnt; i++ )
  {
    if ( n->arg_list[i].is_fmt == 0 )
    {
      if ( n->arg_list[i].is_enable != 0 )
      {
	strcat(buf, n->arg_list[i].name);
	sprintf(buf+strlen(buf), "%d", (int)(n->arg_list[i].user_val));
      }
    }
  }
  strcat(buf, "\"");
  if ( strlen(buf) == 2 )
    return "NULL";
  return buf;
}

const char *mn_GetRTEFmtStr(mn_type n)
{
  static char buf[1024];
  int i;
  strcpy(buf, "");
  for( i = 0; i < n->arg_cnt; i++ )
  {
    if ( n->arg_list[i].is_fmt == 0 )
    {
      if ( n->arg_list[i].is_enable != 0 )
      {
	strcat(buf, n->arg_list[i].name);
	sprintf(buf+strlen(buf), "%d", (int)(n->arg_list[i].user_val));
      }
    }
  }
  return buf;
}
