
#include "m2.h"

void m2_Init(m2_t *m2)
{
  m2_SetRootElement(m2, NULL);
}

void m2_SetRootElement(m2_t *m2, const void *root_el_ptr)
{
  if ( root_el_ptr == NULL )
    root_el_ptr = &m2el_null;
  m2->root_el_ptr = root_el_ptr;
  m2_SetFocusFirst(m2);  
}