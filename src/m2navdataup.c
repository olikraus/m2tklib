/*

  m2navdataup.c
  
*/

#include "m2.h"

uint8_t m2_nav_data_up(m2_nav_p nav)
{
  uint8_t pos = 0;
  if ( nav->depth >= 2 )
    pos = nav->pos[nav->depth-2];
  
  m2_fn_arg_clear();
  m2_fn_arg_set_nav(nav);
  m2_fn_arg_set_element(m2_nav_get_current_element(nav));
  m2_fn_arg_set_arg_data(pos, NULL);
  return m2_fn_arg_call(M2_EL_MSG_DATA_UP);
}
