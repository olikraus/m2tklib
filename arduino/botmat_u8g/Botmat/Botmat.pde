/*

  Botmat.pde
  
  botmat example program for u8g/m2tklib (Botmat graphic)

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
  
  
  sd support
    SdFat (Arduino)
    http://www.mikrocontroller.net/articles/AVR_FAT32                   --> not fully clear how to read dir entries with this
    http://www.holger-klabunde.de/avr/avrboard.htm#FullFAT              --> seems to be a small implementation
    http://www.dharmanitech.com/2009/01/sd-card-interfacing-with-atmega8-fat32.html
    http://code.google.com/p/fullfat/           (http://www.fullfat-fs.co.uk/)
    http://code.google.com/p/nanofat-lib/               -->PIC
    http://elm-chan.org/fsw/ff/00index_p.html   Petit FAT File System Module
    
*/

#include "U8glib.h"
#include <DS1307new.h>
#include <Wire.h>                       // required for DS1307new.h
//#include <SdFat.h>
#include "M2tk.h"
#include "m2ghu8g.h"
#include <string.h>
#include "mas.h"

#define DEFAULT_FONT u8g_font_6x13
#define ICON_FONT u8g_font_m2icon_9
//#define ICON_FONT u8g_font_6x13
#define BIG_FONT u8g_font_fub20r

/*=========================================================================*/
/* u8g object definition for botmat graphic */

//U8GLIB_DOGM128 u8g(7, 5, 1, 2);                    // SPI Com: SCK = 7, MOSI = 5, CS = 1, A0 = 2
U8GLIB_DOGM128 u8g(1, 2);                    // HW SPI CS = 1, A0 = 2


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
  //uint8_t backup_SPCR = SPCR;
  //SPCR = 0;
  if ( info_screen_state == 0 )
  {
    u8g.setFont(BIG_FONT);
    u8g.setPrintPos(0,25);
    u8g.print(millis());
  }
  else if ( info_screen_state == 1 )
  {
    uint16_t adc = analogRead(0);
    
    u8g.setFont(DEFAULT_FONT);
    
    u8g.setPrintPos(0,25);
    u8g.print("raw:  ");
    u8g.print(adc);

    u8g.setPrintPos(0,50);
    u8g.print("volt: ");
    u8g.print((adc*3.3)/512.0);    
  }
  else if ( info_screen_state == 2 )
  {
    RTC.getTime();

    u8g.setFont(BIG_FONT);
    
    u8g.setPrintPos(0,25);
    
    u8g.print(m2_utl_u8d(RTC.hour,2));
    u8g.print(":");
    u8g.print(m2_utl_u8d(RTC.minute,2));
    u8g.print(":");
    u8g.print(m2_utl_u8d(RTC.second,2));
    u8g.setPrintPos(0,50);
    u8g.print(m2_utl_u8d(RTC.day,2));
    u8g.print(".");
    u8g.print(m2_utl_u8d(RTC.month,2));
    u8g.print(".");
    u8g.print(m2_utl_u8d(RTC.year-2000,2));
    
  }
  //SPCR = backup_SPCR;

}

/*=========================================================================*/
/* file selection dialog */
/* limitation: not more than 250 elements per directory allowed */

#define FS_EXTRA_MENUES 1

/* helper variables for the strlist element */
uint8_t fs_m2tk_first = 0;
uint8_t fs_m2tk_cnt = 0;


void fs_set_cnt(void)
{
  uint16_t cnt;
  cnt = mas_GetDirEntryCnt();
  if ( cnt+FS_EXTRA_MENUES < 255 )
    fs_m2tk_cnt = cnt+FS_EXTRA_MENUES;
  else
    fs_m2tk_cnt = 255;
}

