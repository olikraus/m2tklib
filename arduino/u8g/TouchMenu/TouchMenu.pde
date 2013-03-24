/*

  TouchMenu.pde

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

//U8GLIB_NHD27OLED_BW u8g(13, 11, 10, 9);       // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD27OLED_2X_BW u8g(13, 11, 10, 9); // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD27OLED_GR u8g(13, 11, 10, 9);       // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD27OLED_2X_GR u8g(13, 11, 10, 9);  // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD31OLED_BW u8g(13, 11, 10, 9);       // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD31OLED_2X_BW u8g(13, 11, 10, 9); // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGS102 u8g(13, 11, 10, 9);                    // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGM132 u8g(13, 11, 10, 9);                    // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGM128 u8g(13, 11, 10, 9);                    // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
U8GLIB_DOGM128 u8g(10, 9);                    // HW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
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
// Setup 4-Wire Resistive Touch Panel

uint8_t tp_left = A3;
uint8_t tp_right = A5;
uint8_t tp_top = A4;
uint8_t tp_bottom = A2;

//================================================================
// Touch Panel Code

#define TPD_OPEN 0
#define TPD_PRESSED 1
#define TPD_OPEN_TO_PRESSED 2
#define TPD_PRESSED_TO_OPEN 3

/* touch panel dimension */
struct _tpd_struct
{
  uint8_t raw;		/* input value */
  uint8_t delta;		/* delta of the last raw value */
  uint8_t curr;		/* current valid position */
  uint8_t state;		/* state machine */
  
  /* calibration values (set during init phase) */
  uint8_t start;
  uint8_t end;		
  uint8_t open;		/* value, when not touched/pressed */
  
  /* calibration values (must be predefined) */
  uint8_t mt;		/* move and open threshold */
  
  /* user values */
  uint8_t range;	/* result will have range fron 0..range  (including the value of range) */
  
  uint8_t result;	/* output value: position [0...range] */
  uint8_t is_pressed;	/* output value: pressed (=1) or not pressed (=0) */
  uint8_t is_update;	/* will be set to 1 if result or is_pressed has been updated */
};
typedef struct _tpd_struct tpd_struct;


#define TP_STATE_OPEN_DETECT 0
#define TP_STATE_OPEN_DETECT_1 1
#define TP_STATE_START_PRESS 2
#define TP_STATE_WAIT_START_OPEN 3
#define TP_STATE_END_PRESS 4
#define TP_STATE_WAIT_END_OPEN 5
#define TP_STATE_NORMAL 6

#define TP_DISPLAY_DO_NOT_TOUCH 0
#define TP_DISPLAY_PRESS_AND_RELEASE_EDGE 1
#define TP_DISPLAY_PRESS_AND_RELEASE_OPPOSITE_EDGE 2
#define TP_DISPLAY_OK 3

struct _tp_struct
{
  tpd_struct x;
  tpd_struct y;
  uint8_t state;
  uint8_t is_pressed; /* combination of x.is_pressed && y.is_pressed */
  uint8_t is_update;
};
typedef struct _tp_struct tp_struct;

tp_struct tp;


uint8_t tp_GetDisplayState(tp_struct *tp)
{
  switch(tp->state)
  {
    case TP_STATE_OPEN_DETECT:
    case TP_STATE_OPEN_DETECT_1:
      return TP_DISPLAY_DO_NOT_TOUCH;
    case TP_STATE_START_PRESS:
    case TP_STATE_WAIT_START_OPEN:
      return TP_DISPLAY_PRESS_AND_RELEASE_EDGE;
    case TP_STATE_END_PRESS:
    case TP_STATE_WAIT_END_OPEN:
      return TP_DISPLAY_PRESS_AND_RELEASE_OPPOSITE_EDGE;
    case TP_STATE_NORMAL:
    default:
      return TP_DISPLAY_OK;
  }
}


uint8_t tpd_is_active_area(tpd_struct *d)
{
  uint8_t start, end;
  
  if  ( d->start < d->end )
  {
    start = d->start;
    end = d->end;
  }
  else
  {
    end = d->start;
    start = d->end;
  }
    
  if ( d->open < start )
  {
    if ( d->raw < start )	/* it is pressed if "raw >= start", so continue to check otherwise */
    {
      if ( d->raw <= d->open )
	return 0;	/* not pressed */
      if ( (d->raw - d->open) < (start-d->raw) )
	return 0;
    }
  }
  else if ( d->open > end )
  {
    if ( d->raw > end)	/* it is pressed if "raw >= start", so continue to check otherwise */
    {
      if ( d->raw >= d->open )
	return 0;	/* not pressed */
      if ( (d->open - d->raw) < (d->raw - end) )
	return 0;
    }
  }
  return 1;		/*default: assume touch panel pressed */
}

