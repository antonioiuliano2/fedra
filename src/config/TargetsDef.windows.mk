# TargetDef.windows.mk
# author: G. Sirri - INFN BO
#
# the following code is partially based on $(ROOTSYS)\test\makefile.win32, 

# FEDRA MACRO DEFINITIONS

PROJECT_LIBS  = $(PROJECT_LIBS) -LIBPATH:$(LIB_DIR)
PROJECT_LIBS  = $(PROJECT_LIBS) ole32.lib  # ole32 needed by libDataConversion

CXXFLAGS      = $(CXXFLAGS)  -D_USESYSAL -DUSE_ROOT

#------------------------------------------------------------------------------

!IF DEFINED(NAME)

OBJS          = $(headers:.h=.obj) $(NAME)Dict.$(ObjSuf)
TARGETSO      = $(LIB_DIR)/lib$(NAME).$(DllSuf)

all: $(TARGETSO)

$(TARGETSO):  $(OBJS)
    BINDEXPLIB  $* $(OBJS) > $*.def
    lib -nologo -MACHINE:IX86 $(OBJS) -def:$*.def $(OutPutOpt)$(@:.dll=.lib)
    $(LD) $(SOFLAGS) $(LDFLAGS) $(OBJS) $*.exp $(LIBS) $(PROJECT_LIBS:-l=/DEFAULTLIB:lib) $(OutPutOpt)$@
    $(MT_DLL)
    @echo "$@ done"

clean:
    for %F IN ($(OBJS) *Dict.* *.pdb *.def "$(LIB_DIR)"\lib$(NAME).* ) DO IF EXIST "%F" del /q "%F"

!ENDIF

#------------------------------------------------------------------------------

!IF DEFINED(TARGET)

all: $(TARGET)

$(TARGET) : $(*B).obj 
#   $(LD) $** $(LDFLAGS) $(LIBS) $(PROJECT_LIBS:-l=/DEFAULTLIB:lib) $(OutPutOpt)$@
#   $(MT_EXE)
#   @echo "$@ done"

clean:
    @for %F in ($(TARGET)) DO FOR %F IN ( %~nF.obj "$(BIN_DIR)"\%~nF.exe "$(BIN_DIR)"\%~nF.exe.manifest ) DO IF EXIST %F del /q %F

!ENDIF

#------------------------------------------------------------------------------

distclean:      clean
       @del *.exe *.root *.ps *.lib *.dll

{.}.obj{$(BIN_DIR)}.exe:
   $(LD) $** $(LDFLAGS) $(LIBS) $(PROJECT_LIBS:-l=/DEFAULTLIB:lib) $(OutPutOpt)$@
   $(MT_EXE)
   @echo "$@ done"

###
.$(ObjSuf): .$(SrcSuf)

# -p option to request the use of the compiler's preprocessor
# instead of CINT's preprocessor.  This is useful to handle header
# files with macro construct not handled by CINT.
#
$(NAME)Dict.$(SrcSuf): $(headers) $(NAME)LinkDef.h
   @echo "Generating dictionary $@..."
   rootcint -f $@ -c -p -I$(INC_DIR) $(headers) $(NAME)LinkDef.h 

.cxx.$(ObjSuf):
        $(CXX) $(CXXFLAGS) $(EXTRAFLAGS) $(CXXOPT) -I$(INC_DIR) -c $<

.cpp.$(ObjSuf):
        $(CXX) $(CXXFLAGS) $(EXTRAFLAGS) $(CXXOPT) -I$(INC_DIR) -c $<

# /TP compile all files as C++ even if they have *.C extension
.C.$(ObjSuf):
        $(CXX) $(CXXFLAGS) $(EXTRAFLAGS) $(CXXOPT) -I$(INC_DIR) -TP -c $<

