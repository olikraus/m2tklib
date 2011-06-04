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
  
*/

#include "Dogm.h"
#include "m2.h"
#include "m2ghdogm.h"

int a0Pin = 9;      // address line a0 for the dogm module


// frames per second
unsigned long next_sec_time;
uint8_t fps, frame_cnt;

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



// initialize the dogm library
Dogm dogm(a0Pin);


/*==============================================================*/
/* forward & common declaratations */

M2_EXTERN_ALIGN(top_el);
M2_ROOT(goto_top_el, "f4x0y0", "Home", &top_el);

/*==============================================================*/
/* radio box */

uint8_t radio_val = 0;

M2_LABEL(opt1_label, "", "Option 1:");
M2_LABEL(opt2_label, "w99", "Option 2:");
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


M2_ROOT(goto_radio_el, "w70f12","Select", &radio_el);
M2_ROOT(goto_rgb_el, "w70f12","RGB", &rgb_el);
M2_ROOT(goto_xy_el, "w70f12","Name Entry", &xy_el);

  
M2_LIST(top_list) = { 
  &goto_radio_el,
  &goto_rgb_el,
  &goto_xy_el  
  };
M2_GRIDLIST(top_grid_el,"c1", top_list);
M2_ALIGN(top_el, "w160h104", &top_grid_el);

/*==============================================================*/


void setup() {
  
#ifdef DOGXL160_HW_GR
  m2_Init(&top_el, m2_es_arduino, m2_eh_2bs, m2_gh_dogxl160);	
#else
  m2_Init(&top_el, m2_es_arduino, m2_eh_2bs, m2_gh_dogm_fbs);	
#endif

  m2_SetFont(0, font_7x13);
  m2_SetPin(M2_KEY_SELECT, uiKeySelectPin);
  m2_SetPin(M2_KEY_NEXT, uiKeyDownPin);
  
  next_sec_time = millis() + 1000UL;
  fps = 0;
  frame_cnt = 0;
}

void loop() {
  dogm.showLibInfo();
  
  m2_CheckKey();
  if ( m2_HandleKey() != 0 )
  {
    dogm.start();
    do{
      m2_CheckKey();      
      dog_DrawStr(0, 55, font_5x7, dog_itoa(fps)); 
      dog_DrawStr(10, 55, font_5x7, dog_itoa(dog_GetFontBBXHeight(font_7x13))); 
      m2_CheckKey();
      m2_Draw();
    } while( dogm.next() );
  }
  /*
  dog_Delay(1000);
  m2_SetKey(M2_KEY_NEXT);
  */
  
  frame_cnt++;
  if ( next_sec_time < millis() )
  {
    fps = frame_cnt;
    frame_cnt = 0;
    next_sec_time = millis() + 1000UL;
  }
  
}


