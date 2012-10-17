/*

  pwm_pin_list.c

  m2tklib - Mini Interative Interface Toolkit Library
  u8glib - Universal 8bit Graphics Library

  Copyright (C) 2012  olikraus@gmail.com

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
#include "u8g.h"
#include "m2.h"
#include "m2utl.h"
#include "m2ghu8g.h"

#if defined(__AVR__)
#include <avr/interrupt.h>
#include <avr/io.h>
#endif

//=======================================
void my_strcpy(char *s, const char *t) {
  while( (*s++ = *t++) != '\0' )
    ;
}


//=======================================
// utility procedure to convert values
void pwm_value_to_str(uint8_t value, char *dest) {
  switch(value) {
    case 0: my_strcpy(dest, "low"); break;
    case 1: my_strcpy(dest, "25%"); break;
    case 2: my_strcpy(dest, "50%"); break;
    case 3: my_strcpy(dest, "75%"); break;
    case 4: my_strcpy(dest, "high"); break;
  }
}

uint8_t pwm_value_to_analog(uint8_t value) {
  switch(value) {
    case 0: return 0;
    case 1: return 63;
    case 2: return 127;
    case 3: return 191;
  }
  return 255;
}

//=================================================
// forward declaration of the pin selection menu
M2_EXTERN_ALIGN(top_el_pin_list);

//=================================================
// array for the pwm information of all pwm pins 

// array with all the information
#define PWM_PIN_CNT 6
uint8_t pwm_duty_array[PWM_PIN_CNT] = { 0,0,0,0,0,0 };
uint8_t pwm_pin_array[PWM_PIN_CNT] = { 3,5,6,9,10,11 };

//=================================================
// variables for the user interface

// the current index contains the position within the array which gets presented to the user
uint8_t pwm_menu_current_index = 0;

// values for the menu; will be modified by the user
// transfer values from the array to these variables: pwm_prepare_user_input
// transfer values from these variables to the array: pwm_apply_user_input
uint8_t pwm_menu_duty = 0;
uint8_t pwm_menu_pin = 0;

//=================================================
// transfer procedure between menu variables and the array with pwm information

// get pin and value pair from the global array and store them in the menu variables
void pwm_prepare_user_input(void) {
  pwm_menu_duty = pwm_duty_array[pwm_menu_current_index];
  pwm_menu_pin = pwm_pin_array[pwm_menu_current_index];
}

// write user input back to the array and to the analog pin
void pwm_apply_user_input(void) {
  // write user input into array
  pwm_duty_array[pwm_menu_current_index] = pwm_menu_duty;
  pwm_pin_array[pwm_menu_current_index] = pwm_menu_pin;
  // apply user input to the hardware
  // analogWrite(pwm_menu_pin, pwm_value_to_analog(pwm_menu_value));
}

//=================================================
// the menu / user interface for one pwm pin

// this procedure is called by the "ok" button
void pwm_fn_ok(m2_el_fnarg_p fnarg) {
  // finish user entry
  pwm_apply_user_input();
  
  // go back to parent menu
  m2_SetRoot(&top_el_pin_list);
}

// this will return a user readable string for the internal value
const char *pwm_fn_duty(uint8_t idx) {
  static char buf[8];
  pwm_value_to_str(idx, buf);
  return buf;
}

// the arduino pin number is read only for the user (info field)
M2_LABEL(el_pwm_pin_label, NULL, "Pin:");
M2_U8NUM(el_pwm_pin_num, "r1c2", 0, 255, &pwm_menu_pin);

// the duty field can be changed by the user
M2_LABEL(el_pwm_duty_label, NULL, "Duty: ");
M2_COMBO(el_pwm_duty, NULL, &pwm_menu_duty, 5, pwm_fn_duty);

// cancel: do not store user values, go back directly
M2_ROOT(el_pwm_cancel, "f4", "Cancel", &top_el_pin_list);

// ok: write user values back to the array and apply values to the hardware
M2_BUTTON(el_pwm_ok, "f4", "Ok", pwm_fn_ok);

// the following grid-list will arrange the elements on the display
M2_LIST(list_pwm_menu) = { 
    &el_pwm_pin_label, &el_pwm_pin_num, 
    &el_pwm_duty_label, &el_pwm_duty,  
    &el_pwm_cancel, &el_pwm_ok 
};
M2_GRIDLIST(el_pwm_grid, "c2", list_pwm_menu);

// center the menu on the display
M2_ALIGN(el_top_pwm_menu, "-1|1W64H64", &el_pwm_grid);


//=================================================
// this menu selects one of the pwm pins (implemented with the STRLIST element)
uint8_t pin_list_first = 0;
uint8_t pin_list_cnt = PWM_PIN_CNT;

// callback procedure for the STRLIST element
const char *pin_list_cb(uint8_t idx, uint8_t msg) {
  static char s[12];
  s[0] = '\0';
  if ( msg == M2_STRLIST_MSG_SELECT ) {
    // the user has selected a pin, prepare sub menu
    
    // inform the pwm dialog which array index has to be modfied
    pwm_menu_current_index = idx;
    
    // transfer values from the array to the menu variables
    pwm_prepare_user_input();
    
    // give control to the pwm dialog
    m2_SetRoot(&el_top_pwm_menu);
  } else {
    // convert the idx into some readable line for the user
    my_strcpy(s, "Pin ");
    my_strcpy(s+4, m2_utl_u8d(pwm_pin_array[idx], 2));
    // add a space
    s[6] = ' ';
    // also show the current duty
    pwm_value_to_str(pwm_duty_array[idx], s+7);
  }
  return s;
}

// selection menu for the pins, composed of a STRLIST element with a scroll bar
M2_STRLIST(el_pin_list_strlist, "l3W56", &pin_list_first, &pin_list_cnt, pin_list_cb);
M2_SPACE(el_pin_list_space, "W1h1");
M2_VSB(el_pin_list_vsb, "l3W4r1", &pin_list_first, &pin_list_cnt);
M2_LIST(list_pin_list) = { &el_pin_list_strlist, &el_pin_list_space, &el_pin_list_vsb };
M2_HLIST(el_pin_list_hlist, NULL, list_pin_list);
M2_ALIGN(top_el_pin_list, "-1|1W64H64", &el_pin_list_hlist);



/*=========================================================================*/
/* global variables and objects */

