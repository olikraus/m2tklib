/*
  
  m2elspace.c
  
  element which occupies some space
  
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

M2_EL_FN_DEF(m2_el_space_fn)
{
  switch(fn_arg->msg)
  {
    case M2_EL_MSG_IS_READ_ONLY:
      return 1;
    case M2_EL_MSG_GET_HEIGHT:
      return m2_el_fnfmt_get_hH(fn_arg);
    case M2_EL_MSG_GET_WIDTH:
      return m2_el_fnfmt_get_wW(fn_arg);
    case M2_EL_MSG_SHOW:
      /* nothing to show */
      return 1;    
  }
  return m2_el_fnfmt_fn(fn_arg);
}

