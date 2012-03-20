/*

  HelloWorld.pde
  
  DogLcd 16x2 example

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

#include <DogLcd.h>
#include <DS1307new.h>
#include "M2tk.h"
#include "m2utl.h"
#include "m2ghdoglcd.h"

/*=========================================================================*/
/* dog lcd object definition for botmat */

DogLcd lcd(5, 7, 29, 1, -1, 3);

/*=========================================================================*/
/* pin numbers of the keypad */

uint8_t uiKeySelectPin = 18;
uint8_t uiKeyUpPin = 22;
uint8_t uiKeyDownPin = 19;
uint8_t uiKeyLeftPin = 20;
uint8_t uiKeyRightPin = 21;

/*=========================================================================*/
/* forward declarations */

extern M2tk m2;
M2_EXTERN_HLIST(el_top);

/*=========================================================================*/
/* 
  info screens 
  0: millis()
  1: voltage (adc0)
  2: time
*/


uint8_t info_screen_state = 0;

void info_screen_display(void)
{
  if ( info_screen_state == 0 )
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(millis());
    delay(20);
  }
  else if ( info_screen_state == 1 )
  {
    uint16_t adc = analogRead(0);
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("raw:  ");
    lcd.print(adc);
    lcd.setCursor(0,1);
    lcd.print("volt: ");
    lcd.print((adc*3.3)/512.0);
    delay(50);
  }
  else if ( info_screen_state == 2 )
  {
    RTC.getTime();
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(m2_utl_u8d(RTC.hour,2));
    lcd.print(":");
    lcd.print(m2_utl_u8d(RTC.minute,2));
    lcd.print(":");
    lcd.print(m2_utl_u8d(RTC.second,2));
    lcd.setCursor(0,1);
    lcd.print(m2_utl_u8d(RTC.day,2));
    lcd.print(".");
    lcd.print(m2_utl_u8d(RTC.month,2));
    lcd.print(".");
    lcd.print(m2_utl_u8d(RTC.year-2000,2));
    
    delay(50);
  }

}


/*=========================================================================*/
uint8_t td_hour;
uint8_t td_min;
uint8_t td_sec;

M2_U8NUM(el_td_hour, "c2", 0,23,&td_hour);
M2_LABEL(el_td_sep1, NULL, ":");
M2_U8NUM(el_td_min, "c2", 0,59,&td_min);
M2_LABEL(el_td_sep2, NULL, ":");
M2_U8NUM(el_td_sec, "c2", 0,59,&td_sec);

M2_LIST(list_time) = { &el_td_hour, &el_td_sep1, &el_td_min, &el_td_sep2, &el_td_sec };
M2_HLIST(el_time, NULL, list_time);

M2_ROOT(el_td_cancel, NULL, "cancel", &el_top);
M2_LIST(list_td) = {&el_time, &el_td_cancel };
M2_VLIST(el_top_td, NULL, list_td);

/*=========================================================================*/

const char *el_strlist_getstr(uint8_t idx, uint8_t msg) {
  const char *s = "";
  if  ( idx == 0 )
    s = "show millis";
  else if ( idx == 1 )
    s = "show volt";
  else if ( idx == 2 )
    s = "show time";
  else if ( idx == 3 )
    s = "set time";
  else if ( idx == 4 )
    s = "Corn";
  if (msg == M2_STRLIST_MSG_GET_STR) 
  {
    /* nothing else todo, return the correct string */
  } 
  else 
  if ( msg == M2_STRLIST_MSG_SELECT ) 
  {
    if ( idx <= 2 )
    {
      info_screen_state = idx;   
      m2.setRoot(&m2_null_element);      
    }
    if ( idx == 3 )
    {
      m2.setRoot(&el_top_td);      
    }
  }
  return s;
}

uint8_t el_strlist_first = 0;
uint8_t el_strlist_cnt = 5;

M2_STRLIST(el_strlist, "l2w13", &el_strlist_first, &el_strlist_cnt, el_strlist_getstr);
//M2_SPACE(el_space, "w1h1");
M2_VSB(el_strlist_vsb, "l2w1r1", &el_strlist_first, &el_strlist_cnt);
M2_LIST(list_strlist) = { &el_strlist, &el_strlist_vsb };
M2_HLIST(el_top, NULL, list_strlist);


M2_LABEL(hello_world_label, NULL, "Hello World!");
M2tk m2(&el_top, m2_es_arduino, m2_eh_6bs, m2_gh_doglcd);



/*=========================================================================*/
/* arduino entry points */

void setup() 
{
  // 1. Assign the DogLcd object to m2tklib
  m2_SetDogLcd(&lcd, DOG_LCD_M162,0x25,DOG_LCD_VCC_3V3);
  // 2. Set the backlight (sequence is important!) 
  lcd.setBacklight(1, 0);
  // 3. finish setup for m2tk
  m2.setPin(M2_KEY_SELECT, uiKeySelectPin);
  m2.setPin(M2_KEY_NEXT, uiKeyRightPin);
  m2.setPin(M2_KEY_PREV, uiKeyLeftPin);
  m2.setPin(M2_KEY_DATA_UP, uiKeyUpPin);
  m2.setPin(M2_KEY_DATA_DOWN, uiKeyDownPin);

}

void loop() {
  if ( m2.getRoot() == &m2_null_element ) 
  {
    info_screen_display();
    if ( m2.getKey() != M2_KEY_NONE )
      m2.setRoot(&el_top);
  } 
 
  m2.checkKey();
  if ( m2.handleKey() )
    m2.draw();
  m2.checkKey();
}


