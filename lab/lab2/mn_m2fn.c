

#include "mn.h"
#include "mr.h"
#include "m2.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

char mn_m2_buf[1024];


/*
int mn_GetRTEPos(mn_type n)
{
  int pos = -1;
  n->fn(n, MN_MSG_RTE, &pos);
  return pos;
}
*/

/*
  returns n->mr_list_pos if this is >= 0, otherwise rebuild list and assign it to n->mr_list_pos
*/
int mn_BuildRTEList(mn_type n)
{
  int cnt;
  int pos;
  int el_pos;
  void *el_ptr;
  mn_type d;
  
  if ( n == NULL )
    return -1;
  
  if ( n->mr_list_pos >= 0 )
    return n->mr_list_pos;

  /* calculate number of elements */
  cnt  = 0;
  d = n->d;
  while( d != NULL )
  {
    if ( d->fn(d, MN_MSG_IS_M2_ELEMENT, NULL) == 2)
      cnt++;
    d = d->n;
  }
  
  /* create list */
  pos = mrg_AddList(cnt);
  if ( pos < 0 )
    return -1;
  
  n->mr_list_pos = pos;
  
  /* add elements */
  cnt  = 0;
  d = n->d;
  while( d != NULL )
  {
    if ( d->fn(d, MN_MSG_IS_M2_ELEMENT, NULL) == 2)
    {
      if ( d->fn(d, MN_MSG_RTE, NULL) == 0 )
	return -1;
      el_pos = d->mr_element_pos;
      el_ptr = mrg_GetM2Element(el_pos);
      mrg_SetListElement(pos, cnt, el_ptr);
      cnt++;
    }
    d = d->n;
  }
  
  n->mr_list_len = cnt;
  
  return pos;
}

int mn_BuildRTEElement(mn_type n, size_t element_size, m2_el_fnptr fn, const char *fmt)
{
  int pos;
  m2_el_fnfmt_t *element;
  
  if ( n == NULL )
    return -1;
  
  if ( n->mr_element_pos >= 0 )
    return n->mr_element_pos;
  
  /* create object for the element */
  pos = mrg_AddElement(element_size);
  if ( pos < 0 )
    return -1;
  n->mr_element_pos = pos;

  /* fill in some default values */  
  element = (m2_el_fnfmt_t *)mrg_GetM2Element(pos);
  element->fn = fn;
  element->fmt = mrg_StoreStr(fmt);

  return pos;
}

/* build runtime object for container elments, which are based on m2_el_list */
int mn_BuildRTEListElement(mn_type n, m2_el_fnptr fn)
{
    if ( mn_BuildRTEList(n) < 0 )
      return 0;
    if ( mn_BuildRTEElement(n, sizeof(m2_el_list_t), fn, mn_GetRTEFmtStr(n)) < 0 )
      return 0;
    {
      m2_el_list_t *el = (m2_el_list_t *)mrg_GetM2Element(n->mr_element_pos);
      el->len = n->mr_list_len;
      el->el_list = (void *)mrg_GetM2List(n->mr_list_pos);
    }      
    return 1;
}

int mn_BuildRTE(mn_type n)
{
  mrg_Clear();
  if ( n->fn(n, MN_MSG_RTE, NULL) == 0 )
    return -1;
  return n->mr_element_pos;
}

void mn_BuildRTEPost(mn_type n)
{
  if ( n != NULL )
    return;
  n->fn(n, MN_MSG_RTE_POST, NULL);
  mn_BuildRTEPost(n->d);
  mn_BuildRTEPost(n->n);
}


int mn_fn_m2_base(mn_type mn, int msg, void *arg)
{
  switch(msg)
  {
    case MN_MSG_NONE:
      break;
    case MN_MSG_OPEN:
      mn_AddArg(mn, MN_ARG_T_U8, "x", 0, 1);
      mn_AddArg(mn, MN_ARG_T_U8, "X", 0, 1);
      mn_AddArg(mn, MN_ARG_T_U8, "y", 0, 1);
      mn_AddArg(mn, MN_ARG_T_U8, "Y", 0, 1);
      break;
    case MN_MSG_CLOSE:
      break;
    case MN_MSG_COPY:
      break;
    case MN_MSG_GET_LABEL_STRING:
      sprintf(mn_m2_buf, "l%04u", mn->seq_nr);
      *(char **)arg = mn_m2_buf;
      return 1;
    case MN_MSG_IS_M2_ELEMENT: 
      return 2;
  }
  return 1;
}


