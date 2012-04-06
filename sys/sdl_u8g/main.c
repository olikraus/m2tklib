

#include "SDL.h"

#include "m2ghu8g.h"

#include <stdlib.h>
#include <stdio.h>


int u8g_sdl_get_key(void);
void screenshot(void);

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
                            screenshot();
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
M2_BUTTON(el_ok, "f4", " ok ", fn_ok);

M2_LIST(list_combo) = { 
    &el_label1, &el_combo1, 
    &el_label2, &el_combo2,  
    &el_cancel, &el_ok 
};
M2_GRIDLIST(el_top_combo, "c2", list_combo);

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
/* top level sdl menu: tlsm */


const char *el_tlsm_strlist_cb(uint8_t idx, uint8_t msg) {
  const char *s = "";

  if ( idx == 0 ) {
    s = "Select Style";
    if ( msg == M2_STRLIST_MSG_SELECT )
      m2_SetRoot(&top_el_gfx);
  }
  else if ( idx == 1 ) {
    s = "Select Font";
    if ( msg == M2_STRLIST_MSG_SELECT )
      m2_SetRoot(&top_el_fsel);
  }  
  else if ( idx == 2 ) {
    s = "Buttons";
    if ( msg == M2_STRLIST_MSG_SELECT )
      m2_SetRoot(&el_top_btn);
  }
  else if ( idx == 3 ) {
    s = "Text Entry";
    if ( msg == M2_STRLIST_MSG_SELECT )
      m2_SetRoot(&el_top_te);
  }
  else if ( idx == 4 ) {
    s = "Radio";
    if ( msg == M2_STRLIST_MSG_SELECT )
      m2_SetRoot(&el_top_rb);
  }
  else if ( idx == 5 ) {
    s = "Combo";
    if ( msg == M2_STRLIST_MSG_SELECT )
      m2_SetRoot(&el_top_combo);
  }
  else if ( idx == 6 ) {
    s = "Number Entry";
    if ( msg == M2_STRLIST_MSG_SELECT )
      m2_SetRoot(&el_num_menu);
  }
  
  
  return s;
}

uint8_t el_tlsm_first = 0;
uint8_t el_tlsm_cnt = 7;

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
    system(cmd);
  }
  
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

  m2_SetU8gToggleFontIcon(u8g_font_7x13_75r, active_encoding, inactive_encoding);
  m2_SetU8gRadioFontIcon(u8g_font_7x13_75r, active_encoding, inactive_encoding);


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

