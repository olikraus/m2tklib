#
#   Generic and Simple Atmel AVR GNU Makefile
#
#   Desinged for the gnu-avr tool chain
#
#   m2tklib - Mini Interative Interface Toolkit Library
#   u8glib - Universal 8bit Graphics Library
#   
#  Copyright (C) 2012  olikraus@gmail.com
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
# 	Features
#		- upload
#		- create exe from library
#		- create assembler listing (.dis)
#
#	Limitations
#		- only C-files supported
#		- no automatic dependency checking (call 'make clean' if any .h files are changed)
#
#	Targets:
#		make
#			create hex file, no upload
#		make upload
#			create and upload hex file
#		make clean
#			delete all generated files
#
#  Note:
#  	Display list make database: make -p -f/dev/null | less

#================================================
# Project Information
TARGETNAME = u8g_avr
MCU:=atmega328p
F_CPU:=8000000
SRC = $(shell ls *.c)
U8GDIR = ../../../../../u8g/u8glib/csrc/
FONTDIR = ../../../../../u8g/u8glib/sfntsrc/
M2DIR = ../../../src/
M2AVRU8G = ../../../dev/avr_u8g/
M2U8GLIB = ../../../dev/u8glib/

#================================================
# System/Environment Information
AVRTOOLSPATH:=/usr/bin/
# Type: "avrdude -c ?" to get a full listing.
AVRDUDE_PROGRAMMER := avrispmkii
# com1 = serial port. Use lpt1 to connect to parallel port.
AVRDUDE_PORT := usb

#================================================
# Main part of the Makefile starts here. Usually no changes are needed.

# Append U(G Library
# SRC += $(addprefix $(U8GDIR), $(shell ls $(U8GDIR)*.c 2>/dev/null))
SRC += $(shell ls $(U8GDIR)*.c)
SRC += $(shell ls $(FONTDIR)*.c)
SRC += $(shell ls $(M2DIR)*.c)
SRC += $(shell ls $(M2AVRU8G)*.c)
SRC += $(shell ls $(M2U8GLIB)*.c)


# Internal Variable Names
AVRDUDE_FLASH = -U flash:w:$(TARGETNAME).hex
AVRDUDE_FLAGS = -F -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER) -v -v
LIBNAME:=$(TARGETNAME).a
ELFNAME:=$(TARGETNAME).elf
HEXNAME:=$(TARGETNAME).hex
DISNAME:=$(TARGETNAME).dis
OBJ := $(SRC:.c=.o)
# Replace standard build tools by avr tools
CC = $(AVRTOOLSPATH)avr-gcc
CXX = $(AVRTOOLSPATH)avr-g++
AR  = @$(AVRTOOLSPATH)avr-ar
# AVR GNU Tools
OBJCOPY:=$(AVRTOOLSPATH)avr-objcopy
OBJDUMP:=$(AVRTOOLSPATH)avr-objdump
SIZE:=$(AVRTOOLSPATH)avr-size
AVRDUDE = avrdude
# C flags
COMMON_FLAGS = -DF_CPU=$(F_CPU) -mmcu=$(MCU)
COMMON_FLAGS += -g -Os -Wall -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
COMMON_FLAGS += -I. -I$(U8GDIR) -I$(M2DIR) -I$(M2U8GLIB)
COMMON_FLAGS += -ffunction-sections -fdata-sections -Wl,--gc-sections
COMMON_FLAGS += -Wl,--relax
COMMON_FLAGS += -mcall-prologues
CFLAGS = $(COMMON_FLAGS) -std=gnu99 -Wstrict-prototypes  

# Additional Suffixes
.SUFFIXES: .elf .hex .dis

# Targets
.PHONY: all
all: $(DISNAME) $(HEXNAME)
	$(SIZE) $(ELFNAME)

.PHONY: upload
upload: $(DISNAME) $(HEXNAME)
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_FLASH)
	$(SIZE) $(ELFNAME)

.PHONY: clean
clean:
	$(RM) $(HEXNAME) $(ELFNAME) $(LIBNAME) $(DISNAME) $(OBJ)

# implicit rules
.elf.hex:
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# explicit rules
$(ELFNAME): $(LIBNAME)($(OBJ)) 
	$(LINK.o) $(COMMON_FLAGS) $(LIBNAME) $(LOADLIBES) $(LDLIBS) -o $@

$(DISNAME): $(ELFNAME)
	$(OBJDUMP) -S $< > $@

	
