/*

  FreqGen.pde
  
  GLCD Example

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
  along with this program.  If not, see <http://www.gnu.org/licenses/>

  SCL (SPI Clock)   Pin 13
  SI (MOSI)         Pin 11
  CS (Chip Select)  Pin 10
  MISO (Pin 12) is not used, but can not be reused as generic I/O

  Pins 6 (OC0A), 5 (OC0B) 	Timer 0
  Pins 9 (OC1A), 10 (OC1B)	Timer 1
  Pins 11 (OC2A), 3 (OC2B)	Timer 2
  
  OCRnA is used for frequency generation
  OCRnB is used for duty value
  ==> only pins 3, 5 and 10 can be used

  
  Unused PWM Pins
    DOGS102			Pin 6		--> No pin available
    DOGM128 Shield 	Pins 5, 6		--> Pin 5
    DOGXL160 Shield	Pins 5, 6		--> Pin 5
    KS0108			Pin 3		--> Pin 3

*/

/*

  Timer 2 (8 Bit)
   TCCR2B Bits 0-2 Clock Select CS22, CS21, CS20 --> prescalar
	      MGM0 MGM1 MGM3 == 7 fast PWM with OCR2A as top value
	      
  Fast PWM frequency:
    
      f = f_io / ( N * ( 1 + OCRnA) )
      N = { 1, 8, 64, 256, 1024 }
      OCRnA = 0 .. 255
      
      
  SelectPin = 0;
  DownPin = 1;
  UpPin = 2;

*/

#include <avr/interrupt.h>
#include <avr/io.h>


#include <glcd.h>		// inform Arduino IDE that we will use GLCD library
#include "M2tk.h"
#include "m2ghglcd.h"

uint8_t uiKeySelectPin = 0;
uint8_t uiKeyDownPin = 1;
uint8_t uiKeyUpPin = 2;
//uint8_t uiKeyExitPin = 0;

// Timer 0 & 1,       	N = { 1, 8, 64, 256, 1024 }
uint8_t fc_prescalar_div01[] = { 3, 3, 2, 2, 0 };
// Timer 2, 		N = { 1, 8, 32, 64, 128, 256, 1024 }
uint8_t fc_prescalar_div2[] = { 3, 2, 1, 1, 1, 2, 0 };

// input values
uint8_t fc_pin = 3; 			// 3, 5, 11 (only OCRnB)
uint32_t fc_io_freq = F_CPU;
uint32_t fc_user_freq;		// requested user frequency
uint8_t fc_duty = 1;			// PWM value 1..99 %
#define FC_DUTY_MAX 100

// output values
uint8_t fc_timer = 0;		// timer: 0, 1, 2
uint8_t fc_ab = 0;			// 0 = A, 1 = B
uint8_t fc_prescalar_idx;		// calculated value for CS22:0 bits
uint8_t fc_top_value;		// calculated value for OCRnA
uint32_t fc_calc_freq;		// calculated real frequency
uint8_t fc_calc_duty;		// calculated real duty


