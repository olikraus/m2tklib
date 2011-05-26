/*
  m2.c
  
  m2tklib = Mini Interative Interface Toolkit Library
  
  Copyright (C) 2011  olikraus@gmail.com

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.


  
  Simplified Top Level API of the m2tklib.
  Most important: Init and event processing  
  - read keys 
  - pass key values to the event handler
  
  
  
*/

#include "m2.h"

/*
  element: 	the root element
  es:			event source handler, can be NULL
  eh:			event handler to process the source events
  gh:			graphics handler
*/
void m2_InitM2(m2_p m2, m2_rom_void_p element, m2_es_fnptr es, m2_eh_fnptr eh, m2_gfx_fnptr gh)
{
  m2->is_frame_draw_at_end = 0;
  m2->es = es;
  m2->eh = eh;
  m2->gh = gh;
  m2_gfx_init(gh);
  m2->is_frame_draw_at_end = m2_gfx_is_frame_draw_at_end();
  m2->forced_key = M2_KEY_REFRESH;
  m2_nav_init(m2_get_nav(m2),  element);
}

/*
  return:
    0: nothing happend
    1:	event has been processed
*/
uint8_t m2_StepM2(m2_p m2)
{
  uint8_t key;
  
  /* check if a key should be forced */
  key = m2->forced_key;
  if ( key != M2_KEY_NONE )
  {
    m2->forced_key = M2_KEY_NONE;
  }
  else
  {
    /* request key information from the event source */
    if ( m2->es != NULL )
      key = m2->es(m2, M2_ES_MSG_GET_KEY);
    else
      key = M2_KEY_NONE;
  }
    
  /* if there is a valid key, process the key event */
  /* note, that key numbers are equal to message numbers */
  if ( key != M2_KEY_NONE )
  {
    if ( m2->eh != NULL )
    {
      /* handle the key */
      m2->eh(m2, key, 0);
      /* check if the root node has been changed */
      m2_nav_check_and_assign_new_root(m2_get_nav(m2));
    }
    return 1;
  }
  
  return 0;
}
