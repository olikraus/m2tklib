/*

  LEDFlash.pde
  
  Control flashing of a LED (Pin 13/orange LED of the Botmat Board)
  See also: http://arduino.cc/forum/index.php/topic,69580.15.html
  
  M2tklib Botmat Example

  m2tklib = Mini Interative Interface Toolkit Library

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

#include "U8glib.h"
#include "M2tk.h"
#include "m2ghu8g.h"

//U8GLIB_DOGM128 u8g(7, 5, 1, 2);                    // SPI Com: SCK = 7, MOSI = 5, CS = 1, A0 = 2
U8GLIB_DOGM128 u8g(1, 2);                    // HW SPI CS = 1, A0 = 2

//=================================================
// Variables for Flashing Process

uint8_t led_state = 0;
uint32_t next_change = 0;
uint8_t is_flashing = 0;
uint8_t led_on_time = 5;
uint8_t led_off_time = 5;

//=================================================
// Flashing Process

#define STATE_STOP 0
#define STATE_SETUP_ON 1
#define STATE_WAIT_ON 2
#define STATE_SETUP_OFF 3
#define STATE_WAIT_OFF 4

void led_process(void)
{
  switch(led_state) {
    case STATE_STOP:
      if ( is_flashing )
	led_state = STATE_SETUP_ON;
      break;
    case STATE_SETUP_ON:
      next_change = millis() + (led_on_time*50L);
      led_state = STATE_WAIT_ON;
      digitalWrite(13, HIGH);  // set the LED on    
      break;
    case STATE_WAIT_ON:
      if ( is_flashing == 0 )
	led_state = STATE_STOP;
      else if ( next_change < millis() )
	led_state = STATE_SETUP_OFF;
      break;
    case STATE_SETUP_OFF:
      next_change = millis() + (led_off_time*50L);
      led_state = STATE_WAIT_OFF;
      digitalWrite(13, LOW);   // set the LED off
      break;
    case STATE_WAIT_OFF:
      if ( is_flashing == 0 )
	led_state = STATE_STOP;
      else if ( next_change < millis() )
	led_state = STATE_SETUP_ON;
      break;
  }
}


//=================================================
// Menu Process

void fn_start(m2_el_fnarg_p fnarg) {
  is_flashing = 1;
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
M2tk m2(&el_align, m2_es_arduino, m2_eh_6bs, m2_gh_u8g_ffs);

//=================================================
// Draw procedure, Arduino Setup & Loop


void draw(void) {
  m2.draw();
}

void setup(void) {
  // activate backlight
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
  
  // Connect u8glib with m2tklib
  m2_SetU8g(u8g.getU8g(), m2_u8g_box_icon);

  // Assign u8g font to index 0
  m2.setFont(0, u8g_font_6x13r);

  // Setup keys (botmat)
  m2.setPin(M2_KEY_SELECT, 18);
  m2.setPin(M2_KEY_PREV, 20);
  m2.setPin(M2_KEY_NEXT, 21);
  m2.setPin(M2_KEY_DATA_UP, 22);
  m2.setPin(M2_KEY_DATA_DOWN, 19);
  
  pinMode(13, OUTPUT);
}

void loop() {
  // menu management
  m2.checkKey();
  if ( m2.handleKey() != 0 ) {
    u8g.firstPage();  
    do {
      draw();
    } while( u8g.nextPage() );
  }
  // flashing process
  led_process();
}

