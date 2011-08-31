/*

  SensorValues.pde

  Provide two alternative ways to show three sensor values.

  GLCD Example

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

/*==========================================================================*/
/* Includes and menu key definition */
/*==========================================================================*/

#include <glcd.h>		// inform Arduino IDE that we will use GLCD library
#include "M2tk.h"
#include "m2ghglcd.h"

uint8_t uiKeySelectPin = 3;
uint8_t uiKeyDownPin = 2;
uint8_t uiKeyUpPin = 1;
uint8_t uiKeyExitPin = 0;

/*==========================================================================*/
/* Sensor values: Will be set by some measurement processes  */
/*==========================================================================*/

uint32_t value1 = 1;
uint32_t value2 = 2;
uint32_t value3 = 3;

/*==========================================================================*/
/* Active sensor: Output variable by the menu, shows active dialog box  */
/*==========================================================================*/
/* 0: no sensor dialog open */
/* 1: sensor value 1 is on screen */
/* 2: sensor value 2 is on screen */
/* 3: sensor value 3 is on screen */
uint8_t active_sensor = 0; 

/*==========================================================================*/
/* is_value_changed: Output variable by the sensor process */
/*==========================================================================*/
/* 0: value did not change */
/* 1: value has changed */
uint8_t is_value_changed = 0; 

/*==========================================================================*/
/* Forward declaration of the menu class (required for the callback functions)  */
/*==========================================================================*/
extern M2tk m2;

/*==========================================================================*/
/* Option 1:  Hierachical menu, go back button for each sensor value dialog */
/*==========================================================================*/

/*=== Forward declaration of the root menu for option 1 ===*/ 
M2_EXTERN_VLIST(el_o1_vlist);

/*=== "Go Back" button, which will be reused for option 1 example menu ===*/ 

void fn_o1_go_back(m2_el_fnarg_p fnarg) { 
  active_sensor = 0; 
  m2.setRoot(&el_o1_vlist); 
}
M2_BUTTON(el_o1_go_back, NULL, "Go Back", fn_o1_go_back);

/*=== A simple dialog box, which shows the value ===*/ 
/*
  +==============+
  | Description: |
  | 0000.0       |
  | [Go Back]    |
  +==============+
  three menus will be defined, one for each sensor value 
*/

M2_LABEL(el_o1_v1_desc, NULL, "Sensor 1:");
M2_U32NUM(el_o1_v1_num, ".1c4r1", &value1);
M2_LIST(list_o1_v1) = { &el_o1_v1_desc, &el_o1_v1_num, &el_o1_go_back};
M2_VLIST(el_o1_v1_vlist, NULL, list_o1_v1);

M2_LABEL(el_o1_v2_desc, NULL, "Sensor 2:");
M2_U32NUM(el_o1_v2_num, ".1c5r2", &value2);
M2_LIST(list_o1_v2) = { &el_o1_v2_desc, &el_o1_v2_num, &el_o1_go_back};
M2_VLIST(el_o1_v2_vlist, NULL, list_o1_v2);

M2_LABEL(el_o1_v3_desc, NULL, "Sensor 3:");
M2_U32NUM(el_o1_v3_num, ".1c6r3", &value3);
M2_LIST(list_o1_v3) = { &el_o1_v3_desc, &el_o1_v3_num, &el_o1_go_back};
M2_VLIST(el_o1_v3_vlist, NULL, list_o1_v3);

/*=== A parent selection box allows to select one of the sensor values ===*/ 

void fn_o1_select1(m2_el_fnarg_p fnarg) { 
  active_sensor = 1; 
  m2.setRoot(&el_o1_v1_vlist); 
}
M2_BUTTON(el_o1_select1, NULL, "View Sensor 1", fn_o1_select1);
void fn_o1_select2(m2_el_fnarg_p fnarg) { 
  active_sensor = 2; 
  m2.setRoot(&el_o1_v2_vlist); 
}
M2_BUTTON(el_o1_select2, NULL, "View Sensor 2", fn_o1_select2);
void fn_o1_select3(m2_el_fnarg_p fnarg) { 
  active_sensor = 3; 
  m2.setRoot(&el_o1_v3_vlist); 
}
M2_BUTTON(el_o1_select3, NULL, "View Sensor 3", fn_o1_select3);
M2_LIST(list_o1) = { &el_o1_select1, &el_o1_select2, &el_o1_select3 };
M2_VLIST(el_o1_vlist, NULL, list_o1);

/*==========================================================================*/
/* Option 2: Each sensor dialog box can jump to any other sensor value */
/*==========================================================================*/
  
