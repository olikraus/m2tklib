/*

  Example.pde

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


  SCL (SPI Clock)   Pin 13
  SI (MOSI)         Pin 11
  CS (Chip Select)  Pin 10
  MISO (Pin 12) is not used, but can not be reused as generic I/O
  
  Note:
    1) Set correct display hardware in Dogm/utility/dogm128.h
    2) Set top/bottom view (DOG_REVERSE) in Dogm/utility/dogm128.h
    See also: http://code.google.com/p/dogm128/wiki/install 

  User Interface:
    Analog Potentiometer: 
      Assign pin number to variable "sensorPin". Default is analog pin 0.
    Digital Input:
      Assign up button pin to variable uiKeyUpPin. Default is DOGS102 Shield
      Assign down button pin to variable uiKeyDownPin. Default is DOGS102 Shield
      Assign fire button pin to variable uiKeySelectPin. Default is DOGS102 Shield
	  Remember to define DOGS102_HW
    additionally another button set can be define
      Assign up button pin to variable uiKeyUpPin2. Default is DOGM132 Shield
      Assign down button pin to variable uiKeyDownPin2. Default is DOGM132 Shield
      Assign fire button pin to variable uiKeySelectPin2. Default is DOGM132 Shield
	  Remember to define DOGM132_HW and DOG_REVERSE
  
*/

#include "Dogm.h"
#include "m2.h"
#include "m2ghdogm.h"

int a0Pin = 9;      // address line a0 for the dogm module
uint16_t sensorPin = 0;  // analog input


// frames per second
unsigned long next_sec_time;
uint8_t fps, frame_cnt;

// DOGS102 configuration values
uint8_t uiKeyUpPin = 5;
uint8_t uiKeyDownPin = 3;
uint8_t uiKeySelectPin = 4;

// DOGM132 configuration values
uint8_t uiKeyUpPin2 = 7;
uint8_t uiKeyDownPin2 = 3;
uint8_t uiKeySelectPin2 = 2;


// initialize the dogm library
Dogm dogm(a0Pin);

// setup the user interface
void uiSetup(void) {
  // configure input keys 
  
  pinMode(uiKeyUpPin, INPUT);           // set pin to input
  digitalWrite(uiKeyUpPin, HIGH);       // turn on pullup resistors
  pinMode(uiKeyDownPin, INPUT);           // set pin to input
  digitalWrite(uiKeyDownPin, HIGH);       // turn on pullup resistors
  pinMode(uiKeySelectPin, INPUT);           // set pin to input
  digitalWrite(uiKeySelectPin, HIGH);       // turn on pullup resistors

  pinMode(uiKeyUpPin2, INPUT);           // set pin to input
  digitalWrite(uiKeyUpPin2, HIGH);       // turn on pullup resistors
  pinMode(uiKeyDownPin2, INPUT);           // set pin to input
  digitalWrite(uiKeyDownPin2, HIGH);       // turn on pullup resistors
  pinMode(uiKeySelectPin2, INPUT);           // set pin to input
  digitalWrite(uiKeySelectPin2, HIGH);       // turn on pullup resistors
}

uint8_t uiGetKey(void)
{
  uint8_t local_key;
  if ( digitalRead(uiKeyUpPin) == LOW || digitalRead(uiKeyUpPin2) == LOW )
    local_key = M2_KEY_PREV;
  else if ( digitalRead(uiKeyDownPin) == LOW || digitalRead(uiKeyDownPin2) == LOW )
    local_key = M2_KEY_NEXT;
  else if ( digitalRead(uiKeySelectPin) == LOW || digitalRead(uiKeySelectPin2) == LOW )
    local_key = M2_KEY_SELECT;
  else 
    local_key = M2_KEY_NONE;
  return local_key;
}

M2_EXTERN_ALIGN(top_el);
M2_ROOT(goto_top_el, "f4x0y0", "Home", &top_el);

/*==============================================================*/
/* radio box */

