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

#define M2_ROTARY_ENCODER_STATE_CHANGE_DELAY 1


static void m2_arduino_setup_key(uint8_t key) M2_NOINLINE;
static void m2_arduino_setup(void) M2_NOINLINE;
static uint8_t m2_arduino_check_key(uint8_t key) M2_NOINLINE;
static uint8_t m2_arduino_get_key(void) M2_NOINLINE;

static uint8_t m2_rot_enc_get_input(void) M2_NOINLINE;
static uint8_t m2_rot_enc_init_state(uint8_t in) M2_NOINLINE;
static uint8_t m2_rot_enc_next_state(uint8_t state, uint8_t in) M2_NOINLINE;
static uint8_t m2_get_rot_enc_get_key(void) M2_NOINLINE;
static uint8_t m2_get_rot_enc_get_key(void) M2_NOINLINE;

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
  uint8_t i;
  for( i = M2_KEY_LOOP_START; i <= M2_KEY_LOOP_END; i++ )
    m2_arduino_setup_key(i);

  /*
  m2_arduino_setup_key(M2_KEY_SELECT);
  m2_arduino_setup_key(M2_KEY_EXIT);
  m2_arduino_setup_key(M2_KEY_NEXT);
  m2_arduino_setup_key(M2_KEY_PREV);  
  m2_arduino_setup_key(M2_KEY_DATA_UP);  
  m2_arduino_setup_key(M2_KEY_DATA_DOWN);  
  m2_arduino_setup_key(M2_KEY_HOME);  
  m2_arduino_setup_key(M2_KEY_ROT_ENC_A);  
  m2_arduino_setup_key(M2_KEY_ROT_ENC_B);  
  */
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
  uint8_t i;
  if ( m2_arduino_check_key(M2_KEY_SELECT) ) return M2_KEY_SELECT;
  if ( m2_arduino_check_key(M2_KEY_SELECT2) ) return M2_KEY_SELECT;	/* map to first select */
  if ( m2_arduino_check_key(M2_KEY_EXIT) ) return M2_KEY_EXIT;
  if ( m2_arduino_check_key(M2_KEY_NEXT) ) return M2_KEY_NEXT;
  if ( m2_arduino_check_key(M2_KEY_PREV) ) return M2_KEY_PREV;
  if ( m2_arduino_check_key(M2_KEY_DATA_UP) ) return M2_KEY_DATA_UP;
  if ( m2_arduino_check_key(M2_KEY_DATA_DOWN) ) return M2_KEY_DATA_DOWN;
  if ( m2_arduino_check_key(M2_KEY_HOME) ) return M2_KEY_HOME;
  if ( m2_arduino_check_key(M2_KEY_HOME2) ) return M2_KEY_HOME2;
  for( i = M2_KEY_Q1; i <= M2_KEY_LOOP_END; i++ )
    if ( m2_arduino_check_key(i) ) return i;
  
  return M2_KEY_NONE;
}

/*=================================================================*/
/* normal button handler, one button per pin */

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

/*=================================================================*/
/* rotary encode with normal button handler (one button per pin) */

static uint8_t m2_rec_enc_next_state_array[] = { 
#ifdef M2_WITHOUT_INTERMEDIATE_UNKNOWN_STATE
0x090, // 10 01 00 00
0x021, // 00 10 00 01
0x048, // 01 00 10 00
0x006, // 00 00 01 10
  
0x091, // 10 01 00 01
0x025, // 00 10 01 01
0x058, // 01 01 10 00
0x046, // 01 00 01 10

0x092, // 10 01 00 10
0x029, // 00 10 10 01
0x068, // 01 10 10 00
0x086, // 10 00 01 10

#else
  
0x0a0, // 10 10 00 00
0x02a, // 00 10 00 10
0x088, // 10 00 10 00
0x00a, // 00 00 10 10
  
0x099, // 10 01 10 01
0x0a5, // 10 10 01 01
0x05a, // 01 01 10 10
0x066, // 01 10 01 10

0x092, // 10 01 00 10
0x029, // 00 10 10 01
0x068, // 01 10 10 00
0x086, // 10 00 01 10
#endif
};

