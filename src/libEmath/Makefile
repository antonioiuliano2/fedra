#------------------------------------------------------------------------------
 include ../config/ProjectDef.mk
 include ../config/RootDef.mk
#------------------------------------------------------------------------------

LIB_TGT       = $(LIB_DIR)/libEmath.so

headers       = TIndexCell.h

classes       = $(headers:.h=.$(SrcSuf))
Cint          = $(headers) EmathLinkDef.h
SRC           = $(classes) EmathCint.$(SrcSuf)
OBJ           = $(SRC:.$(SrcSuf)=.$(ObjSuf))

#------------------------------------------------------------------------------

all: $(LIB_TGT)

includes: $(headers)
	cp $(headers) $(INC_DIR)

$(LIB_TGT): $(OBJ)
	$(CXX) -O -shared $(LIBS) -o $(LIB_TGT) $(OBJ)

EmathCint.cxx: $(Cint)
	rootcint -f EmathCint.cxx -c $(INCLUDE) $(Cint)

clean:
	@rm -fr core *.bak *.o $(LIB_TGT) *.log *.out *Cint.cxx *Cint.h *~

depend:
	@makedepend $(ROOTCFLAGS) *.cxx >& /dev/null

.SUFFIXES: .$(SrcSuf)

.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $<

# DO NOT DELETE
