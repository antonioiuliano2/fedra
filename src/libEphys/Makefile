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

threadEdbPhys.o: /var/local/root/include/TMath.h
threadEdbPhys.o: /var/local/root/include/Rtypes.h
threadEdbPhys.o: /var/local/root/include/RConfig.h
threadEdbPhys.o: /var/local/root/include/RVersion.h
threadEdbPhys.o: /var/local/root/include/DllImport.h
threadEdbPhys.o: /var/local/root/include/Rtypeinfo.h
threadEdbPhys.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
threadEdbPhys.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
threadEdbPhys.o: /usr/include/stdio.h /usr/include/features.h
threadEdbPhys.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
threadEdbPhys.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
threadEdbPhys.o: /usr/include/bits/types.h /usr/include/libio.h
threadEdbPhys.o: /usr/include/_G_config.h /usr/include/wchar.h
threadEdbPhys.o: /usr/include/bits/wchar.h /usr/include/gconv.h
threadEdbPhys.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
threadEdbPhys.o: /usr/include/bits/stdio_lim.h
threadEdbPhys.o: /var/local/root/include/TGenericClassInfo.h
threadEdbPhys.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
threadEdbPhys.o: EdbPhys.h /var/local/root/include/TObject.h
threadEdbPhys.o: /var/local/root/include/Varargs.h
threadEdbPhys.o: /var/local/root/include/TStorage.h
threadEdbPhys.o: /var/local/root/include/Riosfwd.h
threadEdbPhys.o: /var/local/root/include/TBuffer.h
threadEdbPhys.o: /var/local/root/include/Bytes.h /usr/include/string.h
threadEdbPhys.o: /var/local/root/include/TRandom.h
threadEdbPhys.o: /var/local/root/include/TNamed.h
threadEdbPhys.o: /var/local/root/include/TList.h
threadEdbPhys.o: /var/local/root/include/TSeqCollection.h
threadEdbPhys.o: /var/local/root/include/TCollection.h
threadEdbPhys.o: /var/local/root/include/TIterator.h
threadEdbPhys.o: /var/local/root/include/TString.h
threadEdbPhys.o: /var/local/root/include/TRefCnt.h
