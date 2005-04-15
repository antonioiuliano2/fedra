#------------------------------------------------------------------------------
 include ../../config/ProjectDef.mk
 include ../../config/RootDef.mk
#------------------------------------------------------------------------------
PROJECT_LIBS = -L$(LIB_DIR) -lEdr -lEdb -lEIO -lEmath -lEphys -lvt

CXXFLAGS  += -I$(INC_DIR) 
CPPFLAGS  += $(CXXFLAGS)

checkrun : checkrun.o
	$(LD) checkrun.o $(LIBS) $(ROOTLIBS) $(PROJECT_LIBS) -o $(BIN_DIR)/checkrun;

clean:
	@rm -fr core *.bak .*bak *.o *.log *.out $(BIN_DIR)/checkrun *~ $(depfile)

depend:
	@if [ ! -f $(depfile) ]; then \
	  touch $(depfile); \
	  makedepend -f $(depfile) $(ROOTCFLAGS) *.cpp 2>/dev/null; \
	fi
