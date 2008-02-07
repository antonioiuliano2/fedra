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

PROJECT_LIBS  = -LIBPATH:$(LIB_DIR) -LIBPATH:$(ODYSSEY_LIB) -LIBPATH:$(ODYSSEYMIL_LIB) -lEdb NI/lib/FlexMS32.lib \
      mil.lib odclib.lib milblob.lib milcal.lib milcode.lib miledge.lib milim.lib milmeas.lib \
      libEGA.lib serialport.obj_nodelete

CXXFLAGS = $(CXXFLAGS)  -D_AFXDLL   -wd4430
# -wd4430  (missing type specifier - int assumed. Note: C++ does not support default-int)

INC_DIR = $(INC_DIR) -I$(ODYSSEY_INC) -I$(ODYSSEYMIL_INC) -I$(ACQ_DIR)\Ni\include

#------------------------------------------------------------------------------
include ../config/TargetsDef.mk
#------------------------------------------------------------------------------
