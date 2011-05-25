/*

  m2draw.c

  Code size status: 
    Proccedure attributes: Optimization completed
    Arithmetic optimization: Pending

*/

#include <stddef.h>
#include "m2.h"


/*=========================================================================*/
/* draw procedure */

/* a reference to the object which describes the focus on the current widget */
m2_nav_p m2_draw_focus;

/* a temporary object which holds the draw cursor */
m2_nav_t m2_draw_current;

/* 0: frame is drawn before the text or other content */
/* 1: frame is drawn last */
uint8_t m2_is_frame_draw_at_end;

/*
  This procedure draws (renders) the current object by sending the M2_EL_MSG_SHOW
  message to the element handler.  
*/

void m2_draw_visit_node(m2_pos_p box, uint8_t msg)
{
  uint8_t d, arg;
  arg = 0;
  
  /* calculate minimum of current and focus depth */
  d = m2_draw_current.depth;
  
  /* the depth must be 2 or higher. */
  if ( d > 1 )
  {
    /* an element has focus, if the depth is lower or equal to the element with the current focus */
    if ( d <= m2_draw_focus->depth )
    {
      /* valid child positions are stored at depth-2 */
      d--;
      
      /* compare all positions up to the root */
      /* this is: check if the current element is equal to the focus element */
      for(;;)
      {
	d--;
	if ( m2_draw_current.pos[d] != m2_draw_focus->pos[d] )
	  break;
	if ( d == 0 )
	{
	  /* root has been reached and all positions are equal: FOCUS EQUALS ELEMENT */
	  arg = 1;
	  if ( m2_draw_current.depth == m2_draw_focus->depth )
	  {
	    arg = 2;
	    if ( m2_draw_focus->is_data_entry_active != 0 )
	      arg = 3;
	  }
	  break;
	}
      }
    }
  }
  
  m2_nav_prepare_fn_arg_current_element(&m2_draw_current);
  m2_fn_arg_set_arg_data(arg, box);
  
#ifdef M2_EL_MSG_DBG_SHOW
  if ( msg == M2_EL_MSG_SHOW )
    m2_fn_arg_call(M2_EL_MSG_DBG_SHOW);
#endif
  m2_fn_arg_call(msg);
  
}


/*
  This procedure traverses the tree. For each node, the procedure m2_draw_visit_node()
  will be called.
*/

void m2_draw_sub(m2_pos_p box)
{
  m2_pcbox_t pcbox;
  pcbox.p = *box;
  uint8_t draw_without_focus = 0;
  uint8_t is_focus = 0;
  uint8_t is_auto_skip = 0;
  
  /* the draw depth is limited by the current focus depth */
  /* 2011-02-13: this is probably wrong, it can be deeper because of the 'd' flag */
  /* if ( m2_draw_current.depth < m2_draw_focus->depth ) */
  {
    /* try to go down, if there are children */
    if ( m2_nav_down(&m2_draw_current, 0) != 0 )
    {
      do
      {

	/* request child position from the parent */
	m2_nav_prepare_fn_arg_parent_element(&m2_draw_current);
	m2_fn_arg_set_arg_data(m2_draw_current.pos[m2_draw_current.depth-2], &pcbox);
	m2_fn_arg_call(M2_EL_MSG_GET_LIST_BOX);
	/* request down flag from the child */
	m2_fn_arg_set_element(m2_nav_get_current_element(&m2_draw_current));
	m2_fn_arg_set_arg_data('d', &draw_without_focus);
	m2_fn_arg_call(M2_EL_MSG_GET_OPT);

	/* request auto down from child */
	is_auto_skip = m2_fn_arg_call(M2_EL_MSG_IS_AUTO_SKIP);
	
	is_focus = 0;
	if ( m2_draw_current.depth <= m2_draw_focus->depth )
	  if ( m2_draw_current.pos[m2_draw_current.depth-2]  == m2_draw_focus->pos[m2_draw_current.depth-2] )
	    is_focus = 1;
	
	  
	/* draw the child */
	
	if ( m2_is_frame_draw_at_end == 0 )
	  m2_draw_visit_node(&(pcbox.c), M2_EL_MSG_SHOW);
	  
	/* if ( (draw_without_focus == 1) || (m2_draw_current.pos[m2_draw_current.depth-2]  == m2_draw_focus->pos[m2_draw_current.depth-2]) ) */
	if ( (draw_without_focus == 1) || is_focus != 0 || is_auto_skip != 0 )
	{
	  m2_draw_sub(&(pcbox.c));
	}
	
	if ( m2_is_frame_draw_at_end != 0 )
	  m2_draw_visit_node(&(pcbox.c), M2_EL_MSG_SHOW);
	
      } while( m2_nav_next(&m2_draw_current) != 0 );
      
      
#ifdef M2_EL_MSG_POST_SHOW
      m2_draw_visit_node(&(pcbox.c), M2_EL_MSG_POST_SHOW);
#endif
      m2_nav_up(&m2_draw_current);
    }
  }
}


static void m2_nav_draw(m2_nav_p nav)
{
  m2_pos_t box;
  
  m2_draw_focus = nav;
  
  box.x = 0;
  box.y = 0;
  /*
  box.h = 0;
  box.w = 0;
  */
  if ( nav->depth <= 0 )
    return;
  
  m2_draw_current.element_list[0] = nav->element_list[0];
  m2_draw_current.depth = 1;
  m2_draw_visit_node(&box, M2_EL_MSG_SHOW);
  m2_draw_sub(&box);
#ifdef M2_EL_MSG_POST_SHOW
  m2_draw_visit_node(&box, M2_EL_MSG_POST_SHOW);
#endif
}

void m2_Draw(m2_p ep)
{
  m2_is_frame_draw_at_end = ep->is_frame_draw_at_end;
  m2_gfx_start(ep->gh);
  m2_nav_draw(m2_get_nav(ep));
  m2_gfx_end();
}

