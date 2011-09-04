/*

  m2ghdogmutl.c
  
  utility procedure for the dogm128 graphics handler
  
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

void m2_dogm_draw_frame(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h)
{
  uint8_t x1; 
  uint8_t y1;
  x1 = x0;
  y1 = y0;
  w--;
  h--;
  x1 += w;
  y1 += h;
  dog_SetHLine(x0, x1, y0);
  dog_SetHLine(x0, x1, y1);
  dog_SetVLine(x0, y0, y1);
  dog_SetVLine(x1, y0, y1);
}

void m2_dogm_draw_frame_shadow(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h)
{
  uint8_t x1; 
  uint8_t y1;
  x1 = x0;
  y1 = y0;
  w-=2;
  h-=2;
  x1 += w;
  y1 += h;
  
  y0++;
  y1++;

  dog_SetHLine(x0, x1, y0);
  dog_SetHLine(x0, x1, y1);
  dog_SetVLine(x0, y0, y1);
  dog_SetVLine(x1, y0, y1);
  x0++;
  x1++;
  y0--;
  y1--;
  dog_SetHLine(x0, x1, y0);
  dog_SetVLine(x1, y0, y1);
}


void m2_dogm_draw_xorbox(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h)
{
  uint8_t x1; 
  uint8_t y1;
  x1 = x0;
  y1 = y0;
  w--;
  h--;
  x1 += w;
  y1 += h;
  dog_XorBox(x0, y0, x1, y1);
}


/*
  00000000 0x00
  01111110 0x7e
  01000010 0x42
  01000010 0x42
  01000010 0x42
  01000010 0x42
  01111110 0x7e
  00000000 0x00

  00000000 0x00
  01111111 0x7f
  01000011 0x43
  01100110 0x66
  01111110 0x7e
  01011010 0x5a
  01111110 0x7e
  00000000 0x00

dog_pgm_uint8_t m2_dogm_icon0[8] = {0x00, 0x7e, 0x042, 0x042, 0x042, 0x042, 0x07e, 0x00};
dog_pgm_uint8_t m2_dogm_icon1[8] = {0x00, 0x7f, 0x043, 0x066, 0x07e, 0x05a, 0x07e, 0x00};


  11111111 0xff
  10000001 0x81
  10000001 0x81
  10000001 0x81
  10000001 0x81
  10000001 0x81
  10000001 0x81
  11111111 0xff

  11111111 0xff
  10000001 0x81
  10111101 0xbd
  10111101 0xbd
  10111101 0xbd
  10111101 0xbd
  10000001 0x81
  11111111 0xff

*/

dog_pgm_uint8_t m2_dogm_icon0[8] = {0x0ff, 0x081, 0x081, 0x081, 0x081, 0x081, 0x081, 0x0ff};
dog_pgm_uint8_t m2_dogm_icon1[8] = {0x0ff, 0x081, 0x0bd, 0x0bd, 0x0bd, 0x0bd, 0x081, 0x0ff};

/*
#define M2_ICON_TOGGLE_ACTIVE 1
#define M2_ICON_TOGGLE_INACTIVE 2 
#define M2_ICON_RADIO_ACTIVE 3
#define M2_ICON_RADIO_INACTIVE 4
*/

void m2_dogm_draw_icon(uint8_t x, uint8_t y, uint8_t font, uint8_t icon)
{
  dog_pgm_uint8_t *ptr = m2_dogm_icon0;
  if ( icon == M2_ICON_TOGGLE_ACTIVE || icon == M2_ICON_RADIO_ACTIVE )
    ptr = m2_dogm_icon1;
  dog_SetBitmapP(x,y+7,ptr,8,8);
}

uint8_t m2_dogm_get_icon_height(uint8_t font, uint8_t icon)
{
  return 8;
}

uint8_t m2_dogm_get_icon_width(uint8_t font, uint8_t icon)
{
  return 8;
}


DOG_PGM_P m2_gh_dogm_fonts[4];


uint8_t m2_gh_dogm_base(m2_gfx_arg_p  arg)
{
  switch(arg->msg)
  {
    case M2_GFX_MSG_DRAW_TEXT:
      {
	uint8_t x = arg->x;
	uint8_t y;
	if ( (arg->font & 8) != 0 )
	  if ( arg->w != 0 )
	  {
	    x = arg->w;
	    x -= dog_GetStrWidth(m2_dogm_get_font(arg->font), arg->s);
	    x >>= 1;
	    x += arg->x;
	  }
	y = arg->y;
	y += dog_GetFontBBXDescent(m2_dogm_get_font(arg->font));
	y--;			/* Correction: Baseline for DOG fonts is one pixel above */
	dog_DrawStr(x, y, m2_dogm_get_font(arg->font), arg->s);
      }
      break;
    case M2_GFX_MSG_DRAW_TEXT_P:
      {
	uint8_t x = arg->x;
	uint8_t y;
	if ( (arg->font & 8) != 0 )
	  if ( arg->w != 0 )
	  {
	    x = arg->w;
	    x -= dog_GetStrWidthP(m2_dogm_get_font(arg->font), arg->s);
	    x >>= 1;
	    x += arg->x;
	  }
	y = arg->y;
	y += dog_GetFontBBXDescent(m2_dogm_get_font(arg->font));
	y--;			/* Correction: Baseline for DOG fonts is one pixel above */
	dog_DrawStrP(x, y, m2_dogm_get_font(arg->font), arg->s);
      }
      break;
    case M2_GFX_MSG_SET_FONT:
      {
	uint8_t idx;
	idx = arg->font;
	idx &=3;
	m2_gh_dogm_fonts[idx] = (DOG_PGM_P)(arg->s);
      }
      return 0;
    case M2_GFX_MSG_GET_DISPLAY_WIDTH:
      return DOG_WIDTH;
    case M2_GFX_MSG_GET_DISPLAY_HEIGHT:
      return DOG_HEIGHT;
    case M2_GFX_MSG_DRAW_VERTICAL_SCROLL_BAR:
      /* scroll bar: "total" total number of items */
      /* scroll bar: "top" topmost item (first visible item) 0 .. total-visible*/
      /* scroll bar: "visible" number of visible items 0 .. total-1 */

      m2_dogm_draw_frame(arg->x, arg->y, arg->w, arg->h);
      {
	uint16_t h, y;
	
	h = m2_utl_sb_get_slider_height(arg->h-2, arg->total, arg->visible);
	y = m2_utl_sb_get_slider_position(arg->h-2, h, arg->total, arg->visible, arg->top); 

	
	m2_dogm_draw_xorbox(arg->x+1, arg->y+arg->h-1-h-y, arg->w-2, h);
      }
      return 1;
  }
  return m2_gh_dummy(arg);
}

DOG_PGM_P m2_dogm_get_font(uint8_t font)
{
  font &= 3;
  return m2_gh_dogm_fonts[font];
}

