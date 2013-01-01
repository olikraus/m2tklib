

#include "mn.h"
#include "mr.h"

int main(void)
{
  mn_type n;
  
  if ( mrg_Init() == 0 )
    return 1;
  
  n = mn_OpenWithFn(mn_fn_m2_vlist);
  mn_Show(n);
  mn_BuildCode(n);
  return 0;
}

