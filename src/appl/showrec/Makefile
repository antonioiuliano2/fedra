all: ShowRec

CXXFLAGS = $(shell root-config --cflags)
CXXFLAGS += -I$(FEDRA_ROOT)/include
# CXXFLAGS += -pedantic -Wall -W 

LDFLAGS  += $(shell root-config --nonew --libs --glibs)
LDFLAGS  += -L$(ROOTSYS)/lib -lMLP -lTreePlayer -lSpectrum
LDFLAGS  += -L$(FEDRA_ROOT)/lib -lEdb -lEdr -lEbase -lAlignment -lEmath -lvt -lEphys -lDataConversion -lEIO -lShower

#====================================================================================

all:  
	g++   -o ShowRec $(LDFLAGS) $(CXXFLAGS) ShowRec.cpp
	@echo "$@ done"

clean:
	@rm ShowRec
