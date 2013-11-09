/*

  gps.c 
  
  Main file of the GPS tracker device.
  
  m2tklib - Mini Interative Interface Toolkit Library
  u8glib - Universal 8bit Graphics Library
  
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

#include "u8g.h"
#include "u8g_arm.h"
#include "m2.h"
#include "m2ghu8g.h"
#include "adc.h"
#include "init.h"
#include "util.h"


/*=======================================================================*/
/* low pass filter with 8 bit resolution, p = 0..255 */
#define LOW_PASS_BITS 8
uint32_t low_pass(uint32_t *a, uint32_t x, uint32_t p)
{
  uint32_t n;
  n = ((1<<LOW_PASS_BITS)-p) * (*a) + p * x;
  n >>= LOW_PASS_BITS;
  *a = n;
  return n;
}

/*=======================================================================*/
uint8_t update_gps_tracker_variables(void)
{
  uint16_t tmp16;
  uint8_t is_changed = 0;
  
  tmp16 = adc_get_value(2);
  tmp16 = low_pass( &gps_tracker_variables.adc_low_pass_z, tmp16, 7);
  
  if ( gps_tracker_variables.adc_battery != tmp16 )
  {
    is_changed = 1;
    gps_tracker_variables.adc_battery = tmp16;
  }
  
  if ( gps_tracker_variables.uart_byte_cnt_gui != gps_tracker_variables.uart_byte_cnt_raw )
  {
    is_changed = 1;
    gps_tracker_variables.uart_byte_cnt_gui = gps_tracker_variables.uart_byte_cnt_raw;
  }
  
  return is_changed;
}


/*=========================================================================*/
/* menu definitions */

uint8_t select_color = 0;

void fn_ok(m2_el_fnarg_p fnarg) {
  /* accept selection */
}

void fn_cancel(m2_el_fnarg_p fnarg) {
  /* discard selection */
}

M2_LABEL(el_label1, NULL, "red");
M2_RADIO(el_radio1, "v0", &select_color);

M2_LABEL(el_label2, NULL, "green");
M2_RADIO(el_radio2, "v1", &select_color);

M2_LABEL(el_label3, NULL, "blue");
M2_RADIO(el_radio3, "v2", &select_color);

M2_BUTTON(el_cancel, NULL, "cancel", fn_cancel);
M2_BUTTON(el_ok, NULL, " ok ", fn_ok);

M2_LIST(list) = { 
    &el_label1, &el_radio1, 
    &el_label2, &el_radio2,  
    &el_label3, &el_radio3, 
    &el_cancel, &el_ok 
};
M2_GRIDLIST(el_top, "c2",list);

/*=== extern declarations ===*/
M2_EXTERN_ALIGN(el_home);

/*=== reuseable elements ===*/

M2_ROOT(el_goto_home, NULL, "Home", &el_home);

/*=== test gps ===*/

M2_ALIGN(el_test_gps, "|0", &el_goto_home);

/*=== test compass ===*/

M2_ALIGN(el_test_compass, "|0", &el_goto_home);

/*=== show GPS UART ===*/

M2_ALIGN(el_show_gps_uart, "|0", &el_goto_home);

/*=== show battery ===*/

M2_ALIGN(el_show_battery, "|0", &el_goto_home);

/*=== toplevel menu ===*/

M2_ROOT(el_home_test_gps, NULL, "GPS" , &el_test_gps);
M2_ROOT(el_home_test_compass, NULL, "Compass", &el_test_compass);
M2_ROOT(el_home_show_battery, NULL, "Battery", &el_show_battery);
M2_ROOT(el_home_show_gps_uart, NULL, "GPS UART", &el_show_gps_uart);
M2_LIST(list_home) = {
  &el_home_test_gps,
  &el_home_test_compass,
  &el_home_show_battery,
  &el_home_show_gps_uart
};
M2_VLIST(el_home_vlist, NULL, list_home);
M2_ALIGN(el_home, NULL, &el_home_vlist);


/*=========================================================================*/
/* global variables and objects */

u8g_t u8g;

/*=========================================================================*/
/* controller, u8g and m2 setup */

