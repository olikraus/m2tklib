

#include "SDL.h"

#include "m2ghu8g.h"
#include "mas.h"

#include <stdlib.h>
#include <stdio.h>


int u8g_sdl_get_key(void);
void screenshot(void);
void screenshot100(void);

/* 
  event source for SDL
  this has been copied from m2ghsdl.c
  u8glib only replaces the graphics part, but not the event handling for sdl
  it is assumed, that SDL has been setup
*/
uint8_t m2_es_sdl(m2_p ep, uint8_t msg)
{
  switch(msg)
  {
    case M2_ES_MSG_GET_KEY:
    {
    	SDL_Event event;
	    /* http://www.libsdl.org/cgi/docwiki.cgi/SDL_PollEvent */
	    if ( SDL_PollEvent(&event) != 0 )
	    {
	      switch (event.type) 
	      {
	        case SDL_QUIT:
		        exit(0);
		        break;
	        case SDL_KEYDOWN:
		        switch( event.key.keysym.sym )
		        {
		          case SDLK_s:
		            return M2_KEY_EVENT(M2_KEY_SELECT);
		          case SDLK_x:
		            puts("SDLK_x");
		            return M2_KEY_EVENT(M2_KEY_EXIT);
		          case SDLK_n:
		            puts("SDLK_n");
		            return M2_KEY_EVENT(M2_KEY_NEXT);
		          case SDLK_p:
		            puts("SDLK_p");
		            return M2_KEY_EVENT(M2_KEY_PREV);
		          case SDLK_u:
		            puts("SDLK_u");
		            return M2_KEY_EVENT(M2_KEY_DATA_UP);
		          case SDLK_d:
		            puts("SDLK_d");
		            return M2_KEY_EVENT(M2_KEY_DATA_DOWN);
		          case SDLK_o:                  // screenshot
		            puts("SDLK_o (screenshOt)");
                            // screenshot();
                            screenshot100();
                            break;
		          case SDLK_q:
		            exit(0);
		            break;
		          default:
		            break;
		        }
		        break;
	        }
      	}
      }
      return M2_KEY_NONE;
    case M2_ES_MSG_INIT:
      break;
  }
  return 0;
}

/*======================================================================*/
/* box */
// uint8_t active_encoding = 35;
// uint8_t inactive_encoding = 33;

/* circle */
uint8_t active_encoding = 73;
uint8_t inactive_encoding = 75;

/* diamond */
// uint8_t active_encoding = 72;
// uint8_t inactive_encoding = 71;



/*======================================================================*/
/* extern ref to top element */
M2_EXTERN_ALIGN(top_el_tlsm);

/* dummy button callback */
void fn_dummy(m2_el_fnarg_p fnarg) {
  /* accept selection */
}

/*======================================================================*/
/* radio buttons */

uint8_t rb_select_color = 0;


M2_LABEL(el_rb_label1, NULL, "red");
M2_RADIO(el_rb_radio1, "v0", &rb_select_color);

M2_LABEL(el_rb_label2, NULL, "green");
M2_RADIO(el_rb_radio2, "v1", &rb_select_color);

M2_LABEL(el_rb_label3, NULL, "blue");
M2_RADIO(el_rb_radio3, "v2", &rb_select_color);

M2_ROOT(el_rb_goto_top, NULL, " top menu ", &top_el_tlsm);


M2_LIST(list_rb) = { 
    &el_rb_label1, &el_rb_radio1, 
    &el_rb_label2, &el_rb_radio2,  
    &el_rb_label3, &el_rb_radio3, 
    &el_rb_goto_top
};
M2_GRIDLIST(el_top_rb, "c2",list_rb);


/*======================================================================*/
/* text entry */
char text_str[5] = "(AgI";

M2_LABEL(el_te_l1, NULL, "TE a0:");
M2_TEXT(el_te1, "a0", text_str, 4);
M2_LABEL(el_te_l2, NULL, "TE a1:");
M2_TEXT(el_te2, "a1", text_str, 4);
M2_ROOT(el_te_goto_top, NULL, "top menu", &top_el_tlsm);

M2_LIST(list_te) = { 
    &el_te_l1, &el_te1, 
    &el_te_l2, &el_te2,  
    &el_te_goto_top 
};
M2_GRIDLIST(el_top_te, "c2", list_te);


/*======================================================================*/
/* button examples */


M2_BUTTON(el_btn_highlight, "w125f4", "highlight", fn_dummy);
M2_BUTTON(el_btn_normal, "w128f0", "normal", fn_dummy);

M2_ROOT(el_btn_goto_top, "w128", "top menu", &top_el_tlsm);

M2_LIST(list_btn) = { 
    &el_btn_highlight, 
    &el_btn_normal, 
    &el_btn_goto_top
};

M2_VLIST(el_top_btn, NULL, list_btn);


/*======================================================================*/
/* combo examples */

uint8_t select_color = 0;
uint8_t select_priority = 0;

void fn_ok(m2_el_fnarg_p fnarg) {
  /* accept selection */
}

void fn_cancel(m2_el_fnarg_p fnarg) {
  /* discard selection */
  m2_SetRoot(&top_el_tlsm);
}

