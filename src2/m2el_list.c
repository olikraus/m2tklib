

/*
  Inheritance:
    m2el_base
    m2el_opt
    m2el_list
*/

#include "m2.h"

static uint16_t m2el_list_get_cnt(m2el_list_t *el)
{
  return el->cnt;
}

static const m2el_t *m2el_list_get_el_ptr(m2el_list_t *el, uint16_t pos)
{
  return el->list[pos];
}

uint16_t m2el_list(m2_t *m2, const m2el_t *el, uint8_t el_msg, void *arg)
{
  switch(el_msg)
  {
    case M2EL_MSG_GET_CHILD_CNT:
      return m2el_list_get_cnt((m2el_list_t *)el);

    case M2EL_MSG_GET_CHILD_PTR:
      ((m2el_child_ptr_t *)arg)->el_ptr = m2el_list_get_el_ptr( (m2el_list_t *)el, ((m2el_child_ptr_t *)arg)->pos);
      return 1;
  }
  return m2el_opt(m2, el, el_msg, arg);
}
