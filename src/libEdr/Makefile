#------------------------------------------------------------------------------
include ../config/ProjectDef.mk
include ../config/RootDef.mk
#------------------------------------------------------------------------------

PROJECT_LIBS += -lvt -lEmath -lEdb -lEphys

LIB_TGT       = $(LIB_DIR)/libEdr.so

headers       = EdbPattern.h EdbPVGen.h EdbPVRec.h EdbGA.h EdbDataSet.h EdbIP.h EdbVertex.h

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
EdbDataSet.o: /var/local/root/include/THashList.h EdbPVRec.h EdbPattern.h
EdbDataSet.o: /var/local/root/include/TTree.h
EdbDataSet.o: /var/local/root/include/TClonesArray.h
EdbDataSet.o: /var/local/root/include/TAttLine.h
EdbDataSet.o: /var/local/root/include/Gtypes.h
EdbDataSet.o: /var/local/root/include/TAttFill.h
EdbDataSet.o: /var/local/root/include/TAttMarker.h
EdbDataSet.o: /var/local/root/include/TBranch.h
EdbDataSet.o: /var/local/root/include/TStringLong.h
EdbDataSet.o: /var/local/root/include/TArrayD.h
EdbDataSet.o: /var/local/root/include/TArray.h
EdbDataSet.o: /var/local/root/include/TArrayI.h
EdbDataSet.o: /var/local/root/include/TVirtualTreePlayer.h
EdbDataSet.o: /var/local/root/include/TClass.h
EdbDataSet.o: /var/local/root/include/TDictionary.h
EdbDataSet.o: /var/local/root/include/Property.h
EdbDataSet.o: /var/local/root/include/TSortedList.h
EdbDataSet.o: /var/local/root/include/TArrayL.h
EdbDataSet.o: /var/local/root/include/TMatrixD.h
EdbDataSet.o: /var/local/root/include/TVectorD.h
EdbDataSet.o: /var/local/root/include/TMatrixDUtils.h
EdbGA.o: EdbGA.h EdbIP.h /var/local/root/include/TNamed.h
EdbGA.o: /var/local/root/include/TList.h /var/local/root/include/TArrayF.h
EdbGA.o: /var/local/root/include/TArray.h /usr/include/string.h
EdbGA.o: /usr/include/features.h /usr/include/sys/cdefs.h
EdbGA.o: /usr/include/gnu/stubs.h
EdbGA.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
EdbIP.o: /var/local/root/include/TH1.h
EdbIP.o: /var/local/root/include/TVirtualHistPainter.h
EdbIP.o: /var/local/root/include/TClass.h
EdbIP.o: /var/local/root/include/TDictionary.h
EdbIP.o: /var/local/root/include/Property.h /var/local/root/include/TAxis.h
EdbIP.o: /var/local/root/include/TNamed.h /var/local/root/include/TList.h
EdbIP.o: /var/local/root/include/TAttAxis.h /var/local/root/include/Gtypes.h
EdbIP.o: /var/local/root/include/Riosfwd.h /var/local/root/include/TArrayD.h
EdbIP.o: /var/local/root/include/TArray.h /usr/include/string.h
EdbIP.o: /usr/include/features.h /usr/include/sys/cdefs.h
EdbIP.o: /usr/include/gnu/stubs.h
EdbIP.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
EdbIP.o: /var/local/root/include/THashList.h
EdbIP.o: /var/local/root/include/TAttLine.h
EdbIP.o: /var/local/root/include/TAttFill.h
EdbIP.o: /var/local/root/include/TAttMarker.h
EdbIP.o: /var/local/root/include/TArrayC.h /var/local/root/include/TArrayS.h
EdbIP.o: /var/local/root/include/TArrayI.h /var/local/root/include/TArrayF.h
EdbIP.o: /var/local/root/include/Foption.h /var/local/root/include/TH2.h
EdbIP.o: /var/local/root/include/TTree.h
EdbIP.o: /var/local/root/include/TClonesArray.h
EdbIP.o: /var/local/root/include/TBranch.h
EdbIP.o: /var/local/root/include/TStringLong.h
EdbIP.o: /var/local/root/include/TVirtualTreePlayer.h
EdbIP.o: /var/local/root/include/TFile.h /var/local/root/include/TDirectory.h
EdbIP.o: /var/local/root/include/TDatime.h /var/local/root/include/Htypes.h
EdbIP.o: /var/local/root/include/TUUID.h
EdbIP.o: /var/local/root/include/TInetAddress.h
EdbIP.o: /var/local/root/include/TCache.h EdbIP.h
EdbPVGen.o: /var/local/root/include/TRandom.h
EdbPVGen.o: /var/local/root/include/TNamed.h /var/local/root/include/TList.h
EdbPVGen.o: EdbPVGen.h EdbPVRec.h /var/local/root/include/THashList.h
EdbPVGen.o: EdbPattern.h /var/local/root/include/TTree.h
EdbPVGen.o: /var/local/root/include/TClonesArray.h
EdbPVGen.o: /var/local/root/include/TAttLine.h
EdbPVGen.o: /var/local/root/include/Gtypes.h
EdbPVGen.o: /var/local/root/include/TAttFill.h
EdbPVGen.o: /var/local/root/include/TAttMarker.h
EdbPVGen.o: /var/local/root/include/TBranch.h
EdbPVGen.o: /var/local/root/include/TStringLong.h
EdbPVGen.o: /var/local/root/include/TArrayD.h
EdbPVGen.o: /var/local/root/include/TArray.h /usr/include/string.h
EdbPVGen.o: /usr/include/features.h /usr/include/sys/cdefs.h
EdbPVGen.o: /usr/include/gnu/stubs.h
EdbPVGen.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
EdbPVGen.o: /var/local/root/include/TArrayI.h
EdbPVGen.o: /var/local/root/include/TVirtualTreePlayer.h
EdbPVGen.o: /var/local/root/include/TClass.h
EdbPVGen.o: /var/local/root/include/TDictionary.h
EdbPVGen.o: /var/local/root/include/Property.h
EdbPVGen.o: /var/local/root/include/TObjArray.h
EdbPVGen.o: /var/local/root/include/TSeqCollection.h
EdbPVGen.o: /var/local/root/include/TCollection.h
EdbPVGen.o: /var/local/root/include/TIterator.h
EdbPVGen.o: /var/local/root/include/TSortedList.h
EdbPVGen.o: /var/local/root/include/TArrayL.h
EdbPVGen.o: /var/local/root/include/TMatrixD.h
EdbPVGen.o: /var/local/root/include/TVectorD.h
EdbPVGen.o: /var/local/root/include/TMatrixDUtils.h
EdbPVRec.o: /var/local/root/include/TFile.h
EdbPVRec.o: /var/local/root/include/TDirectory.h
EdbPVRec.o: /var/local/root/include/TNamed.h /var/local/root/include/TList.h
EdbPVRec.o: /var/local/root/include/TDatime.h
EdbPVRec.o: /var/local/root/include/Htypes.h /var/local/root/include/TUUID.h
EdbPVRec.o: /var/local/root/include/TInetAddress.h
EdbPVRec.o: /var/local/root/include/TCache.h
EdbPVRec.o: /var/local/root/include/THashList.h
EdbPVRec.o: /var/local/root/include/TVector3.h
EdbPVRec.o: /var/local/root/include/TMath.h
EdbPVRec.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
EdbPVRec.o: /var/local/root/include/TError.h
EdbPVRec.o: /var/local/root/include/TVector2.h
EdbPVRec.o: /var/local/root/include/TObject.h
EdbPVRec.o: /var/local/root/include/Rtypes.h
EdbPVRec.o: /var/local/root/include/RConfig.h
EdbPVRec.o: /var/local/root/include/RVersion.h
EdbPVRec.o: /var/local/root/include/DllImport.h
EdbPVRec.o: /var/local/root/include/Rtypeinfo.h
EdbPVRec.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
EdbPVRec.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
EdbPVRec.o: /usr/include/stdio.h /usr/include/features.h
EdbPVRec.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
EdbPVRec.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
EdbPVRec.o: /usr/include/bits/types.h /usr/include/libio.h
EdbPVRec.o: /usr/include/_G_config.h /usr/include/wchar.h
EdbPVRec.o: /usr/include/bits/wchar.h /usr/include/gconv.h
EdbPVRec.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
EdbPVRec.o: /usr/include/bits/stdio_lim.h
EdbPVRec.o: /var/local/root/include/TGenericClassInfo.h
EdbPVRec.o: /var/local/root/include/Varargs.h
EdbPVRec.o: /var/local/root/include/TStorage.h
EdbPVRec.o: /var/local/root/include/Riosfwd.h
EdbPVRec.o: /var/local/root/include/TBuffer.h /var/local/root/include/Bytes.h
EdbPVRec.o: /usr/include/string.h /var/local/root/include/TMatrix.h
EdbPVRec.o: /var/local/root/include/TVector.h
EdbPVRec.o: /var/local/root/include/TString.h
EdbPVRec.o: /var/local/root/include/TRefCnt.h
EdbPVRec.o: /var/local/root/include/TMatrixUtils.h
EdbPVRec.o: /var/local/root/include/TArrayF.h
EdbPVRec.o: /var/local/root/include/TArray.h EdbPVRec.h EdbPattern.h
EdbPVRec.o: /var/local/root/include/TTree.h
EdbPVRec.o: /var/local/root/include/TClonesArray.h
EdbPVRec.o: /var/local/root/include/TAttLine.h
EdbPVRec.o: /var/local/root/include/Gtypes.h
EdbPVRec.o: /var/local/root/include/TAttFill.h
EdbPVRec.o: /var/local/root/include/TAttMarker.h
EdbPVRec.o: /var/local/root/include/TBranch.h
EdbPVRec.o: /var/local/root/include/TStringLong.h
EdbPVRec.o: /var/local/root/include/TArrayD.h
EdbPVRec.o: /var/local/root/include/TArrayI.h
EdbPVRec.o: /var/local/root/include/TVirtualTreePlayer.h
EdbPVRec.o: /var/local/root/include/TClass.h
EdbPVRec.o: /var/local/root/include/TDictionary.h
EdbPVRec.o: /var/local/root/include/Property.h
EdbPVRec.o: /var/local/root/include/TObjArray.h
EdbPVRec.o: /var/local/root/include/TSeqCollection.h
EdbPVRec.o: /var/local/root/include/TCollection.h
EdbPVRec.o: /var/local/root/include/TIterator.h
EdbPVRec.o: /var/local/root/include/TSortedList.h
EdbPVRec.o: /var/local/root/include/TArrayL.h
EdbPVRec.o: /var/local/root/include/TMatrixD.h
EdbPVRec.o: /var/local/root/include/TVectorD.h
EdbPVRec.o: /var/local/root/include/TMatrixDUtils.h EdbVertex.h
EdbPVRec.o: /var/local/root/include/TGraphErrors.h
EdbPVRec.o: /var/local/root/include/TGraph.h /var/local/root/include/TH1.h
EdbPVRec.o: /var/local/root/include/TVirtualHistPainter.h
EdbPVRec.o: /var/local/root/include/TAxis.h
EdbPVRec.o: /var/local/root/include/TAttAxis.h
EdbPVRec.o: /var/local/root/include/TArrayC.h
EdbPVRec.o: /var/local/root/include/TArrayS.h
EdbPVRec.o: /var/local/root/include/Foption.h EdbPVGen.h
EdbPVRec.o: /var/local/root/include/TBenchmark.h
EdbPVRec.o: /var/local/root/include/TStopwatch.h
EdbPattern.o: EdbPattern.h /var/local/root/include/TTree.h
EdbPattern.o: /var/local/root/include/TClonesArray.h
EdbPattern.o: /var/local/root/include/TAttLine.h
EdbPattern.o: /var/local/root/include/Gtypes.h
EdbPattern.o: /var/local/root/include/TAttFill.h
EdbPattern.o: /var/local/root/include/TAttMarker.h
EdbPattern.o: /var/local/root/include/TBranch.h
EdbPattern.o: /var/local/root/include/TStringLong.h
EdbPattern.o: /var/local/root/include/TArrayD.h
EdbPattern.o: /var/local/root/include/TArray.h /usr/include/string.h
EdbPattern.o: /usr/include/features.h /usr/include/sys/cdefs.h
EdbPattern.o: /usr/include/gnu/stubs.h
EdbPattern.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
EdbPattern.o: /var/local/root/include/TArrayI.h
EdbPattern.o: /var/local/root/include/TVirtualTreePlayer.h
EdbPattern.o: /var/local/root/include/TClass.h
EdbPattern.o: /var/local/root/include/TDictionary.h
EdbPattern.o: /var/local/root/include/Property.h
EdbPattern.o: /var/local/root/include/TObjArray.h
EdbPattern.o: /var/local/root/include/TSeqCollection.h
EdbPattern.o: /var/local/root/include/TCollection.h
EdbPattern.o: /var/local/root/include/TIterator.h
EdbPattern.o: /var/local/root/include/TSortedList.h
EdbPattern.o: /var/local/root/include/TArrayL.h
EdbPattern.o: /var/local/root/include/TMatrixD.h
EdbPattern.o: /var/local/root/include/TVectorD.h
EdbPattern.o: /var/local/root/include/TMatrixDUtils.h EdbVertex.h
EdbPattern.o: /var/local/root/include/TObject.h
EdbPattern.o: /var/local/root/include/Rtypes.h
EdbPattern.o: /var/local/root/include/RConfig.h
EdbPattern.o: /var/local/root/include/RVersion.h
EdbPattern.o: /var/local/root/include/DllImport.h
EdbPattern.o: /var/local/root/include/Rtypeinfo.h
EdbPattern.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
EdbPattern.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
EdbPattern.o: /usr/include/stdio.h /usr/include/bits/types.h
EdbPattern.o: /usr/include/libio.h /usr/include/_G_config.h
EdbPattern.o: /usr/include/wchar.h /usr/include/bits/wchar.h
EdbPattern.o: /usr/include/gconv.h
EdbPattern.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
EdbPattern.o: /usr/include/bits/stdio_lim.h
EdbPattern.o: /var/local/root/include/TGenericClassInfo.h
EdbPattern.o: /var/local/root/include/Varargs.h
EdbPattern.o: /var/local/root/include/TStorage.h
EdbPattern.o: /var/local/root/include/Riosfwd.h
EdbPattern.o: /var/local/root/include/TBuffer.h
EdbPattern.o: /var/local/root/include/Bytes.h
EdbPattern.o: /var/local/root/include/TVector3.h
EdbPattern.o: /var/local/root/include/TMath.h
EdbPattern.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
EdbPattern.o: /var/local/root/include/TError.h
EdbPattern.o: /var/local/root/include/TVector2.h
EdbPattern.o: /var/local/root/include/TMatrix.h
EdbPattern.o: /var/local/root/include/TVector.h
EdbPattern.o: /var/local/root/include/TString.h
EdbPattern.o: /var/local/root/include/TRefCnt.h
EdbPattern.o: /var/local/root/include/TMatrixUtils.h
EdbPattern.o: /var/local/root/include/TGraphErrors.h
EdbPattern.o: /var/local/root/include/TGraph.h /var/local/root/include/TH1.h
EdbPattern.o: /var/local/root/include/TVirtualHistPainter.h
EdbPattern.o: /var/local/root/include/TAxis.h
EdbPattern.o: /var/local/root/include/TNamed.h
EdbPattern.o: /var/local/root/include/TList.h
EdbPattern.o: /var/local/root/include/TAttAxis.h
EdbPattern.o: /var/local/root/include/THashList.h
EdbPattern.o: /var/local/root/include/TArrayC.h
EdbPattern.o: /var/local/root/include/TArrayS.h
EdbPattern.o: /var/local/root/include/TArrayF.h
EdbPattern.o: /var/local/root/include/Foption.h EdbPVGen.h EdbPVRec.h
EdbVertex.o: /var/local/root/include/TFile.h
EdbVertex.o: /var/local/root/include/TDirectory.h
EdbVertex.o: /var/local/root/include/TNamed.h /var/local/root/include/TList.h
EdbVertex.o: /var/local/root/include/TDatime.h
EdbVertex.o: /var/local/root/include/Htypes.h /var/local/root/include/TUUID.h
EdbVertex.o: /var/local/root/include/TInetAddress.h
EdbVertex.o: /var/local/root/include/TCache.h
EdbVertex.o: /var/local/root/include/THashList.h
EdbVertex.o: /var/local/root/include/TRandom.h
EdbVertex.o: /var/local/root/include/TPolyLine.h
EdbVertex.o: /var/local/root/include/TString.h /usr/include/string.h
EdbVertex.o: /usr/include/features.h /usr/include/sys/cdefs.h
EdbVertex.o: /usr/include/gnu/stubs.h
EdbVertex.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
EdbVertex.o: /usr/include/stdio.h /usr/include/bits/types.h
EdbVertex.o: /usr/include/libio.h /usr/include/_G_config.h
EdbVertex.o: /usr/include/wchar.h /usr/include/bits/wchar.h
EdbVertex.o: /usr/include/gconv.h
EdbVertex.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
EdbVertex.o: /usr/include/bits/stdio_lim.h /var/local/root/include/TRefCnt.h
EdbVertex.o: /var/local/root/include/TObject.h
EdbVertex.o: /var/local/root/include/Rtypes.h
EdbVertex.o: /var/local/root/include/RConfig.h
EdbVertex.o: /var/local/root/include/RVersion.h
EdbVertex.o: /var/local/root/include/DllImport.h
EdbVertex.o: /var/local/root/include/Rtypeinfo.h
EdbVertex.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
EdbVertex.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
EdbVertex.o: /var/local/root/include/TGenericClassInfo.h
EdbVertex.o: /var/local/root/include/Varargs.h
EdbVertex.o: /var/local/root/include/TStorage.h
EdbVertex.o: /var/local/root/include/Riosfwd.h
EdbVertex.o: /var/local/root/include/TBuffer.h
EdbVertex.o: /var/local/root/include/Bytes.h
EdbVertex.o: /var/local/root/include/TAttLine.h
EdbVertex.o: /var/local/root/include/Gtypes.h
EdbVertex.o: /var/local/root/include/TAttFill.h EdbVertex.h
EdbVertex.o: /var/local/root/include/TTree.h
EdbVertex.o: /var/local/root/include/TClonesArray.h
EdbVertex.o: /var/local/root/include/TAttMarker.h
EdbVertex.o: /var/local/root/include/TBranch.h
EdbVertex.o: /var/local/root/include/TStringLong.h
EdbVertex.o: /var/local/root/include/TArrayD.h
EdbVertex.o: /var/local/root/include/TArray.h
EdbVertex.o: /var/local/root/include/TArrayI.h
EdbVertex.o: /var/local/root/include/TVirtualTreePlayer.h
EdbVertex.o: /var/local/root/include/TClass.h
EdbVertex.o: /var/local/root/include/TDictionary.h
EdbVertex.o: /var/local/root/include/Property.h
EdbVertex.o: /var/local/root/include/TVector3.h
EdbVertex.o: /var/local/root/include/TMath.h
EdbVertex.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
EdbVertex.o: /var/local/root/include/TError.h
EdbVertex.o: /var/local/root/include/TVector2.h
EdbVertex.o: /var/local/root/include/TMatrix.h
EdbVertex.o: /var/local/root/include/TVector.h
EdbVertex.o: /var/local/root/include/TMatrixUtils.h
EdbVertex.o: /var/local/root/include/TGraphErrors.h
EdbVertex.o: /var/local/root/include/TGraph.h /var/local/root/include/TH1.h
EdbVertex.o: /var/local/root/include/TVirtualHistPainter.h
EdbVertex.o: /var/local/root/include/TAxis.h
EdbVertex.o: /var/local/root/include/TAttAxis.h
EdbVertex.o: /var/local/root/include/TArrayC.h
EdbVertex.o: /var/local/root/include/TArrayS.h
EdbVertex.o: /var/local/root/include/TArrayF.h
EdbVertex.o: /var/local/root/include/Foption.h EdbPattern.h
EdbVertex.o: /var/local/root/include/TObjArray.h
EdbVertex.o: /var/local/root/include/TSeqCollection.h
EdbVertex.o: /var/local/root/include/TCollection.h
EdbVertex.o: /var/local/root/include/TIterator.h
EdbVertex.o: /var/local/root/include/TSortedList.h
EdbVertex.o: /var/local/root/include/TArrayL.h
EdbVertex.o: /var/local/root/include/TMatrixD.h
EdbVertex.o: /var/local/root/include/TVectorD.h
EdbVertex.o: /var/local/root/include/TMatrixDUtils.h EdbPVGen.h EdbPVRec.h
