/*

  m2navprev.c
  
*/

#include "m2.h"

/*
  Description:
    Navigate to next element.
  Return value:
    0, if max depth is 0 or 1 or if the current child has been the first child
  Implementation Status
    Could be moved to different file
*/

uint8_t m2_nav_prev(m2_nav_p nav)
{
  uint8_t pos;
  uint8_t d;
  d = nav->depth;
  if ( d <= 1 )
    return 0;
  d-=2;
  pos = nav->pos[d];
  pos++;
  if ( pos == 0 )
  {
    return 0;
  }
  m2_nav_load_child(nav, pos);
  return 1;
}

