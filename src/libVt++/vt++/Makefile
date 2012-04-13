#------------------------------------------------------------------------------
include ../../config/ProjectDef.mk
include ../../config/RootDef.mk
#------------------------------------------------------------------------------
SrcSuf        = C

NAME          = vt

headers  = VtVector.hh VtMatrix.hh VtNegMatrix.hh VtSqMatrix.hh VtSymMatrix.hh \
           CMatrix.hh VtIni.hh VtTrack.hh VtRelation.hh VtKalman.hh VtMassC.hh \
           VtRelationList.hh VtVertex.hh VtDistance.hh

# TARGETSO      = $(LIB_DIR)/lib$(NAME).$(DllSuf)

# TargetDef.linux.mk
#
# 

# FEDRA MACRO DEFINITIONS

PROJECT_LIBS   += -L$(LIB_DIR)

CXXFLAGS       += -I$(INC_DIR) 
CPPFLAGS       += $(CXXFLAGS)

depfile         = .depfile

#------------------------------------------------------------------------------

ifdef NAME

LIB_NAME      = lib$(NAME)
CINT_NAME     = vtCint
LINKDEF_NAME  = cintdict/LinkDef.hh

LIB_TGT       = $(LIB_DIR)/$(LIB_NAME).$(DllSuf)
classes	      = $(headers:.hh=.$(SrcSuf))

CINT     = include/VtVector.hh      include/CMatrix.hh        \
           include/VtMatrix.hh      include/VtNegMatrix.hh    \
           include/VtSqMatrix.hh    include/VtSymMatrix.hh    \
           include/VtIni.hh 	    include/VtTrack.hh        \
           include/VtRelation.hh    include/VtKalman.hh       \
           include/VtMassC.hh       include/VtRelationList.hh \
           include/VtVertex.hh      include/VtDistance.hh     \
           $(LINKDEF_NAME)

SRC           = $(classes) $(CINT_NAME).$(SrcSuf)
OBJ           = $(SRC:.$(SrcSuf)=.$(ObjSuf))

$(LIB_TGT): $(OBJ)
	$(CXX) -O -shared $(LIBS) -o $(LIB_TGT) $(OBJ)

$(CINT_NAME).cxx: $(CINT)
	@echo "Generating dictionary $@..."
	rootcint -f $(CINT_NAME).cxx -c -I$(INC_DIR) $(CINT)

$(CINT_NAME).o: $(CINT_NAME).cxx
	@echo "compile cint $@..."
	$(CXX) $(CXXFLAGS)  -DCINTOBJECT -c $(CINT_NAME).cxx

clean:
	@rm -fr core *.bak *.o $(LIB_TGT) *.log *.out *Cint.cxx *Cint.h *~ $(depfile) .*.bak

check:
	@if [ -e $(LIB_TGT) ]; then \
	  echo "lib$(NAME)...ok";\
	else\
	  echo "lib$(NAME)...ERROR!";\
	fi

endif

#------------------------------------------------------------------------------

ifdef TARGET

all: $(TARGET)

clean:
	@rm -fr core *.bak .*bak *.o *.log *.out $(TARGET) *~ $(depfile)

check:
	@for appl in $(TARGET) ; do \
	  if [ -e $${appl} ] ; then \
	    echo "$${appl}...ok";\
	  else\
	    echo "$${appl}...ERROR!";\
	  fi done

endif

#------------------------------------------------------------------------------

dep:
	@makedepend $(ROOTCFLAGS) *.cxx >& /dev/null

depend:
	@if [ ! -f $(depfile) ]; then \
	  echo "--> Make depend"; \
	  touch $(depfile); \
	  makedepend -f $(depfile) $(ROOTCFLAGS) *.cxx 2>/dev/null; \
	fi

$(BIN_DIR)/% : ./%.$(ObjSuf)
	$(LD) $< $(LIBS) $(PROJECT_LIBS) -o $@

.SUFFIXES: .$(SrcSuf)

.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $<

ifeq ($(depfile), $(wildcard $(depfile)))
include $(depfile)
endif
