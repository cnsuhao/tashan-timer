
include Makefile.FLTK
#include ../fltk-1.3.3/makeinclude
#VPATH = ..
#vpath %.c .
#vpath %.cxx .

FLTKCONFIG   = ../fltk-1.3.3/fltk-config
CXX          = $(shell $(FLTKCONFIG) --cxx) -g
CXXFLAGS     = -I. -I../fltk-1.3.3/include $(shell $(FLTKCONFIG) --cxxflags) -g -p -O0 -Wall -I.  `pkg-config --cflags glu`
LINKFLTK     = $(shell $(FLTKCONFIG) --ldstaticflags ) -lGLU
LINKFLTK_GL  = $(shell $(FLTKCONFIG) --use-gl --use-glut --ldstaticflags ) -lGLU
LINKFLTK_IMG = $(shell $(FLTKCONFIG) --use-images --ldstaticflags)
LINKFLTK_ALL = $(shell $(FLTKCONFIG) --use-images --use-gl --use-glut --ldstaticflags)  -g -O0  `pkg-config --libs glu`
.SUFFIXES: .cxx .h .fl .o shapedwindow$(EXEEXT)
#OBJ_PATH = obj/

#CPPFILES =main.cxx cubebox.cxx mainframe.cxx childframe.cxx taskframe.cxx about_panel.cxx flstring.c
#OBJFILES =main.o cubebox.o mainframe.o childframe.o taskframe.o about_panel.o flstring.o

BIN_PATH = .
SRC_PATH := ./ ui/ utils/
SRC= $(foreach x,$(SRC_PATH),$(wildcard $(addprefix $(x)*,.c .cxx))) 
OBJFILES = $(addprefix $(OBJ_PATH), $(addsuffix .o,$(notdir $(basename $(SRC))))) 

TARGET = task-timer

SUBDIRS = ui utils

override define makesub
	echo "call makesub..."
	make -C $(1)
endef

#.PHONY: $(SUBDIRS)
task-timer: $(OBJFILES)  $(SUBDIRS) 
	@echo "*** Linking $@..."
	for n in $(SUBDIRS); do \
	  make -C $$n; \
	done
	$(CXX) $(OBJFILES) $(LINKFLTK_ALL) -o $@

$(OBJ_PATH)%.o:%.cxx
	@echo "*** Compile cxx $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_PATH)%.o:%.c
	@echo "*** Compile c $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

#.PHONY: $(SUBDIRS)
$(OBJFILES): $(SUBDIRS)
#	$(CXX) -I.. $(CXXFLAGS) -c $(SRC) 

.PHONY: testmk
testmk:
	for n in $(SUBDIRS); do \
	  make -C $$n; \
	done

#ui:
#	@echo "enter ./$@"
#	make -C $@

#.PHONY: utils
#utils:
#	@echo "enter ./$@"
#	make -C $@

$(SUBDIRS):
	for n in $(SUBDIRS); do \
	  make -C $$n; \
	done
	#$(foreach x, $(SUBDIRS), $(make -C $$x))  

main.o:$(SRC)
	@echo "*** Compile $<..."
	$(CXX) -I.. $(CXXFLAGS) -c $< -o $(OBJ_PATH)$@

RM    = rm -f
SHELL = /bin/sh
.SILENT:

# Executables
#ALL = shapedwindow$(EXEEXT)

# default target -- build everything
#default all: $(ALL)

# HOW TO COMPILE
$(OBJ_PATH)%.o:%.cxx
	@echo "*** Compile cxx $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_PATH)%.o:%.c
	@echo "*** Compile c $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

#$(OBJ_PATH).cxx.o obj/.o:
#	@echo "*** Compile2 $<..."
#	$(CXX) -I.. $(CXXFLAGS) -c $< -o $(OBJ_PATH)$@

#obj/.cxx.o obj/.c.o:
#	@echo "*** Compile $<..."
#	@echo $(CXXFLAGS)
#	$(CXX) -I.. $(CXXFLAGS) -c $< -o $(OBJ_PATH)$@

cscope:
	-rm cscope.files
	-rm cscope.in.out
	-rm cscope.out
	-rm cscope.po.out
	find . -name "*.c" -o -name "*.cxx" -o -name "*.cpp" -o -name "*.h" -o -name "*.hpp" -o -name "*.H" >cscope.files
	cscope -q -I /home/lixing/include -I . -I /usr/include -R -b -i cscope.files
	export CSCOPE_DB=./cscope.out:/run/media/lixing/works/src/gui/fltk-1.3.3/cscope.out 

# HOW TO LINK
#.o$(EXEEXT):
#	@echo "*** Linking $@..."
#	$(CXX) $< $(LINKFLTK_ALL) -o $@
.PHONY: clean  
# clean everything
clean:
	$(RM) $(OBJ_PATH)*.o
	$(RM) $(TARGET)

