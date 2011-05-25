/*

  m2ghdummy.c

  graphics dummy (null) handler

*/

#include "m2.h"
#include <string.h>

uint8_t m2_gh_dummy(m2_gfx_arg_p arg)
{
  switch(arg->msg)
  {
    case M2_GFX_MSG_GET_TEXT_WIDTH:
      return strlen(arg->s);
    case M2_GFX_MSG_GET_CHAR_WIDTH:
    case M2_GFX_MSG_GET_CHAR_HEIGHT:
    case M2_GFX_MSG_GET_ICON_WIDTH:
    case M2_GFX_MSG_GET_ICON_HEIGHT:
      return 1;
  }
  return 0;
}


