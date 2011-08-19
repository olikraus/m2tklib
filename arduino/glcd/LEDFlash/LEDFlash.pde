/*

  LEDFlash.pde
  
  Control flashing of a LED
  See also: http://arduino.cc/forum/index.php/topic,69580.15.html
  
  GLCD Example

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

#include <glcd.h>		// inform Arduino IDE that we will use GLCD library
#include "M2tk.h"
#include "m2ghglcd.h"

//=================================================
// Variables for Menu Process

uint8_t uiKeySelectPin = 3;
uint8_t uiKeyDownPin = 2;
uint8_t uiKeyUpPin = 1;
uint8_t uiKeyExitPin = 0;

//=================================================
// Variables for Flashing Process

uint8_t led_state = 0;
uint32_t next_change = 0;
uint8_t is_flashing = 0;
uint8_t flash_delay = 5;

//=================================================
// Menu Process

void fn_start(m2_el_fnarg_p fnarg) {
  is_flashing = 1;
}

void fn_stop(m2_el_fnarg_p fnarg) {
  is_flashing = 0;
}

M2_LABEL(el_label, NULL, "Time:");
M2_U8NUM(el_u8, "c1", 1, 9, &flash_delay);

M2_BUTTON(el_stop, NULL, "Stop", fn_stop);
M2_BUTTON(el_start, NULL, "Start", fn_start);

M2_LIST(list) = { 
    &el_label, &el_u8, 
    &el_stop, &el_start 
};
M2_GRIDLIST(list_element, "c2",list);
M2tk m2(&list_element, m2_es_arduino, m2_eh_2bs, m2_gh_glcd_ffs);

//=================================================
// Flashing Process

void flashing_process(void) {
  // wait until timer as expired
  if ( next_change < millis() ) {
    if ( led_state == 0 ) {
      digitalWrite(13, HIGH);  // set the LED on    
      led_state = 1;
    } else {
      digitalWrite(13, LOW);   // set the LED off
      led_state = 0;
    }
    // wait for x*50ms, were x is between 0 and 9
    next_change = millis() + (flash_delay*50L);
  } 
}

//=================================================
// Arduino Setup & Loop

void setup() {
  m2.setPin(M2_KEY_SELECT, uiKeySelectPin);
  m2.setPin(M2_KEY_NEXT, uiKeyDownPin);
  m2.setPin(M2_KEY_PREV, uiKeyUpPin);
  m2.setPin(M2_KEY_EXIT, uiKeyExitPin);  
  pinMode(13, OUTPUT);
}

void loop() {
  // menu management
  m2.checkKey();
  if ( m2.handleKey() ) {
      m2.draw();
  }
  // flashing process
  if ( is_flashing ) 
    flashing_process();
}
