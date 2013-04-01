/*

  TouchPanel.pde

  Example for touch panel
  
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
  
*/

#include "U8glib.h"
#include "M2tk.h"
#include "m2ghu8g.h"

// setup u8g object, please remove comment from one of the following constructor calls
// IMPORTANT NOTE: The complete list of supported devices is here: http://code.google.com/p/u8glib/wiki/device

//U8GLIB_NHD27OLED_BW u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD27OLED_2X_BW u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD27OLED_GR u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD27OLED_2X_GR u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD31OLED_BW u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD31OLED_2X_BW u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD31OLED_GR u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD31OLED_2X_GR u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGS102 u8g(13, 11, 10, 9);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGM132 u8g(13, 11, 10, 9);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGM128 u8g(13, 11, 10, 9);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_ST7920_128X64_1X u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 17, 16);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16
//U8GLIB_ST7920_128X64_4X u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 17, 16);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16
//U8GLIB_ST7920_128X64_1X u8g(18, 16, 17);	// SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
//U8GLIB_ST7920_128X64_4X u8g(18, 16, 17);	// SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
//U8GLIB_ST7920_192X32_1X u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 17, 16);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16
//U8GLIB_ST7920_192X32_4X u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 17, 16);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16
//U8GLIB_ST7920_192X32_1X u8g(18, 16, 17);	// SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
//U8GLIB_ST7920_192X32_4X u8g(18, 16, 17);	// SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
//U8GLIB_ST7920_192X32_1X u8g(13, 11, 10);	// SPI Com: SCK = en = 13, MOSI = rw = 11, CS = di = 10
//U8GLIB_ST7920_192X32_4X u8g(10);		// SPI Com: SCK = en = 13, MOSI = rw = 11, CS = di = 10, HW SPI
//U8GLIB_ST7920_202X32_1X u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 17, 16);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16
//U8GLIB_ST7920_202X32_4X u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 17, 16);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16
//U8GLIB_ST7920_202X32_1X u8g(18, 16, 17);	// SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
//U8GLIB_ST7920_202X32_4X u8g(18, 16, 17);	// SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
//U8GLIB_LM6059 u8g(13, 11, 10, 9);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_LM6063 u8g(13, 11, 10, 9);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGXL160_BW u8g(10, 9);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGXL160_GR u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGXL160_2X_BW u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGXL160_2X_GR u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_PCD8544 u8g(13, 11, 10, 9, 8);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, Reset = 8
//U8GLIB_PCF8812 u8g(13, 11, 10, 9, 8);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, Reset = 8
//U8GLIB_KS0108_128 u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 14, 15, 17, 16); 		// 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs1=14, cs2=15,di=17,rw=16
//U8GLIB_LC7981_160X80 u8g(8, 9, 10, 11, 4, 5, 6, 7,  18, 14, 15, 17, 16); 	// 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs=14 ,di=15,rw=17, reset = 16
//U8GLIB_LC7981_240X64 u8g(8, 9, 10, 11, 4, 5, 6, 7,  18, 14, 15, 17, 16); 	// 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs=14 ,di=15,rw=17, reset = 16
//U8GLIB_LC7981_240X128 u8g(8, 9, 10, 11, 4, 5, 6, 7,  18, 14, 15, 17, 16); 	// 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs=14 ,di=15,rw=17, reset = 16
//U8GLIB_ILI9325D_320x240 u8g(18,17,19,U8G_PIN_NONE,16 );  			// 8Bit Com: D0..D7: 0,1,2,3,4,5,6,7 en=wr=18, cs=17, rs=19, rd=U8G_PIN_NONE, reset = 16
//U8GLIB_SBN1661_122X32 u8g(8,9,10,11,4,5,6,7,14,15, 17, U8G_PIN_NONE, 16); 	// 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 cs1=14, cs2=15,di=17,rw=16,reset = 16
//U8GLIB_SSD1306_128X64 u8g(13, 11, 10, 9);	// SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_SSD1306_128X64 u8g(10, 9);		// HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);	// HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)
//U8GLIB_SSD1306_128X32 u8g(13, 11, 10, 9);	// SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_SSD1306_128X32 u8g(10, 9);             // HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)
//U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);	// HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)
//U8GLIB_SSD1309_128X64 u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_SSD1327_96X96_GR u8g(U8G_I2C_OPT_NONE);	// I2C
//U8GLIB_SSD1327_96X96_2X_GR u8g(U8G_I2C_OPT_NONE);	// I2C
//U8GLIB_NHD_C12864 u8g(13, 11, 10, 9, 8);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, RST = 8
//U8GLIB_NHD_C12832 u8g(13, 11, 10, 9, 8);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, RST = 8
//U8GLIB_T6963_240X128 u8g(8, 9, 10, 11, 4, 5, 6, 7, 14, 15, 17, 18, 16); // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7, cs=14, a0=15, wr=17, rd=18, reset=16
//U8GLIB_T6963_240X64 u8g(8, 9, 10, 11, 4, 5, 6, 7, 14, 15, 17, 18, 16); // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7, cs=14, a0=15, wr=17, rd=18, reset=16
//U8GLIB_T6963_128X64 u8g(8, 9, 10, 11, 4, 5, 6, 7, 14, 15, 17, 18, 16); // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7, cs=14, a0=15, wr=17, rd=18, reset=16


