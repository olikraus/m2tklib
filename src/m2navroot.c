/*

  m2navroot.c
  
  Handle root element changes
  
*/

#include "m2.h"

/*=========================================================================*/
/* new root assignment */
/* idea: assign a new root to an intermediate variable, init the nav with the new root at top level */
/* any element function could call this procedure */

void m2_nav_set_root(m2_nav_p nav,  m2_rom_void_p element)
{
  nav->new_root_element = element;
}

/* should be called only at the top level */
/* currently this is called in the event step procedure m2_ep_step() in m2ep.c */
uint8_t m2_nav_check_and_assign_new_root(m2_nav_p nav)
{
  if ( nav->new_root_element != NULL )
  {
    m2_nav_init(nav, nav->new_root_element);
    return 1;
  }
  return 0;
}


