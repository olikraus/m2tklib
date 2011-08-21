/*

  m2elstrlist.c
  
  list of strings, one can be selected

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
  
  
  strlist specific options:
    w:		Width of the strlist box (required!)
    l:			Number of lines (defaults to 1)
  
  Notes:
    option 'h' is not supported, instead height is calculated by fontheight * number of lines
    option 'a' is NOT SUPPORTED and is always enabled

  TODO
    - change args to element --> Implemented
    - combo procedures, change to element --> not required any more, strlist does not use combo element any more

*/

#include "m2.h"

#ifdef M2_EL_MSG_DBG_SHOW
#include <stdio.h>
#endif


/*==============================================================*/
/* strlist structure access */

m2_strlist_cb_fnptr m2_el_strlist_cb_fnptr(m2_rom_void_p element)
{
  return (m2_strlist_cb_fnptr)m2_rom_get_fnptr(element, offsetof(m2_el_strlist_t, strlist_cb_fnptr));
}

const char *m2_el_strlist_get_str(m2_rom_void_p element, uint8_t idx)
{
  return m2_el_strlist_cb_fnptr(element)(idx, M2_STRLIST_MSG_GET_STR);
}

/*==============================================================*/
/* strlist utility procedures */


void m2_el_strlist_calc_box(m2_rom_void_p el_strlist, uint8_t idx, m2_pcbox_p data)
{
  uint8_t visible_pos = m2_el_slbase_get_visible_pos(el_strlist, idx);
  uint8_t y;
  data->c.y = data->p.y;
  data->c.x = data->p.x;
  
  if ( visible_pos == M2_EL_SLBASE_ILLEGAL )
    return;
  
  y = m2_gfx_get_char_height_with_normal_border(m2_el_fmfmt_get_font_by_element(el_strlist));
  y *= visible_pos;
  visible_pos++;  /* for the overlap correction */
  y = m2_calc_vlist_height_overlap_correction(y, visible_pos);
  data->c.y += y;
#ifdef M2_EL_MSG_DBG_SHOW
  printf("- strlist calc box: idx:%d pos:%d cx:%d cy:%d px:%d py:%d\n", idx, visible_pos, data->c.x, data->c.y, data->p.x, data->p.y);
#endif
}


/*==============================================================*/
/* strline */

void m2_el_strlline_show(m2_el_fnarg_p fn_arg, const char *s)
{
  m2_pos_p b = (m2_pos_p)(fn_arg->data);
  uint8_t pos = m2_nav_get_child_pos(fn_arg->nav);
  uint8_t font = m2_el_parent_get_font(fn_arg->nav);
  uint8_t visible_pos = m2_el_slbase_get_visible_pos(m2_nav_get_parent_element(fn_arg->nav), pos);
  
  /* printf("STRLINE M2_EL_MSG_SHOW arg = %d, visible_pos = %d\n", fn_arg->arg, visible_pos); */
  
  if ( visible_pos != M2_EL_SLBASE_ILLEGAL )
  {
    uint8_t w, h;
    w = m2_el_slbase_calc_width(m2_nav_get_parent_element(fn_arg->nav));
    h = m2_gfx_get_char_height_with_normal_border(font);
    
    /*
      there is a sw architecture problem here: m2_el_fnfmt_fn() can not be called to create
      the focus/border, because the width of the element depends on the parent
    */
    if ( m2_is_frame_draw_at_end != 0 )
      m2_gfx_draw_text_add_normal_border_offset(b->x, b->y, 0, 0, font, s);
      
    if ( fn_arg->arg < 2 )
      m2_gfx_normal_no_focus(b->x, b->y, w, h, font);      
    else if ( fn_arg->arg == 2 )
      m2_gfx_normal_focus(b->x, b->y, w, h, font);
    
    if ( m2_is_frame_draw_at_end == 0 )
      m2_gfx_draw_text_add_normal_border_offset(b->x, b->y, 0, 0, font, s);
  }
}

