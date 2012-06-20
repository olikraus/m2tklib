
U8GPATH = ../../../../u8g/u8glib/
M2TKPATH = ../../


CFLAGS = -g -Wall -I$(U8GPATH)csrc/.  -I$(M2TKPATH)src/. -I$(M2TKPATH)dev/u8glib/. -I$(M2TKPATH)dev/tty/. `sdl-config --cflags` 
# CFLAGS += -DM2_EL_MSG_DBG_SHOW

# CFLAGS += -DM2_EL_MSG_DBG_SHOW -DU8G_16BIT 

SRC = $(shell ls $(U8GPATH)csrc/*.c) $(shell ls $(U8GPATH)sfntsrc/*.c ) 
SRC += $(U8GPATH)sys/sdl/dev/u8g_dev_sdl.c
SRC += $(U8GPATH)sys/pbm/dev/u8g_dev_pbm.c
SRC += $(shell ls $(M2TKPATH)src/*.c) $(shell ls $(M2TKPATH)dev/u8glib/*.c)
SRC += $(shell ls $(M2TKPATH)dev/tty/*.c)
SRC += main.c 


OBJ = $(SRC:.c=.o)

helloworld: $(OBJ) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) `sdl-config --libs` -o u8g_sdl

clean:	
	-rm $(OBJ) u8g_sdl

