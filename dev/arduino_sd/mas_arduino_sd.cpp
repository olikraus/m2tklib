/*

  mas_arduino_sd.c
  
  sdfat device for the mass storage interface subsystem

  part of m2tklib (Mini Interative Interface Toolkit Library)
  
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


  mas_sdfat requires Arduino Environment and SdFat Library (http://code.google.com/p/sdfatlib/)
  

*/


#if ARDUINO < 100 
#include "wiring.h"
//#include <WProgram.h> 
#else 
#include <Arduino.h> 
#endif

#include <stdint.h>
#include <SD.h>
#include "mas.h"



/*
  Description:
    Request for a specific file at a specified position within the directory
*/
uint8_t mas_sd_get_nth_file(const char *path, uint16_t n, char *buf, uint8_t *is_dir)
{
  uint16_t c = 0;
  File dir;
  File f;
	
  dir = SD.open(path);
  for(;;)
  {
    f =  dir.openNextFile();
    if ( !f )
      break;
    if ( n == c )
    {
      strcpy(buf, f.name());
      *is_dir = f.isDirectory();
      f.close();
      return 1;
    }
    c++;
    f.close();
  }
  return 0;
}

uint16_t mas_sd_get_directory_file_cnt(const char *path)
{
  uint8_t cnt;
  File dir;
  File f;
  cnt = 0;
  
  dir = SD.open(path);
  for(;;)
  {
    f =  dir.openNextFile();
    if ( !f )
	break;
    cnt++;
    f.close();
    if ( cnt == 0x0ffff )
      break;
  }
  return cnt;
}


uint8_t mas_device_sd(uint8_t msg, void *arg)
{
  if ( msg == MAS_MSG_GET_DIR_ENTRY_AT_POS )
  {
    mas_arg_get_dir_entry_at_pos_t *a = ((mas_arg_get_dir_entry_at_pos_t *)arg);
    return mas_sd_get_nth_file(a->path, a->n, a->buf, &(a->is_dir));
  }
  else if ( msg == MAS_MSG_GET_DIR_ENTRY_CNT )
  {
    mas_arg_get_dir_entry_cnt_t *a = ((mas_arg_get_dir_entry_cnt_t *)arg);
    a->cnt = mas_sd_get_directory_file_cnt(a->path);
    return 1;
  }
  else if ( msg == MAS_MSG_INIT )
  {
    return 1;
  }
  return 0;
}
