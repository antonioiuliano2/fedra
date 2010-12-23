#------------------------------------------------------------------------------
include $(FEDRA_ROOT)/src/config/ProjectDef.mk
include $(FEDRA_ROOT)/src/config/RootDef.mk
#------------------------------------------------------------------------------

TARGET	=	$(BIN_DIR)/emlink$(ExeSuf)      \
		$(BIN_DIR)/emalign$(ExeSuf)      \
		$(BIN_DIR)/makescanset$(ExeSuf)      \
		$(BIN_DIR)/emtra$(ExeSuf)      \
		$(BIN_DIR)/emrec$(ExeSuf)      

PROJECT_LIBS += -L$(FEDRA_ROOT)/lib -lScan -lEdb -lEbase -lEmath -lEphys -lvt -lEdr -lScan -lEdr -lEIO -lEMC -lAlignment -lDataConversion

#------------------------------------------------------------------------------
include ../../config/TargetsDef.mk
#--------------------------------------------------------------------------
