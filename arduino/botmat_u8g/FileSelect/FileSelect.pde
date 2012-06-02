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
  
*/


/*=========================================================================*/
/* select mass storage sub system */

//#define FS_SdFat
//#define FS_SD
#define FS_PFF
//#define FS_SIM

/*=========================================================================*/
/* generic includes */

#include "U8glib.h"
#include "M2tk.h"
#include "m2ghu8g.h"
#include <string.h>
#include "mas.h"

/*=========================================================================*/
/* specific includes for the mass storage sub system */

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

/*=========================================================================*/
/* specific includes for the mass storage sub system */

#ifdef FS_SdFat
SdFat sdfat;
#endif

#ifdef FS_PFF
FATFS pff_fs;
#endif

/*=========================================================================*/
/* forward declarations */

M2_EXTERN_ALIGN(top_el_start);

/*=========================================================================*/
/* u8g and m2tk object  */

//U8GLIB_DOGM128 u8g(7, 5, 1, 2);                    // SPI Com: SCK = 7, MOSI = 5, CS = 1, A0 = 2
U8GLIB_DOGM128 u8g(1, 2);                    // HW SPI CS = 1, A0 = 2

M2tk m2(&top_el_start, m2_es_arduino, m2_eh_6bs, m2_gh_u8g_ffs);


/*=========================================================================*/
/* pin numbers of the keypad */

uint8_t uiKeySelectPin = 18;
uint8_t uiKeyUpPin = 22;
uint8_t uiKeyDownPin = 19;
uint8_t uiKeyLeftPin = 20;
uint8_t uiKeyRightPin = 21;

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
      return "a";       /* arrow left of the m2icon font */
    /* Not the extra button: Return file or directory icon */
    mas_GetDirEntry(idx - FS_EXTRA_MENUES);
    if ( mas_IsDir() )
      return "A";       /* folder icon of the m2icon font */
    return "B";         /* file icon of the m2icon font */
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

M2_STRLIST(el_fs_strlist, "l5F3e15W49", &fs_m2tk_first, &fs_m2tk_cnt, fs_strlist_getstr);
M2_SPACE(el_fs_space, "W1h1");
M2_VSB(el_fs_strlist_vsb, "l5W4r1", &fs_m2tk_first, &fs_m2tk_cnt);
M2_LIST(list_fs_strlist) = { &el_fs_strlist, &el_fs_space, &el_fs_strlist_vsb };
M2_HLIST(el_top_fs, NULL, list_fs_strlist);


/*=========================================================================*/
/* a simple main menu for the file select */

M2_ROOT(el_start, NULL, "File Selection", &el_top_fs);
M2_ALIGN(top_el_start, "-1|1W64H64", &el_start);

/*=========================================================================*/
/* u8glib draw procedure: Just call the M2tklib draw procedure */

void draw(void) {
  m2.draw();
}

/*=========================================================================*/
/* arduino entry points */

void setup()  {
  // activate backlight
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
  
  // Connect u8glib with m2tklib
  m2_SetU8g(u8g.getU8g(), m2_u8g_box_icon);

  // Assign u8g font to index 0
  m2.setFont(0, u8g_font_6x10);
  
  // Assign icon font to index 3
  m2.setFont(3, u8g_font_m2icon_7);
  
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

#ifdef FS_SdFat
  pinMode(SS, OUTPUT);	// force the hardware chip select to output
  if ( sdfat.init(SPI_HALF_SPEED, 23) ) {
    mas_Init(mas_device_sdfat, (void *)&sdfat);
  }
#endif

#ifdef FS_SD
  pinMode(SS, OUTPUT);	// force the hardware chip select to output
  if (SD.begin(23)) {		// use the global SD object
    mas_Init(mas_device_sd, NULL);
  }
#endif

#ifdef FS_PFF
  if ( pf_mount(&pff_fs) == FR_OK ) {
    mas_Init(mas_device_pff, &pff_fs);
  }
#endif

#ifdef FS_SIM
  mas_Init(mas_device_sim, NULL);
#endif

}

void loop() {
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


