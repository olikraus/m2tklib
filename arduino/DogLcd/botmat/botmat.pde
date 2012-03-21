/*

  botmat.pde
  
  botmat example program

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

#include <DogLcd.h>
#include <DS1307new.h>
#include <SdFat.h>
#include "M2tk.h"
#include "m2utl.h"
#include "m2ghdoglcd.h"
#include <string.h>

/*=========================================================================*/
/* dog lcd object definition for botmat */

DogLcd lcd(5, 7, 29, 1, -1, 3);

/* object for the SdFat library */
SdFat sd;
SdFile file;

/*=========================================================================*/
/* pin numbers of the keypad */

uint8_t uiKeySelectPin = 18;
uint8_t uiKeyUpPin = 22;
uint8_t uiKeyDownPin = 19;
uint8_t uiKeyLeftPin = 20;
uint8_t uiKeyRightPin = 21;

/*=========================================================================*/
/* forward declarations */

extern M2tk m2;
M2_EXTERN_HLIST(el_top);

/*=========================================================================*/
/* 
  info screens 
  0: millis()
  1: voltage (adc0)
  2: time
*/


uint8_t info_screen_state = 0;

void info_screen_display(void)
{
  if ( info_screen_state == 0 )
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(millis());
    delay(20);
  }
  else if ( info_screen_state == 1 )
  {
    uint16_t adc = analogRead(0);
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("raw:  ");
    lcd.print(adc);
    lcd.setCursor(0,1);
    lcd.print("volt: ");
    lcd.print((adc*3.3)/512.0);
    delay(50);
  }
  else if ( info_screen_state == 2 )
  {
    RTC.getTime();
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(m2_utl_u8d(RTC.hour,2));
    lcd.print(":");
    lcd.print(m2_utl_u8d(RTC.minute,2));
    lcd.print(":");
    lcd.print(m2_utl_u8d(RTC.second,2));
    lcd.setCursor(0,1);
    lcd.print(m2_utl_u8d(RTC.day,2));
    lcd.print(".");
    lcd.print(m2_utl_u8d(RTC.month,2));
    lcd.print(".");
    lcd.print(m2_utl_u8d(RTC.year-2000,2));
    
    delay(50);
  }

}

/*=========================================================================*/
/* file selection dialog */
/* limitation: not more than 250 elements per directory allowed */

#define FS_EXTRA_MENUES 1

/* buffer for one file name */
char fs_name[2+12+1];   /* 2 chars for the prefix, 12 chars for the name, 1 for the terminating '\0' */
uint8_t fs_is_dir = 0;

/* cache */
#define FS_CACHE_SIZE 2
char fs_c_name[FS_CACHE_SIZE][2+12+1];
uint8_t fs_c_is_dir[FS_CACHE_SIZE];
uint8_t fs_c_idx[FS_CACHE_SIZE] = { 255, 255 };
uint8_t fs_rr = 0;

/* number of files in the current folder, 255 forces recalculation */
uint8_t fs_file_cnt = 255;

/* helper variables for the strlist element */
uint8_t fs_m2tk_first = 0;
uint8_t fs_m2tk_cnt = 0;

void fs_update_file_cnt(void)
{
  if ( fs_file_cnt == 255 )
  {
    fs_file_cnt = 0;
    sd.vwd()->rewind();
    while (file.openNext(sd.vwd(), O_READ)) 
    {
      fs_file_cnt++;
      if ( fs_file_cnt == 250 )
        break;
      file.close();
    }
    /*
    if  ( fs_file_cnt > 10 )
      fs_file_cnt = 10;
    */
    /* update m2 variable */
    fs_m2tk_cnt = fs_file_cnt;
  }  
}

uint8_t fs_get_cache_entry(uint8_t n)
{
  uint8_t i;
  for( i = 0 ; i < FS_CACHE_SIZE; i++ )
    if ( fs_c_idx[i] == n )
    {
      strcpy(fs_name, fs_c_name[i]);
      fs_is_dir = fs_c_is_dir[i];
      return i;
    }
  return 255;
}

void fs_put_into_cache(uint8_t n)
{
  strcpy(fs_c_name[fs_rr], fs_name);
  fs_c_is_dir[fs_rr] = fs_is_dir;
  fs_rr++;
  if ( fs_rr >= FS_CACHE_SIZE )
    fs_rr = 0;
}



