#------------------------------------------------------------------------------
 include ../../config/ProjectDef.mk
 include ../../config/RootDef.mk
#------------------------------------------------------------------------------

PROJECT_LIBS = -L$(LIB_DIR) -lEdr -lEdb -lEmath

CXXFLAGS  += -I$(INC_DIR)

aln : aln.o
	$(LD) aln.o $(LIBS) $(ROOTLIBS) $(PROJECT_LIBS) -o aln;
clean: 
	rm *.o aln
