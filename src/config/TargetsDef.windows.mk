# TargetDef.windows.mk
# author: G. Sirri - INFN BO
#
# the following code is partially based on $(ROOTSYS)\test\makefile.win32, 

# FEDRA MACRO DEFINITIONS

PROJECT_LIBS  = $(PROJECT_LIBS) -LIBPATH:$(LIB_DIR)
PROJECT_LIBS  = $(PROJECT_LIBS) ole32.lib  # ole32 needed by libDataConversion

CXXFLAGS      = $(CXXFLAGS)  -D_USESYSAL -DUSE_ROOT

#------------------------------------------------------------------------------
!IF !DEFINED(EXTRAOBJS)
EXTRAOBJS   =
!ENDIF
#------------------------------------------------------------------------------

!IF DEFINED(NAME)

OBJS          = $(headers:.h=.obj) $(NAME)Dict.$(ObjSuf) $(EXTRAOBJS)
TARGETSO      = $(LIB_DIR)/lib$(NAME).$(DllSuf)

!ELSE

OBJS         =
TARGETSO     =

!ENDIF

#------------------------------------------------------------------------------

!IF DEFINED(TARGET)

$(TARGET) : $(*B).obj $(EXTRAOBJS) $(TARGETSO)

!ELSE

TARGET    =

!ENDIF

#------------------------------------------------------------------------------

all: $(TARGETSO) $(TARGET)

clean:
	@for %F IN ($(OBJS) *Dict.* *.pdb ) DO IF EXIST "%F" del /q "%F"
	@for %F in ($(TARGETSO)) DO @FOR %F IN ( %~dpnF.def %~dpnF.exp %~dpnF.lib %~dpnF.dll.*  ) DO IF EXIST "%F" del /q "%F"
	@for %F in ($(TARGET))   DO @FOR %F IN ( %~nF.obj %~dpnF.exe.* ) DO IF EXIST "%F" del /q "%F"

distclean:      clean
	@del *.exe *.root *.ps *.lib *.dll

check:
	@for %F in ($(TARGET))   DO @IF EXIST %F ( @ECHO bin\%~nxF...ok ) ELSE ( @ECHO bin\%~nxF...ERROR! )
	@for %F in ($(TARGETSO)) DO @IF NOT EXIST %~dpnF.lib @IF NOT EXIST %F @ECHO lib\%%~nF.lib...ERROR!"		lib\%%~nF.dll...ERROR!
	@for %F in ($(TARGETSO)) DO @IF NOT EXIST %~dpnF.lib @IF     EXIST %F @ECHO lib\%%~nF.lib...ERROR!"		lib\%%~nF.dll...ok
	@for %F in ($(TARGETSO)) DO @IF     EXIST %~dpnF.lib @IF NOT EXIST %F @ECHO lib\%%~nF.lib...ok"    		lib\%%~nF.dll...ERROR!
	@for %F in ($(TARGETSO)) DO @IF     EXIST %~dpnF.lib @IF     EXIST %F @ECHO lib\%%~nF.lib...ok"    		lib\%%~nF.dll...ok

#	@for %F in ($(TARGETSO:.dll=.lib)) DO @IF EXIST %F ( @ECHO lib\%~nxF...ok ) ELSE ( @ECHO lib\%~nxF...ERROR! )
#	@for %F in ($(TARGETSO))           DO @IF EXIST %F ( @ECHO lib\%~nxF...ok ) ELSE ( @ECHO lib\%~nxF...ERROR! )

###

!IF DEFINED(NAME)
$(TARGETSO):  $(OBJS)
	BINDEXPLIB  $* $(OBJS) > $*.def
	lib -nologo -MACHINE:IX86 $(OBJS) -def:$*.def $(OutPutOpt)$(@:.dll=.lib)
	$(LD) $(SOFLAGS) $(LDFLAGS) $(OBJS) $*.exp $(LIBS) $(PROJECT_LIBS:-l=/DEFAULTLIB:lib) $(OutPutOpt)$@
	$(MT_DLL)
	@echo "$@ done"
!ENDIF

# -p option to request the use of the compiler's preprocessor
# instead of CINT's preprocessor.  This is useful to handle header
# files with macro construct not handled by CINT.
#
$(NAME)Dict.$(SrcSuf): $(headers) $(NAME)LinkDef.h
   @echo "Generating dictionary $@..."
   rootcint -f $@ -c -p -I$(INC_DIR) $(headers) $(NAME)LinkDef.h 

# - - - - - - - -

{.}.obj{$(BIN_DIR)}.exe:
!IF DEFINED(NAME)
   $(LD) $(*B).obj $(EXTRAOBJS) $(LDFLAGS) $(LIBS) $(PROJECT_LIBS:-l=/DEFAULTLIB:lib) /DEFAULTLIB:lib$(NAME) $(OutPutOpt)$@
!ELSE
   $(LD) $** $(LDFLAGS) $(LIBS) $(PROJECT_LIBS:-l=/DEFAULTLIB:lib) $(OutPutOpt)$@
!ENDIF
   $(MT_EXE)
   @echo "$@ done"

###

.$(ObjSuf): .$(SrcSuf)

.cxx.$(ObjSuf):
        $(CXX) $(CXXFLAGS) $(EXTRAFLAGS) $(CXXOPT) -I$(INC_DIR) -c $<

.cpp.$(ObjSuf):
        $(CXX) $(CXXFLAGS) $(EXTRAFLAGS) $(CXXOPT) -I$(INC_DIR) -c $<

# /TP compile all files as C++ even if they have *.C extension
.C.$(ObjSuf):
        $(CXX) $(CXXFLAGS) $(EXTRAFLAGS) $(CXXOPT) -I$(INC_DIR) -TP -c $<
