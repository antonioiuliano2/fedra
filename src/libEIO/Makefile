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

threadEdbDataSet.o: /var/local/root/include/TMatrix.h
threadEdbDataSet.o: /var/local/root/include/TMatrixF.h
threadEdbDataSet.o: /var/local/root/include/TMatrixFBase.h
threadEdbDataSet.o: /var/local/root/include/TROOT.h
threadEdbDataSet.o: /var/local/root/include/TDirectory.h
threadEdbDataSet.o: /var/local/root/include/TNamed.h
threadEdbDataSet.o: /var/local/root/include/TObject.h
threadEdbDataSet.o: /var/local/root/include/Rtypes.h
threadEdbDataSet.o: /var/local/root/include/RConfig.h
threadEdbDataSet.o: /var/local/root/include/RVersion.h
threadEdbDataSet.o: /var/local/root/include/DllImport.h
threadEdbDataSet.o: /var/local/root/include/Rtypeinfo.h
threadEdbDataSet.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
threadEdbDataSet.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
threadEdbDataSet.o: /usr/include/stdio.h /usr/include/features.h
threadEdbDataSet.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
threadEdbDataSet.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
threadEdbDataSet.o: /usr/include/bits/types.h /usr/include/libio.h
threadEdbDataSet.o: /usr/include/_G_config.h /usr/include/wchar.h
threadEdbDataSet.o: /usr/include/bits/wchar.h /usr/include/gconv.h
threadEdbDataSet.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
threadEdbDataSet.o: /usr/include/bits/stdio_lim.h
threadEdbDataSet.o: /var/local/root/include/TGenericClassInfo.h
threadEdbDataSet.o: /var/local/root/include/Varargs.h
threadEdbDataSet.o: /var/local/root/include/TStorage.h
threadEdbDataSet.o: /var/local/root/include/Riosfwd.h
threadEdbDataSet.o: /var/local/root/include/TBuffer.h
threadEdbDataSet.o: /var/local/root/include/Bytes.h /usr/include/string.h
threadEdbDataSet.o: /var/local/root/include/TList.h
threadEdbDataSet.o: /var/local/root/include/TSeqCollection.h
threadEdbDataSet.o: /var/local/root/include/TCollection.h
threadEdbDataSet.o: /var/local/root/include/TIterator.h
threadEdbDataSet.o: /var/local/root/include/TString.h
threadEdbDataSet.o: /var/local/root/include/TMath.h
threadEdbDataSet.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
threadEdbDataSet.o: /var/local/root/include/TRefCnt.h
threadEdbDataSet.o: /var/local/root/include/TDatime.h
threadEdbDataSet.o: /var/local/root/include/Htypes.h
threadEdbDataSet.o: /var/local/root/include/TUUID.h
threadEdbDataSet.o: /var/local/root/include/TClass.h
threadEdbDataSet.o: /var/local/root/include/TDictionary.h
threadEdbDataSet.o: /var/local/root/include/Property.h
threadEdbDataSet.o: /var/local/root/include/TObjArray.h
threadEdbDataSet.o: /var/local/root/include/TStreamerInfo.h
threadEdbDataSet.o: /var/local/root/include/TFile.h
threadEdbDataSet.o: /var/local/root/include/TCache.h
threadEdbDataSet.o: /var/local/root/include/THashList.h
threadEdbDataSet.o: /var/local/root/include/TClonesArray.h
threadEdbDataSet.o: /var/local/root/include/TPluginManager.h
threadEdbDataSet.o: /var/local/root/include/TVirtualUtilHist.h
threadEdbDataSet.o: /var/local/root/include/TError.h
threadEdbDataSet.o: /var/local/root/include/TMatrixFUtils.h EdbDataSet.h
threadEdbDataSet.o: EdbLayer.h
threadEdbLayer.o: EdbLayer.h
threadEdbRunAccess.o: EdbRunAccess.h EdbLayer.h
threadEdbRunAccess.o: /var/local/root/include/TSystem.h /usr/include/stdio.h
threadEdbRunAccess.o: /usr/include/features.h /usr/include/sys/cdefs.h
threadEdbRunAccess.o: /usr/include/gnu/stubs.h
threadEdbRunAccess.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
threadEdbRunAccess.o: /usr/include/bits/types.h /usr/include/libio.h
threadEdbRunAccess.o: /usr/include/_G_config.h /usr/include/wchar.h
threadEdbRunAccess.o: /usr/include/bits/wchar.h /usr/include/gconv.h
threadEdbRunAccess.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
threadEdbRunAccess.o: /usr/include/bits/stdio_lim.h
threadEdbRunAccess.o: /var/local/root/include/G__ci.h /usr/include/math.h
threadEdbRunAccess.o: /usr/include/bits/huge_val.h
threadEdbRunAccess.o: /usr/include/bits/mathdef.h
threadEdbRunAccess.o: /usr/include/bits/mathcalls.h /usr/include/string.h
threadEdbRunAccess.o: /usr/include/stdlib.h /usr/include/signal.h
threadEdbRunAccess.o: /usr/include/bits/sigset.h /usr/include/bits/signum.h
threadEdbRunAccess.o: /usr/include/assert.h /usr/include/limits.h
threadEdbRunAccess.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
threadEdbRunAccess.o: /usr/include/setjmp.h /usr/include/bits/setjmp.h
threadEdbRunAccess.o: /usr/include/ctype.h /usr/include/endian.h
threadEdbRunAccess.o: /usr/include/bits/endian.h /usr/include/fcntl.h
threadEdbRunAccess.o: /usr/include/bits/fcntl.h /usr/include/sys/types.h
threadEdbRunAccess.o: /usr/include/time.h /usr/include/unistd.h
threadEdbRunAccess.o: /usr/include/bits/posix_opt.h
threadEdbRunAccess.o: /usr/include/bits/confname.h
threadEdbRunAccess.o: /var/local/root/include/TNamed.h
threadEdbRunAccess.o: /var/local/root/include/TObject.h
threadEdbRunAccess.o: /var/local/root/include/Rtypes.h
threadEdbRunAccess.o: /var/local/root/include/RConfig.h
threadEdbRunAccess.o: /var/local/root/include/RVersion.h
threadEdbRunAccess.o: /var/local/root/include/DllImport.h
threadEdbRunAccess.o: /var/local/root/include/Rtypeinfo.h
threadEdbRunAccess.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
threadEdbRunAccess.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
threadEdbRunAccess.o: /var/local/root/include/TGenericClassInfo.h
threadEdbRunAccess.o: /var/local/root/include/Varargs.h
threadEdbRunAccess.o: /var/local/root/include/TStorage.h
threadEdbRunAccess.o: /var/local/root/include/Riosfwd.h
threadEdbRunAccess.o: /var/local/root/include/TBuffer.h
threadEdbRunAccess.o: /var/local/root/include/Bytes.h
threadEdbRunAccess.o: /var/local/root/include/TList.h
threadEdbRunAccess.o: /var/local/root/include/TSeqCollection.h
threadEdbRunAccess.o: /var/local/root/include/TCollection.h
threadEdbRunAccess.o: /var/local/root/include/TIterator.h
threadEdbRunAccess.o: /var/local/root/include/TString.h
threadEdbRunAccess.o: /var/local/root/include/TMath.h
threadEdbRunAccess.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
threadEdbRunAccess.o: /var/local/root/include/TRefCnt.h
threadEdbRunAccess.o: /var/local/root/include/TInetAddress.h
threadEdbRunAccess.o: /var/local/root/include/TTimer.h
threadEdbRunAccess.o: /var/local/root/include/TSysEvtHandler.h
threadEdbRunAccess.o: /var/local/root/include/TQObject.h
threadEdbRunAccess.o: /var/local/root/include/TClass.h
threadEdbRunAccess.o: /var/local/root/include/TDictionary.h
threadEdbRunAccess.o: /var/local/root/include/Property.h
threadEdbRunAccess.o: /var/local/root/include/TObjArray.h
threadEdbRunAccess.o: /var/local/root/include/TStreamerInfo.h
threadEdbRunAccess.o: /var/local/root/include/TFile.h
threadEdbRunAccess.o: /var/local/root/include/TCache.h
threadEdbRunAccess.o: /var/local/root/include/THashList.h
threadEdbRunAccess.o: /var/local/root/include/TClonesArray.h
threadEdbRunAccess.o: /var/local/root/include/TTime.h
