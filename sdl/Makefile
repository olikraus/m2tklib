#
# Makefile
#	for m2sdl
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

COMMON_INCLUDES = -I. `sdl-config --cflags`

COMMON_FLAGS = -g -Wall 
# COMMON_FLAGS += -pg
# -funsigned-char -funsigned-bitfields -fpack-struct
COMMON_FLAGS += $(COMMON_INCLUDES)

CFLAGS = $(COMMON_FLAGS) -std=gnu99 -I../src -DM2_USE_DBG_SHOW
LDLIBS = `sdl-config --libs` -lSDL_ttf

LIBSRC =
LIBSRC += m2eh2bd.c m2eh2bs.c m2eh4bs.c m2eh6bs.c m2.c m2obj.c m2key.c m2msg.c
LIBSRC += m2gfx.c m2ghdummy.c 
LIBSRC += m2usrnext.c m2usrprev.c m2usrupdn.c m2align.c
LIBSRC += m2navutl.c m2navinit.c m2navroot.c m2navupdn.c
LIBSRC += m2navnext.c m2navprev.c m2navlast.c m2navfirst.c m2navdataup.c m2navdatadn.c
LIBSRC += m2opt.c
LIBSRC += m2ellistbase.c m2ellistcalc.c m2ellistv.c m2ellisth.c m2ellistgrid.c m2ellistxy.c
LIBSRC += m2fnarg.c  
LIBSRC += m2elfnfmt.c m2elalign.c m2elcombo.c m2elbutton.c
LIBSRC += m2elstrptr.c m2elbuttonptr.c m2ellabelptr.c m2elstrlist.c m2elslbase.c m2elsb.c
LIBSRC += m2elroot.c m2null.c
LIBSRC += m2elu8base.c m2elu8num.c m2elu8hs.c
LIBSRC += m2ellabel.c m2elstr.c m2eltext.c m2elu32.c m2elinfo.c
LIBSRC += m2elsetval.c m2eltoggle.c m2elradio.c m2elsubutl.c
LIBSRC += m2draw.c m2elstrup.c m2gfxutl.c m2rom.c m2utlu8d.c
LIBSRC += m2utlstrdat.c m2ellabelfn.c

EXTRASRC = m2ghsdl.c m2sdl.c  

SRC = $(addprefix ../src/,$(LIBSRC)) $(EXTRASRC)

OBJ := $(SRC:.c=.o)

m2sdl: $(OBJ)
	$(LINK.o) $(OBJ) $(COMMON_FLAGS) $(LOADLIBES) $(LDLIBS) -o $@
	
# cflow.txt: $(SRC)
#	cflow -r -T -l -d2 $(COMMON_INCLUDES) $(SRC) > $@


clean:
	-rm $(OBJ) m2sdl
	
