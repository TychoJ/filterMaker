# make.mk
# ^^^^^^^^^^^^^^^^^
# Author  	: TychoJ
# Version 	: 0.1
#
# Inspired by 	: Edwin Boer
#
# File	 	: make.avr.mk
# Contains  	: The make process for building/flashing/debuging of AVR microcontrollers
#		  The settings for this make file can be found in: Makefile

# MIT License
#
# Copyright (c) 20223 TychoJ
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# From stackoverflow https://stackoverflow.com/a/12959764/16373649
# Make does not offer a recursive wildcard function, so here's one:
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
# End from stackoverflow https://stackoverflow.com/a/12959764/16373649

# Generate a list of folders to be included
INCLUDELIST := $(foreach dir, $(EXTFOLDER), -I$(dir))
# Generate a list of source code files relative of the source folder
CLIST       :=  $(call rwildcard,$(SRCFOLDER),*.c)
# Generate a list of library files
EXTLIST     := $(foreach dir, $(EXTFOLDER), $(call rwildcard,$(dir),*.c))
# Generate a list of object files
OBJLIST     := $(patsubst %.c, %.o, $(CLIST)) $(patsubst %.c, %.o, $(EXTLIST))
# Generate a list of object files in the object(sub)folder
OBJLIST2    := $(foreach file, $(OBJLIST), $(OBJFOLDER)$(file))

# Define all tools with standard settings
TOOLC     = gcc $(CFLAGS) 
TOOLDUMP  = objdump

# Public goals

# Goal: make help
help: _start _help1 _end

_start:
	@echo
	@echo "#################################"
	@echo "  > Start: make.mk"

_help1:
	@echo "_________________________________"
	@echo "  > Possible options:"
	@echo "    make             := Show this help message."
	@echo "    make help        := Show this help message."
	@echo "    make all         := Compile and link the source code."
	@echo "    make disasm      := Disassemble the code for debugging."
	@echo "    make clean       := Delete all generated binaries and .o files."
	@echo "    make distclean   := Delete all generated binaries and .o files as wel as the $(BINFOLDER) and the $(OBJFOLDER) folders."

_end:
	@echo
	@echo "#################################"
	@echo "  > Finished :-)"
	@echo

# Goal: make all
all: _start _all1 $(BINFOLDER)$(PROJECTNAME) _end

_all1:
	@echo "  > Goal: Compile and link the source code."
	@echo "  > Object folder: $(OBJFOLDER)"
	@echo "  > Binary folder: $(BINFOLDER)"
	@mkdir -p $(OBJFOLDER)
	@mkdir -p $(BINFOLDER)

# Goal: make disasm
disasm: _start _disasm1 $(BINFOLDER)/$(PROJECTNAME) _disasm2 _end

_disasm1:
	@echo	"  > Goal: Disassemble the code for debugging."

_disasm2: $(BINFOLDER)/$(PROJECTNAME)
	$(TOOLDUMP) -d $(BINFOLDER)$(PROJECTNAME) > $(BINFOLDER)$(PROJECTNAME).asm

# Goal: make clean
clean: _start _clean _end

_clean:
	@echo "  > Goal: Delete all generated binaries, and .o files."
	rm -f    $(BINFOLDER)$(PROJECTNAME)
	rm -f    $(BINFOLDER)$(PROJECTNAME)
	rm -f -r $(OBJFOLDER)$(SRCFOLDER)

distclean: _start _distclean _end

_distclean:
	@echo "  > Goal: Delete all generated binaries and .o files as wel as the $(BINFOLDER) and the $(OBJFOLDER) folders."
	rm -f    $(BINFOLDER)$(PROJECTNAME)
	rm -f -r $(OBJFOLDER)$(SRCFOLDER)
	rm -r 	 $(OBJFOLDER)
	rm -r 	 $(BINFOLDER)

# Create specific file formats
$(BINFOLDER)$(PROJECTNAME): _compiler $(OBJLIST)
	@echo "_________________________________"
	@echo "  > Start linker"
	$(TOOLC) -o $(BINFOLDER)$(PROJECTNAME) $(OBJLIST2)

_compiler:
	@echo "_________________________________"
	@echo "  > Start compiler"

%.o: %.c
	@mkdir -p $(OBJFOLDER)$(dir $@)
	@echo "  > make object files"
	$(TOOLC) $(INCLUDELIST) -c -o $(OBJFOLDER)$@ $<