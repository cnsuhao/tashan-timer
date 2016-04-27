include ../Makefile.FLTK
#.SILENT:

TOPPATH      := ..
SRC_PATH     := ./
OBJ_PATH     := $(TOPPATH)/obj/

SRC := $(foreach x,$(SRC_PATH),$(wildcard $(addprefix $(x)*,.c .cxx))) 
OBJFILES := $(addprefix $(OBJ_PATH), $(addsuffix .o,$(notdir $(basename $(SRC))))) 



$(OBJ_PATH)%.o:%.cxx
	@echo "*** Compile cxx $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_PATH)%.o:%.c
	@echo "*** Compile c $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# default target -- build everything
default all: $(OBJFILES)

test:
	@echo $(SRC)
	@echo $(CXX)

# clean everything
clean:
	$(RM) $(OBJFILES)
	$(RM) *.o

