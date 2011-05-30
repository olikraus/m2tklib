/*

  m2gfx.c
  
  interface to the graphics handler

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

/* local variables */

m2_gfx_arg_t m2_gfx_arg_object;
m2_gfx_fnptr m2_curr_gfx_fnptr;

/* local procedures */

static uint8_t m2_gfx_call_handler(uint8_t msg)
{
  m2_gfx_arg_object.msg = msg;
  return m2_curr_gfx_fnptr(&m2_gfx_arg_object);
}

/* graphics interface */

void m2_gfx_init(m2_gfx_fnptr fnptr) 
{
  m2_curr_gfx_fnptr = fnptr;
  m2_gfx_call_handler(M2_GFX_MSG_INIT);
}

void m2_gfx_start(m2_gfx_fnptr fnptr)
{
  m2_curr_gfx_fnptr = fnptr;
  m2_gfx_call_handler(M2_GFX_MSG_START);
}

void m2_gfx_hline(uint8_t x0, uint8_t y0, uint8_t w)
{
  m2_gfx_arg_object.x = x0;
  m2_gfx_arg_object.y = y0;
  m2_gfx_arg_object.w = w;
  m2_gfx_call_handler(M2_GFX_MSG_DRAW_HLINE);
}

void m2_gfx_vline(uint8_t x0, uint8_t y0, uint8_t h)
{
  m2_gfx_arg_object.x = x0;
  m2_gfx_arg_object.y = y0;
  m2_gfx_arg_object.h = h;
  m2_gfx_call_handler(M2_GFX_MSG_DRAW_VLINE);
}

void m2_gfx_box(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h)
{
  m2_gfx_arg_object.x = x0;
  m2_gfx_arg_object.y = y0;
  m2_gfx_arg_object.w = w;
  m2_gfx_arg_object.h = h;
  m2_gfx_call_handler(M2_GFX_MSG_DRAW_BOX);
}

void m2_gfx_text(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, uint8_t font, const char *s)
{
  m2_gfx_arg_object.x = x0;
  m2_gfx_arg_object.y = y0;
  m2_gfx_arg_object.w = w;
  m2_gfx_arg_object.h = h;
  m2_gfx_arg_object.font = font;
  m2_gfx_arg_object.s = s;
  m2_gfx_call_handler(M2_GFX_MSG_DRAW_TEXT);
}

/* assumes that s is a pointer into PROGMEM */
void m2_gfx_text_p(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, uint8_t font, const char *s)
{
  m2_gfx_arg_object.x = x0;
  m2_gfx_arg_object.y = y0;
  m2_gfx_arg_object.w = w;
  m2_gfx_arg_object.h = h;
  m2_gfx_arg_object.font = font;
  m2_gfx_arg_object.s = s;
  m2_gfx_call_handler(M2_GFX_MSG_DRAW_TEXT_P);
}

void m2_gfx_normal_no_focus(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, uint8_t font)
{
  m2_gfx_arg_object.x = x0;
  m2_gfx_arg_object.y = y0;
  m2_gfx_arg_object.w = w;
  m2_gfx_arg_object.h = h;
  m2_gfx_arg_object.font = font;
  m2_gfx_call_handler(M2_GFX_MSG_DRAW_NORMAL_NO_FOCUS);
}

void m2_gfx_normal_focus(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, uint8_t font)
{
  m2_gfx_arg_object.x = x0;
  m2_gfx_arg_object.y = y0;
  m2_gfx_arg_object.w = w;
  m2_gfx_arg_object.h = h;
  m2_gfx_arg_object.font = font;
  m2_gfx_call_handler(M2_GFX_MSG_DRAW_NORMAL_FOCUS);
}

void m2_gfx_normal_parent_focus(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, uint8_t font)
{
  m2_gfx_arg_object.x = x0;
  m2_gfx_arg_object.y = y0;
  m2_gfx_arg_object.w = w;
  m2_gfx_arg_object.h = h;
  m2_gfx_arg_object.font = font;
  m2_gfx_call_handler(M2_GFX_MSG_DRAW_NORMAL_PARENT_FOCUS);
}

