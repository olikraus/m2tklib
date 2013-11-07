/*

  Combo.pde
  
  LiquidCrystal 16x4 example

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

#include <LiquidCrystal.h>
#include "M2tk.h"
#include "m2ghlc.h"

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

uint8_t uiKeySelectPin = 10;
uint8_t uiKeyNextPin = 9;

/*======================================================================*/
/* forward declaration */
extern M2tk m2;
M2_EXTERN_ALIGN(el_top);

/*======================================================================*/
/* combo */


uint8_t select_color = 0;
uint8_t select_priority = 0;

void fn_ok(m2_el_fnarg_p fnarg) {
  /* accept selection */
}

void fn_cancel(m2_el_fnarg_p fnarg) {
  /* discard selection */
}

const char *fn_idx_to_color(uint8_t idx)
{
  if ( idx == 0 )
    return "red";
  else if (idx == 1 )
    return "green";
  return "blue";
}

const char *fn_idx_to_priority(uint8_t idx)
{
  switch(idx)
  {
    case 0: return "lowest";
    case 1: return "low";
    case 2: return "medium";
    case 3: return "high";
    case 4: return "highest";
  }
  return "";
}


M2_LABEL(el_label1, NULL, "Color:");
M2_COMBO(el_combo1, NULL, &select_color, 3, fn_idx_to_color);

M2_LABEL(el_label2, NULL, "Prio.: ");
M2_COMBO(el_combo2, "v1", &select_priority, 5, fn_idx_to_priority);

M2_BUTTON(el_cancel, NULL, "cancel", fn_cancel);
M2_BUTTON(el_ok, NULL, " ok ", fn_ok);

M2_LIST(list) = { 
    &el_label1, &el_combo1, 
    &el_label2, &el_combo2,  
    &el_cancel, &el_ok 
};
M2_GRIDLIST(el_top_combo, "c2",list);


/*======================================================================*/
/* combofn */

uint8_t combofn_color = 0;
uint8_t combofn_priority = 0;

const char *combofn_color_cb(m2_rom_void_p element, uint8_t msg, uint8_t *valptr)
{
	switch(msg)
	{
		case M2_COMBOFN_MSG_GET_VALUE:
			*valptr = combofn_color;
			break;
		case M2_COMBOFN_MSG_SET_VALUE:
			combofn_color = *valptr;
			break;
		case M2_COMBOFN_MSG_GET_STRING:
			if ( *valptr == 0 )
				return "orange";
			else if (*valptr == 1 )
				return "yellow";
			return "cyan";
	}
	return NULL;
}

const char *combofn_priority_cb(m2_rom_void_p element, uint8_t msg, uint8_t *valptr)
{
	switch(msg)
	{
		case M2_COMBOFN_MSG_GET_VALUE:
			*valptr = combofn_priority;
			break;
		case M2_COMBOFN_MSG_SET_VALUE:
			combofn_priority = *valptr;
			break;
		case M2_COMBOFN_MSG_GET_STRING:
			switch(*valptr)
			{
				case 0: return "lowest";
				case 1: return "low";
				case 2: return "medium";
				case 3: return "high";
				case 4: return "highest";
			}
			return "";
	}
	return NULL;
}

M2_LABEL(el_labelcombofn1, NULL, "Color:");
M2_COMBOFN(el_combofn1, NULL, 3, combofn_color_cb);

M2_LABEL(el_labelcombofn2, NULL, "Priority: ");
M2_COMBOFN(el_combofn2, "v1", 5, combofn_priority_cb);

M2_LIST(list_combofn) = { 
    &el_labelcombofn1, &el_combofn1, 
    &el_labelcombofn2, &el_combofn2,  
    &el_cancel, &el_ok 
};
M2_GRIDLIST(el_top_combofn, "c2", list_combofn);

/*======================================================================*/
/* main menu */


M2_LABEL(el_goto_title, NULL, "Combo Example");
M2_ROOT(el_goto_part1, NULL, "COMBO", &el_top_combo);
M2_ROOT(el_goto_part2, NULL, "COMBOFN", &el_top_combofn);
M2_LIST(list_menu) = {&el_goto_title, &el_goto_part1, &el_goto_part2};
M2_VLIST(el_menu_vlist, NULL, list_menu);
M2_ALIGN(el_top, NULL, &el_menu_vlist);


M2tk m2(&el_top, m2_es_arduino, m2_eh_2bs, m2_gh_lc);

void setup() {
  m2_SetLiquidCrystal(&lcd, 16, 4);
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
