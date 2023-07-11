# Makefile
# ^^^^^^^^^^^^^^^^^
# Author  	: TychoJ
# Version 	: 0.1
#
# Inspired by 	: Edwin Boer
#
# File		: make.avr.mk
# Contains	: The configurations for make.avr.mk

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

# Source code folder
SRCFOLDER = src/
# Object folder
OBJFOLDER = obj/
# Binary folder
BINFOLDER = bin/

# Project name
PROJECTNAME = calcdigitalFilterCoef

# External source code library folders
EXTFOLDER =

# Compiler flags
CFLAGS = -Wall -std=c99 -fsanitize=address -lm

# Execute the configurations
include make.mk