const char *fn_idx_to_color(uint8_t idx)
{
  if ( idx == 0 )
    return "red";
  else if (idx == 1 )
    return "green";
  return "blue";
}

const char *fn_idx_to_priority(uint8_t idx)
{
  switch(idx)
  {
    case 0: return "lowest";
    case 1: return "low";
    case 2: return "medium";
    case 3: return "high";
    case 4: return "highest";
  }
  return "";
}


M2_LABEL(el_label1, NULL, "Color:");
M2_COMBO(el_combo1, NULL, &select_color, 3, fn_idx_to_color);

M2_LABEL(el_label2, NULL, "Priority: ");
M2_COMBO(el_combo2, "v1", &select_priority, 5, fn_idx_to_priority);

M2_BUTTON(el_cancel, "f4", "cancel", fn_cancel);
M2_BUTTON(el_ok, "f4", "ok", fn_ok);

M2_LIST(list_combo) = { 
    &el_label1, &el_combo1, 
    &el_label2, &el_combo2,  
    &el_cancel, &el_ok 
};
M2_GRIDLIST(el_top_combo, "c2", list_combo);

/*======================================================================*/
/* save as (reuse buttons from combo) */
char save_as_str[13] = "filename.txt";

M2_LABEL(el_sa_label, NULL, "Save as:");
M2_TEXT(el_sa_text, "a0", save_as_str, 12);

M2_LIST(list_ok_cancel) = {     &el_cancel, &el_ok };
M2_HLIST(el_ok_cancel, NULL, list_ok_cancel);

M2_LIST(list_sa) = {     &el_sa_label, &el_sa_text, &el_ok_cancel };
M2_VLIST(el_sa, NULL, list_sa);
 
M2_ALIGN(top_el_sa, "-1|1W64H64", &el_sa);

/*======================================================================*/
/* speed mph kmh */
uint8_t speed = 108;
M2_U8NUM(el_num_speed, "f2r0", 0, 255, &speed);

M2_LABEL(el_speed_unit, NULL, "mph");

M2_LIST(list_speed_num) = { &el_num_speed, &el_speed_unit };
M2_HLIST(el_speed_num, NULL, list_speed_num);

M2_SPACE(el_speed_space, "h10w1");


M2_LIST(list_speed) = { &el_speed_num, &el_speed_space, &el_ok_cancel };
M2_VLIST(el_speed, NULL, list_speed);
 
M2_ALIGN(top_el_speed, "-1|1W64H64", &el_speed);

/*======================================================================*/
/* number entry */

uint8_t u8num = 0;
uint32_t u32num = 0;

void fn_num_zero(m2_el_fnarg_p fnarg) {
  u8num = 0;
  u32num = 0;
}

M2_LABEL(el_num_label1, NULL, "U8:");
M2_U8NUM(el_num_1, NULL, 0, 255, &u8num);

M2_LABEL(el_num_label2, NULL, "U32:");
M2_U32NUM(el_num_2, "c5", &u32num);

M2_BUTTON(el_num_zero, "f4", " zero ", fn_num_zero);
M2_ROOT(el_num_goto_top, "f4", " back ", &top_el_tlsm);

M2_LIST(num_list) = { 
    &el_num_label1, &el_num_1, 
    &el_num_label2, &el_num_2,  
    &el_num_zero, &el_num_goto_top
};
M2_GRIDLIST(el_num_menu, "c2", num_list);


/*======================================================================*/
/* gfx handler selection: gfx */

const char *el_gfx_strlist_cb(uint8_t idx, uint8_t msg) {
  const char *s = "";
  if  ( idx == 0 )
    s = "fb";
  else if ( idx == 1 )
    s = "bf";
  else if ( idx == 2 )
    s = "bfs";
  else if ( idx == 3 )
    s = "ffs";

  if (msg == M2_STRLIST_MSG_GET_STR) {
    /* nothing else todo, return the correct string */
  } 
  else if ( msg == M2_STRLIST_MSG_SELECT ) {
    if ( idx == 0 ) {
      m2_SetGraphicsHandler(m2_gh_u8g_fb);
    }
    else if ( idx == 1 ) {
      m2_SetGraphicsHandler(m2_gh_u8g_bf);
    }
    else if ( idx == 2 ) {
      m2_SetGraphicsHandler(m2_gh_u8g_bfs);
    }
    else if ( idx == 3 ) {
      m2_SetGraphicsHandler(m2_gh_u8g_ffs);
    }
    m2_SetRoot(&top_el_tlsm);
  }
  return s;
}

uint8_t el_gfx_first = 0;
uint8_t el_gfx_cnt = 4;

M2_STRLIST(el_gfx_strlist, "l3W56", &el_gfx_first, &el_gfx_cnt, el_gfx_strlist_cb);
M2_SPACE(el_gfx_space, "W1h1");
M2_VSB(el_gfx_vsb, "l3W4r1", &el_gfx_first, &el_gfx_cnt);
M2_LIST(list_gfx_strlist) = { &el_gfx_strlist, &el_gfx_space, &el_gfx_vsb };
M2_HLIST(el_gfx_hlist, NULL, list_gfx_strlist);

