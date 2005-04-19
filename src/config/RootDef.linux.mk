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

# ifeq ($(ARCH),linux)
# Linux with egcs
CXX           = g++ 
CXXFLAGS      = -g -O -Wall -fPIC -DUSE_ROOT
LD            = g++
LDFLAGS       = -g
SOFLAGS       = -shared
# endif

CXXFLAGS     += $(ROOTCFLAGS)
LIBS          = $(ROOTLIBS)
GLIBS         = $(ROOTGLIBS)
