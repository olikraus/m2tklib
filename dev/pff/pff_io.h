/*

  pff_io.h
  
  atmega low level procedures for pff

  part of m2tklib (Mini Interative Interface Toolkit Library)
  
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

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void pff_delay_us(uint16_t val);
void pff_chip_select_high(void);
void pff_chip_select_low(void);
void pff_spi_init(void);
uint8_t pff_spi_out(uint8_t data);

#ifdef __cplusplus
}
#endif
