
#include Makefile.FLTK
#include ../makeinclude

FLTKCONFIG   = ../fltk-config
CXX          = $(shell $(FLTKCONFIG) --cxx)
CXXFLAGS     = $(shell $(FLTKCONFIG) --cxxflags) -O2 -Wall -I.
LINKFLTK     = $(shell $(FLTKCONFIG) --ldstaticflags)
LINKFLTK_GL  = $(shell $(FLTKCONFIG) --use-gl --ldstaticflags)
LINKFLTK_IMG = $(shell $(FLTKCONFIG)  --ldstaticflags)
LINKFLTK_ALL = $(shell $(FLTKCONFIG)  --use-gl --ldstaticflags)  -O1
.SUFFIXES: .cxx .h .fl .o shapedwindow$(EXEEXT)


CPPFILES =shapedwindow.cxx cubebox.cxx
OBJFILES =shapedwindow.o cubebox.o

3Ddemo:$(OBJFILES)
	@echo "*** Linking $@..."
	$(CXX) $(OBJFILES) $(LINKFLTK_ALL) -o $@

shapedwindow.o:shapedwindow.cxx cubebox.cxx
	@echo "*** Compile $<..."
	$(CXX) -I.. $(CXXFLAGS) -c $(CPPFILES) 

RM    = rm -f
SHELL = /bin/sh
.SILENT:

# Executables
ALL = shapedwindow$(EXEEXT)

# default target -- build everything
default all: $(ALL)

# HOW TO COMPILE
.cxx.o:
	@echo "*** Compile $<..."
	$(CXX) -I.. $(CXXFLAGS) -c $< -o $@

# HOW TO LINK
#.o$(EXEEXT):
#	@echo "*** Linking $@..."
#	$(CXX) $< $(LINKFLTK_ALL) -o $@

# clean everything
clean:
	$(RM) $(ALL)
	$(RM) *.o
	$(RM) core

