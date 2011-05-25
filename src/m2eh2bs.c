/*
  Event Handler
  
  2 Buttons: Next + Select
  Simplified data entry without data entry mode
  
*/

#include "m2.h"

uint8_t m2_eh_2bs(m2_p ep, uint8_t msg, uint8_t arg)
{
  m2_nav_p nav = m2_get_nav(ep);
  switch(msg)
  {
    case M2_EP_MSG_SELECT:
      
      /* if the field has data entry, than increment the data */
      if ( m2_nav_is_data_entry(nav) )
	return m2_nav_data_up(m2_get_nav(ep));
      
      /* else go down: do not use m2_nav_down, instead use m2_nav_user_down to skip design elements */
      return m2_nav_user_down(nav, 1);
      
    case M2_EP_MSG_NEXT:
      return m2_nav_user_next(m2_get_nav(ep));
  }
  return 0;
}
