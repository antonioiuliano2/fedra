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

threadEIOCint.o: /var/local/root/include/RConfig.h
threadEIOCint.o: /var/local/root/include/RVersion.h EIOCint.h
threadEIOCint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
threadEIOCint.o: /usr/include/stdio.h /usr/include/features.h
threadEIOCint.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
threadEIOCint.o: /usr/include/bits/types.h /usr/include/libio.h
threadEIOCint.o: /usr/include/_G_config.h /usr/include/wchar.h
threadEIOCint.o: /usr/include/bits/wchar.h /usr/include/gconv.h
threadEIOCint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
threadEIOCint.o: /usr/include/bits/stdio_lim.h /usr/include/stdlib.h
threadEIOCint.o: /usr/include/math.h /usr/include/bits/huge_val.h
threadEIOCint.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
threadEIOCint.o: /usr/include/string.h /var/local/root/include/G__ci.h
threadEIOCint.o: /usr/include/signal.h /usr/include/bits/sigset.h
threadEIOCint.o: /usr/include/bits/signum.h /usr/include/assert.h
threadEIOCint.o: /usr/include/limits.h
threadEIOCint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
threadEIOCint.o: /usr/include/setjmp.h /usr/include/bits/setjmp.h
threadEIOCint.o: /usr/include/ctype.h /usr/include/endian.h
threadEIOCint.o: /usr/include/bits/endian.h /usr/include/fcntl.h
threadEIOCint.o: /usr/include/bits/fcntl.h /usr/include/sys/types.h
threadEIOCint.o: /usr/include/time.h /usr/include/unistd.h
threadEIOCint.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
threadEIOCint.o: /var/local/root/include/TROOT.h
threadEIOCint.o: /var/local/root/include/TDirectory.h
threadEIOCint.o: /var/local/root/include/TNamed.h
threadEIOCint.o: /var/local/root/include/TObject.h
threadEIOCint.o: /var/local/root/include/Rtypes.h
threadEIOCint.o: /var/local/root/include/DllImport.h
threadEIOCint.o: /var/local/root/include/Rtypeinfo.h
threadEIOCint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
threadEIOCint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
threadEIOCint.o: /var/local/root/include/TGenericClassInfo.h
threadEIOCint.o: /var/local/root/include/Varargs.h
threadEIOCint.o: /var/local/root/include/TStorage.h
threadEIOCint.o: /var/local/root/include/Riosfwd.h
threadEIOCint.o: /var/local/root/include/TBuffer.h
threadEIOCint.o: /var/local/root/include/Bytes.h
threadEIOCint.o: /var/local/root/include/TList.h
threadEIOCint.o: /var/local/root/include/TSeqCollection.h
threadEIOCint.o: /var/local/root/include/TCollection.h
threadEIOCint.o: /var/local/root/include/TIterator.h
threadEIOCint.o: /var/local/root/include/TString.h
threadEIOCint.o: /var/local/root/include/TMath.h
threadEIOCint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
threadEIOCint.o: /var/local/root/include/TRefCnt.h
threadEIOCint.o: /var/local/root/include/TDatime.h
threadEIOCint.o: /var/local/root/include/Htypes.h
threadEIOCint.o: /var/local/root/include/TUUID.h
threadEIOCint.o: /var/local/root/include/TMemberInspector.h EdbRunAccess.h
threadEIOCint.o: EdbLayer.h EdbDataSet.h /var/local/root/include/TObjArray.h
threadEIOCint.o: /var/local/root/include/TFile.h
threadEIOCint.o: /var/local/root/include/TCache.h
threadEIOCint.o: /var/local/root/include/THashList.h
threadEIOCint.o: /var/local/root/include/TClass.h
threadEIOCint.o: /var/local/root/include/TDictionary.h
threadEIOCint.o: /var/local/root/include/Property.h
threadEIOCint.o: /var/local/root/include/TStreamerInfo.h
threadEIOCint.o: /var/local/root/include/TClonesArray.h
threadEIOCint.o: /var/local/root/include/TError.h
threadEIOCint.o: /var/local/root/include/RtypesImp.h
threadEIOCint.o: /var/local/root/include/Api.h
threadEIOCint.o: /var/local/root/include/common.h
threadEIOCint.o: /var/local/root/include/security.h
threadEIOCint.o: /var/local/root/include/fproto.h
threadEIOCint.o: /var/local/root/include/global.h
threadEIOCint.o: /var/local/root/include/Class.h
threadEIOCint.o: /var/local/root/include/BaseCls.h
threadEIOCint.o: /var/local/root/include/Type.h
threadEIOCint.o: /var/local/root/include/Method.h
threadEIOCint.o: /var/local/root/include/MethodAr.h
threadEIOCint.o: /var/local/root/include/DataMbr.h
threadEIOCint.o: /var/local/root/include/CallFunc.h
threadEIOCint.o: /var/local/root/include/Typedf.h
threadEIOCint.o: /var/local/root/include/Token.h
threadEIOCint.o: /var/local/root/include/TClassEdit.h
threadEIOCint.o: /var/local/root/include/TCollectionProxy.h
threadEIOCint.o: /var/local/root/include/TClassStreamer.h
threadEIOCint.o: /var/local/root/include/TMemberStreamer.h
threadEdbDataSet.o: /var/local/root/include/TMatrix.h
threadEdbDataSet.o: /var/local/root/include/TMatrixF.h
threadEdbDataSet.o: /var/local/root/include/TMatrixFBase.h
threadEdbDataSet.o: /var/local/root/include/TROOT.h
threadEdbDataSet.o: /var/local/root/include/TDirectory.h
threadEdbDataSet.o: /var/local/root/include/TNamed.h
threadEdbDataSet.o: /var/local/root/include/TObject.h
threadEdbDataSet.o: /var/local/root/include/Rtypes.h
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
threadEdbDataSet.o: /var/local/root/include/TStreamerInfo.h
threadEdbDataSet.o: /var/local/root/include/TClonesArray.h
threadEdbDataSet.o: /var/local/root/include/TPluginManager.h
threadEdbDataSet.o: /var/local/root/include/TVirtualUtilHist.h
threadEdbDataSet.o: /var/local/root/include/TError.h
threadEdbDataSet.o: /var/local/root/include/TMatrixFUtils.h EdbDataSet.h
threadEdbDataSet.o: /var/local/root/include/TObjArray.h
threadEdbDataSet.o: /var/local/root/include/TFile.h
threadEdbDataSet.o: /var/local/root/include/TCache.h
threadEdbDataSet.o: /var/local/root/include/THashList.h EdbLayer.h
threadEdbLayer.o: EdbLayer.h /var/local/root/include/TNamed.h
threadEdbLayer.o: /var/local/root/include/TObject.h
threadEdbLayer.o: /var/local/root/include/Rtypes.h
threadEdbLayer.o: /var/local/root/include/DllImport.h
threadEdbLayer.o: /var/local/root/include/Rtypeinfo.h
threadEdbLayer.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
threadEdbLayer.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
threadEdbLayer.o: /usr/include/stdio.h /usr/include/features.h
threadEdbLayer.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
threadEdbLayer.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
threadEdbLayer.o: /usr/include/bits/types.h /usr/include/libio.h
threadEdbLayer.o: /usr/include/_G_config.h /usr/include/wchar.h
threadEdbLayer.o: /usr/include/bits/wchar.h /usr/include/gconv.h
threadEdbLayer.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
threadEdbLayer.o: /usr/include/bits/stdio_lim.h
threadEdbLayer.o: /var/local/root/include/TGenericClassInfo.h
threadEdbLayer.o: /var/local/root/include/Varargs.h
threadEdbLayer.o: /var/local/root/include/TStorage.h
threadEdbLayer.o: /var/local/root/include/Riosfwd.h
threadEdbLayer.o: /var/local/root/include/TBuffer.h
threadEdbLayer.o: /var/local/root/include/Bytes.h /usr/include/string.h
threadEdbLayer.o: /var/local/root/include/TList.h
threadEdbLayer.o: /var/local/root/include/TSeqCollection.h
threadEdbLayer.o: /var/local/root/include/TCollection.h
threadEdbLayer.o: /var/local/root/include/TIterator.h
threadEdbLayer.o: /var/local/root/include/TString.h
threadEdbLayer.o: /var/local/root/include/TMath.h
threadEdbLayer.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
threadEdbLayer.o: /var/local/root/include/TRefCnt.h
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
threadEdbRunAccess.o: /var/local/root/include/TStreamerInfo.h
threadEdbRunAccess.o: /var/local/root/include/TClonesArray.h
threadEdbRunAccess.o: /var/local/root/include/TTime.h EdbRunAccess.h
threadEdbRunAccess.o: EdbLayer.h