/* get the n'th file an store it into the intermediate buffers fs_is_dir and fs_name */
void fs_get_nth_file(uint8_t n)
{
  uint8_t c = 0;
  if ( fs_get_cache_entry(n) != 255 )
    return;
  
  fs_name[0] = '-';
  fs_name[1] = '-';
  fs_name[2] = '\0';
  fs_is_dir = 0;
  
  sd.vwd()->rewind();
  while (file.openNext(sd.vwd(), O_READ)) 
  {
    if ( n == c )
    {
      fs_name[0] = ' ';
      fs_name[1] = ' ';
      file.getFilename(fs_name+2);
      fs_name[12+2] = '\0';
      fs_is_dir = file.isDir();
      if ( fs_is_dir )
        fs_name[0] = '+';
      file.close();
      fs_put_into_cache(n);
      break;
    }
    c++;
    file.close();
  }
}

const char *fs_strlist_getstr(uint8_t idx, uint8_t msg) 
{
  
  /* update files, if required */
  fs_update_file_cnt();

  /* process message */
  if (msg == M2_STRLIST_MSG_GET_STR) 
  {
    if ( idx == 0 )
      return "-- Back --";
    fs_get_nth_file(idx-FS_EXTRA_MENUES);
    return fs_name;
  } 
  else if ( msg == M2_STRLIST_MSG_SELECT ) 
  {
    if ( idx == 0 )
    {
      m2.setRoot(&el_top);      
    }
    else
    {
      fs_get_nth_file(idx);
      if ( fs_is_dir )
      {
      }
    }
  } 
  return fs_name;
}

M2_STRLIST(el_fs_strlist, "l2w13", &fs_m2tk_first, &fs_m2tk_cnt, fs_strlist_getstr);
//M2_SPACE(el_fs_space, "w1h1");
M2_VSB(el_fs_strlist_vsb, "l2w1r1", &fs_m2tk_first, &fs_m2tk_cnt);
M2_LIST(list_fs_strlist) = { &el_fs_strlist, &el_fs_strlist_vsb };
M2_HLIST(el_top_fs, NULL, list_fs_strlist);


/*=========================================================================*/
/* edit time dialog */

uint8_t td_hour;
uint8_t td_min;
uint8_t td_sec;

void td_get_from_RTC(void)
{
  RTC.getTime();
  td_hour = RTC.hour;
  td_min = RTC.minute;
  td_sec = RTC.second;
}

void td_put_to_RTC(void)
{
  RTC.getTime();
  RTC.fillByHMS(td_hour, td_min, td_sec);
  RTC.setTime();
  RTC.startClock();  
}

void td_ok_fn(m2_el_fnarg_p fnarg) 
{
  td_put_to_RTC();
  m2.setRoot(&el_top);
}

M2_U8NUM(el_td_hour, "c2", 0,23,&td_hour);
M2_LABEL(el_td_sep1, NULL, ":");
M2_U8NUM(el_td_min, "c2", 0,59,&td_min);
M2_LABEL(el_td_sep2, NULL, ":");
M2_U8NUM(el_td_sec, "c2", 0,59,&td_sec);

M2_LIST(list_time) = { &el_td_hour, &el_td_sep1, &el_td_min, &el_td_sep2, &el_td_sec };
M2_HLIST(el_time, NULL, list_time);

M2_ROOT(el_td_cancel, NULL, "cancel", &el_top);
M2_BUTTON(el_td_ok, NULL, "ok", td_ok_fn);
M2_LIST(list_td_buttons) = {&el_td_cancel, &el_td_ok };
M2_HLIST(el_td_buttons, NULL, list_td_buttons);

M2_LIST(list_td) = {&el_time, &el_td_buttons };
M2_VLIST(el_top_td, NULL, list_td);

/*=========================================================================*/
/* edit date dialog */

uint8_t dt_day;
uint8_t dt_month;
uint8_t dt_year;

void dt_get_from_RTC(void)
{
  RTC.getTime();
  dt_day = RTC.day;
  dt_month = RTC.month;
  dt_year = (RTC.year-2000);
}

void dt_put_to_RTC(void)
{
  RTC.getTime();
  RTC.fillByYMD(dt_year+2000, dt_month, dt_day);
  RTC.setTime();
  RTC.startClock();  
}

void dt_ok_fn(m2_el_fnarg_p fnarg) 
{
  dt_put_to_RTC();
  m2.setRoot(&el_top);
}

