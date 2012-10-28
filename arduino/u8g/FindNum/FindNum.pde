/*

  FindNum.pde
  
  "Find my number" game from here:
  http://code.google.com/p/m2tklib/wiki/t04

  U8glib Example

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

#include <stdlib.h>
#include "U8glib.h"
#include "M2tk.h"
#include "m2ghu8g.h"

// setup u8g object, please remove comment from one of the following constructor calls

//U8GLIB_NHD27OLED_BW u8g(13, 11, 10, 9);       // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD27OLED_2X_BW u8g(13, 11, 10, 9); // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD27OLED_GR u8g(13, 11, 10, 9);       // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD27OLED_2X_GR u8g(13, 11, 10, 9);  // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD31OLED_BW u8g(13, 11, 10, 9);       // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD31OLED_2X_BW u8g(13, 11, 10, 9); // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGS102 u8g(13, 11, 10, 9);                    // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGM132 u8g(13, 11, 10, 9);                    // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGM128 u8g(13, 11, 10, 9);                    // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_SSD1306_128X64 u8g(13, 11, 10, 9);                    // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_ST7920_128X64 u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, U8G_PIN_NONE, U8G_PIN_NONE, 17, 16);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16
//U8GLIB_ST7920_128X64 u8g(18, 16, 17, U8G_PIN_NONE);                  // SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
//U8GLIB_ST7920_192X32 u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, U8G_PIN_NONE, U8G_PIN_NONE, 17, 16);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16
//U8GLIB_ST7920_192X32 u8g(18, 16, 17, U8G_PIN_NONE);                  // SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
//U8GLIB_LM6059 u8g(13, 11, 10, 9);                    // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_LM6063 u8g(13, 11, 10, 9);                    // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGXL160_BW u8g(10, 9);            // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGXL160_GR u8g(13, 11, 10, 9);             // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGXL160_2X_BW u8g(13, 11, 10, 9);            // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGXL160_2X_GR u8g(13, 11, 10, 9);             // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_PCD8544 u8g(13, 11, 10, 9, 8);                    // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, Reset = 8
//U8GLIB_PCF8812 u8g(13, 11, 10, 9, 8);                    // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, Reset = 8
//U8GLIB_KS0108_128 u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 14, 15, 17, 16); // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs1=14, cs2=15,di=17,rw=16
//U8GLIB_LC7981_160X80 u8g(8, 9, 10, 11, 4, 5, 6, 7,  18, 14, 15, 17, 16); // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs=14 ,di=15,rw=17, reset = 16
//U8GLIB_LC7981_240X64 u8g(8, 9, 10, 11, 4, 5, 6, 7,  18, 14, 15, 17, 16); // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs=14 ,di=15,rw=17, reset = 16
//U8GLIB_LC7981_240X128 u8g(8, 9, 10, 11, 4, 5, 6, 7,  18, 14, 15, 17, 16); // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs=14 ,di=15,rw=17, reset = 16
//U8GLIB_ILI9325D_320x240 u8g(18,17,19,U8G_PIN_NONE,16 );  // 8Bit Com: D0..D7: 0,1,2,3,4,5,6,7 en=wr=18, cs=17, rs=19, rd=U8G_PIN_NONE, reset = 16
//U8GLIB_SBN1661_122X32 u8g(8,9,10,11,4,5,6,7,14,15, 17, U8G_PIN_NONE, 16); ; // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 cs1=14, cs2=15,di=17,rw=16,reset = 16
//U8GLIB_SSD1306_128X64 u8g(13, 11, 10, 9);             // SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_SSD1306_128X64 u8g(10, 9);             // HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)
//U8GLIB_SSD1327_96X96_GR u8g(U8G_I2C_OPT_NONE);             // I2C
//U8GLIB_SSD1327_96X96_2X_GR u8g(U8G_I2C_OPT_NONE);             // I2C
//U8GLIB_NHD_C12864 u8g(13, 11, 10, 9);                    // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9


// DOGS102 shield 
// uint8_t uiKeyUpPin = 5;
// uint8_t uiKeyDownPin = 3;
// uint8_t uiKeySelectPin = 4;
// uint8_t uiKeyExitPin = 0;

// DOGM132, DOGM128 and DOGXL160 shield
uint8_t uiKeyUpPin = 7;         
uint8_t uiKeyDownPin = 3;
uint8_t uiKeySelectPin = 2;
uint8_t uiKeyExitPin = 0;


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

M2tk m2(&top_el_num_input, m2_es_arduino, m2_eh_2bs, m2_gh_u8g_ffs);

void setup(void) {
  // Connect u8glib with m2tklib
  m2_SetU8g(u8g.getU8g(), m2_u8g_box_icon);

  // Assign u8g font to index 0
  m2.setFont(0, u8g_font_6x12r);

  // Setup keys
  m2.setPin(M2_KEY_SELECT, uiKeySelectPin);
  //m2.setPin(M2_KEY_PREV, uiKeyUpPin);
  m2.setPin(M2_KEY_NEXT, uiKeyDownPin);
  
  // Assign initial state for the game
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

void draw(void) {
  m2.draw();
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
  
  set_next_state();  
}

