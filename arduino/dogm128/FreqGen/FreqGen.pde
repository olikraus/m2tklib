/*

  U32.pde
  
  DOGM128-Lib Example

  m2tklib = Mini Interative Interface Toolkit Library
  
  Copyright (C) 2011  olikraus@gmail.com

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

  SCL (SPI Clock)   Pin 13
  SI (MOSI)         Pin 11
  CS (Chip Select)  Pin 10
  MISO (Pin 12) is not used, but can not be reused as generic I/O

  Pins 5, 6	Timer 0
  Pins 9, 10	Timer 1
  Pins 11, 3	Timer 3
  
  Unused PWM Pins
    DOGS102			Pin 6
    DOGM128 Shield 	Pins 5, 6
    DOGXL160 Shield	Pins 5, 6

*/

/*

  Timer 2 (8 Bit)
   TCCR2B Bits 0-2 Clock Select CS22, CS21, CS20 --> prescalar
	      MGM0 MGM1 MGM3 == 7 fast PWM with OCR2A as top value
	      
  Fast PWM frequency:
    
      f = f_io / ( N * ( 1 + OCRnA) )
      N = { 1, 8, 64, 256, 1024 }
      OCRnA = 0 .. 255
      
    N * ( 1 + OCRnA) = f_io / f
    
      

*/

#include "Dogm.h"
#include "M2tk.h"
#include "m2ghdogm.h"

#ifdef DOGS102_HW
// DOGS102 shield 
uint8_t uiKeyUpPin = 5;
uint8_t uiKeyDownPin = 3;
uint8_t uiKeySelectPin = 4;
#else
// DOGM132, DOGM128 and DOGXL160 shield
uint8_t uiKeyUpPin = 7;
uint8_t uiKeyDownPin = 3;
uint8_t uiKeySelectPin = 2;
#endif

int a0Pin = 9;
Dogm dogm(a0Pin);


// Timer 0 & 1,       	N = { 1, 8, 64, 256, 1024 }
uint8_t fc_prescalar_div01[] = { 3, 3, 2, 2, 0 };
// Timer 2, 		N = { 1, 8, 32, 64, 128, 256, 1024 }
uint8_t fc_prescalar_div2[] = { 3, 2, 1, 1, 1, 2, 0 };

// input values
uint8_t fc_timer = 2;		// timer: 0, 1, 2
uint32_t fc_io_freq = 16000000UL;
uint32_t fc_user_freq;		// requested user frequency
uint8_t fc_duty = 1;			// PWM value 1..99 %

// output values
uint8_t fc_prescalar_idx;		// calculated value for CS22:0 bits
uint8_t fc_top_value;		// calculated value for OCRnA
uint32_t fc_calc_freq;		// calculated frequency

// Fast PWM Mode:      f = fc_io_freq / ( N * ( 1 + OCRnA) )
//  ==>   	 N * ( 1 + OCRnA) = fc_io_freq / f = q
// Algorithm:
//    calculate q
//    try values for N, until q/N is below 256
//    if found, then OCRnA = q/N - 1
uint8_t fc_calc_prescalar_idx_and_top_value(void)
{
  uint8_t idx = 0;
  uint8_t div;
  uint32_t q;
  q = fc_io_freq;
  q /= fc_user_freq;
  for(;;)
  {
    if ( q <= 256UL )
    {
      idx++;
      fc_prescalar_idx = idx;
      q--;
      fc_top_value = q;
      return  1;
    }
    if ( fc_timer == 2 )
      div = fc_prescalar_div2[idx];
    else	
      div = fc_prescalar_div01[idx];
    if ( div == 0 )
      break;
    q >>= div;
    idx++;
  }
  fc_prescalar_idx = 0;
  fc_top_value = 0;
  return 0; 					// no prescalar found 
}

// calculate frequency from prescalar index and top value
// f = fc_io_freq / ( N * ( 1 + OCR2A) )
uint32_t fc_calc_frequency(void)
{
  uint32_t t;
  uint8_t idx = fc_prescalar_idx;
  if ( idx == 0 )
  {
    fc_calc_freq = 0;
    return 0;
  }
  t = fc_top_value;
  t++;
  
  idx--;
  while(  idx > 0 )
  {
    idx--;
    if ( fc_timer == 2 )
      t <<= fc_prescalar_div2[idx];
    else
      t <<= fc_prescalar_div01[idx];
      
  }
  fc_calc_freq =  fc_io_freq;
  fc_calc_freq /= t;
  
  return fc_calc_freq;
}

// input: fc_timer, fc_user_freq
void fc_calc(void)
{
  fc_calc_prescalar_idx_and_top_value();
  fc_calc_frequency();
}



void fn_ok(m2_el_fnarg_p fnarg) {
  fc_calc();
}

M2_LABEL(el_label1, NULL, "Freq: ");
M2_U32NUM(el_freq, "a1c6", &fc_user_freq);
M2_LABEL(el_label2, NULL, "Duty: ");
M2_U8NUM(el_duty, "a1c2", 1, 99, &fc_duty);
M2_LABEL(el_label3, NULL, "Real: ");
M2_U32NUM(el_real_freq, "r1c6", &fc_calc_freq);

M2_BUTTON(el_ok, "", " Apply ", fn_ok);
M2_LIST(list) = { &el_label1, &el_freq, &el_label2, &el_duty, &el_label3, &el_real_freq, &el_ok };
M2_GRIDLIST(top_el_gridlist, "c2", list);
M2tk m2(&top_el_gridlist, m2_es_arduino, m2_eh_2bs, m2_gh_dogm_fbs);

void setup() {
  m2.setFont(0, font_6x10);
  m2.setPin(M2_KEY_SELECT, uiKeySelectPin);
  m2.setPin(M2_KEY_NEXT, uiKeyDownPin);
}

void loop() {
  m2.checkKey();
  if ( m2.handleKey() ) {
    dogm.start();
    do{
      m2.checkKey();
      m2.draw();
    } while( dogm.next() );
  }
}



