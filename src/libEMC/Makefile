#------------------------------------------------------------------------------
include ../config/ProjectDef.mk
include ../config/RootDef.mk
#------------------------------------------------------------------------------

PROJECT_LIBS += -lEdr -lEmath -lEdb -lEphys

LIB_TGT       = $(LIB_DIR)/libEMC.so

headers       = EdbPVGen.h EdbBrick.h EdbBrickGen.h

classes	      = $(headers:.h=.$(SrcSuf))
EMCCint	      = $(headers) EMCLinkDef.h
SRC           = $(classes) EMCCint.$(SrcSuf)
OBJ           = $(SRC:.$(SrcSuf)=.$(ObjSuf))

CXXFLAGS += -I$(INC_DIR)

#------------------------------------------------------------------------------

$(LIB_TGT): $(OBJ)
	$(CXX) -O -shared $(LIBS) -o $(LIB_TGT) $(OBJ)

EMCCint.cxx: $(EMCCint)
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

threadEMCCint.o: /var/local/root/include/RConfig.h
threadEMCCint.o: /var/local/root/include/RVersion.h EMCCint.h
threadEMCCint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
threadEMCCint.o: /usr/include/stdio.h /usr/include/features.h
threadEMCCint.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
threadEMCCint.o: /usr/include/bits/types.h /usr/include/libio.h
threadEMCCint.o: /usr/include/_G_config.h /usr/include/wchar.h
threadEMCCint.o: /usr/include/bits/wchar.h /usr/include/gconv.h
threadEMCCint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
threadEMCCint.o: /usr/include/bits/stdio_lim.h /usr/include/stdlib.h
threadEMCCint.o: /usr/include/math.h /usr/include/bits/huge_val.h
threadEMCCint.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
threadEMCCint.o: /usr/include/string.h /var/local/root/include/G__ci.h
threadEMCCint.o: /usr/include/signal.h /usr/include/bits/sigset.h
threadEMCCint.o: /usr/include/bits/signum.h /usr/include/assert.h
threadEMCCint.o: /usr/include/limits.h
threadEMCCint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
threadEMCCint.o: /usr/include/setjmp.h /usr/include/bits/setjmp.h
threadEMCCint.o: /usr/include/ctype.h /usr/include/endian.h
threadEMCCint.o: /usr/include/bits/endian.h /usr/include/fcntl.h
threadEMCCint.o: /usr/include/bits/fcntl.h /usr/include/sys/types.h
threadEMCCint.o: /usr/include/time.h /usr/include/unistd.h
threadEMCCint.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
threadEMCCint.o: /var/local/root/include/TROOT.h
threadEMCCint.o: /var/local/root/include/TDirectory.h
threadEMCCint.o: /var/local/root/include/TNamed.h
threadEMCCint.o: /var/local/root/include/TObject.h
threadEMCCint.o: /var/local/root/include/Rtypes.h
threadEMCCint.o: /var/local/root/include/DllImport.h
threadEMCCint.o: /var/local/root/include/Rtypeinfo.h
threadEMCCint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
threadEMCCint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
threadEMCCint.o: /var/local/root/include/TGenericClassInfo.h
threadEMCCint.o: /var/local/root/include/Varargs.h
threadEMCCint.o: /var/local/root/include/TStorage.h
threadEMCCint.o: /var/local/root/include/Riosfwd.h
threadEMCCint.o: /var/local/root/include/TBuffer.h
threadEMCCint.o: /var/local/root/include/Bytes.h
threadEMCCint.o: /var/local/root/include/TList.h
threadEMCCint.o: /var/local/root/include/TSeqCollection.h
threadEMCCint.o: /var/local/root/include/TCollection.h
threadEMCCint.o: /var/local/root/include/TIterator.h
threadEMCCint.o: /var/local/root/include/TString.h
threadEMCCint.o: /var/local/root/include/TMath.h
threadEMCCint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
threadEMCCint.o: /var/local/root/include/TRefCnt.h
threadEMCCint.o: /var/local/root/include/TDatime.h
threadEMCCint.o: /var/local/root/include/Htypes.h
threadEMCCint.o: /var/local/root/include/TUUID.h
threadEMCCint.o: /var/local/root/include/TMemberInspector.h EdbPVGen.h
threadEMCCint.o: /var/local/root/include/TGenPhaseSpace.h
threadEMCCint.o: /var/local/root/include/TLorentzVector.h
threadEMCCint.o: /var/local/root/include/TVector3.h
threadEMCCint.o: /var/local/root/include/TError.h
threadEMCCint.o: /var/local/root/include/TVector2.h
threadEMCCint.o: /var/local/root/include/TMatrix.h
threadEMCCint.o: /var/local/root/include/TMatrixF.h
threadEMCCint.o: /var/local/root/include/TMatrixFBase.h
threadEMCCint.o: /var/local/root/include/TClass.h
threadEMCCint.o: /var/local/root/include/TDictionary.h
threadEMCCint.o: /var/local/root/include/Property.h
threadEMCCint.o: /var/local/root/include/TObjArray.h
threadEMCCint.o: /var/local/root/include/TStreamerInfo.h
threadEMCCint.o: /var/local/root/include/TFile.h
threadEMCCint.o: /var/local/root/include/TCache.h
threadEMCCint.o: /var/local/root/include/THashList.h
threadEMCCint.o: /var/local/root/include/TClonesArray.h
threadEMCCint.o: /var/local/root/include/TPluginManager.h
threadEMCCint.o: /var/local/root/include/TVirtualUtilHist.h
threadEMCCint.o: /var/local/root/include/TMatrixFUtils.h
threadEMCCint.o: /var/local/root/include/TRotation.h EdbBrick.h EdbBrickGen.h
threadEMCCint.o: /var/local/root/include/RtypesImp.h
threadEMCCint.o: /var/local/root/include/Api.h
threadEMCCint.o: /var/local/root/include/common.h
threadEMCCint.o: /var/local/root/include/security.h
threadEMCCint.o: /var/local/root/include/fproto.h
threadEMCCint.o: /var/local/root/include/global.h
threadEMCCint.o: /var/local/root/include/Class.h
threadEMCCint.o: /var/local/root/include/BaseCls.h
threadEMCCint.o: /var/local/root/include/Type.h
threadEMCCint.o: /var/local/root/include/Method.h
threadEMCCint.o: /var/local/root/include/MethodAr.h
threadEMCCint.o: /var/local/root/include/DataMbr.h
threadEMCCint.o: /var/local/root/include/CallFunc.h
threadEMCCint.o: /var/local/root/include/Typedf.h
threadEMCCint.o: /var/local/root/include/Token.h
threadEMCCint.o: /var/local/root/include/TClassEdit.h
threadEMCCint.o: /var/local/root/include/TCollectionProxy.h
threadEMCCint.o: /var/local/root/include/TClassStreamer.h
threadEMCCint.o: /var/local/root/include/TMemberStreamer.h
threadEdbBrick.o: EdbBrick.h /var/local/root/include/TObjArray.h
threadEdbBrickGen.o: /var/local/root/include/TRandom.h
threadEdbBrickGen.o: /var/local/root/include/TNamed.h
threadEdbBrickGen.o: /var/local/root/include/TObject.h
threadEdbBrickGen.o: /var/local/root/include/Rtypes.h
threadEdbBrickGen.o: /var/local/root/include/DllImport.h
threadEdbBrickGen.o: /var/local/root/include/Rtypeinfo.h
threadEdbBrickGen.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
threadEdbBrickGen.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
threadEdbBrickGen.o: /usr/include/stdio.h /usr/include/features.h
threadEdbBrickGen.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
threadEdbBrickGen.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
threadEdbBrickGen.o: /usr/include/bits/types.h /usr/include/libio.h
threadEdbBrickGen.o: /usr/include/_G_config.h /usr/include/wchar.h
threadEdbBrickGen.o: /usr/include/bits/wchar.h /usr/include/gconv.h
threadEdbBrickGen.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
threadEdbBrickGen.o: /usr/include/bits/stdio_lim.h
threadEdbBrickGen.o: /var/local/root/include/TGenericClassInfo.h
threadEdbBrickGen.o: /var/local/root/include/Varargs.h
threadEdbBrickGen.o: /var/local/root/include/TStorage.h
threadEdbBrickGen.o: /var/local/root/include/Riosfwd.h
threadEdbBrickGen.o: /var/local/root/include/TBuffer.h
threadEdbBrickGen.o: /var/local/root/include/Bytes.h /usr/include/string.h
threadEdbBrickGen.o: /var/local/root/include/TList.h
threadEdbBrickGen.o: /var/local/root/include/TSeqCollection.h
threadEdbBrickGen.o: /var/local/root/include/TCollection.h
threadEdbBrickGen.o: /var/local/root/include/TIterator.h
threadEdbBrickGen.o: /var/local/root/include/TString.h
threadEdbBrickGen.o: /var/local/root/include/TMath.h
threadEdbBrickGen.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
threadEdbBrickGen.o: /var/local/root/include/TRefCnt.h EdbBrickGen.h
threadEdbBrickGen.o: EdbBrick.h /var/local/root/include/TObjArray.h
threadEdbBrickGen.o: EdbPVGen.h /var/local/root/include/TGenPhaseSpace.h
threadEdbBrickGen.o: /var/local/root/include/TLorentzVector.h
threadEdbBrickGen.o: /var/local/root/include/TVector3.h
threadEdbBrickGen.o: /var/local/root/include/TError.h
threadEdbBrickGen.o: /var/local/root/include/TVector2.h
threadEdbBrickGen.o: /var/local/root/include/TMatrix.h
threadEdbBrickGen.o: /var/local/root/include/TMatrixF.h
threadEdbBrickGen.o: /var/local/root/include/TMatrixFBase.h
threadEdbBrickGen.o: /var/local/root/include/TClass.h
threadEdbBrickGen.o: /var/local/root/include/TDictionary.h
threadEdbBrickGen.o: /var/local/root/include/Property.h
threadEdbBrickGen.o: /var/local/root/include/TStreamerInfo.h
threadEdbBrickGen.o: /var/local/root/include/TFile.h
threadEdbBrickGen.o: /var/local/root/include/TCache.h
threadEdbBrickGen.o: /var/local/root/include/THashList.h
threadEdbBrickGen.o: /var/local/root/include/TClonesArray.h
threadEdbBrickGen.o: /var/local/root/include/TPluginManager.h
threadEdbBrickGen.o: /var/local/root/include/TVirtualUtilHist.h
threadEdbBrickGen.o: /var/local/root/include/TMatrixFUtils.h
threadEdbBrickGen.o: /var/local/root/include/TRotation.h
threadEdbPVGen.o: /var/local/root/include/TRandom.h
threadEdbPVGen.o: /var/local/root/include/TNamed.h
threadEdbPVGen.o: /var/local/root/include/TObject.h
threadEdbPVGen.o: /var/local/root/include/Rtypes.h
threadEdbPVGen.o: /var/local/root/include/DllImport.h
threadEdbPVGen.o: /var/local/root/include/Rtypeinfo.h
threadEdbPVGen.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
threadEdbPVGen.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
threadEdbPVGen.o: /usr/include/stdio.h /usr/include/features.h
threadEdbPVGen.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
threadEdbPVGen.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
threadEdbPVGen.o: /usr/include/bits/types.h /usr/include/libio.h
threadEdbPVGen.o: /usr/include/_G_config.h /usr/include/wchar.h
threadEdbPVGen.o: /usr/include/bits/wchar.h /usr/include/gconv.h
threadEdbPVGen.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
threadEdbPVGen.o: /usr/include/bits/stdio_lim.h
threadEdbPVGen.o: /var/local/root/include/TGenericClassInfo.h
threadEdbPVGen.o: /var/local/root/include/Varargs.h
threadEdbPVGen.o: /var/local/root/include/TStorage.h
threadEdbPVGen.o: /var/local/root/include/Riosfwd.h
threadEdbPVGen.o: /var/local/root/include/TBuffer.h
threadEdbPVGen.o: /var/local/root/include/Bytes.h /usr/include/string.h
threadEdbPVGen.o: /var/local/root/include/TList.h
threadEdbPVGen.o: /var/local/root/include/TSeqCollection.h
threadEdbPVGen.o: /var/local/root/include/TCollection.h
threadEdbPVGen.o: /var/local/root/include/TIterator.h
threadEdbPVGen.o: /var/local/root/include/TString.h
threadEdbPVGen.o: /var/local/root/include/TMath.h
threadEdbPVGen.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
threadEdbPVGen.o: /var/local/root/include/TRefCnt.h EdbPVGen.h
threadEdbPVGen.o: /var/local/root/include/TGenPhaseSpace.h
threadEdbPVGen.o: /var/local/root/include/TLorentzVector.h
threadEdbPVGen.o: /var/local/root/include/TVector3.h
threadEdbPVGen.o: /var/local/root/include/TError.h
threadEdbPVGen.o: /var/local/root/include/TVector2.h
threadEdbPVGen.o: /var/local/root/include/TMatrix.h
threadEdbPVGen.o: /var/local/root/include/TMatrixF.h
threadEdbPVGen.o: /var/local/root/include/TMatrixFBase.h
threadEdbPVGen.o: /var/local/root/include/TClass.h
threadEdbPVGen.o: /var/local/root/include/TDictionary.h
threadEdbPVGen.o: /var/local/root/include/Property.h
threadEdbPVGen.o: /var/local/root/include/TObjArray.h
threadEdbPVGen.o: /var/local/root/include/TStreamerInfo.h
threadEdbPVGen.o: /var/local/root/include/TFile.h
threadEdbPVGen.o: /var/local/root/include/TCache.h
threadEdbPVGen.o: /var/local/root/include/THashList.h
threadEdbPVGen.o: /var/local/root/include/TClonesArray.h
threadEdbPVGen.o: /var/local/root/include/TPluginManager.h
threadEdbPVGen.o: /var/local/root/include/TVirtualUtilHist.h
threadEdbPVGen.o: /var/local/root/include/TMatrixFUtils.h
threadEdbPVGen.o: /var/local/root/include/TRotation.h
