

#include "m2.h"

/*
  return values of the callback function
    M2_DFS_MSG_PRE 
      0: Abort DFS
      1: Continue with childs
      2: Continue, but skip any childs for this element

    M2_DFS_MSG_POST 
      0: Abort DFS
      1: Continue 

    Internal Variables available for the callback
      m2->dfs_depth		Current depth, inside callback this is 1 (root node) and above
      m2->dfs_el_nr		Internal sequence number, do not use this: Instead use the argument to the callback
*/

static uint8_t m2_DoDFS(m2_t *m2, const  m2el_t *el_ptr)
{
  uint16_t el_nr;
  uint16_t i;
  
  m2->dfs_depth++;
  m2->dfs_el_nr++;
  
  m2->dfs_el_ptr = el_ptr;
  el_nr = m2->dfs_el_nr;
  
  /* store the return value in "i", reuse "i" later to save some stack space */
  i = m2->dfs_fn(m2, M2_DFS_MSG_PRE, el_nr, el_ptr);  
  if ( i == 0 )
  {
    /* the information about the current element is still up to date */
    return 0;
  }
  
  /* if the return value has been 2, then skip childs */
  if ( i != 2 )
  {
    /* visit all childs */
    for( i = 0; i < m2el_GetChildCnt(m2, el_ptr); i++ )
    {
      if ( m2_DoDFS(m2, m2el_GetChildPtr(m2, el_ptr, i)) == 0 )
	return 0;
    }
  }
  
  if ( m2->dfs_fn(m2, M2_DFS_MSG_POST, el_nr, el_ptr) == 0 )
  {
    /* store info about current element, so that the element which aborted the traversal can be identified */
    m2->dfs_el_nr = el_nr;
    m2->dfs_el_ptr = el_ptr;
    return 0;
  }
  m2->dfs_depth--;
  return 1;
  
}

/*
  return: 
    0 if m2_DFS() was aborted by the callback procedure
    != 0 if tree was traversed completly
*/
uint8_t m2_DFS(m2_t *m2, m2_dfs_fn fn)
{
  m2->dfs_fn = fn;
  m2->dfs_el_nr = 0;
  m2->dfs_depth = 0;
  
  return m2_DoDFS(m2, m2->root_el_ptr);
}

