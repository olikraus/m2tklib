/*

  mn_get.c

*/


#include "mn.h"
#include <stdlib.h>

/*
  assumes that the provided node is the first node if a chain connected with ->n
  returns the n-th node.
  mn_GetN(n, 0)  return n
  mn_GetN(n, 1)  return n->n
  mn_GetN(n, 2)  return n->n->n
  NULL is returned if the node does not exist
*/
mn_type mn_GetN(mn_type n, int pos)
{
  while( pos > 0 && n != NULL )
  {
    n = n->n;
    pos--;
  }
  return n;
}

/*
  returns child at pos
  mn_GetSubtree(n,0) returns n->d;
  mn_GetSubtree(n,1) returns n->d->n;
  mn_GetSubtree(n,2) returns n->d->n->n;
*/
mn_type mn_GetSubtree(mn_type n, int pos)
{
  if ( n == NULL )
    return NULL;
  return mn_GetN(n->d, pos);
}