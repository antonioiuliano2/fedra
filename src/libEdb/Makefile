#------------------------------------------------------------------------------
include ../config/ProjectDef.mk
include ../config/RootDef.mk
#------------------------------------------------------------------------------

LIB_TGT       = $(LIB_DIR)/libEdb.so

headers       = EdbCluster.h EdbView.h EdbRunHeader.h EdbRun.h \
		EdbImage.h EdbFrame.h EdbSegment.h EdbStage.h \
		EdbAffine.h EdbFiducial.h EdbVirtual.h EdbPrediction.h \
		EdbOnline.h

classes	      = $(headers:.h=.$(SrcSuf))
EdbCint	      = $(headers) EdbLinkDef.h
SRC           = $(classes) EdbCint.$(SrcSuf)
OBJ           = $(SRC:.$(SrcSuf)=.$(ObjSuf))

CXXFLAGS += -I$(INC_DIR)
#------------------------------------------------------------------------------

$(LIB_TGT): $(OBJ)
	$(CXX) -O -shared $(LIBS) -o $(LIB_TGT) $(OBJ)

EdbCint.cxx: $(EdbCint)
	rootcint -f EdbCint.cxx -c $(INCLUDE) $(EdbCint)

clean:
	@rm -fr core *.bak *.o $(LIB_TGT) *.log *.out *Cint.cxx *Cint.h *~

depend:
	@makedepend $(ROOTCFLAGS) *.cxx >& /dev/null

.SUFFIXES: .$(SrcSuf)

.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $<
# DO NOT DELETE

