/*

  m2ghlc16x4.cpp
  
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


LiquidCrystal m2_lcd(12, 11, 5, 4, 3, 2);

static void m2_set_cursor_16x4(uint8_t x, uint8_t y)
{
  /* translate the x-y position from the m2tklib to the LiquidCrystal lib */
  /* assume 16x4 display */
  
  y = 3-y;
  
  if ( y >= 2 )
  {
    x+=16;
    y-=2;
  }
  m2_lcd.setCursor(x, y);
}

/*
  00000000  0x00
  00011111  0x1f
  00010001  0x11
  00010001  0x11
  00010001  0x11
  00010001  0x11
  00011111  0x1f
  00000000  0x00

  00000000  0x00
  00011111  0x1f
  00011111  0x1f
  00011111  0x1f
  00011111  0x1f
  00011111  0x1f 
  00011111  0x1f
  00000000  0x00
*/

uint8_t m2_gh_icon_inactive[] = { 0x00, 0x1f, 0x11, 0x11, 0x11, 0x11, 0x1f, 0x00 };
uint8_t m2_gh_icon_active[] = { 0x00, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x00 };

uint8_t m2_gh_lcd_cursor_x;
uint8_t m2_gh_lcd_cursor_y = 255;

extern "C" uint8_t m2_gh_lc_16x4(m2_gfx_arg_p  arg)
{
  switch(arg->msg)
  {
    case M2_GFX_MSG_INIT:		
      m2_lcd.begin(16, 4);
      m2_lcd.createChar(1, m2_gh_icon_active);
      m2_lcd.createChar(2, m2_gh_icon_inactive);
      break;
    case M2_GFX_MSG_START:
      m2_gh_lcd_cursor_y = 255;
      m2_lcd.noCursor();
      m2_lcd.noBlink();
      m2_lcd.clear();
      break;
    case M2_GFX_MSG_END:
      if ( m2_gh_lcd_cursor_y != 255 )
      {
	m2_set_cursor_16x4(m2_gh_lcd_cursor_x, m2_gh_lcd_cursor_y);
	m2_lcd.cursor();
      }
      break;
    case M2_GFX_MSG_DRAW_TEXT:
      m2_set_cursor_16x4(arg->x, arg->y);
      m2_lcd.print(arg->s);
      return 0;
    case M2_GFX_MSG_DRAW_NORMAL_FOCUS:
    case M2_GFX_MSG_DRAW_NORMAL_PARENT_FOCUS:
      m2_set_cursor_16x4(arg->x, arg->y);
      m2_lcd.print('[');
      m2_set_cursor_16x4(arg->x+arg->w-1, arg->y);
      m2_lcd.print(']');
      return 0;
    case M2_GFX_MSG_DRAW_GO_UP:
      m2_set_cursor_16x4(arg->x-1, arg->y);
      m2_lcd.print('<');
      m2_set_cursor_16x4(arg->x+arg->w, arg->y);
      m2_lcd.print('>');
      return 0;
    case M2_GFX_MSG_DRAW_SMALL_FOCUS:
      m2_gh_lcd_cursor_x = arg->x;
      m2_gh_lcd_cursor_y = arg->y;
      return 0;
    case M2_GFX_MSG_DRAW_ICON:
      m2_set_cursor_16x4(arg->x, arg->y);
      if ( arg->icon == M2_ICON_TOGGLE_ACTIVE || arg->icon == M2_ICON_RADIO_ACTIVE )
	m2_lcd.print("\001");
      else
	m2_lcd.print("\002");
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
      return 16;
    case M2_GFX_MSG_GET_DISPLAY_HEIGHT:
      return 4;
  }
  return m2_gh_dummy(arg);
}
