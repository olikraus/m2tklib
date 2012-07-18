/*

  lab_element.c

*/


#include <string.h>
#include "lab.h"
#include "m2.h"

el_def_struct el_def_list[EL_DEF_CNT] = {
        { 1, "M2_ALIGN", "|1-1w0h0W0H0x0y0", EL_DEF_PARENT_MODE_ONE_CHILD,
          { {0, ""}, {0, ""}, {0, ""}, {0, ""}, {0, ""}, {0, ""}, {0, ""}, {0, ""} } },
        { 1, "M2_BUTTON", "f0r0w0h0x0y0", EL_DEF_PARENT_MODE_NONE,
          { {EL_ARG_STR, "Label:"}, {EL_ARG_FN, "Procedure name:"}, {EL_ARG_FN_CODE, "Code:"}, {0, ""}, {0, ""}, {0, ""}, {0, ""}, {0, ""} } },
        { 1, "M2_COMBO", "f0r0w0h0x0y0", EL_DEF_PARENT_MODE_NONE,
          { {EL_ARG_U8PTR, "Variable:"}, {EL_ARG_U8PTR_DEFAULT, "Initial value:"}, {EL_ARG_NUM, "Count:"}, {EL_ARG_FN, "Procedure name:"}, {EL_ARG_FN_CODE, "Code:"}, {0, ""}, {0, ""}, {0, ""} } },
        { 1, "M2_GRIDLIST", "c1x0y0", EL_DEF_PARENT_MODE_UNLIMITED,
          { {0, ""}, {0, ""}, {0, ""}, {0, ""}, {0, ""}, {0, ""}, {0, ""}, {0, ""} } },
};

int el_def_find_by_type(const char *type_name)
{
        int i, cnt;
        cnt = EL_DEF_CNT;
        for( i = 0; i < EL_DEF_CNT; i++ )
        {
                if ( strcmp(el_def_list[i].type_name, type_name) == 0 )
                {
                        return i;
                }
        }
        return 1;		/* not found */
}

static const char *el_def_get_fmt_full_name(char c)
{
  if ( c == '|' ) return "Vert. align (0=bottom, 1=center, 2=top):";
  if ( c == '-' ) return "Hori. align (0=left, 1=center, 2=right):";
  if ( c == 'c' ) return "Columns: ";
  if ( c == 'f' ) return "Font Number: ";
  if ( c == 'r' ) return "Readonly (0=false, 1=true): ";
  if ( c == 'w' ) return "Width: ";
  if ( c == 'h' ) return "Height: ";
  if ( c == 'x' ) return "X-Position: ";
  if ( c == 'y' ) return "Y-Position: ";
  return "";
}


int el_def_get_arg_cnt(int el_idx)
{
  int arg_cnt = 0;
  for(;;)
  {
    if ( el_def_list[el_idx].args[arg_cnt].arg_type == 0 )
      break;
    arg_cnt++;
  }
  return arg_cnt;
}

int el_def_get_field_cnt(int el_idx)
{
  int options; 
  int arg_cnt;
  options = m2_get_cmd_cnt(el_def_list[el_idx].opt_with_default_values);
  arg_cnt = 0;
  for(;;)
  {
    if ( el_def_list[el_idx].args[arg_cnt].arg_type == 0 )
      break;
    arg_cnt++;
  }
  return 1+ options + arg_cnt ;
}

char el_def_get_nth_cmd(int el_idx, int opt_idx)
{
    return m2_get_nth_cmd(el_def_list[el_idx].opt_with_default_values, opt_idx );
}

/* fld_idx: 0..el_def_get_field_cnt(el_idx)-1 */
const char *el_def_get_field_label_by_index(int el_idx, int fld_idx)
{
  int options; 
  if ( fld_idx == 0 )
    return "Name: ";
  fld_idx --;
  options = m2_get_cmd_cnt(el_def_list[el_idx].opt_with_default_values);
  if ( fld_idx  < options )
  {
    char c;
    c = m2_get_nth_cmd(el_def_list[el_idx].opt_with_default_values, fld_idx );
    return el_def_get_fmt_full_name(c);
  }    
  fld_idx -= options;
  return el_def_list[el_idx].args[fld_idx].arg_label; 
}

/* return one of EL_ARG_xxx values */
int el_def_get_field_arg_type_by_index(int el_idx, int fld_idx)
{
  int options; 
  if ( fld_idx == 0 )
    return EL_ARG_STR;
  fld_idx --;
  options = m2_get_cmd_cnt(el_def_list[el_idx].opt_with_default_values);
  if ( fld_idx  < options )
  {
    return EL_ARG_NUM;
  }
  fld_idx -= options;
  return el_def_list[el_idx].args[fld_idx].arg_type; 
}
