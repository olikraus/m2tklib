
#include "mn.h"
#include <stdlib.h>

mn_type mn_Open(void)
{
  mn_type mn = (mn_type)malloc(sizeof(struct _mn_struct));
  if ( mn != NULL )
  {
    mn->d = NULL;
    mn->n = NULL;
    return mn;
  }
  return NULL;
}

void mn_Close(mn_type mn)
{
  mn->d = NULL;
  mn->n = NULL;
  free(mn);
}

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

