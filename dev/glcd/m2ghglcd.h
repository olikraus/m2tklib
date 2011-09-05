/*

  m2ghglcd.h
  
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

#ifndef _M2GHGLCD_H
#define _M2GHGLCD_H



#ifdef __cplusplus
extern "C" {
#endif

extern void *m2_Arial14;
extern void *m2_System5x7;
extern void *m2_fixednums7x15;
extern void *m2_fixednums15x31;
  
extern uint8_t m2_is_glcd_init;
 
uint8_t m2_gh_glcd_y(uint8_t y);
uint8_t m2_gh_glcd_get_font_height(m2_gfx_arg_p  arg);
uint8_t m2_gh_glcd_get_font_corrcetion(m2_gfx_arg_p  arg);
void m2_gh_glcd_set_font(uint8_t font);
void m2_gh_glcd_draw_frame(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h);
void m2_gh_glcd_draw_frame_shadow(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h);
void m2_gh_glcd_draw_xorbox(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h);
void m2_gh_glcd_draw_icon(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, uint8_t icon);
void m2_gh_glcd_draw_big_icon(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, uint8_t icon);

uint8_t m2_gh_glcd_get_user_font_height(m2_gfx_arg_p  arg);
uint8_t m2_gh_glcd_get_user_font_corrcetion(m2_gfx_arg_p  arg);
void m2_gh_glcd_set_user_font(uint8_t font);
uint8_t m2_gh_glcd_base(m2_gfx_arg_p  arg);

  
/*  
  a simple glcd graphics handler
  fonts are fixed and predefined
    f0: system5x7
    f1: arial14
    f4: system5x7 with frame
    f5: arial14 with frame
*/
uint8_t m2_gh_glcd_bf(m2_gfx_arg_p arg);
uint8_t m2_gh_glcd_ffs(m2_gfx_arg_p arg);
  
/*  
  a simple glcd graphics handler
  fonts can be defined with m2_SetFont()
*/
uint8_t m2_gh_glcd_ubf(m2_gfx_arg_p arg);
uint8_t m2_gh_glcd_uffs(m2_gfx_arg_p arg);
  
#ifdef __cplusplus
}
#endif
#endif
