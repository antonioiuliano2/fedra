#------------------------------------------------------------------------------
include ../config/ProjectDef.mk
include ../config/RootDef.mk
#------------------------------------------------------------------------------

ACQ_DIR = $(PROJECT_ROOT)\src\libACQ
NAME          = ACQ

headers       = AcqStage1.h AcqOdyssey.h AcqMarksScanner.h AcqTrackScanner.h \
                AcqTrackReconstructor.h  AcqSerialPlateChanger.h AcqCOM.h

ODYSSEY_LIB  = $(ACQ_DIR)\Matrox/onl/lib/obj/nt
ODYSSEY_INC  = $(ACQ_DIR)\Matrox/onl/src/headers/common
ODYSSEYMIL_LIB  = $(ACQ_DIR)\Mil\lib
ODYSSEYMIL_INC  = $(ACQ_DIR)\Mil\Include

PROJECT_LIBS  = -LIBPATH:$(LIB_DIR) -LIBPATH:$(ODYSSEY_LIB) -LIBPATH:$(ODYSSEYMIL_LIB) \
                mil.lib odclib.lib milblob.lib milcal.lib milcode.lib miledge.lib milim.lib milmeas.lib \
                NI/lib/FlexMS32.lib -lEdb -lEGA \
                serialport.obj


#!if ("$(_NMAKE_VER)" == "8.00.50727.42") || ("$(_NMAKE_VER)" == "8.00.50727.762")
#	serialport.obj_nodelete80
#!else
#	serialport.obj_nodelete71
#!endif

CXXFLAGS = $(CXXFLAGS)  -D_AFXDLL   -wd4430
# -wd4430  (missing type specifier - int assumed. Note: C++ does not support default-int)

INC_DIR = $(INC_DIR) -I$(ODYSSEY_INC) -I$(ODYSSEYMIL_INC) -I$(ACQ_DIR)\Ni\include

EXTRAOBJS   = serialport.obj

#------------------------------------------------------------------------------
include ../config/TargetsDef.mk
#------------------------------------------------------------------------------

