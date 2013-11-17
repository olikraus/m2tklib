

#ifndef _INIT_H
#define _INIT_H

#include "pq.h"
#include "u8g.h"

#define NORMAL_FONT u8g_font_helvB08r
#define SMALL_FONT u8g_font_5x8r

#define MAP_POS_CNT 4

struct gps_map_pos_struct
{
  gps_pos_t pos;
  uint8_t map_symbol;		/* 0 = none */
};

struct gps_tracker_variables_struct {
  uint32_t cnt_10ms;
  uint32_t sec_cnt_raw;
  uint32_t sec_cnt;
  uint32_t adc_low_pass_z;
  volatile uint16_t adc_battery;
  volatile uint32_t uart_byte_cnt_raw;
  volatile uint32_t uart_byte_cnt_gui;
  
  gps_pos_t map_origin;
  gps_float_t map_e_w_factor; /* cos(lat * M_PI / 180) * 6372795 * M_PI / 180 */
  gps_float_t e_w_distance;
  gps_float_t n_s_distance;
  
  
  gps_float_t half_map_size;	/* in meter */
  uint8_t half_map_size_index;
  uint8_t is_visible_on_map;	/* internal variable during draw procedure */
  
  
  /* data entry */
  uint8_t gps_frac_lat_n_s;
  uint8_t gps_frac_lon_e_w;
  
  uint32_t gps_frac_lat;		/* M2 Data Entry */
  uint32_t gps_frac_lon;		/* M2 Data Entry */

  uint32_t gps_grad_lat;		/* M2 Data Entry */
  uint32_t gps_grad_lon;		/* M2 Data Entry */

  gps_pos_t m2_gps_pos;
  
  /* map special elements */
  struct gps_map_pos_struct map_pos_list[MAP_POS_CNT];
  uint32_t map_pos_idx;
  uint8_t is_frac_mode;
  
  /* str buffer for gps pos */
  char str_lat[16];
  char str_lon[16];

};

extern struct gps_tracker_variables_struct gps_tracker_variables;


extern pq_t pq;	/* GPS Parser Object */



void gps_set_half_map_size_by_index(uint8_t idx) __attribute__((noinline));
void gps_inc_half_map_size(void) __attribute__((noinline));
void gps_dec_half_map_size(void) __attribute__((noinline));
const char *gps_get_half_map_str(void) __attribute__((noinline));

void m2_gps_pos_to_frac_fields(void) __attribute__((noinline));
void m2_frac_fields_to_gps_pos(void) __attribute__((noinline));

void m2_gps_pos_to_sexa_fields(void) __attribute__((noinline));
void m2_sexa_fields_to_gps_pos(void) __attribute__((noinline));

extern void gps_init(void);

#endif
