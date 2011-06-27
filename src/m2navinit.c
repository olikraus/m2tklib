/*

  m2navinit.c
  
  nav constructor 

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

*/

#include "m2.h"

/*=========================================================================*/
/*
  go down, util there is something to select
  this is used after a new root element has been set. 
  Note that other methodes are required for the user select/go down request.
  
  
*/
static void m2_nav_do_down_after_root_assignment(m2_nav_p nav)
{
  /*obsolete: m2_nav_do_auto_down(nav); */
  
  m2_nav_user_first(nav);

  /* obsolete
  if ( m2_nav_get_list_len(nav) >= 1 )
  {
    m2_nav_user_down(nav, 0);
  }

  m2_nav_do_auto_down(nav);
  
  if ( m2_nav_is_read_only(nav) != 0 )
    m2_nav_user_next(nav);
  */
}

/* constructor: init a new cursor into our tree */
/* memory for m2_nav_p nav must be provided by the calling procedure */
/* usually called by m2_ep_init(), see m2ep.c */

void m2_nav_init(m2_nav_p nav,  m2_rom_void_p element)
{
  if ( element == NULL )
    element = &m2_null_element;
  
  nav->is_data_entry_active = 0;
  nav->element_list[0] = (m2_el_fnfmt_p)element;
  nav->pos[0] = 0;
  nav->depth = 1;
  nav->new_root_element = NULL;
  m2_nav_do_down_after_root_assignment(nav);
}

