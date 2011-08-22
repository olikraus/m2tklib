/*

  LEDFlash.pde
  
  Control flashing of a LED (Pin 13 of the Arduino Board)
  See also: http://arduino.cc/forum/index.php/topic,69580.15.html
  Please note, that the CLK line for the display is shared with pin 13, so expect
  some flashing from the display data transfer.
  
  DOGM128 Example
  
  ==============================================================
  1. Requires DOGM128 Library v1.12 or above.
  2. This example will only work with SW SPI: Goto dogm128.h and enable DOG_SPI_SW_ARDUINO
  ==============================================================

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

#include "Dogm.h"
#include "M2tk.h"
#include "m2ghdogm.h"

//=================================================
// Variables for Menu Process

#ifdef DOGS102_HW
// DOGS102 shield 
uint8_t uiKeyUpPin = 5;
uint8_t uiKeyDownPin = 3;
uint8_t uiKeySelectPin = 4;
uint8_t uiKeyExitPin = 0;
#else
// DOGM132, DOGM128 and DOGXL160 shield
uint8_t uiKeyUpPin = 7;
uint8_t uiKeyDownPin = 3;
uint8_t uiKeySelectPin = 2;
uint8_t uiKeyExitPin = 0;
#endif

int a0Pin = 9;
Dogm dogm(a0Pin);

//=================================================
// Variables for Flashing Process

uint8_t led_pin = 13;
uint8_t led_state = 0;
uint32_t next_change = 0;
uint8_t is_flashing = 1;
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
M2tk m2(&list_element, m2_es_arduino, m2_eh_2bs, m2_gh_dogm_fbs);

//=================================================
// Flashing Process

void flashing_process(void) {
  // wait until timer as expired
  if ( next_change < millis() ) {
    if ( led_state == 0 ) {
      digitalWrite(led_pin, HIGH);  // set the LED on    
      led_state = 1;
    } else {
      digitalWrite(led_pin, LOW);   // set the LED off
      led_state = 0;
    }
    // wait for x*50ms, were x is between 0 and 9
    next_change = millis() + (flash_delay*50L);
  } 
}

//=================================================
// Arduino Setup & Loop

void setup() {
  m2.setFont(0, font_6x10);
  m2.setPin(M2_KEY_SELECT, uiKeySelectPin);
  m2.setPin(M2_KEY_NEXT, uiKeyDownPin);
  m2.setPin(M2_KEY_PREV, uiKeyUpPin);
  m2.setPin(M2_KEY_EXIT, uiKeyExitPin);  
  pinMode(led_pin, OUTPUT);
}

void loop() {
  // menu management
  m2.checkKey();
  if ( m2.handleKey() ) {
    dogm.start();
    do{
      m2.checkKey();
      m2.draw();
    } while( dogm.next() );
  }
  // flashing process
  if ( is_flashing ) 
    flashing_process();
}