M2_ALIGN(top_el_gfx, "-1|1W64H64", &el_gfx_hlist);


/*======================================================================*/
/* font selection:fsel */

const char *el_fsel_strlist_cb(uint8_t idx, uint8_t msg) {
  const char *s = "";

  if ( idx == 0 ) {
    s = "4x6";
    if ( msg == M2_STRLIST_MSG_SELECT ) {
      m2_SetFont(0, (const void *)u8g_font_4x6);
      m2_SetU8gToggleFontIcon(u8g_font_6x12_75r, active_encoding, inactive_encoding);
      m2_SetU8gRadioFontIcon(u8g_font_6x12_75r, active_encoding, inactive_encoding);
      m2_SetRoot(&top_el_tlsm);
    }
  }
  else if ( idx == 1 ) {
    s = "5x8";
    if ( msg == M2_STRLIST_MSG_SELECT ) {
      m2_SetFont(0, (const void *)u8g_font_5x8);
      m2_SetU8gToggleFontIcon(u8g_font_6x12_75r, active_encoding, inactive_encoding);
      m2_SetU8gRadioFontIcon(u8g_font_6x12_75r, active_encoding, inactive_encoding);
      m2_SetRoot(&top_el_tlsm);
    }
  }
  else if ( idx == 2 ) {
    s = "6x12";
    if ( msg == M2_STRLIST_MSG_SELECT ) {
      m2_SetFont(0, (const void *)u8g_font_6x12);
      m2_SetU8gToggleFontIcon(u8g_font_6x12_75r, active_encoding, inactive_encoding);
      m2_SetU8gRadioFontIcon(u8g_font_6x12_75r, active_encoding, inactive_encoding);
      m2_SetRoot(&top_el_tlsm);
    }
  }
  else if ( idx == 3 ) {
    s = "6x13";
    if ( msg == M2_STRLIST_MSG_SELECT ) {
      m2_SetFont(0, (const void *)u8g_font_6x13);
      m2_SetU8gToggleFontIcon(u8g_font_6x13_75r, active_encoding, inactive_encoding);
      m2_SetU8gRadioFontIcon(u8g_font_6x13_75r, active_encoding, inactive_encoding);
      m2_SetRoot(&top_el_tlsm);
    }
  }
  else if ( idx == 4 ) {
    s = "7x13";
    if ( msg == M2_STRLIST_MSG_SELECT ) {
      m2_SetFont(0, (const void *)u8g_font_7x13);
      m2_SetU8gToggleFontIcon(u8g_font_7x13_75r, active_encoding, inactive_encoding);
      m2_SetU8gRadioFontIcon(u8g_font_7x13_75r, active_encoding, inactive_encoding);
      m2_SetRoot(&top_el_tlsm);
    }
  }
  else if ( idx == 5 ) {
    s = "8x13";
    if ( msg == M2_STRLIST_MSG_SELECT ) {
      m2_SetFont(0, (const void *)u8g_font_8x13);
      m2_SetU8gToggleFontIcon(u8g_font_8x13_75r, active_encoding, inactive_encoding);
      m2_SetU8gRadioFontIcon(u8g_font_8x13_75r, active_encoding, inactive_encoding);
      m2_SetRoot(&top_el_tlsm);
    }
  }
  else if ( idx == 6 ) {
    s = "tpss";
    if ( msg == M2_STRLIST_MSG_SELECT ) {
      m2_SetFont(0, (const void *)u8g_font_tpss);
      m2_SetU8gToggleFontIcon(u8g_font_7x13_75r, active_encoding, inactive_encoding);
      m2_SetU8gRadioFontIcon(u8g_font_7x13_75r, active_encoding, inactive_encoding);
      m2_SetRoot(&top_el_tlsm);
    }
  }
  else if ( idx == 7 ) {
    s = "cu12";
    if ( msg == M2_STRLIST_MSG_SELECT ) {
      m2_SetFont(0, (const void *)u8g_font_cu12);
      m2_SetU8gToggleFontIcon(u8g_font_cu12_75r, active_encoding, inactive_encoding);
      m2_SetU8gRadioFontIcon(u8g_font_cu12_75r, active_encoding, inactive_encoding);
      m2_SetRoot(&top_el_tlsm);
    }
  }
  else if ( idx == 8 ) {
    s = "unifont";
    if ( msg == M2_STRLIST_MSG_SELECT ) {
      m2_SetFont(0, (const void *)u8g_font_unifont);
      m2_SetU8gToggleFontIcon(u8g_font_unifont_75r, active_encoding, inactive_encoding);
      m2_SetU8gRadioFontIcon(u8g_font_unifont_75r, active_encoding, inactive_encoding);
      m2_SetRoot(&top_el_tlsm);
    }
  }
  else if ( idx == 9 ) {
    s = "9x15";
    if ( msg == M2_STRLIST_MSG_SELECT ) {
      m2_SetFont(0, (const void *)u8g_font_9x15);
      m2_SetU8gToggleFontIcon(u8g_font_9x15_75r, active_encoding, inactive_encoding);
      m2_SetU8gRadioFontIcon(u8g_font_9x15_75r, active_encoding, inactive_encoding);
      m2_SetRoot(&top_el_tlsm);
    }
  }
  return s;
}

