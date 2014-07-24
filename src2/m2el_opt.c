
#include "m2.h"



uint16_t  m2el_opt(m2_t *m2, const m2el_t *el, uint8_t el_msg, void *arg)
{
  if ( el_msg == M2EL_MSG_GET_OPT )
  {
    return m2_opt_get_val_zero_default(((m2el_opt_t *)el)->opt, *(char *)arg);
  }
  return m2el_base(m2, el, el_msg, arg);
}

