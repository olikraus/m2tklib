/*

  m2elsb.c

  scroll bar

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
  
  
  elsb specific options:
    l:			Number of lines (defaults to 1), inherit from elslbase
  
*/

#include "m2.h"

M2_EL_FN_DEF(m2_el_vsb_fn)
{
  switch(fn_arg->msg)
  {
    case M2_EL_MSG_GET_HEIGHT:
      return m2_el_slbase_calc_height(fn_arg->element);
    case M2_EL_MSG_GET_WIDTH:
      return m2_el_slbase_calc_width(fn_arg->element);
#ifdef M2_EL_MSG_DBG_SHOW
    case M2_EL_MSG_DBG_SHOW:
      {
	uint8_t width, height;
	m2_pos_p b = (m2_pos_p)(fn_arg->data);
	width = m2_el_slbase_calc_width((fn_arg->element));
	height = m2_el_slbase_calc_height((fn_arg->element));
	printf("sb w:%d h:%d arg:%d x:%d y:%d len:%d\n", width, height, 
	    (fn_arg->arg), b->x, b->y, m2_el_slbase_get_len(fn_arg->element));
      }
      return 0;
#endif
      case M2_EL_MSG_SHOW:
	/* MSG_SHOW: parent is drawn before the sub elements */
	/* adjust top element to total size, if required */
	m2_el_slbase_adjust_top_to_cnt(fn_arg->element);
	{
	  m2_pos_p b = (m2_pos_p)(fn_arg->data);
	  m2_gfx_draw_vertical_scroll_bar(b->x, b->y, 
	    m2_el_slbase_calc_width(fn_arg->element), 
	    m2_el_slbase_calc_height(fn_arg->element), 
	    m2_el_slbase_get_len(fn_arg->element),
	    m2_el_slbase_get_top(fn_arg->element), 
	    m2_el_slbase_get_visible_lines(fn_arg->element));	  
	}
	break;
  }
  return m2_el_fnfmt_fn(fn_arg);
}
