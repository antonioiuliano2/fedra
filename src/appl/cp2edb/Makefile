#------------------------------------------------------------------------------
 include $(FEDRA_ROOT)/src/config/ProjectDef.mk
 include $(FEDRA_ROOT)/src/config/RootDef.mk
#------------------------------------------------------------------------------
PROJECT_LIBS = -L$(LIB_DIR) -lEdb  -lEdd  -lEdr  -lEmath  -lEphys  -lvt -lEIO

CXXFLAGS  += -I$(INC_DIR)

cp2edb: cp2edb.o
	$(LD) $^ $(LIBS) $(PROJECT_LIBS) -o $(BIN_DIR)/cp2edb;

clean:
	@rm -fr core *.bak *.o *.log *.out $(BIN_DIR)/cp2edb*~

