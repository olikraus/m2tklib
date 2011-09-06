/*

  m2ghglcdutil.cpp
  
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
#include <glcd.h>
#include "fonts/Arial14.h"         
#include "fonts/SystemFont5x7.h"
#include "fonts/fixednums7x15.h"
#include "fonts/fixednums15x31.h"

void *m2_Arial14 = Arial14;
void *m2_System5x7 = System5x7;
void *m2_fixednums7x15 = fixednums7x15;
void *m2_fixednums15x31 = fixednums15x31;

uint8_t m2_is_glcd_init = 0;

extern "C" uint8_t m2_gh_glcd_y(uint8_t y)
{
  uint8_t val;
  val = GLCD.Height;
  val--;
  val -= y;
  return val;
}

extern "C" uint8_t m2_gh_glcd_get_font_height(m2_gfx_arg_p  arg)
{
  if ( (arg->font & 1) == 0 )
    return 8;
  return 16;    
}

extern "C" uint8_t m2_gh_glcd_get_font_corrcetion(m2_gfx_arg_p  arg)
{
  if ( (arg->font & 1) == 0 )
    return 1;
  return 2;    
}

extern "C" void m2_gh_glcd_set_font(uint8_t font)
{
  font &= 1;
  if ( font == 0 )
      GLCD.SelectFont(System5x7); 
  else 
      GLCD.SelectFont(Arial14); 
}


/* y0 is in m2 coordinate system, (x0,y0) = (0,0) = lower left edge  */
extern "C" void m2_gh_glcd_draw_frame(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h)
{
  w--;
  h--;
  y0 +=h;
  GLCD.DrawRect(x0, m2_gh_glcd_y(y0), w, h);
}

extern "C" void m2_gh_glcd_draw_frame_shadow(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h)
{  
  GLCD.DrawHLine(x0+1, m2_gh_glcd_y(y0), w-2);
  GLCD.DrawVLine(x0+w-1, m2_gh_glcd_y(y0)-h+1, h-1);
  m2_gh_glcd_draw_frame(x0, y0+1, w-1, h);
}


/* y0 is in m2 coordinate system, (x0,y0) = (0,0) = lower left edge  */
extern "C" void m2_gh_glcd_draw_xorbox(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h)
{
  w--;
  h--;
  y0 +=h;
  GLCD.InvertRect(x0, m2_gh_glcd_y(y0), w, h);
}

/*
#define M2_ICON_TOGGLE_ACTIVE 1
#define M2_ICON_TOGGLE_INACTIVE 2 
#define M2_ICON_RADIO_ACTIVE 3
#define M2_ICON_RADIO_INACTIVE 4
struct _m2_gfx_arg
{
  uint8_t msg;
  uint8_t x;
  uint8_t y;
  uint8_t w;
  uint8_t h;
  uint8_t font;
  uint8_t icon;
  const char *s;
};
*/

/* y0 is in m2 coordinate system, (x0,y0) = (0,0) = lower left edge  */
extern "C" void m2_gh_glcd_draw_icon(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, uint8_t icon)
{
  x0++;
  y0++;
  w-=2;
  h-=2;
  m2_gh_glcd_draw_frame(x0, y0, w, h);
  if ( icon == M2_ICON_TOGGLE_ACTIVE || icon == M2_ICON_RADIO_ACTIVE )
  {
    x0+=2;
    y0+=2;
    w-=4;
    h-=4;
    m2_gh_glcd_draw_xorbox(x0, y0, w, h);
  }
}

extern "C" void m2_gh_glcd_draw_big_icon(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, uint8_t icon)
{
  m2_gh_glcd_draw_frame(x0, y0, w, h);
  if ( icon == M2_ICON_TOGGLE_ACTIVE || icon == M2_ICON_RADIO_ACTIVE )
  {
    x0+=2;
    y0+=2;
    w-=4;
    h-=4;
    m2_gh_glcd_draw_xorbox(x0, y0, w, h);
  }
}

/* selectable user font */

const void *m2_gh_glcd_fonts[4];

extern "C" uint8_t m2_gh_glcd_get_user_font_height(m2_gfx_arg_p  arg)
{
  uint8_t idx = arg->font;
  PGM_P font;
  
  idx &= 3;
  font = (PGM_P)m2_gh_glcd_fonts[idx];
  font += FONT_HEIGHT;
  return pgm_read_byte(font); 
}

extern "C" uint8_t m2_gh_glcd_get_user_font_corrcetion(m2_gfx_arg_p  arg)
{
  if (m2_gh_glcd_get_user_font_height(arg) > 8)
    return 2;
  return 1;
}

extern "C" void m2_gh_glcd_set_user_font(uint8_t font)
{
  font &= 3;
  GLCD.SelectFont((const uint8_t *)m2_gh_glcd_fonts[font]);
}

extern "C" uint8_t m2_gh_glcd_base(m2_gfx_arg_p  arg)
{
  switch(arg->msg)
  {
    case M2_GFX_MSG_SET_FONT:
      {
	uint8_t idx;
	idx = arg->font;
	idx &=3;
	m2_gh_glcd_fonts[idx] = arg->s;
      }
      return 0;
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
