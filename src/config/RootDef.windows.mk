ObjSuf        = obj
SrcSuf        = cxx
ExeSuf        =
DllSuf        = dll
OutPutOpt     = /out:

ROOTCFLAGS    = -I$(ROOTSYS)/include

ROOTLIBS      = -LIBPATH:$(ROOTSYS)/lib libCore.lib libCint.lib \
                libHist.lib libGraf.lib libGraf3d.lib libGpad.lib libTree.lib \
                libRint.lib libPostscript.lib libMatrix.lib libPhysics.lib libGeom.lib libGui.lib \
!IF EXISTS("$(ROOTSYS)/lib/libWin32gdk.lib")
	        libWin32gdk.lib
!ELSE 
	        libWin32.lib
!ENDIF 

#ROOTGLIBS     = $(shell root-config --glibs)

# Windows
!include <win32.mak>

# /TP compile all files as C++ even if they have *.C extension
CC            = $(cc) 
CXX           = $(cc) 
CXXFLAGS      =  -GX -G5 -GR -MD -DWIN32 -DVISUAL_CPLUSPLUS -DUSE_ROOT \
                 -D_WINDOWS  /Fo$*.obj /TP /nologo
LD            = $(link) 
LDFLAGS       = -opt:ref  -nologo -pdb:none 
DLLFLAGS      = $(dlllflags:/PDB:NONE=) 
####SOFLAGS       = -shared

CXXFLAGS     = $(CXXFLAGS) $(ROOTCFLAGS) -I$(INC_DIR)
LIBS         = $(ROOTLIBS)
####GLIBS       = $(ROOTGLIBS)

CFLAGS   = $(CXXFLAGS)
CPPFLAGS = $(CXXFLAGS)