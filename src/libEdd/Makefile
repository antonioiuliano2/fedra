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

EdbDisplay.o: /var/local/root/include/TStyle.h
EdbDisplay.o: /var/local/root/include/TNamed.h
EdbDisplay.o: /var/local/root/include/TObject.h
EdbDisplay.o: /var/local/root/include/Rtypes.h
EdbDisplay.o: /var/local/root/include/RConfig.h
EdbDisplay.o: /var/local/root/include/RVersion.h
EdbDisplay.o: /var/local/root/include/DllImport.h
EdbDisplay.o: /var/local/root/include/Rtypeinfo.h
EdbDisplay.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
EdbDisplay.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
EdbDisplay.o: /usr/include/stdio.h /usr/include/features.h
EdbDisplay.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
EdbDisplay.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
EdbDisplay.o: /usr/include/bits/types.h /usr/include/libio.h
EdbDisplay.o: /usr/include/_G_config.h /usr/include/wchar.h
EdbDisplay.o: /usr/include/bits/wchar.h /usr/include/gconv.h
EdbDisplay.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
EdbDisplay.o: /usr/include/bits/stdio_lim.h
EdbDisplay.o: /var/local/root/include/TGenericClassInfo.h
EdbDisplay.o: /var/local/root/include/Varargs.h
EdbDisplay.o: /var/local/root/include/TStorage.h
EdbDisplay.o: /var/local/root/include/Riosfwd.h
EdbDisplay.o: /var/local/root/include/TBuffer.h
EdbDisplay.o: /var/local/root/include/Bytes.h /usr/include/string.h
EdbDisplay.o: /var/local/root/include/TList.h
EdbDisplay.o: /var/local/root/include/TSeqCollection.h
EdbDisplay.o: /var/local/root/include/TCollection.h
EdbDisplay.o: /var/local/root/include/TIterator.h
EdbDisplay.o: /var/local/root/include/TString.h
EdbDisplay.o: /var/local/root/include/TMath.h
EdbDisplay.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
EdbDisplay.o: /var/local/root/include/TRefCnt.h
EdbDisplay.o: /var/local/root/include/TAttAxis.h
EdbDisplay.o: /var/local/root/include/Gtypes.h
EdbDisplay.o: /var/local/root/include/Htypes.h
EdbDisplay.o: /var/local/root/include/TAttLine.h
EdbDisplay.o: /var/local/root/include/TAttFill.h
EdbDisplay.o: /var/local/root/include/TAttText.h
EdbDisplay.o: /var/local/root/include/TAttMarker.h
EdbDisplay.o: /var/local/root/include/TArrayI.h
EdbDisplay.o: /var/local/root/include/TArray.h EdbDisplay.h EdbDisplayBase.h
EdbDisplay.o: /var/local/root/include/TCanvas.h
EdbDisplay.o: /var/local/root/include/TPad.h
EdbDisplay.o: /var/local/root/include/TVirtualPad.h
EdbDisplay.o: /var/local/root/include/TAttPad.h
EdbDisplay.o: /var/local/root/include/TVirtualX.h
EdbDisplay.o: /var/local/root/include/GuiTypes.h
EdbDisplay.o: /var/local/root/include/Buttons.h
EdbDisplay.o: /var/local/root/include/TQObject.h
EdbDisplay.o: /var/local/root/include/TClass.h
EdbDisplay.o: /var/local/root/include/TDictionary.h
EdbDisplay.o: /var/local/root/include/Property.h
EdbDisplay.o: /var/local/root/include/TAttCanvas.h
EdbDisplay.o: /var/local/root/include/TCanvasImp.h
EdbDisplay.o: /var/local/root/include/TSlider.h
EdbDisplay.o: /var/local/root/include/TView.h
EdbDisplay.o: /var/local/root/include/TPolyLine3D.h
EdbDisplay.o: /var/local/root/include/TAtt3D.h
EdbDisplay.o: /var/local/root/include/X3DBuffer.h
EdbDisplay.o: /var/local/root/include/TPolyMarker3D.h
EdbDisplay.o: /var/local/root/include/TClonesArray.h
EdbDisplay.o: /var/local/root/include/TObjArray.h
EdbDisplay.o: /var/local/root/include/TArrayF.h
EdbDisplay.o: /var/local/root/include/TButton.h
EdbDisplay.o: /var/local/root/include/TGClient.h
EdbDisplay.o: /var/local/root/include/TGButton.h
EdbDisplay.o: /var/local/root/include/TGFrame.h
EdbDisplay.o: /var/local/root/include/TGWindow.h
EdbDisplay.o: /var/local/root/include/TGObject.h
EdbDisplay.o: /var/local/root/include/TGDimension.h
EdbDisplay.o: /var/local/root/include/TGGC.h /var/local/root/include/TGFont.h
EdbDisplay.o: /var/local/root/include/TGLayout.h
EdbDisplay.o: /var/local/root/include/TGString.h
EdbDisplay.o: /var/local/root/include/TGWidget.h
EdbDisplay.o: /var/local/root/include/WidgetMessageTypes.h
EdbDisplay.o: /var/local/root/include/TTree.h
EdbDisplay.o: /var/local/root/include/TBranch.h
EdbDisplay.o: /var/local/root/include/TStringLong.h
EdbDisplay.o: /var/local/root/include/TArrayD.h
EdbDisplay.o: /var/local/root/include/TVirtualTreePlayer.h
EdbDisplay.o: /var/local/root/include/TPaveLabel.h
EdbDisplay.o: /var/local/root/include/TPave.h /var/local/root/include/TBox.h
EdbDisplay.o: /var/local/root/include/TPaveText.h
EdbDisplay.o: /var/local/root/include/TText.h /var/local/root/include/TLine.h
EdbDisplay.o: /var/local/root/include/TDiamond.h
EdbDisplay.o: /var/local/root/include/TArc.h
EdbDisplay.o: /var/local/root/include/TEllipse.h
EdbDisplay.o: /var/local/root/include/TSliderBox.h
EdbDisplay.o: /var/local/root/include/TWbox.h
EdbDisplay.o: /var/local/root/include/TGaxis.h
EdbDisplayBase.o: EdbDisplayBase.h /var/local/root/include/TObject.h
EdbDisplayBase.o: /var/local/root/include/Rtypes.h
EdbDisplayBase.o: /var/local/root/include/RConfig.h
EdbDisplayBase.o: /var/local/root/include/RVersion.h
EdbDisplayBase.o: /var/local/root/include/DllImport.h
EdbDisplayBase.o: /var/local/root/include/Rtypeinfo.h
EdbDisplayBase.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
EdbDisplayBase.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
EdbDisplayBase.o: /usr/include/stdio.h /usr/include/features.h
EdbDisplayBase.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
EdbDisplayBase.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
EdbDisplayBase.o: /usr/include/bits/types.h /usr/include/libio.h
EdbDisplayBase.o: /usr/include/_G_config.h /usr/include/wchar.h
EdbDisplayBase.o: /usr/include/bits/wchar.h /usr/include/gconv.h
EdbDisplayBase.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
EdbDisplayBase.o: /usr/include/bits/stdio_lim.h
EdbDisplayBase.o: /var/local/root/include/TGenericClassInfo.h
EdbDisplayBase.o: /var/local/root/include/Varargs.h
EdbDisplayBase.o: /var/local/root/include/TStorage.h
EdbDisplayBase.o: /var/local/root/include/Riosfwd.h
EdbDisplayBase.o: /var/local/root/include/TBuffer.h
EdbDisplayBase.o: /var/local/root/include/Bytes.h /usr/include/string.h
EdbDisplayBase.o: /var/local/root/include/TCanvas.h
EdbDisplayBase.o: /var/local/root/include/TPad.h
EdbDisplayBase.o: /var/local/root/include/TVirtualPad.h
EdbDisplayBase.o: /var/local/root/include/TAttPad.h
EdbDisplayBase.o: /var/local/root/include/TVirtualX.h
EdbDisplayBase.o: /var/local/root/include/GuiTypes.h
EdbDisplayBase.o: /var/local/root/include/Buttons.h
EdbDisplayBase.o: /var/local/root/include/TQObject.h
EdbDisplayBase.o: /var/local/root/include/TClass.h
EdbDisplayBase.o: /var/local/root/include/TDictionary.h
EdbDisplayBase.o: /var/local/root/include/Property.h
EdbDisplayBase.o: /var/local/root/include/TAttCanvas.h
EdbDisplayBase.o: /var/local/root/include/TCanvasImp.h
EdbDisplayBase.o: /var/local/root/include/TSlider.h
EdbDisplayBase.o: /var/local/root/include/TView.h
EdbDisplayBase.o: /var/local/root/include/TPolyLine3D.h
EdbDisplayBase.o: /var/local/root/include/TString.h
EdbDisplayBase.o: /var/local/root/include/TMath.h
EdbDisplayBase.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
EdbDisplayBase.o: /var/local/root/include/TRefCnt.h
EdbDisplayBase.o: /var/local/root/include/TAtt3D.h
EdbDisplayBase.o: /var/local/root/include/X3DBuffer.h
EdbDisplayBase.o: /var/local/root/include/TPolyMarker3D.h
EdbDisplayBase.o: /var/local/root/include/TClonesArray.h
EdbDisplayBase.o: /var/local/root/include/TObjArray.h
EdbDisplayBase.o: /var/local/root/include/TArrayF.h
EdbDisplayBase.o: /var/local/root/include/TArrayI.h
EdbDisplayBase.o: /var/local/root/include/TArray.h
EdbDisplayBase.o: /var/local/root/include/TButton.h
EdbDisplayBase.o: /var/local/root/include/TGClient.h
EdbDisplayBase.o: /var/local/root/include/TGButton.h
EdbDisplayBase.o: /var/local/root/include/TGFrame.h
EdbDisplayBase.o: /var/local/root/include/TGWindow.h
EdbDisplayBase.o: /var/local/root/include/TGObject.h
EdbDisplayBase.o: /var/local/root/include/TGDimension.h
EdbDisplayBase.o: /var/local/root/include/TGGC.h
EdbDisplayBase.o: /var/local/root/include/TGFont.h
EdbDisplayBase.o: /var/local/root/include/TGLayout.h
EdbDisplayBase.o: /var/local/root/include/TGString.h
EdbDisplayBase.o: /var/local/root/include/TGWidget.h
EdbDisplayBase.o: /var/local/root/include/WidgetMessageTypes.h
EdbDisplayBase.o: /var/local/root/include/TTree.h
EdbDisplayBase.o: /var/local/root/include/TBranch.h
EdbDisplayBase.o: /var/local/root/include/TStringLong.h
EdbDisplayBase.o: /var/local/root/include/TArrayD.h
EdbDisplayBase.o: /var/local/root/include/TVirtualTreePlayer.h
EdbDisplayBase.o: /var/local/root/include/TPaveLabel.h
EdbDisplayBase.o: /var/local/root/include/TPave.h
EdbDisplayBase.o: /var/local/root/include/TBox.h
EdbDisplayBase.o: /var/local/root/include/TPaveText.h
EdbDisplayBase.o: /var/local/root/include/TText.h
EdbDisplayBase.o: /var/local/root/include/TLine.h
EdbDisplayBase.o: /var/local/root/include/TDiamond.h
EdbDisplayBase.o: /var/local/root/include/TArc.h
EdbDisplayBase.o: /var/local/root/include/TEllipse.h
EdbDisplayBase.o: /var/local/root/include/TSliderBox.h
EdbDisplayBase.o: /var/local/root/include/TWbox.h
EdbDisplayBase.o: /var/local/root/include/TGaxis.h
EdbDisplayBase.o: /var/local/root/include/TROOT.h
EdbDisplayBase.o: /var/local/root/include/TDirectory.h
EdbDisplayBase.o: /var/local/root/include/TNamed.h
EdbDisplayBase.o: /var/local/root/include/TList.h
EdbDisplayBase.o: /var/local/root/include/TSeqCollection.h
EdbDisplayBase.o: /var/local/root/include/TCollection.h
EdbDisplayBase.o: /var/local/root/include/TIterator.h
EdbDisplayBase.o: /var/local/root/include/TDatime.h
EdbDisplayBase.o: /var/local/root/include/Htypes.h
EdbDisplayBase.o: /var/local/root/include/TUUID.h
EdbDisplayBase.o: /var/local/root/include/TInetAddress.h
