/*

  m2gh3310lcd.c
  
  Graphics Handler for the nokia_3310_lcd library (nuelectronics.com)

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

#include <string.h>
#include "n3310_lcd.h"          /* use C interface */
#include "m2.h"

static void m2_nokia_3310_set_cursor(uint8_t x, uint8_t y)
{
  /* translate the x-y position from the m2tklib to the LiquidCrystal lib */
  /* assume 14x6 nokia 3310 display */
  
  y = 5-y;
  x *= 6;
  
  LCD_set_XY(x, y);
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

/*
bitmaps not yet converted, need to be rotated by 90 degree
uint8_t m2_gh_icon_inactive[] = { 0x00, 0x1f, 0x11, 0x11, 0x11, 0x11, 0x1f, 0x00 };
uint8_t m2_gh_icon_active[] = { 0x00, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x00 };
*/

/* used to mark the small focus, not supported at the moment for the 3310 */
uint8_t m2_gh_nokia_3310_cursor_x = 0;
uint8_t m2_gh_nokia_3310_cursor_y = 255;

uint8_t m2_gh_nokia_3310(m2_gfx_arg_p  arg)
{
  switch(arg->msg)
  {
    case M2_GFX_MSG_INIT:		
      LCD_init();
      break;
    case M2_GFX_MSG_START:
      m2_gh_nokia_3310_cursor_y = 255;
      LCD_clear();
      break;
    case M2_GFX_MSG_END:
      if ( m2_gh_nokia_3310_cursor_y != 255 )
      {
	m2_nokia_3310_set_cursor(m2_gh_nokia_3310_cursor_x, m2_gh_nokia_3310_cursor_y);
        /* not clear, what to do now, because, the char/string for the focus is unknown */
        /* u32 element: the text after the small_focus message contains the small focus text */
      }
      break;
    case M2_GFX_MSG_DRAW_TEXT:
      LCD_write_string(arg->x, arg->y,arg->s, MENU_NORMAL);
      return 0;
    case M2_GFX_MSG_DRAW_NORMAL_FOCUS:
    case M2_GFX_MSG_DRAW_NORMAL_PARENT_FOCUS:
      m2_nokia_3310_set_cursor(arg->x, arg->y);
      LCD_write_char('[', MENU_NORMAL);
      m2_nokia_3310_set_cursor(arg->x+arg->w-1, arg->y);
      LCD_write_char(']', MENU_NORMAL);
      return 0;
    case M2_GFX_MSG_DRAW_GO_UP:
      m2_nokia_3310_set_cursor(arg->x-1, arg->y);
      LCD_write_char('<', MENU_NORMAL);
      m2_nokia_3310_set_cursor(arg->x+arg->w, arg->y);
      LCD_write_char('>', MENU_NORMAL);
      return 0;
    case M2_GFX_MSG_DRAW_SMALL_FOCUS:
      m2_gh_nokia_3310_cursor_x = arg->x;
      m2_gh_nokia_3310_cursor_y = arg->y;
      return 0;
    case M2_GFX_MSG_DRAW_ICON:
      m2_nokia_3310_set_cursor(arg->x, arg->y);
      /* not yet converted
      if ( arg->icon == M2_ICON_TOGGLE_ACTIVE || arg->icon == M2_ICON_RADIO_ACTIVE )
	m2_lcd.print("\001");
      else
	m2_lcd.print("\002");
      return 0;
      */
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
      return 14;
    case M2_GFX_MSG_GET_DISPLAY_HEIGHT:
      return 6;
  }
  return m2_gh_dummy(arg);
}
