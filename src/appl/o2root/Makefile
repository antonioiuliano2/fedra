#------------------------------------------------------------------------------
include ../../config/ProjectDef.mk
include ../../config/RootDef.mk
#------------------------------------------------------------------------------

TARGET  =       $(BIN_DIR)/o2root$(ExeSuf)

ORA           = $(OCCIHOME)
ORAINC        = $(ORA)/sdk/include
ORALIB        = $(ORA)

PROJECT_LIBS += -L$(ORACLE_HOME)/instantclient_10_2 -lclntsh -locci
PROJECT_LIBS += -L$(FEDRA_ROOT)/lib -lScan -lEdb -lEmath -lEphys -lvt -lEdr -lScan -lEdr -lEOracle -lEIO -lEMC -lDataConversion

INC_DIR      += -I$(ORAINC)
CXXFLAGS     += -I$(INC_DIR) -DNOTROOTCINT

#------------------------------------------------------------------------------
include ../../config/TargetsDef.mk
#--------------------------------------------------------------------------
