/*

  m2ghdogm.h
  
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

#ifndef _M2GHDOGM_H
#define _M2GHDOGM_H

#include "m2.h"
#include "dogm128.h"

#ifdef __cplusplus
extern "C" {
#endif


/* graphics handler for the dogm128 lib */
uint8_t m2_gh_dogm_bf(m2_gfx_arg_p  arg);
uint8_t m2_gh_dogm_bfs(m2_gfx_arg_p  arg);
uint8_t m2_gh_dogm_fb(m2_gfx_arg_p  arg);
uint8_t m2_gh_dogm_fbs(m2_gfx_arg_p  arg);
uint8_t m2_gh_dogm_ffs(m2_gfx_arg_p  arg);
uint8_t m2_gh_dogxl160(m2_gfx_arg_p arg);

  
/* utility procedures */

void m2_dogm_draw_frame(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h);
void m2_dogm_draw_frame_shadow(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h);
void m2_dogm_draw_xorbox(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h);
void m2_dogm_draw_icon(uint8_t x, uint8_t y, uint8_t font, uint8_t icon);
uint8_t m2_dogm_get_icon_height(uint8_t font, uint8_t icon);
uint8_t m2_dogm_get_icon_width(uint8_t font, uint8_t icon);

uint8_t m2_gh_dogm_base(m2_gfx_arg_p  arg);
DOG_PGM_P m2_dogm_get_font(uint8_t font);
  
#ifdef __cplusplus
}
#endif
#endif