uint8_t radio_val = 0;

M2_LABEL(opt1_label, "", "Option 1:");
M2_LABEL(opt2_label, "", "Option 2:");
M2_LABEL(opt3_label, "", "Option 3:");

M2_RADIO(opt1_el,"v0", &radio_val);
M2_RADIO(opt2_el,"v1", &radio_val);
M2_RADIO(opt3_el,"v2", &radio_val);

M2_LIST(radio_list) = { 
  &opt1_label, &opt1_el, 
  &opt2_label, &opt2_el, 
  &opt3_label, &opt3_el,
  &goto_top_el
};

M2_GRIDLIST(radio_el, "c2", radio_list);

/*==============================================================*/
/* rgb dialog box */

uint8_t red = 0;
uint8_t green = 0;
uint8_t blue = 0;

M2_LABEL(red_label, "", "Red:");
M2_LABEL(green_label, "", "Green:");
M2_LABEL(blue_label, "", "Blue:");

M2_U8NUM(red_el,"", 0, 99,&red);
M2_U8NUM(green_el,"", 0, 99,&green);
M2_U8NUM(blue_el,"", 0, 99,&blue);

M2_LIST(rgb_list) = { 
  &red_label, &red_el, 
  &green_label, &green_el, 
  &blue_label, &blue_el,
  &goto_top_el
};

M2_GRIDLIST(rgb_el, "c2", rgb_list);

/*==============================================================*/
/* xy with text field and u32 field */

#define NAME_LEN 10
char text[NAME_LEN+1] = "0123456789";
uint32_t value = 123456789L;

M2_LABEL(text_label, "x0y30", "Text:");
M2_TEXT(text_el, "x40y30", text, NAME_LEN);
M2_LABEL(value_label, "x0y15", "Value:");
M2_U32NUM(value_el, "x40y15c9", &value);

M2_LIST(xy_list) = { 
  &text_label, 
  &text_el, 
  &value_label, 
  &value_el, 
  &goto_top_el
};

M2_XYLIST(xy_el, "", xy_list);

/*==============================================================*/
/* top menu */


M2_ROOT(goto_radio_el, "","Select", &radio_el);
M2_ROOT(goto_rgb_el, "","rgb", &rgb_el);
M2_ROOT(goto_xy_el, "","Name", &xy_el);

  
M2_LIST(top_list) = { 
  &goto_radio_el,
  &goto_rgb_el,
  &goto_xy_el  
  };
M2_GRIDLIST(top_grid_el,"c1", top_list);
M2_ALIGN(top_el, "w128h64", &top_grid_el);

 
extern "C" uint8_t m2_es_arduino(m2_p ep, uint8_t msg)
{
  
  switch(msg)
  {
    case M2_ES_MSG_GET_KEY:
    {
      return uiGetKey();
    }
    case M2_ES_MSG_INIT:
      uiSetup();
      return 0;
  }
  return 0;
}


/*==============================================================*/


void setup() {
  
  uiSetup();
  m2_Init(&top_el, m2_es_arduino, m2_eh_2bs, m2_gh_dogm_fbs);	
  //m2_Init(&m2, &top_el, m2_es_arduino, m2_eh_2bs, m2_gh_dogm_box);	

  m2_SetFont(0, font_7x13);

  next_sec_time = millis() + 1000UL;
  fps = 0;
  frame_cnt = 0;
}

void loop() {
  dogm.showLibInfo();
  
  m2_CheckKey();
  if ( m2_Step() != 0 )
  {
    dogm.start();
    do{
      m2_CheckKey();
      dog_DrawStr(0, 55, font_5x7, dog_itoa(fps)); 
      m2_CheckKey();
      m2_Draw();
    } while( dogm.next() );
  }
  dog_Delay(10);
  
  frame_cnt++;
  if ( next_sec_time < millis() )
  {
    fps = frame_cnt;
    frame_cnt = 0;
    next_sec_time = millis() + 1000UL;
  }
  
}