/*=== Each dialog box has a jumb bar at the bottom ===*/ 
/*
  +========================+
  | Sensor 2:              |
  | 0000.0                 |
  | [Sensor 1] [Sensor 3]  |
  +========================+
  three menus will be defined, one for each sensor value 
*/

/*=== Forward declaration of the three sensor dialog boxes ===*/ 
M2_EXTERN_VLIST(el_o2_dlg_v1);
M2_EXTERN_VLIST(el_o2_dlg_v2);
M2_EXTERN_VLIST(el_o2_dlg_v3);

/*=== Three buttons for the sensor menues, which will be reused for option 2 example menu ===*/ 
M2_ROOT(el_o2_goto_v1, NULL, "Sensor 1", &el_o2_dlg_v1);
M2_ROOT(el_o2_goto_v2, NULL, "Sensor 2", &el_o2_dlg_v2);
M2_ROOT(el_o2_goto_v3, NULL, "Sensor 3", &el_o2_dlg_v3);


/*=== Sensor 1 dialog box ===*/ 
M2_LABEL(el_o2_v1_desc, NULL, "Sensor 1:");
M2_U32NUM(el_o2_v1_num, ".1c4r1", &value1);
M2_LIST(list_o2_v1_menu) = { &el_o2_goto_v2, &el_o2_goto_v3 };
M2_HLIST(el_o2_v1_menu, NULL, list_o2_v1_menu);
M2_LIST(list_o2_v1) = { &el_o2_v1_desc, &el_o2_v1_num, &el_o2_v1_menu };
M2_VLIST(el_o2_dlg_v1, NULL, list_o2_v1);

/*=== Sensor 2 dialog box ===*/ 
M2_LABEL(el_o2_v2_desc, NULL, "Sensor 2:");
M2_U32NUM(el_o2_v2_num, ".1c5r2", &value2);
M2_LIST(list_o2_v2_menu) = { &el_o2_goto_v1, &el_o2_goto_v3 };
M2_HLIST(el_o2_v2_menu, NULL, list_o2_v2_menu);
M2_LIST(list_o2_v2) = { &el_o2_v2_desc, &el_o2_v2_num, &el_o2_v2_menu };
M2_VLIST(el_o2_dlg_v2, NULL, list_o2_v2);

/*=== Sensor 3 dialog box ===*/ 
M2_LABEL(el_o2_v3_desc, NULL, "Sensor 3:");
M2_U32NUM(el_o2_v3_num, ".1c6r3", &value3);
M2_LIST(list_o2_v3_menu) = { &el_o2_goto_v1, &el_o2_goto_v2 };
M2_HLIST(el_o2_v3_menu, NULL, list_o2_v3_menu);
M2_LIST(list_o2_v3) = { &el_o2_v3_desc, &el_o2_v3_num, &el_o2_v3_menu };
M2_VLIST(el_o2_dlg_v3, NULL, list_o2_v3);

/*==========================================================================*/
/* Option 3: Use a combo box to select the sensor dialog box */
/*==========================================================================*/

/* submenues are identical to option 1 */

/*=== Forward declaration of the root menu for option 3 ===*/ 
M2_EXTERN_VLIST(el_o3_vlist);

/*=== "Go Back" button, which will be reused for option 3 example menu ===*/ 

void fn_o3_go_back(m2_el_fnarg_p fnarg) { 
  active_sensor = 0; 
  m2.setRoot(&el_o3_vlist); 
}
M2_BUTTON(el_o3_go_back, NULL, "Go Back", fn_o3_go_back);

/*=== A simple dialog box, which shows the value ===*/ 
/*
  +==============+
  | Description: |
  | 0000.0       |
  | [Go Back]    |
  +==============+
  three menus will be defined, one for each sensor value 
*/

M2_LABEL(el_o3_v1_desc, NULL, "Sensor 1:");
M2_U32NUM(el_o3_v1_num, ".1c4r1", &value1);
M2_LIST(list_o3_v1) = { &el_o3_v1_desc, &el_o3_v1_num, &el_o3_go_back};
M2_VLIST(el_o3_v1_vlist, NULL, list_o3_v1);

M2_LABEL(el_o3_v2_desc, NULL, "Sensor 2:");
M2_U32NUM(el_o3_v2_num, ".1c5r2", &value2);
M2_LIST(list_o3_v2) = { &el_o3_v2_desc, &el_o3_v2_num, &el_o3_go_back};
M2_VLIST(el_o3_v2_vlist, NULL, list_o3_v2);

