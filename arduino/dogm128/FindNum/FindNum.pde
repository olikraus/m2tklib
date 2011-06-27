/*

  FindNum.pde
  
  "Find my number" game from here:
  http://code.google.com/p/m2tklib/wiki/t04

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

#include <stdlib.h>
#include "Dogm.h"
#include "M2tk.h"
#include "m2ghdogm.h"

#ifdef DOGS102_HW
// DOGS102 shield 
uint8_t uiKeyUpPin = 5;
uint8_t uiKeyDownPin = 3;
uint8_t uiKeySelectPin = 4;
#else
// DOGM132, DOGM128 and DOGXL160 shield
uint8_t uiKeyUpPin = 7;
uint8_t uiKeyDownPin = 3;
uint8_t uiKeySelectPin = 2;
#endif

int a0Pin = 9;
Dogm dogm(a0Pin);

#define GEN_RAND 1
#define SETUP_NUM_INPUT 2
#define WAIT_NUM_INPUT 3
#define CHK_NUM 4
#define SETUP_LOWER 5
#define WAIT_LOWER 6
#define SETUP_HIGHER 7
#define WAIT_HIGHER 8
#define SETUP_SUCCESS 9
#define WAIT_SUCCESS 10

extern M2tk m2;
M2_EXTERN_U8NUM(el_num_input_u8);

uint8_t state; 	/* will contain the current state within the control flow diagram */ 
uint8_t r;     	/* the generated random numer */ 
uint8_t u;     	/* the number, which was entered by the user */ 

/* ===== NUM_INPUT ===== */

void fn_num_input_ok(m2_el_fnarg_p fnarg) {
  state = CHK_NUM;
  m2.clear();
}

M2_LABEL(el_num_input_label, NULL, "Num: ");
M2_U8NUM(el_num_input_u8, "c2", 0, 15, &u);
M2_BUTTON(el_num_input_ok, "", "ok", fn_num_input_ok);
M2_LIST(list_num_input) = { &el_num_input_label, &el_num_input_u8, &el_num_input_ok };
M2_HLIST(top_el_num_input, NULL, list_num_input);

/* ===== LOWER ===== */

void fn_lower_ok(m2_el_fnarg_p fnarg) {
  state = SETUP_NUM_INPUT;
  m2.clear();
}

M2_LABEL(el_lower_label, NULL, "Number too low");
M2_BUTTON(el_lower_ok, "", "ok", fn_lower_ok);
M2_LIST(list_lower) = { &el_lower_label, &el_lower_ok };
M2_VLIST(top_el_lower, NULL, list_lower);

/* ===== HIGHER ===== */

void fn_higher_ok(m2_el_fnarg_p fnarg) {
  state = SETUP_NUM_INPUT;
  m2.clear();
}

M2_LABEL(el_higher_label, NULL, "Number too high");
M2_BUTTON(el_higher_ok, "", "ok", fn_higher_ok);
M2_LIST(list_higher) = { &el_higher_label, &el_higher_ok };
M2_VLIST(top_el_higher, NULL, list_higher);

/* ===== SUCCESS ===== */

void fn_success_ok(m2_el_fnarg_p fnarg) {
  state = GEN_RAND;
  m2.clear();
}

M2_LABEL(el_success_label, NULL, "Number found!");
M2_BUTTON(el_success_ok, "", "ok", fn_success_ok);
M2_LIST(list_success) = { &el_success_label, &el_success_ok };
M2_VLIST(top_el_success, NULL, list_success);

/* ===== setup m2tklib ===== */

M2tk m2(&top_el_num_input, m2_es_arduino, m2_eh_2bs, m2_gh_dogm_fbs);

void setup() {
  m2.setFont(0, font_6x12);
  m2.setPin(M2_KEY_SELECT, uiKeySelectPin);
  m2.setPin(M2_KEY_NEXT, uiKeyDownPin);
  state = GEN_RAND;
}

/* ===== calculate next state of the game ===== */

void set_next_state(void) {
  switch(state) {
    case GEN_RAND: r = rand(); r&=15; state = SETUP_NUM_INPUT; break;
    case SETUP_NUM_INPUT: m2.setRoot(&top_el_num_input); state = WAIT_NUM_INPUT; break;
    case WAIT_NUM_INPUT: break; 	/* state is changed in the fn_num_input_ok() callback procedure */
    case CHK_NUM:
      if ( u < r ) state = SETUP_LOWER;
      else if ( u > r ) state = SETUP_HIGHER;
      else state = SETUP_SUCCESS;
      break;
    case SETUP_LOWER: m2.setRoot(&top_el_lower); state = WAIT_LOWER; break;
    case WAIT_LOWER: break; /* state is changed in the fn_lower_ok() callback procedure */
    case SETUP_HIGHER: m2.setRoot(&top_el_higher); state = WAIT_HIGHER; break;
    case WAIT_HIGHER: break; /* state is changed in the fn_higher_ok() callback procedure */
    case SETUP_SUCCESS: m2.setRoot(&top_el_success); state = WAIT_SUCCESS; break;
    case WAIT_SUCCESS: break; /* state is changed in the fn_success_ok() callback procedure */
    default: state = GEN_RAND; break;
  }
}

/* ===== main loop ===== */

void loop() {
  m2.checkKey();
  if ( m2.handleKey() ) {
    dogm.start();
    do{
      m2.checkKey();
      m2.draw();
    } while( dogm.next() );
  }
  
  set_next_state();  
}


