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
threadEphysCint.o: /var/local/root/include/RConfig.h
threadEphysCint.o: /var/local/root/include/RVersion.h EphysCint.h
threadEphysCint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
threadEphysCint.o: /usr/include/stdio.h /usr/include/features.h
threadEphysCint.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
threadEphysCint.o: /usr/include/bits/types.h /usr/include/libio.h
threadEphysCint.o: /usr/include/_G_config.h /usr/include/wchar.h
threadEphysCint.o: /usr/include/bits/wchar.h /usr/include/gconv.h
threadEphysCint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
threadEphysCint.o: /usr/include/bits/stdio_lim.h /usr/include/stdlib.h
threadEphysCint.o: /usr/include/math.h /usr/include/bits/huge_val.h
threadEphysCint.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
threadEphysCint.o: /usr/include/string.h /var/local/root/include/G__ci.h
threadEphysCint.o: /usr/include/signal.h /usr/include/bits/sigset.h
threadEphysCint.o: /usr/include/bits/signum.h /usr/include/assert.h
threadEphysCint.o: /usr/include/limits.h
threadEphysCint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
threadEphysCint.o: /usr/include/setjmp.h /usr/include/bits/setjmp.h
threadEphysCint.o: /usr/include/ctype.h /usr/include/endian.h
threadEphysCint.o: /usr/include/bits/endian.h /usr/include/fcntl.h
threadEphysCint.o: /usr/include/bits/fcntl.h /usr/include/sys/types.h
threadEphysCint.o: /usr/include/time.h /usr/include/unistd.h
threadEphysCint.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
threadEphysCint.o: /var/local/root/include/TROOT.h
threadEphysCint.o: /var/local/root/include/TDirectory.h
threadEphysCint.o: /var/local/root/include/TNamed.h
threadEphysCint.o: /var/local/root/include/TList.h
threadEphysCint.o: /var/local/root/include/TSeqCollection.h
threadEphysCint.o: /var/local/root/include/TCollection.h
threadEphysCint.o: /var/local/root/include/TIterator.h
threadEphysCint.o: /var/local/root/include/TString.h
threadEphysCint.o: /var/local/root/include/TRefCnt.h
threadEphysCint.o: /var/local/root/include/TDatime.h
threadEphysCint.o: /var/local/root/include/Htypes.h
threadEphysCint.o: /var/local/root/include/Rtypes.h
threadEphysCint.o: /var/local/root/include/DllImport.h
threadEphysCint.o: /var/local/root/include/Rtypeinfo.h
threadEphysCint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
threadEphysCint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
threadEphysCint.o: /var/local/root/include/TGenericClassInfo.h
threadEphysCint.o: /var/local/root/include/TUUID.h
threadEphysCint.o: /var/local/root/include/TMemberInspector.h
threadEphysCint.o: /var/local/root/include/TObject.h
threadEphysCint.o: /var/local/root/include/Varargs.h
threadEphysCint.o: /var/local/root/include/TStorage.h
threadEphysCint.o: /var/local/root/include/Riosfwd.h
threadEphysCint.o: /var/local/root/include/TBuffer.h
threadEphysCint.o: /var/local/root/include/Bytes.h EdbPhys.h
threadEphysCint.o: /var/local/root/include/TRandom.h
threadEphysCint.o: /var/local/root/include/TClass.h
threadEphysCint.o: /var/local/root/include/TDictionary.h
threadEphysCint.o: /var/local/root/include/Property.h
threadEphysCint.o: /var/local/root/include/TObjArray.h
threadEphysCint.o: /var/local/root/include/TStreamerInfo.h
threadEphysCint.o: /var/local/root/include/TFile.h
threadEphysCint.o: /var/local/root/include/TCache.h
threadEphysCint.o: /var/local/root/include/THashList.h
threadEphysCint.o: /var/local/root/include/TClonesArray.h
threadEphysCint.o: /var/local/root/include/TError.h
threadEphysCint.o: /var/local/root/include/RtypesImp.h
threadEphysCint.o: /var/local/root/include/Api.h
threadEphysCint.o: /var/local/root/include/common.h
threadEphysCint.o: /var/local/root/include/security.h
threadEphysCint.o: /var/local/root/include/fproto.h
threadEphysCint.o: /var/local/root/include/global.h
threadEphysCint.o: /var/local/root/include/Class.h
threadEphysCint.o: /var/local/root/include/BaseCls.h
threadEphysCint.o: /var/local/root/include/Type.h
threadEphysCint.o: /var/local/root/include/Method.h
threadEphysCint.o: /var/local/root/include/MethodAr.h
threadEphysCint.o: /var/local/root/include/DataMbr.h
threadEphysCint.o: /var/local/root/include/CallFunc.h
threadEphysCint.o: /var/local/root/include/Typedf.h
threadEphysCint.o: /var/local/root/include/Token.h
threadEphysCint.o: /var/local/root/include/TClassEdit.h
threadEphysCint.o: /var/local/root/include/TCollectionProxy.h
threadEphysCint.o: /var/local/root/include/TClassStreamer.h
threadEphysCint.o: /var/local/root/include/TMemberStreamer.h
