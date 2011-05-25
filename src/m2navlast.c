/*

  m2navlast.c
  
*/

#include "m2.h"

/*
  Description:
    Navigate to last element.
  Return value:
    0, if max depth is 0 or 1.
  Implementation Status
    Could be moved to different file
*/
uint8_t m2_nav_last(m2_nav_p nav)
{
  uint8_t last;
  if ( nav->depth <= 1 )
    return 0;
  last = m2_nav_get_parent_list_len(nav);
  last--;
  m2_nav_load_child(nav, last);
  return 1;  
}
