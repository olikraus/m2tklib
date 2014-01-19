/*

  config.h
  
  Configuration file for the GPS tracker device.
  
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

#ifndef _CONFIG_H
#define _CONFIG_H

/* 
  Define the following variable to enable monitoring of battery voltage. 
  ADC2 at R_PIO1_1 will be used for monitoring.
  This will also enable the battery symbol in the upper left of the display and
  the battery submenu.
*/
//#define USE_BATTERY_ADC


/*
  Select transmit speed of the connected GPS module. 
  If the following macro is defined, use 4800 Baud.
  If the following macro is not defined, use 9600 Baud
*/
#define USE_GPS_4800BAUD

/*
  Define this to generate a LOW_HIGH_LOW Pulse on PIO1_5
*/
#define USE_LOW_HIGH_LOW_PULSE

#endif
