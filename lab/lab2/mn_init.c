/*

  mn_init.c
  
*/


#include "mn.h"
#include <stdlib.h>

mn_type mn_Open(void)
{
  mn_type mn = (mn_type)malloc(sizeof(struct _mn_struct));
  if ( mn != NULL )
  {
    mn->d = NULL;
    mn->n = NULL;
    mn->fn = mn_fn_empty;
    mn->data = NULL;
    return mn;
  }
  return NULL;
}

/*
  Destroys one node. Never call this directly, always use mn_Del();
*/
void mn_Close(mn_type mn)
{
  mn->d = NULL;
  mn->n = NULL;
  free(mn);
}

/*
  Open a copy of the source, but do not copy the subtree.
  The procedure mn_Copy() copyies a node and all subnodes.
*/
mn_type mn_OpenCopy(mn_type source)
{
  mn_type mn = mn_Open();
  if ( mn != NULL )
  {
    /* copy content fo source to mn */
    return mn;
  }
  return NULL;
}