const char *fs_strlist_getstr(uint8_t idx, uint8_t msg) 
{
  /* process message */
  if (msg == M2_STRLIST_MSG_GET_STR) 
  {
    if ( idx == 0 )
   {
      return "Back";
   }
    mas_GetDirEntry(idx - FS_EXTRA_MENUES);
    return mas_entry_name;
  } 
  else if ( msg == M2_STRLIST_MSG_GET_EXTENDED_STR )
  {
    if ( idx == 0 )
      return "a";       // leave menu
    mas_GetDirEntry(idx - FS_EXTRA_MENUES);
    if ( mas_entry_is_dir )
      return "A";       // folder icon
    return "B";         // file icon
  }
  else if ( msg == M2_STRLIST_MSG_SELECT ) 
  {
    if ( idx == 0 )
    {
      if ( mas_pwd[0] == '\0' )
        m2_SetRoot(&el_top);
      else
      {
        mas_ChDirUp();
        fs_set_cnt();
        m2_SetRoot(m2_GetRoot());  // reset menu to first element
      }
    }
    else
    {
      mas_GetDirEntry(idx - FS_EXTRA_MENUES);
      if ( mas_entry_is_dir )
      {
        mas_ChDir(mas_entry_name);
        fs_set_cnt();
        m2_SetRoot(m2_GetRoot());  // reset menu to first element
      }
    }
  } 
  return "";
}





M2_STRLIST(el_fs_strlist, "l4F3e15W47", &fs_m2tk_first, &fs_m2tk_cnt, fs_strlist_getstr);
//M2_SPACE(el_fs_space, "w1h1");
M2_VSB(el_fs_strlist_vsb, "l4W2r1", &fs_m2tk_first, &fs_m2tk_cnt);
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
      fs_set_cnt();
      m2.setRoot(&el_top_fs);
    }
  }
  return s;
}

uint8_t el_strlist_first = 0;
uint8_t el_strlist_cnt = 6;

M2_STRLIST(el_strlist, "l4W55", &el_strlist_first, &el_strlist_cnt, el_strlist_getstr);
//M2_SPACE(el_space, "w1h1");
M2_VSB(el_strlist_vsb, "l4W2r1", &el_strlist_first, &el_strlist_cnt);
M2_LIST(list_strlist) = { &el_strlist, &el_strlist_vsb };
M2_HLIST(el_top, NULL, list_strlist);


M2_LABEL(hello_world_label, NULL, "Hello World!");
M2tk m2(&el_top, m2_es_arduino, m2_eh_6bs, m2_gh_u8g_ffs);


// U8glib draw procedure: Just call the M2tklib draw procedure
void draw(void) {
  if ( m2.getRoot() == &m2_null_element ) 
  {
    info_screen_display();
    if ( m2.getKey() != M2_KEY_NONE )
      m2.setRoot(&el_top);
  } 
  else
  {
    m2.draw();
  }
}

/*=========================================================================*/
/* arduino entry points */

void setup() 
{
  // activate backlight
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
  
  // Connect u8glib with m2tklib
  m2_SetU8g(u8g.getU8g(), m2_u8g_box_icon);

  // Assign u8g font to index 0
  m2.setFont(0, DEFAULT_FONT);
  
  // Assign icon font to index 3
  m2.setFont(3, ICON_FONT);
  
  // Setup keys (botmat)
  m2.setPin(M2_KEY_SELECT, uiKeySelectPin);
  m2.setPin(M2_KEY_NEXT, uiKeyRightPin);
  m2.setPin(M2_KEY_PREV, uiKeyLeftPin);
  m2.setPin(M2_KEY_DATA_UP, uiKeyUpPin);
  m2.setPin(M2_KEY_DATA_DOWN, uiKeyDownPin);

  // sd card setup
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, INPUT);
  pinMode(7, OUTPUT);		
  pinMode(23, OUTPUT);
  mas_Init(mas_device_sdfat, 23);
  //mas_Init(mas_device_sim, 23);
  //mas_Init(mas_device_sd, 23);
}

void loop() 
{
 
  m2.checkKey();
  m2.checkKey();
  if ( m2.handleKey() || m2.getRoot() == &m2_null_element ) {
    u8g.firstPage();  
    do {
      draw();
    } while( u8g.nextPage() );
  }
  m2.checkKey();
}


