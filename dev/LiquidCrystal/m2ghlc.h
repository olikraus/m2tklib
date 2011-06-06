/*

  m2ghlc.h
  
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

#ifndef _M2GHLC_H
#define _M2GHLC_H

#include <LiquidCrystal.h>

void m2_SetLiquidCrystal(LiquidCrystal *lc_ptr, uint8_t cols, uint8_t rows);

#ifdef __cplusplus
extern "C" {
#endif

/* specific 16x4 handler with fixed pin setup */
uint8_t m2_gh_lc_16x4(m2_gfx_arg_p  arg);

/* more flexible handler, but requires a call to m2_SetLiquidCrystal() */
uint8_t m2_gh_lc(m2_gfx_arg_p  arg);
  
#ifdef __cplusplus
}
#endif
#endif
