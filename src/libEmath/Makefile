#------------------------------------------------------------------------------
 include ../config/ProjectDef.mk
 include ../config/RootDef.mk
#------------------------------------------------------------------------------

LIB_TGT       = $(LIB_DIR)/libEmath.so

headers       = TIndexCell.h EdbMath.h

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

threadEdbMath.o: /var/local/root/include/TMath.h
threadEdbMath.o: /var/local/root/include/Rtypes.h
threadEdbMath.o: /var/local/root/include/RConfig.h
threadEdbMath.o: /var/local/root/include/RVersion.h
threadEdbMath.o: /var/local/root/include/DllImport.h
threadEdbMath.o: /var/local/root/include/Rtypeinfo.h
threadEdbMath.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
threadEdbMath.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
threadEdbMath.o: /usr/include/stdio.h /usr/include/features.h
threadEdbMath.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
threadEdbMath.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
threadEdbMath.o: /usr/include/bits/types.h /usr/include/libio.h
threadEdbMath.o: /usr/include/_G_config.h /usr/include/wchar.h
threadEdbMath.o: /usr/include/bits/wchar.h /usr/include/gconv.h
threadEdbMath.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
threadEdbMath.o: /usr/include/bits/stdio_lim.h
threadEdbMath.o: /var/local/root/include/TGenericClassInfo.h
threadEdbMath.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
threadEdbMath.o: EdbMath.h /var/local/root/include/TObject.h
threadEdbMath.o: /var/local/root/include/Varargs.h
threadEdbMath.o: /var/local/root/include/TStorage.h
threadEdbMath.o: /var/local/root/include/Riosfwd.h
threadEdbMath.o: /var/local/root/include/TBuffer.h
threadEdbMath.o: /var/local/root/include/Bytes.h /usr/include/string.h
threadEdbMath.o: /var/local/root/include/TArrayI.h
threadEdbMath.o: /var/local/root/include/TArray.h
threadEdbMath.o: /var/local/root/include/Htypes.h
threadEdbMath.o: /var/local/root/include/TArrayD.h
threadEdbMath.o: /var/local/root/include/TMatrixD.h
threadEdbMath.o: /var/local/root/include/TMatrixDBase.h /usr/include/limits.h
threadEdbMath.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
threadEdbMath.o: /var/local/root/include/TROOT.h
threadEdbMath.o: /var/local/root/include/TDirectory.h
threadEdbMath.o: /var/local/root/include/TNamed.h
threadEdbMath.o: /var/local/root/include/TList.h
threadEdbMath.o: /var/local/root/include/TSeqCollection.h
threadEdbMath.o: /var/local/root/include/TCollection.h
threadEdbMath.o: /var/local/root/include/TIterator.h
threadEdbMath.o: /var/local/root/include/TString.h
threadEdbMath.o: /var/local/root/include/TRefCnt.h
threadEdbMath.o: /var/local/root/include/TDatime.h
threadEdbMath.o: /var/local/root/include/TUUID.h
threadEdbMath.o: /var/local/root/include/TClass.h
threadEdbMath.o: /var/local/root/include/TDictionary.h
threadEdbMath.o: /var/local/root/include/Property.h
threadEdbMath.o: /var/local/root/include/TObjArray.h
threadEdbMath.o: /var/local/root/include/TStreamerInfo.h
threadEdbMath.o: /var/local/root/include/TFile.h
threadEdbMath.o: /var/local/root/include/TCache.h
threadEdbMath.o: /var/local/root/include/THashList.h
threadEdbMath.o: /var/local/root/include/TClonesArray.h
threadEdbMath.o: /var/local/root/include/TPluginManager.h
threadEdbMath.o: /var/local/root/include/TVirtualUtilHist.h
threadEdbMath.o: /var/local/root/include/TError.h
threadEdbMath.o: /var/local/root/include/TMatrixDUtils.h
threadEdbMath.o: /var/local/root/include/TVectorD.h
threadEdbMath.o: /var/local/root/include/TMatrixDSym.h
threadEdbMath.o: /var/local/root/include/TMatrixDSparse.h EmathVer.h
threadEdbMath.o: /var/local/root/include/Riostream.h
threadEmathCint.o: /var/local/root/include/RConfig.h
threadEmathCint.o: /var/local/root/include/RVersion.h EmathCint.h
threadEmathCint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
threadEmathCint.o: /usr/include/stdio.h /usr/include/features.h
threadEmathCint.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
threadEmathCint.o: /usr/include/bits/types.h /usr/include/libio.h
threadEmathCint.o: /usr/include/_G_config.h /usr/include/wchar.h
threadEmathCint.o: /usr/include/bits/wchar.h /usr/include/gconv.h
threadEmathCint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
threadEmathCint.o: /usr/include/bits/stdio_lim.h /usr/include/stdlib.h
threadEmathCint.o: /usr/include/math.h /usr/include/bits/huge_val.h
threadEmathCint.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
threadEmathCint.o: /usr/include/string.h /var/local/root/include/G__ci.h
threadEmathCint.o: /usr/include/signal.h /usr/include/bits/sigset.h
threadEmathCint.o: /usr/include/bits/signum.h /usr/include/assert.h
threadEmathCint.o: /usr/include/limits.h
threadEmathCint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
threadEmathCint.o: /usr/include/setjmp.h /usr/include/bits/setjmp.h
threadEmathCint.o: /usr/include/ctype.h /usr/include/endian.h
threadEmathCint.o: /usr/include/bits/endian.h /usr/include/fcntl.h
threadEmathCint.o: /usr/include/bits/fcntl.h /usr/include/sys/types.h
threadEmathCint.o: /usr/include/time.h /usr/include/unistd.h
threadEmathCint.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
threadEmathCint.o: /var/local/root/include/TROOT.h
threadEmathCint.o: /var/local/root/include/TDirectory.h
threadEmathCint.o: /var/local/root/include/TNamed.h
threadEmathCint.o: /var/local/root/include/TList.h
threadEmathCint.o: /var/local/root/include/TSeqCollection.h
threadEmathCint.o: /var/local/root/include/TCollection.h
threadEmathCint.o: /var/local/root/include/TIterator.h
threadEmathCint.o: /var/local/root/include/TString.h
threadEmathCint.o: /var/local/root/include/TRefCnt.h
threadEmathCint.o: /var/local/root/include/TDatime.h
threadEmathCint.o: /var/local/root/include/TUUID.h
threadEmathCint.o: /var/local/root/include/TMemberInspector.h
threadEmathCint.o: /var/local/root/include/TObject.h
threadEmathCint.o: /var/local/root/include/Varargs.h
threadEmathCint.o: /var/local/root/include/TStorage.h
threadEmathCint.o: /var/local/root/include/Riosfwd.h
threadEmathCint.o: /var/local/root/include/TBuffer.h
threadEmathCint.o: /var/local/root/include/Bytes.h TIndexCell.h
threadEmathCint.o: /var/local/root/include/TObjArray.h EdbMath.h
threadEmathCint.o: /var/local/root/include/TArrayI.h
threadEmathCint.o: /var/local/root/include/TArray.h
threadEmathCint.o: /var/local/root/include/Htypes.h
threadEmathCint.o: /var/local/root/include/TArrayD.h
threadEmathCint.o: /var/local/root/include/TClass.h
threadEmathCint.o: /var/local/root/include/TDictionary.h
threadEmathCint.o: /var/local/root/include/Property.h
threadEmathCint.o: /var/local/root/include/TStreamerInfo.h
threadEmathCint.o: /var/local/root/include/TFile.h
threadEmathCint.o: /var/local/root/include/TCache.h
threadEmathCint.o: /var/local/root/include/THashList.h
threadEmathCint.o: /var/local/root/include/TClonesArray.h
threadEmathCint.o: /var/local/root/include/TError.h
threadEmathCint.o: /var/local/root/include/RtypesImp.h
threadEmathCint.o: /var/local/root/include/Api.h
threadEmathCint.o: /var/local/root/include/common.h
threadEmathCint.o: /var/local/root/include/security.h
threadEmathCint.o: /var/local/root/include/fproto.h
threadEmathCint.o: /var/local/root/include/global.h
threadEmathCint.o: /var/local/root/include/Class.h
threadEmathCint.o: /var/local/root/include/BaseCls.h
threadEmathCint.o: /var/local/root/include/Type.h
threadEmathCint.o: /var/local/root/include/Method.h
threadEmathCint.o: /var/local/root/include/MethodAr.h
threadEmathCint.o: /var/local/root/include/DataMbr.h
threadEmathCint.o: /var/local/root/include/CallFunc.h
threadEmathCint.o: /var/local/root/include/Typedf.h
threadEmathCint.o: /var/local/root/include/Token.h
threadEmathCint.o: /var/local/root/include/TClassEdit.h
threadEmathCint.o: /var/local/root/include/TCollectionProxy.h
threadEmathCint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
threadEmathCint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
threadEmathCint.o: /var/local/root/include/TClassStreamer.h
threadEmathCint.o: /var/local/root/include/Rtypes.h
threadEmathCint.o: /var/local/root/include/DllImport.h
threadEmathCint.o: /var/local/root/include/Rtypeinfo.h
threadEmathCint.o: /var/local/root/include/TGenericClassInfo.h
threadEmathCint.o: /var/local/root/include/TMemberStreamer.h
