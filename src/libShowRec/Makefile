#------------------------------------------------------------------------------
include ../config/ProjectDef.mk
include ../config/RootDef.mk
#------------------------------------------------------------------------------

LIBS += -lMLP


NAME          = ShowRec

headers       = EdbShowRec.h EdbShowerP.h EdbShowAlg.h  EdbShowAlgE.h EdbShowAlg_NN.h EdbShowAlgE_Simple.h EdbShowAlg_GS.h EdbShowAlgClean.h EdbShowAlgID_E_G_Simple.h EdbShowPVRQuality.h

PROJECT_LIBS  = $(PROJECT_LIBS) -lEmath -lEdb -lvt -lEdr -lEIO  # old... but still working
# PROJECT_LIBS  = $(PROJECT_LIBS) -lEmath -lEdb -lEbase -lvt -lEdr -lEIO -lAlignment  # new ferda...

#------------------------------------------------------------------------------
include ../config/TargetsDef.mk
#------------------------------------------------------------------------------
