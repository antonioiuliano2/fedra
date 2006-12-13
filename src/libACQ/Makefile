#------------------------------------------------------------------------------
include ../config/ProjectDef.mk
include ../config/RootDef.mk
#------------------------------------------------------------------------------

ACQ_DIR = $(PROJECT_ROOT)\src\libACQ
NAME          = ACQ

headers       = AcqStage1.h AcqOdyssey.h AcqMarksScanner.h AcqTrackScanner.h AcqTrackReconstructor.h  \
                AcqSerialPlateChanger.h AcqCOM.h 

ODYSSEY_LIB  = $(ACQ_DIR)\Matrox/onl/lib/obj/nt
ODYSSEY_INC  = $(ACQ_DIR)\Matrox/onl/src/headers/common
ODYSSEYMIL_LIB  = $(ACQ_DIR)\Mil\lib
ODYSSEYMIL_INC  = $(ACQ_DIR)\Mil\Include

PROJECT_LIBS  = $(PROJECT_LIBS) -LIBPATH:$(ODYSSEY_LIB) -LIBPATH:$(ODYSSEYMIL_LIB) -lEdb  NI/lib/FlexMS32.lib \
      mil.lib odclib.lib milblob.lib milcal.lib milcode.lib miledge.lib milim.lib milmeas.lib \
      libEGA.lib serialport.obj

CXXFLAGS     = $(CXXFLAGS)  -D_AFXDLL
INC_DIR = $(INC_DIR) -I$(ODYSSEY_INC) -I$(ODYSSEYMIL_INC) -Ic:\fedra\src\libACQ\Ni\include

#------------------------------------------------------------------------------
include ../config/TargetsDef.mk
#------------------------------------------------------------------------------
