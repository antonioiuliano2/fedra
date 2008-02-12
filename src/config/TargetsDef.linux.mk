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
CINT_NAME     = $(NAME)Cint
LINKDEF_NAME  = $(NAME)LinkDef

LIB_TGT       = $(LIB_DIR)/$(LIB_NAME).$(DllSuf)
classes	      = $(headers:.h=.$(SrcSuf))
CINT          = $(headers) $(LINKDEF_NAME).h
SRC           = $(classes) $(CINT_NAME).$(SrcSuf)
OBJ           = $(SRC:.$(SrcSuf)=.$(ObjSuf))

$(LIB_TGT): $(OBJ)
	$(CXX) -O -shared $(LIBS) -o $(LIB_TGT) $(OBJ)

$(CINT_NAME).cxx: $(CINT)
	@echo "Generating dictionary $@..."
	rootcint   -f $@ -c -p -I$(INC_DIR) $^

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

