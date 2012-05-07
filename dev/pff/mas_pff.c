/*

  mas_pff.c
  
  mass storage device for petite fat file system (http://elm-chan.org/fsw/ff/00index_p.html)

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

#include <string.h>
#include "mas.h"
#include "pff.h"

FATFS *mas_pff_fs;
DIR mas_pff_dir;
FILINFO mas_pff_fi;

uint16_t mas_last_readdir_n = 0x0ffff;


	  

static uint8_t mas_pff_init(FATFS *pff_fs)
{
  mas_last_readdir_n = 0x0ffff;
  mas_pff_fs = pff_fs;
  return 1;
}

/*
  Description:
    Requrest for a specific file at a specified position within the directory
*/

static uint8_t mas_pff_get_nth_file(const char *path, uint16_t n, char *buf, uint8_t *is_dir)
{
  FRESULT fr;
  uint16_t c = 0;
	
  if ( mas_last_readdir_n >=  n )
  {
    fr = pf_opendir(&mas_pff_dir, path);
    if ( fr != FR_OK )
      return 0;
  }
  else
  {
     c = mas_last_readdir_n;
     c++;
  }
  while( c < 0x0ffff)
  {
    fr = pf_readdir(&mas_pff_dir, &mas_pff_fi);
    if ( fr != FR_OK )
      return 0;
    if ( mas_pff_fi.fname[0] == '\0' )
      return 0;
    if ( c == n )
    {
      strcpy(buf, mas_pff_fi.fname);
      if ( (mas_pff_fi.fattrib & AM_DIR) != 0 )
      {
        *is_dir = 1;
      }
      else
      {
        *is_dir = 0;        
      }
      mas_last_readdir_n = n;
      return 1;
    }
    c++;
  }
  return 0;
}

static uint16_t mas_pff_get_cnt(const char *path)
{
  FRESULT fr;
  uint16_t c = 0;
	
  fr = pf_opendir(&mas_pff_dir, path);
  if ( fr != FR_OK )
    return 0;
  
  while( c < 0x0ffff)
  {
    fr = pf_readdir(&mas_pff_dir, &mas_pff_fi);
    if ( fr != FR_OK )
      break;
    if ( mas_pff_fi.fname[0] == '\0' )
      break;
    c++;
  }
  
  mas_last_readdir_n = 0x0ffff;
  return c;
}


uint8_t mas_device_pff(uint8_t msg, void *arg)
{
  if ( msg == MAS_MSG_GET_DIR_ENTRY_AT_POS )
  {
    mas_arg_get_dir_entry_at_pos_t *a = ((mas_arg_get_dir_entry_at_pos_t *)arg);
    return mas_pff_get_nth_file(a->path, a->n, a->buf, &(a->is_dir));
  }
  else if ( msg == MAS_MSG_GET_DIR_ENTRY_CNT )
  {
    mas_arg_get_dir_entry_cnt_t *a = ((mas_arg_get_dir_entry_cnt_t *)arg);    
    a->cnt = mas_pff_get_cnt(a->path);
    return 1;
  }
  else if ( msg == MAS_MSG_INIT )
  {
    return mas_pff_init((FATFS *)arg);
  }
  return 0;
}

