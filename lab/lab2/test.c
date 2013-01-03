

#include "mn.h"
#include "mr.h"
#include "m2.h"
#include "m2ghtty.h"

int main(void)
{
  mn_type n;
  int pos;
  
  if ( mrg_Init() == 0 )
    return 1;
  
  
  n = mn_OpenWithFn(mn_fn_m2_vlist);
  mn_AppendSubtree(n, mn_OpenWithFn(mn_fn_m2_label));
  mn_AppendSubtree(n, mn_OpenWithFn(mn_fn_m2_label));
  mn_AppendSubtree(n, mn_OpenWithFn(mn_fn_m2_u8num));
  mn_AppendSubtree(n, mn_OpenWithFn(mn_fn_m2_root));
  
  mn_SetArgStrByName(n->d, "Label", "abc");
  mn_SetArgStrByName(n->d->n, "Label", "xyz");
  mn_SetArgStrByName(n->d->n->n->n, "Label", "ok");
  mn_SetArgNodeByName(n->d->n->n->n, "Element",  n);
  
  mn_Show(n);
  mn_BuildCode(n);
  
  
  pos = mn_BuildRTE(n);
  mn_BuildRTEPost(n);

  m2_Init(mrg_GetM2Element(pos), NULL, NULL, m2_gh_tty);
  m2_Draw();
  return 0;
}