uint8_t el_fsel_first = 0;
uint8_t el_fsel_cnt = 10;

M2_STRLIST(el_fsel_strlist, "l3W56", &el_fsel_first, &el_fsel_cnt, el_fsel_strlist_cb);
M2_SPACE(el_fsel_space, "W1h1");
M2_VSB(el_fsel_vsb, "l3W4r1", &el_fsel_first, &el_fsel_cnt);
M2_LIST(list_fsel_strlist) = { &el_fsel_strlist, &el_fsel_space, &el_fsel_vsb };
M2_HLIST(el_fsel_hlist, NULL, list_fsel_strlist);

M2_ALIGN(top_el_fsel, "-1|1W64H64", &el_fsel_hlist);

/*======================================================================*/
/* Edit x width properties */

void fn_xwp_zero(m2_el_fnarg_p fnarg) {
  m2_gh_u8g_invisible_frame_border_x_size = 0;
  m2_gh_u8g_additional_text_border_x_size = 0;
}

M2_LABEL(el_xwp_label1, NULL, "Add to Frame:");
M2_U8NUM(el_xwp_1, "c1", 0, 4, &m2_gh_u8g_invisible_frame_border_x_size);

M2_LABEL(el_xwp_label2, NULL, "Add to Text:");
M2_U8NUM(el_xwp_2, "c1", 0, 4, &m2_gh_u8g_additional_text_border_x_size);

M2_ROOT(el_xwp_goto_top, "f4", "Goto Top Menu", &top_el_tlsm);
M2_BUTTON(el_xwp_zero, "f4", "Zero", fn_xwp_zero);

M2_LIST(xwp_list) = { 
    &el_xwp_label1, &el_xwp_1, 
    &el_xwp_label2, &el_xwp_2,  
    &el_xwp_goto_top, &el_xwp_zero
};
M2_GRIDLIST(el_xwp_menu, "c2", xwp_list);
M2_ALIGN(top_el_xwp, "-1|1W64H64", &el_xwp_menu);

/*======================================================================*/
/* hello world label */
M2_LABEL(hello_world_label, NULL, "Hello World");

/*======================================================================*/
/* Edit a long int number */
uint32_t number = 1234;
M2_U32NUM(el_u32num, "a1c4", &number);


/*======================================================================*/
/* StrList example */
const char *selected = "Nothing";
const char *el_strlist_getstr(uint8_t idx, uint8_t msg) {
  const char *s = "";
  if  ( idx == 0 )
    s = "Apple";
  else if ( idx == 1 )
    s = "Banana";
  else if ( idx == 2 )
    s = "Peach";
  else if ( idx == 3 )
    s = "Pumpkin";
  else if ( idx == 4 )
    s = "Corn";
  else if ( idx == 5 )
    s = "[Main Menu]";
  if (msg == M2_STRLIST_MSG_GET_STR) {
    /* nothing else todo, return the correct string */
  } else if ( msg == M2_STRLIST_MSG_SELECT ) {
    if ( idx == 5 ) {
      m2_SetRoot(&top_el_tlsm);
    } 
    else {
      selected = s;    
    }
  }
  return s;
}

uint8_t el_strlist_first = 0;
uint8_t el_strlist_cnt = 6;

M2_STRLIST(el_strlist, "l2w90", &el_strlist_first, &el_strlist_cnt, el_strlist_getstr);
M2_SPACE(el_strlist_space, "w1h1");
M2_VSB(el_strlist_vsb, "l2w5r1", &el_strlist_first, &el_strlist_cnt);
M2_LIST(list_strlist) = { &el_strlist, &el_strlist_space, &el_strlist_vsb };
M2_HLIST(el_strlist_hlist, NULL, list_strlist);

M2_LABEL(el_strlist_label,NULL, "Selected:");
M2_LABELPTR(el_strlist_labelptr,NULL, &selected);
M2_LIST(list_strlist_label) = { &el_strlist_label, &el_strlist_labelptr };
M2_HLIST(el_strlist_label_hlist, NULL, list_strlist_label);

M2_LIST(strlist_list) = { &el_strlist_hlist, &el_strlist_label_hlist };
M2_VLIST(el_strlist_vlist, NULL, strlist_list);
M2_ALIGN(top_el_strlist, "-1|1W64H64", &el_strlist_vlist);


/*======================================================================*/
/* multi selection */

#define MULTI_SELECT_CNT 3
const char *multi_select_strings[MULTI_SELECT_CNT] = { "red", "green", "blue" };
uint8_t multi_select_status[MULTI_SELECT_CNT] = { 0, 0, 0};

uint8_t el_muse_first = 0;
uint8_t el_muse_cnt = MULTI_SELECT_CNT;

const char *el_muse_strlist_cb(uint8_t idx, uint8_t msg) {
  const char *s = "";
  if ( msg == M2_STRLIST_MSG_SELECT ) {
    if ( multi_select_status[idx] == 0 ) {
      multi_select_status[idx] = 1;
    }
    else {
      multi_select_status[idx] = 0;
    }
  }
  if ( msg == M2_STRLIST_MSG_GET_STR ) {
    s = multi_select_strings[idx];
  }
  if ( msg == M2_STRLIST_MSG_GET_EXTENDED_STR ) {
    if ( multi_select_status[idx] == 0 ) {
      s = "\x15";
    }
    else {
      s = "\x14";
    }
  }
  return s;  
}

