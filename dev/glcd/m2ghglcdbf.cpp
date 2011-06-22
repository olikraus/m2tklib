/*

  m2ghglcdbf.cpp
  
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
#include "m2.h"

#ifdef AAA

#include <glcd.h>
#include "fonts/Arial14.h"         
#include "fonts/SystemFont5x7.h"


uint8_t m2_gh_glcd_y(uint8_t y)
{
  uint8_t val;
  val = GLCD.Height;
  val--;
  val -= y;
  return val;
}

void m2_gh_glcd_set_font(uint8_t font)
{
  font &= 1;
  if ( font == 0 )
      GLCD.SelectFont(System5x7); 
  else 
      GLCD.SelectFont(Arial14); 
}

void m2_gh_glcd_draw_frame(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h)
{
  w--;
  h--;
  GLCD.DrawRect(x0, y0, w, h);
}

void m2_gh_glcd_draw_xorbox(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h)
{
  w--;
  h--;
  GLCD.InvertRect(x0, y0, w, h);
}
#endif

extern "C" uint8_t m2_gh_glcd_bf(m2_gfx_arg_p  arg)
{
  return 0;
#ifdef AAA
  switch(arg->msg)
  {
    case M2_GFX_MSG_INIT:
      break;
    case M2_GFX_MSG_START:
      //GLCD.ClearScreen();  
      break;
    case M2_GFX_MSG_END:
      break;
    case M2_GFX_MSG_DRAW_TEXT:
      m2_gh_glcd_set_font(arg->font);
      GLCD.GotoXY(arg->x,m2_gh_glcd_y(arg->y));
      GLCD.Puts(arg->s);
      return 0;
    case M2_GFX_MSG_DRAW_NORMAL_NO_FOCUS:
      if ( (arg->font & 4) != 0 )
	m2_gh_glcd_draw_frame(arg->x, arg->y, arg->w, arg->h);
      break;
    case M2_GFX_MSG_DRAW_NORMAL_FOCUS:
      m2_gh_glcd_draw_xorbox(arg->x, arg->y, arg->w, arg->h);
      break;
    case M2_GFX_MSG_DRAW_SMALL_FOCUS:
      m2_gh_glcd_draw_xorbox(arg->x, arg->y, arg->w, arg->h);
      break;
    
    case M2_GFX_MSG_DRAW_NORMAL_DATA_ENTRY:
      GLCD.DrawHLine(arg->x, arg->y, arg->w-1);
      break;
    case M2_GFX_MSG_DRAW_SMALL_DATA_ENTRY:
      GLCD.DrawHLine(arg->x, arg->y, arg->w-1);
      break;
    
    case M2_GFX_MSG_DRAW_GO_UP:
      m2_gh_glcd_draw_xorbox(arg->x, arg->y, arg->w, arg->h/2);
      return 0;
    case M2_GFX_MSG_DRAW_ICON:
      m2_gh_glcd_set_font(arg->font);
      GLCD.GotoXY(arg->x,m2_gh_glcd_y(arg->y));
      if ( arg->icon == M2_ICON_TOGGLE_ACTIVE || arg->icon == M2_ICON_RADIO_ACTIVE )
	GLCD.Puts("+");
      else
	GLCD.Puts("-");
      return 0;
    case M2_GFX_MSG_GET_TEXT_WIDTH:
      m2_gh_glcd_set_font(arg->font);
      return GLCD.StringWidth(arg->s);
    case M2_GFX_MSG_GET_ICON_WIDTH:
      m2_gh_glcd_set_font(arg->font);
      return GLCD.CharWidth('m');
    case M2_GFX_MSG_GET_ICON_HEIGHT:
      if ( (arg->font & 1) == 0 )
	return 8;
      return 16;    
    case M2_GFX_MSG_GET_CHAR_WIDTH:
      m2_gh_glcd_set_font(arg->font);
      return GLCD.CharWidth('m');
    case M2_GFX_MSG_GET_CHAR_HEIGHT:
      if ( (arg->font & 1) == 0 )
	return 8;
      return 16;
    case M2_GFX_MSG_GET_NORMAL_BORDER_HEIGHT:
      if ( (arg->font & 4) != 0 )
	return 2;
      return 0;
    case M2_GFX_MSG_GET_NORMAL_BORDER_WIDTH:
      if ( (arg->font & 4) != 0 )
	return 2;
      return 0;
    case M2_GFX_MSG_GET_NORMAL_BORDER_X_OFFSET:
      if ( (arg->font & 4) != 0 )
	return 1;
      return 0;
    case M2_GFX_MSG_GET_NORMAL_BORDER_Y_OFFSET:
      if ( (arg->font & 4) != 0 )
	return 1;
      return 0;
    case M2_GFX_MSG_GET_SMALL_BORDER_HEIGHT:
      return 0;
    case M2_GFX_MSG_GET_SMALL_BORDER_WIDTH:
      return 0;
    case M2_GFX_MSG_GET_SMALL_BORDER_X_OFFSET:
      return 0;
    case M2_GFX_MSG_GET_SMALL_BORDER_Y_OFFSET:
      return 0;
      
    case M2_GFX_MSG_GET_READONLY_BORDER_HEIGHT:
      return 0;
    case M2_GFX_MSG_GET_READONLY_BORDER_WIDTH:
      return 0;
    case M2_GFX_MSG_GET_READONLY_BORDER_X_OFFSET:
      return 0;
    case M2_GFX_MSG_GET_READONLY_BORDER_Y_OFFSET:
      return 0;
    case M2_GFX_MSG_GET_LIST_OVERLAP_HEIGHT:
      return 0;
    case M2_GFX_MSG_GET_LIST_OVERLAP_WIDTH:
      return 0;
    case M2_GFX_MSG_GET_DISPLAY_WIDTH:
      return GLCD.Width;
    case M2_GFX_MSG_GET_DISPLAY_HEIGHT:
      return GLCD.Height;
  }
  return m2_gh_dummy(arg);
#endif 
}
