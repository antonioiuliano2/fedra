#------------------------------------------------------------------------------
include ../config/ProjectDef.mk
include ../config/RootDef.mk
#------------------------------------------------------------------------------

PROJECT_LIBS += 

LIB_TGT       = $(LIB_DIR)/libEphys.so

headers       = EdbPhys.h

classes	      = $(headers:.h=.$(SrcSuf))
EphysCint     = $(headers) EphysLinkDef.h
SRC           = $(classes) EphysCint.$(SrcSuf)
OBJ           = $(SRC:.$(SrcSuf)=.$(ObjSuf))

CXXFLAGS += -I$(INC_DIR)

#------------------------------------------------------------------------------

$(LIB_TGT): $(OBJ)
	$(CXX) -O -shared $(LIBS) -o $(LIB_TGT) $(OBJ)

EphysCint.cxx: $(EphysCint)
	@echo "Generating dictionary $@..."
	rootcint -f $@ -c -I$(INC_DIR) $^

clean:
	@rm -fr core *.bak *.o $(LIB_TGT) *.log *.out *Cint.cxx *Cint.h *~

depend:
	@makedepend $(ROOTCFLAGS) *.cxx >& /dev/null

.SUFFIXES: .$(SrcSuf)

.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $<

# DO NOT DELETE

EdbPhys.o: EdbPhys.h /var/local/root/include/TObject.h
EdbPhys.o: /var/local/root/include/Rtypes.h /var/local/root/include/RConfig.h
EdbPhys.o: /var/local/root/include/RVersion.h
EdbPhys.o: /var/local/root/include/DllImport.h
EdbPhys.o: /var/local/root/include/Rtypeinfo.h
EdbPhys.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
EdbPhys.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
EdbPhys.o: /usr/include/stdio.h /usr/include/features.h
EdbPhys.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
EdbPhys.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
EdbPhys.o: /usr/include/bits/types.h /usr/include/libio.h
EdbPhys.o: /usr/include/_G_config.h /usr/include/wchar.h
EdbPhys.o: /usr/include/bits/wchar.h /usr/include/gconv.h
EdbPhys.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
EdbPhys.o: /usr/include/bits/stdio_lim.h
EdbPhys.o: /var/local/root/include/TGenericClassInfo.h
EdbPhys.o: /var/local/root/include/Varargs.h
EdbPhys.o: /var/local/root/include/TStorage.h
EdbPhys.o: /var/local/root/include/Riosfwd.h
EdbPhys.o: /var/local/root/include/TBuffer.h /var/local/root/include/Bytes.h
EdbPhys.o: /usr/include/string.h
