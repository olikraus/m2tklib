/*

  m2elinfo.c

  Display scrollable long text (based on "strlist")

  m2tklib = Mini Interative Interface Toolkit Library
  
  Copyright (C) 2011  olikraus@gmail.com

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
  
*/

#include "m2.h"

uint8_t m2_el_info_goto_next_line_break(void) M2_NOINLINE;
void m2_el_info_ptr_inc(void) M2_NOINLINE;
uint8_t m2_el_info_get_lines(void) M2_NOINLINE;
void m2_el_info_goto_line(uint8_t line) M2_NOINLINE;
void m2_el_info_copy_line(void) M2_NOINLINE;


/* number of chars in the RAM buffer, must include the terminating '\0' character */
#define M2_INFO_LINE_LEN 40
uint8_t *m2_el_info_ptr;
char m2_el_info_line[M2_INFO_LINE_LEN];

void m2_el_info_ptr_inc(void)
{
  m2_el_info_ptr++;
}

/* move m2_el_info_str_ptr to the next line break */
/* returns the terminating char which was found, \n or \0 */
uint8_t m2_el_info_goto_next_line_break(void)
{
  uint8_t c;
  uint8_t *ptr = m2_el_info_ptr;
  for(;;)
  {
    c = *ptr;
    if ( c == '\n' )
      break;
    if ( c == '\0' )
      break;
    ptr++;
  }
  m2_el_info_ptr = ptr;
  return c;
}

/* count the number of lines */
/* m2_el_info_ptr must be set to the start of the string, before calling this procedure */
/* m2_el_info_ptr will point to the terminating \0 after calling this procedure */
uint8_t m2_el_info_get_lines(void)
{
  uint8_t lines = 1;
  do
  {
    if ( m2_el_info_goto_next_line_break() == '\0' )
      break;
    m2_el_info_ptr_inc();
    lines++;
  } while( lines < ((1<<(sizeof(lines)*8))-2) );
  return lines;
}

/* count the number of lines */
/* m2_el_info_ptr must be set to the start of the string, before calling this procedure */
/* m2_el_info_ptr will point to the first char of the spezified line or \0  */
void m2_el_info_goto_line(uint8_t line)
{
  uint8_t pos = 0;
  for(;;)
  {
    if ( pos == line )
      break;
    if ( m2_el_info_goto_next_line_break() == '\0' )
      break;
    m2_el_info_ptr_inc();
    pos++;
  }
}

/* copy the line to the RAM buffer */
/* m2_el_info_ptr must be set to the start of the line, before calling this procedure */
void m2_el_info_copy_line(void)
{
  uint8_t pos;
  uint8_t c;
  uint8_t *ptr = m2_el_info_ptr;
  pos = 0;
  for(;;)
  {
    c = *ptr;
    m2_el_info_line[pos] = c;
    if ( c == '\n' )
      break;
    if ( c == '\0' )
      break;
    if ( pos >= M2_INFO_LINE_LEN-1 )
      break;
    ptr++;
  }
  m2_el_info_line[pos] = '\0';
}

/* copy the line to the RAM buffer */
/* m2_el_info_ptr must be set to the start of the string, before calling this procedure */
void m2_el_info_get_line(uint8_t line)
{
  m2_el_info_goto_line(line);
  m2_el_info_copy_line();
}

/*==============================================================*/
/* info structure access */

/* returned ptr might point to ram or rom */
uint8_t *m2_el_info_get_ptr(m2_rom_void_p element)
{
  return (uint8_t *)m2_rom_get_ram_ptr(element, offsetof(m2_el_info_t, info_str));
}


/*==============================================================*/
/* info function */

#ifdef NOT_YET_FINISHED

M2_EL_FN_DEF(m2_el_info_fn)
{
  switch(fn_arg->msg)
  {
    case M2_EL_MSG_GET_LIST_LEN:
      m2_el_info_ptr = m2_el_info_get_ptr(fn_arg->element);
      return m2_el_info_get_lines();
      //return m2_el_slbase_get_len(fn_arg->element);
    case M2_EL_MSG_GET_LIST_ELEMENT:
      *((m2_rom_void_p *)(fn_arg->data)) = &m2_el_virtual_strline;
      return 1;
    case M2_EL_MSG_IS_AUTO_SKIP:
      return 1;
    case M2_EL_MSG_GET_OPT:
	if ( fn_arg->arg == 'd' )
	{
	  *(uint8_t *)(fn_arg->data) = 1;
	  return 1;
	}
	/* else... break out of the switch and let the base class do the rest of the work */
	break;
    case M2_EL_MSG_GET_LIST_BOX:
      m2_el_slbase_calc_box(fn_arg->element, fn_arg->arg, ((m2_pcbox_p)(fn_arg->data)));
      return 1;
    case M2_EL_MSG_GET_HEIGHT:
      return m2_el_slbase_calc_height((fn_arg->element));
    case M2_EL_MSG_GET_WIDTH:
      return m2_el_slbase_calc_width((fn_arg->element));
#ifdef M2_EL_MSG_DBG_SHOW
    case M2_EL_MSG_DBG_SHOW:
      {
	uint8_t width, height;
	m2_pos_p b = (m2_pos_p)(fn_arg->data);
	width = m2_el_slbase_calc_width((fn_arg->element));
	height = m2_el_slbase_calc_height((fn_arg->element));
	printf("info_p w:%d h:%d arg:%d x:%d y:%d len:%d\n", width, height, 
	    (fn_arg->arg), b->x, b->y, m2_el_slbase_get_len(fn_arg->element));
      }
      return 0;
#endif
      case M2_EL_MSG_SHOW:
	/* MSG_SHOW: parent is drawn before the sub elements */
	/* adjust top element to total size, if required */
	m2_el_slbase_adjust_top_to_cnt(fn_arg->element);
	break;
  }
  return m2_el_fnfmt_fn(fn_arg);
}

#endif
