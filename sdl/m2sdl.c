/*

  m2sdl.c
  
  sdl interactive test

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
#include "m2.h"
#include "m2utl.h"



m2_rom_char_t base_fmt[] = "w7h8a0";

/*==========================================*/

extern m2_el_align_t el_top;
M2_ROOT(el_goto_top, "f10", "Goto Top", &el_top);

/*===================================================================*/
/* common fields */
m2_el_str_t t0 = { {m2_el_str_fn, base_fmt}, "AAA" };
m2_el_str_t t1 = { {m2_el_str_fn, base_fmt}, "BBB" };
m2_el_str_t t2 = { {m2_el_str_fn, base_fmt}, "CCC" };
m2_el_str_t t3 = { {m2_el_str_up_fn, base_fmt}, "OK" };

/*===================================================================*/
/* yes/no combo */
uint8_t yn_val = 0;

const char *yes_no_fn(uint8_t idx)
{
  if ( idx == 0 )
    return "no";
  return "yes";
}


M2_COMBO(yn_combo, NULL, &yn_val, 2, yes_no_fn);


/*===================================================================*/
/* gridlist */

void *li[] = { &t0, &t1,  &t2,  &t3, &yn_combo, &el_goto_top };
m2_el_list_t el_gridlist = { { m2_el_gridlist_fn, "c3d0h50w100" } , sizeof(li)/sizeof(*li), li };

m2_el_str_t s1 = { {m2_el_str_fn, base_fmt}, "abc" };

m2_el_str_t s2 = { {m2_el_str_fn, base_fmt}, "def" };

char text[4] = "XYZ";
m2_el_text_t txt = { { m2_el_text_fn, "" } , text, 3 };

uint8_t val = 123;
m2_el_u8_t v1 = { {m2_el_u8num_fn, "" } , 1, 130, &val };
m2_el_str_t s3 = { {m2_el_str_fn, ""}, "ghi" };

void *l[] = { &s1, &txt,  &v1,  &el_gridlist, &s3 };
m2_el_list_t el = { { m2_el_vlist_fn, base_fmt } , sizeof(l)/sizeof(*l), l };

M2_ALIGN(a_eli, "h64w128x2y2", &el_gridlist);
 
/*===================================================================*/
/* list with align block */

M2_ALIGN(align_go_top, "h20w70", &el_goto_top);
M2_ALIGN(align_t0, "h20w70", &t0);

void *align_list[] = { &align_go_top, &align_t0 };
m2_el_list_t el_alignlist = { { m2_el_vlist_fn, "" } , sizeof(align_list)/sizeof(*align_list), align_list };

/*===================================================================*/

M2_LABEL(flabel0, "f0", "5x7 abcdefg");
M2_LABEL(flabel1, "f1", "6x12 abcdefg");
M2_LABEL(flabel2, "f2", "7x13 abcdefg");
M2_LIST(flabel_list) = { &flabel0, &flabel1, &flabel2 };
M2_VLIST(flist_element,"",flabel_list);


/*===================================================================*/
/* hlist */

uint8_t radio_val = 0;
m2_el_setval_t r1 = { { m2_el_radio_fn, "v0" }, &radio_val };
m2_el_setval_t r2 = { { m2_el_radio_fn, "v1" }, &radio_val };
m2_el_setval_t r3 = { { m2_el_radio_fn, "v2" }, &radio_val };

void *hlist[] = { &el_goto_top, &r1, &r2, &r3 };
m2_el_list_t el_hlist = { { m2_el_hlist_fn, "" } , sizeof(hlist)/sizeof(*hlist), hlist };

void *hlist2[] = { &r1, &r2, &r3 };
m2_el_list_t el_hlist2 = { { m2_el_hlist_fn, "d1" } , sizeof(hlist2)/sizeof(*hlist2), hlist2 };


/*===================================================================*/
/* labeled list */

