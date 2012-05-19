/*

  blink.c 
  
  Let a box blink at the lower left edge. Derived from the Arduino LEDFlash example.
  Instead of an external LED, there will be a box at the upper left of the display.
  This is also an example for mixing graphics and menues.
  

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

/*=========================================================================*/
/* Variables for Blink/Flashing Process */

volatile uint8_t led_state = 0;
volatile uint32_t next_change = 0;
volatile uint8_t is_flashing = 0;
uint8_t led_on_time = 5;
uint8_t led_off_time = 5;
volatile uint8_t is_led_on = 0;

/*=========================================================================*/
/* Blink/Flashing Process */

#define STATE_STOP 0
#define STATE_SETUP_ON 1
#define STATE_WAIT_ON 2
#define STATE_SETUP_OFF 3
#define STATE_WAIT_OFF 4

/* returns 1 if an update of the screen is required */
uint8_t led_process(void)
{
  uint8_t is_update = 0;
  switch(led_state) {
    case STATE_STOP:
      if ( is_flashing )
	led_state = STATE_SETUP_ON;
      break;
    case STATE_SETUP_ON:
      next_change = sys_10ms_cnt + (led_on_time*5L);
      led_state = STATE_WAIT_ON;
      is_led_on = 1;
      is_update = 1;
      break;
    case STATE_WAIT_ON:
      if ( is_flashing == 0 )
	led_state = STATE_STOP;
      else if ( next_change < sys_10ms_cnt )
	led_state = STATE_SETUP_OFF;
      break;
    case STATE_SETUP_OFF:
      next_change = sys_10ms_cnt + (led_off_time*5L);
      led_state = STATE_WAIT_OFF;
      is_led_on = 0;
      is_update = 1;
      break;
    case STATE_WAIT_OFF:
      if ( is_flashing == 0 )
	led_state = STATE_STOP;
      else if ( next_change < sys_10ms_cnt )
	led_state = STATE_SETUP_ON;
      break;
  }
  return is_update;
}


/*=========================================================================*/
/* m2tk related code */

void fn_start(m2_el_fnarg_p fnarg) {
  is_flashing = 1;
  led_state = STATE_SETUP_ON;
  is_led_on = 1;
}

void fn_stop(m2_el_fnarg_p fnarg) {
  is_flashing = 0;
}

M2_LABEL(el_label_on, NULL, "On Time:");
M2_U8NUM(el_u8_on, "c1", 1, 9, &led_on_time);

M2_LABEL(el_label_off, NULL, "Off Time:");
M2_U8NUM(el_u8_off, "c1", 1, 9, &led_off_time);

M2_BUTTON(el_stop, NULL, "Stop", fn_stop);
M2_BUTTON(el_start, NULL, "Start", fn_start);

M2_LIST(list) = { 
    &el_label_on, &el_u8_on, 
    &el_label_off, &el_u8_off, 
    &el_stop, &el_start 
};
M2_GRIDLIST(list_element, "c2",list);
M2_ALIGN(el_align, "W64H64", &list_element);



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
  u8g_InitSPI(&u8g, &u8g_dev_st7565_dogm132_sw_spi, PN(1, 5), PN(1, 3), PN(1, 2), PN(1, 1), U8G_PIN_NONE); 
  /*u8g_InitHWSPI(&u8g, &u8g_dev_st7565_dogm132_hw_spi, PN(1, 2), PN(1, 1), U8G_PIN_NONE);*/

  /* 2. Setup m2 */
  m2_Init(&el_align, m2_es_avr_u8g, m2_eh_4bs, m2_gh_u8g_bfs);

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
  u8g_SetDefaultForegroundColor(&u8g);
  
  if ( is_led_on )
    u8g_DrawBox(&u8g, 0,0,5,5);
  else
    u8g_DrawFrame(&u8g, 0,0,5,5);
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
    if ( m2_HandleKey() || led_process() ) {
      /* picture loop */
      u8g_FirstPage(&u8g);
      do {
	draw();
        m2_CheckKey();
      } while( u8g_NextPage(&u8g) );
    }
  }  
}

