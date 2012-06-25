/*

  FileSelect.pde
  
  LiquidCrystal 16x4 example

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

*/


/*=========================================================================*/
/* generic includes */

#include <LiquidCrystal.h>
#include "M2tk.h"
#include "m2ghlc.h"
#include <string.h>
#include "mas.h"

/*=========================================================================*/
/* Specific sections for the mass storage sub system. */
/* Please uncomment one of the following sections (remove all // from one section)  */

/* === SdFat Library === */
/*
  1) Download and install SdFat 
  2) Copy "mas_sdfat.cpp" from M2tklib/addon/mas_sdfat.cpp to M2tklib/utility/mas_sdfat.cpp
  3) Uncomment the following tree lines
*/
//#define FS_SdFat
//#include <SdFat.h>
//SdFat sdfat;

/* === Arduino SD Library === */
/*
  1) Copy "mas_arduino_sd.cpp" from M2tklib/addon/mas_arduino_sd.cpp to M2tklib/utility/mas_arduino_sd.cpp
  2) Uncomment the following two lines
*/
//#define FS_SD
//#include <SD.h>

/* === Petit Fat File System Library === */
//#define FS_PFF
//#include "pff.h"
//FATFS pff_fs;

/* === Petit Fat File System Library === */
#define FS_SIM

/*=========================================================================*/
/* lcd object  */

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

/*=========================================================================*/
/* forward declarations */

M2_EXTERN_ALIGN(top_el_start);

/*=========================================================================*/
/* M2tk lib start */
M2tk m2(&top_el_start, m2_es_arduino, m2_eh_4bs, m2_gh_lc);

/*=========================================================================*/
/* pin numbers of the keypad */

uint8_t uiKeySelectPin = 10;
uint8_t uiKeyNextPin = 9;


/*=========================================================================*/
/* file selection box */

/* defines the number of additional buttons at the beginning of the STRLIST lines */
#define FS_EXTRA_MENUES 1

/* helper variables for the strlist element */
uint8_t fs_m2tk_first = 0;
uint8_t fs_m2tk_cnt = 0;

/* show selected file */

const char *fs_show_file_label_cb(m2_rom_void_p element) {
  return mas_GetFilename();
}

M2_LABEL(el_show_file_label, NULL, "Selected file:");
M2_LABELFN(el_show_filename, NULL, fs_show_file_label_cb);
M2_ROOT(el_show_file_ok, NULL, "ok", &top_el_start);
M2_LIST(list_show_file) = { &el_show_file_label, &el_show_filename, &el_show_file_ok };
M2_VLIST(el_show_file_Vlist, NULL, list_show_file);
M2_ALIGN(top_el_show_file, "-1|1W64H64", &el_show_file_Vlist);

/* callback procedure for the file selection dialog */
const char *fs_strlist_getstr(uint8_t idx, uint8_t msg)  {
  if (msg == M2_STRLIST_MSG_GET_STR)  {
    /* Check for the extra button: Return string for this extra button */
    if ( idx == 0 )
      return "..";
    /* Not the extra button: Return file/directory name */
    mas_GetDirEntry(idx - FS_EXTRA_MENUES);
    return mas_GetFilename();
  } else if ( msg == M2_STRLIST_MSG_GET_EXTENDED_STR ) {
    /* Check for the extra button: Return icon for this extra button */
    if ( idx == 0 )
      return "<";       /* go back */
    /* Not the extra button: Return file or directory icon */
    mas_GetDirEntry(idx - FS_EXTRA_MENUES);
    if ( mas_IsDir() )
      return "+";       /* folder */
    return " ";         /* file */
  } else if ( msg == M2_STRLIST_MSG_SELECT ) {
    /* Check for the extra button: Execute button action */
    if ( idx == 0 ) {
      if ( mas_GetPath()[0] == '\0' )
        m2_SetRoot(&top_el_start);      	/* go back to previous menu */
      else {
        mas_ChDirUp();
        m2_SetRoot(m2_GetRoot());  /* reset menu to first element, send NEW_DIALOG and force recount */
      }
    /* Not the extra button: Goto subdir or return (with selected file) */
    } else {
      mas_GetDirEntry(idx - FS_EXTRA_MENUES);
      if ( mas_IsDir() ) {
        mas_ChDir(mas_GetFilename());
        m2_SetRoot(m2_GetRoot());  /* reset menu to first element, send NEW_DIALOG and force recount */
      } else {
	/* File has been selected. Here: Show the file to the user */
        m2_SetRoot(&top_el_show_file);  
      }
    }
  } else if ( msg == M2_STRLIST_MSG_NEW_DIALOG ) {
    /* (re-) calculate number of entries, limit no of entries to 250 */
    if ( mas_GetDirEntryCnt() < 250-FS_EXTRA_MENUES )
      fs_m2tk_cnt = mas_GetDirEntryCnt()+FS_EXTRA_MENUES;
    else
      fs_m2tk_cnt = 250;
  }
  return NULL;
}

M2_STRLIST(el_fs_strlist, "l4e1w12", &fs_m2tk_first, &fs_m2tk_cnt, fs_strlist_getstr);
M2_VSB(el_fs_strlist_vsb, "l4w1r1", &fs_m2tk_first, &fs_m2tk_cnt);
M2_LIST(list_fs_strlist) = { &el_fs_strlist, &el_fs_strlist_vsb };
M2_HLIST(el_top_fs, NULL, list_fs_strlist);


/*=========================================================================*/
/* a simple main menu for the file select */

M2_ROOT(el_start, NULL, "File Selection", &el_top_fs);
M2_ALIGN(top_el_start, "-1|1W64H64", &el_start);

/*=========================================================================*/

void setup() {
  m2_SetLiquidCrystal(&lcd, 16, 4);
  m2.setPin(M2_KEY_SELECT, uiKeySelectPin);
  m2.setPin(M2_KEY_NEXT, uiKeyNextPin);

  // setup storage library and mas subsystem
  // CS=23 	Botmat Project www.botmat.cc
  // CS=10	Seeedstudio Shield http://www.seeedstudio.com/wiki/SD_Card_Shield
#if defined(FS_SdFat)
  pinMode(SS, OUTPUT);	// force the hardware chip select to output
  if ( sdfat.init(SPI_HALF_SPEED, 10) ) {
    mas_Init(mas_device_sdfat, (void *)&sdfat);
  }
  
#elif defined(FS_SD)
  pinMode(SS, OUTPUT);	// force the hardware chip select to output
  if (SD.begin(10)) {		// use the global SD object
    mas_Init(mas_device_sd, NULL);
  }
  
#elif defined(FS_PFF)
  //pff_arduino_chip_select_pin = 23; 	/* Botmat Project www.botmat.cc */
  pff_arduino_chip_select_pin = 10; 	/* Seeedstudio Shield http://www.seeedstudio.com/wiki/SD_Card_Shield */
  if ( pf_mount(&pff_fs) == FR_OK ) {
    mas_Init(mas_device_pff, &pff_fs);
  }
  
#elif defined(FS_SIM)
  mas_Init(mas_device_sim, NULL);
#endif
}

void loop() {
  m2.checkKey();
  m2.checkKey();
  if ( m2.handleKey() )
    m2.draw();
  m2.checkKey();
}
