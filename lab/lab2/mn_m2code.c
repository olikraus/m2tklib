/*

  mn_m2code.c 
  
*/
  
#include "stddef.h"
#include "mn.h"


void mn_BuildCodeStr(const char *s)
{
}

void mn_BuildCodeListLabel(mn_type n)
{
  mn_BuildCodeStr("list_");
  mn_BuildCodeStr(mn_GetLabelString(n));  
}


/* build M2_LIST with all sub elements */
void mn_BuildCodeList(mn_type n)
{
  int is_first = 1;
  mn_BuildCodeStr("M2_LIST(");
  mn_BuildCodeListLabel(n);
  mn_BuildCodeStr(") = {");
  
  n = n->d;
  while( n != NULL )
  {
    if ( is_first == 0 )
    {
      mn_BuildCodeStr(",");
    }
    is_first = 0;
    mn_BuildCodeStr("&");
    mn_BuildCodeStr(mn_GetLabelString(n));
  }
  mn_BuildCodeStr("};\n");
}


