/*

  m2navroot.c
  
  Handle root element changes

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
/* new root assignment */
/* idea: assign a new root to an intermediate variable, init the nav with the new root at top level */
/* any element function could call this procedure */

void m2_nav_set_root(m2_nav_p nav,  m2_rom_void_p element)
{
  if ( element == NULL )
    element = &m2_null_element;
  nav->new_root_element = element;
}

/* should be called only at the top level */
/* currently this is called in the event step procedure m2_ep_step() in m2ep.c */
uint8_t m2_nav_check_and_assign_new_root(m2_nav_p nav)
{
  if ( nav->new_root_element != NULL )
  {
    m2_nav_init(nav, nav->new_root_element);
    nav->new_root_element = NULL;
    return 1;
  }
  return 0;
}


