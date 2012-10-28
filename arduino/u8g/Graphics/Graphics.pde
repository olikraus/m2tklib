/*

  Graphics.pde

  How to mix Graphics with M2tklib
  
  U8glib Example

  m2tklib = Mini Interative Interface Toolkit Library

  >>> Before compiling: Please remove comment from the constructor of the 
  >>> connected graphics display (see below).

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
  
  Two methods:
    - mix/combine menu and graphics
    - switch between menu and graphcs
    
   Graphic procedure contain two parts:
    - draw
    - update, which also returns if a redraw is required
    
    draw_graphics():
      depending on m2tk state, draw graphics
    
    update_graphics():
      depending on m2tk state, update graphics for animation and force redraw
      
   
*/

#include "U8glib.h"
#include "M2tk.h"
#include "m2ghu8g.h"

// setup u8g object, please remove comment from one of the following constructor calls

//U8GLIB_NHD27OLED_BW u8g(13, 11, 10, 9);       // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD27OLED_2X_BW u8g(13, 11, 10, 9); // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD27OLED_GR u8g(13, 11, 10, 9);       // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD27OLED_2X_GR u8g(13, 11, 10, 9);  // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD31OLED_BW u8g(13, 11, 10, 9);       // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD31OLED_2X_BW u8g(13, 11, 10, 9); // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGS102 u8g(13, 11, 10, 9);                    // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGM132 u8g(13, 11, 10, 9);                    // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGM128 u8g(13, 11, 10, 9);                    // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_SSD1306_128X64 u8g(13, 11, 10, 9);                    // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_ST7920_128X64 u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, U8G_PIN_NONE, U8G_PIN_NONE, 17, 16);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16
//U8GLIB_ST7920_128X64 u8g(18, 16, 17, U8G_PIN_NONE);                  // SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
//U8GLIB_ST7920_192X32 u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, U8G_PIN_NONE, U8G_PIN_NONE, 17, 16);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16
//U8GLIB_ST7920_192X32 u8g(18, 16, 17, U8G_PIN_NONE);                  // SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
//U8GLIB_LM6059 u8g(13, 11, 10, 9);                    // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_LM6063 u8g(13, 11, 10, 9);                    // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGXL160_BW u8g(10, 9);            // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGXL160_GR u8g(13, 11, 10, 9);             // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGXL160_2X_BW u8g(13, 11, 10, 9);            // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGXL160_2X_GR u8g(13, 11, 10, 9);             // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_PCD8544 u8g(13, 11, 10, 9, 8);                    // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, Reset = 8
//U8GLIB_PCF8812 u8g(13, 11, 10, 9, 8);                    // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, Reset = 8
//U8GLIB_KS0108_128 u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 14, 15, 17, 16); // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs1=14, cs2=15,di=17,rw=16
//U8GLIB_LC7981_160X80 u8g(8, 9, 10, 11, 4, 5, 6, 7,  18, 14, 15, 17, 16); // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs=14 ,di=15,rw=17, reset = 16
//U8GLIB_LC7981_240X64 u8g(8, 9, 10, 11, 4, 5, 6, 7,  18, 14, 15, 17, 16); // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs=14 ,di=15,rw=17, reset = 16
//U8GLIB_LC7981_240X128 u8g(8, 9, 10, 11, 4, 5, 6, 7,  18, 14, 15, 17, 16); // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs=14 ,di=15,rw=17, reset = 16
//U8GLIB_ILI9325D_320x240 u8g(18,17,19,U8G_PIN_NONE,16 );  // 8Bit Com: D0..D7: 0,1,2,3,4,5,6,7 en=wr=18, cs=17, rs=19, rd=U8G_PIN_NONE, reset = 16
//U8GLIB_SBN1661_122X32 u8g(8,9,10,11,4,5,6,7,14,15, 17, U8G_PIN_NONE, 16); ; // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 cs1=14, cs2=15,di=17,rw=16,reset = 16
//U8GLIB_SSD1306_128X64 u8g(13, 11, 10, 9);             // SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_SSD1306_128X64 u8g(10, 9);             // HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)
//U8GLIB_SSD1327_96X96_GR u8g(U8G_I2C_OPT_NONE);             // I2C
//U8GLIB_SSD1327_96X96_2X_GR u8g(U8G_I2C_OPT_NONE);             // I2C
//U8GLIB_NHD_C12864 u8g(13, 11, 10, 9);                    // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9

