/*

  m2ghnlc.cpp
  
  Graphics Handler for the LiquidCrystal library from the Arduino Environment

  m2tklib = Mini Interative Interface Toolkit Library
  
  Support for New LiquidCrystal library
  https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home
  
  Copyright (C) 2013  olikraus@gmail.com

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

#include <LCD.h>
#include <string.h>
#include "m2.h"


LCD *m2_lc_ptr;
uint8_t m2_lc_rows;
uint8_t m2_lc_cols;

uint8_t m2_gh_lc_icon_inactive[] = { 0x00, 0x1f, 0x11, 0x11, 0x11, 0x11, 0x1f, 0x00 };
uint8_t m2_gh_lc_icon_active[] = { 0x00, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x00 };

uint8_t m2_gh_lc_cursor_x;
uint8_t m2_gh_lc_cursor_y = 255;

void m2_SetNewLiquidCrystal(LCD *lc_ptr, uint8_t cols, uint8_t rows)
{
  m2_lc_ptr = lc_ptr;
  m2_lc_rows = rows;
  m2_lc_cols = cols;
  m2_lc_ptr->begin(m2_lc_cols, m2_lc_rows);
  //m2_lc_ptr->print("xyz");
  m2_lc_ptr->createChar(1, m2_gh_lc_icon_active);
  m2_lc_ptr->createChar(2, m2_gh_lc_icon_inactive);
}

static void m2_gh_lc_set_cursor(uint8_t x, uint8_t y)
{
  /* translate the x-y position from the m2tklib to the LiquidCrystal lib */
  /* assume 16x4 display */
  
  if ( y >= m2_lc_rows )
    return;
  
  y = m2_lc_rows-y;
  y--;
  
  if ( y >= 2 )
  {
    x+=m2_lc_cols;
    y-=2;
  }
  m2_lc_ptr->setCursor(x, y);
}

/*
  t: total height, 16 for two line display, 32 for 4 line display
  b: border from top
  y: distance from top where the slider starts (true start is y+b)
  h: height of the slider
*/
void m2_gh_lc_set_vsb_char(uint8_t t, uint8_t b, uint8_t y, uint8_t h, uint8_t byte_index)
{
  uint8_t i;
  uint8_t d;
  uint8_t m2_gh_lc_vsb_char[8]; 
  for( i = 0; i < t; i++ )
  {
    d = 0x11;
    if ( i == 0 || i+1 == t )
      d = 0x01f;
    else if ( i >= y+b && i < y+b+h )
      d = 0x01f;
    if ( (i >> 3) == byte_index )
    {
      m2_gh_lc_vsb_char[i&7] = d;
    }
  }
  
  m2_lc_ptr->createChar(3+byte_index, m2_gh_lc_vsb_char);
  
}

extern "C" uint8_t m2_gh_nlc(m2_gfx_arg_p  arg)
{
  switch(arg->msg)
  {
    case M2_GFX_MSG_INIT:
      /* should already be done in m2_SetLiquidCrystal */
      break;
    case M2_GFX_MSG_START:
      m2_gh_lc_cursor_y = 255;
      m2_lc_ptr->noCursor();
      m2_lc_ptr->noBlink();
      m2_lc_ptr->clear();
      break;
    case M2_GFX_MSG_END:
      if ( m2_gh_lc_cursor_y != 255 )
      {
	m2_gh_lc_set_cursor(m2_gh_lc_cursor_x, m2_gh_lc_cursor_y);
	m2_lc_ptr->cursor();
      }
      break;
    case M2_GFX_MSG_DRAW_TEXT:
      m2_gh_lc_set_cursor(arg->x, arg->y);
      m2_lc_ptr->print(arg->s);
      return 0;
    case M2_GFX_MSG_DRAW_NORMAL_FOCUS:
    case M2_GFX_MSG_DRAW_NORMAL_PARENT_FOCUS:
      m2_gh_lc_set_cursor(arg->x, arg->y);
      m2_lc_ptr->print('[');
      m2_gh_lc_set_cursor(arg->x+arg->w-1, arg->y);
      m2_lc_ptr->print(']');
      return 0;
    case M2_GFX_MSG_DRAW_NORMAL_DATA_ENTRY:		/* added 14 okt 2012 */
      m2_gh_lc_set_cursor(arg->x, arg->y);
      m2_lc_ptr->print('<');
      m2_gh_lc_set_cursor(arg->x+arg->w-1, arg->y);
      m2_lc_ptr->print('>');
      return 0;
    case M2_GFX_MSG_DRAW_GO_UP:
      m2_gh_lc_set_cursor(arg->x-1, arg->y);
      m2_lc_ptr->print('<');
      m2_gh_lc_set_cursor(arg->x+arg->w, arg->y);
      m2_lc_ptr->print('>');
      return 0;
    case M2_GFX_MSG_DRAW_SMALL_FOCUS:
      m2_gh_lc_cursor_x = arg->x;
      m2_gh_lc_cursor_y = arg->y;
      return 0;
    case M2_GFX_MSG_DRAW_ICON:
      m2_gh_lc_set_cursor(arg->x, arg->y);
      if ( arg->icon == M2_ICON_TOGGLE_ACTIVE || arg->icon == M2_ICON_RADIO_ACTIVE )
	m2_lc_ptr->print("\001");
      else
	m2_lc_ptr->print("\002");
      return 0;
    case M2_GFX_MSG_GET_TEXT_WIDTH:
      return strlen(arg->s);
    case M2_GFX_MSG_GET_ICON_WIDTH:
    case M2_GFX_MSG_GET_ICON_HEIGHT:
    case M2_GFX_MSG_GET_NUM_CHAR_WIDTH:
    case M2_GFX_MSG_GET_CHAR_WIDTH:
    case M2_GFX_MSG_GET_CHAR_HEIGHT:
      return 1;
    case M2_GFX_MSG_GET_NORMAL_BORDER_WIDTH:
      return 2;
    case M2_GFX_MSG_GET_NORMAL_BORDER_X_OFFSET:
      return 1;
    case M2_GFX_MSG_GET_DISPLAY_WIDTH:
      return m2_lc_cols;
    case M2_GFX_MSG_GET_DISPLAY_HEIGHT:
      return m2_lc_rows;
    case M2_GFX_MSG_DRAW_VERTICAL_SCROLL_BAR:
      /* scroll bar: "total" total number of items */
      /* scroll bar: "top" topmost item (first visible item) 0 .. total-visible*/
      /* scroll bar: "visible" number of visible items 0 .. total-1 */

      {
	uint16_t h, y, byte_index;
	char c;
	h = m2_utl_sb_get_slider_height(arg->h*8-2, arg->total, arg->visible);
	y = m2_utl_sb_get_slider_position(arg->h*8-2, h, arg->total, arg->visible, arg->top); 
	for( byte_index = 0; byte_index < arg->h; byte_index++ )
	{
	  m2_gh_lc_set_vsb_char(arg->h*8, 1, y, h, byte_index);
	  m2_gh_lc_set_cursor(arg->x, arg->y+arg->h-1-byte_index);
	  c = 3;
	  c += byte_index;
	  m2_lc_ptr->print(c);
	}
      }
      return 1;
  }
  return m2_gh_dummy(arg);
}
