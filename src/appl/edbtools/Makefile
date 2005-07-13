#------------------------------------------------------------------------------
 include $(FEDRA_ROOT)/src/config/ProjectDef.mk
 include $(FEDRA_ROOT)/src/config/RootDef.mk
#------------------------------------------------------------------------------
PROJECT_LIBS = -L$(LIB_DIR) -lEdb

CXXFLAGS  += -I$(INC_DIR)

edbtools: edbtools.o
	$(LD) $^ $(LIBS) $(PROJECT_LIBS) -o $(BIN_DIR)/edbtools;

clean:
	@rm -fr core *.bak *.o *.log *.out $(BIN_DIR)/edbtools*~

