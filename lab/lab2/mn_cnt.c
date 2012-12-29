/*

  mn_cnt.c

*/

#include <stddef.h>
#include "mn.h"

/* return number of sub elements */
int mn_GetCnt(mn_type n)
{
  int cnt = 0;
  n = n->d;
  while( n != NULL )
  {
    cnt++;
    n = n->n;
  }
  return cnt;
}

