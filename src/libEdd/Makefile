#------------------------------------------------------------------------------
include ../config/ProjectDef.mk
include ../config/RootDef.mk
#------------------------------------------------------------------------------

PROJECT_LIBS += -lEmath -lEdb

LIB_TGT       = $(LIB_DIR)/libEdd.so

headers       = EdbDisplayBase.h  EdbDisplay.h

classes	      = $(headers:.h=.$(SrcSuf))
EddCint	      = $(headers) EddLinkDef.h
SRC           = $(classes) EddCint.$(SrcSuf)
OBJ           = $(SRC:.$(SrcSuf)=.$(ObjSuf))

CXXFLAGS += -I$(INC_DIR)

#------------------------------------------------------------------------------

$(LIB_TGT): $(OBJ)
	$(CXX) -O -shared $(LIBS) -o $(LIB_TGT) $(OBJ)

EddCint.cxx: $(EddCint)
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

threadEdbDisplay.o: /var/local/root/include/TStyle.h
threadEdbDisplay.o: /var/local/root/include/TNamed.h
threadEdbDisplay.o: /var/local/root/include/TObject.h
threadEdbDisplay.o: /var/local/root/include/Rtypes.h
threadEdbDisplay.o: /var/local/root/include/RConfig.h
threadEdbDisplay.o: /var/local/root/include/RVersion.h
threadEdbDisplay.o: /var/local/root/include/DllImport.h
threadEdbDisplay.o: /var/local/root/include/Rtypeinfo.h
threadEdbDisplay.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
threadEdbDisplay.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
threadEdbDisplay.o: /usr/include/stdio.h /usr/include/features.h
threadEdbDisplay.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
threadEdbDisplay.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
threadEdbDisplay.o: /usr/include/bits/types.h /usr/include/libio.h
threadEdbDisplay.o: /usr/include/_G_config.h /usr/include/wchar.h
threadEdbDisplay.o: /usr/include/bits/wchar.h /usr/include/gconv.h
threadEdbDisplay.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
threadEdbDisplay.o: /usr/include/bits/stdio_lim.h
threadEdbDisplay.o: /var/local/root/include/TGenericClassInfo.h
threadEdbDisplay.o: /var/local/root/include/Varargs.h
threadEdbDisplay.o: /var/local/root/include/TStorage.h
threadEdbDisplay.o: /var/local/root/include/Riosfwd.h
threadEdbDisplay.o: /var/local/root/include/TBuffer.h
threadEdbDisplay.o: /var/local/root/include/Bytes.h /usr/include/string.h
threadEdbDisplay.o: /var/local/root/include/TList.h
threadEdbDisplay.o: /var/local/root/include/TSeqCollection.h
threadEdbDisplay.o: /var/local/root/include/TCollection.h
threadEdbDisplay.o: /var/local/root/include/TIterator.h
threadEdbDisplay.o: /var/local/root/include/TString.h
threadEdbDisplay.o: /var/local/root/include/TMath.h
threadEdbDisplay.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
threadEdbDisplay.o: /var/local/root/include/TRefCnt.h
threadEdbDisplay.o: /var/local/root/include/TAttAxis.h
threadEdbDisplay.o: /var/local/root/include/Gtypes.h
threadEdbDisplay.o: /var/local/root/include/Htypes.h
threadEdbDisplay.o: /var/local/root/include/TAttLine.h
threadEdbDisplay.o: /var/local/root/include/TAttFill.h
threadEdbDisplay.o: /var/local/root/include/TAttText.h
threadEdbDisplay.o: /var/local/root/include/TAttMarker.h
threadEdbDisplay.o: /var/local/root/include/TArrayI.h
threadEdbDisplay.o: /var/local/root/include/TArray.h EdbDisplay.h
threadEdbDisplay.o: EdbDisplayBase.h /var/local/root/include/TCanvas.h
threadEdbDisplay.o: /var/local/root/include/TPad.h
threadEdbDisplay.o: /var/local/root/include/TVirtualPad.h
threadEdbDisplay.o: /var/local/root/include/TAttPad.h
threadEdbDisplay.o: /var/local/root/include/TVirtualX.h
threadEdbDisplay.o: /var/local/root/include/GuiTypes.h
threadEdbDisplay.o: /var/local/root/include/Buttons.h
threadEdbDisplay.o: /var/local/root/include/TQObject.h
threadEdbDisplay.o: /var/local/root/include/TClass.h
threadEdbDisplay.o: /var/local/root/include/TDictionary.h
threadEdbDisplay.o: /var/local/root/include/Property.h
threadEdbDisplay.o: /var/local/root/include/TROOT.h
threadEdbDisplay.o: /var/local/root/include/TDirectory.h
threadEdbDisplay.o: /var/local/root/include/TDatime.h
threadEdbDisplay.o: /var/local/root/include/TUUID.h
threadEdbDisplay.o: /var/local/root/include/TObjArray.h
threadEdbDisplay.o: /var/local/root/include/TStreamerInfo.h
threadEdbDisplay.o: /var/local/root/include/TFile.h
threadEdbDisplay.o: /var/local/root/include/TCache.h
threadEdbDisplay.o: /var/local/root/include/THashList.h
threadEdbDisplay.o: /var/local/root/include/TClonesArray.h
threadEdbDisplay.o: /var/local/root/include/TBuffer3D.h
threadEdbDisplay.o: /var/local/root/include/TVirtualViewer3D.h
threadEdbDisplay.o: /var/local/root/include/TAttCanvas.h
threadEdbDisplay.o: /var/local/root/include/TCanvasImp.h
threadEdbDisplay.o: /var/local/root/include/TSlider.h
threadEdbDisplay.o: /var/local/root/include/TView.h
threadEdbDisplay.o: /var/local/root/include/TPolyLine3D.h
threadEdbDisplay.o: /var/local/root/include/TAtt3D.h
threadEdbDisplay.o: /var/local/root/include/TPolyMarker3D.h
threadEdbDisplay.o: /var/local/root/include/TArrayF.h
threadEdbDisplay.o: /var/local/root/include/TButton.h
threadEdbDisplay.o: /var/local/root/include/TGClient.h
threadEdbDisplay.o: /var/local/root/include/TGButton.h
threadEdbDisplay.o: /var/local/root/include/TGFrame.h
threadEdbDisplay.o: /var/local/root/include/TGWindow.h
threadEdbDisplay.o: /var/local/root/include/TGObject.h
threadEdbDisplay.o: /var/local/root/include/TGDimension.h
threadEdbDisplay.o: /var/local/root/include/TGGC.h
threadEdbDisplay.o: /var/local/root/include/TGFont.h
threadEdbDisplay.o: /var/local/root/include/TGLayout.h
threadEdbDisplay.o: /var/local/root/include/TGString.h
threadEdbDisplay.o: /var/local/root/include/TGWidget.h
threadEdbDisplay.o: /var/local/root/include/WidgetMessageTypes.h
threadEdbDisplay.o: /var/local/root/include/TTree.h
threadEdbDisplay.o: /var/local/root/include/TBranch.h
threadEdbDisplay.o: /var/local/root/include/TStringLong.h
threadEdbDisplay.o: /var/local/root/include/TArrayD.h
threadEdbDisplay.o: /var/local/root/include/TVirtualTreePlayer.h
threadEdbDisplay.o: /var/local/root/include/TPaveLabel.h
threadEdbDisplay.o: /var/local/root/include/TPave.h
threadEdbDisplay.o: /var/local/root/include/TBox.h
threadEdbDisplay.o: /var/local/root/include/TPaveText.h
threadEdbDisplay.o: /var/local/root/include/TText.h
threadEdbDisplay.o: /var/local/root/include/TLine.h
threadEdbDisplay.o: /var/local/root/include/TDiamond.h
threadEdbDisplay.o: /var/local/root/include/TArc.h
threadEdbDisplay.o: /var/local/root/include/TEllipse.h
threadEdbDisplay.o: /var/local/root/include/TSliderBox.h
threadEdbDisplay.o: /var/local/root/include/TWbox.h
threadEdbDisplay.o: /var/local/root/include/TGaxis.h
threadEdbDisplayBase.o: EdbDisplayBase.h /var/local/root/include/TObject.h
threadEdbDisplayBase.o: /var/local/root/include/Rtypes.h
threadEdbDisplayBase.o: /var/local/root/include/RConfig.h
threadEdbDisplayBase.o: /var/local/root/include/RVersion.h
threadEdbDisplayBase.o: /var/local/root/include/DllImport.h
threadEdbDisplayBase.o: /var/local/root/include/Rtypeinfo.h
threadEdbDisplayBase.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
threadEdbDisplayBase.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
threadEdbDisplayBase.o: /usr/include/stdio.h /usr/include/features.h
threadEdbDisplayBase.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
threadEdbDisplayBase.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
threadEdbDisplayBase.o: /usr/include/bits/types.h /usr/include/libio.h
threadEdbDisplayBase.o: /usr/include/_G_config.h /usr/include/wchar.h
threadEdbDisplayBase.o: /usr/include/bits/wchar.h /usr/include/gconv.h
threadEdbDisplayBase.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
threadEdbDisplayBase.o: /usr/include/bits/stdio_lim.h
threadEdbDisplayBase.o: /var/local/root/include/TGenericClassInfo.h
threadEdbDisplayBase.o: /var/local/root/include/Varargs.h
threadEdbDisplayBase.o: /var/local/root/include/TStorage.h
threadEdbDisplayBase.o: /var/local/root/include/Riosfwd.h
threadEdbDisplayBase.o: /var/local/root/include/TBuffer.h
threadEdbDisplayBase.o: /var/local/root/include/Bytes.h /usr/include/string.h
threadEdbDisplayBase.o: /var/local/root/include/TCanvas.h
threadEdbDisplayBase.o: /var/local/root/include/TPad.h
threadEdbDisplayBase.o: /var/local/root/include/TVirtualPad.h
threadEdbDisplayBase.o: /var/local/root/include/TAttPad.h
threadEdbDisplayBase.o: /var/local/root/include/TVirtualX.h
threadEdbDisplayBase.o: /var/local/root/include/GuiTypes.h
threadEdbDisplayBase.o: /var/local/root/include/Buttons.h
threadEdbDisplayBase.o: /var/local/root/include/TQObject.h
threadEdbDisplayBase.o: /var/local/root/include/TClass.h
threadEdbDisplayBase.o: /var/local/root/include/TDictionary.h
threadEdbDisplayBase.o: /var/local/root/include/Property.h
threadEdbDisplayBase.o: /var/local/root/include/TROOT.h
threadEdbDisplayBase.o: /var/local/root/include/TDirectory.h
threadEdbDisplayBase.o: /var/local/root/include/TDatime.h
threadEdbDisplayBase.o: /var/local/root/include/TUUID.h
threadEdbDisplayBase.o: /var/local/root/include/TObjArray.h
threadEdbDisplayBase.o: /var/local/root/include/TStreamerInfo.h
threadEdbDisplayBase.o: /var/local/root/include/TFile.h
threadEdbDisplayBase.o: /var/local/root/include/TCache.h
threadEdbDisplayBase.o: /var/local/root/include/THashList.h
threadEdbDisplayBase.o: /var/local/root/include/TClonesArray.h
threadEdbDisplayBase.o: /var/local/root/include/TBuffer3D.h
threadEdbDisplayBase.o: /var/local/root/include/TVirtualViewer3D.h
threadEdbDisplayBase.o: /var/local/root/include/TAttCanvas.h
threadEdbDisplayBase.o: /var/local/root/include/TCanvasImp.h
threadEdbDisplayBase.o: /var/local/root/include/TSlider.h
threadEdbDisplayBase.o: /var/local/root/include/TView.h
threadEdbDisplayBase.o: /var/local/root/include/TPolyLine3D.h
threadEdbDisplayBase.o: /var/local/root/include/TString.h
threadEdbDisplayBase.o: /var/local/root/include/TMath.h
threadEdbDisplayBase.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
threadEdbDisplayBase.o: /var/local/root/include/TRefCnt.h
threadEdbDisplayBase.o: /var/local/root/include/TAtt3D.h
threadEdbDisplayBase.o: /var/local/root/include/TPolyMarker3D.h
threadEdbDisplayBase.o: /var/local/root/include/TArrayF.h
threadEdbDisplayBase.o: /var/local/root/include/TArrayI.h
threadEdbDisplayBase.o: /var/local/root/include/TArray.h
threadEdbDisplayBase.o: /var/local/root/include/TButton.h
threadEdbDisplayBase.o: /var/local/root/include/TGClient.h
threadEdbDisplayBase.o: /var/local/root/include/TGButton.h
threadEdbDisplayBase.o: /var/local/root/include/TGFrame.h
threadEdbDisplayBase.o: /var/local/root/include/TGWindow.h
threadEdbDisplayBase.o: /var/local/root/include/TGObject.h
threadEdbDisplayBase.o: /var/local/root/include/TGDimension.h
threadEdbDisplayBase.o: /var/local/root/include/TGGC.h
threadEdbDisplayBase.o: /var/local/root/include/TGFont.h
threadEdbDisplayBase.o: /var/local/root/include/TGLayout.h
threadEdbDisplayBase.o: /var/local/root/include/TGString.h
threadEdbDisplayBase.o: /var/local/root/include/TGWidget.h
threadEdbDisplayBase.o: /var/local/root/include/WidgetMessageTypes.h
threadEdbDisplayBase.o: /var/local/root/include/TTree.h
threadEdbDisplayBase.o: /var/local/root/include/TBranch.h
threadEdbDisplayBase.o: /var/local/root/include/TStringLong.h
threadEdbDisplayBase.o: /var/local/root/include/TArrayD.h
threadEdbDisplayBase.o: /var/local/root/include/TVirtualTreePlayer.h
threadEdbDisplayBase.o: /var/local/root/include/TPaveLabel.h
threadEdbDisplayBase.o: /var/local/root/include/TPave.h
threadEdbDisplayBase.o: /var/local/root/include/TBox.h
threadEdbDisplayBase.o: /var/local/root/include/TPaveText.h
threadEdbDisplayBase.o: /var/local/root/include/TText.h
threadEdbDisplayBase.o: /var/local/root/include/TLine.h
threadEdbDisplayBase.o: /var/local/root/include/TDiamond.h
threadEdbDisplayBase.o: /var/local/root/include/TArc.h
threadEdbDisplayBase.o: /var/local/root/include/TEllipse.h
threadEdbDisplayBase.o: /var/local/root/include/TSliderBox.h
threadEdbDisplayBase.o: /var/local/root/include/TWbox.h
threadEdbDisplayBase.o: /var/local/root/include/TGaxis.h
threadEddCint.o: /var/local/root/include/RConfig.h
threadEddCint.o: /var/local/root/include/RVersion.h EddCint.h
threadEddCint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
threadEddCint.o: /usr/include/stdio.h /usr/include/features.h
threadEddCint.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
threadEddCint.o: /usr/include/bits/types.h /usr/include/libio.h
threadEddCint.o: /usr/include/_G_config.h /usr/include/wchar.h
threadEddCint.o: /usr/include/bits/wchar.h /usr/include/gconv.h
threadEddCint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
threadEddCint.o: /usr/include/bits/stdio_lim.h /usr/include/stdlib.h
threadEddCint.o: /usr/include/math.h /usr/include/bits/huge_val.h
threadEddCint.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
threadEddCint.o: /usr/include/string.h /var/local/root/include/G__ci.h
threadEddCint.o: /usr/include/signal.h /usr/include/bits/sigset.h
threadEddCint.o: /usr/include/bits/signum.h /usr/include/assert.h
threadEddCint.o: /usr/include/limits.h
threadEddCint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
threadEddCint.o: /usr/include/setjmp.h /usr/include/bits/setjmp.h
threadEddCint.o: /usr/include/ctype.h /usr/include/endian.h
threadEddCint.o: /usr/include/bits/endian.h /usr/include/fcntl.h
threadEddCint.o: /usr/include/bits/fcntl.h /usr/include/sys/types.h
threadEddCint.o: /usr/include/time.h /usr/include/unistd.h
threadEddCint.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
threadEddCint.o: /var/local/root/include/TROOT.h
threadEddCint.o: /var/local/root/include/TDirectory.h
threadEddCint.o: /var/local/root/include/TDatime.h
threadEddCint.o: /var/local/root/include/TUUID.h
threadEddCint.o: /var/local/root/include/TMemberInspector.h
threadEddCint.o: /var/local/root/include/TObject.h
threadEddCint.o: /var/local/root/include/Rtypes.h
threadEddCint.o: /var/local/root/include/DllImport.h
threadEddCint.o: /var/local/root/include/Rtypeinfo.h
threadEddCint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
threadEddCint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
threadEddCint.o: /var/local/root/include/TGenericClassInfo.h
threadEddCint.o: /var/local/root/include/Varargs.h
threadEddCint.o: /var/local/root/include/TStorage.h
threadEddCint.o: /var/local/root/include/Riosfwd.h
threadEddCint.o: /var/local/root/include/TBuffer.h
threadEddCint.o: /var/local/root/include/Bytes.h EdbDisplayBase.h
threadEddCint.o: /var/local/root/include/TCanvas.h
threadEddCint.o: /var/local/root/include/TPad.h
threadEddCint.o: /var/local/root/include/TVirtualPad.h
threadEddCint.o: /var/local/root/include/TAttPad.h
threadEddCint.o: /var/local/root/include/TVirtualX.h
threadEddCint.o: /var/local/root/include/GuiTypes.h
threadEddCint.o: /var/local/root/include/Buttons.h
threadEddCint.o: /var/local/root/include/TQObject.h
threadEddCint.o: /var/local/root/include/TClass.h
threadEddCint.o: /var/local/root/include/TDictionary.h
threadEddCint.o: /var/local/root/include/Property.h
threadEddCint.o: /var/local/root/include/TObjArray.h
threadEddCint.o: /var/local/root/include/TStreamerInfo.h
threadEddCint.o: /var/local/root/include/TFile.h
threadEddCint.o: /var/local/root/include/TCache.h
threadEddCint.o: /var/local/root/include/THashList.h
threadEddCint.o: /var/local/root/include/TClonesArray.h
threadEddCint.o: /var/local/root/include/TBuffer3D.h
threadEddCint.o: /var/local/root/include/TVirtualViewer3D.h
threadEddCint.o: /var/local/root/include/TAttCanvas.h
threadEddCint.o: /var/local/root/include/TCanvasImp.h
threadEddCint.o: /var/local/root/include/TSlider.h
threadEddCint.o: /var/local/root/include/TView.h
threadEddCint.o: /var/local/root/include/TPolyLine3D.h
threadEddCint.o: /var/local/root/include/TString.h
threadEddCint.o: /var/local/root/include/TMath.h
threadEddCint.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
threadEddCint.o: /var/local/root/include/TRefCnt.h
threadEddCint.o: /var/local/root/include/TAtt3D.h
threadEddCint.o: /var/local/root/include/TPolyMarker3D.h
threadEddCint.o: /var/local/root/include/TArrayF.h
threadEddCint.o: /var/local/root/include/TArrayI.h
threadEddCint.o: /var/local/root/include/TArray.h
threadEddCint.o: /var/local/root/include/TButton.h
threadEddCint.o: /var/local/root/include/TGClient.h
threadEddCint.o: /var/local/root/include/TGButton.h
threadEddCint.o: /var/local/root/include/TGFrame.h
threadEddCint.o: /var/local/root/include/TGWindow.h
threadEddCint.o: /var/local/root/include/TGObject.h
threadEddCint.o: /var/local/root/include/TGDimension.h
threadEddCint.o: /var/local/root/include/TGGC.h
threadEddCint.o: /var/local/root/include/TGFont.h
threadEddCint.o: /var/local/root/include/TGLayout.h
threadEddCint.o: /var/local/root/include/TGString.h
threadEddCint.o: /var/local/root/include/TGWidget.h
threadEddCint.o: /var/local/root/include/WidgetMessageTypes.h
threadEddCint.o: /var/local/root/include/TTree.h
threadEddCint.o: /var/local/root/include/TBranch.h
threadEddCint.o: /var/local/root/include/TStringLong.h
threadEddCint.o: /var/local/root/include/TArrayD.h
threadEddCint.o: /var/local/root/include/TVirtualTreePlayer.h
threadEddCint.o: /var/local/root/include/TPaveLabel.h
threadEddCint.o: /var/local/root/include/TPave.h
threadEddCint.o: /var/local/root/include/TBox.h
threadEddCint.o: /var/local/root/include/TPaveText.h
threadEddCint.o: /var/local/root/include/TText.h
threadEddCint.o: /var/local/root/include/TLine.h
threadEddCint.o: /var/local/root/include/TDiamond.h
threadEddCint.o: /var/local/root/include/TArc.h
threadEddCint.o: /var/local/root/include/TEllipse.h
threadEddCint.o: /var/local/root/include/TSliderBox.h
threadEddCint.o: /var/local/root/include/TWbox.h
threadEddCint.o: /var/local/root/include/TGaxis.h EdbDisplay.h
threadEddCint.o: /var/local/root/include/TError.h
threadEddCint.o: /var/local/root/include/RtypesImp.h
threadEddCint.o: /var/local/root/include/Api.h
threadEddCint.o: /var/local/root/include/common.h
threadEddCint.o: /var/local/root/include/security.h
threadEddCint.o: /var/local/root/include/fproto.h
threadEddCint.o: /var/local/root/include/global.h
threadEddCint.o: /var/local/root/include/Class.h
threadEddCint.o: /var/local/root/include/BaseCls.h
threadEddCint.o: /var/local/root/include/Type.h
threadEddCint.o: /var/local/root/include/Method.h
threadEddCint.o: /var/local/root/include/MethodAr.h
threadEddCint.o: /var/local/root/include/DataMbr.h
threadEddCint.o: /var/local/root/include/CallFunc.h
threadEddCint.o: /var/local/root/include/Typedf.h
threadEddCint.o: /var/local/root/include/Token.h
threadEddCint.o: /var/local/root/include/TClassEdit.h
threadEddCint.o: /var/local/root/include/TCollectionProxy.h
threadEddCint.o: /var/local/root/include/TClassStreamer.h
threadEddCint.o: /var/local/root/include/TMemberStreamer.h
