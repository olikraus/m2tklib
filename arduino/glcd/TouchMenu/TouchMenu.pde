/*

  TouchMenu.pde

  Example for touch panel
  
  THIS EXAMPLE IS INCOMPELTE: CODE FOR TOUCH PANEL IS NOT INCLUDED. 
  
  GLCD Example

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

#include <glcd.h>		// inform Arduino IDE that we will use GLCD library
#include "M2tk.h"
#include "m2ghglcd.h"

//================================================================
// touch panel procedures

uint8_t is_touch_panel_pressed() {
  // add code here
  // return 1 if the touch panel is pressed
  return 0;
}

uint8_t get_touch_panel_x() {
  // add code here  
  // return x position between 0 and display width-1
  return 0;
}

uint8_t get_touch_panel_y() {
  // add code here
  // return y position between 0 and display height-1
  return 0;
}


//================================================================
// m2tklib forward declarations

extern M2tk m2;
M2_EXTERN_ALIGN(top_el_ts_mnu);

//================================================================

// note: for all selectable elements the read only flag is set, so that the element focus only appears for the
// touch panel focus.

M2_ROOT(el_ts_mnu1_sel, "r1t1", "Menu 1 selected", &top_el_ts_mnu);
M2_ALIGN(top_el_ts_mnu1_sel, "-1|1W64H64", &el_ts_mnu1_sel);
M2_ROOT(el_ts_mnu2_sel, "r1t1", "Menu 2 selected", &top_el_ts_mnu);
M2_ALIGN(top_el_ts_mnu2_sel, "-1|1W64H64", &el_ts_mnu2_sel);
M2_ROOT(el_ts_mnu3_sel, "r1t1", "Menu 3 selected", &top_el_ts_mnu);
M2_ALIGN(top_el_ts_mnu3_sel, "-1|1W64H64", &el_ts_mnu3_sel);


M2_ROOT(el_ts_mnu1, "r1t1w60f8", "menu 1", &top_el_ts_mnu1_sel);
M2_ROOT(el_ts_mnu2, "r1t1w60f8", "menu 2", &top_el_ts_mnu2_sel);
M2_ROOT(el_ts_mnu3, "r1t1w60f8", "menu 3", &top_el_ts_mnu3_sel);

M2_LIST(list_ts_mnu) = { 
    &el_ts_mnu1, 
    &el_ts_mnu2, 
    &el_ts_mnu3
};

M2_VLIST(el_ts_mnu_vlist, NULL, list_ts_mnu);
M2_ALIGN(top_el_ts_mnu, "-1|1W64H64", &el_ts_mnu_vlist);

//================================================================
// new event source and m2tklib constructor
uint8_t m2_es_touch_screen(m2_p ep, uint8_t msg) {
  switch(msg) {
    case M2_ES_MSG_GET_KEY:
      if ( is_touch_panel_pressed()  ) {
	/* set x and y position of the touch screen */
	/* (x,y) is a M2 position: (0,0) is lower left */
	m2_SetEventSourceArgsM2(ep, get_touch_panel_x(), get_touch_panel_y() );
	/* no debounce: return M2_KEY_EVENT(M2_KEY_TOUCH_PRESS); */
	/* with debounce: return M2_KEY_TOUCH_PRESS; */
	return M2_KEY_EVENT(M2_KEY_TOUCH_PRESS);
      }
      break;      
    case M2_ES_MSG_INIT:
      break;
  }
  /* return 0 or call other event source */
  return m2_es_arduino(ep, msg);  
}

M2tk m2(&top_el_ts_mnu, m2_es_touch_screen, m2_eh_6bsts, m2_gh_glcd_ffs);

//================================================================
// overall draw procedure for u8glib, includes touch panel calibration

void draw(void) {
  m2.draw();
}

//================================================================
// Arduino setup and loop

void setup() {
}

void loop() {
  m2.checkKey();
  if ( m2.handleKey() ) {
      m2.draw();
  }
}

