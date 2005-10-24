#------------------------------------------------------------------------------
 include ../../config/ProjectDef.mk
 include ../../config/RootDef.mk
#------------------------------------------------------------------------------
PROJECT_LIBS = -L$(LIB_DIR) -lEdr -lEdb -lEIO -lEmath -lEphys -lvt -lDataConversion

CXXFLAGS  += -I$(INC_DIR) 
CPPFLAGS  += $(CXXFLAGS)

recset : recset.o
	$(LD) recset.o $(LIBS) $(ROOTLIBS) $(PROJECT_LIBS) -o $(BIN_DIR)/recset;

clean:
	@rm -fr core *.bak .*bak *.o *.log *.out $(BIN_DIR)/recset *~ $(depfile)

depend:
	@if [ ! -f $(depfile) ]; then \
	  touch $(depfile); \
	  makedepend -f $(depfile) $(ROOTCFLAGS) *.cpp 2>/dev/null; \
	fi
