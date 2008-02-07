# RootDef.windows.mk
# author: G. Sirri - INFN BO
#
# the following code is taken from $(ROOTSYS)\test\makefile.win32, 
# except for:
#
# - default CFG is Release
# - pre-processing to define LDFLAGS  (dependency on root version)
# - pre-processing to define ROOTLIBS (dependency on root version)


!IF "$(CFG)" == ""
CFG = Release
!MESSAGE No configuration specified. Defaulting to Win32 "$(CFG)".
!ENDIF 

!IF "$(CFG)" != "Release" && "$(CFG)" != "Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Makefile.win32" CFG="Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Release" (based on "Win32 (x86) Library") DEFAULT
!MESSAGE "Debug" (based on "Win32 (x86) Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

ObjSuf        = obj
SrcSuf        = cxx
ExeSuf        = .exe
DllSuf        = dll
OutPutOpt     = -out:

# Win32 system with Microsoft Visual C/C++
!include <win32.mak>
CC            = $(cc)
CXX           = $(cc)
CXXFLAGS      = $(cvarsdll) -EHsc -nologo -GR -DWIN32 \
                -DVISUAL_CPLUSPLUS -D_WINDOWS -I$(ROOTSYS)/include
LD            = $(link)

!IF  "$(CFG)" == "Release"
CXXOPT        = -O2 -MD
LDOPT         = -opt:ref
CXXFLAGS      = $(CXXFLAGS:-MDd=)
!ELSEIF  "$(CFG)" == "Debug"
CXXOPT        = -Z7 -MDd
LDOPT         = -debug
!ENDIF 

!if ("$(_NMAKE_VER)" == "8.00.50727.42") || ("$(_NMAKE_VER)" == "8.00.50727.762")
MT_EXE        = mt -nologo -manifest $@.manifest -outputresource:$@;1
MT_DLL        = mt -nologo -manifest $@.manifest -outputresource:$@;2
EXTRAFLAGS    = -D_CRT_SECURE_NO_DEPRECATE
!else
MT_EXE        = 
MT_DLL        = 
EXTRAFLAGS    = -G5
!endif

# pre-processing to define LDFLAGS  (dependent on root version)

!IF [ %FEDRA_ROOT%\win32\tools\rootversioncode.cmd ] <= 331264   #ROOTSTS <= 5.14.00
LDFLAGS       = $(LDOPT) $(conlflags) -nologo
!ELSE                                                            #ROOTSYS >= 5.16.00
LDFLAGS       = $(LDOPT) $(conlflags) -nologo -include:_G__cpp_setupG__Hist \
                -include:_G__cpp_setupG__Graf1 -include:_G__cpp_setupG__G3D \
                -include:_G__cpp_setupG__GPad -include:_G__cpp_setupG__Tree \
                -include:_G__cpp_setupG__Rint -include:_G__cpp_setupG__PostScript \
                -include:_G__cpp_setupG__Matrix -include:_G__cpp_setupG__Physics
!ENDIF
#
                
SOFLAGS       = $(dlllflags:-pdb:none=)

# - pre-processing to define ROOTLIBS (dependent on root version)
ROOTLIBS      = $(ROOTSYS)/lib/libCore.lib \
                $(ROOTSYS)/lib/libCint.lib $(ROOTSYS)/lib/libHist.lib \
                $(ROOTSYS)/lib/libGraf.lib $(ROOTSYS)/lib/libGraf3d.lib \
                $(ROOTSYS)/lib/libGpad.lib $(ROOTSYS)/lib/libTree.lib \
                $(ROOTSYS)/lib/libRint.lib $(ROOTSYS)/lib/libPostscript.lib \
                $(ROOTSYS)/lib/libMatrix.lib $(ROOTSYS)/lib/libPhysics.lib \
!IF EXISTS("$(ROOTSYS)/lib/libNet.lib ")
                $(ROOTSYS)/lib/libNet.lib \
!ENDIF
!IF EXISTS("$(ROOTSYS)/lib/libRIO.lib ")
                 $(ROOTSYS)/lib/libRIO.lib
!ENDIF  
#

LIBS          = $(ROOTLIBS)
GLIBS         = $(LIBS) $(ROOTSYS)/lib/libGui.lib $(ROOTSYS)/lib/libGraf.lib \
                $(ROOTSYS)/lib/libGpad.lib
LIBSALL       = $(ROOTLIBS) 
