/*

  mn_show.c
  
*/


#include "mn.h"
#include <stdlib.h>
#include <stdio.h>

void mn_ShowRecur(mn_type n, int depth)
{
  char *name = "";
  int i;

  if ( n == NULL )
    return;
  
  n->fn(n, MN_MSG_GET_DISPLAY_STRING, &name);
  
  for( i = 0; i < depth; i++ )
    printf("  ");
  printf("%p %s arg_cnt:%d\n", n, name, n->arg_cnt);
  
  mn_Show(n->d);
  mn_Show(n->n);
}

void mn_Show(mn_type n)
{
  mn_ShowRecur(n, 0);
}
