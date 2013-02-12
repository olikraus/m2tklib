/*

  StrList.pde

  New LiquidCrystal 16x4 example

  m2tklib = Mini Interative Interface Toolkit Library
  
  Copyright (C) 2013  olikraus@gmail.com
  
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

/*
    New LiquidCrystal 
    https://bitbucket.org/fmalpartida/new-liquidcrystal/
  
    Constructor				Include
    
    LiquidCrystal			#include <LiquidCrystal.h>
    LiquidCrystal_I2C		#include <LiquidCrystal_I2C.h>
    LiquidCrystal_SR			#include <LiquidCrystal_SR.h>
    LiquidCrystal_SR2W		#include <LiquidCrystal_SR2W.h>
    LiquidCrystal_SR3W		#include <LiquidCrystal_SR3W.h>
*/


#include <LiquidCrystal.h>
#include "M2tk.h"
#include "m2ghnlc.h"

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

uint8_t uiKeySelectPin = 10;
uint8_t uiKeyNextPin = 9;

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

M2_STRLIST(el_strlist, "l2w12", &el_strlist_first, &el_strlist_cnt, el_strlist_getstr);
M2_SPACE(el_space, "w1h1");
M2_VSB(el_strlist_vsb, "l2w1r1", &el_strlist_first, &el_strlist_cnt);
M2_LIST(list_strlist) = { &el_strlist, &el_space, &el_strlist_vsb };
M2_HLIST(el_strlist_hlist, NULL, list_strlist);

M2_SPACE(el_vspace, "w1h1");

M2_LABEL(el_label,NULL, "Selected:");
M2_LABELPTR(el_labelptr,NULL, &selected);
M2_LIST(list_label) = { &el_label, &el_labelptr };
M2_HLIST(el_label_hlist, NULL, list_label);

M2_LIST(list) = { &el_strlist_hlist, &el_vspace, &el_label_hlist };
M2_VLIST(el_vlist, NULL, list);
M2_ALIGN(top_el, "-1|1W64H64", &el_vlist);

M2tk m2(&top_el, m2_es_arduino, m2_eh_4bs, m2_gh_nlc);

void setup() {
  m2_SetNewLiquidCrystal(&lcd, 16, 4);
  m2.setPin(M2_KEY_SELECT, uiKeySelectPin);
  m2.setPin(M2_KEY_NEXT, uiKeyNextPin);
}

void loop() {
  m2.checkKey();
  m2.checkKey();
  if ( m2.handleKey() )
    m2.draw();
  m2.checkKey();
}


