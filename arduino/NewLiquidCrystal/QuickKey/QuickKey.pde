/*

  QuickKey.pde
  
  New LiquidCrystal 16x2 example

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


  This example shows the usage of quick keys. Usually quick keys are used 
  together with NEXT, PREV, SELECT keys, but the menues are designed in
  that way, that only two quick keys are needed for full control.

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

M2_EXTERN_ALIGN(top_el_quick_key);


uint8_t u8num = 0;
M2_LABEL(el_qknum_label, NULL, "q1:");
M2_U8NUM(el_qknum_val, "q1", 0, 255, &u8num);
M2_LIST(list_qknum) = { &el_qknum_label, &el_qknum_val };
M2_HLIST(el_qknum_hlist, "", list_qknum);
M2_ROOT(el_qknum_goto_top, "f4q2", "q2: main menu", &top_el_quick_key);
M2_LIST(list_qknum_v) = { &el_qknum_hlist, &el_qknum_goto_top };
M2_VLIST(el_qknum_vlist, NULL, list_qknum_v);
M2_ALIGN(top_el_qknum, "-1|1W64H64", &el_qknum_vlist);

uint8_t qktoggle_val = 0;
M2_LABEL(el_qktoggle_label, NULL, "q1:");
M2_TOGGLE(el_qktoggle_val, "q1", &qktoggle_val);
M2_LIST(list_qktoggle) = { &el_qktoggle_label, &el_qktoggle_val };
M2_HLIST(el_qktoggle_hlist, "", list_qktoggle);
M2_ROOT(el_qktoggle_goto_top, "f4q2", "q2: main menu", &top_el_quick_key);
M2_LIST(list_qktoggle_v) = { &el_qktoggle_hlist, &el_qktoggle_goto_top };
M2_VLIST(el_qktoggle_vlist, NULL, list_qktoggle_v);
M2_ALIGN(top_el_qktoggle, "-1|1W64H64", &el_qktoggle_vlist);


M2_ROOT(el_qk_menu1, "f4q1", "q1:U8NUM", &top_el_qknum);
M2_ROOT(el_qk_menu2, "f4q2", "q2:TOGGLE", &top_el_qktoggle);
M2_LIST(list_qk_menu) = { &el_qk_menu1, &el_qk_menu2 };
M2_VLIST(el_qk_menu_vlist, NULL, list_qk_menu);
M2_ALIGN(top_el_quick_key, "-1|1W64H64", &el_qk_menu_vlist);

M2tk m2(&top_el_quick_key, m2_es_arduino, m2_eh_6bs, m2_gh_nlc);

void setup() {
  m2_SetNewLiquidCrystal(&lcd, 16, 2);
  m2.setPin(M2_KEY_Q1, uiKeySelectPin);
  m2.setPin(M2_KEY_Q2, uiKeyNextPin);
}

void loop() {
  m2.checkKey();
  m2.checkKey();
  if ( m2.handleKey() )
    m2.draw();
  m2.checkKey();
}
