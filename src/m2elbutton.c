/*
  
  m2elbutton.c

  a button with a callback procedure
  based on the string element
  
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

#include <stddef.h>
#include "m2.h"

m2_button_fnptr m2_el_button_get_callback(m2_el_fnarg_p fn_arg)
{
  return (m2_button_fnptr)m2_rom_get_ram_ptr(fn_arg->element, offsetof(m2_el_button_t, button_callback));
}

M2_EL_FN_DEF(m2_el_button_fn)
{
  switch(fn_arg->msg)
  {
    case M2_EL_MSG_GET_OPT:
	if ( fn_arg->arg == 't' )
	{
	  *(uint8_t *)(fn_arg->data) = m2_el_fmfmt_opt_get_val_any_default(fn_arg, 't', 1);		/* touch screen allowed by default*/
	  return 1;
	}
	/* else... break out of the switch and let the base class do the rest of the work */
	break;
    case M2_EL_MSG_SELECT:
    {
      m2_button_fnptr fn;
      fn = m2_el_button_get_callback(fn_arg);
      if ( fn != NULL )
	fn(fn_arg);
      return 1;
    }
  }
  return m2_el_str_fn(fn_arg);
}