static uint8_t tpd_delta(uint8_t a, uint8_t b)
{
  if ( a < b )
    return b-a;
  return a-b;
}

static void tpd_next_step(tpd_struct *d, uint8_t raw)
{
  uint8_t is_pressed;
  d->delta = tpd_delta(d->raw, raw);
  d->raw = raw;
  is_pressed = tpd_is_active_area(d);
  
  switch(d->state)
  {
    case TPD_OPEN:
      if ( is_pressed == 0 )
      {
	d->curr = d->raw;    
      }
      else
      {
	d->state = TPD_OPEN_TO_PRESSED;
      }
      break;
    case TPD_PRESSED:
      if ( is_pressed == 0 )
      {
	d->state = TPD_OPEN;
	d->curr = d->raw;
      }
      else
      {
	if ( d->delta > d->mt )
	{
	  d->state = TPD_PRESSED_TO_OPEN;
	}
	else
	{
	  /* stay in PRESSED state */
	  d->curr = d->raw;
	}
      }
      break;
    case TPD_PRESSED_TO_OPEN:
    case TPD_OPEN_TO_PRESSED:
      if ( is_pressed == 0 )
      {
	d->state = TPD_OPEN;
	d->curr = d->raw;
      }
      else
      {
	if ( d->delta > d->mt )
	{
	  /* stay in current state */
	}
	else
	{
	  d->state = TPD_PRESSED;
	  d->curr = d->raw;
	}
      }
      break;
  }
  
  if ( d->state == TPD_OPEN || d->state == TPD_OPEN_TO_PRESSED )
    is_pressed = 0;
  else
    is_pressed = 1;
  if ( d->is_pressed != is_pressed )
    d->is_update = 1;
  d->is_pressed = is_pressed;
}


static void tpd_do_calibrate(tpd_struct *d)
{
  if ( tpd_delta(d->raw, d->open) < d->mt )
    return;
  if ( d->state != TPD_PRESSED )
    return;

  if  ( d->start < d->end )
  {
    if ( d->end < d->raw )
      d->end = d->raw;
    if ( d->start > d->raw )
      d->start = d->raw;
  }
  else
  {
    if ( d->end > d->raw )
      d->end = d->raw;
    if ( d->start < d->raw )
      d->start = d->raw;
  }

}



/* map curr value to 0...range (result) */
void tpd_map_touch_position(tpd_struct *d)
{
  uint16_t p;
  uint8_t start, end;
  if  ( d->start < d->end )
  {
    start = d->start;
    end = d->end;
  }
  else
  {
    end = d->start;
    start = d->end;
  }
  
  if ( d->curr <= start )
  {
    d->result = 0;
    return;
  }
  
  if ( d->curr >= end )
  {
    d->result = d->range;
    return;
  }
  
  p = d->curr;
  p -= start;
  p *= d->range;
  end -= start;
  p /= end;
  
  if ( d->result != p )
    d->is_update = 1;
  d->result = p;
}


void tp_Init(tp_struct *tp, uint8_t width, uint8_t height)
{
  tp->x.start = 0;
  tp->x.end = 0;
  tp->x.open = 0;
  tp->x.range = width-1;
  tp->x.mt = width/8;
  tp->x.state = TPD_OPEN;
  
  tp->y.start = 0;
  tp->y.end = 0;
  tp->y.open = 0;
  tp->y.range = height-1;
  tp->y.mt = height/8;
  tp->y.state = TPD_OPEN;
  
  tp->state = TP_STATE_OPEN_DETECT;
  tp->is_update = 1;
}

uint8_t tp_IsOpen(tp_struct *tp, uint8_t x, uint8_t y)
{
  if ( tpd_delta(tp->x.open, x) < tp->x.mt &&  tpd_delta(tp->y.open, y)  < tp->y.mt )
    return 1;
  return 0;
}

