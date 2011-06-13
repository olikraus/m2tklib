/*

  m2usrnext.c
  
  navigation procedure for the event handler: next

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

  Code size status: 
    Proccedure attributes: Optimization completed
    Arithmetic optimization: Done


*/

#include "m2.h"

static uint8_t m2_nav_user_next_sub(m2_nav_p nav)
{
  if ( m2_nav_next(nav) != 0 )
    return m2_nav_do_auto_down(nav);
  if ( m2_nav_do_auto_up(nav) == 0 )
    return 0;
  /* if auto up does not change the depth, then the next command would be repeated */
  if ( m2_nav_next(nav) != 0 )
    return m2_nav_do_auto_down(nav);
  if ( m2_nav_first(nav) != 0 )
    return m2_nav_do_auto_down(nav);
  return 0;  
}

uint8_t m2_nav_user_next(m2_nav_p nav)
{
  uint8_t cnt = 255;
  for(;;)
  {
    if ( m2_nav_user_next_sub(nav) == 0 )
      return 0;
    if ( m2_nav_is_read_only(nav) == 0 )
      break;
    /* this is an emergency exit. it means that there are only nonselectable fields here */
    /* usually this should never happen */
    if ( cnt == 0 )
      return 0;
    cnt--;
  }
  return 1;
}

