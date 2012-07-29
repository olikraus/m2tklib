/*

  mn_del.c
  
*/


#include "mn.h"
#include <assert.h>
#include <stdlib.h>


void mn_Del(mn_type n)
{
  if ( n == NULL )
    return;
  mn_Del(n->d);
  mn_Del(n->n);
  mn_Close(n);
}

/*
  1) remove subtree at pos
  2) return the removed subtree
  3) update subtree with parent n
*/
mn_type mn_CutSubtree(mn_type n, int pos)
{
  mn_type r;
  
  r = mn_GetSubtree(n, pos);
  if ( r == NULL )
    return NULL;
  
  if ( pos == 0 )
  {
    n->d = r->n;
  }
  else
  {
    mn_type p;
    p = mn_GetSubtree(n, pos-1);
    p->n = r->n;
  }
  r->n = NULL;
  return n;
}

void mn_DelSubtree(mn_type n, int pos)
{
  mn_Del(mn_CutSubtree(n, pos));
}