// based on the fc_pin value, calculate the timer
void fc_calc_timer_ab(void)
{
  switch(fc_pin)
  {
    case 5: fc_timer = 0; fc_ab = 1; break;
    //case 6: fc_timer = 0; fc_ab = 0; break;
    //case 9: fc_timer = 1; fc_ab = 0; break;
    case 10: fc_timer = 1; fc_ab = 1; break;
    // case 11: fc_timer = 2; fc_ab = 0; break;
    case 3: fc_timer = 2; fc_ab = 1; break;
    default: fc_timer = 0; fc_ab = 1; break;
  }
}

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
void fc_calc_frequency_and_duty(void)
{
  uint32_t t;
  uint8_t idx = fc_prescalar_idx;
  if ( idx == 0 )
  {
    fc_calc_freq = 0;
    return ;
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

  fc_calc_duty = ((uint16_t)fc_top_value * (uint16_t)fc_duty)/FC_DUTY_MAX;
  
}

// OCRnA contains the top value
// OCRnB contains the duty value
// --> Only port pin B can be used 
void fc_set_hw(void)
{
  uint8_t d;

  d = fc_calc_duty;
    
  pinMode(fc_pin, OUTPUT);
  //analogWrite(fc_pin, 40);
  //digitalWrite(fc_pin, HIGH);

  if ( fc_timer == 0 )
  {
    // Set fast PWM mode, TOP is OCRA
    TCCR0A |= _BV(WGM01) | _BV(WGM00);
    TCCR0B |= _BV(WGM02);

    // none inverting fast PWM
    if ( fc_ab == 0 )
    {
      TCCR0A |= _BV(COM0A1);
      TCCR0A &= ~_BV(COM0A0);
    }
    else
    {
      TCCR0A |= _BV(COM0B1);
      TCCR0A &= ~_BV(COM0B0);
    }
    
    // prescalar
    TCCR0B &= ~7;
    TCCR0B |= fc_prescalar_idx;
    
    // set top value
    OCR0A = fc_top_value;
    // set duty
    OCR0B = d;
    
    
  }
  else if ( fc_timer == 1 )
  {
    // Set fast PWM mode, TOP is OCRA
    TCCR1A |= _BV(WGM11) | _BV(WGM10);
    TCCR1B |= _BV(WGM12) | _BV(WGM13) ;
    
    // none inverting fast PWM
    if ( fc_ab == 0 )
    {
      TCCR1A |= _BV(COM1A1);
      TCCR1A &= ~_BV(COM1A0);
    }
    else
    {
      TCCR1A |= _BV(COM1B1);
      TCCR1A &= ~_BV(COM1B0);
    }
    
    // prescalar
    TCCR1B &= ~7;
    TCCR1B |= fc_prescalar_idx;
    
    // set top value
    sei();
    OCR1AH = 0;
    OCR1AL = fc_top_value;
    // set duty
    OCR1BH = 0;
    OCR1BL = d;
    cli();
    
  }
  else if ( fc_timer == 2 )
  {
    
    // ensure internal clock
    ASSR &= ~(_BV(EXCLK) | _BV(AS2));
    
    // Set fast PWM mode, TOP is OCRA
    TCCR2A |= _BV(WGM21) | _BV(WGM20);
    TCCR2B |= _BV(WGM22);

    // none inverting fast PWM
    if ( fc_ab == 0 )
    {
      TCCR2A |= _BV(COM2A1);
      TCCR2A &= ~_BV(COM2A0);
    }
    else
    {
      TCCR2A |= _BV(COM2B1);
      TCCR2A &= ~_BV(COM2B0);
    }
    
    // prescalar
    TCCR2B &= ~7;
    TCCR2B |= fc_prescalar_idx;
    
    // set top value
    OCR2A = fc_top_value;
    // set duty
    OCR2B = d;
  }
}

// input: fc_timer, fc_user_freq
void fc_calc(void)
{
  fc_calc_timer_ab();
  fc_calc_prescalar_idx_and_top_value();
  fc_calc_frequency_and_duty();
  fc_set_hw();
}



void fn_ok(m2_el_fnarg_p fnarg) {
  fc_calc();
}

M2_LABEL(el_label1, NULL, "Freq.: ");
M2_U32NUM(el_freq, "a1c6", &fc_user_freq);
M2_LABEL(el_label2, NULL, "Duty: ");
M2_U8NUM(el_duty, "a1c2", 1, 99, &fc_duty);
M2_LABEL(el_label3, "", "Real Freq.: ");
M2_U32NUM(el_real_freq, "r1c6", &fc_calc_freq);
M2_LABEL(el_label4, "", "Real Duty: ");
M2_U8NUM(el_real_duty, "r1c3",0, 255, &fc_calc_duty);

M2_BUTTON(el_ok, "", " Apply ", fn_ok);
M2_LIST(list) = { &el_label1, &el_freq, &el_label2, &el_duty, &el_label3, &el_real_freq, &el_label4, &el_real_duty, &el_ok };
M2_GRIDLIST(top_el_gridlist, "c2", list);
M2tk m2(&top_el_gridlist, m2_es_arduino, m2_eh_2bs, m2_gh_glcd_ffs);

void setup() {
  m2.setPin(M2_KEY_SELECT, uiKeySelectPin);
  m2.setPin(M2_KEY_NEXT, uiKeyDownPin);
  m2.setPin(M2_KEY_PREV, uiKeyUpPin);
  //m2.setPin(M2_KEY_EXIT, uiKeyExitPin);  
}

void loop() {
  m2.checkKey();
  if ( m2.handleKey() ) {
      m2.draw();
  }
}



