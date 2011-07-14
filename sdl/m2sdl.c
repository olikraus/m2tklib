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
/* top menu */

M2_ROOT(el_to_gridlist, base_fmt,"Grid",&el_gridlist);
M2_ROOT(el_to_alignlist, base_fmt,"AlignList",&el_alignlist);
M2_ROOT(el_to_labellist, base_fmt,"LabelList",&el_label_list);
M2_ROOT(el_to_hlist, base_fmt, "H-List", &el_hlist);
M2_ROOT(el_to_single_u32, base_fmt, "U32", &single_u32);
M2_ROOT(el_to_xy_list, base_fmt, "XY", &el_xy_list);
M2_ROOT(el_to_issue6, base_fmt, "Issue6", &issue6__main);
M2_ROOT(el_to_issue8, base_fmt, "Issue8", &issue8_setup);
M2_BUTTON(el_set_msg, NULL, "MSG", fn_set_msg);



void *top_list[] = { &t0, &el_to_hlist, &el_to_gridlist, &el_to_alignlist, &el_to_labellist, &el_to_single_u32, &el_to_xy_list, &el_to_issue6, &el_to_issue8, &el_set_msg };

M2_GRIDLIST(_el_top,"c2",top_list);

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