void m2_gfx_small_focus(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, uint8_t font)
{
  m2_gfx_arg_object.x = x0;
  m2_gfx_arg_object.y = y0;
  m2_gfx_arg_object.w = w;
  m2_gfx_arg_object.h = h;
  m2_gfx_arg_object.font = font;
  m2_gfx_call_handler(M2_GFX_MSG_DRAW_SMALL_FOCUS);
}



void m2_gfx_data_entry(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, uint8_t font)
{
  m2_gfx_arg_object.x = x0;
  m2_gfx_arg_object.y = y0;
  m2_gfx_arg_object.w = w;
  m2_gfx_arg_object.h = h;
  m2_gfx_arg_object.font = font;
  m2_gfx_call_handler(M2_GFX_MSG_DRAW_DATA_ENTRY);
}

void m2_gfx_go_up(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, uint8_t font)
{
  m2_gfx_arg_object.x = x0;
  m2_gfx_arg_object.y = y0;
  m2_gfx_arg_object.w = w;
  m2_gfx_arg_object.h = h;
  m2_gfx_arg_object.font = font;
  m2_gfx_call_handler(M2_GFX_MSG_DRAW_GO_UP);
}

uint8_t m2_gfx_get_text_width(uint8_t font, const char *s)
{
  m2_gfx_arg_object.font = font;
  m2_gfx_arg_object.s = s;
  return m2_gfx_call_handler(M2_GFX_MSG_GET_TEXT_WIDTH);
}

/* assumes that s is a pointer into PROGMEM */
uint8_t m2_gfx_get_text_width_p(uint8_t font, const char *s)
{
  m2_gfx_arg_object.font = font;
  m2_gfx_arg_object.s = s;
  return m2_gfx_call_handler(M2_GFX_MSG_GET_TEXT_WIDTH_P);
}

uint8_t m2_gfx_get_char_width(uint8_t font)
{
  m2_gfx_arg_object.font = font;
  return m2_gfx_call_handler(M2_GFX_MSG_GET_CHAR_WIDTH);
}

uint8_t m2_gfx_get_char_height(uint8_t font)
{
  m2_gfx_arg_object.font = font;
  return m2_gfx_call_handler(M2_GFX_MSG_GET_CHAR_HEIGHT);
}


uint8_t m2_gfx_add_normal_border_height(uint8_t font, uint8_t height) 
{
  m2_gfx_arg_object.font = font;
  height +=  m2_gfx_call_handler(M2_GFX_MSG_GET_NORMAL_BORDER_HEIGHT);
  return height;
}

uint8_t m2_gfx_add_normal_border_width(uint8_t font, uint8_t width) 
{
  m2_gfx_arg_object.font = font;
  width +=  m2_gfx_call_handler(M2_GFX_MSG_GET_NORMAL_BORDER_WIDTH);
  return width;
}

uint8_t m2_gfx_add_normal_border_x(uint8_t font, uint8_t x)
{
  m2_gfx_arg_object.font = font;
  x +=  m2_gfx_call_handler(M2_GFX_MSG_GET_NORMAL_BORDER_X_OFFSET);
  return x;
}

uint8_t m2_gfx_add_normal_border_y(uint8_t font, uint8_t y)
{
  m2_gfx_arg_object.font = font;
  y +=  m2_gfx_call_handler(M2_GFX_MSG_GET_NORMAL_BORDER_Y_OFFSET);
  return y;
}


uint8_t m2_gfx_add_small_border_height(uint8_t font, uint8_t height)
{
  m2_gfx_arg_object.font = font;
  height +=  m2_gfx_call_handler(M2_GFX_MSG_GET_SMALL_BORDER_HEIGHT);
  return height;
}