// DOGS102 shield 
// uint8_t uiKeyUpPin = 5;
// uint8_t uiKeyDownPin = 3;
// uint8_t uiKeySelectPin = 4;

// DOGM132, DOGM128 and DOGXL160 shield
uint8_t uiKeyUpPin = 7;         
uint8_t uiKeyDownPin = 3;
uint8_t uiKeySelectPin = 2;

//================================================================
// Setup 4-Wire Resistive Touch Panel

uint8_t tp_left = A3;
uint8_t tp_right = A5;
uint8_t tp_top = A4;
uint8_t tp_bottom = A2;

/* Run "Touch4WSetup" and enter values here */
#define X_START 64
#define X_END 200
#define Y_START 105
#define Y_END 160  

//================================================================
// Touch Panel Code

/* touch panel dimension */
struct tpd_struct
{
  /* raw value */
  uint8_t raw;
  
  /* calibration values  */
  uint8_t start;
  uint8_t end;		
  
  /* user values */
  uint8_t range;	/* result will have range fron 0..range  (including the value of range) */
  
  uint8_t result;	/* output value: position [0...range] */
  uint8_t is_pressed;	/* output value: pressed (=1) or not pressed (=0) */
  uint8_t is_update;	/* will be set to 1 if result or is_pressed has been updated */
};

struct tp_struct
{
  struct tpd_struct x;
  struct tpd_struct y;
  uint8_t is_pressed; /* combination of x.is_pressed && y.is_pressed */
  uint8_t is_update;
};

struct tp_struct tp;

/* map raw value to 0...range (result) */
void tpd_map_touch_position(struct  tpd_struct *d, uint8_t raw)
{
  uint8_t is_pressed;
  uint16_t p;
  uint8_t start, end;
  
  d->raw = raw;
  
  start = d->start;
  end = d->end;

  /* check if position is within active area; store result in "is_pressed" */
  is_pressed = 1;  
  if ( raw < start )
  {
    d->result = 0;
    is_pressed = 0;
  }
  if ( raw >= end )
  {
    d->result = d->range;
    is_pressed = 0;
  }
  
  /* store "is_pressed" in the global structure, set update flag */
  if ( d->is_pressed != is_pressed )
    d->is_update = 1;
  d->is_pressed = is_pressed;
    
  /* map "raw" value into target range */
  if ( is_pressed != 0 )
  {    
    p = raw;
    p -= start;
    p *= d->range;
    end -= start;
    p /= end;

    if ( d->result != p )
      d->is_update = 1;
    d->result = p;
  }
}

