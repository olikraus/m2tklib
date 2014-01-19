/*

  init.c 
  
  Init procedure of the GPS tracker device.
  
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


#include "LPC11xx.h"
#include "u8g_arm.h"
#include "uart.h"
#include "init.h"
#include "exmem.h"
#include "config.h"


/*========================================================================*/
/* variables of the gps tracker device */

struct gps_tracker_variables_struct gps_tracker_variables;


/*========================================================================*/
/* SystemInit & SysTick Interrupt */

#define SYS_TICK_PERIOD_IN_MS 10

void SystemInit()
{    
  init_system_clock();		/* SystemCoreClock will be set here */
  
  /* SysTick is defined in core_cm0.h */
  SysTick->LOAD = (SystemCoreClock/1000UL*(unsigned long)SYS_TICK_PERIOD_IN_MS) - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = 7;   /* enable, generate interrupt (SysTick_Handler), do not divide by 2 */
}

void __attribute__ ((interrupt, used)) SysTick_Handler(void)
{
  gps_tracker_variables.cnt_10ms++;
  if ( gps_tracker_variables.cnt_10ms >= 100 )
  {
    gps_tracker_variables.cnt_10ms = 0;
    gps_tracker_variables.sec_cnt_raw++;    
  }
}

/*========================================================================*/
/* setup gps device */

pq_t pq;

void gps_rx(uint8_t value)
{
  gps_tracker_variables.uart_byte_cnt_raw++;
  pq_AddChar(&pq, (uint8_t)value);
}

#define GPS_HALF_MAP_CNT 9
const char *gps_half_map_str[GPS_HALF_MAP_CNT] = { "5m", "10m", "20m", "50m", "100m", "200m", "500m", "1km", "2km" };
uint16_t gps_half_map_len[GPS_HALF_MAP_CNT] = { 5, 10, 20, 50, 100, 200, 500, 1000, 2000 };


void gps_set_half_map_size_by_index(uint8_t idx)
{
  if ( idx >= GPS_HALF_MAP_CNT )
    idx = GPS_HALF_MAP_CNT-1;
  gps_tracker_variables.half_map_size_index = idx;
  gps_tracker_variables.half_map_size = gps_half_map_len[idx];
  gps_tracker_variables.gps_view_mode = 0;	/* switch to map mode */
}

void gps_inc_half_map_size(void)
{
  if ( gps_tracker_variables.half_map_size_index < GPS_HALF_MAP_CNT-1 )
    gps_tracker_variables.half_map_size_index++;
    
  /*
  gps_tracker_variables.half_map_size_index++;
  if ( gps_tracker_variables.half_map_size_index >= GPS_HALF_MAP_CNT )
    gps_tracker_variables.half_map_size_index = 0;
  */
  
  gps_set_half_map_size_by_index(gps_tracker_variables.half_map_size_index);
}

void gps_dec_half_map_size(void)
{
  if ( gps_tracker_variables.half_map_size_index > 0 )
    gps_tracker_variables.half_map_size_index--;
  /*
  if ( gps_tracker_variables.half_map_size_index == 0 )
    gps_tracker_variables.half_map_size_index = GPS_HALF_MAP_CNT;
  gps_tracker_variables.half_map_size_index--;
  */
  
  gps_set_half_map_size_by_index(gps_tracker_variables.half_map_size_index);
}

const char *gps_get_half_map_str(void)
{
  return gps_half_map_str[gps_tracker_variables.half_map_size_index];
}

gps_float_t tenthousand = 10000.0;

/* convert from m2_gps_pos to m2 fields (frac notation) */
void m2_gps_pos_to_frac_fields(void)
{
  gps_float_t f;
  
  f = gps_tracker_variables.m2_gps_pos.latitude;
  
  gps_tracker_variables.gps_frac_lat_n_s = 0;
  if ( f < 0 )
  {
    f = -f;
    gps_tracker_variables.gps_frac_lat_n_s = 1;
  }
  f *=tenthousand;
  gps_tracker_variables.gps_frac_lat = (long)f;

  f = gps_tracker_variables.m2_gps_pos.longitude;
  
  gps_tracker_variables.gps_frac_lon_e_w = 0;
  if ( f < 0 )
  {
    f = -f;
    gps_tracker_variables.gps_frac_lon_e_w = 1;
  }
  f *= tenthousand;
  gps_tracker_variables.gps_frac_lon = (long)f;  
}

