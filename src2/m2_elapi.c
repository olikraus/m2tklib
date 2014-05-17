
#include "m2.h"




uint16_t m2el_GetChildCnt(m2_t *m2, const m2el_t *el)
{
  /* if this msg is not supported, 0 will be returned */
  return el->fn(m2, el, M2EL_MSG_GET_CHILD_CNT, NULL);
}

const m2el_t *m2el_GetChildPtr(m2_t *m2, const m2el_t *el, uint16_t pos)
{
  m2el_child_ptr_t arg;
  arg.pos = pos;
  if ( el->fn(m2, el, M2EL_MSG_GET_CHILD_PTR, &arg) == 0 )
    return NULL;
  return arg.el_ptr;
}

uint8_t m2el_GetOpt(m2_t *m2, const m2el_t *el, char opt)
{
  return (uint8_t)el->fn(m2, el, M2EL_MSG_GET_OPT, &opt);
}
