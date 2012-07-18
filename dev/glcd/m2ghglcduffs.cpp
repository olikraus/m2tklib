/*

  m2ghglcdffs.cpp
  
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


extern "C" uint8_t m2_gh_glcd_uffs(m2_gfx_arg_p arg)
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
      break;
    case M2_GFX_MSG_END:
      break;
    case M2_GFX_MSG_DRAW_TEXT:
      m2_gh_glcd_set_user_font(arg->font);
      //GLCD.GotoXY(arg->x,m2_gh_glcd_y(arg->y)-m2_gh_glcd_get_user_font_height(arg)+m2_gh_glcd_get_user_font_corrcetion(arg));
      GLCD.CursorToXY(arg->x,m2_gh_glcd_y(arg->y)-m2_gh_glcd_get_user_font_height(arg)+m2_gh_glcd_get_user_font_corrcetion(arg));
      GLCD.Puts(arg->s);
      return 0;
    case M2_GFX_MSG_DRAW_NORMAL_NO_FOCUS:
      if ( (arg->font & 4) != 0 )
	m2_gh_glcd_draw_frame_shadow(arg->x, arg->y, arg->w, arg->h);
      break;
    case M2_GFX_MSG_DRAW_NORMAL_FOCUS:
    case M2_GFX_MSG_DRAW_NORMAL_PARENT_FOCUS:
      if ( (arg->font & 4) != 0 )
      {
	m2_gh_glcd_draw_xorbox(arg->x+1, arg->y+1, arg->w-3, arg->h-1);
      }
      m2_gh_glcd_draw_frame_shadow(arg->x, arg->y, arg->w, arg->h);
      break;
    case M2_GFX_MSG_DRAW_SMALL_FOCUS:
      m2_gh_glcd_draw_xorbox(arg->x, arg->y, arg->w, arg->h+1);
      break;
    
    case M2_GFX_MSG_DRAW_NORMAL_DATA_ENTRY:
      m2_gh_glcd_draw_frame_shadow(arg->x, arg->y, arg->w, arg->h);
      GLCD.DrawHLine(arg->x, m2_gh_glcd_y(arg->y)-2, arg->w-3);
      break;
    case M2_GFX_MSG_DRAW_SMALL_DATA_ENTRY:
      GLCD.DrawHLine(arg->x, m2_gh_glcd_y(arg->y), arg->w-1);
      break;
    case M2_GFX_MSG_DRAW_GO_UP:
      m2_gh_glcd_draw_xorbox(arg->x, arg->y, arg->w, arg->h/2);
      return 0;
    case M2_GFX_MSG_DRAW_ICON:
      m2_gh_glcd_draw_big_icon(arg->x,arg->y+m2_gh_glcd_get_user_font_corrcetion(arg), 
	  m2_gh_glcd_get_user_font_height(arg), m2_gh_glcd_get_user_font_height(arg), arg->icon);
      return 0;
    case M2_GFX_MSG_GET_TEXT_WIDTH:
      m2_gh_glcd_set_user_font(arg->font);
      return GLCD.StringWidth(arg->s);
    case M2_GFX_MSG_GET_ICON_WIDTH:
    case M2_GFX_MSG_GET_ICON_HEIGHT:
      return m2_gh_glcd_get_user_font_height(arg);
    case M2_GFX_MSG_GET_NUM_CHAR_WIDTH:
      m2_gh_glcd_set_user_font(arg->font);
      return GLCD.CharWidth('0');
    case M2_GFX_MSG_GET_CHAR_WIDTH:
      m2_gh_glcd_set_user_font(arg->font);
      return GLCD.CharWidth('m');
    case M2_GFX_MSG_GET_CHAR_HEIGHT:
      return m2_gh_glcd_get_user_font_height(arg);
    case M2_GFX_MSG_GET_NORMAL_BORDER_HEIGHT:
      return 3;
    case M2_GFX_MSG_GET_NORMAL_BORDER_WIDTH:
      return 3;
    case M2_GFX_MSG_GET_NORMAL_BORDER_X_OFFSET:
      return 1;
    case M2_GFX_MSG_GET_NORMAL_BORDER_Y_OFFSET:
      return 2;
    case M2_GFX_MSG_GET_SMALL_BORDER_HEIGHT:
      return 0;
    case M2_GFX_MSG_GET_SMALL_BORDER_WIDTH:
      return 0;
    case M2_GFX_MSG_GET_SMALL_BORDER_X_OFFSET:
      return 0;
    case M2_GFX_MSG_GET_SMALL_BORDER_Y_OFFSET:
      return 0;
      
    case M2_GFX_MSG_GET_READONLY_BORDER_HEIGHT:
      return 3;
    case M2_GFX_MSG_GET_READONLY_BORDER_WIDTH:
      return 3;
    case M2_GFX_MSG_GET_READONLY_BORDER_X_OFFSET:
      return 1;
    case M2_GFX_MSG_GET_READONLY_BORDER_Y_OFFSET:
      return 2;
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
  }
  return m2_gh_glcd_base(arg);
}
