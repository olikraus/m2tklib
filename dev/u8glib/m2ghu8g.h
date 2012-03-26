/*

  m2ghu8g.h
  
  graphics handler for u8glib
  
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

#ifndef _M2GHU8G_H
#define _M2GHU8G_H

#include "m2.h"
#include "u8g.h"

#ifdef __cplusplus
extern "C" {
#endif

extern u8g_t *m2_u8g;
extern u8g_uint_t m2_u8g_height_minus_one;
extern uint8_t m2_u8g_fg_text_color;
extern uint8_t m2_u8g_bg_text_color;
extern uint8_t m2_u8g_current_text_color;


/* graphics handler for the u8glib */
/*

logic:
  1. Part: Focus 
  2. Part: Highlight
  Parts:
    b = Box
    f = Frame
    fs = Shadow Frame

uint8_t m2_gh_dogm_bf(m2_gfx_arg_p  arg);
uint8_t m2_gh_dogm_bfs(m2_gfx_arg_p  arg);
uint8_t m2_gh_dogm_fb(m2_gfx_arg_p  arg);
uint8_t m2_gh_dogm_fbs(m2_gfx_arg_p  arg);
uint8_t m2_gh_dogm_ffs(m2_gfx_arg_p  arg);
uint8_t m2_gh_dogxl160(m2_gfx_arg_p arg);
*/
/* graphics handler for the u8glib */
uint8_t m2_gh_u8g_fb(m2_gfx_arg_p arg);


/* utility procedures */

void m2_SetU8g(u8g_t *u8g);
void m2_u8g_draw_frame(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h);
void m2_u8g_draw_frame_shadow(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h);
void m2_u8g_draw_box(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h);
void m2_u8g_draw_icon(uint8_t x, uint8_t y, uint8_t font, uint8_t icon);
uint8_t m2_u8g_get_icon_height(uint8_t font, uint8_t icon);
uint8_t m2_u8g_get_icon_width(uint8_t font, uint8_t icon);
const u8g_fntpgm_uint8_t *m2_u8g_get_font(uint8_t font);
uint8_t m2_gh_u8g_base(m2_gfx_arg_p  arg);
 
#ifdef __cplusplus
}
#endif
#endif


