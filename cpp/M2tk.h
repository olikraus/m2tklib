/*

  M2tk.h

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

class M2tk
{
  public:
    M2tk(m2_rom_void_p element, m2_es_fnptr es, m2_eh_fnptr eh, m2_gfx_fnptr gh) { m2_Init(element, es, eh, gh); }
    void checkKey(void) { m2_CheckKey(); }
    uint8_t handleKey(void) { return m2_HandleKey(); }
    void draw(void) { m2_Draw(); }
    void setKey(uint8_t key) { m2_SetKey(key); }
    uint8_t getKey(void) { return m2_GetKey(); }
    void setFont(uint8_t font_idx, const void *font_ptr) { m2_SetFont(font_idx, font_ptr); }
    void setPin(uint8_t key, uint8_t pin) { m2_SetPin(key, pin); }
    void setRoot(m2_rom_void_p element, uint8_t next_cnt = 0, uint8_t cv = 0) { m2_SetRootExtended(element, next_cnt, cv); }
    void setHome(m2_rom_void_p element) { m2_SetHome(element); }
    void setRootChangeCallback(m2_root_change_fnptr cb) { m2_SetRootChangeCallback(cb); }

    void clear(void) { m2_Clear(); }
    m2_rom_void_p getRoot() { return m2_GetRoot(); }    
};

