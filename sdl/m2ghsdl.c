/*

  m2ghsdl.c

  graphics handler for sdl (www.libsdl.org)  
  implements event source handler and graphic handler
  
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
#include "SDL.h"
#include "SDL_video.h"
#include "SDL_ttf.h"

#define HEIGHT 64
#define WIDTH 128
#define CHARHEIGHT 8

int m2_sdl_multiple = 2;
SDL_Surface *m2_sdl_screen;
TTF_Font *m2_sdl_font;
Uint32 m2_sdl_color;

void m2_sdl_set_pixel(int x, int y)
{
  Uint32  *ptr;
  int i, j;
  
  if ( y >= HEIGHT )
    return;
  if ( y < 0 )
    return;
  if ( x >= WIDTH )
    return;
  if ( x < 0 )
    return;
  
  for( i = 0; i < m2_sdl_multiple; i++ )
    for( j = 0; j < m2_sdl_multiple; j++ )
    {
      ptr = m2_sdl_screen->pixels + 
	(y * m2_sdl_multiple + j) * m2_sdl_screen->w * m2_sdl_screen->format->BytesPerPixel + 
	(x * m2_sdl_multiple + i) * m2_sdl_screen->format->BytesPerPixel;
      *ptr = m2_sdl_color;
    }
}

void m2_sdl_hline(int x0, int x1, int y)
{
  int i;
  for( i = x0; i <= x1; i++ )
    m2_sdl_set_pixel(i, y);
}

void m2_sdl_vline(int x, int y0, int y1)
{
  int i;
  for( i = y0; i <= y1; i++ )
    m2_sdl_set_pixel(x, i);
}

void m2_sdl_init(void)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) 
  {
    printf("Unable to initialize SDL:  %s\n", SDL_GetError());
    exit(1);
  }
  
  /* http://www.libsdl.org/cgi/docwiki.cgi/SDL_SetVideoMode */
  m2_sdl_screen = SDL_SetVideoMode(WIDTH*m2_sdl_multiple,HEIGHT*m2_sdl_multiple,24,SDL_SWSURFACE|SDL_ANYFORMAT);
  if ( m2_sdl_screen == NULL ) 
  {
    printf("Couldn't set video mode: %s\n", SDL_GetError());
    exit(1);
  }
  printf("At %d bits-per-pixel mode\n", m2_sdl_screen->format->BitsPerPixel);
  
  TTF_Init();
  
  m2_sdl_font=TTF_OpenFont("FreeMono.ttf", CHARHEIGHT*m2_sdl_multiple);
  if(!m2_sdl_font) 
  {
    printf("TTF_OpenFont: %s\n", TTF_GetError());
    exit(1);
  }

  
  m2_sdl_color = SDL_MapRGB( m2_sdl_screen->format, 100, 255, 0 );

  m2_sdl_set_pixel(0,0);
  m2_sdl_set_pixel(1,1);
  m2_sdl_set_pixel(2,2);

    /* update all */
  /* http://www.libsdl.org/cgi/docwiki.cgi/SDL_UpdateRect */
  SDL_UpdateRect(m2_sdl_screen, 0,0,0,0);


  //atexit(TTF_Quit);
  atexit(SDL_Quit);
  return;
}

/* pos should be removed */
void m2_sdl_text(int x0, int y0, const char *s, int pos, int style)
{
  SDL_Color color={255,255,255};
  SDL_Surface *text_surface;
  /*http://jcatki.no-ip.org:8080/SDL_ttf/SDL_ttf_frame.html*/
  if(!(text_surface=TTF_RenderText_Solid(m2_sdl_font,s,color))) 
  {
      //handle error here, perhaps print TTF_GetError at least
  } 
  else 
  {
    SDL_Rect dstrect;
    int len;    
    int w  = 1, h = 1;
    int xs = 0, xe;
    len = strlen(s);
    xs = 0;
    TTF_SizeText(m2_sdl_font, s, &w, &h);
    xe = w;
    
    dstrect.x = x0*m2_sdl_multiple;
    dstrect.y = y0*m2_sdl_multiple;
    dstrect.w = 0;
    dstrect.h = 0;
    /* http://www.libsdl.org/cgi/docwiki.cgi/SDL_BlitSurface */
    SDL_BlitSurface(text_surface,NULL,m2_sdl_screen,&dstrect);
    SDL_UpdateRect(m2_sdl_screen, 0,0,0,0);

    
    /* http://www.libsdl.org/cgi/docwiki.cgi/SDL_UpdateRect */
    SDL_UpdateRect(m2_sdl_screen, 0,0,0,0);
    SDL_FreeSurface(text_surface);

    if ( style != 0 )
      m2_sdl_hline((x0+xs/m2_sdl_multiple), (x0+xe/m2_sdl_multiple), (y0-1));
    if ( style ==2 )
      m2_sdl_hline((x0+xs/m2_sdl_multiple), (x0+xe/m2_sdl_multiple), (y0-2));
    
  }
}

