#------------------------------------------------------------------------------
include $(FEDRA_ROOT)/src/config/ProjectDef.mk
include $(FEDRA_ROOT)/src/config/RootDef.mk
#------------------------------------------------------------------------------

TARGET	=	$(BIN_DIR)/mc2raw$(ExeSuf) 

ROOTLIBS      += -lMLP
PROJECT_LIBS += $(GLIBS) -L$(FEDRA_ROOT)/lib -lScan -lEdb -lEbase -lEmath -lEphys -lvt -lEdr -lScan -lEdr -lEIO -lEMC -lAlignment -lDataConversion -lEdd -lTreePlayer -lScan -lEdb -lEbase -lEmath -lEphys -lvt -lEdr -lScan -lEdr -lEIO -lEMC -lAlignment -lDataConversion -lEdd -lTreePlayer $(ROOTLIBS)

#------------------------------------------------------------------------------
include ../../config/TargetsDef.mk
#--------------------------------------------------------------------------
