/*

  m2navfirst.c

  Code size status: 
    Proccedure attributes: Optimization completed
    Arithmetic optimization: Done

*/

#include "m2.h"

/*
  Description:
    Navigate to first element.
  Return value:
    0, if max depth is 0 or 1.
  Implementation Status
    Could be moved to different file
*/
uint8_t m2_nav_first(m2_nav_p nav)
{
  if ( nav->depth <= 1 )
    return 0;
  m2_nav_load_child(nav, 0);
  return 1;  
}

