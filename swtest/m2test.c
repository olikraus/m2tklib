/*

  m2test.c
  
  test framework for m2tklib

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

#include <stdio.h>
#include <string.h>
#include "m2.h"

#define KEY_EVENT_MAX 10

uint8_t key_events[KEY_EVENT_MAX];
uint8_t key_pos;

m2_t ep;

uint16_t total_tests = 0;
uint16_t failed_tests = 0;

/* event source of the test framework */
uint8_t m2_es_test(m2_p ep, uint8_t msg)
{
  uint8_t key;
  switch(msg)
  {
    case M2_ES_MSG_GET_KEY:
      key = key_events[key_pos];
      if ( key != M2_KEY_NONE) 
      {
	key_pos++;
	return M2_KEY_EVENT(key);
      }
      return key;
    case M2_ES_MSG_INIT:
      key_pos = 0;
  }
  return 0;      
}

void test_set_1_key(uint8_t k1)
{
  key_events[0] = k1;
  key_events[1] = M2_KEY_NONE;
  key_pos = 0;
}

void test_set_2_key(uint8_t k1, uint8_t k2)
{
  key_events[0] = k1;
  key_events[1] = k2;
  key_events[2] = M2_KEY_NONE;
  key_pos = 0;
}

void test_set_3_key(uint8_t k1, uint8_t k2, uint8_t k3)
{
  key_events[0] = k1;
  key_events[1] = k2;
  key_events[2] = k3;
  key_events[3] = M2_KEY_NONE;
  key_pos = 0;
}

void test_exec_steps(void)
{
   m2_DrawM2(&ep);
  m2_CheckKeyM2(&ep);
  while(m2_HandleKeyM2(&ep) != 0)
  {
    m2_DrawM2(&ep); 
    m2_CheckKeyM2(&ep);
  }
}

void test_exec_1_key(uint8_t k1)
{
  test_set_1_key(k1);
  test_exec_steps();
}

void test_exec_2_key(uint8_t k1, uint8_t k2)
{
  test_set_2_key(k1, k2);
  test_exec_steps();
}

void test_exec_3_key(uint8_t k1, uint8_t k2, uint8_t k3)
{
  test_set_3_key(k1, k2, k3);
  test_exec_steps();
}

#ifdef OBSOLETE_CODE
/* graphics interface */
void m2_gfx_init(void) {}
void m2_gfx_start(void) {}
void m2_gfx_hline(uint8_t x0, uint8_t y0, uint8_t w) {}
void m2_gfx_vline(uint8_t x0, uint8_t y0, uint8_t h) {}
void m2_gfx_box(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h) {}
void m2_gfx_text(uint8_t x0, uint8_t y0, const char *s) {}
void m2_gfx_focus(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h) {}
void m2_gfx_normal_data_entry(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h) {}
void m2_gfx_go_up(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h) {}
uint8_t m2_gfx_get_text_width(const char *s) { return strlen(s); }
uint8_t m2_gfx_get_char_width(void) { return 1; }
uint8_t m2_gfx_get_char_height(void) { return 1; }

uint8_t m2_gfx_add_normal_border_height(uint8_t height) { return height; }
uint8_t m2_gfx_add_normal_border_width(uint8_t width) { return width; }
uint8_t m2_gfx_add_normal_border_x(uint8_t x) { return x; }
uint8_t m2_gfx_add_normal_border_y(uint8_t y) { return y; }

uint8_t m2_gfx_add_small_border_height(uint8_t height) { return height; }
uint8_t m2_gfx_add_small_border_width(uint8_t width) { return width; }
uint8_t m2_gfx_add_small_border_x(uint8_t x) { return x; }
uint8_t m2_gfx_add_small_border_y(uint8_t y) { return y; }

uint8_t m2_gfx_add_readonly_border_height(uint8_t height) { return height; }
uint8_t m2_gfx_add_readonly_border_width(uint8_t width) { return width; }
uint8_t m2_gfx_add_readonly_border_x(uint8_t x) { return x; }
uint8_t m2_gfx_add_readonly_border_y(uint8_t y) { return y; }

uint8_t m2_gfx_get_list_overlap_height(void) { return 0; }
uint8_t m2_gfx_get_list_overlap_width(void) { return 0; }

void m2_gfx_draw_icon(uint8_t x0, uint8_t y0, uint8_t font, uint8_t icon_number) {}
uint8_t m2_gfx_get_icon_height(uint8_t font, uint8_t icon_number) { return 1; }
uint8_t m2_gfx_get_icon_width(uint8_t font, uint8_t icon_number) { return 1; }
void m2_gfx_end(void) {}
#endif

int test_assert(const char *s, int condition_result)
{
  total_tests++;
  if ( condition_result != 0 )
  {
    printf("passed: %s\n", s);
  }
  else
  {
    printf("failed: %s\n", s);
    failed_tests++;
  }
  return condition_result;
}
  
void test001(void)
{
  uint32_t val32 = 12345UL;
  M2_U32NUM(u32,"a1c5",&val32);
  m2_InitM2(&ep, &u32, m2_es_test, m2_eh_2bs, m2_gh_dummy);
  test_exec_1_key(M2_KEY_SELECT);
  test_assert("001 (u32 increment, s)", val32 == 22345UL );
}
  
void test002(void)
{
  uint32_t val32 = 12345UL;
  M2_U32NUM(u32,"a1c5",&val32);
  m2_InitM2(&ep, &u32, m2_es_test, m2_eh_2bs, m2_gh_dummy);
  test_exec_2_key(M2_KEY_NEXT, M2_KEY_SELECT);
  test_assert("002 (u32 increment, sn)", val32 == 13345UL );
}