void tp_SetRaw(tp_struct *tp, uint8_t x, uint8_t y)
{
  switch( tp->state )
  {
    case TP_STATE_OPEN_DETECT:
      tp->x.open = x; 
      tp->y.open = y;
      tp->state = TP_STATE_OPEN_DETECT_1;
      tp->is_update = 1;
      break;
    case TP_STATE_OPEN_DETECT_1:
      if ( tpd_delta(tp->x.open, x) <= tp->x.mt &&  tpd_delta(tp->y.open, y)  <= tp->y.mt )
      {
	tp->is_update = 1;
	tp->state = TP_STATE_START_PRESS;
      }
      tp->x.open = x; 
      tp->y.open = y;
      break;
    case TP_STATE_START_PRESS:
      if ( tp_IsOpen(tp,x,y) == 0 )
      {
	if ( tpd_delta(tp->x.start, x) <= tp->x.mt &&  tpd_delta(tp->y.start, y)  <= tp->y.mt )
	{
	  tp->state = TP_STATE_WAIT_START_OPEN;
	  tp->is_update = 1;
	}
	tp->x.start = x;
	tp->y.start = y;
      }
      break;
    case TP_STATE_WAIT_START_OPEN:
      if ( tp_IsOpen(tp,x,y) != 0 )
      {      
	tp->state = TP_STATE_END_PRESS;
	tp->is_update = 1;
      }
      break;
    case TP_STATE_END_PRESS:
      if ( tp_IsOpen(tp,x,y) == 0 )
      {
	if ( tpd_delta(tp->x.start, x) > tp->x.mt &&  tpd_delta(tp->y.start, y)  > tp->y.mt )
	{
	  if ( tpd_delta(tp->x.end, x) <= tp->x.mt &&  tpd_delta(tp->y.end, y)  <= tp->y.mt )
	  {
	    tp->state = TP_STATE_WAIT_END_OPEN;
	    tp->is_update = 1;
	  }
	  tp->x.end = x;
	  tp->y.end = y;
	}
      }
      break;
    case TP_STATE_WAIT_END_OPEN:
      if ( tp_IsOpen(tp,x,y) != 0 )
      {
	tp->state = TP_STATE_NORMAL;
	tp->is_update = 1;
      }
      break;
    default:
      tpd_next_step(&(tp->x), x);
      tpd_map_touch_position(&(tp->x));
      tpd_do_calibrate(&(tp->x));
      
      tpd_next_step(&(tp->y), y);
      tpd_map_touch_position(&(tp->y));
      tpd_do_calibrate(&(tp->y));
      
      tp->is_pressed = tp->x.is_pressed && tp->y.is_pressed;
      tp->is_update = tp->x.is_update && tp->y.is_update;
      break;
  }
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
  
  tp_SetRaw(&tp, tp_raw_x, tp_raw_y);  
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


M2tk m2(&top_el_ts_mnu, m2_es_touch_screen, m2_eh_6bsts, m2_gh_u8g_ffs);

//================================================================
// overall draw procedure for u8glib, includes touch panel calibration

void center(u8g_uint_t y, const char *str)
{
  u8g_uint_t x;
  x = u8g.getWidth();
  x -= u8g.getStrWidth(str);
  x /= 2;
  u8g.drawStr(x, y, str);
}


void draw(void) {
  switch( tp_GetDisplayState(&tp) )
  {
    case TP_DISPLAY_DO_NOT_TOUCH:
      center(12, "Do Not Touch");
      break;
    case TP_DISPLAY_PRESS_AND_RELEASE_EDGE:
      /* it does not matter which edge is pressed, but later the opposite edge must be pressed */
      center( 12, "Press");
      center( 24, "Lower Left Edge");
      u8g.drawLine(0, u8g.getHeight()-1, 10, u8g.getHeight()-11);
      break;
    case TP_DISPLAY_PRESS_AND_RELEASE_OPPOSITE_EDGE:
      center( 12, "Press");
      center( 24, "Upper Right Edge");
      u8g.drawLine(u8g.getWidth()-11, 10, u8g.getWidth()-1, 0);
      break;
    case TP_DISPLAY_OK:
    default:
      m2.draw();
      break;
  }
}

//================================================================
// Arduino setup and loop

void setup(void) {
  // Connect u8glib with m2tklib
  m2_SetU8g(u8g.getU8g(), m2_u8g_box_icon);

  // Assign u8g font to index 0
  m2.setFont(0, u8g_font_helvR10r);

  // Setup keys
  m2.setPin(M2_KEY_SELECT, uiKeySelectPin);
  m2.setPin(M2_KEY_NEXT, uiKeyDownPin);
  
  // setup cursor
  u8g.setCursorFont(u8g_font_cursorr);
  u8g.setCursorStyle(66);

  // setup touch panel
  tp_Init(&tp, u8g.getWidth(), u8g.getHeight());
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


