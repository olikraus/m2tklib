/* 

  b_pl.c
  
  basic pointer list
  
*/

#include "b_pl.h"

#include <stdlib.h>
#include <assert.h>

#define B_PL_EXPAND 32  
  
int b_pl_init(b_pl_type b_pl)
{
  b_pl->cnt = 0;
  b_pl->max = 0;
  b_pl->link_cnt = 0;
  b_pl->list = (void **)malloc(B_PL_EXPAND*sizeof(void *));
  if ( b_pl->list == NULL )
    return 0;
  b_pl->max = B_PL_EXPAND;
  return 1;
}

int b_pl_expand(b_pl_type b_pl)
{
  void *ptr;
  ptr = realloc(b_pl->list, (b_pl->max+B_PL_EXPAND)*sizeof(void *));
  if ( ptr == NULL )
    return 0;
  b_pl->list = (void **)ptr;
  b_pl->max += B_PL_EXPAND;
  return 1;
}

void b_pl_destroy(b_pl_type b_pl)
{
  b_pl_Clear(b_pl);
  free(b_pl->list);
  b_pl->max = 0;
}

/*----------------------------------------------------------------------------*/

b_pl_type b_pl_Open()
{
  b_pl_type b_pl;
  b_pl = (b_pl_type)malloc(sizeof(b_pl_struct));
  if ( b_pl != NULL )
  {
    if ( b_pl_init(b_pl) != 0 )
    {
      return b_pl;
    }
    free(b_pl);
  }
  return NULL;
}

b_pl_type b_pl_Link(b_pl_type b_pl)
{
  b_pl->link_cnt++;
  return b_pl;
}

void b_pl_Close(b_pl_type b_pl)
{
  if ( b_pl->link_cnt > 0 )
  {
    b_pl->link_cnt--;
    return;
  }
  b_pl_destroy(b_pl);
  free(b_pl);
}

/* returns position or -1 */
int b_pl_Add(b_pl_type b_pl, void *ptr)
{
  while( b_pl->cnt >= b_pl->max )
    if ( b_pl_expand(b_pl) == 0 )
      return -1;
  
  b_pl->list[b_pl->cnt] = ptr;
  b_pl->cnt++;
  return b_pl->cnt-1;
}

void b_pl_DelByPos(b_pl_type b_pl, int pos)
{
  while( pos+1 < b_pl->cnt )
  {
    b_pl->list[pos] = b_pl->list[pos+1];
    pos++;
  }
  if ( b_pl->cnt > 0 )
    b_pl->cnt--;
}

/* 0: error */
int b_pl_InsByPos(b_pl_type b_pl, void *ptr, int pos)
{
  int i;
  if ( b_pl_Add(b_pl, ptr) < 0 )
    return 0;
  assert(b_pl->cnt > 0 );
  i = b_pl->cnt-1;
  while( i > pos + 4 )
  {
    b_pl->list[i-0] = b_pl->list[i-1];
    b_pl->list[i-1] = b_pl->list[i-2];
    b_pl->list[i-2] = b_pl->list[i-3];
    b_pl->list[i-3] = b_pl->list[i-4];
    i-=4;
  }
  while( i > pos )
  {
    b_pl->list[i] = b_pl->list[i-1];
    i--;
  }
  b_pl->list[i] = ptr;
  return 1;
}

int b_pl_While(b_pl_type b_pl, int *pos)
{
  if ( b_pl->cnt == 0 )
  {
    *pos = -1;
    return 0;
  }
  else if ( *pos < 0 )
  {
    *pos = 0;
    return 1;
  }
  else if ( *pos+1 >= b_pl->cnt )
  {
    *pos = -1;
    return 0;
  }
  (*pos)++;
  return 1;
}



/*----------------------------------------------------------------------------*/

size_t b_pl_GetMemUsage(b_pl_type b_pl)
{
  size_t m;
  m = sizeof(b_pl_struct);
  m += sizeof(void *)*b_pl->max;
  return m;
}
