# all: ShowRec

CXXFLAGS += $(shell root-config --cflags --libs)
CXXFLAGS += -I$(FEDRA_ROOT)/include
# CXXFLAGS += -pedantic -Wall -Werror
CXXFLAGS += -w

#LDFLAGS  += $(shell root-config --nonew --libs --glibs)
LDFLAGS  += -L$(ROOTSYS)/lib -lMLP -lSpectrum
LDFLAGS  += -L$(FEDRA_ROOT)/lib -lEdb -lEdr -lEbase -lEIO -lAlignment -lEmath -lvt -lEphys -lDataConversion -lEIO -lShower -lShowRec 


#====================================================================================

all:  
#(1)	doesnt work anymore, many "undefined reference" errors
#	g++   $(CXXFLAGS) $(LDFLAGS) -o ShowRec  ShowRec.cpp 
#(2)	looks better, with including libEIO now it works (2018-06-01)
#	I dont know why the placement of the flags plays an role ...
	g++   -o ShowRec  ShowRec.cpp $(CXXFLAGS) $(LDFLAGS)
# WORKING.
clean:
	@rm ShowRec
