#------------------------------------------------------------------------------
LIB_NAME      = lib$(NAME)
CINT_NAME     = $(NAME)Cint
LINKDEF_NAME  = $(NAME)LinkDef

LIB_TGT       = $(LIB_DIR)/$(LIB_NAME).$(DllSuf)
classes	      = $(headers:.h=.$(SrcSuf))
CINT          = $(headers) $(LINKDEF_NAME).h
SRC           = $(classes) $(CINT_NAME).$(SrcSuf)
OBJ           = $(SRC:.$(SrcSuf)=.$(ObjSuf))
CXXFLAGS     += -I$(INC_DIR)
#------------------------------------------------------------------------------

$(LIB_TGT): $(OBJ)
	$(CXX) -O -shared $(LIBS) -o $(LIB_TGT) $(OBJ)

$(CINT_NAME).cxx: $(CINT)
	@echo "Generating dictionary $@..."
	rootcint   -f $@ -c -I$(INC_DIR) $^

clean:
	@rm -fr core *.bak *.o $(LIB_TGT) *.log *.out *Cint.cxx *Cint.h *~

depend:
	@makedepend $(ROOTCFLAGS) *.cxx >& /dev/null

.SUFFIXES: .$(SrcSuf)

.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $<
