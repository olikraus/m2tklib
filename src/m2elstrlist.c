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
    w:		Width of the strlist box
    l:			Number of lines 
  
  Note:
    option 'h' is not supported, instead height is calculated by fontheight * number of lines

  TODO
    - change args to element --> ERLEDIGT
    - combo procedures, change to element

*/


#include "m2.h"

/*==============================================================*/
/* strlist utility procedures */

#define M2_STRLIST_ILLEGAL 255

/*
  return value from 0 .. cX-1 if the 
  based on the number of visible lines, the top visible element and the total number of lines, calculate if the 
  current (fn->arg) is visible. Also calc the position

  visible_lines  0...
  total_lines 0...
  top_line	0...total_lines-1
  curr_line (fn_arg->arg) 0...total_lines-1

  if ( curr_line < top_line )
    not visible
  if ( curr_line >= top_line + visible_lines )
    not_visible
  vis_pos = curr_line - top_line
  
*/

m2_get_str_fnptr m2_el_strlist_get_str_fnptr(m2_rom_void_p element)
{
  return (m2_get_str_fnptr)m2_rom_get_fnptr(element, offsetof(m2_el_combo_t, get_str_fnptr));
}

const char *m2_el_strlist_get_str(m2_rom_void_p element, uint8_t idx)
{
  return m2_el_strlist_get_str_fnptr(element)(idx);
}

/* argument must be of type m2_el_strlist_p */ 
uint8_t m2_el_strlist_get_visible_lines(m2_rom_void_p element)
{
  return m2_el_fmfmt_opt_get_val_any_by_element(element, 'l', 1);
}

/* argument must be of type m2_el_strlist_p */ 
uint8_t *m2_el_strlist_get_top_ptr(m2_rom_void_p element)
{
  return (uint8_t *)m2_rom_get_ram_ptr(element, offsetof(m2_el_strlist_t, top_element));
}

/* argument must be of type m2_el_strlist_p */ 
uint8_t m2_el_strlist_get_top_val(m2_rom_void_p element)
{
  return *m2_el_strlist_get_top_ptr(element);
}

/*
  calculate the visible position, based on the internal index
  position order of the result is inverted, 0 is botton element
  return M2_STRLIST_ILLEGAL if the value is not visible 
*/
/* argument must be of type m2_el_strlist_p */ 
uint8_t m2_el_strlist_get_visible_pos(m2_rom_void_p el_strlist, uint8_t idx)
{
  uint8_t top = m2_el_strlist_get_top_val(el_strlist);
  uint8_t lines;
  if ( idx < top )
    return M2_STRLIST_ILLEGAL;
  idx -= top;
  lines = m2_el_strlist_get_visible_lines(el_strlist);
  if ( idx >= lines )
    return M2_STRLIST_ILLEGAL;
  lines--;
  lines -= idx;
  return lines;
}

/* argument must be of type m2_el_strlist_p */ 
uint8_t m2_el_strlist_calc_height(m2_rom_void_p el_strlist)
{
  uint8_t h;
  uint8_t visible_lines = m2_el_strlist_get_visible_lines(el_strlist);
  h = m2_gfx_get_char_height_with_normal_border(m2_el_fmfmt_get_font_by_element(el_strlist));
  h *= visible_lines;
  h = m2_calc_vlist_height_overlap_correction(h, visible_lines);
  return h;
}

/* argument must be of type m2_el_strlist_p */ 
uint8_t m2_el_strlist_calc_width(m2_rom_void_p el_strlist)
{
  return m2_gfx_add_normal_border_width(m2_el_fmfmt_get_font_by_element(el_strlist), m2_el_fnfmt_get_wW_by_element(el_strlist));
}

void m2_el_strlist_calc_box(m2_rom_void_p el_strlist, uint8_t idx, m2_pcbox_p data)
{
  uint8_t visible_pos = m2_el_strlist_get_visible_pos(el_strlist, idx);
  uint8_t y;
  data->c.y = data->p.y;
  data->c.x = data->p.x;
  
  if ( visible_pos == M2_STRLIST_ILLEGAL )
    return;
  
  y = m2_gfx_get_char_height_with_normal_border(m2_el_fmfmt_get_font_by_element(el_strlist));
  y *= visible_pos;
  y = m2_calc_vlist_height_overlap_correction(y, visible_pos);
  data->c.y += y;
}

void m2_el_strlist_adjust_top(m2_rom_void_p el_strlist, uint8_t pos)
{
  uint8_t top = m2_el_strlist_get_top_val(el_strlist);
  uint8_t lines;
  if ( pos < top )
  {
    top = pos;
  }
  else 
  {
    lines = m2_el_strlist_get_visible_lines(el_strlist);
    if ( pos >= top+lines )
    {
      top = pos;
      top -= lines;
      top++;
    }
  }
  *m2_el_strlist_get_top_ptr(el_strlist) = top;
}


/*==============================================================*/
/* strline */


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
      return m2_el_strlist_calc_width(m2_nav_get_parent_element(fn_arg->nav));
    case M2_EL_MSG_NEW_FOCUS:
      /* printf("STRLINE M2_EL_MSG_NEW_FOCUS pos = %d\n", pos); */
      m2_el_strlist_adjust_top(m2_nav_get_parent_element(fn_arg->nav), pos);
      return 1;
    case M2_EL_MSG_SHOW:
    {
      m2_pos_p b = (m2_pos_p)(fn_arg->data);
      const char *s = m2_el_strlist_get_str(m2_nav_get_parent_element(fn_arg->nav), pos);
      uint8_t font = m2_el_parent_get_font(fn_arg->nav);
      uint8_t visible_pos = m2_el_strlist_get_visible_pos(m2_nav_get_parent_element(fn_arg->nav), pos);
      
      /* printf("STRLINE M2_EL_MSG_SHOW arg = %d, visible_pos = %d\n", fn_arg->arg, visible_pos); */
      
      if ( visible_pos != M2_STRLIST_ILLEGAL )
      {
	uint8_t w, h;
	w = m2_el_strlist_calc_width(m2_nav_get_parent_element(fn_arg->nav));
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
      return 1;
    }    
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
      /* printf("STRLIST: M2_EL_MSG_GET_LIST_LEN %d\n", m2_el_combo_get_len(fn_arg)); */
      return m2_el_combo_get_len(fn_arg);
    case M2_EL_MSG_GET_LIST_ELEMENT:
      *((m2_rom_void_p *)(fn_arg->data)) = &m2_el_virtual_strline;
      return 1;
    case M2_EL_MSG_IS_AUTO_SKIP:
      return 0;
    case M2_EL_MSG_GET_LIST_BOX:
      m2_el_strlist_calc_box(fn_arg->element, fn_arg->arg, ((m2_pcbox_p)(fn_arg->data)));
      return 1;
    case M2_EL_MSG_GET_HEIGHT:
      return m2_el_strlist_calc_height((fn_arg->element));
    case M2_EL_MSG_GET_WIDTH:
      return m2_el_strlist_calc_width((fn_arg->element));
#ifdef M2_EL_MSG_DBG_SHOW
    case M2_EL_MSG_DBG_SHOW:
      {
	uint8_t width, height;
	m2_pos_p b = (m2_pos_p)(fn_arg->data);
	width = m2_el_strlist_calc_width((fn_arg->element));
	height = m2_el_strlist_calc_height((fn_arg->element));
	printf("strlist w:%d h:%d arg:%d x:%d y:%d len:%d\n", width, height, 
	    (fn_arg->arg), b->x, b->y, m2_el_combo_get_len(fn_arg));
      }
      return 0;
#endif
  }
  return 0;
}



