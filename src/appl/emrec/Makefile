#------------------------------------------------------------------------------
include $(FEDRA_ROOT)/src/config/ProjectDef.mk
include $(FEDRA_ROOT)/src/config/RootDef.mk
#------------------------------------------------------------------------------

TARGET	=	$(BIN_DIR)/emlink$(ExeSuf)      \
		$(BIN_DIR)/emalign$(ExeSuf)      \
		$(BIN_DIR)/makescanset$(ExeSuf)      \
		$(BIN_DIR)/emtra$(ExeSuf)      \
		$(BIN_DIR)/emshow$(ExeSuf)      \
		$(BIN_DIR)/emrec$(ExeSuf)      \
		$(BIN_DIR)/emvtx$(ExeSuf)      \
		$(BIN_DIR)/emcheck$(ExeSuf)    \
		$(BIN_DIR)/empred$(ExeSuf)    \
		$(BIN_DIR)/emtrackan$(ExeSuf)



ROOTLIBS      += -lMLP
##PROJECT_LIBS += -L$(FEDRA_ROOT)/lib -lScan -lEdb -lEbase -lEmath -lEphys -lvt -lEdr -lScan -lEdr -lEIO -lEMC -lAlignment -lDataConversion -lShower -lTreePlayer
PROJECT_LIBS += $(GLIBS) -L$(FEDRA_ROOT)/lib -lScan -lEdb -lEbase -lEmath -lEphys -lvt -lEdr -lScan -lEdr -lEIO -lEMC -lAlignment -lDataConversion -lEdd -lTreePlayer

#------------------------------------------------------------------------------
include ../../config/TargetsDef.mk
#--------------------------------------------------------------------------
