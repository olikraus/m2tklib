/*

  mn_init.c
  
*/


#include "mn.h"
#include <stdlib.h>

mn_type mn_OpenWithFn(mn_fn fn)
{
  mn_type mn = (mn_type)malloc(sizeof(struct _mn_struct));
  if ( mn != NULL )
  {
    mn->d = NULL;
    mn->n = NULL;
    mn->seq_nr = 0;
    mn->fn = fn;
    //mn->data = NULL;
    mn->arg_cnt = 0;
    mn->arg_list = NULL;
    mn->mr_list_pos = -1;
    mn->mr_list_len = 0;
    mn->mr_element_pos = -1;
    
    if ( mn->fn(mn, MN_MSG_OPEN, NULL) != 0 )
    {      
      return mn;
    }
    free(mn);
  }
  return NULL;
}

mn_type mn_Open(void)
{
  return mn_OpenWithFn(mn_fn_empty);
}

/*
  Destroys one node. Never call this directly, always use mn_Del();
*/
void mn_Close(mn_type mn)
{
  mn->fn(mn, MN_MSG_CLOSE, NULL);
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

