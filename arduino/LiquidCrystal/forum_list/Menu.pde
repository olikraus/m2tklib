//  Menu.pde
// http://arduino.cc/forum/index.php/topic,99693.15.html

#include <LiquidCrystal.h>
#include "M2tk.h"
//#include "utility/m2ghlc.h"
#include "m2ghlc.h"

//=========================================================
// fotward declaration
extern M2tk m2;

//=========================================================
// keys 
uint8_t uiKeySelectPin = 10;
uint8_t uiKeyNextPin = 9;

//=========================================================
// global variable: selected line
uint8_t choice = 0;

//=========================================================
// display result
M2_U8NUM(result_menu, NULL, 0, 255, &choice);

//=========================================================
// line selection

// callbacks for the buttons
void fn_line1(m2_el_fnarg_p fnarg) { choice = 0;  m2.setRoot(&result_menu); }
void fn_line2(m2_el_fnarg_p fnarg) { choice = 1;  m2.setRoot(&result_menu); }
void fn_line3(m2_el_fnarg_p fnarg) { choice = 2;  m2.setRoot(&result_menu); }
void fn_line4(m2_el_fnarg_p fnarg) { choice = 3;  m2.setRoot(&result_menu); }

// layout of the menu
M2_BUTTON(el_line1, NULL, "Line 1", fn_line1);
M2_BUTTON(el_line2, NULL, "Line 2", fn_line2);
M2_BUTTON(el_line3, NULL, "Line 3", fn_line3);
M2_BUTTON(el_line4, NULL, "Line 4", fn_line4);
M2_LIST(list_lines) = { &el_line1, &el_line2, &el_line3, &el_line4 };
M2_VLIST(line_menu, NULL, list_lines);

//=========================================================
// lcd setup
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//=========================================================
// M2tklib setup
// m2_es_arduino: Arduino event source
// m2_eh_2bs: Two button handler
// m2_gh_lc: Character subsystem
M2tk m2(&line_menu, m2_es_arduino, m2_eh_2bs, m2_gh_lc);


//=========================================================
// Arduino setup()
void setup() {
  // Connect M2tklib to LCD 
  m2_SetLiquidCrystal(&lcd, 16, 4);
  
  // Apply keys
  m2.setPin(M2_KEY_SELECT, uiKeySelectPin);
  m2.setPin(M2_KEY_NEXT, uiKeyNextPin);
}

//=========================================================
// Arduino loop()
void loop() {
  m2.checkKey();
  if ( m2.handleKey() )
    m2.draw();
  m2.checkKey();
}

