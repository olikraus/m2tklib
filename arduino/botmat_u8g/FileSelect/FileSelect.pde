/*

  FileSelect.pde
  
  File selection box with u8g and m2tklib (Botmat graphic)

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

//#define FS_SdFat
//#define FS_SD
#define FS_PFF
//#define FS_SIM

#include "U8glib.h"
#include "M2tk.h"
#include "m2ghu8g.h"
#include <string.h>
#include "mas.h"


#ifdef FS_SdFat
#include <SdFat.h>
#endif

#ifdef FS_SD
#include <SD.h>
#endif

#ifdef FS_PFF
#include "pff.h"
#endif

#ifdef FS_SIM
#endif


#define DEFAULT_FONT u8g_font_6x12
#define ICON_FONT u8g_font_m2icon_7

#ifdef FS_SdFat
SdFat sdfat;
#endif

#ifdef FS_PFF
FATFS pff_fs;
#endif

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
uint8_t sd_card_status = 0;


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
      {
	mas_ClearFilename();
	m2_SetRoot(&el_top);
      }
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
      else
      {
	m2_SetRoot(&el_top);
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
/* main menu for the file select */


void mm_fs_fn(m2_el_fnarg_p fnarg) 
{
  fs_set_cnt();
  m2.setRoot(&el_top_fs);
}

const char *ptr_to_pathname = "";

const char *ptr_to_filename = "";

M2_LABEL(el_mm_path, NULL, "Path:");
M2_LABELPTR(el_mm_pathname, NULL, &ptr_to_pathname);


M2_LABEL(el_mm_selected, NULL, "Selected:");
M2_LABELPTR(el_mm_filename, NULL, &ptr_to_filename);
M2_BUTTON(el_mm_fs, NULL, "Select File", mm_fs_fn);
M2_LIST(list_mm) = {&el_mm_path, &el_mm_pathname, &el_mm_selected, &el_mm_filename, &el_mm_fs };
M2_VLIST(el_top, NULL, list_mm);

M2tk m2(&el_top, m2_es_arduino, m2_eh_6bs, m2_gh_u8g_ffs);


// U8glib draw procedure: Just call the M2tklib draw procedure
void draw(void) {
  m2.draw();
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

  sd_card_status = 0;
  ptr_to_filename = mas_entry_name;
  ptr_to_pathname = mas_pwd;

#ifdef FS_SdFat
  pinMode(SS, OUTPUT);	// force the hardware chip select to output
  if ( sdfat.init(SPI_HALF_SPEED, 23) )
  {
    sd_card_status = 1;
    mas_Init(mas_device_sdfat, (void *)&sdfat);
  }
#endif

#ifdef FS_SD
  pinMode(SS, OUTPUT);	// force the hardware chip select to output
  if (SD.begin(23))		// use the global SD object
  {
    sd_card_status = 1;
    mas_Init(mas_device_sd, NULL);
  }
#endif

#ifdef FS_PFF
  sd_card_status = 0;
  if ( pf_mount(&pff_fs) == FR_OK )
  {
    sd_card_status = 1;
    mas_Init(mas_device_pff, &pff_fs);
  }
#endif

#ifdef FS_SIM
  mas_Init(mas_device_sim, NULL);
#endif

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


