#------------------------------------------------------------------------------
 include ../../config/ProjectDef.mk
 include ../../config/RootDef.mk
#------------------------------------------------------------------------------
PROJECT_LIBS = -L$(LIB_DIR) -lEdb

CXXFLAGS  += -I$(INC_DIR)
CPPFLAGS  += $(CXXFLAGS)

rfxx : rfxx.o
	$(LD) rfxx.o $(LIBS) $(ROOTLIBS) $(PROJECT_LIBS) -o rfxx;
