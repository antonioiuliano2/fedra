#------------------------------------------------------------------------------
include ../config/ProjectDef.mk
include ../config/RootDef.mk
#------------------------------------------------------------------------------

PROJECT_LIBS += -lEdb -lEdr

LIB_TGT       = $(LIB_DIR)/libEIO.so

headers       = EdbRunAccess.h EdbLayer.h EdbDataSet.h

classes	      = $(headers:.h=.$(SrcSuf))
EIOCint	      = $(headers) EIOLinkDef.h
SRC           = $(classes) EIOCint.$(SrcSuf)
OBJ           = $(SRC:.$(SrcSuf)=.$(ObjSuf))

CXXFLAGS += -I$(INC_DIR)

#------------------------------------------------------------------------------

$(LIB_TGT): $(OBJ)
	$(CXX) -O -shared $(LIBS) -o $(LIB_TGT) $(OBJ)

EIOCint.cxx: $(EIOCint)
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

EdbDataSet.o: /var/local/root/include/TMatrix.h
EdbDataSet.o: /var/local/root/include/TVector.h
EdbDataSet.o: /var/local/root/include/TObject.h
EdbDataSet.o: /var/local/root/include/Rtypes.h
EdbDataSet.o: /var/local/root/include/RConfig.h
EdbDataSet.o: /var/local/root/include/RVersion.h
EdbDataSet.o: /var/local/root/include/DllImport.h
EdbDataSet.o: /var/local/root/include/Rtypeinfo.h
EdbDataSet.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
EdbDataSet.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
EdbDataSet.o: /usr/include/stdio.h /usr/include/features.h
EdbDataSet.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
EdbDataSet.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
EdbDataSet.o: /usr/include/bits/types.h /usr/include/libio.h
EdbDataSet.o: /usr/include/_G_config.h /usr/include/wchar.h
EdbDataSet.o: /usr/include/bits/wchar.h /usr/include/gconv.h
EdbDataSet.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
EdbDataSet.o: /usr/include/bits/stdio_lim.h
EdbDataSet.o: /var/local/root/include/TGenericClassInfo.h
EdbDataSet.o: /var/local/root/include/Varargs.h
EdbDataSet.o: /var/local/root/include/TStorage.h
EdbDataSet.o: /var/local/root/include/Riosfwd.h
EdbDataSet.o: /var/local/root/include/TBuffer.h
EdbDataSet.o: /var/local/root/include/Bytes.h /usr/include/string.h
EdbDataSet.o: /var/local/root/include/TMath.h
EdbDataSet.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
EdbDataSet.o: /var/local/root/include/TError.h
EdbDataSet.o: /var/local/root/include/TString.h
EdbDataSet.o: /var/local/root/include/TRefCnt.h
EdbDataSet.o: /var/local/root/include/TMatrixUtils.h EdbDataSet.h
EdbDataSet.o: /var/local/root/include/TObjArray.h
EdbDataSet.o: /var/local/root/include/TSeqCollection.h
EdbDataSet.o: /var/local/root/include/TCollection.h
EdbDataSet.o: /var/local/root/include/TIterator.h
EdbDataSet.o: /var/local/root/include/TFile.h
EdbDataSet.o: /var/local/root/include/TDirectory.h
EdbDataSet.o: /var/local/root/include/TNamed.h
EdbDataSet.o: /var/local/root/include/TList.h
EdbDataSet.o: /var/local/root/include/TDatime.h
EdbDataSet.o: /var/local/root/include/Htypes.h
EdbDataSet.o: /var/local/root/include/TUUID.h
EdbDataSet.o: /var/local/root/include/TInetAddress.h
EdbDataSet.o: /var/local/root/include/TCache.h
EdbDataSet.o: /var/local/root/include/THashList.h EdbLayer.h
EdbLayer.o: EdbLayer.h
EdbRunAccess.o: EdbRunAccess.h EdbLayer.h /var/local/root/include/TSystem.h
EdbRunAccess.o: /usr/include/stdio.h /usr/include/features.h
EdbRunAccess.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
EdbRunAccess.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
EdbRunAccess.o: /usr/include/bits/types.h /usr/include/libio.h
EdbRunAccess.o: /usr/include/_G_config.h /usr/include/wchar.h
EdbRunAccess.o: /usr/include/bits/wchar.h /usr/include/gconv.h
EdbRunAccess.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
EdbRunAccess.o: /usr/include/bits/stdio_lim.h /var/local/root/include/G__ci.h
EdbRunAccess.o: /usr/include/math.h /usr/include/bits/huge_val.h
EdbRunAccess.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
EdbRunAccess.o: /usr/include/string.h /usr/include/stdlib.h
EdbRunAccess.o: /usr/include/signal.h /usr/include/bits/sigset.h
EdbRunAccess.o: /usr/include/bits/signum.h /usr/include/assert.h
EdbRunAccess.o: /usr/include/limits.h
EdbRunAccess.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
EdbRunAccess.o: /usr/include/setjmp.h /usr/include/bits/setjmp.h
EdbRunAccess.o: /usr/include/ctype.h /usr/include/endian.h
EdbRunAccess.o: /usr/include/bits/endian.h /usr/include/fcntl.h
EdbRunAccess.o: /usr/include/bits/fcntl.h /usr/include/sys/types.h
EdbRunAccess.o: /usr/include/time.h /usr/include/unistd.h
EdbRunAccess.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
EdbRunAccess.o: /var/local/root/include/TNamed.h
EdbRunAccess.o: /var/local/root/include/TList.h
EdbRunAccess.o: /var/local/root/include/TString.h
EdbRunAccess.o: /var/local/root/include/TRefCnt.h
EdbRunAccess.o: /var/local/root/include/TInetAddress.h
EdbRunAccess.o: /var/local/root/include/TTimer.h
EdbRunAccess.o: /var/local/root/include/TSysEvtHandler.h
EdbRunAccess.o: /var/local/root/include/TObject.h
EdbRunAccess.o: /var/local/root/include/Rtypes.h
EdbRunAccess.o: /var/local/root/include/RConfig.h
EdbRunAccess.o: /var/local/root/include/RVersion.h
EdbRunAccess.o: /var/local/root/include/DllImport.h
EdbRunAccess.o: /var/local/root/include/Rtypeinfo.h
EdbRunAccess.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
EdbRunAccess.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
EdbRunAccess.o: /var/local/root/include/TGenericClassInfo.h
EdbRunAccess.o: /var/local/root/include/Varargs.h
EdbRunAccess.o: /var/local/root/include/TStorage.h
EdbRunAccess.o: /var/local/root/include/Riosfwd.h
EdbRunAccess.o: /var/local/root/include/TBuffer.h
EdbRunAccess.o: /var/local/root/include/Bytes.h
EdbRunAccess.o: /var/local/root/include/TQObject.h
EdbRunAccess.o: /var/local/root/include/TClass.h
EdbRunAccess.o: /var/local/root/include/TDictionary.h
EdbRunAccess.o: /var/local/root/include/Property.h
EdbRunAccess.o: /var/local/root/include/TTime.h