uint8_t m2_sdl_get_text_width(const char *s)
{
  int w  = 1, h = 1;
  TTF_SizeText(m2_sdl_font, s, &w, &h);
  if ( w > 255 )
    w = 255;
  return w;  
}

/* event source for SDL */
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

/*
int main(void)
{
  SDL_Event event;
  m2_sdl_init();
  m2_sdl_text(0, 10, "Hello World");
  for(;;)
    while(SDL_PollEvent(&event)) 
    {      
	switch (event.type) 
	{
	    case SDL_QUIT:
		exit(0);
		break;
	}
    }
}
*/

/*==============================================================*/


void m2_sdlgfx_hline(uint8_t x0, uint8_t y0, uint8_t w)
{
  /* w is the total width */
  w--;
  m2_sdl_hline(x0, x0+w, HEIGHT-1-y0);  
}

void m2_sdlgfx_vline(uint8_t x0, uint8_t y0, uint8_t h)
{
  h--;
  m2_sdl_vline(x0, HEIGHT-1-(y0+h), HEIGHT-1-y0);
}

void m2_sdlgfx_box(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h)
{
  w--;
  h--;
  m2_sdl_hline(x0, x0+w, HEIGHT-1-y0);
  m2_sdl_hline(x0, x0+w, HEIGHT-1-(y0+h));
  m2_sdl_vline(x0, HEIGHT-1-(y0+h), HEIGHT-1-y0);
  m2_sdl_vline(x0+w, HEIGHT-1-(y0+h), HEIGHT-1-y0);
    SDL_UpdateRect(m2_sdl_screen, 0,0,0,0);
}

void m2_sdlgfx_normal_focus(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h)
{
  w--;
  h--;
  m2_sdl_hline(x0, x0+w, HEIGHT-1-y0);
  m2_sdl_hline(x0, x0+w, HEIGHT-1-(y0+h));
  m2_sdl_vline(x0, HEIGHT-1-(y0+h), HEIGHT-1-y0);
  m2_sdl_vline(x0+w, HEIGHT-1-(y0+h), HEIGHT-1-y0);
    SDL_UpdateRect(m2_sdl_screen, 0,0,0,0);
}

void m2_sdlgfx_small_focus(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h)
{
  w--;
  h--;
  m2_sdl_hline(x0, x0+w, HEIGHT-1-y0);
  m2_sdl_hline(x0, x0+w, HEIGHT-1-(y0+h));
  /*
  m2_sdl_vline(x0, HEIGHT-1-(y0+h), HEIGHT-1-y0);
  m2_sdl_vline(x0+w, HEIGHT-1-(y0+h), HEIGHT-1-y0);
  */
    SDL_UpdateRect(m2_sdl_screen, 0,0,0,0);
}

void m2_sdlgfx_data_entry(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h)
{
  w--;
  h--;
  m2_sdl_hline(x0, x0+w, HEIGHT-1-y0);
    SDL_UpdateRect(m2_sdl_screen, 0,0,0,0);
}

void m2_sdlgfx_go_up(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h)
{
  m2_sdl_vline(x0, HEIGHT-1-(y0+h), HEIGHT-1-y0);
  m2_sdl_vline(x0+w, HEIGHT-1-(y0+h), HEIGHT-1-y0);
    SDL_UpdateRect(m2_sdl_screen, 0,0,0,0);
}

uint8_t m2_sdlgfx_get_text_width(const char *s, uint8_t font)
{
  uint8_t w;
  w = m2_sdl_get_text_width(s)/m2_sdl_multiple;
  if ( font >= 10 )
    w += 4;
  return w;
}


