/*

  m2key.c

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

void m2_SetDetectedKey(m2_p m2, uint8_t key_code)
{
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