uint8_t label_val = 111;
M2_LABEL(label0,"","Number: ");
M2_U8NUM(label_el_val,"c3",110, 125, &label_val);
M2_LABEL(label0hs,"","");
M2_U8HS(label_el_val_hs,"",110, 125, &label_val);

char label_text[4] = "XgZ";
M2_LABEL(label1, "", "++++Text: ");
M2_TEXT(label_el_text,"",label_text,3);

uint32_t label_val32 = 123456789;
M2_LABEL(label2, "", "+++++U32: ");
M2_U32NUM(label_el_val32,"a1c9",&label_val32);

uint8_t label_toggle_val = 0;
M2_LABEL(label3, "", "Toggle: ");
M2_TOGGLE(label_el_toggle_val,"",&label_toggle_val);

M2_LABEL(label4, "", "Radio: ");

void *label_list[] = 
{ &label0, &label_el_val, 
  //&label0hs, &label_el_val_hs, 
  &label1, &label_el_text, 
  &label2, &label_el_val32, 
  &m2_null_element, &label_el_toggle_val, 
  &label4, &el_hlist2, 
  &el_goto_top };
M2_GRIDLIST(el_label_list,"c2d0h50w120",label_list);

/*===================================================================*/
/* xy list */

uint8_t xy_val = 111;
M2_LABEL(xy0,"x0y30","Number: ");
M2_U8NUM(xy_el_val,"c3x10y15",110, 125, &xy_val);

void *xy_list[] = 
{ &xy0, &xy_el_val, 
  &el_goto_top };

M2_XYLIST(el_xy_list,"",xy_list);
  
/*===================================================================*/
/* u32num */

M2_U32NUM(single_u32,"a1c5",&label_val32);

/*===================================================================*/
/* m2_Message */
  
void fn_msg_ok(m2_el_fnarg_p fnarg) 
{
  m2_SetRoot(&el_top);
}

void fn_set_msg(m2_el_fnarg_p fnarg) 
{
  m2_MessageB2Fn("==== Hello World ====", " Ok ", fn_msg_ok, " Cancel ", fn_msg_ok);
}

/*===================================================================*/
/* issue 6 */

M2_LABEL(issue6__main_title,   "f1", "Title");
M2_ROOT(issue6__main_dummy,    "f0", "Dummy",     &el_top);
M2_LIST(issue6__main_list) = { &issue6__main_title, &issue6__main_dummy };
M2_VLIST(issue6__main, NULL, issue6__main_list);


/*===================================================================*/
/* issue 8 */

const char *_setup_beep_strings(uint8_t idx)
{
  if ( idx == 0 )
    return "aaa";
  else if ( idx == 1 )
    return "bbb";
  return "ccc";
}

uint8_t _setup_beep_state = 0;

M2_LABEL(_setup_title, "f1", "Setup");

M2_LABEL(_setup_beep_l, "f0", "Beep");
M2_COMBO(_setup_beep_c, "f0", &_setup_beep_state, 3, _setup_beep_strings);
M2_LIST(_setup_beep_list) = { &_setup_beep_l, &_setup_beep_c };
M2_HLIST(_setup_beep, NULL, _setup_beep_list);

M2_ROOT(_setup_exit, "f0", "exit", &el_top);

M2_LIST(_setup_list) = { &_setup_title, &_setup_beep, &_setup_exit };
M2_VLIST(issue8_setup, NULL, _setup_list);

/*===================================================================*/

const char *el_combo_getstr(uint8_t idx)
{
  if  ( idx == 0 )
    return "aaaa";
  else if ( idx == 1 )
    return "bbbb";
  else if ( idx == 2 )
    return "cccc";
  return "dddd";
}

uint8_t pos = 0;
M2_COMBO(el_combo_top, NULL, &pos, 4, el_combo_getstr);

/*===================================================================*/
/* strlist */

