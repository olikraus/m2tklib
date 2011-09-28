#
# Makefile
#	for m2lab
#
#  m2tklib = Mini Interative Interface Toolkit Library
#  
#  Copyright (C) 2011  olikraus@gmail.com
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

M2PATH = ../src

COMMON_INCLUDES = -I. `pkg-config --cflags gtk+-2.0` -I$(M2PATH)

COMMON_FLAGS = -g -Wall 
COMMON_FLAGS += $(COMMON_INCLUDES)

CFLAGS = $(COMMON_FLAGS) -std=gnu99
LDLIBS = `pkg-config --cflags --libs gtk+-2.0`

M2SRC = $(shell ls $(M2PATH)/*.c)


EXTRASRC = lab_main.c  lab_el_def.c lab_el_edit.c

SRC = $(M2SRC) $(EXTRASRC)

OBJ := $(SRC:.c=.o)

m2lab: $(OBJ)
	$(LINK.o) $(OBJ) $(COMMON_FLAGS) $(LOADLIBES) $(LDLIBS) -o $@
	
clean:
	-rm $(OBJ) m2lab
	