void tp_Init(uint8_t width, uint8_t height)
{
  tp.x.start = X_START;
  tp.x.end = X_END;
  tp.x.range = width-1;
  
  tp.y.start = Y_START;
  tp.y.end = Y_END;
  tp.y.range = height-1;
  
  tp.is_update = 1;
}

void setTouchRawValues(uint8_t x, uint8_t y)
{
      tpd_map_touch_position(&(tp.x), x);
      tpd_map_touch_position(&(tp.y), y);
      
      tp.is_pressed = tp.x.is_pressed && tp.y.is_pressed;
      if ( tp.x.is_update || tp.y.is_update )
	tp.is_update = 1;
}


uint8_t getTouchPos(uint8_t hiPin, uint8_t lowPin, uint8_t sensePin, uint8_t dcPin)
{
  uint8_t val;
  pinMode(dcPin, INPUT);
  pinMode(sensePin, INPUT_PULLUP);
  pinMode(hiPin, OUTPUT);
  pinMode(lowPin, OUTPUT);  
  
  digitalWrite(hiPin, HIGH);
  digitalWrite(lowPin, LOW);
  delay(10);
  val = analogRead(sensePin) >> 2;
  pinMode(hiPin, INPUT);
  pinMode(lowPin, INPUT);  
  delay(10);
  return val;
}

void updateTouchPanel(void)
{
  uint8_t tp_raw_x;
  uint8_t tp_raw_y;

  tp_raw_x = getTouchPos(tp_right, tp_left, tp_bottom, tp_top);
  tp_raw_y = getTouchPos(tp_top, tp_bottom, tp_left, tp_right);
  
  setTouchRawValues(tp_raw_x, tp_raw_y);  
}


//================================================================
// m2tklib forward declarations

extern M2tk m2;
M2_EXTERN_ALIGN(top_el_xtsk_num_menu);

//================================================================
static char down_bits[] U8G_PROGMEM = {
   0x00, 0x00, 0xf8, 0x1f, 0x04, 0x20, 0x82, 0x41, 0x82, 0x41, 0x82, 0x41,
   0x82, 0x41, 0x82, 0x41, 0x82, 0x41, 0xf2, 0x4f, 0xe2, 0x47, 0xc2, 0x43,
   0x82, 0x41, 0x04, 0x20, 0xf8, 0x1f, 0x00, 0x00};
static char enter_bits[] U8G_PROGMEM  = {
   0x00, 0x00, 0xf8, 0x1f, 0x04, 0x20, 0x02, 0x40, 0x02, 0x40, 0x02, 0x4c,
   0x02, 0x4c, 0x22, 0x4c, 0x32, 0x4c, 0xfa, 0x4f, 0xfa, 0x4f, 0x32, 0x40,
   0x22, 0x40, 0x04, 0x20, 0xf8, 0x1f, 0x00, 0x00};
static char left_bits[] U8G_PROGMEM  = {
   0x00, 0x00, 0xf8, 0x1f, 0x04, 0x20, 0x02, 0x40, 0x42, 0x40, 0x62, 0x40,
   0x72, 0x40, 0xfa, 0x5f, 0xfa, 0x5f, 0x72, 0x40, 0x62, 0x40, 0x42, 0x40,
   0x02, 0x40, 0x04, 0x20, 0xf8, 0x1f, 0x00, 0x00};
static char right_bits[] U8G_PROGMEM  = {
   0x00, 0x00, 0xf8, 0x1f, 0x04, 0x20, 0x02, 0x40, 0x02, 0x42, 0x02, 0x46,
   0x02, 0x4e, 0xfa, 0x5f, 0xfa, 0x5f, 0x02, 0x4e, 0x02, 0x46, 0x02, 0x42,
   0x02, 0x40, 0x04, 0x20, 0xf8, 0x1f, 0x00, 0x00};
