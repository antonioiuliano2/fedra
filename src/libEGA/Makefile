#------------------------------------------------------------------------------
include ../config/ProjectDef.mk
include ../config/RootDef.mk
#------------------------------------------------------------------------------

PROJECT_LIBS += -lvt -lEmath -lEdb

LIB_TGT       = $(LIB_DIR)/libEGA.so

headers       = EdbGA.h EdbIP.h

classes	      = $(headers:.h=.$(SrcSuf))
EGACint	      = $(headers) EGALinkDef.h
SRC           = $(classes) EGACint.$(SrcSuf)
OBJ           = $(SRC:.$(SrcSuf)=.$(ObjSuf))

CXXFLAGS += -I$(INC_DIR)

#------------------------------------------------------------------------------

$(LIB_TGT): $(OBJ)
	$(CXX) -O -shared $(LIBS) -o $(LIB_TGT) $(OBJ)

EGACint.cxx: $(EGACint)
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

threadEGACint.o: /var/local/root/include/RConfig.h
threadEGACint.o: /var/local/root/include/RVersion.h EGACint.h
threadEGACint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
threadEGACint.o: /usr/include/stdio.h /usr/include/features.h
threadEGACint.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
threadEGACint.o: /usr/include/bits/types.h /usr/include/libio.h
threadEGACint.o: /usr/include/_G_config.h /usr/include/wchar.h
threadEGACint.o: /usr/include/bits/wchar.h /usr/include/gconv.h
threadEGACint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
threadEGACint.o: /usr/include/bits/stdio_lim.h /usr/include/stdlib.h
threadEGACint.o: /usr/include/math.h /usr/include/bits/huge_val.h
threadEGACint.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
threadEGACint.o: /usr/include/string.h /var/local/root/include/G__ci.h
threadEGACint.o: /usr/include/signal.h /usr/include/bits/sigset.h
threadEGACint.o: /usr/include/bits/signum.h /usr/include/assert.h
threadEGACint.o: /usr/include/limits.h
threadEGACint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
threadEGACint.o: /usr/include/setjmp.h /usr/include/bits/setjmp.h
threadEGACint.o: /usr/include/ctype.h /usr/include/endian.h
threadEGACint.o: /usr/include/bits/endian.h /usr/include/fcntl.h
threadEGACint.o: /usr/include/bits/fcntl.h /usr/include/sys/types.h
threadEGACint.o: /usr/include/time.h /usr/include/unistd.h
threadEGACint.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
threadEGACint.o: /var/local/root/include/TROOT.h
threadEGACint.o: /var/local/root/include/TDirectory.h
threadEGACint.o: /var/local/root/include/TNamed.h
threadEGACint.o: /var/local/root/include/TObject.h
threadEGACint.o: /var/local/root/include/Rtypes.h
threadEGACint.o: /var/local/root/include/DllImport.h
threadEGACint.o: /var/local/root/include/Rtypeinfo.h
threadEGACint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
threadEGACint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
threadEGACint.o: /var/local/root/include/TGenericClassInfo.h
threadEGACint.o: /var/local/root/include/Varargs.h
threadEGACint.o: /var/local/root/include/TStorage.h
threadEGACint.o: /var/local/root/include/Riosfwd.h
threadEGACint.o: /var/local/root/include/TBuffer.h
threadEGACint.o: /var/local/root/include/Bytes.h
threadEGACint.o: /var/local/root/include/TList.h
threadEGACint.o: /var/local/root/include/TSeqCollection.h
threadEGACint.o: /var/local/root/include/TCollection.h
threadEGACint.o: /var/local/root/include/TIterator.h
threadEGACint.o: /var/local/root/include/TString.h
threadEGACint.o: /var/local/root/include/TMath.h
threadEGACint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
threadEGACint.o: /var/local/root/include/TRefCnt.h
threadEGACint.o: /var/local/root/include/TDatime.h
threadEGACint.o: /var/local/root/include/Htypes.h
threadEGACint.o: /var/local/root/include/TUUID.h
threadEGACint.o: /var/local/root/include/TMemberInspector.h EdbGA.h EdbIP.h
threadEGACint.o: /var/local/root/include/TArrayF.h
threadEGACint.o: /var/local/root/include/TArray.h
threadEGACint.o: /var/local/root/include/TClass.h
threadEGACint.o: /var/local/root/include/TDictionary.h
threadEGACint.o: /var/local/root/include/Property.h
threadEGACint.o: /var/local/root/include/TObjArray.h
threadEGACint.o: /var/local/root/include/TStreamerInfo.h
threadEGACint.o: /var/local/root/include/TFile.h
threadEGACint.o: /var/local/root/include/TCache.h
threadEGACint.o: /var/local/root/include/THashList.h
threadEGACint.o: /var/local/root/include/TClonesArray.h
threadEGACint.o: /var/local/root/include/TError.h
threadEGACint.o: /var/local/root/include/RtypesImp.h
threadEGACint.o: /var/local/root/include/Api.h
threadEGACint.o: /var/local/root/include/common.h
threadEGACint.o: /var/local/root/include/security.h
threadEGACint.o: /var/local/root/include/fproto.h
threadEGACint.o: /var/local/root/include/global.h
threadEGACint.o: /var/local/root/include/Class.h
threadEGACint.o: /var/local/root/include/BaseCls.h
threadEGACint.o: /var/local/root/include/Type.h
threadEGACint.o: /var/local/root/include/Method.h
threadEGACint.o: /var/local/root/include/MethodAr.h
threadEGACint.o: /var/local/root/include/DataMbr.h
threadEGACint.o: /var/local/root/include/CallFunc.h
threadEGACint.o: /var/local/root/include/Typedf.h
threadEGACint.o: /var/local/root/include/Token.h
threadEGACint.o: /var/local/root/include/TClassEdit.h
threadEGACint.o: /var/local/root/include/TCollectionProxy.h
threadEGACint.o: /var/local/root/include/TClassStreamer.h
threadEGACint.o: /var/local/root/include/TMemberStreamer.h
threadEdbGA.o: EdbGA.h EdbIP.h /var/local/root/include/TNamed.h
threadEdbGA.o: /var/local/root/include/TObject.h
threadEdbGA.o: /var/local/root/include/Rtypes.h
threadEdbGA.o: /var/local/root/include/DllImport.h
threadEdbGA.o: /var/local/root/include/Rtypeinfo.h
threadEdbGA.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
threadEdbGA.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
threadEdbGA.o: /usr/include/stdio.h /usr/include/features.h
threadEdbGA.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
threadEdbGA.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
threadEdbGA.o: /usr/include/bits/types.h /usr/include/libio.h
threadEdbGA.o: /usr/include/_G_config.h /usr/include/wchar.h
threadEdbGA.o: /usr/include/bits/wchar.h /usr/include/gconv.h
threadEdbGA.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
threadEdbGA.o: /usr/include/bits/stdio_lim.h
threadEdbGA.o: /var/local/root/include/TGenericClassInfo.h
threadEdbGA.o: /var/local/root/include/Varargs.h
threadEdbGA.o: /var/local/root/include/TStorage.h
threadEdbGA.o: /var/local/root/include/Riosfwd.h
threadEdbGA.o: /var/local/root/include/TBuffer.h
threadEdbGA.o: /var/local/root/include/Bytes.h /usr/include/string.h
threadEdbGA.o: /var/local/root/include/TList.h
threadEdbGA.o: /var/local/root/include/TSeqCollection.h
threadEdbGA.o: /var/local/root/include/TCollection.h
threadEdbGA.o: /var/local/root/include/TIterator.h
threadEdbGA.o: /var/local/root/include/TString.h
threadEdbGA.o: /var/local/root/include/TMath.h
threadEdbGA.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
threadEdbGA.o: /var/local/root/include/TRefCnt.h
threadEdbGA.o: /var/local/root/include/TArrayF.h
threadEdbGA.o: /var/local/root/include/TArray.h
threadEdbIP.o: /var/local/root/include/TH1.h
threadEdbIP.o: /var/local/root/include/TVirtualHistPainter.h
threadEdbIP.o: /var/local/root/include/TClass.h
threadEdbIP.o: /var/local/root/include/TDictionary.h
threadEdbIP.o: /var/local/root/include/Property.h
threadEdbIP.o: /var/local/root/include/TObjArray.h
threadEdbIP.o: /var/local/root/include/TStreamerInfo.h
threadEdbIP.o: /var/local/root/include/TFile.h
threadEdbIP.o: /var/local/root/include/TCache.h
threadEdbIP.o: /var/local/root/include/THashList.h
threadEdbIP.o: /var/local/root/include/TClonesArray.h
threadEdbIP.o: /var/local/root/include/TAxis.h
threadEdbIP.o: /var/local/root/include/TNamed.h
threadEdbIP.o: /var/local/root/include/TObject.h
threadEdbIP.o: /var/local/root/include/Rtypes.h
threadEdbIP.o: /var/local/root/include/DllImport.h
threadEdbIP.o: /var/local/root/include/Rtypeinfo.h
threadEdbIP.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
threadEdbIP.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
threadEdbIP.o: /usr/include/stdio.h /usr/include/features.h
threadEdbIP.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
threadEdbIP.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
threadEdbIP.o: /usr/include/bits/types.h /usr/include/libio.h
threadEdbIP.o: /usr/include/_G_config.h /usr/include/wchar.h
threadEdbIP.o: /usr/include/bits/wchar.h /usr/include/gconv.h
threadEdbIP.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
threadEdbIP.o: /usr/include/bits/stdio_lim.h
threadEdbIP.o: /var/local/root/include/TGenericClassInfo.h
threadEdbIP.o: /var/local/root/include/Varargs.h
threadEdbIP.o: /var/local/root/include/TStorage.h
threadEdbIP.o: /var/local/root/include/Riosfwd.h
threadEdbIP.o: /var/local/root/include/TBuffer.h
threadEdbIP.o: /var/local/root/include/Bytes.h /usr/include/string.h
threadEdbIP.o: /var/local/root/include/TList.h
threadEdbIP.o: /var/local/root/include/TSeqCollection.h
threadEdbIP.o: /var/local/root/include/TCollection.h
threadEdbIP.o: /var/local/root/include/TIterator.h
threadEdbIP.o: /var/local/root/include/TString.h
threadEdbIP.o: /var/local/root/include/TMath.h
threadEdbIP.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
threadEdbIP.o: /var/local/root/include/TRefCnt.h
threadEdbIP.o: /var/local/root/include/TAttAxis.h
threadEdbIP.o: /var/local/root/include/Gtypes.h
threadEdbIP.o: /var/local/root/include/Htypes.h
threadEdbIP.o: /var/local/root/include/TArrayD.h
threadEdbIP.o: /var/local/root/include/TArray.h
threadEdbIP.o: /var/local/root/include/TAttLine.h
threadEdbIP.o: /var/local/root/include/TAttFill.h
threadEdbIP.o: /var/local/root/include/TAttMarker.h
threadEdbIP.o: /var/local/root/include/TArrayC.h
threadEdbIP.o: /var/local/root/include/TArrayS.h
threadEdbIP.o: /var/local/root/include/TArrayI.h
threadEdbIP.o: /var/local/root/include/TArrayF.h
threadEdbIP.o: /var/local/root/include/Foption.h
threadEdbIP.o: /var/local/root/include/TH2.h /var/local/root/include/TTree.h
threadEdbIP.o: /var/local/root/include/TBranch.h
threadEdbIP.o: /var/local/root/include/TStringLong.h
threadEdbIP.o: /var/local/root/include/TVirtualTreePlayer.h EdbIP.h
