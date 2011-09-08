/*

  StrList.pde

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

const char *selected = "Nothing";

const char *el_strlist_getstr(uint8_t idx, uint8_t msg)
{
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
  
  if (msg == M2_STRLIST_MSG_GET_STR)
  {
    /* nothing else todo, return the correct string */
  }
  else if ( msg == M2_STRLIST_MSG_SELECT )
  {
    selected = s;
  }
  return s;
}


uint8_t el_strlist_first = 0;
uint8_t el_strlist_cnt = 5;


M2_STRLIST(el_strlist, "l2w90", &el_strlist_first, &el_strlist_cnt, el_strlist_getstr);
M2_SPACE(el_space, "w1h1");
M2_VSB(el_strlist_vsb, "l2w5r1", &el_strlist_first, &el_strlist_cnt);
M2_LIST(list_strlist) = { &el_strlist, &el_space, &el_strlist_vsb };
M2_HLIST(el_strlist_hlist, NULL, list_strlist);

M2_LABEL(el_label,NULL, "Selected:");
M2_LABELPTR(el_labelptr,NULL, &selected);
M2_LIST(list_label) = { &el_label, &el_labelptr };
M2_HLIST(el_label_hlist, NULL, list_label);

M2_LIST(list) = { &el_strlist_hlist, &el_label_hlist };
M2_VLIST(el_vlist, NULL, list);
M2_ALIGN(top_el, "-1|1W64H64", &el_vlist);

M2tk m2(&top_el, m2_es_arduino, m2_eh_4bs, m2_gh_dogm_fb);

void setup() {
  m2.setFont(0, font_6x10);
  m2.setPin(M2_KEY_SELECT, uiKeySelectPin);
  m2.setPin(M2_KEY_NEXT, uiKeyDownPin);
  m2.setPin(M2_KEY_PREV, uiKeyUpPin);
}

void loop() {
  m2.checkKey();
  if ( m2.handleKey() ) {
    dogm.start();
    do{
      m2.checkKey();
      m2.draw();
    } while( dogm.next() );
  }
}


