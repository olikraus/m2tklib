/*

  m2cbslmenu.c

  2-level menu callback procedure for M2_STRLIST

  m2tklib = Mini Interative Interface Toolkit Library
  
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

/*========================================================================*/

#include "m2.h"

struct _m2_menu_entry
{
  const char *label;
  m2_rom_void_p element;
};
typedef struct _m2_menu_entry m2_menu_entry;

/*========================================================================*/
uint8_t m2_strlist_menu_first = 0;
uint8_t m2_strlist_menu_cnt = 2;
uint8_t m2_strlist_menu_expanded = 255;
m2_menu_entry *m2_strlist_menu_data;

/*========================================================================*/


static uint8_t m2_strlist_menu_is_submenu(uint8_t defidx)
{
  if ( m2_strlist_menu_data[defidx].label == NULL )
    return 0;
  if ( m2_strlist_menu_data[defidx].label[0] != '.' )
    return 0;
  return 1;
}

static uint8_t m2_strlist_menu_has_submenu(uint8_t defidx)
{
  if ( m2_strlist_menu_data[defidx].label == NULL )
    return 0;
  if ( m2_strlist_menu_data[defidx].label[0] == '.' )
    return 0;
  return m2_strlist_menu_is_submenu(defidx+1);
}

static uint8_t m2_strlist_menu_get_submenu_cnt(uint8_t defidx)
{
  uint8_t cnt = 0;
  for(;;)
  {
    defidx++;
    if ( m2_strlist_menu_is_submenu(defidx) == 0 )
      break;
    cnt++;
  }
  return cnt;
}

static uint8_t m2_strlist_menu_get_defidx_by_strlistidx(uint8_t strlistidx)
{
  uint8_t strlistcnt = 0;
  uint8_t defidx = 0;
  for(;;)
  {
    if ( strlistidx == strlistcnt )
      break;
    if ( m2_strlist_menu_data[defidx].label == NULL )
      break;
    if ( m2_strlist_menu_has_submenu(defidx) != 0 )
    {
      if ( defidx == m2_strlist_menu_expanded )
      {
	defidx++;
	strlistcnt++;
	while(m2_strlist_menu_is_submenu(defidx))
	{
	  if ( strlistidx == strlistcnt )
	    break;
	  defidx++;
	  strlistcnt++;
	}
      }
      else
      {
	defidx++;
	strlistcnt++;
	while(m2_strlist_menu_is_submenu(defidx))
	{
	  defidx++;
	}
      }
    }
    else
    {
      defidx++;
      strlistcnt++;
    }
  }
  if ( strlistidx == 255 )
    return strlistcnt;
  return defidx;
}

/* calculate "cnt", based on "expanded" */
static void m2_strlist_menu_update_cnt(void)
{
  m2_strlist_menu_cnt = m2_strlist_menu_get_defidx_by_strlistidx(255);
}

/*========================================================================*/

const char *m2_strlist_menu_cb(uint8_t idx, uint8_t msg) {
  uint8_t defidx;
  
  defidx = m2_strlist_menu_get_defidx_by_strlistidx(idx);
  
  if ( msg == M2_STRLIST_MSG_GET_EXTENDED_STR )
  {
      if ( m2_strlist_menu_expanded == defidx )
      {
	return "-";
      }
      if ( m2_strlist_menu_has_submenu(defidx) != 0 )
      {
	return "+";
      }
      return "";
  }
  else if ( msg == M2_STRLIST_MSG_SELECT )
  {
    if ( m2_strlist_menu_has_submenu(defidx) != 0 )
    {
      if ( m2_strlist_menu_expanded == defidx )
      {
	m2_strlist_menu_expanded = 255;
	m2_strlist_menu_update_cnt();
      }
      else
      {
	if ( m2_strlist_menu_expanded < defidx )
	{
	  uint8_t cnt = m2_strlist_menu_get_submenu_cnt(m2_strlist_menu_expanded);
	  m2_strlist_menu_expanded = defidx;
	  m2_strlist_menu_update_cnt();
	  while( cnt > 0 )
	  {
	    cnt--;
	    m2_HandleKey();				/* first, handle the existing keys */
	    m2_SetKey(M2_KEY_PREV);		/* put new key into queue, leave last added key in the queue */
	  }
	}
	else
	{
	  m2_strlist_menu_expanded = defidx;
	  m2_strlist_menu_update_cnt();
	}
      }
    }
    else
    {
      m2_SetRoot(m2_strlist_menu_data[defidx].element);
    }
  }

  if ( m2_strlist_menu_data[defidx].label == NULL )
    return "---";
  if ( m2_strlist_menu_is_submenu(defidx) != 0 )
    return m2_strlist_menu_data[defidx].label+1;
  return m2_strlist_menu_data[defidx].label;
}

/*========================================================================*/

void m2_SetStrlistMenuData(m2_menu_entry *menu_data)
{
  m2_strlist_menu_data = menu_data;
  m2_strlist_menu_update_cnt();
}

