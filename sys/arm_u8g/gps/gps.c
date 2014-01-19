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
#include "exmem.h"
#include "config.h"

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
#ifdef USE_BATTERY_ADC  
  uint16_t tmp16;
#endif
  uint8_t is_changed = 0;

 
#ifdef USE_BATTERY_ADC  
  tmp16 = adc_get_value(2);
  tmp16 = low_pass( &gps_tracker_variables.adc_low_pass_z, tmp16, 7);
  
  if ( gps_tracker_variables.adc_battery != tmp16 )
  {
    // is_changed = 1;
    gps_tracker_variables.adc_battery = tmp16;
  }
#endif
  
  if ( gps_tracker_variables.sec_cnt != gps_tracker_variables.sec_cnt_raw )
  {
    is_changed = 1;
    gps_tracker_variables.sec_cnt = gps_tracker_variables.sec_cnt_raw;
  }
  
  if ( gps_tracker_variables.uart_byte_cnt_gui != gps_tracker_variables.uart_byte_cnt_raw )
  {
    //is_changed = 1;
    gps_tracker_variables.uart_byte_cnt_gui = gps_tracker_variables.uart_byte_cnt_raw;
  }
  
  return is_changed;
}

/*=========================================================================*/
/* create strings from provided position */

void create_gps_pos_string(gps_pos_t *pos)  __attribute__((noinline));
void create_gps_pos_string(gps_pos_t *pos)
{
  if ( gps_tracker_variables.pref.is_frac_mode == 0 )
  {
    pq_FloatToDegreeMinutes(&pq, pos->latitude);
    pq_DegreeMinutesToStr(&pq, 1, gps_tracker_variables.str_lat);

    pq_FloatToDegreeMinutes(&pq, pos->longitude);
    pq_DegreeMinutesToStr(&pq, 0, gps_tracker_variables.str_lon);    
  }
  else
  {
    pq_FloatToStr(pos->latitude, 1, gps_tracker_variables.str_lat);
    pq_FloatToStr(pos->longitude, 0, gps_tracker_variables.str_lon);
  }  

}

