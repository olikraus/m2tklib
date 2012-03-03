

#include "SDL.h"

#include "m2ghu8g.h"

#include <stdlib.h>
#include <stdio.h>


int u8g_sdl_get_key(void);

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


uint8_t select_color = 0;
uint8_t select_priority = 0;

void fn_ok(m2_el_fnarg_p fnarg) {
  /* accept selection */
}

void fn_cancel(m2_el_fnarg_p fnarg) {
  /* discard selection */
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

M2_BUTTON(el_cancel, NULL, "cancel", fn_cancel);
M2_BUTTON(el_ok, NULL, " ok ", fn_ok);

M2_LIST(list) = { 
    &el_label1, &el_combo1, 
    &el_label2, &el_combo2,  
    &el_cancel, &el_ok 
};
M2_GRIDLIST(list_element, "c2",list);


int main(void)
{  
  u8g_t u8g;

  /* this is the setup sequence, steps 1..4 should be in this order */
  
  /* 1. Setup and create device access */
  u8g_Init(&u8g, &u8g_dev_sdl_1bit);

  /* 2. Connect the u8g display to m2. Note: M2 is setup later */
  m2_SetU8g(&u8g);

  /* 3. Now, setup m2 */ 
  m2_Init(&list_element, m2_es_sdl, m2_eh_6bs, m2_gh_u8g_fb);

  /* 4. And finally, set at least one font, use normal u8g_font's */
  m2_SetFont(0, (const void *)u8g_font_6x13);

  /*
  u8g_FirstPage(&u8g);
  
  do
  {
    u8g_SetFont(&u8g, u8g_font_unifont);
    //u8g_SetFont(&u8g, u8g_font_osb18r);
    
    u8g_DrawStr(&u8g, 0, 20, "Hello World!");
  } while( u8g_NextPage(&u8g) );
*/

  for(;;)
  {
    m2_CheckKey();
    if ( m2_HandleKey() ) {
      u8g_FirstPage(&u8g);
      do{
        u8g_SetFont(&u8g, u8g_font_unifont);
        u8g_DrawStr(&u8g, 0, 20, "Hello World!");
        m2_CheckKey();
        m2_Draw();
      } while( u8g_NextPage(&u8g) );
    }
  }
  return 0;
}

