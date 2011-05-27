/*

  m2key.c

  key debounce and queue

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

#define M2_DEBOUNCE_CNT 3

uint8_t m2_GetKeyFromQueue(m2_p m2)
{
  uint8_t key;
  if ( m2->key_queue_len == 0 )
    return M2_KEY_NONE;
  key = m2->key_queue_array[m2->key_queue_pos];
  
  m2->key_queue_pos++;
  m2->key_queue_pos &= (M2_KEY_QUEUE_LEN-1);
  m2->key_queue_len--;
  
  return key;
}

void m2_PutKeyIntoQueue(m2_p m2, uint8_t key_code)
{
  uint8_t pos;
  
  if ( key_code == M2_KEY_NONE )
    return;

  pos = m2->key_queue_pos;
  pos += m2->key_queue_len;
  pos &= (M2_KEY_QUEUE_LEN-1);
  m2->key_queue_array[pos] = key_code;
  
  if ( m2->key_queue_len >= M2_KEY_QUEUE_LEN ) 
    m2->key_queue_pos++;
  else
    m2->key_queue_len++;
}

/*
  debounce key and put key into queue
*/
void m2_SetDetectedKey(m2_p m2, uint8_t key_code)
{
  if ( M2_IS_KEY_EVENT(key_code) )
  {
    key_code &= ~M2_KEY_EVENT_MASK;
    m2_PutKeyIntoQueue(m2, key_code);
    return;
  }
  
  if ( m2->pressed_key_code == M2_KEY_NONE )
  {
    if ( m2->detected_key_code != key_code )
    {
      m2->detected_key_code = key_code;
      m2->detected_key_timer = M2_DEBOUNCE_CNT;
    }
    else
    {
      if ( m2->detected_key_timer == 0 )
      {
	m2->pressed_key_code = m2->detected_key_code;
      }
      else
      {
	m2->detected_key_timer--;
      }
    }
  }
  else
  {
    if ( m2->pressed_key_code != key_code )
    {
      m2_PutKeyIntoQueue(m2, m2->pressed_key_code);
      m2->pressed_key_code = key_code;
      if ( key_code != M2_KEY_NONE )
      {
	m2->detected_key_code = key_code;
	m2->detected_key_timer = M2_DEBOUNCE_CNT;
      }
    }
  }
}


