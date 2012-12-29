/*
  
    mn_seqnr.c
    
*/

#include "stddef.h"
#include "mn.h"

void mn_ClearSeqNr(mn_type n)
{
  if ( n == NULL )
    return;
  n->seq_nr = 0;
  mn_ClearSeqNr(n->d);
  mn_ClearSeqNr(n->n);
}

static void mn_BuildSeqNrSub(mn_type n, unsigned *curr_nr)
{
  if ( n == NULL )
    return;
  if ( n->seq_nr == 0 )
  {
    n->seq_nr = *curr_nr;
    (*curr_nr)++;
  }
  mn_BuildSeqNrSub(n->d, curr_nr);
  mn_BuildSeqNrSub(n->n, curr_nr);
}

/* assign unique numbers to all subnodes. start at "start_nr" (must be > 0) */
/* returns last assigned number +1 */
unsigned mn_BuildSeqNr(mn_type n, unsigned start_nr)
{
  mn_ClearSeqNr(n);
  mn_BuildSeqNrSub(n, &start_nr);
  return start_nr;
}