const char *el_strlist_getstr(uint8_t idx, uint8_t msg)
{
  if (msg == M2_STRLIST_MSG_GET_STR)
  {
    if  ( idx == 0 )
      return "aaaa";
    else if ( idx == 1 )
      return "bbbb";
    else if ( idx == 2 )
      return "cccc";
    else if ( idx == 3 )
      return "dddd";
    return "zzzz";
  }
  else
  {
    return "";
  }
}


uint8_t el_strlist_first = 0;
uint8_t el_strlist_cnt = 5;

/*
M2_STRLIST(el_strlist, "l2w100", &el_strlist_first, &el_strlist_cnt, el_strlist_getstr);
M2_VSB(el_strlist_vsb, "l2w10r1", &el_strlist_first, &el_strlist_cnt);

M2_LIST(el_strlist_sblist) = { &el_strlist, &el_strlist_vsb };
M2_HLIST(el_strlist_hlist, NULL, el_strlist_sblist);

M2_U8NUM(el_strlist_el_cnt,"c3",0, 10, &el_strlist_cnt);
M2_LIST(el_strlist_list) = { &el_strlist_hlist, &el_strlist_el_cnt, &el_goto_top };
M2_VLIST(el_strlist_top, NULL, el_strlist_list);
*/

const char *selected = "Nothing";

M2_STRLIST(el_strlist, "l2w90", &el_strlist_first, &el_strlist_cnt, el_strlist_getstr);
M2_VSB(el_strlist_vsb, "l2w5r1", &el_strlist_first, &el_strlist_cnt);
M2_LIST(list_strlist) = { &el_strlist, &el_strlist_vsb };
M2_HLIST(el_strlist_hlist, NULL, list_strlist);

M2_LABEL(el_label,NULL, "Selected:");
M2_LABELPTR(el_labelptr,NULL, &selected);
M2_LIST(list_label) = { &el_label, &el_labelptr };
M2_HLIST(el_label_hlist, NULL, list_label);

M2_LIST(list) = { &el_strlist_hlist, &el_label_hlist };
M2_VLIST(el_vlist, NULL, list);
M2_ALIGN(el_strlist_top, "W64H64-1|1", &el_vlist);


/*===================================================================*/
/* infolist */

uint8_t el_infolist_first = 0;
uint8_t el_infolist_cnt = 5;

M2_INFO(el_infolist, "l2w100", &el_infolist_first, &el_infolist_cnt, "1\n2\n3\n4\n5\n6\n7\n8\n9", NULL);
M2_VSB(el_infolist_vsb, "l2w10r1", &el_infolist_first, &el_infolist_cnt);

M2_LIST(el_infolist_sblist) = { &el_infolist, &el_infolist_vsb };
M2_HLIST(el_infolist_hlist, NULL, el_infolist_sblist);

M2_U8NUM(el_infolist_el_cnt,"c3",0, 10, &el_infolist_cnt);
M2_U8NUM(el_infolist_el_first,"c3",0, 10, &el_infolist_first);
M2_LIST(el_infolist_list) = { &el_infolist_hlist, &el_infolist_el_cnt, &el_infolist_el_first, &el_goto_top };
M2_VLIST(el_infolist_top, NULL, el_infolist_list);

/*===================================================================*/
/* time */

uint32_t time_h, time_m, time_s;
M2_U32NUM(el_h, "c2a1", &time_h);
M2_TEXT(el_l1, "r1", ":", 1);
M2_U32NUM(el_m, "c2a1", &time_m);
M2_TEXT(el_l2, "r1", ":", 1);
M2_U32NUM(el_s, "c2a1", &time_s);
M2_LABEL(el_l3, NULL, " ");
M2_LIST(list_time) = { &el_h, &el_l1, &el_m, &el_l2, &el_s, &el_l3, &el_goto_top };
M2_HLIST(el_timelist_top, NULL, list_time);


/*===================================================================*/
/* v-list */

uint8_t u8_x = 2;
uint32_t u32_x = 112233;