// DOGS102 shield 
// uint8_t uiKeyUpPin = 5;
// uint8_t uiKeyDownPin = 3;
// uint8_t uiKeySelectPin = 4;

// DOGM132, DOGM128 and DOGXL160 shield
uint8_t uiKeyUpPin = 7;         
uint8_t uiKeyDownPin = 3;
uint8_t uiKeySelectPin = 2;

//================================================================
// m2tklib forward declarations

extern M2tk m2;
M2_EXTERN_ALIGN(el_top);

//================================================================
// low level graphics

uint8_t is_bigger = 0;
uint32_t next_state_change = 0;
uint8_t size = 1;

/* draw a rectange at x/y */
void draw_rectangle(uint8_t x, uint8_t y) {
  u8g.setDefaultForegroundColor();
  u8g.drawBox(x,y,size,size);
}

// state machine for the animation of the rectangle
// will return none-zero if an update is required
uint8_t update_rectangle(void) {
  if ( next_state_change < millis() ) {
    next_state_change = millis();
    next_state_change += 300;
    if ( is_bigger == 0 ) {
      size -= 1;
      if ( size <= 1 )
        is_bigger = 1;
    }
    else {
      size += 1;
      if ( size >= 10 )
        is_bigger = 0;
    }
    return 1;
  }
  return 0;
}

//================================================================
// m2tk related code

uint8_t y = 0;                   // position of the low level graphics

void fn_inc(m2_el_fnarg_p fnarg) {
  if ( y < 63-10 )
    y += 1;
}

void fn_dec(m2_el_fnarg_p fnarg) {
  if ( y > 0 )
    y -= 1;
}

M2_BUTTON(el_plus, "x30y41", "-1", fn_dec);
M2_BUTTON(el_minus, "x30y21", "+1", fn_inc);
M2_ROOT(el_leave_combine, "x30y1", "Back", &el_top);
M2_LIST(el_btn_list) = { &el_plus, &el_minus, &el_leave_combine};
M2_XYLIST(el_combine, NULL, el_btn_list);

M2_LABEL(el_goto_title, NULL, "Graphics and M2tk");
M2_ROOT(el_goto_combine, NULL, "Combine", &el_combine);
M2_ROOT(el_goto_switch, NULL, "Switch", &m2_null_element);             // selecting this, will remove all menues
M2_LIST(list_menu) = {&el_goto_title, &el_goto_combine, &el_goto_switch};
M2_VLIST(el_menu_vlist, NULL, list_menu);
M2_ALIGN(el_top, "W64H64", &el_menu_vlist);

M2tk m2(&el_top, m2_es_arduino, m2_eh_4bs, m2_gh_u8g_ffs);

//================================================================
// high level draw and update procedures

void draw_graphics(void) {
  // show the graphics depending on the current toplevel element
  
  if ( m2.getRoot() == &el_combine ) {
      // combine is active, do add the rectangle
      // menu is on the right, put the rectangle to the left
      draw_rectangle(0,y);
  }

  if ( m2.getRoot() == &m2_null_element ) {
      // all menus are gone, show the rectangle
      draw_rectangle(10,10);
  }
}

// update graphics, will return none-zero if an update is required
uint8_t update_graphics(void) {  
  if ( m2.getRoot() == &el_combine ) {
      // combine is active, update the rectangle for animation
      return update_rectangle();
  }

  if ( m2.getRoot() == &m2_null_element ) {
      // check for any keys and assign a suitable menu again
      if ( m2.getKey() != M2_KEY_NONE )
        m2.setRoot(&el_top);
    
      // all menus are gone, rectangle is shown, so do update
      return update_rectangle();
  }
  
  // no update for the graphics required
  return 0;
}


//================================================================
// overall draw procedure for u8glib

void draw(void) {
  draw_graphics();
  m2.draw();
}

//================================================================
// Arduino setup and loop

void setup(void) {
  // Connect u8glib with m2tklib
  m2_SetU8g(u8g.getU8g(), m2_u8g_box_icon);

  // Assign u8g font to index 0
  m2.setFont(0, u8g_font_7x13);

  // Setup keys
  m2.setPin(M2_KEY_SELECT, uiKeySelectPin);
  m2.setPin(M2_KEY_NEXT, uiKeyDownPin);
}

void loop() {
  m2.checkKey();
  if ( m2.handleKey() != 0 || update_graphics() != 0 ) {
    u8g.firstPage();  
    do {
      draw();
    } while( u8g.nextPage() );
  }
}


