#------------------------------------------------------------------------------
 include $(FEDRA_ROOT)/src/config/ProjectDef.mk
 include $(FEDRA_ROOT)/src/config/RootDef.mk
#------------------------------------------------------------------------------
PROJECT_LIBS = -L$(LIB_DIR) -lEdr -lEdb -lEmath -lEphys -lvt

CXXFLAGS  += -I$(INC_DIR) 
CPPFLAGS  += $(CXXFLAGS)

all: rwc2edb rwdread rwcread

rwc2edb: libDataConversion.o  rwc2edb.o 
	$(LD) libDataConversion.o rwc2edb.o   -Wall -fPIC $(LIBS) $(ROOTLIBS) $(PROJECT_LIBS) -o $(FEDRA_ROOT)/bin/rwc2edb ;

rwdread: libDataConversion.o  rwdread.o 
	$(LD) libDataConversion.o rwdread.o   -Wall -fPIC $(LIBS) $(ROOTLIBS) $(PROJECT_LIBS) -o $(FEDRA_ROOT)/bin/rwdread ;

rwcread: libDataConversion.o  rwcread.o 
	$(LD) libDataConversion.o rwcread.o   -Wall -fPIC $(LIBS) $(ROOTLIBS) $(PROJECT_LIBS) -o $(FEDRA_ROOT)/bin/rwcread;

clean:
	@rm -fr $(FEDRA_ROOT)/bin/rwc2edb $(FEDRA_ROOT)/bin/rwcread $(FEDRA_ROOT)/bin/rwdread core *.bak *.o *.log *.out *~


libDataConversion.obj: dataIO/dataIO.h
libDataConversion.obj: dataIO/dataIO.cpp