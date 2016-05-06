#
#  (C) Copyright 2016 I/O Blocks Toolkit (https://iobtoolkit.com/) and others.
# 
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
# 
#      http://www.apache.org/licenses/LICENSE-2.0
# 
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
# 
#  Contributors:
#      I/O Blocks Toolkit Team
#

# Makefile manual
#
# SRCDIRS  -- Source directories. All files from these will be compiled and 
#             linked. Files are included on wildcard (*.c) basis.
# SRCFILES -- Add individial *.c files here.
# TARGET   -- Final executable basename.
# DEFINES  -- globaly defined preprocessor symbols
#
# Compiler paths and options are in Toolchain definition section.

##########################
# Hardware configuration #
##########################

MCU     = atmega328p
AVRISP  = avrispmkii
ISPPORT = usb

DEFINES += F_CPU=16000000
DEFINES += F_USB=16000000

############################
# Source code and includes #
############################

SRCDIRS += src
SRCDIRS += src/drivers

INCDIRS += src

TARGET   := binary

#####################################
# Toolchain definition and settings #
#####################################

CFLAGS += -mmcu=$(MCU)
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
CFLAGS += -std=c99
CFLAGS += -Wall
#CFLAGS += -pedantic
CFLAGS += -c

LDFLAGS += -mmcu=$(MCU)
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -Wl,-Map,$(TARGET).map
LDFLAGS += -lgcc
LDFLAGS += -lc

OPT   = -Os
DEBUG = -g3 -gdwarf-2

# compiler, linker and stuff 
TOOLCHAIN := avr
CC  := $(TOOLCHAIN)-gcc
AS  := $(TOOLCHAIN)-as
AR  := $(TOOLCHAIN)-ar
GDB := $(TOOLCHAIN)-gdb
LD  := $(TOOLCHAIN)-gcc
SZ  := $(TOOLCHAIN)-size

############################################################
# Try not to fiddle with code below. Automagic starts here #
############################################################

DEFS := $(foreach _DEF, $(DEFINES), -D$(_DEF))
SRCS := $(foreach _DIR, $(SRCDIRS), $(wildcard $(_DIR)/*.c)) $(SRCFILES)
OBJS := $(SRCS:.c=.o)
INCS := $(foreach _INC, $(INCDIRS), -I$(_INC))

COMPILER := $(CC) $(DEBUG) $(OPT) $(CFLAGS) $(DEFS) $(INCS)

all: objects link hex sizes

objects: $(OBJS)

link: objects
    ifdef VERBOSE
	$(LD) $(LDFLAGS) $(DEBUG) -o $(TARGET).elf $(OBJS)
    else
	@echo "  LD    $(TARGET).elf"
	@$(LD) $(LDFLAGS) $(DEBUG) -o $(TARGET).elf $(OBJS)
    endif

%.o: %.c
    ifdef VERBOSE
	$(COMPILER) -o $(@) $(<)
    else
	@echo "  CC    $(@)"
	@$(COMPILER) -o $(@) $(<)
    endif

hex: link
	@echo "Generating HEX"
	@avr-objcopy -O ihex $(TARGET).elf $(TARGET).hex

sizes: link
	@echo
	@echo Sizes:
	@$(SZ) --format=berkeley $(TARGET).elf
	@echo

avrdude: all
	@avrdude -c $(AVRISP) -P $(ISPPORT) -p $(MCU) -U flash:w:$(TARGET).hex

clean:
	@echo -n "Cleaning source tree... "
	@rm -rf $(OBJS) $(TARGET).*
	@rm -rf $(OBJS:.o=.d)
	@echo "done"

settings:
	@echo "*** Internal variables ***"
	@echo "Settings:"
	@echo " * Definitions:  "$(DEFS)
	@echo " * Source files: "$(SRCS)
	@echo " * Object files: "$(OBJS)
	@echo " * Includes:     "$(INCS)
	@echo
	@echo Toolchain:
	@echo " * Compiler:  "$(CC)
	@echo " * Assembler: "$(AS)
	@echo " * Debugger:  "$(GDB)
	@echo " * Linker:    "$(LD)
	@echo " * CFLAGS:    "$(CFLAGS)
	@echo " * LDFLAGS:   "$(LDFLAGS)

help:
	@echo "Available targets:"
	@echo "all      -- builds project executable"
	@echo "clean    -- clean project files"
	@echo "objects  -- build all source files but don't link"
	@echo "link     -- link built *.o files"
	@echo "gdb      -- build target and starts GDB with gdb_script"
	@echo "settings -- show project settings"
	@echo "help     -- this puny help message"
	@echo

