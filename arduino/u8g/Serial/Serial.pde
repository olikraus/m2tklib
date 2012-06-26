/*

  Serial.pde
  
  Simulate button input and output to 20x4 character display

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

#include "U8glib.h"		// ensure that the include path is set
#include "M2tk.h"


//=================================================
// Forward declaration of the toplevel element
M2_EXTERN_HLIST(top_el_expandable_menu);


/*======================================================================*/
/* number entry */

uint8_t u8num = 0;
uint32_t u32num = 0;

void fn_num_zero(m2_el_fnarg_p fnarg) {
  u8num = 0;
  u32num = 0;
}

M2_LABEL(el_num_label1, NULL, "U8:");
M2_U8NUM(el_num_1, NULL, 0, 255, &u8num);

M2_LABEL(el_num_label2, NULL, "U32:");
M2_U32NUM(el_num_2, "c5", &u32num);

M2_BUTTON(el_num_zero, "f4", " zero ", fn_num_zero);
M2_ROOT(el_num_goto_top, "f4", " back ", &top_el_expandable_menu);

M2_LIST(num_list) = { 
    &el_num_label1, &el_num_1, 
    &el_num_label2, &el_num_2,  
    &el_num_zero, &el_num_goto_top
};
M2_GRIDLIST(el_num_menu, "c2", num_list);

/*=========================================================================*/
/* edit date dialog */

uint8_t dt_day = 1;
uint8_t dt_month = 1;
uint8_t dt_year = 12;

void dt_ok_fn(m2_el_fnarg_p fnarg)  {
  m2_SetRoot(&top_el_expandable_menu);
}

M2_U8NUM(el_dt_year, "c2", 0,99,&dt_year);
M2_LABEL(el_dt_sep1, "b1", "-");
M2_U8NUM(el_dt_month, "c2", 1,12,&dt_month);
M2_LABEL(el_dt_sep2, "b1", "-");
M2_U8NUM(el_dt_day, "c2", 1,31,&dt_day);

M2_LIST(list_date) = { &el_dt_year, &el_dt_sep1, &el_dt_month, &el_dt_sep2, &el_dt_day };
M2_HLIST(el_date, NULL, list_date);

M2_ROOT(el_dt_cancel, NULL, "cancel", &top_el_expandable_menu);
M2_BUTTON(el_dt_ok, NULL, "ok", dt_ok_fn);
M2_LIST(list_dt_buttons) = {&el_dt_cancel, &el_dt_ok };
M2_HLIST(el_dt_buttons, NULL, list_dt_buttons);

M2_LIST(list_dt) = {&el_date, &el_dt_buttons };
M2_VLIST(el_top_dt, NULL, list_dt);

/*=========================================================================*/
/* radio */

uint8_t select_color = 0;

void fn_radio_ok(m2_el_fnarg_p fnarg) {
  /* accept selection */
  m2_SetRoot(&top_el_expandable_menu);
}

void fn_radio_cancel(m2_el_fnarg_p fnarg) {
  /* discard selection */
  m2_SetRoot(&top_el_expandable_menu);
}

M2_LABEL(el_radio_label1, NULL, "red");
M2_RADIO(el_radio_radio1, "v0", &select_color);

M2_LABEL(el_radio_label2, NULL, "green");
M2_RADIO(el_radio_radio2, "v1", &select_color);

M2_LABEL(el_radio_label3, NULL, "blue");
M2_RADIO(el_radio_radio3, "v2", &select_color);

M2_BUTTON(el_radio_cancel, NULL, "cancel", fn_radio_cancel);
M2_BUTTON(el_radio_ok, NULL, "ok", fn_radio_ok);

M2_LIST(list_radio) = { 
    &el_radio_label1, &el_radio_radio1, 
    &el_radio_label2, &el_radio_radio2,  
    &el_radio_label3, &el_radio_radio3, 
    &el_radio_cancel, &el_radio_ok 
};
M2_GRIDLIST(top_el_radio, "c2",list_radio);

/*=========================================================================*/
/* combo */

uint8_t select_priority = 0;

void fn_combo_ok(m2_el_fnarg_p fnarg) {
  /* accept selection */
  m2_SetRoot(&top_el_expandable_menu);
}

void fn_combo_cancel(m2_el_fnarg_p fnarg) {
  /* discard selection */
  m2_SetRoot(&top_el_expandable_menu);
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


M2_LABEL(el_combo_label1, NULL, "Color:");
M2_COMBO(el_combo_combo1, NULL, &select_color, 3, fn_idx_to_color);

M2_LABEL(el_combo_label2, NULL, "Prio.: ");
M2_COMBO(el_combo_combo2, "v1", &select_priority, 5, fn_idx_to_priority);

M2_BUTTON(el_combo_cancel, NULL, "cancel", fn_combo_cancel);
M2_BUTTON(el_combo_ok, NULL, " ok ", fn_combo_ok);

M2_LIST(list_combo) = { 
    &el_combo_label1, &el_combo_combo1, 
    &el_combo_label2, &el_combo_combo2,  
    &el_combo_cancel, &el_combo_ok 
};
M2_GRIDLIST(top_el_combo, "c2",list_combo);

/*=========================================================================*/
/* main menu */


// Left entry: Menu name. Submenus must have a '.' at the beginning
// Right entry: Reference to the target dialog box (In this example all menus call the toplevel element again
m2_menu_entry m2_2lmenu_data[] = 
{
  { "Numeric", NULL },
  { ". U8 U32", &el_num_menu },
  { ". Date", &el_top_dt },
  { "Radio", &top_el_radio },
  { "Combo", &top_el_combo },
  { NULL, NULL },
};

// The first visible line and the total number of visible lines.
// Both values are written by M2_2LMENU and read by M2_VSB
uint8_t m2_2lmenu_first;
uint8_t m2_2lmenu_cnt;

// M2_2LMENU definition
// Option l4 = four visible lines
// Option e1 = first column has a width of 1 char
// Option w12 = second column has a width of 12 chars

M2_2LMENU(el_2lmenu,"l4e1w12",&m2_2lmenu_first,&m2_2lmenu_cnt, m2_2lmenu_data,'+','-','\0');
M2_VSB(el_vsb, "l4w1r1", &m2_2lmenu_first, &m2_2lmenu_cnt);
M2_LIST(list_2lmenu) = { &el_2lmenu, &el_vsb };
M2_HLIST(top_el_expandable_menu, NULL, list_2lmenu);

// m2 object and constructor
M2tk m2(&top_el_expandable_menu, m2_es_arduino_serial, m2_eh_4bs, m2_gh_arduino_serial);

void setup() {
}

void loop() {
  m2.checkKey();
  m2.checkKey();
  if ( m2.handleKey() )
    m2.draw();
  m2.checkKey();
}