/*
  pos: 0...strlen(s)-1 or 255
  style: 
    0 no style
    1 selected
    2 data entry
*/
void m2_sdlgfx_text(uint8_t x0, uint8_t y0, uint8_t font, const char *s)
{
  uint8_t w,h;
  if ( font >= 10 )
  {
    w = m2_sdlgfx_get_text_width(s, font);
    w--;
    h = CHARHEIGHT+1;
    m2_sdl_hline(x0, x0+w, HEIGHT-1-y0);
    m2_sdl_hline(x0, x0+w, HEIGHT-1-(y0+h));
    m2_sdl_vline(x0, HEIGHT-1-(y0+h), HEIGHT-1-y0);
    m2_sdl_vline(x0+w, HEIGHT-1-(y0+h), HEIGHT-1-y0);
    m2_sdl_text(x0+2, HEIGHT-y0-CHARHEIGHT-4+2, s, 0,0);  
    
  }
  else
  {  
    m2_sdl_text(x0, HEIGHT-y0-CHARHEIGHT, s, 0,0);  
  }
    SDL_UpdateRect(m2_sdl_screen, 0,0,0,0);
}

uint8_t m2_sdlgfx_get_char_width(void)
{
  return m2_sdl_get_text_width("m")/m2_sdl_multiple;
}

uint8_t m2_sdlgfx_get_char_height(uint8_t font)
{
  if ( font >= 10 )
    return CHARHEIGHT+2;
  return CHARHEIGHT;
}

void m2_sdlgfx_start(void)
{
  Uint32 color = SDL_MapRGB( m2_sdl_screen->format, 0, 0, 0 );
  /* http://www.libsdl.org/cgi/docwiki.cgi/SDL_FillRect */
  SDL_FillRect(m2_sdl_screen, NULL, color);
  m2_sdl_set_pixel(0,0);
  m2_sdl_set_pixel(10,0);
  m2_sdl_set_pixel(20,0);
  m2_sdl_set_pixel(30,0);
  m2_sdl_set_pixel(40,0);
  m2_sdl_set_pixel(50,0);
}

void m2_sdlgfx_end(void)
{
}

uint8_t m2_sdlgfx_get_icon_height(uint8_t font, uint8_t icon_number)
{
  return m2_sdlgfx_get_char_height(font);
}

uint8_t m2_sdlgfx_get_icon_width(uint8_t font, uint8_t icon_number)
{
  return m2_sdlgfx_get_char_width()*2;
}

/*
  draw icon, similar to text draw procedure. (x0,y0) is lower left corner of the icon
*/
void m2_sdlgfx_draw_icon(uint8_t x0, uint8_t y0, uint8_t font, uint8_t icon_number)
{
  uint8_t w = m2_sdlgfx_get_icon_width(font, icon_number);
  uint8_t h = m2_sdlgfx_get_icon_height(font, icon_number);
  
  w-=2;
  h-=2;
  x0++;
  y0++;
  
  m2_sdlgfx_box(x0, y0, w, h);
  if ( icon_number == M2_ICON_TOGGLE_ACTIVE || icon_number == M2_ICON_RADIO_ACTIVE )
  {
    x0+=1;
    y0+=1;
    w -= 2;
    h -= 2;
    m2_sdlgfx_box(x0, y0, w, h);
  }
}


/*==============================================================*/
/* sdl graphics handler */

