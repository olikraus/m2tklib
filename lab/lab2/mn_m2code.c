/*

  mn_m2code.c 
  
*/
  
#include "stddef.h"
#include "mn.h"


void mn_BuildCodeStr(const char *s)
{
}


/* build M2_LIST with all sub elements */
void mn_BuildCodeList(mn_type n, char *name)
{
  n = n->d;
  mn_BuildCodeStr("M2_LIST(");
  mn_BuildCodeStr(name);
  mn_BuildCodeStr(") = {");
  while( n != NULL )
  {
    
  }
}