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

#include <math.h>
#include "u8g.h"
#include "u8g_arm.h"
#include "m2.h"
#include "m2ghu8g.h"
#include "adc.h"
#include "init.h"
#include "util.h"

/*=========================================================================*/
/* global variables and objects */

u8g_t u8g;


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

struct float_num_t{
  float radius_earth;
  float pi;
  float _pi_div_180;
  float _2_x_pi;
  float _pi_div_2;
  float _4_div_pi;  /* 4.0/num.pi */
  float _m4_div_pi_x_pi; 	/* -4.0/(num.pi*num.pi) */
  float _0225;
};
const struct float_num_t num = { 6372795.0f, 3.14159265f, 0.0174532925f , 6.28318530f, 1.570796325f, 1.2732395461f,  -0.4052847354f, 0.225f};

float gps_abs(float x) __attribute__((noinline));
float gps_abs(float x)
{
  if ( x > 0.0 )
    return x;
  return -x;
}

/*
sin/cos calculation taken from
http://devmaster.net/posts/9648/fast-and-accurate-sine-cosine
*/
float gps_sin(float x) __attribute__((noinline));
float gps_sin(float x)
{
    float y;
    y =  num._4_div_pi * x + num._m4_div_pi_x_pi * x * gps_abs(x);
    y = num._0225 * (y * gps_abs(y) - y) + y; 
    return y;
}

