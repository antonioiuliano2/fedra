#------------------------------------------------------------------------------
include ../config/ProjectDef.mk
include ../config/RootDef.mk
#------------------------------------------------------------------------------

NAME          = ACQ

headers       = AcqStage1.h AcqOdyssey.h AcqMarksScanner.h AcqTrackScanner.h AcqTrackReconstructor.h  \
                AcqSerialPlateChanger.h AcqCOM.h 

ODYSSEY_LIB  = c:\fedra\src\libACQ\Matrox/onl/lib/obj/nt
ODYSSEY_INC  = c:\fedra\src\libACQ\Matrox/onl/src/headers/common
ODYSSEYMIL_LIB  = c:\fedra\src\libACQ\Mil\lib
ODYSSEYMIL_INC  = c:\fedra\src\libACQ\Mil\Include

PROJECT_LIBS  = $(PROJECT_LIBS) -LIBPATH:$(ODYSSEY_LIB) -LIBPATH:$(ODYSSEYMIL_LIB) -lEdb  NI/lib/FlexMS32.lib \
      mil.lib odclib.lib milblob.lib milcal.lib milcode.lib miledge.lib milim.lib milmeas.lib \
      libEGA.lib serialport.obj

CXXFLAGS     = $(CXXFLAGS)  -D_AFXDLL
INC_DIR = $(INC_DIR) -I$(ODYSSEY_INC) -I$(ODYSSEYMIL_INC) -Ic:\fedra\src\libACQ\Ni\include

#------------------------------------------------------------------------------
include ../config/TargetsDef.mk
#------------------------------------------------------------------------------
