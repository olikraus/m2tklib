/*

  mn_copy.c
  
*/

#include "mn.h"
#include <assert.h>
#include <stdlib.h>

mn_type mn_Copy(mn_type n)
{
  mn_type t;
  if ( n == NULL )
    return NULL;
  
  t = mn_OpenCopy(n);
  if ( t == NULL )
    return NULL;
    
  t->d = mn_Copy(n->d);
  t->n = mn_Copy(n->n);
  
  return t;
}

mn_type mn_CopySubtree(mn_type n, int pos)
{
  mn_type s, c;
  mn_type backup;
  
  s = mn_GetSubtree(n, pos);
  if ( s == NULL )
    return NULL;
  
  backup = s->n;
  s->n = NULL;
  c = mn_Copy(s);
  s->n = backup;
  
  return c;
  
}