void display_init(void)
{  

  /* eval board */
  /*
  u8g_pin_a0 = PIN(1,0);
  u8g_pin_cs = PIN(0,8);
  u8g_pin_rst = PIN(0,6);
  */

  /* GPS2 Board */
  u8g_pin_a0 = PIN(1,0);
  u8g_pin_cs = PIN(0,8);
  u8g_pin_rst = PIN(0,6);
  
  /* 1. Setup and create u8g device */
  u8g_InitComFn(&u8g, &u8g_dev_uc1701_dogs102_hw_spi, u8g_com_hw_spi_fn);
  u8g_SetFontRefHeightAll(&u8g);

  /* 2. Setup m2 */
  m2_Init(&el_home, m2_es_arm_u8g, m2_eh_4bs, m2_gh_u8g_bfs);

  /* 3. Connect u8g display to m2  */
  m2_SetU8g(&u8g, m2_u8g_box_icon);

  /* 4. Set a font, use normal u8g_font's */
  //m2_SetFont(0, (const void *)u8g_font_6x10r);
  m2_SetFont(0, (const void *)u8g_font_helvB08r);
	
  /* 5. Define keys */
    
  /* ARM LPC1114 GPS2 Board */
  m2_SetPin(M2_KEY_PREV, PIN(0, 1));
  m2_SetPin(M2_KEY_SELECT, PIN(1, 2));
  m2_SetPin(M2_KEY_NEXT, PIN(0, 11));
  
  u8g_SetRot180(&u8g);  
}

/*=========================================================================*/
/* u8g draw procedure (body of picture loop) */

/* draw procedure of the u8g picture loop */
void draw(void)
{	
  if ( m2_GetRoot() == &el_show_battery )
  {
    u8g_SetFont(&u8g, u8g_font_helvB08r);
    u8g_SetDefaultForegroundColor(&u8g);
    u8g_DrawStr(&u8g,  0, 12, "Battery Status");
    u8g_DrawStr(&u8g,  0, 12*2, "Raw: ");
    u8g_DrawStr(&u8g,  30, 12*2, u8g_u16toa(gps_tracker_variables.adc_battery, 4));
    u8g_DrawStr(&u8g,  0, 12*3, "mV: ");
    u8g_DrawStr(&u8g,  30, 12*3, u8g_u16toa((gps_tracker_variables.adc_battery*3300UL)/1024, 4));    
  }
  else if ( m2_GetRoot() == &el_show_gps_uart ) 
  {
    uint32_t h = 9;
    //u8g_SetFont(&u8g, u8g_font_helvB08r);
    u8g_SetFont(&u8g, u8g_font_5x8r);
    u8g_SetDefaultForegroundColor(&u8g);
    u8g_DrawStr(&u8g,  0, h, "GPS UART");
    u8g_DrawStr(&u8g,  0, h*2, "RX: ");
    u8g_DrawStr(&u8g,  30, h*2, u32toa(gps_tracker_variables.uart_byte_cnt_raw, 9));
    u8g_DrawStr(&u8g,  0, h*3, "CRB: ");
    u8g_DrawStr(&u8g,  30, h*3, u8g_u16toa(pq.crb.start, 3));    
    u8g_DrawStr(&u8g,  0, h*4, "Msg: ");
    u8g_DrawStr(&u8g,  30, h*4, u32toa(pq.processed_sentences, 9));
    u8g_DrawStr(&u8g,  30+30, h*3, u8g_u16toa(pq.crb.end, 3));    
    u8g_DrawStr(&u8g,  0, h*5, "Unsupported: ");
    u8g_DrawStr(&u8g,  60, h*5, pq.last_unknown_msg);
    
  }
  
  
  /* call the m2 draw procedure */
  m2_Draw();
}


/*=========================================================================*/
/* main procedure with u8g picture loop */

int main(void)
{
  uint8_t is_changed;
	
  /* setup u8g and m2 libraries */
  display_init();
  
  /* setup all other parts of the gps device */
  gps_init();

  /* application main loop */
  for(;;)
  {

    is_changed = update_gps_tracker_variables();
    
    m2_CheckKey();
    if ( m2_HandleKey() || is_changed ) 
    {
      /* picture loop */
      u8g_FirstPage(&u8g);
      do
      {
	draw();
        m2_CheckKey();
      } while( u8g_NextPage(&u8g) );
    }
    
    pq_ParseSentence(&pq);
  }  
}

