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
}

/*========================================================================*/
/* setup gps device */

pq_t pq;

void gps_rx(uint8_t value)
{
  gps_tracker_variables.uart_byte_cnt_raw++;
  pq_AddChar(&pq, (uint8_t)value);
}

void gps_init(void)
{
  
  gps_tracker_variables.half_map_size = 10;
  
  /* init gps parser */  
  pq_Init(&pq);

  /* init uart, connect uart to gps parser */
  UART_Init(1, gps_rx);
  
}
