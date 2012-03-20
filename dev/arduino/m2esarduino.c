/*

  m2esarduino.c
  
  event source handler for arduino environment
  
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

#if ARDUINO < 100 
#include "wiring.h"
//#include <WProgram.h> 
#else 
#include <Arduino.h> 
#endif

#include "m2.h"

static void m2_arduino_setup_key(uint8_t key) M2_NOINLINE;
static void m2_arduino_setup(void) M2_NOINLINE;
static uint8_t m2_arduino_check_key(uint8_t key) M2_NOINLINE;
static uint8_t m2_arduino_get_key(void) M2_NOINLINE;

static void m2_arduino_setup_key(uint8_t key)
{
  uint8_t pin;
  if ( m2_IsPinAssigned(key) != 0 )
  {
    pin = m2_GetPin(key);
    pinMode(pin, INPUT);           /* set pin to input */
    digitalWrite(pin, HIGH);       /* turn on pullup resistors */
  }
}

static void m2_arduino_setup(void) 
{
  m2_arduino_setup_key(M2_KEY_SELECT);
  m2_arduino_setup_key(M2_KEY_EXIT);
  m2_arduino_setup_key(M2_KEY_NEXT);
  m2_arduino_setup_key(M2_KEY_PREV);  
  m2_arduino_setup_key(M2_KEY_DATA_UP);  
  m2_arduino_setup_key(M2_KEY_DATA_DOWN);  
}

static uint8_t m2_arduino_check_key(uint8_t key)
{
  uint8_t pin;
  if ( m2_IsPinAssigned(key) != 0 )
  {
    pin = m2_GetPin(key);
    if ( digitalRead(pin) == LOW )
      return 1;
  }
  return 0;
}

static uint8_t m2_arduino_get_key(void)
{
  if ( m2_arduino_check_key(M2_KEY_SELECT) ) return M2_KEY_SELECT;
  if ( m2_arduino_check_key(M2_KEY_EXIT) ) return M2_KEY_EXIT;
  if ( m2_arduino_check_key(M2_KEY_NEXT) ) return M2_KEY_NEXT;
  if ( m2_arduino_check_key(M2_KEY_PREV) ) return M2_KEY_PREV;
  if ( m2_arduino_check_key(M2_KEY_DATA_UP) ) return M2_KEY_DATA_UP;
  if ( m2_arduino_check_key(M2_KEY_DATA_DOWN) ) return M2_KEY_DATA_DOWN;
  return M2_KEY_NONE;
}

uint8_t m2_es_arduino(m2_p ep, uint8_t msg)
{
  
  switch(msg)
  {
    case M2_ES_MSG_GET_KEY:
      return m2_arduino_get_key();
    case M2_ES_MSG_INIT:
      m2_arduino_setup();
      return 0;
  }
  return 0;
}

