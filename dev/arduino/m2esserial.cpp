/*

  m2esserial.cpp
  
  event source handler for serial interface of arduino environment
  
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

#if ARDUINO < 100 
#include "wiring.h"
#include <WProgram.h> 
#else 
#include <Arduino.h> 
#endif

#include "m2.h"

extern "C" uint8_t m2_serial_char_to_key(uint8_t c)
{
  if ( c == 's' ) return M2_KEY_SELECT;
  if ( c == 'x' ) return M2_KEY_EXIT;
  if ( c == 'n' ) return M2_KEY_NEXT;
  if ( c == 'p' ) return M2_KEY_PREV;
  if ( c == 'u' ) return M2_KEY_DATA_UP;
  if ( c == 'd' ) return M2_KEY_DATA_DOWN;
  if ( c == 'h' ) return M2_KEY_HOME;
  return M2_KEY_NONE;
}

extern "C" void m2_serial_show_help(void)
{
  Serial.println("");
  Serial.println("'s' Select submenu");
  Serial.println("'x' eXit current menu");
  Serial.println("'n' Next element");
  Serial.println("'p' Previous element");
  Serial.println("'u' data increment (Up)");
  Serial.println("'d' data decrement (Down)");
  Serial.println("'h' Home menu");
  Serial.println("Key binding depends on the current event handler.");  
}

uint8_t m2_es_arduino_serial_is_init = 0;

extern "C" uint8_t m2_es_arduino_serial(m2_p ep, uint8_t msg)
{
  uint8_t c, r;
  
  switch(msg)
  {
    case M2_ES_MSG_GET_KEY:
      if ( m2_es_arduino_serial_is_init == 0 )
      {
	Serial.begin(9600);        // init serial
	Serial.println("m2tklib serial (press 'h' for help)");
	Serial.println("");
	m2_es_arduino_serial_is_init = 1;
      }
      r = M2_KEY_NONE;
      if ( Serial.available() )
      {
	c = Serial.read();
	if ( c == 'h' )
	{
	  m2_serial_show_help();
	}
	else
	{
	  r = m2_serial_char_to_key(c);
	  if ( r != M2_KEY_NONE )
	  {
	    Serial.println((char)c);
	    r = M2_KEY_EVENT(r);
	  }
	}
      }
      return r;
    case M2_ES_MSG_INIT:
      return 0;
  }
  return 0;
}
