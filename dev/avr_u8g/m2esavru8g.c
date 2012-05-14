/*

  m2esavru8g.c
  
  event source handler for avr controller with u8glib
  
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


#include "u8g.h"
#include "m2.h"

static void m2_avr_u8g_setup_key(uint8_t key) M2_NOINLINE;
static void m2_avr_u8g_setup(void) M2_NOINLINE;
static uint8_t m2_avr_u8g_check_key(uint8_t key) M2_NOINLINE;
static uint8_t m2_avr_u8g_get_key(void) M2_NOINLINE;

static void m2_avr_u8g_setup_key(uint8_t key)
{
  uint8_t pin;
  if ( m2_IsPinAssigned(key) != 0 )
  {
    /* here, we assume that the user has assigned the internal u8g pin number: PN(port,bit) */
    pin = m2_GetPin(key);
    u8g_SetPinInput(pin);		/* set pin to input */
    u8g_SetPinLevel(pin, 1);	/* turn on pullup resistors */
  }
}

static void m2_avr_u8g_setup(void) 
{
  m2_avr_u8g_setup_key(M2_KEY_SELECT);
  m2_avr_u8g_setup_key(M2_KEY_EXIT);
  m2_avr_u8g_setup_key(M2_KEY_NEXT);
  m2_avr_u8g_setup_key(M2_KEY_PREV);  
  m2_avr_u8g_setup_key(M2_KEY_DATA_UP);  
  m2_avr_u8g_setup_key(M2_KEY_DATA_DOWN);  
}

static uint8_t m2_avr_u8g_check_key(uint8_t key)
{
  uint8_t pin;
  if ( m2_IsPinAssigned(key) != 0 )
  {
    pin = m2_GetPin(key);
    if ( u8g_GetPinLevel(pin) == 0 )
      return 1;
  }
  return 0;
}

static uint8_t m2_avr_u8g_get_key(void)
{
  if ( m2_avr_u8g_check_key(M2_KEY_SELECT) ) return M2_KEY_SELECT;
  if ( m2_avr_u8g_check_key(M2_KEY_EXIT) ) return M2_KEY_EXIT;
  if ( m2_avr_u8g_check_key(M2_KEY_NEXT) ) return M2_KEY_NEXT;
  if ( m2_avr_u8g_check_key(M2_KEY_PREV) ) return M2_KEY_PREV;
  if ( m2_avr_u8g_check_key(M2_KEY_DATA_UP) ) return M2_KEY_DATA_UP;
  if ( m2_avr_u8g_check_key(M2_KEY_DATA_DOWN) ) return M2_KEY_DATA_DOWN;
  return M2_KEY_NONE;
}

uint8_t m2_es_avr_u8g(m2_p ep, uint8_t msg)
{
  switch(msg)
  {
    case M2_ES_MSG_GET_KEY:
      return m2_avr_u8g_get_key();
    case M2_ES_MSG_INIT:
      m2_avr_u8g_setup();
      return 0;
  }
  return 0;
}