uint8_t m2_gfx_add_small_border_width(uint8_t font, uint8_t width)
{
  m2_gfx_arg_object.font = font;
  width +=  m2_gfx_call_handler(M2_GFX_MSG_GET_SMALL_BORDER_WIDTH);
  return width;
}

uint8_t m2_gfx_add_small_border_x(uint8_t font, uint8_t x)
{
  m2_gfx_arg_object.font = font;
  x +=  m2_gfx_call_handler(M2_GFX_MSG_GET_SMALL_BORDER_X_OFFSET);
  return x;
}

uint8_t m2_gfx_add_small_border_y(uint8_t font, uint8_t y) 
{
  m2_gfx_arg_object.font = font;
  y +=  m2_gfx_call_handler(M2_GFX_MSG_GET_SMALL_BORDER_Y_OFFSET);
  return y;
}


uint8_t m2_gfx_add_readonly_border_height(uint8_t font, uint8_t height)
{
  m2_gfx_arg_object.font = font;
  height +=  m2_gfx_call_handler(M2_GFX_MSG_GET_READONLY_BORDER_HEIGHT);
  return height;
}

uint8_t m2_gfx_add_readonly_border_width(uint8_t font, uint8_t width)
{
  m2_gfx_arg_object.font = font;
  width +=  m2_gfx_call_handler(M2_GFX_MSG_GET_READONLY_BORDER_WIDTH);
  return width;
}

uint8_t m2_gfx_add_readonly_border_x(uint8_t font, uint8_t x)
{
  x +=  m2_gfx_call_handler(M2_GFX_MSG_GET_READONLY_BORDER_X_OFFSET);
  m2_gfx_arg_object.font = font;
  return x;
}

uint8_t m2_gfx_add_readonly_border_y(uint8_t font, uint8_t y)
{
  m2_gfx_arg_object.font = font;
  y +=  m2_gfx_call_handler(M2_GFX_MSG_GET_READONLY_BORDER_Y_OFFSET);
  return y;
}


uint8_t m2_gfx_get_list_overlap_height(void)
{	
  return m2_gfx_call_handler(M2_GFX_MSG_GET_LIST_OVERLAP_HEIGHT);
}

uint8_t m2_gfx_get_list_overlap_width(void)
{
  return m2_gfx_call_handler(M2_GFX_MSG_GET_LIST_OVERLAP_WIDTH);
}


void m2_gfx_draw_icon(uint8_t x0, uint8_t y0, uint8_t font, uint8_t icon_number)
{
  m2_gfx_arg_object.x = x0;
  m2_gfx_arg_object.y = y0;
  m2_gfx_arg_object.font = font;
  m2_gfx_arg_object.icon = icon_number;
  m2_gfx_call_handler(M2_GFX_MSG_DRAW_ICON);
}

uint8_t m2_gfx_get_icon_height(uint8_t font, uint8_t icon_number)
{
  m2_gfx_arg_object.font = font;
  m2_gfx_arg_object.icon = icon_number;
  return m2_gfx_call_handler(M2_GFX_MSG_GET_ICON_HEIGHT);
}

uint8_t m2_gfx_get_icon_width(uint8_t font, uint8_t icon_number)
{
  m2_gfx_arg_object.font = font;
  m2_gfx_arg_object.icon = icon_number;
  return m2_gfx_call_handler(M2_GFX_MSG_GET_ICON_WIDTH);
}

uint8_t m2_gfx_is_frame_draw_at_end(void)
{
  return m2_gfx_call_handler(M2_GFX_MSG_IS_FRAME_DRAW_AT_END);
}

void m2_gfx_end(void)
{
  m2_gfx_call_handler(M2_GFX_MSG_END);
}

void m2_gfx_set_font(m2_gfx_fnptr fnptr, uint8_t font_idx, const void *font_ptr)
{
  m2_curr_gfx_fnptr = fnptr;
  m2_gfx_arg_object.font = font_idx;
  m2_gfx_arg_object.s = (const char *)font_ptr;
  m2_gfx_call_handler(M2_GFX_MSG_SET_FONT);
}