EdbAffine.o: /usr/include/math.h /usr/include/features.h
EdbAffine.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
EdbAffine.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
EdbAffine.o: /usr/include/bits/mathcalls.h /var/local/root/include/TMath.h
EdbAffine.o: /var/local/root/include/Rtypes.h
EdbAffine.o: /var/local/root/include/RConfig.h
EdbAffine.o: /var/local/root/include/RVersion.h
EdbAffine.o: /var/local/root/include/DllImport.h
EdbAffine.o: /var/local/root/include/Rtypeinfo.h
EdbAffine.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
EdbAffine.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
EdbAffine.o: /usr/include/stdio.h
EdbAffine.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
EdbAffine.o: /usr/include/bits/types.h /usr/include/libio.h
EdbAffine.o: /usr/include/_G_config.h /usr/include/wchar.h
EdbAffine.o: /usr/include/bits/wchar.h /usr/include/gconv.h
EdbAffine.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
EdbAffine.o: /usr/include/bits/stdio_lim.h
EdbAffine.o: /var/local/root/include/TGenericClassInfo.h
EdbAffine.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
EdbAffine.o: EdbAffine.h /var/local/root/include/TObject.h
EdbAffine.o: /var/local/root/include/Varargs.h
EdbAffine.o: /var/local/root/include/TStorage.h
EdbAffine.o: /var/local/root/include/Riosfwd.h
EdbAffine.o: /var/local/root/include/TBuffer.h
EdbAffine.o: /var/local/root/include/Bytes.h /usr/include/string.h
EdbAffine.o: EdbVirtual.h
EdbBasic.o: /usr/include/stdio.h /usr/include/features.h
EdbBasic.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
EdbBasic.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
EdbBasic.o: /usr/include/bits/types.h /usr/include/libio.h
EdbBasic.o: /usr/include/_G_config.h /usr/include/wchar.h
EdbBasic.o: /usr/include/bits/wchar.h /usr/include/gconv.h
EdbBasic.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
EdbBasic.o: /usr/include/bits/stdio_lim.h
EdbCluster.o: /var/local/root/include/TPolyMarker3D.h
EdbCluster.o: /var/local/root/include/TObject.h
EdbCluster.o: /var/local/root/include/Varargs.h
EdbCluster.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
EdbCluster.o: /var/local/root/include/TStorage.h
EdbCluster.o: /var/local/root/include/Riosfwd.h
EdbCluster.o: /var/local/root/include/TBuffer.h
EdbCluster.o: /var/local/root/include/Bytes.h /usr/include/string.h
EdbCluster.o: /usr/include/features.h /usr/include/sys/cdefs.h
EdbCluster.o: /usr/include/gnu/stubs.h
EdbCluster.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
EdbCluster.o: /var/local/root/include/TAttMarker.h
EdbCluster.o: /var/local/root/include/Gtypes.h
EdbCluster.o: /var/local/root/include/Htypes.h
EdbCluster.o: /var/local/root/include/Rtypes.h
EdbCluster.o: /var/local/root/include/RConfig.h
EdbCluster.o: /var/local/root/include/RVersion.h
EdbCluster.o: /var/local/root/include/DllImport.h
EdbCluster.o: /var/local/root/include/Rtypeinfo.h
EdbCluster.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
EdbCluster.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
EdbCluster.o: /usr/include/stdio.h /usr/include/bits/types.h
EdbCluster.o: /usr/include/libio.h /usr/include/_G_config.h
EdbCluster.o: /usr/include/wchar.h /usr/include/bits/wchar.h
EdbCluster.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
EdbCluster.o: /var/local/root/include/TGenericClassInfo.h
EdbCluster.o: /var/local/root/include/TString.h
EdbCluster.o: /var/local/root/include/TMath.h
EdbCluster.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
EdbCluster.o: /var/local/root/include/TRefCnt.h
EdbCluster.o: /var/local/root/include/TAtt3D.h
EdbCluster.o: /var/local/root/include/X3DBuffer.h EdbCluster.h
EdbCluster.o: /var/local/root/include/TClonesArray.h
EdbCluster.o: /var/local/root/include/TObjArray.h
EdbCluster.o: /var/local/root/include/TSeqCollection.h
EdbCluster.o: /var/local/root/include/TCollection.h
EdbCluster.o: /var/local/root/include/TIterator.h EdbVirtual.h
EdbFiducial.o: /usr/include/stdio.h /usr/include/features.h
EdbFiducial.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
EdbFiducial.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
EdbFiducial.o: /usr/include/bits/types.h /usr/include/libio.h
EdbFiducial.o: /usr/include/_G_config.h /usr/include/wchar.h
EdbFiducial.o: /usr/include/bits/wchar.h /usr/include/gconv.h
EdbFiducial.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
EdbFiducial.o: /usr/include/bits/stdio_lim.h /var/local/root/include/TMath.h
EdbFiducial.o: /var/local/root/include/Rtypes.h
EdbFiducial.o: /var/local/root/include/RConfig.h
EdbFiducial.o: /var/local/root/include/RVersion.h
EdbFiducial.o: /var/local/root/include/DllImport.h
EdbFiducial.o: /var/local/root/include/Rtypeinfo.h
EdbFiducial.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
EdbFiducial.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
EdbFiducial.o: /var/local/root/include/TGenericClassInfo.h
EdbFiducial.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
EdbFiducial.o: /var/local/root/include/TText.h
EdbFiducial.o: /var/local/root/include/TAttText.h
EdbFiducial.o: /var/local/root/include/Gtypes.h
EdbFiducial.o: /var/local/root/include/Htypes.h
EdbFiducial.o: /var/local/root/include/TString.h /usr/include/string.h
EdbFiducial.o: /var/local/root/include/TRefCnt.h
EdbFiducial.o: /var/local/root/include/TNamed.h
EdbFiducial.o: /var/local/root/include/TObject.h
EdbFiducial.o: /var/local/root/include/Varargs.h
EdbFiducial.o: /var/local/root/include/TStorage.h
EdbFiducial.o: /var/local/root/include/Riosfwd.h
EdbFiducial.o: /var/local/root/include/TBuffer.h
EdbFiducial.o: /var/local/root/include/Bytes.h
EdbFiducial.o: /var/local/root/include/TList.h
EdbFiducial.o: /var/local/root/include/TSeqCollection.h
EdbFiducial.o: /var/local/root/include/TCollection.h
EdbFiducial.o: /var/local/root/include/TIterator.h EdbFiducial.h
EdbFiducial.o: /var/local/root/include/TClonesArray.h
EdbFiducial.o: /var/local/root/include/TObjArray.h EdbVirtual.h EdbAffine.h
EdbFrame.o: EdbFrame.h EdbImage.h /var/local/root/include/TObject.h
EdbFrame.o: /var/local/root/include/Varargs.h
EdbFrame.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
EdbFrame.o: /var/local/root/include/TStorage.h
EdbFrame.o: /var/local/root/include/Riosfwd.h
EdbFrame.o: /var/local/root/include/TBuffer.h /var/local/root/include/Bytes.h
EdbFrame.o: /usr/include/string.h /usr/include/features.h
EdbFrame.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
EdbFrame.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
EdbFrame.o: /var/local/root/include/TNamed.h /var/local/root/include/TList.h
EdbFrame.o: /var/local/root/include/TSeqCollection.h
EdbFrame.o: /var/local/root/include/TCollection.h
EdbFrame.o: /var/local/root/include/TIterator.h
EdbFrame.o: /var/local/root/include/TArrayF.h
EdbFrame.o: /var/local/root/include/TArray.h /var/local/root/include/Htypes.h
EdbFrame.o: /var/local/root/include/Rtypes.h
EdbFrame.o: /var/local/root/include/RConfig.h
EdbFrame.o: /var/local/root/include/RVersion.h
EdbFrame.o: /var/local/root/include/DllImport.h
EdbFrame.o: /var/local/root/include/Rtypeinfo.h
EdbFrame.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
EdbFrame.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
EdbFrame.o: /usr/include/stdio.h /usr/include/bits/types.h
EdbFrame.o: /usr/include/libio.h /usr/include/_G_config.h
EdbFrame.o: /usr/include/wchar.h /usr/include/bits/wchar.h
EdbFrame.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
EdbFrame.o: /var/local/root/include/TGenericClassInfo.h
EdbFrame.o: /var/local/root/include/TArrayC.h /var/local/root/include/TH2.h
EdbFrame.o: /var/local/root/include/TH1.h
EdbFrame.o: /var/local/root/include/TVirtualHistPainter.h
EdbFrame.o: /var/local/root/include/TClass.h
EdbFrame.o: /var/local/root/include/TDictionary.h
EdbFrame.o: /var/local/root/include/Property.h
EdbFrame.o: /var/local/root/include/TString.h /var/local/root/include/TMath.h
EdbFrame.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
EdbFrame.o: /var/local/root/include/TRefCnt.h /var/local/root/include/TAxis.h
EdbFrame.o: /var/local/root/include/TAttAxis.h
EdbFrame.o: /var/local/root/include/Gtypes.h
EdbFrame.o: /var/local/root/include/TArrayD.h
EdbFrame.o: /var/local/root/include/THashList.h
EdbFrame.o: /var/local/root/include/TAttLine.h
EdbFrame.o: /var/local/root/include/TAttFill.h
EdbFrame.o: /var/local/root/include/TAttMarker.h
EdbFrame.o: /var/local/root/include/TArrayS.h
EdbImage.o: /var/local/root/include/TH1.h
EdbImage.o: /var/local/root/include/TVirtualHistPainter.h
EdbImage.o: /var/local/root/include/TClass.h
EdbImage.o: /var/local/root/include/TDictionary.h
EdbImage.o: /var/local/root/include/Property.h
EdbImage.o: /var/local/root/include/TString.h /usr/include/string.h
EdbImage.o: /usr/include/features.h /usr/include/sys/cdefs.h
EdbImage.o: /usr/include/gnu/stubs.h
EdbImage.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
EdbImage.o: /var/local/root/include/TMath.h /var/local/root/include/Rtypes.h
EdbImage.o: /var/local/root/include/RConfig.h
EdbImage.o: /var/local/root/include/RVersion.h
EdbImage.o: /var/local/root/include/DllImport.h
EdbImage.o: /var/local/root/include/Rtypeinfo.h
EdbImage.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
EdbImage.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
EdbImage.o: /usr/include/stdio.h /usr/include/bits/types.h
EdbImage.o: /usr/include/libio.h /usr/include/_G_config.h
EdbImage.o: /usr/include/wchar.h /usr/include/bits/wchar.h
EdbImage.o: /usr/include/gconv.h
EdbImage.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
EdbImage.o: /usr/include/bits/stdio_lim.h
EdbImage.o: /var/local/root/include/TGenericClassInfo.h
EdbImage.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
EdbImage.o: /var/local/root/include/TRefCnt.h /var/local/root/include/TAxis.h
EdbImage.o: /var/local/root/include/TAttAxis.h
EdbImage.o: /var/local/root/include/Gtypes.h /var/local/root/include/Htypes.h
EdbImage.o: /var/local/root/include/TArrayD.h
EdbImage.o: /var/local/root/include/THashList.h
EdbImage.o: /var/local/root/include/TAttLine.h
EdbImage.o: /var/local/root/include/TAttFill.h
EdbImage.o: /var/local/root/include/TAttMarker.h
EdbImage.o: /var/local/root/include/TArrayS.h /var/local/root/include/TH2.h
EdbImage.o: EdbImage.h /var/local/root/include/TObject.h
EdbImage.o: /var/local/root/include/Varargs.h
EdbImage.o: /var/local/root/include/TStorage.h
EdbImage.o: /var/local/root/include/Riosfwd.h
EdbImage.o: /var/local/root/include/TBuffer.h /var/local/root/include/Bytes.h
EdbImage.o: /var/local/root/include/TNamed.h /var/local/root/include/TList.h
EdbImage.o: /var/local/root/include/TSeqCollection.h
EdbImage.o: /var/local/root/include/TCollection.h
EdbImage.o: /var/local/root/include/TIterator.h
EdbImage.o: /var/local/root/include/TArrayF.h
EdbImage.o: /var/local/root/include/TArray.h
EdbImage.o: /var/local/root/include/TArrayC.h
EdbImage.o: /var/local/root/include/Riostream.h
EdbOnline.o: /usr/include/string.h /usr/include/features.h
EdbOnline.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
EdbOnline.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
EdbOnline.o: /var/local/root/include/TSystem.h /usr/include/stdio.h
EdbOnline.o: /usr/include/bits/types.h /usr/include/libio.h
EdbOnline.o: /usr/include/_G_config.h /usr/include/wchar.h
EdbOnline.o: /usr/include/bits/wchar.h /usr/include/gconv.h
EdbOnline.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
EdbOnline.o: /usr/include/bits/stdio_lim.h /var/local/root/include/G__ci.h
EdbOnline.o: /usr/include/math.h /usr/include/bits/huge_val.h
EdbOnline.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
EdbOnline.o: /usr/include/stdlib.h /usr/include/signal.h
EdbOnline.o: /usr/include/bits/sigset.h /usr/include/bits/signum.h
EdbOnline.o: /usr/include/assert.h /usr/include/limits.h
EdbOnline.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
EdbOnline.o: /usr/include/setjmp.h /usr/include/bits/setjmp.h
EdbOnline.o: /usr/include/ctype.h /usr/include/endian.h
EdbOnline.o: /usr/include/bits/endian.h /usr/include/fcntl.h
EdbOnline.o: /usr/include/bits/fcntl.h /usr/include/sys/types.h
EdbOnline.o: /usr/include/time.h /usr/include/unistd.h
EdbOnline.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
EdbOnline.o: /var/local/root/include/TNamed.h
EdbOnline.o: /var/local/root/include/TObject.h
EdbOnline.o: /var/local/root/include/Varargs.h
EdbOnline.o: /var/local/root/include/TStorage.h
EdbOnline.o: /var/local/root/include/Riosfwd.h
EdbOnline.o: /var/local/root/include/TBuffer.h
EdbOnline.o: /var/local/root/include/Bytes.h /var/local/root/include/TList.h
EdbOnline.o: /var/local/root/include/TSeqCollection.h
EdbOnline.o: /var/local/root/include/TCollection.h
EdbOnline.o: /var/local/root/include/TIterator.h
EdbOnline.o: /var/local/root/include/TString.h
EdbOnline.o: /var/local/root/include/TMath.h /var/local/root/include/Rtypes.h
EdbOnline.o: /var/local/root/include/RConfig.h
EdbOnline.o: /var/local/root/include/RVersion.h
EdbOnline.o: /var/local/root/include/DllImport.h
EdbOnline.o: /var/local/root/include/Rtypeinfo.h
EdbOnline.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
EdbOnline.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
EdbOnline.o: /var/local/root/include/TGenericClassInfo.h
EdbOnline.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
EdbOnline.o: /var/local/root/include/TRefCnt.h
EdbOnline.o: /var/local/root/include/TInetAddress.h
EdbOnline.o: /var/local/root/include/TTimer.h
EdbOnline.o: /var/local/root/include/TSysEvtHandler.h
EdbOnline.o: /var/local/root/include/TQObject.h
EdbOnline.o: /var/local/root/include/TClass.h
EdbOnline.o: /var/local/root/include/TDictionary.h
EdbOnline.o: /var/local/root/include/Property.h
EdbOnline.o: /var/local/root/include/TTime.h EdbOnline.h
EdbOnline.o: /var/local/root/include/TROOT.h
EdbOnline.o: /var/local/root/include/TDirectory.h
EdbOnline.o: /var/local/root/include/TDatime.h
EdbOnline.o: /var/local/root/include/Htypes.h /var/local/root/include/TUUID.h
EdbOnline.o: EdbRun.h /var/local/root/include/TFile.h
EdbOnline.o: /var/local/root/include/TCache.h
EdbOnline.o: /var/local/root/include/THashList.h
EdbOnline.o: /var/local/root/include/TTree.h
EdbOnline.o: /var/local/root/include/TObjArray.h
EdbOnline.o: /var/local/root/include/TClonesArray.h
EdbOnline.o: /var/local/root/include/TAttLine.h
EdbOnline.o: /var/local/root/include/TAttFill.h
EdbOnline.o: /var/local/root/include/TAttMarker.h
EdbOnline.o: /var/local/root/include/Gtypes.h
EdbOnline.o: /var/local/root/include/TBranch.h
EdbOnline.o: /var/local/root/include/TStringLong.h
EdbOnline.o: /var/local/root/include/TCut.h /var/local/root/include/TArrayD.h
EdbOnline.o: /var/local/root/include/TArrayI.h
EdbOnline.o: /var/local/root/include/TArray.h
EdbOnline.o: /var/local/root/include/TVirtualTreePlayer.h
EdbOnline.o: /var/local/root/include/TObjString.h EdbView.h
EdbOnline.o: /var/local/root/include/TPolyMarker3D.h
EdbOnline.o: /var/local/root/include/TAtt3D.h
EdbOnline.o: /var/local/root/include/X3DBuffer.h
EdbOnline.o: /var/local/root/include/TArrayF.h EdbAffine.h EdbRunHeader.h
EdbOnline.o: /var/local/root/include/TStopwatch.h EdbStage.h EdbFiducial.h
EdbOnline.o: EdbVirtual.h EdbPrediction.h /var/local/root/include/TNtuple.h
EdbPrediction.o: EdbFiducial.h /var/local/root/include/TClonesArray.h
EdbPrediction.o: /var/local/root/include/TObjArray.h
EdbPrediction.o: /var/local/root/include/TSeqCollection.h
EdbPrediction.o: /var/local/root/include/TCollection.h
EdbPrediction.o: /var/local/root/include/TIterator.h EdbVirtual.h EdbAffine.h
EdbPrediction.o: /var/local/root/include/TObject.h
EdbPrediction.o: /var/local/root/include/Varargs.h
EdbPrediction.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
EdbPrediction.o: /var/local/root/include/TStorage.h
EdbPrediction.o: /var/local/root/include/Riosfwd.h
EdbPrediction.o: /var/local/root/include/TBuffer.h
EdbPrediction.o: /var/local/root/include/Bytes.h /usr/include/string.h
EdbPrediction.o: /usr/include/features.h /usr/include/sys/cdefs.h
EdbPrediction.o: /usr/include/gnu/stubs.h
EdbPrediction.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
EdbPrediction.o: EdbPrediction.h /var/local/root/include/TNtuple.h
EdbRun.o: /var/local/root/include/TBranchClones.h
EdbRun.o: /var/local/root/include/TBranch.h
EdbRun.o: /var/local/root/include/TStringLong.h
EdbRun.o: /var/local/root/include/TClonesArray.h
EdbRun.o: /var/local/root/include/TObjArray.h
EdbRun.o: /var/local/root/include/TSeqCollection.h
EdbRun.o: /var/local/root/include/TCollection.h
EdbRun.o: /var/local/root/include/TIterator.h
EdbRun.o: /var/local/root/include/TSystem.h /usr/include/stdio.h
EdbRun.o: /usr/include/features.h /usr/include/sys/cdefs.h
EdbRun.o: /usr/include/gnu/stubs.h
EdbRun.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
EdbRun.o: /usr/include/bits/types.h /usr/include/libio.h
EdbRun.o: /usr/include/_G_config.h /usr/include/wchar.h
EdbRun.o: /usr/include/bits/wchar.h /usr/include/gconv.h
EdbRun.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
EdbRun.o: /usr/include/bits/stdio_lim.h /var/local/root/include/G__ci.h
EdbRun.o: /usr/include/math.h /usr/include/bits/huge_val.h
EdbRun.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
EdbRun.o: /usr/include/string.h /usr/include/stdlib.h /usr/include/signal.h
EdbRun.o: /usr/include/bits/sigset.h /usr/include/bits/signum.h
EdbRun.o: /usr/include/assert.h /usr/include/limits.h
EdbRun.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
EdbRun.o: /usr/include/setjmp.h /usr/include/bits/setjmp.h
EdbRun.o: /usr/include/ctype.h /usr/include/endian.h
EdbRun.o: /usr/include/bits/endian.h /usr/include/fcntl.h
EdbRun.o: /usr/include/bits/fcntl.h /usr/include/sys/types.h
EdbRun.o: /usr/include/time.h /usr/include/unistd.h
EdbRun.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
EdbRun.o: /var/local/root/include/TNamed.h /var/local/root/include/TObject.h
EdbRun.o: /var/local/root/include/Varargs.h
EdbRun.o: /var/local/root/include/TStorage.h
EdbRun.o: /var/local/root/include/Riosfwd.h /var/local/root/include/TBuffer.h
EdbRun.o: /var/local/root/include/Bytes.h /var/local/root/include/TList.h
EdbRun.o: /var/local/root/include/TString.h /var/local/root/include/TMath.h
EdbRun.o: /var/local/root/include/Rtypes.h /var/local/root/include/RConfig.h
EdbRun.o: /var/local/root/include/RVersion.h
EdbRun.o: /var/local/root/include/DllImport.h
EdbRun.o: /var/local/root/include/Rtypeinfo.h
EdbRun.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
EdbRun.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
EdbRun.o: /var/local/root/include/TGenericClassInfo.h
EdbRun.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
EdbRun.o: /var/local/root/include/TRefCnt.h
EdbRun.o: /var/local/root/include/TInetAddress.h
EdbRun.o: /var/local/root/include/TTimer.h
EdbRun.o: /var/local/root/include/TSysEvtHandler.h
EdbRun.o: /var/local/root/include/TQObject.h /var/local/root/include/TClass.h
EdbRun.o: /var/local/root/include/TDictionary.h
EdbRun.o: /var/local/root/include/Property.h /var/local/root/include/TTime.h
EdbRun.o: EdbRun.h /var/local/root/include/TFile.h
EdbRun.o: /var/local/root/include/TCache.h
EdbRun.o: /var/local/root/include/THashList.h /var/local/root/include/TTree.h
EdbRun.o: /var/local/root/include/TAttLine.h
EdbRun.o: /var/local/root/include/TAttFill.h
EdbRun.o: /var/local/root/include/TAttMarker.h
EdbRun.o: /var/local/root/include/Gtypes.h /var/local/root/include/Htypes.h
EdbRun.o: /var/local/root/include/TCut.h /var/local/root/include/TArrayD.h
EdbRun.o: /var/local/root/include/TArrayI.h /var/local/root/include/TArray.h
EdbRun.o: /var/local/root/include/TVirtualTreePlayer.h
EdbRun.o: /var/local/root/include/TObjString.h EdbView.h
EdbRun.o: /var/local/root/include/TPolyMarker3D.h
EdbRun.o: /var/local/root/include/TAtt3D.h
EdbRun.o: /var/local/root/include/X3DBuffer.h
EdbRun.o: /var/local/root/include/TArrayF.h EdbAffine.h EdbRunHeader.h
EdbRun.o: /var/local/root/include/TDatime.h
EdbRun.o: /var/local/root/include/TStopwatch.h EdbStage.h EdbFiducial.h
EdbRun.o: EdbVirtual.h EdbPrediction.h /var/local/root/include/TNtuple.h
EdbRunHeader.o: /var/local/root/include/TSystem.h /usr/include/stdio.h
EdbRunHeader.o: /usr/include/features.h /usr/include/sys/cdefs.h
EdbRunHeader.o: /usr/include/gnu/stubs.h
EdbRunHeader.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
EdbRunHeader.o: /usr/include/bits/types.h /usr/include/libio.h
EdbRunHeader.o: /usr/include/_G_config.h /usr/include/wchar.h
EdbRunHeader.o: /usr/include/bits/wchar.h /usr/include/gconv.h
EdbRunHeader.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
EdbRunHeader.o: /usr/include/bits/stdio_lim.h /var/local/root/include/G__ci.h
EdbRunHeader.o: /usr/include/math.h /usr/include/bits/huge_val.h
EdbRunHeader.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
EdbRunHeader.o: /usr/include/string.h /usr/include/stdlib.h
EdbRunHeader.o: /usr/include/signal.h /usr/include/bits/sigset.h
EdbRunHeader.o: /usr/include/bits/signum.h /usr/include/assert.h
EdbRunHeader.o: /usr/include/limits.h
EdbRunHeader.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
EdbRunHeader.o: /usr/include/setjmp.h /usr/include/bits/setjmp.h
EdbRunHeader.o: /usr/include/ctype.h /usr/include/endian.h
EdbRunHeader.o: /usr/include/bits/endian.h /usr/include/fcntl.h
EdbRunHeader.o: /usr/include/bits/fcntl.h /usr/include/sys/types.h
EdbRunHeader.o: /usr/include/time.h /usr/include/unistd.h
EdbRunHeader.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
EdbRunHeader.o: /var/local/root/include/TNamed.h
EdbRunHeader.o: /var/local/root/include/TObject.h
EdbRunHeader.o: /var/local/root/include/Varargs.h
EdbRunHeader.o: /var/local/root/include/TStorage.h
EdbRunHeader.o: /var/local/root/include/Riosfwd.h
EdbRunHeader.o: /var/local/root/include/TBuffer.h
EdbRunHeader.o: /var/local/root/include/Bytes.h
EdbRunHeader.o: /var/local/root/include/TList.h
EdbRunHeader.o: /var/local/root/include/TSeqCollection.h
EdbRunHeader.o: /var/local/root/include/TCollection.h
EdbRunHeader.o: /var/local/root/include/TIterator.h
EdbRunHeader.o: /var/local/root/include/TString.h
EdbRunHeader.o: /var/local/root/include/TMath.h
EdbRunHeader.o: /var/local/root/include/Rtypes.h
EdbRunHeader.o: /var/local/root/include/RConfig.h
EdbRunHeader.o: /var/local/root/include/RVersion.h
EdbRunHeader.o: /var/local/root/include/DllImport.h
EdbRunHeader.o: /var/local/root/include/Rtypeinfo.h
EdbRunHeader.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
EdbRunHeader.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
EdbRunHeader.o: /var/local/root/include/TGenericClassInfo.h
EdbRunHeader.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
EdbRunHeader.o: /var/local/root/include/TRefCnt.h
EdbRunHeader.o: /var/local/root/include/TInetAddress.h
EdbRunHeader.o: /var/local/root/include/TTimer.h
EdbRunHeader.o: /var/local/root/include/TSysEvtHandler.h
EdbRunHeader.o: /var/local/root/include/TQObject.h
EdbRunHeader.o: /var/local/root/include/TClass.h
EdbRunHeader.o: /var/local/root/include/TDictionary.h
EdbRunHeader.o: /var/local/root/include/Property.h
EdbRunHeader.o: /var/local/root/include/TTime.h EdbRunHeader.h
EdbRunHeader.o: /var/local/root/include/TObjString.h
EdbRunHeader.o: /var/local/root/include/TDatime.h
EdbRunHeader.o: /var/local/root/include/Htypes.h
EdbRunHeader.o: /var/local/root/include/TStopwatch.h EdbStage.h EdbFiducial.h
EdbRunHeader.o: /var/local/root/include/TClonesArray.h
EdbRunHeader.o: /var/local/root/include/TObjArray.h EdbVirtual.h
EdbSegment.o: EdbSegment.h /var/local/root/include/TArrayI.h
EdbSegment.o: /var/local/root/include/TArray.h
EdbSegment.o: /var/local/root/include/Htypes.h
EdbSegment.o: /var/local/root/include/Rtypes.h
EdbSegment.o: /var/local/root/include/RConfig.h
EdbSegment.o: /var/local/root/include/RVersion.h
EdbSegment.o: /var/local/root/include/DllImport.h
EdbSegment.o: /var/local/root/include/Rtypeinfo.h
EdbSegment.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
EdbSegment.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
EdbSegment.o: /usr/include/stdio.h /usr/include/features.h
EdbSegment.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
EdbSegment.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
EdbSegment.o: /usr/include/bits/types.h /usr/include/libio.h
EdbSegment.o: /usr/include/_G_config.h /usr/include/wchar.h
EdbSegment.o: /usr/include/bits/wchar.h /usr/include/gconv.h
EdbSegment.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
EdbSegment.o: /usr/include/bits/stdio_lim.h
EdbSegment.o: /var/local/root/include/TGenericClassInfo.h
EdbSegment.o: /usr/include/string.h /var/local/root/include/TObjArray.h
EdbSegment.o: /var/local/root/include/TSeqCollection.h
EdbSegment.o: /var/local/root/include/TCollection.h
EdbSegment.o: /var/local/root/include/TIterator.h EdbAffine.h
EdbSegment.o: /var/local/root/include/TObject.h
EdbSegment.o: /var/local/root/include/Varargs.h
EdbSegment.o: /var/local/root/include/TStorage.h
EdbSegment.o: /var/local/root/include/Riosfwd.h
EdbSegment.o: /var/local/root/include/TBuffer.h
EdbSegment.o: /var/local/root/include/Bytes.h
EdbStage.o: EdbStage.h
EdbTest.o: /usr/include/string.h /usr/include/features.h
EdbTest.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
EdbTest.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
EdbTest.o: /var/local/root/include/TSystem.h /usr/include/stdio.h
EdbTest.o: /usr/include/bits/types.h /usr/include/libio.h
EdbTest.o: /usr/include/_G_config.h /usr/include/wchar.h
EdbTest.o: /usr/include/bits/wchar.h /usr/include/gconv.h
EdbTest.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
EdbTest.o: /usr/include/bits/stdio_lim.h /var/local/root/include/G__ci.h
EdbTest.o: /usr/include/math.h /usr/include/bits/huge_val.h
EdbTest.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
EdbTest.o: /usr/include/stdlib.h /usr/include/signal.h
EdbTest.o: /usr/include/bits/sigset.h /usr/include/bits/signum.h
EdbTest.o: /usr/include/assert.h /usr/include/limits.h
EdbTest.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
EdbTest.o: /usr/include/setjmp.h /usr/include/bits/setjmp.h
EdbTest.o: /usr/include/ctype.h /usr/include/endian.h
EdbTest.o: /usr/include/bits/endian.h /usr/include/fcntl.h
EdbTest.o: /usr/include/bits/fcntl.h /usr/include/sys/types.h
EdbTest.o: /usr/include/time.h /usr/include/unistd.h
EdbTest.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
EdbTest.o: /var/local/root/include/TNamed.h /var/local/root/include/TObject.h
EdbTest.o: /var/local/root/include/Varargs.h
EdbTest.o: /var/local/root/include/TStorage.h
EdbTest.o: /var/local/root/include/Riosfwd.h
EdbTest.o: /var/local/root/include/TBuffer.h /var/local/root/include/Bytes.h
EdbTest.o: /var/local/root/include/TList.h
EdbTest.o: /var/local/root/include/TSeqCollection.h
EdbTest.o: /var/local/root/include/TCollection.h
EdbTest.o: /var/local/root/include/TIterator.h
EdbTest.o: /var/local/root/include/TString.h /var/local/root/include/TMath.h
EdbTest.o: /var/local/root/include/Rtypes.h /var/local/root/include/RConfig.h
EdbTest.o: /var/local/root/include/RVersion.h
EdbTest.o: /var/local/root/include/DllImport.h
EdbTest.o: /var/local/root/include/Rtypeinfo.h
EdbTest.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
EdbTest.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
EdbTest.o: /var/local/root/include/TGenericClassInfo.h
EdbTest.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
EdbTest.o: /var/local/root/include/TRefCnt.h
EdbTest.o: /var/local/root/include/TInetAddress.h
EdbTest.o: /var/local/root/include/TTimer.h
EdbTest.o: /var/local/root/include/TSysEvtHandler.h
EdbTest.o: /var/local/root/include/TQObject.h
EdbTest.o: /var/local/root/include/TClass.h
EdbTest.o: /var/local/root/include/TDictionary.h
EdbTest.o: /var/local/root/include/Property.h /var/local/root/include/TTime.h
EdbTest.o: EdbTest.h /var/local/root/include/TROOT.h
EdbTest.o: /var/local/root/include/TDirectory.h
EdbTest.o: /var/local/root/include/TDatime.h /var/local/root/include/Htypes.h
EdbTest.o: /var/local/root/include/TUUID.h EdbRun.h
EdbTest.o: /var/local/root/include/TFile.h /var/local/root/include/TCache.h
EdbTest.o: /var/local/root/include/THashList.h
EdbTest.o: /var/local/root/include/TTree.h
EdbTest.o: /var/local/root/include/TObjArray.h
EdbTest.o: /var/local/root/include/TClonesArray.h
EdbTest.o: /var/local/root/include/TAttLine.h
EdbTest.o: /var/local/root/include/TAttFill.h
EdbTest.o: /var/local/root/include/TAttMarker.h
EdbTest.o: /var/local/root/include/Gtypes.h /var/local/root/include/TBranch.h
EdbTest.o: /var/local/root/include/TStringLong.h
EdbTest.o: /var/local/root/include/TCut.h /var/local/root/include/TArrayD.h
EdbTest.o: /var/local/root/include/TArrayI.h /var/local/root/include/TArray.h
EdbTest.o: /var/local/root/include/TVirtualTreePlayer.h
EdbTest.o: /var/local/root/include/TObjString.h EdbView.h
EdbTest.o: /var/local/root/include/TPolyMarker3D.h
EdbTest.o: /var/local/root/include/TAtt3D.h
EdbTest.o: /var/local/root/include/X3DBuffer.h
EdbTest.o: /var/local/root/include/TArrayF.h EdbAffine.h EdbRunHeader.h
EdbTest.o: /var/local/root/include/TStopwatch.h EdbStage.h EdbFiducial.h
EdbTest.o: EdbVirtual.h EdbPrediction.h /var/local/root/include/TNtuple.h
EdbView.o: /var/local/root/include/TSystem.h /usr/include/stdio.h
EdbView.o: /usr/include/features.h /usr/include/sys/cdefs.h
EdbView.o: /usr/include/gnu/stubs.h
EdbView.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
EdbView.o: /usr/include/bits/types.h /usr/include/libio.h
EdbView.o: /usr/include/_G_config.h /usr/include/wchar.h
EdbView.o: /usr/include/bits/wchar.h /usr/include/gconv.h
EdbView.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
EdbView.o: /usr/include/bits/stdio_lim.h /var/local/root/include/G__ci.h
EdbView.o: /usr/include/math.h /usr/include/bits/huge_val.h
EdbView.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
EdbView.o: /usr/include/string.h /usr/include/stdlib.h /usr/include/signal.h
EdbView.o: /usr/include/bits/sigset.h /usr/include/bits/signum.h
EdbView.o: /usr/include/assert.h /usr/include/limits.h
EdbView.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
EdbView.o: /usr/include/setjmp.h /usr/include/bits/setjmp.h
EdbView.o: /usr/include/ctype.h /usr/include/endian.h
EdbView.o: /usr/include/bits/endian.h /usr/include/fcntl.h
EdbView.o: /usr/include/bits/fcntl.h /usr/include/sys/types.h
EdbView.o: /usr/include/time.h /usr/include/unistd.h
EdbView.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
EdbView.o: /var/local/root/include/TNamed.h /var/local/root/include/TObject.h
EdbView.o: /var/local/root/include/Varargs.h
EdbView.o: /var/local/root/include/TStorage.h
EdbView.o: /var/local/root/include/Riosfwd.h
EdbView.o: /var/local/root/include/TBuffer.h /var/local/root/include/Bytes.h
EdbView.o: /var/local/root/include/TList.h
EdbView.o: /var/local/root/include/TSeqCollection.h
EdbView.o: /var/local/root/include/TCollection.h
EdbView.o: /var/local/root/include/TIterator.h
EdbView.o: /var/local/root/include/TString.h /var/local/root/include/TMath.h
EdbView.o: /var/local/root/include/Rtypes.h /var/local/root/include/RConfig.h
EdbView.o: /var/local/root/include/RVersion.h
EdbView.o: /var/local/root/include/DllImport.h
EdbView.o: /var/local/root/include/Rtypeinfo.h
EdbView.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
EdbView.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
EdbView.o: /var/local/root/include/TGenericClassInfo.h
EdbView.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
EdbView.o: /var/local/root/include/TRefCnt.h
EdbView.o: /var/local/root/include/TInetAddress.h
EdbView.o: /var/local/root/include/TTimer.h
EdbView.o: /var/local/root/include/TSysEvtHandler.h
EdbView.o: /var/local/root/include/TQObject.h
EdbView.o: /var/local/root/include/TClass.h
EdbView.o: /var/local/root/include/TDictionary.h
EdbView.o: /var/local/root/include/Property.h /var/local/root/include/TTime.h
EdbView.o: /var/local/root/include/TRandom.h /var/local/root/include/TH3.h
EdbView.o: /var/local/root/include/TH1.h
EdbView.o: /var/local/root/include/TVirtualHistPainter.h
EdbView.o: /var/local/root/include/TAxis.h /var/local/root/include/TAttAxis.h
EdbView.o: /var/local/root/include/Gtypes.h /var/local/root/include/Htypes.h
EdbView.o: /var/local/root/include/TArrayD.h
EdbView.o: /var/local/root/include/THashList.h
EdbView.o: /var/local/root/include/TAttLine.h
EdbView.o: /var/local/root/include/TAttFill.h
EdbView.o: /var/local/root/include/TAttMarker.h
EdbView.o: /var/local/root/include/TArrayS.h /var/local/root/include/TAtt3D.h
EdbView.o: /var/local/root/include/TPolyLine3D.h
EdbView.o: /var/local/root/include/X3DBuffer.h EdbCluster.h
EdbView.o: /var/local/root/include/TClonesArray.h
EdbView.o: /var/local/root/include/TObjArray.h EdbVirtual.h EdbFrame.h
EdbView.o: EdbImage.h /var/local/root/include/TArrayF.h
EdbView.o: /var/local/root/include/TArray.h /var/local/root/include/TArrayC.h
EdbView.o: EdbView.h /var/local/root/include/TPolyMarker3D.h EdbAffine.h
EdbView.o: EdbSegment.h /var/local/root/include/TArrayI.h EdbRun.h
EdbView.o: /var/local/root/include/TFile.h /var/local/root/include/TCache.h
EdbView.o: /var/local/root/include/TTree.h /var/local/root/include/TBranch.h
EdbView.o: /var/local/root/include/TStringLong.h
EdbView.o: /var/local/root/include/TCut.h
EdbView.o: /var/local/root/include/TVirtualTreePlayer.h
EdbView.o: /var/local/root/include/TObjString.h EdbRunHeader.h
EdbView.o: /var/local/root/include/TDatime.h
EdbView.o: /var/local/root/include/TStopwatch.h EdbStage.h EdbFiducial.h
EdbView.o: EdbPrediction.h /var/local/root/include/TNtuple.h
EdbVirtual.o: /var/local/root/include/TH2.h /var/local/root/include/TH1.h
EdbVirtual.o: /var/local/root/include/TVirtualHistPainter.h
EdbVirtual.o: /var/local/root/include/TClass.h
EdbVirtual.o: /var/local/root/include/TDictionary.h
EdbVirtual.o: /var/local/root/include/Property.h
EdbVirtual.o: /var/local/root/include/TString.h /usr/include/string.h
EdbVirtual.o: /usr/include/features.h /usr/include/sys/cdefs.h
EdbVirtual.o: /usr/include/gnu/stubs.h
EdbVirtual.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
EdbVirtual.o: /var/local/root/include/TMath.h
EdbVirtual.o: /var/local/root/include/Rtypes.h
EdbVirtual.o: /var/local/root/include/RConfig.h
EdbVirtual.o: /var/local/root/include/RVersion.h
EdbVirtual.o: /var/local/root/include/DllImport.h
EdbVirtual.o: /var/local/root/include/Rtypeinfo.h
EdbVirtual.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
EdbVirtual.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
EdbVirtual.o: /usr/include/stdio.h /usr/include/bits/types.h
EdbVirtual.o: /usr/include/libio.h /usr/include/_G_config.h
EdbVirtual.o: /usr/include/wchar.h /usr/include/bits/wchar.h
EdbVirtual.o: /usr/include/gconv.h
EdbVirtual.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
EdbVirtual.o: /usr/include/bits/stdio_lim.h
EdbVirtual.o: /var/local/root/include/TGenericClassInfo.h
EdbVirtual.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
EdbVirtual.o: /var/local/root/include/TRefCnt.h
EdbVirtual.o: /var/local/root/include/TAxis.h
EdbVirtual.o: /var/local/root/include/TAttAxis.h
EdbVirtual.o: /var/local/root/include/Gtypes.h
EdbVirtual.o: /var/local/root/include/Htypes.h
EdbVirtual.o: /var/local/root/include/TArrayD.h
EdbVirtual.o: /var/local/root/include/THashList.h
EdbVirtual.o: /var/local/root/include/TAttLine.h
EdbVirtual.o: /var/local/root/include/TAttFill.h
EdbVirtual.o: /var/local/root/include/TAttMarker.h
EdbVirtual.o: /var/local/root/include/TArrayS.h
EdbVirtual.o: /var/local/root/include/TGraph.h
EdbVirtual.o: /var/local/root/include/TNamed.h
EdbVirtual.o: /var/local/root/include/TObject.h
EdbVirtual.o: /var/local/root/include/Varargs.h
EdbVirtual.o: /var/local/root/include/TStorage.h
EdbVirtual.o: /var/local/root/include/Riosfwd.h
EdbVirtual.o: /var/local/root/include/TBuffer.h
EdbVirtual.o: /var/local/root/include/Bytes.h /var/local/root/include/TList.h
EdbVirtual.o: /var/local/root/include/TSeqCollection.h
EdbVirtual.o: /var/local/root/include/TCollection.h
EdbVirtual.o: /var/local/root/include/TIterator.h
EdbVirtual.o: /var/local/root/include/TCanvas.h
EdbVirtual.o: /var/local/root/include/TPad.h
EdbVirtual.o: /var/local/root/include/TVirtualPad.h
EdbVirtual.o: /var/local/root/include/TAttPad.h
EdbVirtual.o: /var/local/root/include/TVirtualX.h
EdbVirtual.o: /var/local/root/include/TAttText.h
EdbVirtual.o: /var/local/root/include/GuiTypes.h
EdbVirtual.o: /var/local/root/include/Buttons.h
EdbVirtual.o: /var/local/root/include/TQObject.h
EdbVirtual.o: /var/local/root/include/TAttCanvas.h
EdbVirtual.o: /var/local/root/include/TCanvasImp.h EdbVirtual.h EdbAffine.h
