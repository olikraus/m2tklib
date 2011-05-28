

#include "m2.h"


#define M2_IS_KEY_ASSIGNED 1

struct _m2_key_to_pin_entry
{
  uint8_t status;
  uint8_t pin;
};
typedef struct _m2_key_to_pin_entry m2_key_to_pin_entry_t;

m2_key_to_pin_entry_t m2_key_to_pin_array[M2_KEY_CNT];

static uint8_t m2_get_idx_by_key(uint8_t key) M2_NOINLINE;

static uint8_t m2_get_idx_by_key(uint8_t key)
{
  if ( key > 0 )
  {
    key--;
    if ( key >= M2_KEY_CNT )
    {
      key = M2_KEY_CNT-1;
    }
  }
  return key;
}


void m2_SetPin(uint8_t key, uint8_t pin)
{
  uint8_t idx = m2_get_idx_by_key(key);
  m2_key_to_pin_array[idx].status |= M2_IS_KEY_ASSIGNED;
  m2_key_to_pin_array[idx].pin = pin;
}

uint8_t m2_GetPin(uint8_t key)
{
  uint8_t idx = m2_get_idx_by_key(key);
  return m2_key_to_pin_array[idx].pin;
}

uint8_t m2_IsPinAssigned(uint8_t key)
{
  uint8_t idx = m2_get_idx_by_key(key);
  return m2_key_to_pin_array[key].status & M2_IS_KEY_ASSIGNED;
}

