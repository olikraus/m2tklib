/*

  mn_m2code.c 
  
*/
  
#include "mn.h"

/* build M2_LIST with all sub elements */
int mn_BuildCodeList(mn_type n, char *name)
{
  n = n->d;
  mn_BuildCodeStr("M2_LIST(");
  mn_BuildCodeStr(name);
  mn_BuildCodeStr(") = {");
  while( n != NULL )
  {
  }
}