int mn_fn_m2_vlist(mn_type n, int msg, void *arg)
{
  switch(msg)
  {
    case MN_MSG_GET_DISPLAY_STRING:
      sprintf(mn_m2_buf, "VLIST");
      *(char **)arg = mn_m2_buf;
      return 1;
    case MN_MSG_C_CODE:
      mn_BuildCodeListStatement(n);
      mn_BuildCodeStr("M2_VLIST(");
      mn_BuildCodeLabel(n);
      mn_BuildCodeStr(", ");
      mn_BuildCodeStr(mn_GetFmtStr(n));
      mn_BuildCodeStr(", ");
      mn_BuildCodeListLabel(n);
      mn_BuildCodeStr(");\n");
      return 1;
    case MN_MSG_RTE:
      return mn_BuildRTEListElement(n, m2_el_vlist_fn);
  }
  return mn_fn_m2_base(n, msg, arg);
}

int mn_fn_m2_label(mn_type n, int msg, void *arg)
{
  static const char arg_label[] = "Label";
  switch(msg)
  {
    case MN_MSG_OPEN:
      mn_AddArg(n, MN_ARG_T_STR, arg_label, 0, 0);
      break;
    case MN_MSG_GET_DISPLAY_STRING:
      sprintf(mn_m2_buf, "LABEL");
      *(char **)arg = mn_m2_buf;
      return 1;
    case MN_MSG_C_CODE:
      mn_BuildCodeStr("M2_LABEL(");
      mn_BuildCodeLabel(n);
      mn_BuildCodeStr(", ");
      mn_BuildCodeStr(mn_GetFmtStr(n));
      mn_BuildCodeStr(", \"");
      mn_BuildCodeStr(mn_GetArgStrByName(n, arg_label));
      mn_BuildCodeStr("\");\n");
      return 1;
    case MN_MSG_RTE:
      if ( mn_BuildRTEElement(n, sizeof(m2_el_str_t), m2_el_label_fn, mn_GetRTEFmtStr(n)) < 0 )
	return 0;
      {
	m2_el_str_t *el = (m2_el_str_t *)mrg_GetM2Element(n->mr_element_pos);
	el->str = mrg_StoreStr(mn_GetArgStrByName(n, arg_label));
      }      
      return 1;
  }
  return mn_fn_m2_base(n, msg, arg);
}

int mn_fn_m2_root(mn_type n, int msg, void *arg)
{
  static const char arg_label[] = "Label";
  static const char arg_element[] = "Element";
  switch(msg)
  {
    case MN_MSG_OPEN:
      mn_AddArg(n, MN_ARG_T_STR, arg_label, 0, 0);
      mn_AddArg(n, MN_ARG_T_MN, arg_element, 0, 0);
      break;
    case MN_MSG_GET_DISPLAY_STRING:
      sprintf(mn_m2_buf, "ROOT");
      *(char **)arg = mn_m2_buf;
      return 1;
    case MN_MSG_C_CODE:
      mn_BuildCodeStr("M2_ROOT(");
      mn_BuildCodeLabel(n);
      mn_BuildCodeStr(", ");
      mn_BuildCodeStr(mn_GetFmtStr(n));
      mn_BuildCodeStr(", \"");
      mn_BuildCodeStr(mn_GetArgStrByName(n, arg_label));
      mn_BuildCodeStr("\", &");
      mn_BuildCodeLabel(mn_GetArgNodeByName(n, arg_element));
      mn_BuildCodeStr(");\n");
      return 1;
    case MN_MSG_RTE:
      if ( mn_BuildRTEElement(n, sizeof(m2_el_root_t), m2_el_root_fn, mn_GetRTEFmtStr(n)) < 0 )
	return 0;
      {
	m2_el_root_t *el = (m2_el_root_t *)mrg_GetM2Element(n->mr_element_pos);
	el->el_str.str = mrg_StoreStr(mn_GetArgStrByName(n, arg_label));
	el->element = NULL;
      }      
      return 1;
    case MN_MSG_RTE_POST:
      if ( n->mr_element_pos >= 0 )
      {
	mn_type ref;
	m2_el_root_t *el = (m2_el_root_t *)mrg_GetM2Element(n->mr_element_pos);
	ref = mn_GetArgNodeByName(n, arg_element); 
	if ( ref != NULL )
	  el->element = mrg_GetM2Element(ref->mr_element_pos);
      }  
      
      return 1;
  }
  return mn_fn_m2_base(n, msg, arg);
}

