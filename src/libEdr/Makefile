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

EdbGA.o: EdbGA.h EdbIP.h /var/local/root/include/TNamed.h
EdbGA.o: /var/local/root/include/TObject.h /var/local/root/include/Rtypes.h
EdbGA.o: /var/local/root/include/RConfig.h /var/local/root/include/RVersion.h
EdbGA.o: /var/local/root/include/DllImport.h
EdbGA.o: /var/local/root/include/Rtypeinfo.h
EdbGA.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
EdbGA.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
EdbGA.o: /usr/include/stdio.h /usr/include/features.h
EdbGA.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
EdbGA.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
EdbGA.o: /usr/include/bits/types.h /usr/include/libio.h
EdbGA.o: /usr/include/_G_config.h /usr/include/wchar.h
EdbGA.o: /usr/include/bits/wchar.h /usr/include/gconv.h
EdbGA.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
EdbGA.o: /usr/include/bits/stdio_lim.h
EdbGA.o: /var/local/root/include/TGenericClassInfo.h
EdbGA.o: /var/local/root/include/Varargs.h /var/local/root/include/TStorage.h
EdbGA.o: /var/local/root/include/Riosfwd.h /var/local/root/include/TBuffer.h
EdbGA.o: /var/local/root/include/Bytes.h /usr/include/string.h
EdbGA.o: /var/local/root/include/TList.h
EdbGA.o: /var/local/root/include/TSeqCollection.h
EdbGA.o: /var/local/root/include/TCollection.h
EdbGA.o: /var/local/root/include/TIterator.h
EdbGA.o: /var/local/root/include/TString.h /var/local/root/include/TMath.h
EdbGA.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
EdbGA.o: /var/local/root/include/TRefCnt.h /var/local/root/include/TArrayF.h
EdbGA.o: /var/local/root/include/TArray.h /var/local/root/include/Htypes.h
EdbIP.o: /var/local/root/include/TH1.h
EdbIP.o: /var/local/root/include/TVirtualHistPainter.h
EdbIP.o: /var/local/root/include/TClass.h
EdbIP.o: /var/local/root/include/TDictionary.h
EdbIP.o: /var/local/root/include/TNamed.h /var/local/root/include/TObject.h
EdbIP.o: /var/local/root/include/Rtypes.h /var/local/root/include/RConfig.h
EdbIP.o: /var/local/root/include/RVersion.h
EdbIP.o: /var/local/root/include/DllImport.h
EdbIP.o: /var/local/root/include/Rtypeinfo.h
EdbIP.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
EdbIP.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
EdbIP.o: /usr/include/stdio.h /usr/include/features.h
EdbIP.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
EdbIP.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
EdbIP.o: /usr/include/bits/types.h /usr/include/libio.h
EdbIP.o: /usr/include/_G_config.h /usr/include/wchar.h
EdbIP.o: /usr/include/bits/wchar.h /usr/include/gconv.h
EdbIP.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
EdbIP.o: /usr/include/bits/stdio_lim.h
EdbIP.o: /var/local/root/include/TGenericClassInfo.h
EdbIP.o: /var/local/root/include/Varargs.h /var/local/root/include/TStorage.h
EdbIP.o: /var/local/root/include/Riosfwd.h /var/local/root/include/TBuffer.h
EdbIP.o: /var/local/root/include/Bytes.h /usr/include/string.h
EdbIP.o: /var/local/root/include/TList.h
EdbIP.o: /var/local/root/include/TSeqCollection.h
EdbIP.o: /var/local/root/include/TCollection.h
EdbIP.o: /var/local/root/include/TIterator.h
EdbIP.o: /var/local/root/include/TString.h /var/local/root/include/TMath.h
EdbIP.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
EdbIP.o: /var/local/root/include/TRefCnt.h /var/local/root/include/Property.h
EdbIP.o: /var/local/root/include/TAxis.h /var/local/root/include/TAttAxis.h
EdbIP.o: /var/local/root/include/Gtypes.h /var/local/root/include/Htypes.h
EdbIP.o: /var/local/root/include/TArrayD.h /var/local/root/include/TArray.h
EdbIP.o: /var/local/root/include/THashList.h
EdbIP.o: /var/local/root/include/TAttLine.h
EdbIP.o: /var/local/root/include/TAttFill.h
EdbIP.o: /var/local/root/include/TAttMarker.h
EdbIP.o: /var/local/root/include/TArrayC.h /var/local/root/include/TArrayS.h
EdbIP.o: /var/local/root/include/TArrayI.h /var/local/root/include/TArrayF.h
EdbIP.o: /var/local/root/include/Foption.h /var/local/root/include/TH2.h
EdbIP.o: /var/local/root/include/TTree.h /var/local/root/include/TObjArray.h
EdbIP.o: /var/local/root/include/TClonesArray.h
EdbIP.o: /var/local/root/include/TBranch.h
EdbIP.o: /var/local/root/include/TStringLong.h
EdbIP.o: /var/local/root/include/TVirtualTreePlayer.h
EdbIP.o: /var/local/root/include/TFile.h /var/local/root/include/TDirectory.h
EdbIP.o: /var/local/root/include/TDatime.h /var/local/root/include/TUUID.h
EdbIP.o: /var/local/root/include/TInetAddress.h
EdbIP.o: /var/local/root/include/TCache.h EdbIP.h
EdbPVRec.o: /var/local/root/include/TFile.h
EdbPVRec.o: /var/local/root/include/TDirectory.h
EdbPVRec.o: /var/local/root/include/TDatime.h /var/local/root/include/TUUID.h
EdbPVRec.o: /var/local/root/include/TInetAddress.h
EdbPVRec.o: /var/local/root/include/TCache.h
EdbPVRec.o: /var/local/root/include/TVector3.h
EdbPVRec.o: /var/local/root/include/TMath.h
EdbPVRec.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
EdbPVRec.o: /var/local/root/include/TError.h /var/local/root/include/Rtypes.h
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
EdbPVRec.o: /var/local/root/include/TVector2.h
EdbPVRec.o: /var/local/root/include/TObject.h
EdbPVRec.o: /var/local/root/include/TStorage.h
EdbPVRec.o: /var/local/root/include/Riosfwd.h
EdbPVRec.o: /var/local/root/include/TBuffer.h /var/local/root/include/Bytes.h
EdbPVRec.o: /usr/include/string.h /var/local/root/include/TMatrix.h
EdbPVRec.o: /var/local/root/include/TVector.h
EdbPVRec.o: /var/local/root/include/TString.h
EdbPVRec.o: /var/local/root/include/TRefCnt.h
EdbPVRec.o: /var/local/root/include/TMatrixUtils.h
EdbPVRec.o: /var/local/root/include/TArrayF.h
EdbPVRec.o: /var/local/root/include/TArray.h /var/local/root/include/Htypes.h
EdbPVRec.o: /var/local/root/include/TBenchmark.h
EdbPVRec.o: /var/local/root/include/TNamed.h /var/local/root/include/TList.h
EdbPVRec.o: /var/local/root/include/TSeqCollection.h
EdbPVRec.o: /var/local/root/include/TCollection.h
EdbPVRec.o: /var/local/root/include/TIterator.h
EdbPVRec.o: /var/local/root/include/TStopwatch.h EdbVertex.h
EdbPVRec.o: /var/local/root/include/TTree.h
EdbPVRec.o: /var/local/root/include/TObjArray.h
EdbPVRec.o: /var/local/root/include/TClonesArray.h
EdbPVRec.o: /var/local/root/include/TBranch.h
EdbPVRec.o: /var/local/root/include/TStringLong.h
EdbPVRec.o: /var/local/root/include/TVirtualTreePlayer.h
EdbPVRec.o: /var/local/root/include/TGraphErrors.h
EdbPVRec.o: /var/local/root/include/TGraph.h /var/local/root/include/TH1.h
EdbPVRec.o: /var/local/root/include/TVirtualHistPainter.h
EdbPVRec.o: /var/local/root/include/TClass.h
EdbPVRec.o: /var/local/root/include/TDictionary.h
EdbPVRec.o: /var/local/root/include/Property.h
EdbPVRec.o: /var/local/root/include/TAxis.h
EdbPVRec.o: /var/local/root/include/TAttAxis.h
EdbPVRec.o: /var/local/root/include/Gtypes.h
EdbPVRec.o: /var/local/root/include/TArrayD.h
EdbPVRec.o: /var/local/root/include/THashList.h
EdbPVRec.o: /var/local/root/include/TAttLine.h
EdbPVRec.o: /var/local/root/include/TAttFill.h
EdbPVRec.o: /var/local/root/include/TAttMarker.h
EdbPVRec.o: /var/local/root/include/TArrayC.h
EdbPVRec.o: /var/local/root/include/TArrayS.h
EdbPVRec.o: /var/local/root/include/TArrayI.h
EdbPVRec.o: /var/local/root/include/Foption.h EdbPattern.h
EdbPVRec.o: /var/local/root/include/TSortedList.h
EdbPVRec.o: /var/local/root/include/TArrayL.h
EdbPVRec.o: /var/local/root/include/TMatrixD.h
EdbPVRec.o: /var/local/root/include/TVectorD.h
EdbPVRec.o: /var/local/root/include/TMatrixDUtils.h EdbPVRec.h
EdbPattern.o: /var/local/root/include/TPolyLine.h
EdbPattern.o: /var/local/root/include/TString.h /usr/include/string.h
EdbPattern.o: /usr/include/features.h /usr/include/sys/cdefs.h
EdbPattern.o: /usr/include/gnu/stubs.h
EdbPattern.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
EdbPattern.o: /usr/include/stdio.h /usr/include/bits/types.h
EdbPattern.o: /usr/include/libio.h /usr/include/_G_config.h
EdbPattern.o: /usr/include/wchar.h /usr/include/bits/wchar.h
EdbPattern.o: /usr/include/gconv.h
EdbPattern.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
EdbPattern.o: /usr/include/bits/stdio_lim.h /var/local/root/include/TMath.h
EdbPattern.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
EdbPattern.o: /var/local/root/include/TRefCnt.h
EdbPattern.o: /var/local/root/include/TObject.h
EdbPattern.o: /var/local/root/include/Rtypes.h
EdbPattern.o: /var/local/root/include/RConfig.h
EdbPattern.o: /var/local/root/include/RVersion.h
EdbPattern.o: /var/local/root/include/DllImport.h
EdbPattern.o: /var/local/root/include/Rtypeinfo.h
EdbPattern.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
EdbPattern.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
EdbPattern.o: /var/local/root/include/TGenericClassInfo.h
EdbPattern.o: /var/local/root/include/Varargs.h
EdbPattern.o: /var/local/root/include/TStorage.h
EdbPattern.o: /var/local/root/include/Riosfwd.h
EdbPattern.o: /var/local/root/include/TBuffer.h
EdbPattern.o: /var/local/root/include/Bytes.h
EdbPattern.o: /var/local/root/include/TAttLine.h
EdbPattern.o: /var/local/root/include/TAttFill.h EdbPattern.h
EdbPattern.o: /var/local/root/include/TTree.h
EdbPattern.o: /var/local/root/include/TObjArray.h
EdbPattern.o: /var/local/root/include/TClonesArray.h
EdbPattern.o: /var/local/root/include/TBranch.h
EdbPattern.o: /var/local/root/include/TStringLong.h
EdbPattern.o: /var/local/root/include/TVirtualTreePlayer.h
EdbPattern.o: /var/local/root/include/TSortedList.h
EdbPattern.o: /var/local/root/include/TArrayL.h
EdbPattern.o: /var/local/root/include/TMatrixD.h
EdbPattern.o: /var/local/root/include/TVectorD.h
EdbPattern.o: /var/local/root/include/TMatrixDUtils.h EdbVertex.h
EdbPattern.o: /var/local/root/include/TVector3.h
EdbPattern.o: /var/local/root/include/TError.h
EdbPattern.o: /var/local/root/include/TVector2.h
EdbPattern.o: /var/local/root/include/TMatrix.h
EdbPattern.o: /var/local/root/include/TVector.h
EdbPattern.o: /var/local/root/include/TMatrixUtils.h
EdbPattern.o: /var/local/root/include/TGraphErrors.h
EdbPattern.o: /var/local/root/include/TGraph.h /var/local/root/include/TH1.h
EdbPattern.o: /var/local/root/include/TVirtualHistPainter.h
EdbPattern.o: /var/local/root/include/TClass.h
EdbPattern.o: /var/local/root/include/TDictionary.h
EdbPattern.o: /var/local/root/include/TNamed.h
EdbPattern.o: /var/local/root/include/TList.h
EdbPattern.o: /var/local/root/include/TSeqCollection.h
EdbPattern.o: /var/local/root/include/TCollection.h
EdbPattern.o: /var/local/root/include/TIterator.h
EdbPattern.o: /var/local/root/include/Property.h
EdbPattern.o: /var/local/root/include/TAxis.h
EdbPattern.o: /var/local/root/include/TAttAxis.h
EdbPattern.o: /var/local/root/include/Gtypes.h
EdbPattern.o: /var/local/root/include/Htypes.h
EdbPattern.o: /var/local/root/include/TArrayD.h
EdbPattern.o: /var/local/root/include/TArray.h
EdbPattern.o: /var/local/root/include/THashList.h
EdbPattern.o: /var/local/root/include/TAttMarker.h
EdbPattern.o: /var/local/root/include/TArrayC.h
EdbPattern.o: /var/local/root/include/TArrayS.h
EdbPattern.o: /var/local/root/include/TArrayI.h
EdbPattern.o: /var/local/root/include/TArrayF.h
EdbPattern.o: /var/local/root/include/Foption.h
EdbVertex.o: /var/local/root/include/TFile.h
EdbVertex.o: /var/local/root/include/TDirectory.h
EdbVertex.o: /var/local/root/include/TDatime.h
EdbVertex.o: /var/local/root/include/TUUID.h
EdbVertex.o: /var/local/root/include/TInetAddress.h
EdbVertex.o: /var/local/root/include/TCache.h
EdbVertex.o: /var/local/root/include/TRandom.h
EdbVertex.o: /var/local/root/include/TNamed.h
EdbVertex.o: /var/local/root/include/TObject.h
EdbVertex.o: /var/local/root/include/Rtypes.h
EdbVertex.o: /var/local/root/include/RConfig.h
EdbVertex.o: /var/local/root/include/RVersion.h
EdbVertex.o: /var/local/root/include/DllImport.h
EdbVertex.o: /var/local/root/include/Rtypeinfo.h
EdbVertex.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
EdbVertex.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
EdbVertex.o: /usr/include/stdio.h /usr/include/features.h
EdbVertex.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
EdbVertex.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
EdbVertex.o: /usr/include/bits/types.h /usr/include/libio.h
EdbVertex.o: /usr/include/_G_config.h /usr/include/wchar.h
EdbVertex.o: /usr/include/bits/wchar.h /usr/include/gconv.h
EdbVertex.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
EdbVertex.o: /usr/include/bits/stdio_lim.h
EdbVertex.o: /var/local/root/include/TGenericClassInfo.h
EdbVertex.o: /var/local/root/include/Varargs.h
EdbVertex.o: /var/local/root/include/TStorage.h
EdbVertex.o: /var/local/root/include/Riosfwd.h
EdbVertex.o: /var/local/root/include/TBuffer.h
EdbVertex.o: /var/local/root/include/Bytes.h /usr/include/string.h
EdbVertex.o: /var/local/root/include/TList.h
EdbVertex.o: /var/local/root/include/TSeqCollection.h
EdbVertex.o: /var/local/root/include/TCollection.h
EdbVertex.o: /var/local/root/include/TIterator.h
EdbVertex.o: /var/local/root/include/TString.h
EdbVertex.o: /var/local/root/include/TMath.h
EdbVertex.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
EdbVertex.o: /var/local/root/include/TRefCnt.h EdbVertex.h
EdbVertex.o: /var/local/root/include/TTree.h
EdbVertex.o: /var/local/root/include/TObjArray.h
EdbVertex.o: /var/local/root/include/TClonesArray.h
EdbVertex.o: /var/local/root/include/TBranch.h
EdbVertex.o: /var/local/root/include/TStringLong.h
EdbVertex.o: /var/local/root/include/TVirtualTreePlayer.h
EdbVertex.o: /var/local/root/include/TVector3.h
EdbVertex.o: /var/local/root/include/TError.h
EdbVertex.o: /var/local/root/include/TVector2.h
EdbVertex.o: /var/local/root/include/TMatrix.h
EdbVertex.o: /var/local/root/include/TVector.h
EdbVertex.o: /var/local/root/include/TMatrixUtils.h
EdbVertex.o: /var/local/root/include/TGraphErrors.h
EdbVertex.o: /var/local/root/include/TGraph.h /var/local/root/include/TH1.h
EdbVertex.o: /var/local/root/include/TVirtualHistPainter.h
EdbVertex.o: /var/local/root/include/TClass.h
EdbVertex.o: /var/local/root/include/TDictionary.h
EdbVertex.o: /var/local/root/include/Property.h
EdbVertex.o: /var/local/root/include/TAxis.h
EdbVertex.o: /var/local/root/include/TAttAxis.h
EdbVertex.o: /var/local/root/include/Gtypes.h
EdbVertex.o: /var/local/root/include/Htypes.h
EdbVertex.o: /var/local/root/include/TArrayD.h
EdbVertex.o: /var/local/root/include/TArray.h
EdbVertex.o: /var/local/root/include/THashList.h
EdbVertex.o: /var/local/root/include/TAttLine.h
EdbVertex.o: /var/local/root/include/TAttFill.h
EdbVertex.o: /var/local/root/include/TAttMarker.h
EdbVertex.o: /var/local/root/include/TArrayC.h
EdbVertex.o: /var/local/root/include/TArrayS.h
EdbVertex.o: /var/local/root/include/TArrayI.h
EdbVertex.o: /var/local/root/include/TArrayF.h
EdbVertex.o: /var/local/root/include/Foption.h EdbPattern.h
EdbVertex.o: /var/local/root/include/TSortedList.h
EdbVertex.o: /var/local/root/include/TArrayL.h
EdbVertex.o: /var/local/root/include/TMatrixD.h
EdbVertex.o: /var/local/root/include/TVectorD.h
EdbVertex.o: /var/local/root/include/TMatrixDUtils.h