float gps_cos(float x) __attribute__((noinline));
float gps_cos(float x)
{
  x += num._pi_div_2;
  if ( x >= num.pi ) 
    x-= num._2_x_pi;
  return gps_sin(x);
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

/*
M_PI
M_TWOPI
*/

// returns distance in meters between two positions, both specified
// as signed decimal-degrees latitude and longitude. Uses great-circle
// distance computation for hypothised sphere of radius 6372795 meters.
// Because Earth is no exact sphere, rounding errors may be upto 0.5%.
// Source:  Maarten Lamers, http://www.maartenlamers.com/nmea/
/*
gps_float_t  distance(gps_float_t  lat1, gps_float_t  long1, gps_float_t lat2, gps_float_t long2) 
{
  gps_float_t delta, sdlong, cdlong, slat1, clat1, slat2, clat2, denom;
  
  delta = long1-long2;
  delta *= M_PI;
  delta /= 180;
  sdlong = sin(delta);
  cdlong = cos(delta);
  lat1 *= M_PI;
  lat1 /= 180;
  lat2 *= M_PI;
  lat2 /= 180;
  slat1 = sin(lat1);
  clat1 = cos(lat1);
  slat2 = sin(lat2);
  clat2 = cos(lat2);
  delta = (clat1 * slat2) - (slat1 * clat2 * cdlong);
  delta = sq(delta);
  delta += sq(clat2 * sdlong);
  delta = sqrt(delta);
  denom = (slat1 * slat2) + (clat1 * clat2 * cdlong);
  delta = atan2(delta, denom);
  return delta * 6372795;
}
*/

/*
  N-S distance
  (lat1 - lat2) * 111000 

  E-W distance 
  cos(lat * M_PI / 180) * 6372795 * M_PI / 180  * (long1 - long2)
*/

void set_map_origin_from_current_pos(void)
{
  gps_tracker_variables.map_origin = pq.interface.pos;
  gps_tracker_variables.map_e_w_factor = gps_cos(pq.interface.pos.latitude*num._pi_div_180) * num.radius_earth * num._pi_div_180;
}

/*
  calculate
      e_w_distance;
      n_s_distance;
      is_visible_on_map
  based on current map_origin
*/
void calculate_map_distance(gps_pos_t *pos)
{
  gps_tracker_variables.n_s_distance = (pos->latitude - gps_tracker_variables.map_origin.latitude)*(gps_float_t)111000;
  gps_tracker_variables.e_w_distance = gps_tracker_variables.map_e_w_factor * (pos->longitude- gps_tracker_variables.map_origin.longitude);
  
  gps_tracker_variables.is_visible_on_map = 1;
  if ( gps_tracker_variables.n_s_distance > gps_tracker_variables.half_map_size )
    gps_tracker_variables.is_visible_on_map = 0;
  if ( gps_tracker_variables.n_s_distance < -gps_tracker_variables.half_map_size )
    gps_tracker_variables.is_visible_on_map = 0;
  if ( gps_tracker_variables.e_w_distance > gps_tracker_variables.half_map_size )
    gps_tracker_variables.is_visible_on_map = 0;
  if ( gps_tracker_variables.e_w_distance < -gps_tracker_variables.half_map_size )
    gps_tracker_variables.is_visible_on_map = 0;
}


void prepare_map(void)
{
}

#define MAP_RADIUS 30
void draw_map(void)
{
  uint8_t i;
  gps_float_t half_map_pixel_size = MAP_RADIUS;
  u8g_int_t x_org = 51;
  u8g_int_t y_org = 32;
  u8g_int_t x,y;

  set_map_origin_from_current_pos();
  
  u8g_DrawCircle(&u8g,  x_org,y_org,MAP_RADIUS, U8G_DRAW_ALL);
  for( i = 0; i < pq.cnt; i++ )
  {
    calculate_map_distance( &(pq.queue[i].pos) );
    if ( gps_tracker_variables.is_visible_on_map != 0 )
    {
      x = x_org + (gps_tracker_variables.e_w_distance * half_map_pixel_size)/gps_tracker_variables.half_map_size;
      y = y_org - (gps_tracker_variables.e_w_distance * half_map_pixel_size)/gps_tracker_variables.half_map_size;
      u8g_DrawPixel(&u8g,  x,y);
      u8g_DrawPixel(&u8g,  x-1,y);
      u8g_DrawPixel(&u8g,  x+1,y);
      u8g_DrawPixel(&u8g,  x,y-1);
      u8g_DrawPixel(&u8g,  x,y+1);      
    }
  }
}


/*=========================================================================*/
/* menu definitions */

void fn_ok(m2_el_fnarg_p fnarg) {
  /* accept selection */
}

void fn_cancel(m2_el_fnarg_p fnarg) {
  /* discard selection */
}


/*=== extern declarations ===*/
M2_EXTERN_ALIGN(el_home);

/*=== reuseable elements ===*/

M2_ROOT(el_goto_home, NULL, "Home", &el_home);


/*=== Info Menu ===*/

/*=== show battery ===*/

M2_ALIGN(el_show_battery, "|0", &el_goto_home);

/*=== show GPS UART ===*/

M2_ALIGN(el_show_gps_uart, "|0", &el_goto_home);

/*=== show GPS Stat ===*/

M2_ALIGN(el_show_gps_stat, "|0", &el_goto_home);



M2_ROOT(el_info_show_battery, NULL, "Battery", &el_show_battery);
M2_ROOT(el_info_show_gps_uart, NULL, "GPS UART", &el_show_gps_uart);
M2_ROOT(el_info_show_gps_stat, NULL, "GPS Status", &el_show_gps_stat);
M2_ROOT(el_info_back, NULL, "Back", &el_home);
M2_LIST(list_info) = {
  &el_info_show_battery,
  &el_info_show_gps_uart,
  &el_info_show_gps_stat,
  &el_info_back
};
M2_VLIST(el_info_vlist, NULL, list_info);
M2_ALIGN(top_el_info, NULL, &el_info_vlist);




/*=== map ===*/

M2_ALIGN(el_map, "|0", &el_goto_home);

/*=== test compass ===*/

M2_ALIGN(el_test_compass, "|0", &el_goto_home);

/*=== toplevel menu ===*/

M2_ROOT(el_home_map, NULL, "MAP" , &el_map);
M2_ROOT(el_home_test_compass, NULL, "Compass", &el_test_compass);
M2_ROOT(el_home_sys_info, NULL, "System Info", &top_el_info);
M2_LIST(list_home) = {
  &el_home_map,
  &el_home_test_compass,
  &el_home_sys_info
};
M2_VLIST(el_home_vlist, NULL, list_home);
M2_ALIGN(el_home, NULL, &el_home_vlist);


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
  m2_SetFont(0, (const void *)NORMAL_FONT);
	
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
    u8g_SetFont(&u8g, NORMAL_FONT);
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
    u8g_SetFont(&u8g, SMALL_FONT);
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
  else if ( m2_GetRoot() == &el_show_gps_stat )
  {
    uint32_t h = 7;

    char lat[16];
    char lon[16];

    char flat[12];
    char flon[12];
    char speed[4];
    char course[4];
    
    gps_float_t kmh;
    
    kmh = pq.interface.speed_in_knots * (gps_float_t)1.852;
    pq_itoa(speed, (uint16_t)kmh, 3);

    pq_itoa(course, (uint16_t)pq.interface.true_course, 3);
    
    pq_FloatToDegreeMinutes(&pq, pq.interface.pos.latitude);
    pq_DegreeMinutesToStr(&pq, 1, lat);
    pq_FloatToStr(pq.interface.pos.latitude, flat);
    
    pq_FloatToDegreeMinutes(&pq, pq.interface.pos.longitude);
    pq_DegreeMinutesToStr(&pq, 0, lon);
    pq_FloatToStr(pq.interface.pos.longitude, flon);
    
    
    u8g_SetFont(&u8g, SMALL_FONT);
    u8g_SetDefaultForegroundColor(&u8g);
    u8g_DrawStr(&u8g,  0, h, "GPS Status");
    u8g_DrawStr(&u8g,  0, h*2, "Sat: ");
    u8g_DrawStr(&u8g,  30, h*2, u8g_u16toa(pq.sat_cnt, 2));
    
    u8g_DrawStr(&u8g,  0, h*3, "Course: ");
    u8g_DrawStr(&u8g,  35, h*3, course);
    u8g_DrawStr(&u8g,  58, h*3, "km/h: ");
    u8g_DrawStr(&u8g,  83, h*3, speed);
    
    u8g_DrawStr(&u8g,  0, h*4, flat);
    u8g_DrawStr(&u8g,  0, h*5, lat);
    u8g_DrawStr(&u8g,  0, h*6, flon);
    u8g_DrawStr(&u8g,  0, h*7, lon);
    
  }    
  else if ( m2_GetRoot() == &el_map )
  {
    draw_map();
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