static char up_bits[] U8G_PROGMEM  = {
   0x00, 0x00, 0xf8, 0x1f, 0x04, 0x20, 0x82, 0x41, 0xc2, 0x43, 0xe2, 0x47,
   0xf2, 0x4f, 0x82, 0x41, 0x82, 0x41, 0x82, 0x41, 0x82, 0x41, 0x82, 0x41,
   0x82, 0x41, 0x04, 0x20, 0xf8, 0x1f, 0x00, 0x00};


uint32_t xu32val = 0;
M2_LABEL(el_xtsk_num_label, NULL, "U32:");
M2_U32NUM(el_xtsk_num_u32, "a1c5", &xu32val);


M2_XBMTSKP(el_xtsk_up, "f1w16h16r1", 16, 16, up_bits, M2_KEY_DATA_UP);		// data up
M2_XBMTSKP(el_xtsk_down, "f1w16h16", 16, 16, down_bits, M2_KEY_DATA_DOWN);		// data down
M2_XBMTSKP(el_xtsk_left, "f1w16h16", 16, 16, left_bits, M2_KEY_PREV);		// left
M2_XBMTSKP(el_xtsk_right, "f1w16h16", 16, 16, right_bits, M2_KEY_NEXT);		// right
M2_XBMROOTP(el_xtsk_enter, "f1t1r1w16h16", 16, 16, enter_bits, &top_el_xtsk_num_menu);		// enter



M2_LIST(xtsk_list) = { 
    &el_xtsk_num_label, &m2_null_element, &el_xtsk_up,    &m2_null_element, 
    &el_xtsk_num_u32,  &el_xtsk_left,            &el_xtsk_enter, &el_xtsk_right, 
    &m2_null_element, &m2_null_element, &el_xtsk_down, &m2_null_element, 
};
M2_GRIDLIST(el_xtsk_num_menu, "c4", xtsk_list);
M2_ALIGN(top_el_xtsk_num_menu, "-1|1W64H64", &el_xtsk_num_menu);




//================================================================
// new event source and m2tklib constructor


uint8_t m2_es_touch_screen(m2_p ep, uint8_t msg)
{
  switch(msg)
  {
    case M2_ES_MSG_GET_KEY:
      // update touch panel and handle return values
      updateTouchPanel();
    
      if ( tp.is_pressed  )
      {
	/* set x and y position of the touch screen */
	/* (x,y) is a M2 position: (0,0) is lower left */
	m2_SetEventSourceArgsM2(ep, tp.x.result, tp.y.result );
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


M2tk m2(&top_el_xtsk_num_menu, m2_es_touch_screen, m2_eh_6bsts, m2_gh_u8g_ffs);

//================================================================
// overall draw procedure for u8glib

void draw(void) {
  m2.draw();
}

//================================================================
// Arduino setup and loop

void setup(void) {
  // Connect u8glib with m2tklib
  m2_SetU8g(u8g.getU8g(), m2_u8g_box_icon);

  // Assign u8g font to index 0
  m2.setFont(0, u8g_font_6x10r);

  // Setup keys
  m2.setPin(M2_KEY_SELECT, uiKeySelectPin);
  m2.setPin(M2_KEY_NEXT, uiKeyDownPin);
  
  // setup cursor
  u8g.setCursorFont(u8g_font_cursorr);
  u8g.setCursorStyle(66);

  // setup touch panel
  tp_Init(u8g.getWidth(), u8g.getHeight());
}

void loop() {
  // poll touch panel 
  m2.checkKey();
  
  // enable cursor and update cursor position
  if ( tp.is_pressed != 0 )
    u8g.enableCursor();
  else
    u8g.disableCursor();
  u8g.setCursorPos(tp.x.result, u8g.getHeight()-tp.y.result-1);
  
  // menu redraw
  if ( m2.handleKey() != 0 ||  tp.is_update ) 
  {
    tp.is_update = 0;
    
    // optional: poll more often
    // if ( tp_GetDisplayState(&tp) == TP_DISPLAY_OK )
    //  m2.checkKey();

    u8g.firstPage();  
    do {
      draw();
    } while( u8g.nextPage() );
  }
}


