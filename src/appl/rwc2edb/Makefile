#------------------------------------------------------------------------------
 include $(FEDRA_ROOT)/src/config/ProjectDef.mk
 include $(FEDRA_ROOT)/src/config/RootDef.mk
#------------------------------------------------------------------------------
PROJECT_LIBS = -L$(LIB_DIR) -lEdb -lDataConversion

CXXFLAGS  += -I$(INC_DIR) 
CPPFLAGS  += $(CXXFLAGS)

all: rwc2edb rwdread rwcread

rwc2edb: rwc2edb.o 
	$(LD) rwc2edb.o   -Wall -fPIC $(LIBS) $(ROOTLIBS) $(PROJECT_LIBS) -o $(FEDRA_ROOT)/bin/rwc2edb ;

rwdread: rwdread.o 
	$(LD) rwdread.o   -Wall -fPIC $(LIBS) $(ROOTLIBS) $(PROJECT_LIBS) -o $(FEDRA_ROOT)/bin/rwdread ;

rwcread: rwcread.o 
	$(LD) rwcread.o   -Wall -fPIC $(LIBS) $(ROOTLIBS) $(PROJECT_LIBS) -o $(FEDRA_ROOT)/bin/rwcread;

clean:
	@rm -fr $(FEDRA_ROOT)/bin/rwc2edb $(FEDRA_ROOT)/bin/rwcread $(FEDRA_ROOT)/bin/rwdread core *.bak *.o *.log *.out *~
