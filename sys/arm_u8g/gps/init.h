

#ifndef _INIT_H
#define _INIT_H

#include "pq.h"
#include "u8g.h"

#define NORMAL_FONT u8g_font_helvB08r
#define SMALL_FONT u8g_font_5x8r

struct gps_tracker_variables_struct {
  uint32_t adc_low_pass_z;
  volatile uint16_t adc_battery;
  volatile uint32_t uart_byte_cnt_raw;
  volatile uint32_t uart_byte_cnt_gui;
  
  gps_pos_t map_origin;
  gps_float_t map_e_w_factor; /* cos(lat * M_PI / 180) * 6372795 * M_PI / 180 */
  gps_float_t e_w_distance;
  gps_float_t n_s_distance;
  
  gps_float_t half_map_size;	/* in meter */
  uint8_t is_visible_on_map;
  
};

extern struct gps_tracker_variables_struct gps_tracker_variables;


extern pq_t pq;	/* GPS Parser Object */

extern void gps_init(void);

#endif
