ARCH         := $(shell root-config --arch)
PLATFORM     := $(shell root-config --platform)

ObjSuf        = o
SrcSuf        = cxx
ExeSuf        =
DllSuf        = so
OutPutOpt     = -o

ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs)
ROOTGLIBS     = $(shell root-config --glibs)

ifeq ($(ARCH),linux)
# Linux with egcs
CXX           = g++
CXXFLAGS      = -g -O -Wall -fPIC -DUSE_ROOT
LD            = g++
LDFLAGS       = -g
SOFLAGS       = -shared
endif

ifeq ($(ARCH),win32)
# Windows with the VC++ compiler
ObjSuf        = obj
SrcSuf        = cxx
ExeSuf        = .exe
DllSuf        = dll
OutPutOpt     = -out:
CXX           = cl
CXXOPT        = -O2
#CXXOPT        = -Z7
CXXFLAGS      = $(CXXOPT) -G5 -GR -GX -MD -DWIN32 -D_WINDOWS -nologo \
                -DVISUAL_CPLUSPLUS -D_X86_=1 -D_DLL
LD            = link
LDOPT         = -opt:ref
#LDOPT         = -debug
LDFLAGS       = $(LDOPT) -pdb:none -nologo
SOFLAGS       = -DLL

ROOTLIBS     := $(shell root-config --nonew --libs)
ROOTGLIBS    := $(shell root-config --nonew --glibs)
EXPLLINKLIBS  = $(ROOTLIBS) $(ROOTGLIBS)
endif

CXXFLAGS     += $(ROOTCFLAGS)
LIBS          = $(ROOTLIBS)
GLIBS         = $(ROOTGLIBS)

