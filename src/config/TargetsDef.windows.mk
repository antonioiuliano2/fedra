#------------------------------------------------------------------------------
LIB_NAME      = lib$(NAME)
CINT_NAME     = $(NAME)Cint
LINKDEF_NAME  = $(NAME)LinkDef

LIB_TGT       = $(LIB_DIR)/$(LIB_NAME).$(DllSuf)
#classes	  = $(headers:.h=.$(SrcSuf))
classes   = $(headers:.h=.cxx)
CINT          = $(headers) $(LINKDEF_NAME).h
SRC           = $(classes) $(CINT_NAME).$(SrcSuf)
#OBJ           = $(SRC:.$(SrcSuf)=.$(ObjSuf))
OBJ       = $(SRC:.cxx=.obj)

#------------------------------------------------------------------------------

CXXFLAGS      = $(CXXFLAGS) -I$(INC_DIR)

$(LIB_TGT): $(OBJ)
    BINDEXPLIB  $* $(OBJ) > $*.def
    lib /nologo /MACHINE:IX86 $(OBJ) /def:$*.def      $(OutPutOpt)$(LIB_TGT:.dll=.lib)
    $(LD) $(DLLFLAGS) $(LDDEBUG) $(PROJECT_LIBS:-l=/DEFAULTLIB:lib) $(OBJ) $*.exp $(LIBS) $(OutPutOpt)$(LIB_TGT)

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