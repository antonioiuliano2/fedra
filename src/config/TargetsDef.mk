#------------------------------------------------------------------------------
LIB_NAME      = lib$(NAME)
CINT_NAME     = $(NAME)Cint
LINKDEF_NAME  = $(NAME)LinkDef

LIB_TGT       = $(LIB_DIR)/$(LIB_NAME).$(DllSuf)
classes	      = $(headers:.h=.$(SrcSuf))
CINT          = $(headers) $(LINKDEF_NAME).h
SRC           = $(classes) $(CINT_NAME).$(SrcSuf)
OBJ           = $(SRC:.$(SrcSuf)=.$(ObjSuf))
#------------------------------------------------------------------------------

ifeq ($(ARCH),linux)
# vvvvvvvvvvvvvvvvvvvvvvvv-- Linux with egcs --vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

CXXFLAGS      += -I$(INC_DIR)
depfile  = .depfile

$(LIB_TGT): $(OBJ)
	$(CXX) -O -shared $(LIBS) -o $(LIB_TGT) $(OBJ)

$(CINT_NAME).cxx: $(CINT)
	@echo "Generating dictionary $@..."
	rootcint   -f $@ -c -I$(INC_DIR) $^

clean:
	@rm -fr core *.bak *.o $(LIB_TGT) *.log *.out *Cint.cxx *Cint.h *~ $(depfile) .*.bak

dep:
	@makedepend $(ROOTCFLAGS) *.cxx >& /dev/null

depend:
	@if [ ! -f $(depfile) ]; then \
	  echo "--> Make depend"; \
	  touch $(depfile); \
	  makedepend -f $(depfile) $(ROOTCFLAGS) *.cxx 2>/dev/null; \
	fi

.SUFFIXES: .$(SrcSuf)

.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $<

ifeq ($(depfile), $(wildcard $(depfile)))
include $(depfile)
endif

#^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^-- end --^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
endif


ifeq ($(ARCH),win32)
# vvvvvvvvvvvvvvvvv-- Windows with the VC++ compiler --vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

CXXFLAGS      =$(CXXFLAGS) -I$(INC_DIR)

all: $(LIB_TGT)

$(LIB_TGT): $(OBJ)
    BINDEXPLIB  $* $(OBJ) > $*.def
    lib /nologo /MACHINE:IX86 $(OBJ) /def:$*.def      $(OutPutOpt)$(LIB_TGT:.dll=.lib)
    $(LD) $(DLLFLAGS) $(LDDEBUG) $(PROJECT_LIBS) $(OBJ) $*.exp $(LIBS) $(OutPutOpt)$(LIB_TGT)

$(CINT_NAME).cxx: $(CINT)
    @echo "Generating dictionary Cint..."
    rootcint -f $(CINT_NAME).cxx -c -I$(INC_DIR) $(CINT)

clean:
    del /q $(OBJ) *Cint.* *.pdb *.def "$(LIB_DIR)"\$(LIB_NAME).*

depend:
    @makedepend -I$(INCLUDE) *.cxx >& /dev/null

.SUFFIXES: .$(SrcSuf)

.$(SrcSuf).$(ObjSuf):
    $(CXX) $(CXXFLAGS) -c $<


# ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^-- end --^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
endif