static uint8_t m2_rot_enc_get_input(void)
{
  uint8_t in = 0;
  
  if ( m2_arduino_check_key(M2_KEY_ROT_ENC_A) != 0 )
    in |= 1;
  if ( m2_arduino_check_key(M2_KEY_ROT_ENC_B) != 0 )
    in |= 2;
  return in;
}

/* based on the input, get the initial state */
static uint8_t m2_rot_enc_init_state(uint8_t in)
{
  return (in & 3) | 8;
}

/* based on the input, calculate the next state */
static uint8_t m2_rot_enc_next_state(uint8_t state, uint8_t in)
{
  uint8_t new_state;
  if ( state > 12 )
    state = m2_rot_enc_init_state(in);
  new_state = m2_rec_enc_next_state_array[state] >> ((in)*2);
  new_state &= 3;
  new_state <<= 2;
  new_state |= in;
  return new_state;
}

uint8_t m2_rot_enc_state;
uint8_t m2_rot_enc_debounce_cnt;
int8_t m2_rot_enc_div_cnt;
#define ROT_ENC_DEBOUNCE_VAL 3
#define ROT_ENC_EVENT_DIVISION 3

static uint8_t m2_get_rot_enc_get_key(void)
{
  uint8_t new_m2_rot_enc_state;
  
  /* calculate the next state */
  new_m2_rot_enc_state = m2_rot_enc_next_state(m2_rot_enc_state, m2_rot_enc_get_input());

  if ( new_m2_rot_enc_state != m2_rot_enc_state )
  {
    /* if state has changed, then reset the debounce counter */
    m2_rot_enc_debounce_cnt = 0;
    /* ... and store new state */
    m2_rot_enc_state = new_m2_rot_enc_state;
    
    delay(M2_ROTARY_ENCODER_STATE_CHANGE_DELAY);
  }
  else
  {
    /* only if the direction is known */
    if ( new_m2_rot_enc_state < 8 ) 
    {
      /* increment the debounce counter */
      if ( m2_rot_enc_debounce_cnt < ROT_ENC_DEBOUNCE_VAL )
      { 
        m2_rot_enc_debounce_cnt++;
        if ( m2_rot_enc_debounce_cnt >= ROT_ENC_DEBOUNCE_VAL )
        {
	  if ( new_m2_rot_enc_state & 4 )
	  {
	    if ( m2_rot_enc_div_cnt > ROT_ENC_EVENT_DIVISION )
	    {
	      m2_rot_enc_div_cnt = 0;
	      return M2_KEY_EVENT(M2_KEY_NEXT);		/* disable debounce by M2tk algoritm */	      
	    }
	    else
	    {
	      m2_rot_enc_div_cnt++;
	    }
	  }
	  else
	  {
	    if ( m2_rot_enc_div_cnt < -ROT_ENC_EVENT_DIVISION )
	    {
	      m2_rot_enc_div_cnt = 0;
	      return M2_KEY_EVENT(M2_KEY_PREV);		/* disable debounce by M2tk algoritm */
	    }
	    else
	    {
	      m2_rot_enc_div_cnt--;
	    }
	  }
        }
      }
    }
  }
  
  return M2_KEY_NONE;
}

uint8_t m2_es_arduino_rotary_encoder(m2_p ep, uint8_t msg)
{
  uint8_t key, i;
  switch(msg)
  {
    case M2_ES_MSG_GET_KEY:
      for( i = 0; i < 4; i++ )
      {
	key = m2_get_rot_enc_get_key();
	if ( key != M2_KEY_NONE )
	  return key;
      }
      return m2_arduino_get_key();
    case M2_ES_MSG_INIT:
      m2_arduino_setup();
      m2_rot_enc_init_state(m2_rot_enc_get_input());
      return 0;
  }
  return 0;
}

