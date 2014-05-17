


#include "m2.h"
#include <stdio.h>

/*========================================================================*/

uint8_t m2_dfs_debug_fn(m2_t *m2, uint8_t dfs_msg, uint16_t el_seq_nr, const m2el_t *el_ptr)
{
  uint8_t i;
  switch(dfs_msg)
  {
    case M2_DFS_MSG_PRE:
      printf("%c ", m2->nav_focus_el_nr == el_seq_nr ? '*' : ' ');
      for( i = 0; i < m2->dfs_depth; i++)
	printf("  ");
      printf("PRE:  %d %p 'o'=%d 't'=%d '#'=%d\n", el_seq_nr, el_ptr, m2el_GetOpt(m2, el_ptr, 'o'), m2el_GetOpt(m2, el_ptr, 't'), m2el_GetOpt(m2, el_ptr, '#'));
      break;
    
    case M2_DFS_MSG_POST:
      printf("%c ", m2->nav_focus_el_nr == el_seq_nr ? '*' : ' ');
      for( i = 0; i < m2->dfs_depth; i++)
	printf("  ");
      printf("POST: %d %p\n", el_seq_nr, el_ptr);
      break;
  }
  return 1;
}

/*========================================================================*/

void m2tk_test_assert(const char *fn, int line, const char *str_condition, int condition)
{
  if ( condition == 0 )
  {
    printf("test case %s (line %d) '%s' failed\n", fn, line, str_condition);
  }
}

#define T(condition) m2tk_test_assert(__func__, __LINE__, #condition, (condition))

/*========================================================================*/


void test000(void)
{
  m2_t m2;
  m2_Init(&m2);
  T( m2_GetFocusElement(&m2) == 0 );
  
  /* no element, first and next must return 0 */
  T( m2_SetFocusFirst(&m2) == 0);
  T( m2_SetFocusNext(&m2) == 0);
  
  m2_SetRootElement(&m2, NULL);
  T( m2_SetFocusFirst(&m2) == 0);
  T( m2_SetFocusNext(&m2) == 0);
  T( m2_SetFocusPrev(&m2) == 0);
  T( m2_SetFocusLast(&m2) == 0);
  
  m2_SetRootElement(&m2, &m2el_null);
  T( m2_SetFocusFirst(&m2) == 0);
  T( m2_SetFocusNext(&m2) == 0);
  T( m2_SetFocusPrev(&m2) == 0);
  T( m2_SetFocusLast(&m2) == 0);

  m2_SetRootElement(&m2, &m2el_null);
  T( m2_SetFocusPrev(&m2) == 0);
}

/*========================================================================*/

M2_TESTEL(t001_01, "#1");

void test001(void)
{
  m2_t m2;
  m2_Init(&m2);
  T( m2_GetFocusElement(&m2) == 0 );
  m2_SetRootElement(&m2, &t001_01);
  T( m2_GetFocusElement(&m2) == 0 );
  
  /* the singe element is not selectable, first and next must return 0 */
  T( m2_SetFocusFirst(&m2) == 0);
  T( m2_GetFocusElement(&m2) == 0 );
  T( m2_SetFocusNext(&m2) == 0);
  T( m2_GetFocusElement(&m2) == 0 );
  T( m2_SetFocusPrev(&m2) == 0);
  T( m2_GetFocusElement(&m2) == 0 );
  T( m2_SetFocusLast(&m2) == 0);
  T( m2_GetFocusElement(&m2) == 0 );
  
  m2_SetRootElement(&m2, &t001_01);
  T( m2_SetFocusPrev(&m2) == 0);
  T( m2_GetFocusElement(&m2) == 0 );
}


/*========================================================================*/

M2_TESTEL(t002_01, "o1#1");

void test002(void)
{
  m2_t m2;
  m2_Init(&m2);
  T( m2_GetFocusElement(&m2) == 0 );
  m2_SetRootElement(&m2, &t002_01);
  T( m2_GetFocusElement(&m2) == 1 );
  
  /* the singe element is selectable, first will retorn 1 and next must return 0 */
  T( m2_SetFocusFirst(&m2) == 1);
  T( m2_SetFocusNext(&m2) == 0);

  m2_SetRootElement(&m2, &t002_01);
  
  /* the singe element is selectable, first will retorn 1 and next must return 0 */
  T( m2_SetFocusFirst(&m2) == 1);
  T( m2_SetFocusPrev(&m2) == 0);
  T( m2_GetFocusElement(&m2) == 0 );
}

/*========================================================================*/

M2_TESTEL(t003_01, "#1");
M2_TESTEL(t003_02, "o1#2");
M2_TESTEL(t003_03, "o1#3");
M2_LIST(t003_tlist) = { &t003_01, &t003_02, &t003_03 };
M2_SIMPLELIST(t003_el, "", t003_tlist);

void test003(void)
{
  m2_t m2;
  m2_Init(&m2);
  T( m2_GetFocusElement(&m2) == 0 );
  m2_SetRootElement(&m2, &t003_el);
  T( m2_GetFocusElement(&m2) == 3 );
  
  /* the singe element is not selectable, first will retorn 1 and next must return 0 */
  T( m2_SetFocusFirst(&m2) == 1);
  T( m2_SetFocusNext(&m2) == 1);
  T( m2_SetFocusNext(&m2) == 0);
  T( m2_GetFocusElement(&m2) == 0 );

  m2_SetRootElement(&m2, &t003_el);
  T( m2_SetFocusFirst(&m2) == 1);
  T( m2_SetFocusNext(&m2) == 1);
  T( m2_SetFocusPrev(&m2) == 1);
  m2_DFS(&m2, m2_dfs_debug_fn);
  T( m2_SetFocusPrev(&m2) == 0);
  T( m2_GetFocusElement(&m2) == 0 );
  
  T( m2_SetFocusLast(&m2) == 1);
  T( m2_SetFocusNext(&m2) == 0);
  T( m2_GetFocusElement(&m2) == 0 );
}

/*========================================================================*/


M2_TESTEL(t004_t1, "#1");
M2_TESTEL(t004_t2, "o1#2");
M2_TESTEL(t004_t3, "o1#3");
M2_LIST(t004_slist) = { &t004_t1, &t004_t2, &t004_t3 };
M2_SIMPLELIST(t004_slist_el, "o1", t004_slist);

M2_TESTEL(t004_t4, "#4");
M2_TESTEL(t004_t5, "o1#5");
M2_TESTEL(t004_t6, "o1#6");
M2_LIST(t004_tlist) = { &t004_t4, &t004_t5, &t004_slist_el, &t004_t6 };
M2_SIMPLELIST(t004_tlist_el, "", t004_tlist);


void test004(void)
{
  m2_t m2;
  m2_Init(&m2);
  m2_SetRootElement(&m2, &t004_tlist_el);
  
  /* the singe element is not selectable, first will retorn 1 and next must return 0 */
  T( m2_SetFocusFirst(&m2) == 1);
  T( m2_SetFocusNext(&m2) == 1);
  T( m2_SetFocusNext(&m2) == 1);
  T( m2_SetFocusNext(&m2) == 1);
  T( m2_SetFocusNext(&m2) == 1);
  //m2_DFS(&m2, m2_dfs_debug_fn);
  T( m2_SetFocusNext(&m2) == 0);
  T( m2_SetFocusLast(&m2) == 1);
  T( m2_SetFocusNext(&m2) == 0);
}


int main(void)
{
  test001();
  test002();
  test003();
  test004();
  
  
  
  return 0;
}