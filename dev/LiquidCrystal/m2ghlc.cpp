/*

  m2ghlc.cpp
  
  Graphics Handler for the LiquidCrystal library from the Arduino Environment

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

#include <LiquidCrystal.h>
#include <string.h>
#include "m2.h"


LiquidCrystal *m2_lc_ptr;
uint8_t m2_lc_rows;
uint8_t m2_lc_cols;

uint8_t m2_gh_lc_icon_inactive[] = { 0x00, 0x1f, 0x11, 0x11, 0x11, 0x11, 0x1f, 0x00 };
uint8_t m2_gh_lc_icon_active[] = { 0x00, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x00 };

uint8_t m2_gh_lc_cursor_x;
uint8_t m2_gh_lc_cursor_y = 255;

void m2_SetLiquidCrystal(LiquidCrystal *lc_ptr, uint8_t cols, uint8_t rows)
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

extern "C" uint8_t m2_gh_lc(m2_gfx_arg_p  arg)
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
  }
  return m2_gh_dummy(arg);
}
