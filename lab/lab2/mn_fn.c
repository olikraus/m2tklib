/*

  mn_fn.c
  
*/


#include "mn.h"
#include <stdlib.h>

/*================================================*/
/* default callback */

int mn_fn_empty(mn_type mn, int msg, void *arg)
{
  switch(msg)
  {
    case MN_MSG_NONE:
      break;
    case MN_MSG_OPEN:
      break;
    case MN_MSG_CLOSE:
      break;
    case MN_MSG_COPY:
      break;
    case MN_MSG_GET_DISPLAY_STRING:
      *(char **)arg = "empty";
      return 1;
    case MN_MSG_GET_LABEL_STRING:
      *(char **)arg = "";
      return 1;
  }
  return 1;
}

/*================================================*/
/* API procedures */

const char *mn_GetLabelString(mn_type mn)
{
  const char *s = ""; 
  mn->fn(mn, MN_MSG_GET_LABEL_STRING, (void *)&s);
  return s;
}