uint8_t u8_cb(m2_rom_void_p element, uint8_t msg, uint8_t val)
{
  if ( msg == M2_U8_MSG_SET_VALUE )
    u8_x = val;
  return u8_x;
}

uint32_t u32_cb(m2_rom_void_p element, uint8_t msg, uint32_t val)
{
  if ( msg == M2_U32_MSG_SET_VALUE )
    u32_x = val;
  return u32_x;
}

const char *label_cb(m2_rom_void_p element)
{
  return m2_utl_u8d(u8_x, 3);
}


M2_LABELFN(el_lfn, NULL, label_cb);
M2_U8NUM(el_u8_ptr, NULL, 0, 10, &u8_x);
M2_U8NUMFN(el_u8_fn, NULL, 0, 10, u8_cb);
M2_U32NUM(el_u32_ptr, "c6", &u32_x);
M2_U32NUMFN(el_u32_fn, "c6", u32_cb);
M2_LIST(list_vlist) = { &el_lfn, &el_u8_ptr, &el_u8_fn, &el_u32_ptr, &el_u32_fn, &el_goto_top };
M2_VLIST(el_vlist_top, NULL, list_vlist);


/*===================================================================*/
/* top menu */

M2_ROOT(el_to_gridlist, base_fmt,"Grid",&el_gridlist);
M2_ROOT(el_to_alignlist, base_fmt,"AlignLst",&el_alignlist);
M2_ROOT(el_to_labellist, base_fmt,"LabelLst",&el_label_list);
M2_ROOT(el_to_hlist, base_fmt, "H-List", &el_hlist);
M2_ROOT(el_to_single_u32, base_fmt, "U32", &single_u32);
M2_ROOT(el_to_xy_list, base_fmt, "XY", &el_xy_list);
M2_ROOT(el_to_issue6, base_fmt, "Issue6", &issue6__main);
M2_ROOT(el_to_issue8, base_fmt, "Issue8", &issue8_setup);
M2_BUTTON(el_set_msg, NULL, "MSG", fn_set_msg);
M2_ROOT(el_to_combo, base_fmt, "combo", &el_combo_top);
M2_ROOT(el_to_strlist, base_fmt, "strlst", &el_strlist_top);
M2_ROOT(el_to_infolist, base_fmt, "info", &el_infolist_top);
M2_ROOT(el_to_timelist, base_fmt, "time", &el_timelist_top);
M2_ROOT(el_to_vlist, base_fmt, "V-List", &el_vlist_top);



void *top_list[] = { &t0, &el_to_hlist, &el_to_gridlist, &el_to_alignlist, &el_to_labellist, &el_to_single_u32, 
  &el_to_xy_list, &el_to_issue6, &el_to_issue8, &el_set_msg, &el_to_combo, &el_to_strlist, &el_to_infolist, &el_to_timelist, &el_to_vlist };

M2_GRIDLIST(_el_top,"c3",top_list);

/*m2_el_list_t _el_top = { { m2_el_vlist_fn, "" } , sizeof(top_list)/sizeof(*top_list), top_list };*/
M2_ALIGN(el_top, "w128h64", &_el_top);

M2_LABEL(hello_world_label, "", "Hello World!");


int main(void)
{
  // m2_ep_init(&ep, 0, &a_eli, m2_es_sdl, m2_eh_2b);
  //m2_ep_init(&ep, 0, &el_top, m2_es_sdl, m2_eh_2b);
  //m2_Init(&hello_world_label, m2_es_sdl, m2_eh_2bs, m2_gh_sdl);
  m2_Init(&el_top, m2_es_sdl, m2_eh_6bs, m2_gh_sdl);
  //m2_Init(&flist_element, m2_es_sdl, m2_eh_4bs, m2_gh_sdl);
  //flist_element
  m2_Draw();
  for(;;)
  {
    m2_CheckKey();
    if ( m2_HandleKey() != 0 )
      m2_Draw();
  }

  return 0;
}

