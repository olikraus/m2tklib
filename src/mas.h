/*

  mas.h
  
  mass storage interface subsystem

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

*/



#ifndef _MAS_H
#define _MAS_H

#include <stdint.h>

#define MAS_PATH_MAX 64


/*
  This must be '/' for the  Petit FAT File System Module (http://elm-chan.org/fsw/ff/00index_p.html)
*/
#define MAS_DIR_SEP '/'


typedef uint8_t mas_device_fn(uint8_t msg, void *arg);

#define MAS_MSG_INIT 0
#define MAS_MSG_GET_DIR_ENTRY_AT_POS 1
#define MAS_MSG_GET_DIR_ENTRY_CNT 2


struct _mas_arg_init
{
  /* provided input */
  uint8_t cs_pin;
};
typedef struct _mas_arg_init mas_arg_init_t;

struct _mas_arg_get_dir_entry_at_pos
{
  /* provided input */
  const char *path;
  uint16_t n;
  /* expected output */
  char *buf; 
  uint8_t is_dir;
};
typedef struct _mas_arg_get_dir_entry_at_pos mas_arg_get_dir_entry_at_pos_t;

struct _mas_arg_get_dir_entry_cnt
{
  /* provided input */
  const char *path;
  /* expected output */
  uint16_t cnt;
};
typedef struct _mas_arg_get_dir_entry_cnt mas_arg_get_dir_entry_cnt_t;

/*======================================================*/

extern char mas_entry_name[];   /* 12 chars for the name, 1 for the terminating '\0' */
extern uint8_t mas_entry_is_dir;
extern char mas_pwd[MAS_PATH_MAX];


/*======================================================*/


/* devices, first argument to mas_init() */
uint8_t mas_device_sim(uint8_t msg, void *arg);
uint8_t mas_device_pff(uint8_t msg, void *arg);


/* user api */
uint8_t mas_change_dir_down(const char *subdir);
uint8_t mas_change_dir_up(void);
void mas_change_dir_to_root(void);
uint8_t mas_get_nth_file(uint16_t n);
uint16_t mas_get_dir_entry_cnt(void);
uint8_t mas_init(mas_device_fn *device, uint8_t cs_pin);

#endif

