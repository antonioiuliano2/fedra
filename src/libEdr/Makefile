#------------------------------------------------------------------------------
include ../config/ProjectDef.mk
include ../config/RootDef.mk
#------------------------------------------------------------------------------

PROJECT_LIBS += -lvt -lEmath -lEdb -lEphys

LIB_TGT       = $(LIB_DIR)/libEdr.so

headers       = EdbPattern.h EdbPVRec.h EdbGA.h EdbIP.h EdbVertex.h

classes	      = $(headers:.h=.$(SrcSuf))
EdrCint	      = $(headers) EdrLinkDef.h
SRC           = $(classes) EdrCint.$(SrcSuf)
OBJ           = $(SRC:.$(SrcSuf)=.$(ObjSuf))

CXXFLAGS += -I$(INC_DIR)

#------------------------------------------------------------------------------

$(LIB_TGT): $(OBJ)
	$(CXX) -O -shared $(LIBS) -o $(LIB_TGT) $(OBJ)

EdrCint.cxx: $(EdrCint)
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

threadEdbGA.o: EdbGA.h EdbIP.h /var/local/root/include/TNamed.h
threadEdbGA.o: /var/local/root/include/TObject.h
threadEdbGA.o: /var/local/root/include/Rtypes.h
threadEdbGA.o: /var/local/root/include/RConfig.h
threadEdbGA.o: /var/local/root/include/RVersion.h
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
threadEdbGA.o: /var/local/root/include/Htypes.h
threadEdbIP.o: /var/local/root/include/TH1.h
threadEdbIP.o: /var/local/root/include/TVirtualHistPainter.h
threadEdbIP.o: /var/local/root/include/TClass.h
threadEdbIP.o: /var/local/root/include/TDictionary.h
threadEdbIP.o: /var/local/root/include/TNamed.h
threadEdbIP.o: /var/local/root/include/TObject.h
threadEdbIP.o: /var/local/root/include/Rtypes.h
threadEdbIP.o: /var/local/root/include/RConfig.h
threadEdbIP.o: /var/local/root/include/RVersion.h
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
threadEdbIP.o: /var/local/root/include/Property.h
threadEdbIP.o: /var/local/root/include/TROOT.h
threadEdbIP.o: /var/local/root/include/TDirectory.h
threadEdbIP.o: /var/local/root/include/TDatime.h
threadEdbIP.o: /var/local/root/include/Htypes.h
threadEdbIP.o: /var/local/root/include/TUUID.h
threadEdbIP.o: /var/local/root/include/TObjArray.h
threadEdbIP.o: /var/local/root/include/TStreamerInfo.h
threadEdbIP.o: /var/local/root/include/TFile.h
threadEdbIP.o: /var/local/root/include/TCache.h
threadEdbIP.o: /var/local/root/include/THashList.h
threadEdbIP.o: /var/local/root/include/TClonesArray.h
threadEdbIP.o: /var/local/root/include/TAxis.h
threadEdbIP.o: /var/local/root/include/TAttAxis.h
threadEdbIP.o: /var/local/root/include/Gtypes.h
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
threadEdbPVRec.o: /var/local/root/include/TFile.h
threadEdbPVRec.o: /var/local/root/include/TCache.h
threadEdbPVRec.o: /var/local/root/include/THashList.h
threadEdbPVRec.o: /var/local/root/include/TVector3.h
threadEdbPVRec.o: /var/local/root/include/TMath.h
threadEdbPVRec.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
threadEdbPVRec.o: /var/local/root/include/TError.h
threadEdbPVRec.o: /var/local/root/include/Rtypes.h
threadEdbPVRec.o: /var/local/root/include/RConfig.h
threadEdbPVRec.o: /var/local/root/include/RVersion.h
threadEdbPVRec.o: /var/local/root/include/DllImport.h
threadEdbPVRec.o: /var/local/root/include/Rtypeinfo.h
threadEdbPVRec.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
threadEdbPVRec.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
threadEdbPVRec.o: /usr/include/stdio.h /usr/include/features.h
threadEdbPVRec.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
threadEdbPVRec.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
threadEdbPVRec.o: /usr/include/bits/types.h /usr/include/libio.h
threadEdbPVRec.o: /usr/include/_G_config.h /usr/include/wchar.h
threadEdbPVRec.o: /usr/include/bits/wchar.h /usr/include/gconv.h
threadEdbPVRec.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
threadEdbPVRec.o: /usr/include/bits/stdio_lim.h
threadEdbPVRec.o: /var/local/root/include/TGenericClassInfo.h
threadEdbPVRec.o: /var/local/root/include/Varargs.h
threadEdbPVRec.o: /var/local/root/include/TVector2.h
threadEdbPVRec.o: /var/local/root/include/TObject.h
threadEdbPVRec.o: /var/local/root/include/TStorage.h
threadEdbPVRec.o: /var/local/root/include/Riosfwd.h
threadEdbPVRec.o: /var/local/root/include/TBuffer.h
threadEdbPVRec.o: /var/local/root/include/Bytes.h /usr/include/string.h
threadEdbPVRec.o: /var/local/root/include/TMatrix.h
threadEdbPVRec.o: /var/local/root/include/TMatrixF.h
threadEdbPVRec.o: /var/local/root/include/TMatrixFBase.h
threadEdbPVRec.o: /var/local/root/include/TROOT.h
threadEdbPVRec.o: /var/local/root/include/TDirectory.h
threadEdbPVRec.o: /var/local/root/include/TDatime.h
threadEdbPVRec.o: /var/local/root/include/Htypes.h
threadEdbPVRec.o: /var/local/root/include/TUUID.h
threadEdbPVRec.o: /var/local/root/include/TClass.h
threadEdbPVRec.o: /var/local/root/include/TDictionary.h
threadEdbPVRec.o: /var/local/root/include/TNamed.h
threadEdbPVRec.o: /var/local/root/include/TList.h
threadEdbPVRec.o: /var/local/root/include/TSeqCollection.h
threadEdbPVRec.o: /var/local/root/include/TCollection.h
threadEdbPVRec.o: /var/local/root/include/TIterator.h
threadEdbPVRec.o: /var/local/root/include/TString.h
threadEdbPVRec.o: /var/local/root/include/TRefCnt.h
threadEdbPVRec.o: /var/local/root/include/Property.h
threadEdbPVRec.o: /var/local/root/include/TObjArray.h
threadEdbPVRec.o: /var/local/root/include/TStreamerInfo.h
threadEdbPVRec.o: /var/local/root/include/TClonesArray.h
threadEdbPVRec.o: /var/local/root/include/TPluginManager.h
threadEdbPVRec.o: /var/local/root/include/TVirtualUtilHist.h
threadEdbPVRec.o: /var/local/root/include/TMatrixFUtils.h
threadEdbPVRec.o: /var/local/root/include/TArrayF.h
threadEdbPVRec.o: /var/local/root/include/TArray.h
threadEdbPVRec.o: /var/local/root/include/TBenchmark.h
threadEdbPVRec.o: /var/local/root/include/TStopwatch.h EdbVertex.h
threadEdbPVRec.o: /var/local/root/include/TTree.h
threadEdbPVRec.o: /var/local/root/include/TBranch.h
threadEdbPVRec.o: /var/local/root/include/TStringLong.h
threadEdbPVRec.o: /var/local/root/include/TVirtualTreePlayer.h
threadEdbPVRec.o: /var/local/root/include/TGraphErrors.h
threadEdbPVRec.o: /var/local/root/include/TGraph.h
threadEdbPVRec.o: /var/local/root/include/TH1.h
threadEdbPVRec.o: /var/local/root/include/TVirtualHistPainter.h
threadEdbPVRec.o: /var/local/root/include/TAxis.h
threadEdbPVRec.o: /var/local/root/include/TAttAxis.h
threadEdbPVRec.o: /var/local/root/include/Gtypes.h
threadEdbPVRec.o: /var/local/root/include/TArrayD.h
threadEdbPVRec.o: /var/local/root/include/TAttLine.h
threadEdbPVRec.o: /var/local/root/include/TAttFill.h
threadEdbPVRec.o: /var/local/root/include/TAttMarker.h
threadEdbPVRec.o: /var/local/root/include/TArrayC.h
threadEdbPVRec.o: /var/local/root/include/TArrayS.h
threadEdbPVRec.o: /var/local/root/include/TArrayI.h
threadEdbPVRec.o: /var/local/root/include/Foption.h EdbPattern.h
threadEdbPVRec.o: /var/local/root/include/TSortedList.h
threadEdbPVRec.o: /var/local/root/include/TArrayL.h
threadEdbPVRec.o: /var/local/root/include/TMatrixD.h
threadEdbPVRec.o: /var/local/root/include/TMatrixDBase.h
threadEdbPVRec.o: /usr/include/limits.h
threadEdbPVRec.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
threadEdbPVRec.o: /var/local/root/include/TMatrixDUtils.h EdbPVRec.h
threadEdbPattern.o: /var/local/root/include/TPolyLine.h
threadEdbPattern.o: /var/local/root/include/TString.h /usr/include/string.h
threadEdbPattern.o: /usr/include/features.h /usr/include/sys/cdefs.h
threadEdbPattern.o: /usr/include/gnu/stubs.h
threadEdbPattern.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
threadEdbPattern.o: /usr/include/stdio.h /usr/include/bits/types.h
threadEdbPattern.o: /usr/include/libio.h /usr/include/_G_config.h
threadEdbPattern.o: /usr/include/wchar.h /usr/include/bits/wchar.h
threadEdbPattern.o: /usr/include/gconv.h
threadEdbPattern.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
threadEdbPattern.o: /usr/include/bits/stdio_lim.h
threadEdbPattern.o: /var/local/root/include/TMath.h
threadEdbPattern.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
threadEdbPattern.o: /var/local/root/include/TRefCnt.h
threadEdbPattern.o: /var/local/root/include/TObject.h
threadEdbPattern.o: /var/local/root/include/Rtypes.h
threadEdbPattern.o: /var/local/root/include/RConfig.h
threadEdbPattern.o: /var/local/root/include/RVersion.h
threadEdbPattern.o: /var/local/root/include/DllImport.h
threadEdbPattern.o: /var/local/root/include/Rtypeinfo.h
threadEdbPattern.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
threadEdbPattern.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
threadEdbPattern.o: /var/local/root/include/TGenericClassInfo.h
threadEdbPattern.o: /var/local/root/include/Varargs.h
threadEdbPattern.o: /var/local/root/include/TStorage.h
threadEdbPattern.o: /var/local/root/include/Riosfwd.h
threadEdbPattern.o: /var/local/root/include/TBuffer.h
threadEdbPattern.o: /var/local/root/include/Bytes.h
threadEdbPattern.o: /var/local/root/include/TAttLine.h
threadEdbPattern.o: /var/local/root/include/TAttFill.h EdbPattern.h
threadEdbPattern.o: /var/local/root/include/TTree.h
threadEdbPattern.o: /var/local/root/include/TBranch.h
threadEdbPattern.o: /var/local/root/include/TStringLong.h
threadEdbPattern.o: /var/local/root/include/TVirtualTreePlayer.h
threadEdbPattern.o: /var/local/root/include/TObjArray.h
threadEdbPattern.o: /var/local/root/include/TSortedList.h
threadEdbPattern.o: /var/local/root/include/TArrayL.h
threadEdbPattern.o: /var/local/root/include/TMatrixD.h
threadEdbPattern.o: /var/local/root/include/TMatrixDBase.h
threadEdbPattern.o: /usr/include/limits.h
threadEdbPattern.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
threadEdbPattern.o: /var/local/root/include/TMatrixDUtils.h EdbVertex.h
threadEdbPattern.o: /var/local/root/include/TVector3.h
threadEdbPattern.o: /var/local/root/include/TError.h
threadEdbPattern.o: /var/local/root/include/TVector2.h
threadEdbPattern.o: /var/local/root/include/TMatrix.h
threadEdbPattern.o: /var/local/root/include/TMatrixF.h
threadEdbPattern.o: /var/local/root/include/TMatrixFBase.h
threadEdbPattern.o: /var/local/root/include/TROOT.h
threadEdbPattern.o: /var/local/root/include/TDirectory.h
threadEdbPattern.o: /var/local/root/include/TDatime.h
threadEdbPattern.o: /var/local/root/include/Htypes.h
threadEdbPattern.o: /var/local/root/include/TUUID.h
threadEdbPattern.o: /var/local/root/include/TClass.h
threadEdbPattern.o: /var/local/root/include/TDictionary.h
threadEdbPattern.o: /var/local/root/include/TNamed.h
threadEdbPattern.o: /var/local/root/include/TList.h
threadEdbPattern.o: /var/local/root/include/TSeqCollection.h
threadEdbPattern.o: /var/local/root/include/TCollection.h
threadEdbPattern.o: /var/local/root/include/TIterator.h
threadEdbPattern.o: /var/local/root/include/Property.h
threadEdbPattern.o: /var/local/root/include/TStreamerInfo.h
threadEdbPattern.o: /var/local/root/include/TFile.h
threadEdbPattern.o: /var/local/root/include/TCache.h
threadEdbPattern.o: /var/local/root/include/THashList.h
threadEdbPattern.o: /var/local/root/include/TClonesArray.h
threadEdbPattern.o: /var/local/root/include/TPluginManager.h
threadEdbPattern.o: /var/local/root/include/TVirtualUtilHist.h
threadEdbPattern.o: /var/local/root/include/TMatrixFUtils.h
threadEdbPattern.o: /var/local/root/include/TGraphErrors.h
threadEdbPattern.o: /var/local/root/include/TGraph.h
threadEdbPattern.o: /var/local/root/include/TH1.h
threadEdbPattern.o: /var/local/root/include/TVirtualHistPainter.h
threadEdbPattern.o: /var/local/root/include/TAxis.h
threadEdbPattern.o: /var/local/root/include/TAttAxis.h
threadEdbPattern.o: /var/local/root/include/Gtypes.h
threadEdbPattern.o: /var/local/root/include/TArrayD.h
threadEdbPattern.o: /var/local/root/include/TArray.h
threadEdbPattern.o: /var/local/root/include/TAttMarker.h
threadEdbPattern.o: /var/local/root/include/TArrayC.h
threadEdbPattern.o: /var/local/root/include/TArrayS.h
threadEdbPattern.o: /var/local/root/include/TArrayI.h
threadEdbPattern.o: /var/local/root/include/TArrayF.h
threadEdbPattern.o: /var/local/root/include/Foption.h
threadEdbVertex.o: /var/local/root/include/TFile.h
threadEdbVertex.o: /var/local/root/include/TCache.h
threadEdbVertex.o: /var/local/root/include/THashList.h
threadEdbVertex.o: /var/local/root/include/TRandom.h
threadEdbVertex.o: /var/local/root/include/TNamed.h
threadEdbVertex.o: /var/local/root/include/TObject.h
threadEdbVertex.o: /var/local/root/include/Rtypes.h
threadEdbVertex.o: /var/local/root/include/RConfig.h
threadEdbVertex.o: /var/local/root/include/RVersion.h
threadEdbVertex.o: /var/local/root/include/DllImport.h
threadEdbVertex.o: /var/local/root/include/Rtypeinfo.h
threadEdbVertex.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
threadEdbVertex.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
threadEdbVertex.o: /usr/include/stdio.h /usr/include/features.h
threadEdbVertex.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
threadEdbVertex.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
threadEdbVertex.o: /usr/include/bits/types.h /usr/include/libio.h
threadEdbVertex.o: /usr/include/_G_config.h /usr/include/wchar.h
threadEdbVertex.o: /usr/include/bits/wchar.h /usr/include/gconv.h
threadEdbVertex.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
threadEdbVertex.o: /usr/include/bits/stdio_lim.h
threadEdbVertex.o: /var/local/root/include/TGenericClassInfo.h
threadEdbVertex.o: /var/local/root/include/Varargs.h
threadEdbVertex.o: /var/local/root/include/TStorage.h
threadEdbVertex.o: /var/local/root/include/Riosfwd.h
threadEdbVertex.o: /var/local/root/include/TBuffer.h
threadEdbVertex.o: /var/local/root/include/Bytes.h /usr/include/string.h
threadEdbVertex.o: /var/local/root/include/TList.h
threadEdbVertex.o: /var/local/root/include/TSeqCollection.h
threadEdbVertex.o: /var/local/root/include/TCollection.h
threadEdbVertex.o: /var/local/root/include/TIterator.h
threadEdbVertex.o: /var/local/root/include/TString.h
threadEdbVertex.o: /var/local/root/include/TMath.h
threadEdbVertex.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
threadEdbVertex.o: /var/local/root/include/TRefCnt.h EdbVertex.h
threadEdbVertex.o: /var/local/root/include/TTree.h
threadEdbVertex.o: /var/local/root/include/TBranch.h
threadEdbVertex.o: /var/local/root/include/TStringLong.h
threadEdbVertex.o: /var/local/root/include/TVirtualTreePlayer.h
threadEdbVertex.o: /var/local/root/include/TVector3.h
threadEdbVertex.o: /var/local/root/include/TError.h
threadEdbVertex.o: /var/local/root/include/TVector2.h
threadEdbVertex.o: /var/local/root/include/TMatrix.h
threadEdbVertex.o: /var/local/root/include/TMatrixF.h
threadEdbVertex.o: /var/local/root/include/TMatrixFBase.h
threadEdbVertex.o: /var/local/root/include/TROOT.h
threadEdbVertex.o: /var/local/root/include/TDirectory.h
threadEdbVertex.o: /var/local/root/include/TDatime.h
threadEdbVertex.o: /var/local/root/include/Htypes.h
threadEdbVertex.o: /var/local/root/include/TUUID.h
threadEdbVertex.o: /var/local/root/include/TClass.h
threadEdbVertex.o: /var/local/root/include/TDictionary.h
threadEdbVertex.o: /var/local/root/include/Property.h
threadEdbVertex.o: /var/local/root/include/TObjArray.h
threadEdbVertex.o: /var/local/root/include/TStreamerInfo.h
threadEdbVertex.o: /var/local/root/include/TClonesArray.h
threadEdbVertex.o: /var/local/root/include/TPluginManager.h
threadEdbVertex.o: /var/local/root/include/TVirtualUtilHist.h
threadEdbVertex.o: /var/local/root/include/TMatrixFUtils.h
threadEdbVertex.o: /var/local/root/include/TGraphErrors.h
threadEdbVertex.o: /var/local/root/include/TGraph.h
threadEdbVertex.o: /var/local/root/include/TH1.h
threadEdbVertex.o: /var/local/root/include/TVirtualHistPainter.h
threadEdbVertex.o: /var/local/root/include/TAxis.h
threadEdbVertex.o: /var/local/root/include/TAttAxis.h
threadEdbVertex.o: /var/local/root/include/Gtypes.h
threadEdbVertex.o: /var/local/root/include/TArrayD.h
threadEdbVertex.o: /var/local/root/include/TArray.h
threadEdbVertex.o: /var/local/root/include/TAttLine.h
threadEdbVertex.o: /var/local/root/include/TAttFill.h
threadEdbVertex.o: /var/local/root/include/TAttMarker.h
threadEdbVertex.o: /var/local/root/include/TArrayC.h
threadEdbVertex.o: /var/local/root/include/TArrayS.h
threadEdbVertex.o: /var/local/root/include/TArrayI.h
threadEdbVertex.o: /var/local/root/include/TArrayF.h
threadEdbVertex.o: /var/local/root/include/Foption.h EdbPattern.h
threadEdbVertex.o: /var/local/root/include/TSortedList.h
threadEdbVertex.o: /var/local/root/include/TArrayL.h
threadEdbVertex.o: /var/local/root/include/TMatrixD.h
threadEdbVertex.o: /var/local/root/include/TMatrixDBase.h
threadEdbVertex.o: /usr/include/limits.h
threadEdbVertex.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
threadEdbVertex.o: /var/local/root/include/TMatrixDUtils.h
