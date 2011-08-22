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

/* number of chars in the RAM buffer, must include the terminating '\0' character */
#define M2_INFO_LINE_LEN 40
void *m2_el_info_ptr;
char m2_el_info_line[M2_INFO_LINE_LEN];

void m2_el_info_ptr_inc(void)
{
  ((uint8_t *)m2_el_info_str_ptr)++;
}

/* move m2_el_info_str_ptr to the next line break */
/* returns the terminating char which was found, \n or \0 */
uint8_t m2_el_info_goto_next_line_break(void)
{
  uint8_t c;
  for(;;)
  {
    c = *(uint8_t *)m2_el_info_str_ptr;
    if ( c == '\n' )
      break;
    if ( c == '\0' )
      break;
    m2_el_info_ptr_inc();
  }
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
/* m2_el_info_ptr will point to the first car of the spezified line or \0  */
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