M2_EL_FN_DEF(m2_el_strline_fn)
{
  uint8_t font;
  uint8_t pos;

  font = m2_el_parent_get_font(fn_arg->nav);
  pos = m2_nav_get_child_pos(fn_arg->nav);
  
  switch(fn_arg->msg)
  {
    case M2_EL_MSG_GET_LIST_LEN:
        return 0;  /* not a list, return 0 */
    case M2_EL_MSG_GET_HEIGHT:
      return m2_gfx_get_char_height_with_normal_border(font);
    case M2_EL_MSG_GET_WIDTH:
      return m2_el_slbase_calc_width(m2_nav_get_parent_element(fn_arg->nav));
    case M2_EL_MSG_NEW_FOCUS:
      /* printf("STRLINE M2_EL_MSG_NEW_FOCUS pos = %d\n", pos); */
      m2_el_slbase_adjust_top_to_focus(m2_nav_get_parent_element(fn_arg->nav), pos);
      return 1;
#ifdef M2_EL_MSG_DBG_SHOW
    case M2_EL_MSG_DBG_SHOW:
      {
	uint8_t width, height;
	m2_pos_p b = (m2_pos_p)(fn_arg->data);
	width = m2_el_slbase_calc_width((fn_arg->element));
	height = m2_el_slbase_calc_height((fn_arg->element));
	printf("strline w:%d h:%d arg:%d x:%d y:%d\n", width, height, 
	    (fn_arg->arg), b->x, b->y);
      }
      return 0;
#endif
    case M2_EL_MSG_SHOW:
      m2_el_strlline_show(fn_arg, m2_el_strlist_get_str(m2_nav_get_parent_element(fn_arg->nav), pos));
      return 1;
  }
  return 0;
}


/*==============================================================*/
/* strlist function */

m2_el_fnfmt_t m2_el_virtual_strline M2_SECTION_PROGMEM = 
{
  m2_el_strline_fn, NULL
};

M2_EL_FN_DEF(m2_el_strlist_fn)
{
  switch(fn_arg->msg)
  {
    case M2_EL_MSG_GET_LIST_LEN:
      /* printf("STRLIST: M2_EL_MSG_GET_LIST_LEN %d\n", m2_el_strlist_get_cnt(fn_arg->element)); */
      return m2_el_slbase_get_len(fn_arg->element);
    case M2_EL_MSG_GET_LIST_ELEMENT:
      *((m2_rom_void_p *)(fn_arg->data)) = &m2_el_virtual_strline;
      return 1;
    case M2_EL_MSG_IS_AUTO_SKIP:
      return 1;
    case M2_EL_MSG_GET_OPT:
	if ( fn_arg->arg == 'd' )
	{
	  *(uint8_t *)(fn_arg->data) = 1;
	  return 1;
	}
	/* else... break out of the switch and let the base class do the rest of the work */
	break;
    case M2_EL_MSG_GET_LIST_BOX:
      m2_el_strlist_calc_box(fn_arg->element, fn_arg->arg, ((m2_pcbox_p)(fn_arg->data)));
      return 1;
    case M2_EL_MSG_GET_HEIGHT:
      return m2_el_slbase_calc_height((fn_arg->element));
    case M2_EL_MSG_GET_WIDTH:
      return m2_el_slbase_calc_width((fn_arg->element));
#ifdef M2_EL_MSG_DBG_SHOW
    case M2_EL_MSG_DBG_SHOW:
      {
	uint8_t width, height;
	m2_pos_p b = (m2_pos_p)(fn_arg->data);
	width = m2_el_slbase_calc_width((fn_arg->element));
	height = m2_el_slbase_calc_height((fn_arg->element));
	printf("strlist w:%d h:%d arg:%d x:%d y:%d len:%d\n", width, height, 
	    (fn_arg->arg), b->x, b->y, m2_el_slbase_get_len(fn_arg->element));
      }
      return 0;
#endif
      case M2_EL_MSG_SHOW:
	/* MSG_SHOW: parent is drawn before the sub elements */
	/* adjust top element to total size, if required */
	m2_el_slbase_adjust_top_to_cnt(fn_arg->element);
	break;
  }
  return m2_el_fnfmt_fn(fn_arg);
}