M2_LABEL(el_o3_v3_desc, NULL, "Sensor 3:");
M2_U32NUM(el_o3_v3_num, ".1c6r3", &value3);
M2_LIST(list_o3_v3) = { &el_o3_v3_desc, &el_o3_v3_num, &el_o3_go_back};
M2_VLIST(el_o3_v3_vlist, NULL, list_o3_v3);

/*=== Combo box for the sub menu selection ===*/ 

uint8_t o3_submenu = 0;
const char *fn_o3_submenu(uint8_t idx)
{
  if ( idx == 0 )
    return "Sensor 1";
  else if (idx == 1 )
    return "Sensor 2";
  return "Sensor 3";
}
M2_COMBO(el_o3_combo, NULL, &o3_submenu, 3, fn_o3_submenu);
void fn_o3_ok(m2_el_fnarg_p fnarg) { 
  switch( o3_submenu )
  {
    case 0:
      active_sensor = 1; 
      m2.setRoot(&el_o3_v1_vlist); 
      break;
    case 1:
      active_sensor = 2; 
      m2.setRoot(&el_o3_v2_vlist); 
      break;
    case 2:
      active_sensor = 3; 
      m2.setRoot(&el_o3_v3_vlist); 
      break;
  }
}
M2_BUTTON(el_o3_ok, NULL, " ok ", fn_o3_ok);
M2_LIST(list_o3) = { &el_o3_combo, &el_o3_ok};
M2_VLIST(el_o3_vlist, NULL, list_o3);



/*==========================================================================*/
/* Overall menu to select between the two example options */
/*==========================================================================*/

M2_ROOT(el_goto_o1, NULL, "Option 1: Parent Menu", &el_o1_vlist);
void fn_o2_start(m2_el_fnarg_p fnarg) { 
  active_sensor = 1; 
  m2.setRoot(&el_o2_dlg_v1); 
}
M2_BUTTON(el_goto_o2, NULL, "Option 2: Menu Bar", fn_o2_start);
M2_ROOT(el_goto_o3, NULL, "Option 3: Combo", &el_o3_vlist);
M2_LIST(list_options) = { &el_goto_o1, &el_goto_o2, &el_goto_o3 };
M2_VLIST(el_options, NULL, list_options);

/*==========================================================================*/
/* Menu constructor */
/*==========================================================================*/

M2tk m2(&el_options, m2_es_arduino, m2_eh_2bs, m2_gh_glcd_ffs);

/*==========================================================================*/
/* Sensor process */
/*==========================================================================*/

/* sensor1 process assignes state of a flashing LED */

uint8_t led_state = 0;
uint32_t next_change = 0;
uint8_t is_flashing = 0;
uint8_t flash_delay = 7;

void sensor1_process(void) {
  // wait until timer as expired
  if ( next_change < millis() ) {
    if ( led_state == 0 ) {
      digitalWrite(13, HIGH);  // set the LED on    
      led_state = 1;
    } else {
      digitalWrite(13, LOW);   // set the LED off
      led_state = 0;
    }
    // wait for x*50ms, were x is between 0 and 9
    next_change = millis() + (flash_delay*50L);
  } 
  if ( led_state != value1 ) {
    value1 = led_state;
    is_value_changed = 1;
  }
}

/* sensor1 process does nothing */

void sensor2_process(void) {
}

/* sensor3 process assignes a 1/10 seconds since reset */

void sensor3_process(void) {
  uint32_t tmp;
  /* just assign the 1/10 seconds since last reset */
  tmp = millis();
  tmp /= 100;
  if ( value3 != tmp ) {
    value3 = tmp;
    is_value_changed = 1;
  }
}

/* reads active_sensor and writes values to "value1", "value2" or "value3" */
void sensor_process(void)
{
  switch(active_sensor) {
    case 1:
      sensor1_process();
      break;
    case 2:
      sensor2_process();
      break;
    case 3:
      sensor3_process();
      break;
    default:
      break;
  }
}

/*==========================================================================*/
/* Arduino setup & loop */
/*==========================================================================*/

void setup() {
  m2.setPin(M2_KEY_SELECT, uiKeySelectPin);
  m2.setPin(M2_KEY_NEXT, uiKeyDownPin);
  m2.setPin(M2_KEY_PREV, uiKeyUpPin);
  m2.setPin(M2_KEY_EXIT, uiKeyExitPin);  
}

void loop() {
  m2.checkKey();
  if ( m2.handleKey() || is_value_changed ) {
      m2.draw();
      is_value_changed = 0;
  }
  
  sensor_process();
}
