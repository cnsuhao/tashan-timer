include ../Makefile.FLTK
#.SILENT:

TOPPATH      := ..
SRC_PATH     := ./
OBJ_PATH     := ./obj/

SRC := $(foreach x,$(SRC_PATH),$(wildcard $(addprefix $(x)*,.c .cxx))) 
OBJFILES := $(addprefix $(OBJ_PATH), $(addsuffix .o,$(notdir $(basename $(SRC))))) 

CXXFLAGS +=  `pkg-config --cflags libxml-2.0`

$(OBJ_PATH)%.o:%.cxx
	@echo "*** Compile cxx $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_PATH)%.o:%.c
	@echo "*** Compile c $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@


# default target -- build everything
#default all: $(OBJFILES)

main:$(OBJ_PATH)main.o
	@echo "*** Linking $@..."
	$(CXX) $< ../obj/TskTimer.o ../obj/TskTimerMgr.o ../obj/TskTimerMgrHlp.o $(LINKFLTK_ALL) -o $@

mftest:
	echo `pkg-config --version`
	@echo $(SRC)
	@echo $(CXXFLAGS)

# clean everything
clean:
	$(RM) $(OBJFILES)
	$(RM) *.o

