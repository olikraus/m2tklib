

#ifndef _INIT_H
#define _INIT_H

#include "pq.h"

struct gps_tracker_variables_struct {
  uint32_t adc_low_pass_z;
  volatile uint16_t adc_battery;
  volatile uint32_t uart_byte_cnt_raw;
  volatile uint32_t uart_byte_cnt_gui;
};

extern struct gps_tracker_variables_struct gps_tracker_variables;

extern pq_t pq;	/* GPS Parser Object */

extern void gps_init(void);

#endif
