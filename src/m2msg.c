/*

  m2msg.c
  
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

const char *m2_message_text;
const char *m2_message_button;
m2_button_fnptr m2_message_fn;

void m2_fn_message_ok(m2_el_fnarg_p fnarg) 
{
  if ( m2_message_fn != (m2_button_fnptr)NULL )
    m2_message_fn(fnarg);
}

M2_LABELPTR(m2_message_el_label, NULL, &m2_message_text);
M2_BUTTONPTR(m2_message_el_button, NULL, &m2_message_button, m2_fn_message_ok);
M2_LIST(m2_message_list) = {&m2_message_el_label, &m2_message_el_button };
M2_VLIST(m2_message_el_vlist, NULL, m2_message_list);


void m2_MessageFn(const char *text, const char *button, m2_button_fnptr fn)
{
  m2_message_text = text;
  m2_message_button = button;
  m2_message_fn = fn;
  m2_SetRoot(&m2_message_el_vlist);
}


