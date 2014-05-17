#include "m2.h"


/*=======================================================================*/
/* is selectable (can get focus?) */

/* 
  =0: not selectable, can not get focus 
  !=0: can get focus
*/
static uint8_t m2_is_selectable(m2_t *m2, const m2el_t *el_ptr)
{
  return m2el_GetOpt(m2, el_ptr, 'o');
}


/*=======================================================================*/
/* find first */

/* callback procedure to find the first element that can have the focus */
static uint8_t m2_dfs_focus_any_first(m2_t *m2, uint8_t dfs_msg, uint16_t el_seq_nr, const m2el_t *el_ptr)
{
  if ( dfs_msg == M2_DFS_MSG_PRE )
  {
      if ( m2_is_selectable(m2, el_ptr) > 0 )
      {
	/* found first element, which can have focus */
	return 0;
      }
  }
  return 1;
}

/*
  Updateds m2->nav_focus_el_nr
  m2->nav_focus_el_nr can be 0 if no element can get focus.
*/
uint8_t m2_SetFocusFirst(m2_t *m2)
{
  m2->nav_focus_el_nr = 0;
  /* execute DFS */
  if ( m2_DFS(m2, m2_dfs_focus_any_first) == 0 )
  {
    /* if the DFS has been stopped, this means a element was found, which can have focus */
    m2->nav_focus_el_nr = m2->dfs_el_nr;
    return 1;
  }
  return 0;
}


/*=======================================================================*/
/* find next */

/*
  find next focus
*/
uint8_t m2_dfs_focus_next(m2_t *m2, uint8_t dfs_msg, uint16_t el_seq_nr, const m2el_t *el_ptr)
{
  if ( dfs_msg == M2_DFS_MSG_PRE )
  {
    
    if ( m2->nav_state == 0 )
    {
      /* check for the focus element, if match, then increase state */
      if ( el_seq_nr == m2->nav_focus_el_nr )
      {
	m2->nav_state++;
      }
    }
    else
    {
      /* searching for the next element which could get focus */
      if ( m2_is_selectable(m2, el_ptr) > 0 )
      {
	/* found next element, which can have focus */
	return 0;
      }
    }
  }
  return 1;
}

/*
  return 1 if m2->nav_focus_el_nr has been changed.
  return 0 if (A) no element can get focs or (B) there is no next element, which can get focus
  it is possible to do this:
  r = m2_SetFocusNext(m2)
  if ( r == 0 )
    r = m2_SetFocusFirst(m2)

  if r is still 0 then no element can have focus
*/
uint8_t m2_SetFocusNext(m2_t *m2)
{
  m2->nav_state = 0;
  /* execute DFS */
  if ( m2_DFS(m2, m2_dfs_focus_next) == 0 )
  {
    /* if the DFS has been stopped, this means a element was found, which can have focus */
    /* still the value of m2->dfs_el_nr can be 0, if there is no focus at all or in the focus group */
    
    if ( m2->nav_state == 1 )
    {
      m2->nav_focus_el_nr = m2->dfs_el_nr;
      return 1;
    }
    else
    {
      /* m2->nav_state == 0: no element found, that can have focus */
      m2->nav_focus_el_nr = 0;
      return 0;
    }
  }
  else
  {
    /* not a single element can have focus: remove focus (probably not required) */
      m2->nav_focus_el_nr = 0;
    return 0;
  }
}

/*=======================================================================*/
/* find prev */

/*
  find prev focus
*/
uint8_t m2_dfs_focus_prev(m2_t *m2, uint8_t dfs_msg, uint16_t el_seq_nr, const m2el_t *el_ptr)
{
  
  if ( dfs_msg == M2_DFS_MSG_PRE )
  {
  
    /* check for the focus element, if match, then increase state */
    if ( el_seq_nr == m2->nav_focus_el_nr )
    {
      /* stop: two cases (A) m2->tmp_el_nr == 0 / prev not possible (B) m2->tmp_el_nr != 0 */
      return 0;
    }
    else
    {
      if ( m2_is_selectable(m2, el_ptr) > 0 )
      {
	m2->tmp_el_nr = el_seq_nr;
      }
    }
  }
  return 1;
}

/*
  return 1 if m2->nav_focus_el_nr has been changed.
  return 0 if (A) no element can get focs or (B) there is no next element, which can get focus
  it is possible to do this:
  r = m2_SetFocusPrev(m2)
  if ( r == 0 )
    r = m2_SetFocusLast(m2)

  if r is still 0 then no element can have focus
*/
uint8_t m2_SetFocusPrev(m2_t *m2)
{
  m2->nav_state = 0;
  m2->tmp_el_nr = 0;
  
  /* execute DFS */
  if ( m2_DFS(m2, m2_dfs_focus_prev) == 0 )
  {
    if ( m2->tmp_el_nr == 0 )
    {
      m2->nav_focus_el_nr = 0;
      return 0;      
    }
    else
    {
      m2->nav_focus_el_nr = m2->tmp_el_nr;
      return 1;      
    }
  }
  else
  {
    /* not a single element can have focus: remove focus (probably not required) */
      m2->nav_focus_el_nr = 0;
    return 0;
  }
}

/*=======================================================================*/
/* find last */

/* callback procedure to find the first element that can have the focus */
static uint8_t m2_dfs_focus_last(m2_t *m2, uint8_t dfs_msg, uint16_t el_seq_nr, const m2el_t *el_ptr)
{
  if ( dfs_msg == M2_DFS_MSG_POST )
  {
      if ( m2_is_selectable(m2, el_ptr) > 0 )
      {
	m2->nav_focus_el_nr = el_seq_nr;
      }
  }
  return 1;
}

/*
  Updateds m2->nav_focus_el_nr
  m2->nav_focus_el_nr can be 0 if no element can get focus.
*/
uint8_t m2_SetFocusLast(m2_t *m2)
{
  m2->nav_focus_el_nr = 0;
  /* execute DFS */
  m2_DFS(m2, m2_dfs_focus_last);
  if ( m2->nav_focus_el_nr != 0 )
    return 1;
  return 0;
}
