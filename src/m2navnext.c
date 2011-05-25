/*

  m2navnext.c

  Code size status: 
    Proccedure attributes: Optimization completed
    Arithmetic optimization: Completed

*/

#include "m2.h"

/*
  Description:
    Navigate to next element. Used by draw algorithm.
  Return value:
    0, if max depth is 0 or 1 or if the current child has been the last child
*/
#ifdef OLD
uint8_t m2_nav_next(m2_nav_p nav)
{
  uint8_t len, pos;
  if ( nav->depth <= 1 )
    return 0;
  nav->depth--;
  len = m2_nav_get_list_len(nav);
  pos = nav->pos[nav->depth-1];
  pos++;
  if ( pos >= len )
  {
    /* restore original level */
    nav->depth++;
    return 0;
  }
  nav->depth++;
  m2_nav_load_child(nav, pos);
  return 1;
}
#endif

uint8_t m2_nav_next(m2_nav_p nav)
{
  uint8_t len, pos;
  uint8_t d;
  d = nav->depth;
  if ( d <= 1 )
    return 0;
  d-=2;
  len = m2_nav_get_parent_list_len(nav);
  pos = nav->pos[d];
  pos++;
  if ( pos >= len )
  {
    return 0;
  }
  m2_nav_load_child(nav, pos);
  return 1;
}