uint8_t m2_gh_sdl(m2_gfx_arg_p  arg)
{
  switch(arg->msg)
  {
    case M2_GFX_MSG_INIT:				
      m2_sdl_init();	
      break;
    case M2_GFX_MSG_START:
      m2_sdlgfx_start();
      break;
    case M2_GFX_MSG_END:
      m2_sdlgfx_end();
      break;
    case M2_GFX_MSG_DRAW_HLINE:
      m2_sdlgfx_hline(arg->x, arg->y, arg->w);
      break;
    case M2_GFX_MSG_DRAW_VLINE:
      m2_sdlgfx_vline(arg->x, arg->y, arg->h);
      break;
    case M2_GFX_MSG_DRAW_BOX:
      m2_sdlgfx_box(arg->x, arg->y, arg->w, arg->h);
      break;
    case M2_GFX_MSG_DRAW_TEXT:
      {
      	uint8_t x = arg->x;
	if ( (arg->font & 8) != 0 )
	  if ( arg->w != 0 )
	  {
	    x = arg->w;
	    x -= m2_sdlgfx_get_text_width(arg->s, arg->font);
	    x >>= 1;
	    x += arg->x;
	  }
	m2_sdlgfx_text(x, arg->y, arg->font, arg->s);
      }
      break;
    case M2_GFX_MSG_DRAW_NORMAL_FOCUS:
      m2_sdlgfx_normal_focus(arg->x, arg->y, arg->w, arg->h);
      break;
    case M2_GFX_MSG_DRAW_SMALL_FOCUS:
      m2_sdlgfx_small_focus(arg->x, arg->y, arg->w, arg->h);
      break;
    case M2_GFX_MSG_DRAW_NORMAL_DATA_ENTRY:
      m2_sdlgfx_data_entry(arg->x, arg->y, arg->w, arg->h);
      break;
    case M2_GFX_MSG_DRAW_GO_UP:
      m2_sdlgfx_go_up(arg->x, arg->y, arg->w, arg->h);
      break;
    case M2_GFX_MSG_DRAW_ICON:
      m2_sdlgfx_draw_icon(arg->x, arg->y, arg->font, arg->icon);
      break;    
    case M2_GFX_MSG_GET_TEXT_WIDTH:
    case M2_GFX_MSG_GET_TEXT_WIDTH_P:
      return m2_sdlgfx_get_text_width(arg->s, arg->font);
    case M2_GFX_MSG_GET_CHAR_WIDTH:
      return m2_sdlgfx_get_char_width();
    case M2_GFX_MSG_GET_CHAR_HEIGHT:
      return m2_sdlgfx_get_char_height(arg->font);
    case M2_GFX_MSG_GET_NORMAL_BORDER_HEIGHT:
      return 2;
    case M2_GFX_MSG_GET_NORMAL_BORDER_WIDTH:
      return 2;
    case M2_GFX_MSG_GET_NORMAL_BORDER_X_OFFSET:
      return 1;
    case M2_GFX_MSG_GET_NORMAL_BORDER_Y_OFFSET:
      return 1;
    case M2_GFX_MSG_GET_SMALL_BORDER_HEIGHT:
      return 2;
    case M2_GFX_MSG_GET_SMALL_BORDER_WIDTH:
      return 0;
    case M2_GFX_MSG_GET_SMALL_BORDER_X_OFFSET:
      return 0;
    case M2_GFX_MSG_GET_SMALL_BORDER_Y_OFFSET:
      return 1;
    case M2_GFX_MSG_GET_READONLY_BORDER_HEIGHT:
      return 0;
    case M2_GFX_MSG_GET_READONLY_BORDER_WIDTH:
      return 0;
    case M2_GFX_MSG_GET_READONLY_BORDER_X_OFFSET:
      return 0;
    case M2_GFX_MSG_GET_READONLY_BORDER_Y_OFFSET:
      return 0;
    case M2_GFX_MSG_GET_LIST_OVERLAP_HEIGHT:
      return 1;
    case M2_GFX_MSG_GET_LIST_OVERLAP_WIDTH:
      return 1;
    case M2_GFX_MSG_GET_ICON_WIDTH:
      return m2_sdlgfx_get_icon_width(arg->font, arg->icon);
    case M2_GFX_MSG_GET_ICON_HEIGHT:
      return m2_sdlgfx_get_icon_height(arg->font, arg->icon);
    case M2_GFX_MSG_DRAW_VERTICAL_SCROLL_BAR:
      /* scroll bar: "total" total number of items */
      /* scroll bar: "top" topmost item (first visible item) 0 .. total-visible*/
      /* scroll bar: "visible" number of visible items 0 .. total-1 */

      m2_sdlgfx_box(arg->x, arg->y, arg->w, arg->h);
      {
	uint16_t h, y;
	
	h = m2_utl_sb_get_slider_height(arg->h-2, arg->total, arg->visible);
	y = m2_utl_sb_get_slider_position(arg->h-2, h, arg->total, arg->visible, arg->top); 

	
	m2_sdlgfx_box(arg->x+1, arg->y+arg->h-1-h-y, arg->w-2, h);
      }
      return 1;
    case M2_GFX_MSG_GET_DISPLAY_WIDTH:
      return WIDTH;
    case M2_GFX_MSG_GET_DISPLAY_HEIGHT:
      return HEIGHT;
  }
  return m2_gh_dummy(arg);
}

