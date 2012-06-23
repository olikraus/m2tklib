/*

  m2ghtty.c
  
  Graphics Handler for the simpe character output

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

#if ARDUINO < 100 
#include "wiring.h"
#include <WProgram.h> 
#else 
#include <Arduino.h> 
#endif

#include <string.h>
#include "m2.h"

/*====================================================*/

#define WIDTH 40
#define HEIGHT 4

static char m2_gh_serial_screen[HEIGHT*WIDTH];

static uint8_t m2_gh_serial_small_cursor_pos = 255;


/* x,y origin is at lower left (m2tklib system) */
static void m2_gh_serial_draw_text(uint8_t x, uint8_t y, const char *text)
{
  uint8_t idx;
  char *ptr = m2_gh_serial_screen;
  idx = HEIGHT - 1;
  idx -= y;
  idx *= WIDTH;
  idx += x;
  ptr += idx;
  while( *text != '\0' )
    *ptr++ = *text++;
}

static void m2_gh_serial_out(char c)
{
  Serial.print(c);
}

static void m2_gh_serial_cr(void)
{
  Serial.println("");
}

static void m2_gh_serial_show(void)
{
  uint8_t i, j, pos;
  char *line = m2_gh_serial_screen;
  pos = 0;
  for( i = 0; i < HEIGHT; i++ )
  {
    for( j = 0; j < WIDTH; j++ )
    {
      if ( m2_gh_serial_small_cursor_pos != 255 )
      {
	if ( (m2_gh_serial_small_cursor_pos == pos) || (m2_gh_serial_small_cursor_pos == (pos + 1)) )
	{
	  m2_gh_serial_out('_');
	}
	else
	{
	  m2_gh_serial_out(' ');
	}
      }
      m2_gh_serial_out(*line);
      line++;
      pos++;
    }
    m2_gh_serial_cr();
  }
}

static void m2_gh_serial_clear_screen(void)
{
  uint8_t i;
  for( i = 0; i < HEIGHT*WIDTH; i++ )
    m2_gh_serial_screen[i] = ' ';
}

/*====================================================*/

uint8_t m2_gh_arduino_serial_is_init = 0;


extern "C" uint8_t m2_gh_arduino_serial(m2_gfx_arg_p  arg)
{
  switch(arg->msg)
  {
    case M2_GFX_MSG_INIT:
      break;
    case M2_GFX_MSG_START:
      if ( m2_gh_arduino_serial_is_init == 0 )
      {
	Serial.begin(9600);        // init serial
	Serial.println("m2tklib LCD simulator");
	Serial.println("");
	m2_gh_arduino_serial_is_init = 1;
      }
      m2_gh_serial_small_cursor_pos = 255;
      m2_gh_serial_clear_screen();
      break;
    case M2_GFX_MSG_END:
      m2_gh_serial_show();
      break;
    case M2_GFX_MSG_DRAW_TEXT:
      m2_gh_serial_draw_text(arg->x, arg->y, arg->s);
      return 0;
    case M2_GFX_MSG_DRAW_NORMAL_FOCUS:
    case M2_GFX_MSG_DRAW_NORMAL_PARENT_FOCUS:
      m2_gh_serial_draw_text(arg->x, arg->y, "[");
      m2_gh_serial_draw_text(arg->x+arg->w-1, arg->y, "]");
      return 0;
    case M2_GFX_MSG_DRAW_GO_UP:
      m2_gh_serial_draw_text(arg->x-1, arg->y, "<");
      m2_gh_serial_draw_text(arg->x+arg->w, arg->y, ">");
      return 0;
    case M2_GFX_MSG_DRAW_SMALL_FOCUS:
      m2_gh_serial_small_cursor_pos = HEIGHT -  1 - arg->y;
      m2_gh_serial_small_cursor_pos*= WIDTH;
      m2_gh_serial_small_cursor_pos += arg->x+1;  // why is +1 required here???
      return 0;
    case M2_GFX_MSG_DRAW_ICON:
      if ( arg->icon == M2_ICON_TOGGLE_ACTIVE || arg->icon == M2_ICON_RADIO_ACTIVE )
	m2_gh_serial_draw_text(arg->x, arg->y, "*");
      else
	m2_gh_serial_draw_text(arg->x, arg->y, ".");
      return 0;
    case M2_GFX_MSG_GET_TEXT_WIDTH:
      return strlen(arg->s);
    case M2_GFX_MSG_GET_ICON_WIDTH:
    case M2_GFX_MSG_GET_ICON_HEIGHT:
    case M2_GFX_MSG_GET_CHAR_WIDTH:
    case M2_GFX_MSG_GET_CHAR_HEIGHT:
      return 1;
    case M2_GFX_MSG_GET_NORMAL_BORDER_WIDTH:
      return 2;
    case M2_GFX_MSG_GET_NORMAL_BORDER_X_OFFSET:
      return 1;
    case M2_GFX_MSG_GET_DISPLAY_WIDTH:
      return WIDTH;
    case M2_GFX_MSG_GET_DISPLAY_HEIGHT:
      return HEIGHT;
    case M2_GFX_MSG_DRAW_VERTICAL_SCROLL_BAR:
      /* scroll bar: "total" total number of items */
      /* scroll bar: "top" topmost item (first visible item) 0 .. total-visible*/
      /* scroll bar: "visible" number of visible items 0 .. total-1 */

      /* not yet implemented
      {
	uint16_t h, y, byte_index;
	char c;
	h = m2_utl_sb_get_slider_height(arg->h*8-2, arg->total, arg->visible);
	y = m2_utl_sb_get_slider_position(arg->h*8-2, h, arg->total, arg->visible, arg->top); 
	for( byte_index = 0; byte_index < arg->h; byte_index++ )
	{
	  m2_gh_lc_set_vsb_char(arg->h*8, 1, y, h, byte_index);
	  m2_gh_lc_set_cursor(arg->x, arg->y+arg->h-1-byte_index);
	  c = 3;
	  c += byte_index;
	  m2_lc_ptr->print(c);
	}
      }
      */
      return 1;
  }
  return m2_gh_dummy(arg);
}
