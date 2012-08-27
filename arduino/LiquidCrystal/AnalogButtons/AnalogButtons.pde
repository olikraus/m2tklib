
/*

  AnalogButtons.pde (derived from StrList.pde)

  m2tklib = Mini Interative Interface Toolkit Library
  
  Copyright (C) 2012  olikraus@gmail.com
  
  LiquidCrystal 16x2 example

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

#include <LiquidCrystal.h>
#include "M2tk.h"
#include "utility/m2ghlc.h"
//#include "m2ghlc.h"

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//LiquidCrystal lcd(8, 9, 4, 5, 6, 7); //Pins used on YourDuino.com LCD Display with Pushbuttons shield

/*
  YourDuino.com LCD Display with Pushbuttons shield
*/
int read_LCD_buttons_original()
{
  uint16_t adc_key_in = analogRead(0);      // read the value from the sensor 
  if (adc_key_in < 50)   return M2_KEY_NEXT /* btnRIGHT*/;  
  if (adc_key_in < 195)  return M2_KEY_DATA_UP; 
  if (adc_key_in < 380)  return M2_KEY_DATA_DOWN; 
  if (adc_key_in < 555)  return M2_KEY_PREV /* btnLEFT */; 
  if (adc_key_in < 790)  return M2_KEY_SELECT /*btnSELECT*/;   
  return M2_KEY_NONE;  // default
}

/*
  M2tklib Test Board
  R Div	R Div Scaled	Center Value
  0.000     000			104
  0.203	208			303
  0.389     398			479
  0.548	561			633
  0.688	705			864
  
*/
int read_LCD_buttons()
{
  uint16_t adc_key_in = analogRead(0);      // read the value from the sensor 
  if (adc_key_in < 104)   return M2_KEY_NEXT;  
  if (adc_key_in < 303)  return M2_KEY_DATA_UP; 
  if (adc_key_in < 479)  return M2_KEY_DATA_DOWN; 
  if (adc_key_in < 633)  return M2_KEY_PREV; 
  if (adc_key_in < 864)  return M2_KEY_SELECT;   
  return M2_KEY_NONE;  // default
}


uint8_t m2_es_arduino_analog_input(m2_p ep, uint8_t msg) 
{
  switch(msg)
  {
    case M2_ES_MSG_GET_KEY:
      return read_LCD_buttons();
    case M2_ES_MSG_INIT:
      return 0;
  }
  return 0;
}

const char *selected = "Nothing";
const char *el_strlist_getstr(uint8_t idx, uint8_t msg) {
  const char *s = "";
  if  ( idx == 0 )
    s = "Apple";
  else if ( idx == 1 )
    s = "Banana";
  else if ( idx == 2 )
    s = "Peach";
  else if ( idx == 3 )
    s = "Pumpkin";
  else if ( idx == 4 )
    s = "Corn";
  if (msg == M2_STRLIST_MSG_GET_STR) {
    /* nothing else todo, return the correct string */
  } else if ( msg == M2_STRLIST_MSG_SELECT ) {
    selected = s;
  }
  return s;
}

uint8_t el_strlist_first = 0;
uint8_t el_strlist_cnt = 5;

M2_STRLIST(el_strlist, "l1w12", &el_strlist_first, &el_strlist_cnt, el_strlist_getstr);
M2_SPACE(el_space, "w1h1");
M2_VSB(el_strlist_vsb, "l1w1r1", &el_strlist_first, &el_strlist_cnt);
M2_LIST(list_strlist) = { &el_strlist, &el_space, &el_strlist_vsb };
M2_HLIST(el_strlist_hlist, NULL, list_strlist);

M2_SPACE(el_vspace, "w1h1");

M2_LABEL(el_label,NULL, "Selected:");
M2_LABELPTR(el_labelptr,NULL, &selected);
M2_LIST(list_label) = { &el_label, &el_labelptr };
M2_HLIST(el_label_hlist, NULL, list_label);

M2_LIST(list) = { &el_strlist_hlist, &el_label_hlist };
M2_VLIST(el_vlist, NULL, list);
M2_ALIGN(top_el, "-1|1W64H64", &el_vlist);

M2tk m2(&top_el, m2_es_arduino_analog_input, m2_eh_4bs, m2_gh_lc);

void setup() {
  m2_SetLiquidCrystal(&lcd, 16, 2);
}

void loop() {
  m2.checkKey();
  m2.checkKey();
  if ( m2.handleKey() )
    m2.draw();
  m2.checkKey();
}


