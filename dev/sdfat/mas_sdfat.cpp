/*

  mas_sdfat.c
  
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
#include <SdFat.h>
#include "mas.h"

/* objects for the SdFat library */
SdFat *mas_sdfat_sd;
SdFile mas_sdfat_file;
uint8_t mas_sdfat_file_is_open = 0;


/* Helper function: Called only from "mas_sdfat_move_to_pwd" */
static uint8_t mas_sdfat_change_to_dir(const char *directory_name)
{
  mas_sdfat_sd->chdir(directory_name);
  return 1;
}

/* Helper function: Called only from "mas_sdfat_move_to_pwd" */
static uint8_t mas_sdfat_goto_root_dir(void)
{
  mas_sdfat_sd->chdir();
  return 1;
}

/*
  Description:
    Set the current working directory to the specified path.
    The path has a fixed format. It is composed of zero, one or more
    directory names, where each directory name is prefixed with MAS_DIR_SEP
    Examples: 
      ""                Empty directory path (should be treated as root directory)
      "/dir1"         One directory
      "/dir1/dir2"  Two directories
*/
static uint8_t mas_sdfat_move_to_pwd(const char *path)
{
  uint8_t i, j;
  
  if ( mas_sdfat_goto_root_dir() == 0 )
    return 0;
  i = 0;
  for(;;)
  {
    if ( path[i] == '\0' )
      break;
    if ( path[i] == MAS_DIR_SEP )
    {
      i++;
      j = 0;
      for(;;)
      {
        if ( path[i] == MAS_DIR_SEP || path[i] == '\0' )
          break;
        mas_entry_name[j] = path[i];
        j++;
        i++;
      }
      mas_entry_name[j] = '\0';
      if ( mas_sdfat_change_to_dir(mas_entry_name) == 0 )
        return 0;
      if ( path[i] == MAS_DIR_SEP )
        i++;
    }
  }
  return 1;
}

/*
  Description:
    Requrest for a specific file at a specified position within the directory
    The path argument is identical to the path which was provided to last
    call to "mas_sdfat_move_to_pwd"
*/
uint8_t mas_sdfat_get_nth_file(const char *path, uint16_t n, char *buf, uint8_t *is_dir)
{
  uint16_t c = 0;

  if ( mas_sdfat_move_to_pwd(path) == 0 )
    return 0;
  
  if ( mas_sdfat_file_is_open != 0 )
  {
    mas_sdfat_file.close();
    mas_sdfat_file_is_open = 0;
  }
  
  mas_sdfat_sd->vwd()->rewind();
  while (mas_sdfat_file.openNext(mas_sdfat_sd->vwd(), O_READ)) 
  {
    if ( n == c )
    {
      mas_sdfat_file.getFilename(buf);
      buf[12] = '\0';
      *is_dir = mas_sdfat_file.isDir();
      mas_sdfat_file.close();
      return 1;
    }
    c++;
    mas_sdfat_file.close();
  }
  return 0;
}

uint16_t mas_sdfat_get_directory_file_cnt(const char *path)
{
  uint8_t cnt;
  cnt = 0;
  
  if ( mas_sdfat_move_to_pwd(path) == 0 )
    return 0;

  if ( mas_sdfat_file_is_open != 0 )
  {
    mas_sdfat_file.close();
    mas_sdfat_file_is_open = 0;
  }
  
  mas_sdfat_sd->vwd()->rewind();
  while (mas_sdfat_file.openNext(mas_sdfat_sd->vwd(), O_READ)) 
  {
    cnt++;
    mas_sdfat_file.close();
    if ( cnt == 0x0ffff )
      break;
  }
  return cnt;
}


uint8_t mas_device_sdfat(uint8_t msg, void *arg)
{
  if ( msg == MAS_MSG_GET_DIR_ENTRY_AT_POS )
  {
    mas_arg_get_dir_entry_at_pos_t *a = ((mas_arg_get_dir_entry_at_pos_t *)arg);
    return mas_sdfat_get_nth_file(a->path, a->n, a->buf, &(a->is_dir));
  }
  else if ( msg == MAS_MSG_GET_DIR_ENTRY_CNT )
  {
    mas_arg_get_dir_entry_cnt_t *a = ((mas_arg_get_dir_entry_cnt_t *)arg);
    a->cnt = mas_sdfat_get_directory_file_cnt(a->path);
    return 1;
  }
  else if ( msg == MAS_MSG_INIT )
  {
    mas_sdfat_sd = (SdFat *)arg;
    return 1;
  }
  return 0;
}
