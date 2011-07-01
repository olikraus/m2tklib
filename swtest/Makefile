

COMMON_INCLUDES = -I.

COMMON_FLAGS = -g -Os -Wall 
COMMON_FLAGS += --coverage
# -funsigned-char -funsigned-bitfields -fpack-struct
COMMON_FLAGS += $(COMMON_INCLUDES)

CFLAGS = $(COMMON_FLAGS) -std=gnu99 -Wstrict-prototypes -I../src
LDLIBS =

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
LIBSRC += m2elstrptr.c m2elbuttonptr.c m2ellabelptr.c
LIBSRC += m2elroot.c m2null.c
LIBSRC += m2elu8base.c m2elu8num.c m2elu8hs.c
LIBSRC += m2ellabel.c m2elstr.c m2eltext.c m2elu32.c 
LIBSRC += m2elsetval.c m2eltoggle.c m2elradio.c m2elsubutl.c
LIBSRC += m2draw.c m2elstrup.c m2gfxutl.c m2rom.c m2utlu8d.c
LIBSRC += m2utlstrdat.c  

EXTRASRC = m2test.c

SRC = $(addprefix ../src/,$(LIBSRC)) $(EXTRASRC)

OBJ := $(SRC:.c=.o)

#GCNO := $(SRC:.c=.gcno)
#GCDA := $(SRC:.c=.gcda)

m2test: $(OBJ)
	$(LINK.o) $(OBJ) $(COMMON_FLAGS) $(LOADLIBES) $(LDLIBS) -o $@

#exec:
#	./m2test
#	gcov -a -o ../src $(SRC) > gcov.log


clean:
	-rm $(OBJ) m2test
	-rm $(GCNO) $(GCDA)
	-rm *.gcov gcov.log
	
