/*
  Event Handler
  
  2 Buttons: Next + Select
  Requires/Uses Data Entry Mode
  
*/

#include "m2.h"

uint8_t m2_eh_2bd(m2_p ep, uint8_t msg, uint8_t arg)
{
  m2_nav_p nav = m2_get_nav(ep);
  switch(msg)
  {
    case M2_EP_MSG_SELECT:
      /* if this is data entry mode, switch back */
      if ( nav->is_data_entry_active != 0 )
      {
	nav->is_data_entry_active = 0;
	return 1;
      }
      
      /* data entry mode not active; if we could enter data entry mode, then activate it */
      if ( m2_nav_is_data_entry(nav) )
      {
	nav->is_data_entry_active = 1;
	return 1;
      }
      
      /* go down: do not use m2_nav_down, instead use m2_nav_user_down to skip design elements */
      return m2_nav_user_down(nav, 1);
      
    case M2_EP_MSG_NEXT:
      if ( nav->is_data_entry_active != 0 )
	return m2_nav_data_up(m2_get_nav(ep));
      return m2_nav_user_next(m2_get_nav(ep));
  }
  return 0;
}