M2_STRLIST(el_muse_strlist, "l3F3E10W46", &el_muse_first, &el_muse_cnt, el_muse_strlist_cb);
M2_ROOT(el_muse_goto_top, "f4", "Goto Top Menu", &top_el_tlsm);

M2_LIST(muse_list) = { 
    &el_muse_strlist, 
    &el_muse_goto_top,  
};
M2_VLIST(el_muse_vlist, "c2", muse_list);
M2_ALIGN(top_el_muse, "-1|1W64H64", &el_muse_vlist);

/*======================================================================*/
/* file selection dialog */

#define FS_EXTRA_MENUES 1

/* helper variables for the strlist element */
uint8_t fs_m2tk_first = 0;
uint8_t fs_m2tk_cnt = 0;

void fs_set_cnt(void)
{
  if ( mas_GetDirEntryCnt()+FS_EXTRA_MENUES < 255 )
    fs_m2tk_cnt = mas_GetDirEntryCnt()+FS_EXTRA_MENUES;
  else
    fs_m2tk_cnt = 255;
}

const char *fs_strlist_getstr(uint8_t idx, uint8_t msg) 
{
  
  /* process message */
  if (msg == M2_STRLIST_MSG_GET_STR) 
  {
    if ( idx == 0 )
      return "Back";
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
        m2_SetRoot(&top_el_tlsm);      
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

M2_STRLIST(el_fs_strlist, "l5F3e15W47", &fs_m2tk_first, &fs_m2tk_cnt, fs_strlist_getstr);
M2_SPACE(el_fs_space, "W1h1");
M2_VSB(el_fs_strlist_vsb, "l5W4r1", &fs_m2tk_first, &fs_m2tk_cnt);
M2_LIST(list_fs_strlist) = { &el_fs_strlist, &el_fs_space, &el_fs_strlist_vsb };
M2_HLIST(el_top_fs, NULL, list_fs_strlist);



/*======================================================================*/
/* menue examples: show several menue types 
  VLIST Menu: M2_ROOT within M2_VLIST
    Advantage: No callback procedure required, automatic menu width calculation
  STRLIST Menu: M2_STRLIST 
    Advantage: Scrollable, simple form has only one element

  Nested Menues:
    VLIST and STRLIST Menus can nest each other.

*/

M2_EXTERN_ALIGN(top_el_select_menu);

/*=== VLIST Sub-Menu ===*/
/* each menu line is defined by a M2_ROOT element */
M2_ROOT(el_vlistsum_m1, NULL, "Sub-Menu 1", &top_el_select_menu);
M2_ROOT(el_vlistsum_m2, NULL, "Sub-Menu 2", &top_el_select_menu);
M2_ROOT(el_vlistsum_m3, NULL, "Sub-Menu 3", &top_el_select_menu);
M2_ROOT(el_vlistsum_m4, NULL, "Sub-Menu 4", &top_el_select_menu);
/* all menu lines are grouped by a vlist element */
M2_LIST(list_vlistsum) = { &el_vlistsum_m1, &el_vlistsum_m2, &el_vlistsum_m3, &el_vlistsum_m4 };
M2_VLIST(el_vlistsum_list, NULL, list_vlistsum);
/* center the menu on the display */
M2_ALIGN(top_el_vlist_submenu, "-1|1W64H64", &el_vlistsum_list);

/*=== VLIST Main-Menu with VLIST Sub-Menues ===*/
/* each menu line is defined by a M2_ROOT element */
/* in this example, all four menues refer to the same submenu: top_el_vlist_submenu */
M2_ROOT(el_vlistmm_m1, NULL, "Menu 1", &top_el_vlist_submenu);
M2_ROOT(el_vlistmm_m2, NULL, "Menu 2", &top_el_vlist_submenu);
M2_ROOT(el_vlistmm_m3, NULL, "Menu 3", &top_el_vlist_submenu);
M2_ROOT(el_vlistmm_m4, NULL, "Menu 4", &top_el_vlist_submenu);

/* all menu lines are grouped by a vlist element */
M2_LIST(list_vlistmm) = { &el_vlistmm_m1, &el_vlistmm_m2, &el_vlistmm_m3, &el_vlistmm_m4 };
M2_VLIST(el_vlistmm_list, NULL, list_vlistmm);

/* center the menu on the display */
M2_ALIGN(top_el_vlist_mainmenu, "-1|1W64H64", &el_vlistmm_list);

/*=== STRLIST Sub-Menu ===*/
/* all STRLIST elements requre the following two variables */
uint8_t el_strlistsum_first = 0;
uint8_t el_strlistsum_cnt = 4;
/* sub menu names can be stored in a string list */
const char * strlistsumdefs[] = { "Sub-Menu 1", "Sub-Menu 2", "Sub-Menu 3", "Sub-Menu 4" };  
/* the STRLIST element always requires a callback procedure */
const char *el_strlistsum_cb(uint8_t idx, uint8_t msg) {
  /* for any value in "idx", jumpo back to the selection menu */
  if ( msg == M2_STRLIST_MSG_SELECT )
    m2_SetRoot(&top_el_select_menu);
  
  /* "idx" contains the selected or requested submenu line (starts with 0) */
  return strlistsumdefs[idx];;
}

M2_STRLIST(el_strlistsum, "l4W64", &el_strlistsum_first, &el_strlistsum_cnt, el_strlistsum_cb);
/* center the STRLIST sub-menu on the display */
M2_ALIGN(top_el_strlist_submenu, "-1|1W64H64", &el_strlistsum);


/*=== STRLIST Main-Menu with STRLIST Sub-Menues ===*/
/* all STRLIST elements requre the following two variables */
uint8_t el_strlistmm_first = 0;
uint8_t el_strlistmm_cnt = 4;
/* sub menu names can be stored in a string list */
const char * strlistmmdefs[] = { "Menu 1", "Menu 2", "Menu 3", "Menu 4" };  
/* the STRLIST element always requires a callback procedure */
const char *el_strlistmm_cb(uint8_t idx, uint8_t msg) {
  /* for any value in "idx", jumpo to the sub-menu */
  if ( msg == M2_STRLIST_MSG_SELECT )
    m2_SetRoot(&top_el_strlist_submenu);
  
  /* "idx" contains the selected or requested submenu line (starts with 0) */
  return strlistmmdefs[idx];;
}

M2_STRLIST(el_strlistmm, "l4W64", &el_strlistmm_first, &el_strlistmm_cnt, el_strlistmm_cb);
/* center the STRLIST sub-menu on the display */
M2_ALIGN(top_el_strlist_mainmenu, "-1|1W64H64", &el_strlistmm);

/*=== Expandable Menu ===*/

m2_menu_entry exmedef[] = 
{
  { M2_PSTR("Menu 1"), NULL },
  { M2_PSTR(". Sub 1-1"), &top_el_select_menu },
  { M2_PSTR(". Sub 1-2"), &top_el_select_menu },
  { M2_PSTR("Menu 2"), &top_el_select_menu },
  { M2_PSTR("Menu 3"), NULL },
  { M2_PSTR(". Sub 3-1"), &top_el_select_menu },
  { M2_PSTR(". Sub 3-2"), &top_el_select_menu },
  { M2_PSTR("Menu 4"), &top_el_select_menu },
  { M2_PSTR("Menu 5"), NULL },
  { M2_PSTR(". Sub 5-1"), &top_el_select_menu },
  { M2_PSTR(". Sub 5-2"), &top_el_select_menu },
  { M2_PSTR(". Sub 5-3"), &top_el_select_menu },
  { NULL, NULL },
};



//M2_STRLIST(el_exme_strlist, "l4e15W47", &el_exme_first, &el_exme_cnt, el_exme_strlist_cb);
/* this has been removed, use M2_2LMENU
M2_STRLIST(el_exme_strlist, "l4e15W47", &m2_strlist_menu_first, &m2_strlist_menu_cnt, m2_strlist_menu_cb);

M2_SPACE(el_exme_space, "W1h1");
M2_VSB(el_exme_vsb, "l4W4r1", &m2_strlist_menu_first, &m2_strlist_menu_cnt);
M2_LIST(list_exme_strlist) = { &el_exme_strlist, &el_exme_space, &el_exme_vsb };
M2_HLIST(el_exme_hlist, NULL, list_exme_strlist);
M2_ALIGN(top_el_expandable_menu, "-1|1W64H64", &el_exme_hlist);
*/

/*=== Expandable Menu 2 ===*/

uint8_t el_2lme_first = 0;
uint8_t el_2lme_cnt = 3;

/* for m2icon fonts, 65: closed folder, 102: open folder */
M2_2LMENU(el_2lme_strlist, "l4e15F3W47", &el_2lme_first, &el_2lme_cnt, exmedef, 65, 102, '\0');
M2_SPACE(el_2lme_space, "W1h1");
M2_VSB(el_2lme_vsb, "l4W4r1", &el_2lme_first, &el_2lme_cnt);
M2_LIST(list_2lme_strlist) = { &el_2lme_strlist, &el_2lme_space, &el_2lme_vsb };
M2_HLIST(el_2lme_hlist, NULL, list_2lme_strlist);
M2_ALIGN(top_el_2l_menu, "-1|1W64H64", &el_2lme_hlist);

/*=== Menu Selection ===*/

uint8_t el_seme_first = 0;
uint8_t el_seme_cnt = 3;
const char *el_seme_strlist_cb(uint8_t idx, uint8_t msg) {
  const char *s = "";
  if ( idx == 0 ) {
    s = "VLIST->VLIST";
    if ( msg == M2_STRLIST_MSG_SELECT )
      m2_SetRoot(&top_el_vlist_mainmenu);
  }
  else if ( idx == 1 ) {
    s = "STRLIST->STRLIST";
    if ( msg == M2_STRLIST_MSG_SELECT )
      m2_SetRoot(&top_el_strlist_mainmenu);
  }
  /*
  else if ( idx == 2 ) {
    s = "Expandable Menu";
    if ( msg == M2_STRLIST_MSG_SELECT )
    {
      m2_SetStrlistMenuData(exmedef, '+', '-', ' ');
      m2_SetRoot(&top_el_expandable_menu);
    }
  }
  */
  else if ( idx == 2 ) {
    s = "2L Menu";
    if ( msg == M2_STRLIST_MSG_SELECT )
    {
      //el_exme_update_cnt();
      m2_SetRoot(&top_el_2l_menu);
    }
  }

  
  return s;
}


M2_STRLIST(el_seme_strlist, "l3W56", &el_seme_first, &el_seme_cnt, el_seme_strlist_cb);
M2_SPACE(el_seme_space, "W1h1");
M2_VSB(el_seme_vsb, "l3W4r1", &el_seme_first, &el_seme_cnt);
M2_LIST(list_seme_strlist) = { &el_seme_strlist, &el_seme_space, &el_seme_vsb };
M2_HLIST(el_seme_hlist, NULL, list_seme_strlist);
M2_ALIGN(top_el_select_menu, "-1|1W64H64", &el_seme_hlist);



/*======================================================================*/
/* top level sdl menu: tlsm */


const char *el_tlsm_strlist_cb(uint8_t idx, uint8_t msg) {
  const char *s = "";

  if ( idx == 0 ) {
    s = "Select Style";
    if ( msg == M2_STRLIST_MSG_SELECT )
      m2_SetRoot(&top_el_gfx);
  }
  else if ( idx == 1 ) {
    s = "Adjust Style";
    if ( msg == M2_STRLIST_MSG_SELECT )
      m2_SetRoot(&top_el_xwp);
  }  
  else if ( idx == 2 ) {
    s = "Select Font";
    if ( msg == M2_STRLIST_MSG_SELECT )
      m2_SetRoot(&top_el_fsel);
  }  
  else if ( idx == 3 ) {
    s = "Buttons";
    if ( msg == M2_STRLIST_MSG_SELECT )
      m2_SetRoot(&el_top_btn);
  }
  else if ( idx == 4 ) {
    s = "Text Entry";
    if ( msg == M2_STRLIST_MSG_SELECT )
      m2_SetRoot(&el_top_te);
  }
  else if ( idx == 5 ) {
    s = "Radio";
    if ( msg == M2_STRLIST_MSG_SELECT )
      m2_SetRoot(&el_top_rb);
  }
  else if ( idx == 6 ) {
    s = "Combo";
    if ( msg == M2_STRLIST_MSG_SELECT )
      m2_SetRoot(&el_top_combo);
  }
  else if ( idx == 7 ) {
    s = "Number Entry";
    if ( msg == M2_STRLIST_MSG_SELECT )
      m2_SetRoot(&el_num_menu);
  }
  else if ( idx == 8 ) {
    s = "Hello World";
    if ( msg == M2_STRLIST_MSG_SELECT )
      m2_SetRoot(&hello_world_label);
  }
  else if ( idx == 9 ) {
    s = "U32Plain";
    if ( msg == M2_STRLIST_MSG_SELECT )
      m2_SetRoot(&el_u32num);
  }
  else if ( idx == 10 ) {
    s = "StrList";
    if ( msg == M2_STRLIST_MSG_SELECT )
      m2_SetRoot(&top_el_strlist);
  }
  else if ( idx == 11 ) {
    s = "MultiSelect";
    if ( msg == M2_STRLIST_MSG_SELECT )
      m2_SetRoot(&top_el_muse);
  }
  else if ( idx == 12 ) {
    s = "FileSelection";
    if ( msg == M2_STRLIST_MSG_SELECT )
    {
      fs_set_cnt();
      m2_SetRoot(&el_top_fs);
    }
  }
  else if ( idx == 13 ) {
    s = "Save As";
    if ( msg == M2_STRLIST_MSG_SELECT )
    {
      m2_SetRoot(&top_el_sa);
    }
  }
  else if ( idx == 14 ) {
    s = "Speed";
    if ( msg == M2_STRLIST_MSG_SELECT )
    {
      m2_SetRoot(&top_el_speed);
    }
  }
  else if ( idx == 15 ) {
    s = "Menues";
    if ( msg == M2_STRLIST_MSG_SELECT )
    {
      m2_SetRoot(&top_el_select_menu);
    }
  }
  return s;
}


uint8_t el_tlsm_first = 0;
uint8_t el_tlsm_cnt = 16;

M2_STRLIST(el_tlsm_strlist, "l3W56", &el_tlsm_first, &el_tlsm_cnt, el_tlsm_strlist_cb);
M2_SPACE(el_tlsm_space, "W1h1");
M2_VSB(el_tlsm_vsb, "l3W4r1", &el_tlsm_first, &el_tlsm_cnt);
M2_LIST(list_tlsm_strlist) = { &el_tlsm_strlist, &el_tlsm_space, &el_tlsm_vsb };
M2_HLIST(el_tlsm_hlist, NULL, list_tlsm_strlist);

M2_ALIGN(top_el_tlsm, "-1|1W64H64", &el_tlsm_hlist);


/*======================================================================*/
/* master u8g object */

u8g_t u8g;

/*======================================================================*/

void screenshot(void)
{
  u8g_t screenshot_u8g;
  /* 1. Setup and create device access */
  u8g_Init(&screenshot_u8g, &u8g_dev_pbm);
  /* 2. Connect the u8g display to m2. Note: M2 is setup later */
  m2_SetU8g(&screenshot_u8g, m2_u8g_font_icon);

  u8g_FirstPage(&screenshot_u8g);
  do{
    m2_Draw();
  } while( u8g_NextPage(&screenshot_u8g) );

  m2_SetU8g(&u8g, m2_u8g_font_icon);

  {
    char cmd[256*4];
    sprintf(cmd, "convert u8g.pbm -trim -scale '200%%' %s.png", "u8g" );

    sprintf(cmd, "convert u8g.pbm -crop '128x64+0+704' -extent '130x66-1-1' -draw 'line 0 0 129 0' -draw 'line 0 65 129 65'  -scale '200%%' %s.png", "u8g" );
    sprintf(cmd, "convert u8g.pbm -extent '130x66-1-1' -draw 'line 0 0 3 0' -draw 'line 126 0 129 0' -draw 'line 0 65 3 65' -draw 'line 126 65 129 65'  -draw 'line 0 0 0 3' -draw 'line 129 0 129 3'  -draw 'line 0 62 0 65' -draw 'line 129 62 129 65' -scale '200%%' %s.png", "u8g" );

    sprintf(cmd, "convert u8g.pbm -extent '128x64-0-0' -fill orange -opaque white  -scale '200%%' %s.png", "u8g" );
    
    system(cmd);
  }
  
}

void screenshot100(void)
{
  static int cnt = 0;
  
  u8g_t screenshot_u8g;
  /* 1. Setup and create device access */
  u8g_Init(&screenshot_u8g, &u8g_dev_pbm);
  /* 2. Connect the u8g display to m2. Note: M2 is setup later */
  m2_SetU8g(&screenshot_u8g, m2_u8g_font_icon);

  u8g_FirstPage(&screenshot_u8g);
  do{
    m2_Draw();
  } while( u8g_NextPage(&screenshot_u8g) );

  m2_SetU8g(&u8g, m2_u8g_font_icon);

  {
    char cmd[256*4];
    sprintf(cmd, "convert u8g.pbm -trim -scale '200%%' %s.png", "u8g" );

    sprintf(cmd, "convert u8g.pbm -crop '128x64+0+704' -extent '130x66-1-1' -draw 'line 0 0 129 0' -draw 'line 0 65 129 65'  -scale '100%%' %s.png", "u8g" );
    sprintf(cmd, "convert u8g.pbm -extent '130x66-1-1' -draw 'line 0 0 3 0' -draw 'line 126 0 129 0' -draw 'line 0 65 3 65' -draw 'line 126 65 129 65'  -draw 'line 0 0 0 3' -draw 'line 129 0 129 3'  -draw 'line 0 62 0 65' -draw 'line 129 62 129 65' -scale '200%%' %s.png", "u8g" );

    sprintf(cmd, "convert u8g.pbm -extent '128x64-0-0' -fill yellow -opaque white  -scale '100%%' %s%03d.png", "u8g", cnt );
    
    system(cmd);
  }
  cnt++;
}


/*======================================================================*/


int main(void)
{  
  /* this is the setup sequence, steps 1..4 should be in this order */
  
  /* 1. Setup and create device access */
  u8g_Init(&u8g, &u8g_dev_sdl_1bit);
  
  /* 2. Now, setup m2 */
  m2_Init(&top_el_tlsm, m2_es_sdl, m2_eh_6bs, m2_gh_u8g_bfs);
  // m2_Init(&list_element, m2_es_sdl, m2_eh_6bs, m2_gh_u8g_fb);

  /* 3. Connect the u8g display to m2.  */
  m2_SetU8g(&u8g, m2_u8g_box_icon);

  /* 4. And finally, set at least one font, use normal u8g_font's */
  m2_SetFont(0, (const void *)u8g_font_7x13);
  m2_SetFont(2, (const void *)u8g_font_fub25);

  m2_SetU8gToggleFontIcon(u8g_font_7x13_75r, active_encoding, inactive_encoding);
  m2_SetU8gRadioFontIcon(u8g_font_7x13_75r, active_encoding, inactive_encoding);

  // m2_SetU8gAdditionalReadOnlyXBorder(0);  
  /* set the font for the multi selection */
  m2_SetFont(3, (const void *)u8g_font_m2icon_9);

  mas_Init(mas_device_sim, 0);

  for(;;)
  {
    m2_CheckKey();
    if ( m2_HandleKey() ) {
      u8g_FirstPage(&u8g);
      do{
        //u8g_SetFont(&u8g, u8g_font_unifont);
        //u8g_DrawStr(&u8g, 0, 20, "Hello World!");
        m2_CheckKey();
        m2_Draw();
      } while( u8g_NextPage(&u8g) );
    }
  }
  return 0;
}


