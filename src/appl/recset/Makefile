#------------------------------------------------------------------------------
 include ../../config/ProjectDef.mk
 include ../../config/RootDef.mk
#------------------------------------------------------------------------------
PROJECT_LIBS = -L$(LIB_DIR) -lEdr -lEdb -lEmath

CXXFLAGS  += -I$(INC_DIR)
CPPFLAGS  += $(CXXFLAGS)

recset : recset.o
	$(LD) recset.o $(LIBS) $(ROOTLIBS) $(PROJECT_LIBS) -o $(BIN_DIR)/recset;
