#------------------------------------------------------------------------------
 include $(FEDRA_ROOT)/src/config/ProjectDef.mk
 include $(FEDRA_ROOT)/src/config/RootDef.mk
#------------------------------------------------------------------------------
PROJECT_LIBS = -L$(LIB_DIR) -lEdr -lEdb -lEmath -lEphys -lvt

CXXFLAGS  += -I$(INC_DIR) 
CPPFLAGS  += $(CXXFLAGS)

all: rwc2edb

rwc2edb : rwc2edb.o 
	$(LD)  rwc2edb.o  -Wall -fPIC $(LIBS) $(ROOTLIBS) $(PROJECT_LIBS) -o $(FEDRA_ROOT)/bin/rwc2edb;

clean:
	@rm -fr $(FEDRA_ROOT)/bin/rwc2edb core *.bak *.o *.log *.out *~
