#------------------------------------------------------------------------------
include $(FEDRA_ROOT)/src/config/ProjectDef.mk
include $(FEDRA_ROOT)/src/config/RootDef.mk
#------------------------------------------------------------------------------

TARGET = $(BIN_DIR)/rwcToEdb$(ExeSuf)

PROJECT_LIBS  +=  $(OBJJ) -L$(FEDRA_ROOT)/lib -lEdb

OBJJ = Catalog.o Fragment.o ProcessRawData.o Side.o Track.o View.o fitter.o

#------------------------------------------------------------------------------
include ../../config/TargetsDef.mk
#------------------------------------------------------------------------------

$(BIN_DIR)/rwcToEdb$(ExeSuf): Catalog.o Fragment.o ProcessRawData.o Side.o Track.o View.o fitter.o
