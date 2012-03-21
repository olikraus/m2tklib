/*

  m2ghdoglcd.cpp
  
  Graphics Handler for the DogLcd library from the Arduino Environment
  http://code.google.com/p/doglcd/

  m2tklib = Mini Interative Interface Toolkit Library
  
  Copyright (C) 2012  olikraus@gmail.com

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

#include <DogLcd.h>
#include <string.h>
#include "m2.h"


DogLcd *m2_gh_doglcd_ptr;
uint8_t m2_gh_doglcd_rows;
uint8_t m2_gh_doglcd_cols;

uint8_t m2_gh_doglcd_icon_inactive[] = { 0x00, 0x1f, 0x11, 0x11, 0x11, 0x11, 0x1f, 0x00 };
uint8_t m2_gh_doglcd_icon_active[] = { 0x00, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x00 };

uint8_t m2_gh_doglcd_cursor_x;
uint8_t m2_gh_doglcd_cursor_y = 255;

void m2_SetDogLcd(DogLcd *doglcd_ptr, int model, int contrast=0x28, int vcc=DOG_LCD_VCC_5V)
{
  m2_gh_doglcd_rows = 2;
  m2_gh_doglcd_cols = 16;
  if ( model == DOG_LCD_M081 )
  {
    m2_gh_doglcd_rows = 1;
    m2_gh_doglcd_cols = 8;
  }
  else if ( model == DOG_LCD_M162 ) 
 {
    m2_gh_doglcd_rows = 2;
    m2_gh_doglcd_cols = 16;
  }
  else if ( model == DOG_LCD_M163 )
  {
    m2_gh_doglcd_rows = 3;
    m2_gh_doglcd_cols = 16;
  }
  
  m2_gh_doglcd_ptr = doglcd_ptr;
  m2_gh_doglcd_ptr->begin(model, contrast, vcc);
  //m2_gh_doglcd_ptr->print("xyz");
  m2_gh_doglcd_ptr->createChar(1, m2_gh_doglcd_icon_active);
  m2_gh_doglcd_ptr->createChar(2, m2_gh_doglcd_icon_inactive);
}

static void m2_gh_doglcd_set_cursor(uint8_t x, uint8_t y)
{
  /* translate the x-y position from the m2tklib to the LiquidCrystal lib */
  /* assume 16x4 display */
  
  if ( y >= m2_gh_doglcd_rows )
    return;
  
  y = m2_gh_doglcd_rows-y;
  y--;
  
  m2_gh_doglcd_ptr->setCursor(x, y);
}

/*
  t: total height, 16 for two line display, 32 for 4 line display
  b: border from top
  y: distance from top where the slider starts (true start is y+b)
  h: height of the slider
*/
void m2_gh_doglcd_set_vsb_char(uint8_t t, uint8_t b, uint8_t y, uint8_t h, uint8_t byte_index)
{
  uint8_t i;
  uint8_t d;
  uint8_t m2_gh_vsb_char[8]; 
  for( i = 0; i < t; i++ )
  {
    d = 0x11;
    if ( i == 0 || i+1 == t )
      d = 0x01f;
    else if ( i >= y+b && i < y+b+h )
      d = 0x01f;
    if ( (i >> 3) == byte_index )
    {
      m2_gh_vsb_char[i&7] = d;
    }
  }
  
  m2_gh_doglcd_ptr->createChar(3+byte_index, m2_gh_vsb_char);
  
}

extern "C" uint8_t m2_gh_doglcd(m2_gfx_arg_p  arg)
{
  uint8_t backup_SPCR = SPCR;
  SPCR = 0;
  switch(arg->msg)
  {
    case M2_GFX_MSG_INIT:
      /* should already be done in m2_SetDogLcd */
      break;
    case M2_GFX_MSG_START:
      m2_gh_doglcd_cursor_y = 255;
      m2_gh_doglcd_ptr->noCursor();
      m2_gh_doglcd_ptr->noBlink();
      m2_gh_doglcd_ptr->clear();
      break;
    case M2_GFX_MSG_END:
      if ( m2_gh_doglcd_cursor_y != 255 )
      {
	m2_gh_doglcd_set_cursor(m2_gh_doglcd_cursor_x, m2_gh_doglcd_cursor_y);
	m2_gh_doglcd_ptr->cursor();
      }
      break;
    case M2_GFX_MSG_DRAW_TEXT:
      m2_gh_doglcd_set_cursor(arg->x, arg->y);
      m2_gh_doglcd_ptr->print(arg->s);
      SPCR = backup_SPCR;
      return 0;
    case M2_GFX_MSG_DRAW_NORMAL_FOCUS:
    case M2_GFX_MSG_DRAW_NORMAL_PARENT_FOCUS:
      m2_gh_doglcd_set_cursor(arg->x, arg->y);
      m2_gh_doglcd_ptr->print('[');
      m2_gh_doglcd_set_cursor(arg->x+arg->w-1, arg->y);
      m2_gh_doglcd_ptr->print(']');
      SPCR = backup_SPCR;
      return 0;
    case M2_GFX_MSG_DRAW_GO_UP:
      m2_gh_doglcd_set_cursor(arg->x-1, arg->y);
      m2_gh_doglcd_ptr->print('<');
      m2_gh_doglcd_set_cursor(arg->x+arg->w, arg->y);
      m2_gh_doglcd_ptr->print('>');
      SPCR = backup_SPCR;
      return 0;
    case M2_GFX_MSG_DRAW_SMALL_FOCUS:
      m2_gh_doglcd_cursor_x = arg->x;
      m2_gh_doglcd_cursor_y = arg->y;
      SPCR = backup_SPCR;
      return 0;
    case M2_GFX_MSG_DRAW_ICON:
      m2_gh_doglcd_set_cursor(arg->x, arg->y);
      if ( arg->icon == M2_ICON_TOGGLE_ACTIVE || arg->icon == M2_ICON_RADIO_ACTIVE )
	m2_gh_doglcd_ptr->print("\001");
      else
	m2_gh_doglcd_ptr->print("\002");
      SPCR = backup_SPCR;
      return 0;
    case M2_GFX_MSG_GET_TEXT_WIDTH:
      SPCR = backup_SPCR;
      return strlen(arg->s);
    case M2_GFX_MSG_GET_ICON_WIDTH:
    case M2_GFX_MSG_GET_ICON_HEIGHT:
    case M2_GFX_MSG_GET_CHAR_WIDTH:
    case M2_GFX_MSG_GET_CHAR_HEIGHT:
      SPCR = backup_SPCR;
      return 1;
    case M2_GFX_MSG_GET_NORMAL_BORDER_WIDTH:
      SPCR = backup_SPCR;
      return 2;
    case M2_GFX_MSG_GET_NORMAL_BORDER_X_OFFSET:
      SPCR = backup_SPCR;
      return 1;
    case M2_GFX_MSG_GET_DISPLAY_WIDTH:
      SPCR = backup_SPCR;
      return m2_gh_doglcd_cols;
    case M2_GFX_MSG_GET_DISPLAY_HEIGHT:
      SPCR = backup_SPCR;
      return m2_gh_doglcd_rows;
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
	  m2_gh_doglcd_set_vsb_char(arg->h*8, 1, y, h, byte_index);
	  m2_gh_doglcd_set_cursor(arg->x, arg->y+arg->h-1-byte_index);
	  c = 3;
	  c += byte_index;
	  m2_gh_doglcd_ptr->print(c);
	}
      }
      SPCR = backup_SPCR;
      return 1;
  }
  SPCR = backup_SPCR;
  return m2_gh_dummy(arg);
}