int mn_fn_m2_u8num(mn_type n, int msg, void *arg)
{
  static const char arg_variable[] = "Variable";
  static const char arg_min[] = "Min";
  static const char arg_max[] = "Max";
  switch(msg)
  {
    case MN_MSG_OPEN:
      mn_AddArg(n, MN_ARG_T_STR, arg_variable, 0, 0);
      mn_AddArg(n, MN_ARG_T_U8, arg_min, 0, 0);
      mn_AddArg(n, MN_ARG_T_U8, arg_max, 0, 0);
      break;
    case MN_MSG_GET_DISPLAY_STRING:
      sprintf(mn_m2_buf, "U8NUM");
      *(char **)arg = mn_m2_buf;
      return 1;
    case MN_MSG_C_CODE:
      mn_BuildCodeStr("M2_U8NUM(");
      mn_BuildCodeLabel(n);
      mn_BuildCodeStr(", ");
      mn_BuildCodeStr(mn_GetFmtStr(n));
      mn_BuildCodeStr(", ");
      mn_BuildCodeNum(mn_GetArgU8ByName(n, arg_min));
      mn_BuildCodeStr(", ");
      mn_BuildCodeNum(mn_GetArgU8ByName(n, arg_max));
      mn_BuildCodeStr(", &");
      mn_BuildCodeStr(mn_GetArgStrByName(n, arg_variable));
      mn_BuildCodeStr(");\n");
      return 1;
    case MN_MSG_RTE:
      if ( mn_BuildRTEElement(n, sizeof(m2_el_u8_ptr_t), m2_el_u8num_fn, mn_GetRTEFmtStr(n)) < 0 )
	return 0;
      {
	m2_el_u8_ptr_t *el = (m2_el_u8_ptr_t *)mrg_GetM2Element(n->mr_element_pos);
	el->u8.min = mn_GetArgU8ByName(n, arg_min);
	el->u8.max = mn_GetArgU8ByName(n, arg_max);
	el->val = mrg_AddU8();
	if ( el->val != NULL )
	  *(el->val) = 0;
      }      
      return 1;
  }
  return mn_fn_m2_base(n, msg, arg);
}

/*================================================*/
/* fn list */

struct _mn_fn_struct
{
  int16_t code;
  mn_fn fn;
};
typedef struct _mn_fn_struct mn_fn_struct;

mn_fn_struct mn_fn_list[] = 
{
  { 1000, mn_fn_m2_vlist },
  { 2000, mn_fn_m2_label },
  { 2010, mn_fn_m2_root },
  { 2020, mn_fn_m2_u8num }
};

int16_t mn_GetCodeByFn(mn_fn fn)
{
  int i, cnt;
  cnt = sizeof(mn_fn_list) / sizeof(mn_fn_struct);
  for( i = 0; i < cnt; i++ )
    if ( mn_fn_list[i].fn == fn )
      return mn_fn_list[i].code;
  return -1;
}

mn_fn mn_GetFnByCode(int16_t code)
{
  int i, cnt;
  cnt = sizeof(mn_fn_list) / sizeof(mn_fn_struct);
  for( i = 0; i < cnt; i++ )
    if ( mn_fn_list[i].code == code )
      return mn_fn_list[i].fn;
  return (mn_fn)0;
}

