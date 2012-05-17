/*

  graphics.c 
  
  How to mix Graphics with M2tklib

  m2tklib - Mini Interative Interface Toolkit Library
  u8glib - Universal 8bit Graphics Library
  
  Copyright (C) 2012  olikraus@gmail.com

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



  Two methods:
    - mix/combine menu and graphics
    - switch between menu and graphcs
    
   Graphic procedure contain two parts:
    - draw
    - update, which also returns if a redraw is required
    
    draw_graphics():
      depending on m2tk state, draw graphics
    
    update_graphics():
      depending on m2tk state, update graphics for animation and force redraw
      

*/

#include "u8g.h"
#include "m2.h"
#include "m2ghu8g.h"

#if defined(__AVR__)
#include <avr/interrupt.h>
#include <avr/io.h>
#endif

/*=========================================================================*/
/* global variables and objects */

/* system 10ms counter, will overflow after 497 days */
volatile uint32_t sys_10ms_cnt = 0;

/* u8g object */
u8g_t u8g;

/*=========================================================================*/
/* menu definitions */

M2_EXTERN_ALIGN(el_top);

/*=========================================================================*/
/* low level graphics */

uint8_t is_bigger = 0;
uint32_t next_state_change = 0;
uint8_t size = 1;

/* draw a rectange at x/y */
void draw_rectangle(uint8_t x, uint8_t y) {
  u8g_SetDefaultForegroundColor(&u8g);
  u8g_DrawBox(&u8g,x,y,size,size);
}

/* state machine for the animation of the rectangle */
/* will return none-zero if an update is required */
uint8_t update_rectangle(void) {
  if ( next_state_change < sys_10ms_cnt ) {
    next_state_change = sys_10ms_cnt;
    next_state_change += 30;
    if ( is_bigger == 0 ) {
      size -= 1;
      if ( size <= 1 )
        is_bigger = 1;
    }
    else {
      size += 1;
      if ( size >= 10 )
        is_bigger = 0;
    }
    return 1;
  }
  return 0;
}

/*=========================================================================*/
/* m2tk related code */

uint8_t y = 0;                   // position of the low level graphics

void fn_inc(m2_el_fnarg_p fnarg) {
  if ( y < 63-10 )
    y += 1;
}

void fn_dec(m2_el_fnarg_p fnarg) {
  if ( y > 0 )
    y -= 1;
}

M2_BUTTON(el_plus, "x30y21", "-1", fn_dec);
M2_BUTTON(el_minus, "x30y11", "+1", fn_inc);
M2_ROOT(el_leave_combine, "x30y1", "Back", &el_top);
M2_LIST(el_btn_list) = { &el_plus, &el_minus, &el_leave_combine};
M2_XYLIST(el_combine, NULL, el_btn_list);

M2_LABEL(el_goto_title, NULL, "Graphics and M2tk");
M2_ROOT(el_goto_combine, NULL, "Combine", &el_combine);
M2_ROOT(el_goto_switch, NULL, "Switch", &m2_null_element);             // selecting this, will remove all menues
M2_LIST(list_menu) = {&el_goto_title, &el_goto_combine, &el_goto_switch};
M2_VLIST(el_menu_vlist, NULL, list_menu);
M2_ALIGN(el_top, "W64H64", &el_menu_vlist);

/*=========================================================================*/
/* high level draw and update procedures */

void draw_graphics(void) {
  /* show the graphics depending on the current toplevel element */
  
  if ( m2_GetRoot() == &el_combine ) {
      /* combine is active, do add the rectangle */
      /* menu is on the right, put the rectangle to the left */
      draw_rectangle(0,y);
  }

  if ( m2_GetRoot() == &m2_null_element ) {
      /* all menus are gone, show the rectangle */
      draw_rectangle(10,10);
  }
}

/* update graphics, will return none-zero if an update is required */
uint8_t update_graphics(void) {  
  if ( m2_GetRoot() == &el_combine ) {
      /* "combine graphics" is active, update the rectangle for animation */
      return update_rectangle();
  }

  if ( m2_GetRoot() == &m2_null_element ) {
      /* check for any keys and assign a suitable menu again */
      if ( m2_GetKey() != M2_KEY_NONE )
        m2_SetRoot(&el_top);
    
      /* all menus are gone, rectangle is shown, so do update */
      return update_rectangle();
  }
  
  /* no update for the graphics required */
  return 0;
}



/*=========================================================================*/
/* controller, u8g and m2 setup */

void setup(void) {
  /*
    Test Envionment, ATMEGA with the following settings:
    CS: PORTB, Bit 2
    A0: PORTB, Bit 1
    SCK: PORTB, Bit 5
    MOSI: PORTB, Bit 3
  */
  /* 1. Setup and create u8g device */
  /* u8g_InitSPI(&u8g, &u8g_dev_st7565_dogm132_sw_spi, PN(1, 5), PN(1, 3), PN(1, 2), PN(1, 1), U8G_PIN_NONE); */
  u8g_InitHWSPI(&u8g, &u8g_dev_st7565_dogm132_hw_spi, PN(1, 2), PN(1, 1), U8G_PIN_NONE);

  /* 2. Setup m2 */
  m2_Init(&el_top, m2_es_avr_u8g, m2_eh_4bs, m2_gh_u8g_bfs);

  /* 3. Connect u8g display to m2  */
  m2_SetU8g(&u8g, m2_u8g_box_icon);

  /* 4. Set a font, use normal u8g_font's */
  m2_SetFont(0, (const void *)u8g_font_5x8r);
	
  /* 5. Define keys */
  m2_SetPin(M2_KEY_EXIT, PN(3, 5));
  m2_SetPin(M2_KEY_SELECT, PN(3, 6));
  m2_SetPin(M2_KEY_NEXT, PN(3, 7));
  m2_SetPin(M2_KEY_PREV, PN(1, 7));
}


/*=========================================================================*/
/* 10ms interrupt */

/* 256 - 8000000Hz / (100 Hz *1024) = 256L - F_CPU / 102400L */
#define TCNT_START_VALUE (256L - F_CPU / 102400L)

ISR(TIMER0_OVF_vect) {
  TCNT0 = TCNT_START_VALUE;
  sys_10ms_cnt++;
}

/*=========================================================================*/
/* system setup */


void sys_init(void) {
#if defined(__AVR__)
  cli();
  
  /* select minimal prescaler (max system speed) */
  CLKPR = 0x80;
  CLKPR = 0x00;
  
  /* setup 10ms interrupt (overflow Timer 0) */
  TCCR0A = 0;           /* all external pins disconnected, normal mode */
  TCCR0B = 5;           /* devide by 1024 */
  TCNT0 = TCNT_START_VALUE;	
  TIMSK0 |= 1; /* interrupt on overflow */
  
  sei();
#endif
}

/*=========================================================================*/
/* u8g draw procedure (body of picture loop) */

/* draw procedure of the u8g picture loop */
void draw(void)
{
  draw_graphics();
  /* call the m2 draw procedure */
  m2_Draw();
}

/*=========================================================================*/
/* main procedure with u8g picture loop */

int main(void) {
  /* setup controller */
  sys_init();
	
  /* setup u8g and m2 libraries */
  setup();

  /* application main loop */
  for(;;) {  
    m2_CheckKey();
    if ( m2_HandleKey() || update_graphics() ) {
      /* picture loop */
      u8g_FirstPage(&u8g);
      do {
	draw();
        m2_CheckKey();
      } while( u8g_NextPage(&u8g) );
    }
  }  
}

