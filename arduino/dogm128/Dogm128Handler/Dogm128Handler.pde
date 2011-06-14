/*

  Dogm128Handler.pde

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

  SCL (SPI Clock)   Pin 13
  SI (MOSI)         Pin 11
  CS (Chip Select)  Pin 10
  MISO (Pin 12) is not used, but can not be reused as generic I/O
  
*/

#include "Dogm.h"
#include "M2tk.h"
#include "m2ghdogm.h"

void fn_ok(m2_el_fnarg_p fnarg);
void fn_cancel(m2_el_fnarg_p fnarg);
void fn_goto_handler_selection(m2_el_fnarg_p fnarg);
M2_EXTERN_VLIST(el_text_menu);

extern M2tk m2;

#ifdef DOGS102_HW
// DOGS102 shield 
uint8_t uiKeyUpPin = 5;
uint8_t uiKeyDownPin = 3;
uint8_t uiKeySelectPin = 4;
uint8_t uiKeyExitPin = 2;
#else
// DOGM132, DOGM128 and DOGXL160 shield
uint8_t uiKeyUpPin = 7;
uint8_t uiKeyDownPin = 3;
uint8_t uiKeySelectPin = 2;
uint8_t uiKeyExitPin = 8; // ????
#endif

int a0Pin = 9;
Dogm dogm(a0Pin);


/*=============================*/

uint8_t select_handler = 0;
uint8_t old_value_select_handler = 0;

M2_LABEL(el_label1, "f1", "bf");
M2_RADIO(el_radio1, "v0", &select_handler);

M2_LABEL(el_label2, "f1", "bfs");
M2_RADIO(el_radio2, "v1", &select_handler);

M2_LABEL(el_label3, "f1", "fb");
M2_RADIO(el_radio3, "v2", &select_handler);

M2_LABEL(el_label4, "f1", "fbs");
M2_RADIO(el_radio4, "v3", &select_handler);

M2_LABEL(el_label5, "f1", "ffs");
M2_RADIO(el_radio5, "v4", &select_handler);

M2_BUTTON(el_cancel, "f4", "cancel", fn_cancel);
M2_BUTTON(el_ok, "f4", " ok ", fn_ok);

M2_LIST(handler_list) = { 
    &el_label1, &el_radio1, 
    &el_label2, &el_radio2,  
    &el_label3, &el_radio3, 
    &el_label4, &el_radio4, 
    &el_label5, &el_radio5, 
    &el_cancel, &el_ok 
};
M2_GRIDLIST(el_handler_menu, "c2", handler_list);

/*=============================*/

void fn_4x6(m2_el_fnarg_p fnarg) { m2.setFont(0, font_4x6); }
void fn_5x7(m2_el_fnarg_p fnarg) { m2.setFont(0, font_5x7); }
void fn_5x8(m2_el_fnarg_p fnarg) { m2.setFont(0, font_5x8); }
void fn_6x9(m2_el_fnarg_p fnarg) { m2.setFont(0, font_6x9); }
void fn_6x10(m2_el_fnarg_p fnarg) { m2.setFont(0, font_6x10); }
void fn_6x12(m2_el_fnarg_p fnarg) { m2.setFont(0, font_6x12); }
void fn_6x13(m2_el_fnarg_p fnarg) { m2.setFont(0, font_6x13); }


M2_BUTTON(el_4x6, "", "4x6 ", fn_4x6);
M2_BUTTON(el_5x7, "", "5x7 ", fn_5x7);
M2_BUTTON(el_5x8, "", "5x8 ", fn_5x8);
M2_BUTTON(el_6x9, "", "6x9 ", fn_6x9);
M2_BUTTON(el_6x10, "", "6x10 ", fn_6x10);
M2_BUTTON(el_6x12, "", "6x12 ", fn_6x12);
M2_BUTTON(el_6x13, "", "6x13 ", fn_6x13);
  
M2_EXTERN_VLIST(el_text_menu);
M2_ROOT(el_root, "", "Back ", &el_text_menu);
M2_LIST(list_font_select) = { &el_4x6, &el_5x7, &el_5x8, &el_6x9, &el_6x10, &el_6x12 , &el_6x13, &el_root };
M2_GRIDLIST(el_font_select, "c2", list_font_select);

/*=============================*/

#define TEXT_LEN 4
char text1[TEXT_LEN+1] = "abcd";
char text2[TEXT_LEN+1] = "abcd";

M2_LABEL(el_label, "", "Enter Text:");
M2_TEXT(el_text1, NULL, text1, TEXT_LEN);
M2_TEXT(el_text2, "f4a1", text2, TEXT_LEN);
M2_BUTTON(el_goto_handler_selection, "f4", "gfx handler", fn_goto_handler_selection);
M2_ROOT(el_goto_font_selection, "f4", "select font", &el_font_select);
M2_LIST(list) = { &el_label, &el_text1, &el_text2,&el_goto_handler_selection, &el_goto_font_selection };
M2_VLIST(el_text_menu, NULL, list);


M2tk m2(&el_text_menu, m2_es_arduino, m2_eh_4bd, m2_gh_dogm_fbs);

void set_handler(void) {
  switch(select_handler) {
    case 0: m2_SetGraphicsHandler(m2_gh_dogm_bf ); break;
    case 1: m2_SetGraphicsHandler(m2_gh_dogm_bfs); break;
    case 2: m2_SetGraphicsHandler(m2_gh_dogm_fb); break;
    case 3: m2_SetGraphicsHandler(m2_gh_dogm_fbs ); break;
    case 4: m2_SetGraphicsHandler(m2_gh_dogm_ffs ); break;
  }
}

void fn_ok(m2_el_fnarg_p fnarg) {
  old_value_select_handler = select_handler;
  set_handler();
  m2.setRoot(&el_text_menu);
}

void fn_cancel(m2_el_fnarg_p fnarg) {
  select_handler = old_value_select_handler;
  m2.setRoot(&el_text_menu);
}

void fn_goto_handler_selection(m2_el_fnarg_p fnarg) {
  m2.setRoot(&el_handler_menu);
}

void setup() {
  set_handler();
  m2.setFont(0, font_4x6);
  m2.setFont(1, font_4x6);
  m2.setPin(M2_KEY_SELECT, uiKeySelectPin);
  m2.setPin(M2_KEY_NEXT, uiKeyDownPin);
  m2.setPin(M2_KEY_PREV, uiKeyUpPin);
  m2.setPin(M2_KEY_EXIT, uiKeyExitPin);
  
}

void loop() {
  m2.checkKey();
  m2.checkKey();
  if ( m2.handleKey() ) {
    dogm.start();
    do{
      m2.checkKey();
      m2.checkKey();
      m2.draw();
    } while( dogm.next() );
  }
}


