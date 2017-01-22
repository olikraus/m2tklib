
U8G2PATH = ../../../u8g2/
M2TKPATH = ../../


CFLAGS = -g -Wall -I$(U8G2PATH)csrc/.  -I$(M2TKPATH)src/. -I$(M2TKPATH)dev/u8g2/. -I$(M2TKPATH)dev/tty/. `sdl-config --cflags` 
# CFLAGS += -DM2_EL_MSG_DBG_SHOW

# CFLAGS += -DM2_EL_MSG_DBG_SHOW -DU8G_16BIT 

SRC = $(shell ls $(U8G2PATH)csrc/*.c) 
SRC += $(U8G2PATH)sys/sdl/common/u8x8_d_sdl_128x64.c
#SRC += $(U8GPATH)sys/pbm/dev/u8g_dev_pbm.c
SRC += $(shell ls $(M2TKPATH)src/*.c) $(shell ls $(M2TKPATH)dev/u8g2/*.c)
SRC += $(shell ls $(M2TKPATH)dev/tty/*.c)
SRC += main.c 


OBJ = $(SRC:.c=.o)

helloworld: $(OBJ) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) `sdl-config --libs` -o u8g2_sdl

clean:	
	-rm $(OBJ) u8g2_sdl

