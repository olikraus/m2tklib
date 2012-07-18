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
#include <glcd.h>
#include "m2ghglcd.h"


extern "C" uint8_t m2_gh_glcd_bf(m2_gfx_arg_p  arg)
{
  switch(arg->msg)
  {
    case M2_GFX_MSG_INIT:
      m2_is_glcd_init=0;
      break;
    case M2_GFX_MSG_START:
      if ( m2_is_glcd_init == 0 )
      {
	GLCD.Init(NON_INVERTED);   // initialise the library, non inverted writes pixels onto a clear screen
	m2_is_glcd_init = 1;
      }
      GLCD.ClearScreen();  
      //GLCD.FillRect(0, 0, 64, 61, BLACK); 
      break;
    case M2_GFX_MSG_END:
      break;
    case M2_GFX_MSG_DRAW_TEXT:
      m2_gh_glcd_set_font(arg->font);
      //GLCD.GotoXY(arg->x,m2_gh_glcd_y(arg->y)-m2_gh_glcd_get_font_height(arg)+m2_gh_glcd_get_font_corrcetion(arg));
      GLCD.CursorToXY(arg->x,m2_gh_glcd_y(arg->y)-m2_gh_glcd_get_font_height(arg)+m2_gh_glcd_get_font_corrcetion(arg));
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
      GLCD.DrawHLine(arg->x, m2_gh_glcd_y(arg->y), arg->w-1);
      break;
    case M2_GFX_MSG_DRAW_SMALL_DATA_ENTRY:
      GLCD.DrawHLine(arg->x, m2_gh_glcd_y(arg->y), arg->w-1);
      break;
    
    case M2_GFX_MSG_DRAW_GO_UP:
      m2_gh_glcd_draw_xorbox(arg->x, arg->y, arg->w, arg->h/2);
      return 0;
    case M2_GFX_MSG_DRAW_ICON:
      m2_gh_glcd_draw_icon(arg->x,arg->y+m2_gh_glcd_get_font_corrcetion(arg), 
	  m2_gh_glcd_get_font_height(arg), m2_gh_glcd_get_font_height(arg), arg->icon);
      return 0;
    case M2_GFX_MSG_GET_TEXT_WIDTH:
      m2_gh_glcd_set_font(arg->font);
      return GLCD.StringWidth(arg->s);
    case M2_GFX_MSG_GET_ICON_WIDTH:
    case M2_GFX_MSG_GET_ICON_HEIGHT:
      return m2_gh_glcd_get_font_height(arg);
    case M2_GFX_MSG_GET_NUM_CHAR_WIDTH:
      m2_gh_glcd_set_font(arg->font);
      return GLCD.CharWidth('0');
    case M2_GFX_MSG_GET_CHAR_WIDTH:
      m2_gh_glcd_set_font(arg->font);
      return GLCD.CharWidth('m');
    case M2_GFX_MSG_GET_CHAR_HEIGHT:
      return m2_gh_glcd_get_font_height(arg);
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
    case M2_GFX_MSG_IS_FRAME_DRAW_AT_END:
      return 1;
    case M2_GFX_MSG_DRAW_VERTICAL_SCROLL_BAR:
      /* scroll bar: "total" total number of items */
      /* scroll bar: "top" topmost item (first visible item) 0 .. total-visible*/
      /* scroll bar: "visible" number of visible items 0 .. total-1 */

      m2_gh_glcd_draw_frame(arg->x, arg->y, arg->w, arg->h);
      {
	uint16_t h, y;
	
	h = m2_utl_sb_get_slider_height(arg->h-2, arg->total, arg->visible);
	y = m2_utl_sb_get_slider_position(arg->h-2, h, arg->total, arg->visible, arg->top); 

	
	m2_gh_glcd_draw_xorbox(arg->x+1, arg->y+arg->h-1-h-y, arg->w-2, h);
      }
      return 1;
  }
  return m2_gh_dummy(arg);
}