void create_gps_speed_course_time(void) __attribute__((noinline));
void create_gps_speed_course_time(void)
{
  gps_float_t kmh;    
  uint8_t h = pq.interface.hour;
  kmh = pq.interface.speed_in_knots * (gps_float_t)1.852;
  pq_itoa(gps_tracker_variables.speed, (uint16_t)kmh, 3);
  pq_itoa(gps_tracker_variables.course, (uint16_t)pq.interface.true_course, 3);
  
  h += gps_tracker_variables.pref.utc_offset;
  h += 12;
  h %= 24;
  
  pq_itoa(gps_tracker_variables.time, h, 2);
  pq_itoa(gps_tracker_variables.time+3, pq.interface.minute, 2);
  pq_itoa(gps_tracker_variables.time+6, pq.interface.second, 2);
  gps_tracker_variables.time[2] = ':';
  gps_tracker_variables.time[5] = ':';
  
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
  just calculate the direct view. Should be good for small distances.

  assumes
    gps_tracker_variables.map_origin
    gps_tracker_variables.map_e_w_factor
 to be set correctly (call set_map_origin_from_current_pos)
*/
void calculate_map_distance(gps_pos_t *pos) __attribute__ (( noinline ));
void calculate_map_distance(gps_pos_t *pos)
{
  gps_tracker_variables.n_s_distance = (pos->latitude - gps_tracker_variables.map_origin.latitude)*(gps_float_t)111000;
  gps_tracker_variables.e_w_distance = gps_tracker_variables.map_e_w_factor * (pos->longitude- gps_tracker_variables.map_origin.longitude);
}

/*
  calculate
      e_w_distance;
      n_s_distance;
      is_visible_on_map
  based on current map_origin
*/
void calculate_map_distance_and_visibility(gps_pos_t *pos) __attribute__ (( noinline ));
void calculate_map_distance_and_visibility(gps_pos_t *pos)
{
  calculate_map_distance(pos);
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

/*
  scale down distance, so that it fits to the screen.
  calculate
      e_w_distance;
      n_s_distance;
*/
void calculate_map_direction(gps_pos_t *pos) __attribute__ (( noinline ));
void calculate_map_direction(gps_pos_t *pos)
{
  gps_float_t max;
  calculate_map_distance(pos);
  
  gps_tracker_variables.is_visible_on_map = 1;
  max = gps_abs(gps_tracker_variables.n_s_distance);
  if ( max < gps_abs(gps_tracker_variables.e_w_distance) )
    max = gps_abs(gps_tracker_variables.e_w_distance);
  if ( max > gps_tracker_variables.half_map_size )
  {
    gps_tracker_variables.is_visible_on_map = 0;
    gps_tracker_variables.n_s_distance *= gps_tracker_variables.half_map_size;
    gps_tracker_variables.n_s_distance /= max;
    gps_tracker_variables.e_w_distance *= gps_tracker_variables.half_map_size;
    gps_tracker_variables.e_w_distance /= max;  
  }
}


void prepare_map(void)
{
}

#define MAP_OFFSET 6
#define MAP_SCALE_OFFSET 6
#define MAP_RADIUS 21
#define MAP_TARGET_RADIUS 2
#define MAP_COURSE_RADIUS 3
static void draw_map_mode_0(void)
{
  uint8_t i;
  gps_float_t half_map_pixel_size = MAP_RADIUS;
  gps_float_t course;
  u8g_int_t x_org = MAP_OFFSET+MAP_RADIUS;
  u8g_int_t y_org = MAP_OFFSET+MAP_RADIUS;
  u8g_int_t x,y;

  set_map_origin_from_current_pos();

  u8g_SetFont(&u8g, SMALL_FONT);
  u8g_SetDefaultForegroundColor(&u8g);
  
  u8g_DrawHLine(&u8g, MAP_OFFSET, MAP_OFFSET+ 2*MAP_RADIUS + MAP_SCALE_OFFSET, MAP_RADIUS);
  u8g_DrawVLine(&u8g, MAP_OFFSET, MAP_OFFSET+ 2*MAP_RADIUS + MAP_SCALE_OFFSET - 1, 3);
  u8g_DrawVLine(&u8g, MAP_OFFSET+MAP_RADIUS, MAP_OFFSET+ 2*MAP_RADIUS + MAP_SCALE_OFFSET - 1, 3);
  u8g_DrawStr(&u8g, MAP_OFFSET+MAP_RADIUS+2, MAP_OFFSET+ 2*MAP_RADIUS + MAP_SCALE_OFFSET + 4, gps_get_half_map_str());
  u8g_DrawCircle(&u8g,  x_org,y_org,MAP_RADIUS, U8G_DRAW_ALL);
  
  /* true_course == 0: geogr. north, counts clockwise (?) */
  /* true_course == 90: east */
  course = ((gps_float_t)(360+90))-pq.interface.true_course;
  course *= num._pi_div_180;
  while ( course > num.pi )
  {
    course -= num.pi;
    course -= num.pi;
  }
  
  //x = x_org + half_map_pixel_size*gps_cos(course);
  //y = y_org - half_map_pixel_size*gps_sin(course);
  x = half_map_pixel_size*gps_cos(course);
  y = half_map_pixel_size*gps_sin(course);
  //u8g_DrawCircle(&u8g,  x,y, MAP_COURSE_RADIUS, U8G_DRAW_ALL);
  u8g_DrawLine(&u8g, x_org + x/2, y_org -y/2,  x_org + x, y_org -y);
  
  
  /* loop through last positions, returned from the GPS receiver */
  for( i = 0; i < pq.cnt; i++ )
  {
    calculate_map_distance_and_visibility( &(pq.queue[i].pos) );
    if ( gps_tracker_variables.is_visible_on_map != 0 )
    {
      x = x_org + (gps_tracker_variables.e_w_distance * half_map_pixel_size)/gps_tracker_variables.half_map_size;
      y = y_org - (gps_tracker_variables.n_s_distance * half_map_pixel_size)/gps_tracker_variables.half_map_size;
      
      u8g_DrawPixel(&u8g,  x,y);
      
      if ( i > (PQ_LEN*2)/3 )
      {
	u8g_DrawPixel(&u8g,  x-1,y);
	u8g_DrawPixel(&u8g,  x+1,y);
	u8g_DrawPixel(&u8g,  x,y-1);
	u8g_DrawPixel(&u8g,  x,y+1); 
      }
      else if ( i > (PQ_LEN*1)/3 )
      {
	u8g_DrawPixel(&u8g,  x+1,y);	
      }
    }
  }
  
  /* loop through fixed positions */
  for( i = 0; i < MAP_POS_CNT; i++ )
  {
    if ( gps_tracker_variables.map_pos_list[i].map_symbol != 0 )
    {
      calculate_map_direction(&(gps_tracker_variables.map_pos_list[i].pos));
      x = x_org + (gps_tracker_variables.e_w_distance * half_map_pixel_size)/gps_tracker_variables.half_map_size;
      y = y_org - (gps_tracker_variables.n_s_distance * half_map_pixel_size)/gps_tracker_variables.half_map_size;
      
      if ( gps_tracker_variables.is_visible_on_map != 0 )
      {
	u8g_DrawGlyph(&u8g,  x+MAP_TARGET_RADIUS, y-MAP_TARGET_RADIUS, gps_tracker_variables.map_pos_list[i].map_symbol+'A'-1);
	u8g_DrawCircle(&u8g,  x,y,MAP_TARGET_RADIUS, U8G_DRAW_ALL);
      }
      else
      {
	u8g_DrawGlyph(&u8g,  x, y, gps_tracker_variables.map_pos_list[i].map_symbol+'A'-1);
      }
    }
  }
  
}

#define MODE_1_TOP_OFFSET 20
#define MODE_1_LINE 18
static void draw_map_mode_1(void)
{
  // gps_tracker_variables.gps_view_mode == 1

  u8g_SetDefaultForegroundColor(&u8g);
  
  create_gps_pos_string(&(pq.interface.pos));
  create_gps_speed_course_time();
  
  u8g_SetFont(&u8g, NORMAL_FONT);
  u8g_DrawStr(&u8g, 0, MODE_1_TOP_OFFSET, gps_tracker_variables.str_lat);
  u8g_DrawStr(&u8g, 0, MODE_1_TOP_OFFSET+1*MODE_1_LINE, gps_tracker_variables.str_lon);
  u8g_DrawStr(&u8g, 0, MODE_1_TOP_OFFSET+2*MODE_1_LINE, gps_tracker_variables.speed);
  u8g_DrawStr(&u8g, 24, MODE_1_TOP_OFFSET+2*MODE_1_LINE, gps_tracker_variables.time);
  u8g_SetFont(&u8g, SMALL_FONT);
  u8g_DrawStr(&u8g, 0, MODE_1_TOP_OFFSET-10, "Latitude");
  u8g_DrawStr(&u8g, 0, MODE_1_TOP_OFFSET-10+1*MODE_1_LINE, "Longitude");
  u8g_DrawStr(&u8g, 0, MODE_1_TOP_OFFSET-10+2*MODE_1_LINE, "km/h");
  u8g_DrawStr(&u8g, 24, MODE_1_TOP_OFFSET-10+2*MODE_1_LINE, "Time");

}

void draw_map(void)
{
  if ( gps_tracker_variables.gps_view_mode == 0 )
  {
    draw_map_mode_0();
  }
  else
  {
    draw_map_mode_1();
  }
}

/*=========================================================================*/

/*

  +++###++	0x01c
  +#######	0x07f
  +#+++++#	0x041
  +#+++++#
  +#+++++#
  +#+++++#
  +#+++++#
  +#######
  +#######
  +#######


  ++++++++	0x00
  #+++++++  0x80
  ##++++#+  0xc2
  +##++#++  0x64
  +####+++  0x78
  +####+++  0x78
  +#####++  0x7c
  +######+  0x7e
  +##+++##  0x63
  +##+++++  0x60

  ++++++++  0x00
  +++#++++  0x10
  #++#++#+	0x92
  +#+#+#++  0x54
  ++###+++  0x38
  +++#++++  0x10
  +++#++++  0x10
  +++#++++  0x10
  +++#++++  0x10
  +++#++++  0x10


*/

uint8_t battery_bitmap[10] = { 0x01c, 0x07f, 0x041, 0x041, 0x041, 0x041, 0x041, 0x041, 0x041, 0x07f };
//const uint8_t satellite_bitmap[10] = { 0x00, 0x080, 0x0c2, 0x064, 0x078, 0x078, 0x07c, 0x07e, 0x063, 0x060};
const uint8_t satellite_bitmap[10] = { 0x00, 0x010, 0x092, 0x054, 0x038, 0x010, 0x010, 0x010, 0x010, 0x010};

#define BATTERY_EMPTY_LEVEL_mV 1800

void draw_status(void)
{
#ifdef USE_BATTERY_ADC  
  int32_t i;
  int32_t battery;
  battery = (gps_tracker_variables.adc_battery*3300L);
  battery /= 1024;
  battery -= BATTERY_EMPTY_LEVEL_mV;
  if ( battery < 0 )
    battery = 0;
  battery *= 8;
  battery /= 3300L-BATTERY_EMPTY_LEVEL_mV;
  if ( battery >= 8 )
    battery = 7;
  for ( i = 0; i < 7; i++ )
  {
    if ( i < battery )
      battery_bitmap[8-i] = 0x07f;
    else
      battery_bitmap[8-i] = 0x041;
  }
  u8g_DrawBitmap(&u8g, u8g_GetWidth(&u8g) - 8, 0, 1, 10, battery_bitmap);
  u8g_DrawBitmap(&u8g, u8g_GetWidth(&u8g) - 27, 0, 1, 10, satellite_bitmap);
  u8g_SetFont(&u8g, SMALL_FONT);
  u8g_DrawStr(&u8g,  u8g_GetWidth(&u8g) - 20, 8, u8g_u16toa(pq.sat_cnt, 2));
#else
  u8g_DrawBitmap(&u8g, u8g_GetWidth(&u8g) - 27+10, 0, 1, 10, satellite_bitmap);
  u8g_SetFont(&u8g, SMALL_FONT);
  u8g_DrawStr(&u8g,  u8g_GetWidth(&u8g) - 20+10, 8, u8g_u16toa(pq.sat_cnt, 2));
#endif    
}


/*=========================================================================*/
/* menu definitions */

const char fmt_f8w32[] = "f8w30";
const char fmt_f4[] = "f4";
const char fmt_lat_lon_u32[] = "a1c7.4";
const char fmt_w4h4[] = "w4h4";
const char fmt_a1c3[] = "a1c3";
const char fmt_a1c53[] = "a1c5.3";
const char fmt_W64f8[] = "W64f8";



void fn_ok(m2_el_fnarg_p fnarg) {
  /* accept selection */
}

void fn_cancel(m2_el_fnarg_p fnarg) {
  /* discard selection */
}


/*=== extern declarations ===*/
M2_EXTERN_ALIGN(el_home);
M2_EXTERN_ALIGN(top_el_ml);

/*=== reuseable elements ===*/

M2_ROOT(el_goto_home, fmt_f8w32, "Home", &el_home);

M2_SPACE(el_space4, fmt_w4h4);


/*=== GPS Data Entry (fractional notation) ===*/

const char gps_text_E[] = "E";
const char gps_text_W[] = "W";
const char gps_text_N[] = "N";
const char gps_text_S[] = "S";

const char *combo_symbol(uint8_t idx)
{
  static char s[2];
  if ( idx == 0 )
    s[0] = 32;
  else
    s[0] = idx - 1 + 'A';
  return s;
}

const char *combo_fn_n_s(uint8_t idx)
{
  if ( idx == 0 )
    return gps_text_N;
  return gps_text_S;
}

const char *combo_fn_e_w(uint8_t idx)
{
  if ( idx == 0 )
    return gps_text_E;
  return gps_text_W;
}

M2_LABEL(el_gps_symbol_label, NULL, "Sym: ");
M2_COMBO(el_gps_symbol_combo, "w10", &gps_tracker_variables.gps_symbol, 27, combo_symbol);

// M2_LABEL(el_gps_text_label, NULL, "Inf: ");
// M2_TEXT(el_gps_text, "a1", &gps_tracker_variables.gps_pos_text, 4);

M2_LABEL(el_gps_frac_lat_label, NULL, "Lat: ");
M2_U32NUM(el_gps_frac_lat_num, fmt_lat_lon_u32, &gps_tracker_variables.gps_frac_lat);
M2_COMBO(el_gps_frac_lat_n_s, "w10", &gps_tracker_variables.gps_frac_lat_n_s, 2, combo_fn_n_s);
M2_LABEL(el_gps_frac_lon_label, NULL, "Lon: ");
M2_U32NUM(el_gps_frac_lon_num, fmt_lat_lon_u32, &gps_tracker_variables.gps_frac_lon);
M2_COMBO(el_gps_frac_lon_e_w, "w10", &gps_tracker_variables.gps_frac_lon_e_w, 2, combo_fn_e_w);
M2_LIST(list_gps_frac) = {
  &el_gps_symbol_label,
  &el_gps_symbol_combo,
  &m2_null_element,
  
//  &el_gps_text_label,
//  &el_gps_text,
//  &m2_null_element,
  
  &el_gps_frac_lat_label,
  &el_gps_frac_lat_num,
  &el_gps_frac_lat_n_s,
  
  &el_gps_frac_lon_label,
  &el_gps_frac_lon_num,
  &el_gps_frac_lon_e_w
};
M2_GRIDLIST(el_gps_frac_grid, "c3", list_gps_frac);

void cb_gps_frac_load_pos(m2_el_fnarg_p fnarg) 
{
  /*
    read from gps_tracker_variables.m2_gps_pos
    write to gps_tracker_variables.gps_frac_lat_n_s and others
  */
  m2_gps_pos_to_frac_fields();
}
M2_SPACECB(el_gps_frac_space4, fmt_w4h4, cb_gps_frac_load_pos);

void cb_gps_frac_ok(m2_el_fnarg_p fnarg) 
{
  if ( gps_tracker_variables.pref.is_frac_mode != 0 )
  {
    m2_frac_fields_to_gps_pos();
  }
  else
  {
    m2_sexa_fields_to_gps_pos();
  }
  gps_tracker_variables.map_pos_list[gps_tracker_variables.map_pos_idx].map_symbol = gps_tracker_variables.gps_symbol;
  gps_tracker_variables.map_pos_list[gps_tracker_variables.map_pos_idx].pos = gps_tracker_variables.m2_gps_pos;
  write_MapPos(gps_tracker_variables.map_pos_idx, &(gps_tracker_variables.map_pos_list[gps_tracker_variables.map_pos_idx]));
  m2_SetRoot(&top_el_ml);
}

void cb_gps_frac_zero(m2_el_fnarg_p fnarg) 
{
  gps_tracker_variables.gps_symbol = 0;
  gps_tracker_variables.gps_frac_lat = 0;
  gps_tracker_variables.gps_grad_lat = 0;
  gps_tracker_variables.gps_frac_lat_n_s = 0;
  gps_tracker_variables.gps_frac_lon = 0;
  gps_tracker_variables.gps_grad_lon = 0;
  gps_tracker_variables.gps_frac_lon_e_w = 0;
}

void cb_gps_frac_current(m2_el_fnarg_p fnarg) 
{
  gps_tracker_variables.m2_gps_pos = pq.interface.pos;
  if ( gps_tracker_variables.pref.is_frac_mode != 0 )
  {
    m2_gps_pos_to_frac_fields();  
  }
  else
  {
    m2_gps_pos_to_sexa_fields();
  }
}

M2_BUTTON(el_gps_frac_ok, fmt_f4, "Ok", cb_gps_frac_ok);
M2_ROOT(el_gps_frac_cancel, fmt_f4, "Cancel", &top_el_ml);
M2_BUTTON(el_gps_frac_default, fmt_f4, "0", cb_gps_frac_zero);
M2_BUTTON(el_gps_frac_current, fmt_f4, "X", cb_gps_frac_current);
M2_LIST(list_gps_frac_btns) = {
  &el_gps_frac_ok,
  &el_gps_frac_cancel,
  &el_gps_frac_default,
  &el_gps_frac_current
};
M2_HLIST(el_gps_frac_btns, NULL, list_gps_frac_btns);

M2_LIST(list_gps_frac_vlist) = {
  &el_gps_frac_grid,
  &el_gps_frac_space4,
  &el_gps_frac_btns
};
M2_VLIST(el_gps_frac_vlist, NULL, list_gps_frac_vlist);
M2_ALIGN(top_el_gps_frac, NULL, &el_gps_frac_vlist);

/*=== GPS Data Entry (sexagesimal notation) ===*/

M2_U32NUM(el_gps_sexa_lat_grad, fmt_a1c3, &gps_tracker_variables.gps_grad_lat);
M2_U32NUM(el_gps_sexa_lat_num, fmt_a1c53, &gps_tracker_variables.gps_frac_lat);
M2_U32NUM(el_gps_sexa_lon_grad, fmt_a1c3, &gps_tracker_variables.gps_grad_lon);
M2_U32NUM(el_gps_sexa_lon_num, fmt_a1c53, &gps_tracker_variables.gps_frac_lon);

M2_LIST(list_gps_sexa) = {
  &el_gps_symbol_label,
  &el_gps_symbol_combo,
  &m2_null_element,
  &m2_null_element,
  
  &el_gps_frac_lat_label,
  &el_gps_frac_lat_n_s,
  &el_gps_sexa_lat_grad,
  &el_gps_sexa_lat_num,
  
  &el_gps_frac_lon_label,
  &el_gps_frac_lon_e_w,
  &el_gps_sexa_lon_grad,
  &el_gps_sexa_lon_num
};
M2_GRIDLIST(el_gps_sexa_grid, "c4", list_gps_sexa);

void cb_gps_sexa_load_pos(m2_el_fnarg_p fnarg) 
{
  /*
    read from gps_tracker_variables.m2_gps_pos
    write to gps_tracker_variables.gps_frac_lat_n_s and others
  */
  m2_gps_pos_to_sexa_fields();
}
M2_SPACECB(el_gps_sexa_space4, fmt_w4h4, cb_gps_sexa_load_pos);


M2_LIST(list_gps_sexa_vlist) = {
  &el_gps_sexa_grid,
  &el_gps_sexa_space4,
  &el_gps_frac_btns
};
M2_VLIST(el_gps_sexa_vlist, NULL, list_gps_sexa_vlist);
M2_ALIGN(top_el_gps_sexa, NULL, &el_gps_sexa_vlist);

/*=== edit special map position list ===*/

void map_pos_update(void)
{
  gps_tracker_variables.gps_symbol = gps_tracker_variables.map_pos_list[gps_tracker_variables.map_pos_idx].map_symbol;
  gps_tracker_variables.str_idx_and_symbol[0] = gps_tracker_variables.map_pos_idx+'0';
  gps_tracker_variables.str_idx_and_symbol[1] = '\0';
  
  if ( gps_tracker_variables.gps_symbol > 0 )
  {
    gps_tracker_variables.str_idx_and_symbol[1] = '/';
    gps_tracker_variables.str_idx_and_symbol[2] = gps_tracker_variables.gps_symbol + 'A'-1;
    gps_tracker_variables.str_idx_and_symbol[3] = '\0';
  }
  
  create_gps_pos_string(&(gps_tracker_variables.map_pos_list[gps_tracker_variables.map_pos_idx].pos));
    
  /*
  if ( gps_tracker_variables.pref.is_frac_mode == 0 )
  {
    pq_FloatToDegreeMinutes(&pq, gps_tracker_variables.map_pos_list[gps_tracker_variables.map_pos_idx].pos.latitude);
    pq_DegreeMinutesToStr(&pq, 1, gps_tracker_variables.str_lat);

    pq_FloatToDegreeMinutes(&pq, gps_tracker_variables.map_pos_list[gps_tracker_variables.map_pos_idx].pos.longitude);
    pq_DegreeMinutesToStr(&pq, 0, gps_tracker_variables.str_lon);    
  }
  else
  {
    pq_FloatToStr(gps_tracker_variables.map_pos_list[gps_tracker_variables.map_pos_idx].pos.latitude, 1, gps_tracker_variables.str_lat);
    pq_FloatToStr(gps_tracker_variables.map_pos_list[gps_tracker_variables.map_pos_idx].pos.longitude, 0, gps_tracker_variables.str_lon);
  }
  */
}

M2_LABEL(el_ml_idx_label, NULL, "Idx: ");
//M2_U32NUM(el_ml_idx_num, "c1r1", &gps_tracker_variables.map_pos_idx);
M2_LABEL(el_ml_idx_num, NULL, gps_tracker_variables.str_idx_and_symbol);
//M2_LABEL(el_ml_lat_label, NULL, "Lat: ");
M2_LABEL(el_ml_lat, NULL, gps_tracker_variables.str_lat);
//M2_LABEL(el_ml_lon_label, NULL, "Lon: ");
M2_LABEL(el_ml_lon, NULL, gps_tracker_variables.str_lon);

M2_LIST(list_ml_grid) = {
  &el_ml_idx_label,
  &el_ml_idx_num,
  &el_gps_frac_lat_label,
  &el_ml_lat,
  &el_gps_frac_lon_label,
  &el_ml_lon,
};
M2_GRIDLIST(el_ml_grid, "c2", list_ml_grid);

void ml_dec(m2_el_fnarg_p fnarg) 
{
  if ( gps_tracker_variables.map_pos_idx == 0 )
    gps_tracker_variables.map_pos_idx = MAP_POS_CNT;
  gps_tracker_variables.map_pos_idx--;
  map_pos_update();
}

void ml_inc(m2_el_fnarg_p fnarg) 
{
  gps_tracker_variables.map_pos_idx++;
  if ( gps_tracker_variables.map_pos_idx >= MAP_POS_CNT )
    gps_tracker_variables.map_pos_idx = 0;
  map_pos_update();
}

void ml_edit(m2_el_fnarg_p fnarg) 
{
  gps_tracker_variables.m2_gps_pos = gps_tracker_variables.map_pos_list[gps_tracker_variables.map_pos_idx].pos;
  
  if ( gps_tracker_variables.pref.is_frac_mode == 0 )
  {
    m2_SetRoot(&top_el_gps_sexa);
  }
  else
  {
    m2_SetRoot(&top_el_gps_frac);
  }
}

M2_SPACECB(el_ml_space4, fmt_w4h4, map_pos_update);		/* gcc warning is ok here */

M2_BUTTON(el_ml_inc, "f12w10", "+", ml_inc);
M2_BUTTON(el_ml_dec, "f12w10", "-", ml_dec);
M2_BUTTON(el_ml_edit, fmt_f4, "Edit", ml_edit);
M2_ROOT(el_ml_home, fmt_f4, "Home", &el_home);

M2_LIST(list_ml_btns) = {
  &el_ml_inc,
  &el_ml_dec,
  &el_ml_edit,
  &el_ml_home
};
M2_HLIST(el_ml_btns, NULL, list_ml_btns);

M2_LIST(list_ml_vlist) = {
  &el_ml_grid,
  &el_ml_space4,
  &el_ml_btns
};
M2_VLIST(el_ml_vlist, NULL, list_ml_vlist);
M2_ALIGN(top_el_ml, NULL, &el_ml_vlist);



/*=== Info Menu ===*/

/*=== show battery ===*/

#ifdef USE_BATTERY_ADC
M2_ALIGN(el_show_battery, "|0", &el_goto_home);
#endif

/*=== show system ===*/

M2_ALIGN(el_show_system, "|0", &el_goto_home);

/*=== show GPS UART ===*/

M2_ALIGN(el_show_gps_uart, "|0", &el_goto_home);

/*=== show GPS Stat ===*/

M2_ALIGN(el_show_gps_stat, "|0", &el_goto_home);


#ifdef USE_BATTERY_ADC
M2_ROOT(el_info_show_battery, fmt_W64f8, "Battery", &el_show_battery);
#endif
M2_ROOT(el_info_show_system, fmt_W64f8, "System Info", &el_show_system);
M2_ROOT(el_info_show_gps_uart, fmt_W64f8, "GPS UART", &el_show_gps_uart);
M2_ROOT(el_info_show_gps_stat, fmt_W64f8, "GPS Status", &el_show_gps_stat);
M2_ROOT(el_info_back, fmt_W64f8, "Home", &el_home);
M2_LIST(list_info) = {
#ifdef USE_BATTERY_ADC
  &el_info_show_battery,
#endif
  &el_info_show_system,
  &el_info_show_gps_uart,
  &el_info_show_gps_stat,
  &el_info_back
};
M2_VLIST(el_info_vlist, NULL, list_info);
M2_ALIGN(top_el_info, "|0", &el_info_vlist);


/*=== map ===*/

void btn_cb_map_mode(m2_el_fnarg_p fnarg)
{
  gps_tracker_variables.gps_view_mode++;
  if ( gps_tracker_variables.gps_view_mode >= GPS_VIEW_MODE_CNT )
    gps_tracker_variables.gps_view_mode = 0;
}

M2_BUTTON(el_map_mode, fmt_f8w32, "Mode", btn_cb_map_mode);

void btn_cb_map_zoom_dec(m2_el_fnarg_p fnarg)
{
  gps_dec_half_map_size();
}

M2_BUTTON(el_map_zoom_dec, fmt_f8w32, "In", btn_cb_map_zoom_dec);

void btn_cb_map_zoom_inc(m2_el_fnarg_p fnarg)
{  
  gps_inc_half_map_size();
}

M2_BUTTON(el_map_zoom_inc, fmt_f8w32, "Out", btn_cb_map_zoom_inc);

M2_LIST(list_map) = {
  &el_map_mode,
  &el_map_zoom_dec,
  &el_map_zoom_inc,
  &el_goto_home
};

M2_VLIST(el_map_vlist, NULL, list_map);
M2_ALIGN(el_map, "|0-2", &el_map_vlist);

/*=== preferences ===*/

const char str_sexa[] = "sexa";
const char str_frac[] = "frac";

const char *combo_fn_frac_or_sexa(uint8_t idx)
{
  if ( idx == 0 )
    return str_sexa;
  return str_frac;
}

const char *combo_fn_utc_offset(uint8_t idx)
{
  char *s = gps_tracker_variables.str_utc_offset;
  if ( idx < 12 )
  {
    *s = '-';
    s++;
    idx = 12-idx;
  }
  else
  {
    idx -= 12;
  }
  pq_itoa(s, idx, 2);
  return gps_tracker_variables.str_utc_offset;
}

void btn_cb_save_pref(m2_el_fnarg_p fnarg)
{
  write_Preferences();
  m2_SetRoot(&el_home);
}



M2_LABEL(el_pref_frac_sexa_label, NULL, "Notation:");
M2_COMBO(el_pref_frac_sexa, "w30", &gps_tracker_variables.pref.is_frac_mode, 2, combo_fn_frac_or_sexa);
M2_LABEL(el_pref_utc_offset_label, NULL, "UTC offset:");
M2_COMBO(el_pref_utc_offset, "w30", &gps_tracker_variables.pref.utc_offset, 25, combo_fn_utc_offset);

M2_ROOT(el_pref_goto_home, "f4", "Home", &el_home);
M2_BUTTON(el_pref_save, "f4", "Save", btn_cb_save_pref);

M2_LIST(list_pref) = {
  &el_pref_frac_sexa_label,
  &el_pref_frac_sexa,
  &el_pref_utc_offset_label, 
  &el_pref_utc_offset,
  &el_pref_goto_home,
  &el_pref_save
};
M2_GRIDLIST(el_pref_grid, "c2", list_pref);

/*
M2_LIST(list_pref_vlist) = {
  &el_pref_grid,
  &el_space4,
  &el_pref_goto_home
};
M2_VLIST(el_pref_vlist, NULL, list_pref_vlist);
M2_ALIGN(top_el_pref, NULL, &el_pref_vlist);
*/
M2_ALIGN(top_el_pref, NULL, &el_pref_grid);

/*=== toplevel menu ===*/

M2_ROOT(el_home_map, fmt_W64f8, "GPS" , &el_map);
M2_ROOT(el_home_pos_list, fmt_W64f8, "List", &top_el_ml);
M2_ROOT(el_home_preferences, fmt_W64f8, "Preferences", &top_el_pref);
M2_ROOT(el_home_sys_info, fmt_W64f8, "System", &top_el_info);
M2_LIST(list_home) = {
  &el_home_map,
  &el_home_pos_list,
  &el_home_preferences,
  &el_home_sys_info,
  &el_space4
};
M2_VLIST(el_home_vlist, NULL, list_home);
M2_ALIGN(el_home, "|0", &el_home_vlist);


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
  
  //u8g_InitComFn(&u8g, &u8g_dev_uc1701_dogs102_hw_spi, u8g_com_hw_spi_fn);
  u8g_InitComFn(&u8g, &u8g_dev_uc1701_dogs102_2x_hw_spi, u8g_com_hw_spi_fn);
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
  u8g_SetDefaultForegroundColor(&u8g);
  draw_status();
#ifdef USE_BATTERY_ADC  
  if ( m2_GetRoot() == &el_show_battery )
  {
    u8g_SetFont(&u8g, NORMAL_FONT);
    u8g_DrawStr(&u8g,  0, 12, "Battery Status");
    u8g_DrawStr(&u8g,  0, 12*2, "Raw: ");
    u8g_DrawStr(&u8g,  30, 12*2, u8g_u16toa(gps_tracker_variables.adc_battery, 4));
    u8g_DrawStr(&u8g,  0, 12*3, "mV: ");
    u8g_DrawStr(&u8g,  30, 12*3, u8g_u16toa((gps_tracker_variables.adc_battery*3300UL)/1024, 4));    
  }
  else 
#endif
  if ( m2_GetRoot() == &el_show_system )
  {
    uint32_t h = 8;
    u8g_SetFont(&u8g, SMALL_FONT);
    u8g_DrawStr(&u8g,  0, h, "System Info:");
    u8g_DrawStr(&u8g,  0, h*2, "Clk: ");
    u8g_DrawStr(&u8g,  30, h*2, u32toa(gps_tracker_variables.sec_cnt, 9));
    u8g_DrawStr(&u8g,  0, h*3, "Stack: ");
    u8g_DrawStr(&u8g,  30, h*3, u32toa(stackmon_GetUsage(), 9));
    u8g_DrawStr(&u8g,  0, h*4, "Top: ");
    u8g_DrawStr(&u8g,  30, h*4, u32toa(stackmon_upper_limit, 9));
    u8g_DrawStr(&u8g,  0, h*5, "Low: ");
    u8g_DrawStr(&u8g,  30, h*5, u32toa(stackmon_start_adr, 9));
    u8g_DrawStr(&u8g,  0, h*6, "EE: ");
    u8g_DrawStr(&u8g,  30, h*6, u32toa(1-is_exmem_not_available, 1));
    
  }
  
  else if ( m2_GetRoot() == &el_show_gps_uart ) 
  {
    uint32_t h = 8;
    u8g_SetFont(&u8g, SMALL_FONT);
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
    u8g_DrawStr(&u8g,  0, h*6, "Clk: ");
    u8g_DrawStr(&u8g,  30, h*6, u32toa(gps_tracker_variables.sec_cnt, 9));
  }
  else if ( m2_GetRoot() == &el_show_gps_stat )
  {
    uint32_t h = 7;

    create_gps_pos_string(&(pq.interface.pos));
    create_gps_speed_course_time();

    u8g_SetFont(&u8g, SMALL_FONT);
    u8g_DrawStr(&u8g,  0, h, "GPS Status");
    u8g_DrawStr(&u8g,  0, h*2, "Sat: ");
    u8g_DrawStr(&u8g,  30, h*2, u8g_u16toa(pq.sat_cnt, 2));
    
    u8g_DrawStr(&u8g,  0, h*3, "Course: ");
    u8g_DrawStr(&u8g,  35, h*3, gps_tracker_variables.course);
    u8g_DrawStr(&u8g,  58, h*3, "km/h: ");
    u8g_DrawStr(&u8g,  83, h*3, gps_tracker_variables.speed);
    
    u8g_DrawStr(&u8g,  0, h*4, gps_tracker_variables.str_lat);
    u8g_DrawStr(&u8g,  0, h*5, gps_tracker_variables.str_lon);
    /*
    u8g_DrawStr(&u8g,  0, h*6, flon);
    u8g_DrawStr(&u8g,  0, h*7, lon);
    */
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
	
  stackmon_Init();
  
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

