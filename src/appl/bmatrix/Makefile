#------------------------------------------------------------------------------
include ../../config/ProjectDef.mk
include ../../config/RootDef.mk
#------------------------------------------------------------------------------

PROJECT_LIBS += -lEdb 

LIB_TGT       = $(LIB_DIR)/bmatrix.so

headers       = bitmatrix.h basematrix.h bytematrix.h bitview.h

classes	      = $(headers:.h=.$(SrcSuf))
bitmatrixCint	      = $(headers) bitmatrixCint
SRC           = $(classes) bitmatrixCint.$(SrcSuf)
OBJ           = $(SRC:.$(SrcSuf)=.$(ObjSuf))

CXXFLAGS += -I$(INC_DIR)

#------------------------------------------------------------------------------

$(LIB_TGT): $(OBJ)
	$(CXX) -O -shared $(LIBS) -o $(LIB_TGT) $(OBJ)

bitmatrixCint.cxx: $(bitmatrixCint)
	@echo "Generating dictionary $@..."
	rootcint   -f $@ -c -I$(INC_DIR) $^

clean:
	@rm -fr core *.bak *.o $(LIB_TGT) *.log *.out *Cint.cxx *Cint.h *~

depend:
	@makedepend $(ROOTCFLAGS) *.cxx >& /dev/null

.SUFFIXES: .$(SrcSuf)

.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $<

# DO NOT DELETE

#############threadEdbDataSet.o: /var/local/root/include/TMatrix.h

