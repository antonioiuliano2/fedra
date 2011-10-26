#------------------------------------------------------------------------------
include $(FEDRA_ROOT)/src/config/ProjectDef.mk
include $(FEDRA_ROOT)/src/config/RootDef.mk
#------------------------------------------------------------------------------

TARGET	=	$(BIN_DIR)/viewdist$(ExeSuf)

ROOTLIBS      += -lMLP
PROJECT_LIBS += -L$(FEDRA_ROOT)/lib -lScan -lEdb -lEbase -lEmath -lEphys -lvt -lEdr -lScan -lEdr -lEIO -lEMC -lAlignment -lDataConversion -lShower -lTreePlayer

#------------------------------------------------------------------------------
include ../../config/TargetsDef.mk
#--------------------------------------------------------------------------