/* convert from m2_gps_pos to m2 fields */
void m2_frac_fields_to_gps_pos(void)
{
  gps_float_t f;
  
  f = gps_tracker_variables.gps_frac_lat;
  f /= tenthousand;
  if ( gps_tracker_variables.gps_frac_lat_n_s != 0 )
    f = -f;  
  gps_tracker_variables.m2_gps_pos.latitude = f;

  f = gps_tracker_variables.gps_frac_lon;
  f /= tenthousand;
  if ( gps_tracker_variables.gps_frac_lon_e_w != 0 )
    f = -f;  
  gps_tracker_variables.m2_gps_pos.longitude = f;
}

/* convert from m2_gps_pos to m2 fields (sexa notation) */
void m2_gps_pos_to_sexa_fields(void)
{
  pq_FloatToDegreeMinutes(&pq, gps_tracker_variables.m2_gps_pos.latitude);
  /*
    results are in:
    pq->pos_is_neg
    pq->pos_degree
    pq->pos_minutes
    pq->pos_minutes_frac
    pq->pos_fraction
  */  
  gps_tracker_variables.gps_frac_lat_n_s = pq.pos_is_neg;
  gps_tracker_variables.gps_grad_lat = (long)(pq.pos_degree);
  gps_tracker_variables.gps_frac_lat = (long)(pq.pos_minutes_frac*(gps_float_t)1000.0);

  pq_FloatToDegreeMinutes(&pq, gps_tracker_variables.m2_gps_pos.longitude);
  gps_tracker_variables.gps_frac_lon_e_w = pq.pos_is_neg;
  gps_tracker_variables.gps_grad_lon = (long)(pq.pos_degree);
  gps_tracker_variables.gps_frac_lon = (long)(pq.pos_minutes_frac*(gps_float_t)1000.0);  
}

void m2_sexa_fields_to_gps_pos(void)
{
  gps_float_t f;
  
  f = gps_tracker_variables.gps_frac_lat;
  f /= (gps_float_t)60000.0;
  f += (gps_float_t)gps_tracker_variables.gps_grad_lat;
  if ( gps_tracker_variables.gps_frac_lat_n_s != 0 )
    f = -f;  
  gps_tracker_variables.m2_gps_pos.latitude = f;
  
  f = gps_tracker_variables.gps_frac_lon ;
  f /= (gps_float_t)60000.0;
  f += (gps_float_t)gps_tracker_variables.gps_grad_lon;
  if ( gps_tracker_variables.gps_frac_lon_e_w != 0 )
    f = -f;  
  gps_tracker_variables.m2_gps_pos.longitude = f;
  
}


void gps_init(void)
{
  uint16_t i;
  
  
  gps_set_half_map_size_by_index(1);
  
  /* init gps parser */  
  pq_Init(&pq);

  /* init uart, connect uart to gps parser */
  UART_Init(1, gps_rx);
  
#ifdef USE_LOW_HIGH_LOW_PULSE
  /* generate high pulse on PIO1_5 */
  set_gpio_mode(PIN(1,5), 1, 0);
  
  set_gpio_level(PIN(1,5), 0);
  delay_micro_seconds(1*1000*1000);	/* wait 1 second */
  set_gpio_level(PIN(1,5), 1);
  delay_micro_seconds(200*1000);	/* wait 200ms second */
  set_gpio_level(PIN(1,5), 0);
#endif
  
  /* check for external memory */
  check_ExternalMemory();

  /* read map position list from external memory */  
  for( i = 0; i < MAP_POS_CNT; i++ )
  {
    read_MapPos(i, &(gps_tracker_variables.map_pos_list[i]));
  }

  gps_tracker_variables.cnt_10ms = 0;
  
  read_Preferences();
  gps_tracker_variables.pref.is_frac_mode &= 1;
  gps_tracker_variables.pref.utc_offset %= 25;
}
