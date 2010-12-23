#------------------------------------------------------------------------------
include ../../config/ProjectDef.mk
include ../../config/RootDef.mk
#------------------------------------------------------------------------------

TARGET  =       $(BIN_DIR)/eda$(ExeSuf)

ifeq ($(ROOTVERSION),NEWER522)
  PROJECT_LIBS  = -lMLP -lEmath -lEdb -lEbase -lEdr -lvt -lEphys -lEIO -lScan -lEDA -lEIO -lAlignment -lDataConversion -lShower -lEve -lGui -lGed -lGeom -lEG -lRGL -lTreePlayer
else
  PROJECT_LIBS  = -lMLP -lEmath -lEdb -lEbase -lEdr -lvt -lEphys -lEIO -lEve -lGui -lRGL -lScan -lEDA -lEIO -lAlignment -lDataConversion -lShower
endif

#------------------------------------------------------------------------------
include ../../config/TargetsDef.mk
#--------------------------------------------------------------------------