void test003(void)
{
  uint32_t val32 = 7UL;
  M2_U32NUM(u32,"a1c1",&val32);
  m2_InitM2(&ep, &u32, m2_es_test, m2_eh_2bs, m2_gh_dummy);
  test_exec_2_key(M2_KEY_NEXT, M2_KEY_SELECT);
  test_assert("003 (u32 increment, ns, c1)", val32 == 8UL );
}

void test004(void)
{
  uint8_t val8 = 78;
  M2_LABEL(label,"","Number: ");
  M2_U8NUM(u8,"c3",0, 100, &val8);
  M2_LIST(list) = { &label, &u8 };
  M2_VLIST(top,"",list);
  m2_InitM2(&ep, &top, m2_es_test, m2_eh_2bs, m2_gh_dummy);
  test_exec_1_key(M2_KEY_SELECT);
  test_assert("004 (skip first label)", val8 == 79 );
}
  
void test005(void)
{
  uint8_t val8 = 78;
  M2_LABEL(label,"","Number: ");
  M2_U8NUM(u8,"c3",0, 100, &val8);
  M2_LIST(list) = { &label, &u8 };
  M2_VLIST(top,"",list);
  m2_InitM2(&ep, &top, m2_es_test, m2_eh_2bs, m2_gh_dummy);
  test_exec_1_key(M2_KEY_SELECT);
  test_assert("005 (label as second child)", val8 == 79 );
}

void test006(void)
{
  uint32_t val32 = 1111UL;
  M2_LABEL(label,"","Number: ");
  M2_U32NUM(u32,"c4a0",&val32);
  M2_LIST(list) = { &label, &u32 };
  M2_VLIST(top,"",list);
  m2_InitM2(&ep, &top, m2_es_test, m2_eh_2bs, m2_gh_dummy);
  test_exec_1_key(M2_KEY_SELECT);
  test_assert("006 (u32 a0 mode, test 1/3)", val32 == 1111UL );
}

void test007(void)
{
  uint32_t val32 = 1111UL;
  M2_LABEL(label,"","Number: ");
  M2_U32NUM(u32,"c4a0",&val32);
  M2_LIST(list) = { &label, &u32 };
  M2_VLIST(top,"",list);
  m2_InitM2(&ep, &top, m2_es_test, m2_eh_2bs, m2_gh_dummy);
  test_exec_2_key(M2_KEY_SELECT, M2_KEY_SELECT);
  test_assert("007 (u32 a0 mode, test 2/3)", val32 == 2111UL );
}

void test008(void)
{
  uint32_t val32 = 771111UL;
  M2_LABEL(label,"","Number: ");
  M2_U32NUM(u32,"c4a0",&val32);
  M2_LIST(list) = { &label, &u32 };
  M2_VLIST(top,"",list);
  m2_InitM2(&ep, &top, m2_es_test, m2_eh_2bs, m2_gh_dummy);
  test_exec_3_key(M2_KEY_SELECT, M2_KEY_NEXT, M2_KEY_SELECT);
  test_assert("008 (u32 a0 mode, test 3/3)", val32 == 771211UL );
}

void test009(void)
{
  uint32_t val32 = 771111UL;
  M2_LABEL(label,"","Number: ");
  M2_U32NUM(u32,"c1a0",&val32);
  M2_LIST(list) = { &label, &u32 };
  M2_VLIST(top,"",list);

  m2_InitM2(&ep, &top, m2_es_test, m2_eh_2bs, m2_gh_dummy);
  test_exec_3_key(M2_KEY_SELECT, M2_KEY_NEXT, M2_KEY_SELECT);
  test_assert("009 (u32 a0 mode, no number changed)", val32 == 771111UL );
}

void test010(void)
{
  uint32_t val32 = 771111UL;
  M2_LABEL(label,"","Number: ");
  M2_U32NUM(u32,"c4a1",&val32);
  M2_LIST(list) = { &label, &u32 };
  M2_VLIST(top,"",list);

  m2_InitM2(&ep, &top, m2_es_test, m2_eh_2bs, m2_gh_dummy);
  test_exec_3_key(M2_KEY_SELECT, M2_KEY_NEXT, M2_KEY_SELECT);
  test_assert("010 (u32 a1 mode)", val32 == 772211UL );
}

void test011(void)
{
  uint32_t val32 = 22UL;
  M2_LABEL(label,"","Number: ");
  M2_U32NUM(u32,"c2.1a1",&val32);
  M2_LIST(list) = { &label, &u32 };
  M2_VLIST(top,"",list);

  m2_InitM2(&ep, &top, m2_es_test, m2_eh_2bs, m2_gh_dummy);
  test_exec_3_key(M2_KEY_SELECT, M2_KEY_NEXT, M2_KEY_SELECT);
  test_assert("010 (u32 a1 mode, dot)", val32 == 33UL );
}

void test012(void)
{
  uint32_t val32 = 22UL;
  M2_LABEL(label,"","Number: ");
  M2_U32NUM(u32,"c2.2a1",&val32);
  M2_LIST(list) = { &label, &u32 };
  M2_VLIST(top,"",list);

  m2_InitM2(&ep, &top, m2_es_test, m2_eh_2bs, m2_gh_dummy);
  test_exec_3_key(M2_KEY_SELECT, M2_KEY_NEXT, M2_KEY_SELECT);
  test_assert("010 (u32 a1 mode, dot)", val32 == 33UL );
}

int main(void)
{
  test001();
  test002();
  test003();
  test004();
  test005();
  test006();
  test007();
  test008();
  test009();
  test010();
  test011();
  test012();
  
  printf("%d/%d failed\n", failed_tests, total_tests);
}

