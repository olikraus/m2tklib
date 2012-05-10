/*

  m2ghdogmfbs.c
  
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
#include "m2ghdogm.h"

/*
  focus cursor: shadow frame
  buttom style: filled (xor) box
*/

void m2_dogm_draw_frame_dogx160(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, uint8_t is_in)
{
  uint8_t x1; 
  uint8_t y1;
  uint8_t c0 = 0;
  uint8_t c2 = 2;
  if ( is_in )
  {
    c0 = 2;
    c2 = 0;
  }
  x1 = x0;
  y1 = y0;
  w-=1;
  h-=1;
  x1 += w;
  y1 += h;
  
  //y0++;
  //y1++;

  dog_SetPixelValue(c0);
  dog_SetHLine(x0, x1, y1);
  dog_SetVLine(x0, y0, y1);
  dog_SetPixelValue(c2);
  dog_SetHLine(x0, x1, y0);
  dog_SetVLine(x1, y0, y1);
  /*
  if ( is_in == 0 )
  {  
    x0++;
    x1++;
    y0--;
    y1--;
    dog_SetHLine(x0, x1, y0);
    dog_SetVLine(x1, y0, y1);
  }
  */
  dog_SetPixelValue(3);
}

uint8_t m2_gh_dogxl160(m2_gfx_arg_p arg)
{
  switch(arg->msg)
  {
    case M2_GFX_MSG_INIT:		
      break;
    case M2_GFX_MSG_START:
      dog_SetPixelValue(1);
      dog_SetBox(0, 0, DOG_WIDTH-1, DOG_HEIGHT-1);
      dog_SetPixelValue(3);
      break;
    case M2_GFX_MSG_END:
      break;
    case M2_GFX_MSG_DRAW_HLINE:
      dog_SetHLine(arg->x, arg->x+arg->w, arg->y);
      break;
    case M2_GFX_MSG_DRAW_VLINE:
      dog_SetVLine(arg->x, arg->y, arg->y+arg->h);
      break;
    case M2_GFX_MSG_DRAW_BOX:
      dog_SetBox(arg->x, arg->y, arg->x+arg->w, arg->y+arg->h);
      break;
    case M2_GFX_MSG_DRAW_TEXT:
    {
      /* arg->w contains the cell width if arg->w > 0 */
      uint8_t x = arg->x;
      if ( (arg->font & 8) != 0 )
	if ( arg->w != 0 )
	{
	  x = arg->w;
	  x -= dog_GetStrWidth(m2_dogm_get_font(arg->font), arg->s);
	  x >>= 1;
	  x += arg->x;
	}
      dog_DrawStr(x, arg->y+dog_GetFontBBXDescent(m2_dogm_get_font(arg->font)), m2_dogm_get_font(arg->font), arg->s);
      break;
    }
    case M2_GFX_MSG_DRAW_TEXT_P:
      dog_DrawStrP(arg->x, arg->y+dog_GetFontBBXDescent(m2_dogm_get_font(arg->font)), m2_dogm_get_font(arg->font), arg->s);
      break;
    case M2_GFX_MSG_DRAW_NORMAL_NO_FOCUS:
      if ( (arg->font & 4) != 0 )
	m2_dogm_draw_frame_dogx160(arg->x, arg->y, arg->w, arg->h, 0);
	//m2_dogm_draw_xorbox(arg->x, arg->y, arg->w, arg->h);
      break;
    case M2_GFX_MSG_DRAW_NORMAL_FOCUS:
    case M2_GFX_MSG_DRAW_NORMAL_PARENT_FOCUS:
      m2_dogm_draw_frame_dogx160(arg->x, arg->y, arg->w, arg->h, 1);
      break;
    case M2_GFX_MSG_DRAW_SMALL_FOCUS:
      m2_dogm_draw_xorbox(arg->x, arg->y, arg->w, arg->h);
      break;
    case M2_GFX_MSG_DRAW_NORMAL_DATA_ENTRY:
      m2_dogm_draw_frame_dogx160(arg->x, arg->y, arg->w, arg->h, 1);
      break;
    case M2_GFX_MSG_DRAW_GO_UP:
      m2_dogm_draw_xorbox(arg->x, arg->y, arg->w, arg->h);
      break;
    case M2_GFX_MSG_DRAW_ICON:
      m2_dogm_draw_icon(arg->x, arg->y, arg->font, arg->icon);
      break;    
    case M2_GFX_MSG_GET_TEXT_WIDTH:
      return dog_GetStrWidth(m2_dogm_get_font(arg->font), arg->s);
    case M2_GFX_MSG_GET_TEXT_WIDTH_P:
      return dog_GetStrWidthP(m2_dogm_get_font(arg->font), arg->s);
    case M2_GFX_MSG_GET_NUM_CHAR_WIDTH:
    case M2_GFX_MSG_GET_CHAR_WIDTH:
      return dog_GetFontBBXWidth(m2_dogm_get_font(arg->font));
    case M2_GFX_MSG_GET_CHAR_HEIGHT:
      return dog_GetFontBBXHeight(m2_dogm_get_font(arg->font));

    case M2_GFX_MSG_GET_NORMAL_BORDER_HEIGHT:
      return 2;
    case M2_GFX_MSG_GET_NORMAL_BORDER_WIDTH:
      return 2;
    case M2_GFX_MSG_GET_NORMAL_BORDER_X_OFFSET:
      return 1;
    case M2_GFX_MSG_GET_NORMAL_BORDER_Y_OFFSET:
      return 1;
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
    case M2_GFX_MSG_GET_ICON_WIDTH:
      return m2_dogm_get_icon_width(arg->font, arg->icon);
    case M2_GFX_MSG_GET_ICON_HEIGHT:
      return m2_dogm_get_icon_height(arg->font, arg->icon);
    case M2_GFX_MSG_IS_FRAME_DRAW_AT_END:
      return 1;
  }
  return m2_gh_dogm_base(arg);
}
