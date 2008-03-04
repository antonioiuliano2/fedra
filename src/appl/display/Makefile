#------------------------------------------------------------------------------
include ../../config/ProjectDef.mk
include ../../config/RootDef.mk
#------------------------------------------------------------------------------

TARGET        = $(BIN_DIR)/display$(ExeSuf)

EXTRALIBS     = -lThread -lGui -lGeom

PROJECT_LIBS  = $(EXTRALIBS) -lEdr -lEdb -lEIO -lEmath -lEphys -lvt \
		    -lDataConversion -lEGraphTool -lScan -lEMC -lEdd

#------------------------------------------------------------------------------
include ../../config/TargetsDef.mk
#------------------------------------------------------------------------------
