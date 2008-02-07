#------------------------------------------------------------------------------
 include ../../config/ProjectDef.mk
 include ../../config/RootDef.mk
#------------------------------------------------------------------------------
PROJECT_LIBS = -L$(LIB_DIR) -lEdr -lEdb -lEIO -lEmath -lEphys -lScan -lEMC -lvt -lDataConversion -lEGraphTool

CXXFLAGS  += -I$(INC_DIR) 
CPPFLAGS  += $(CXXFLAGS)
ROOTLIBS  += -lGui -lRGL -lGed

edisplay : display.o
	$(LD) display.o $(LIBS) $(ROOTLIBS) $(PROJECT_LIBS) -o $(BIN_DIR)/edisplay

clean:
	@rm -fr core *.bak .*bak *.o *.log *.out $(BIN_DIR)/edisplay *~ $(depfile)

depend:
	@if [ ! -f $(depfile) ]; then \
	  touch $(depfile); \
	  makedepend -f $(depfile) $(ROOTCFLAGS) *.cpp 2>/dev/null; \
	fi