u8g_t u8g;

/*=========================================================================*/
/* controller, u8g and m2 setup */

void setup(void)
{  
  /*
    Test Envionment, ATMEGA with the following settings:
    CS: PORTB, Bit 2
    A0: PORTB, Bit 1
    SCK: PORTB, Bit 5
    MOSI: PORTB, Bit 3
  */
  /* 1. Setup and create u8g device: http://code.google.com/p/u8glib/wiki/avr#U8glib_Init */
  /* u8g_InitSPI(&u8g, &u8g_dev_st7565_dogm132_sw_spi, PN(1, 5), PN(1, 3), PN(1, 2), PN(1, 1), U8G_PIN_NONE); */
  u8g_InitHWSPI(&u8g, &u8g_dev_st7565_dogm132_hw_spi, PN(1, 2), PN(1, 1), U8G_PIN_NONE);

  /* 2. Setup m2 */
  m2_Init(&top_el_pin_list, m2_es_avr_u8g, m2_eh_4bs, m2_gh_u8g_bfs);
  //m2_Init(&top_el_pin_list, m2_es_avr_rotary_encoder_u8g, m2_eh_4bd, m2_gh_u8g_bfs);

  /* 3. Connect u8g display to m2  */
  m2_SetU8g(&u8g, m2_u8g_box_icon);

  /* 4. Set a font, use normal u8g_font's */
  m2_SetFont(0, (const void *)u8g_font_5x8r);
	
  /* 5. Define keys (PN() description, see http://code.google.com/p/u8glib/wiki/avr#U8glib_Init) */
  m2_SetPin(M2_KEY_EXIT, PN(3, 5));
  m2_SetPin(M2_KEY_SELECT, PN(3, 6));
  m2_SetPin(M2_KEY_NEXT, PN(3, 7));
  m2_SetPin(M2_KEY_PREV, PN(1, 7));
}

/*=========================================================================*/
/* system setup */

void sys_init(void)
{
#if defined(__AVR__)
  /* select minimal prescaler (max system speed) */
  CLKPR = 0x80;
  CLKPR = 0x00;
#endif
}

/*=========================================================================*/
/* u8g draw procedure (body of picture loop) */

/* draw procedure of the u8g picture loop */
void draw(void)
{	
  /* call the m2 draw procedure */
  m2_Draw();
}

/*=========================================================================*/
/* main procedure with u8g picture loop */

int main(void)
{
  /* setup controller */
  sys_init();
	
  /* setup u8g and m2 libraries */
  setup();

  /* application main loop */
  for(;;)
  {  
    m2_CheckKey();
    if ( m2_HandleKey() ) 
    {
      /* picture loop */
      u8g_FirstPage(&u8g);
      do
      {
	draw();
        m2_CheckKey();
      } while( u8g_NextPage(&u8g) );
    }
  }  
}