M2_U8NUM(el_dt_day, "c2", 1,31,&dt_day);
M2_LABEL(el_dt_sep1, NULL, ".");
M2_U8NUM(el_dt_month, "c2", 1,12,&dt_month);
M2_LABEL(el_dt_sep2, NULL, ".");
M2_U8NUM(el_dt_year, "c2", 0,99,&dt_year);

M2_LIST(list_date) = { &el_dt_day, &el_dt_sep1, &el_dt_month, &el_dt_sep2, &el_dt_year };
M2_HLIST(el_date, NULL, list_date);

M2_ROOT(el_dt_cancel, NULL, "cancel", &el_top);
M2_BUTTON(el_dt_ok, NULL, "ok", dt_ok_fn);
M2_LIST(list_dt_buttons) = {&el_dt_cancel, &el_dt_ok };
M2_HLIST(el_dt_buttons, NULL, list_dt_buttons);

M2_LIST(list_dt) = {&el_date, &el_dt_buttons };
M2_VLIST(el_top_dt, NULL, list_dt);




/*=========================================================================*/

const char *el_strlist_getstr(uint8_t idx, uint8_t msg) 
{
  const char *s = "";
  if  ( idx == 0 )
    s = "show millis";
  else if ( idx == 1 )
    s = "show volt";
  else if ( idx == 2 )
    s = "show time";
  else if ( idx == 3 )
    s = "set time";
  else if ( idx == 4 )
    s = "set date";
  else if ( idx == 5 )
    s = "file select";
  if (msg == M2_STRLIST_MSG_GET_STR) 
  {
    /* nothing else todo, return the correct string */
  } 
  else if ( msg == M2_STRLIST_MSG_SELECT ) 
  {
    if ( idx <= 2 )
    {
      info_screen_state = idx;   
      m2.setRoot(&m2_null_element);      
    }
    if ( idx == 3 )
    {
      td_get_from_RTC();
      m2.setRoot(&el_top_td);      
    }
    if ( idx == 4 )
    {
      dt_get_from_RTC();
      m2.setRoot(&el_top_dt);
    }
    if ( idx == 5 )
    {
      pinMode(4, OUTPUT);
      pinMode(5, OUTPUT);
      pinMode(6, INPUT);
      pinMode(7, OUTPUT);
      pinMode(23, OUTPUT);
      if (sd.init(SPI_HALF_SPEED, 23))
      {
        fs_file_cnt = 255;
        fs_update_file_cnt();
        m2.setRoot(&el_top_fs);
      }
    }
  }
  return s;
}

uint8_t el_strlist_first = 0;
uint8_t el_strlist_cnt = 6;

M2_STRLIST(el_strlist, "l2w13", &el_strlist_first, &el_strlist_cnt, el_strlist_getstr);
//M2_SPACE(el_space, "w1h1");
M2_VSB(el_strlist_vsb, "l2w1r1", &el_strlist_first, &el_strlist_cnt);
M2_LIST(list_strlist) = { &el_strlist, &el_strlist_vsb };
M2_HLIST(el_top, NULL, list_strlist);


M2_LABEL(hello_world_label, NULL, "Hello World!");
M2tk m2(&el_top, m2_es_arduino, m2_eh_6bs, m2_gh_doglcd);



/*=========================================================================*/
/* arduino entry points */

void setup() 
{
  // 1. Assign the DogLcd object to m2tklib
  m2_SetDogLcd(&lcd, DOG_LCD_M162,0x25,DOG_LCD_VCC_3V3);
  // 2. Set the backlight (sequence is important!) 
  lcd.setBacklight(1, 0);
  // 3. finish setup for m2tk
  m2.setPin(M2_KEY_SELECT, uiKeySelectPin);
  m2.setPin(M2_KEY_NEXT, uiKeyRightPin);
  m2.setPin(M2_KEY_PREV, uiKeyLeftPin);
  m2.setPin(M2_KEY_DATA_UP, uiKeyUpPin);
  m2.setPin(M2_KEY_DATA_DOWN, uiKeyDownPin);

}

void loop() 
{
  if ( m2.getRoot() == &m2_null_element ) 
  {
    info_screen_display();
    if ( m2.getKey() != M2_KEY_NONE )
      m2.setRoot(&el_top);
  } 
 
  m2.checkKey();
  m2.checkKey();
  if ( m2.handleKey() )
    m2.draw();
  m2.checkKey();
}


