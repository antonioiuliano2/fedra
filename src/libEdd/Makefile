#------------------------------------------------------------------------------
include ../config/ProjectDef.mk
include ../config/RootDef.mk
#------------------------------------------------------------------------------

PROJECT_LIBS += -lEmath -lEdb

LIB_TGT       = $(LIB_DIR)/libEdd.so

headers       = EdbDisplay.h

classes	      = $(headers:.h=.$(SrcSuf))
EddCint	      = $(headers) EddLinkDef.h
SRC           = $(classes) EddCint.$(SrcSuf)
OBJ           = $(SRC:.$(SrcSuf)=.$(ObjSuf))

CXXFLAGS += -I$(INC_DIR)

#------------------------------------------------------------------------------

$(LIB_TGT): $(OBJ)
	$(CXX) -O -shared $(LIBS) -o $(LIB_TGT) $(OBJ)

#EddCint.cxx: $(EddCint)
#	rootcint -f EddCint.cxx -c $(INCLUDE) -I$(INC_DIR) $(EddCint)

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

EdbDisplay.o: EdbDisplay.h /var/local/root/include/TObject.h
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
EdbDisplay.o: /var/local/root/include/TCanvas.h
EdbDisplay.o: /var/local/root/include/TPad.h
EdbDisplay.o: /var/local/root/include/TVirtualPad.h
EdbDisplay.o: /var/local/root/include/TAttPad.h
EdbDisplay.o: /var/local/root/include/Gtypes.h
EdbDisplay.o: /var/local/root/include/Htypes.h
EdbDisplay.o: /var/local/root/include/TVirtualX.h
EdbDisplay.o: /var/local/root/include/TNamed.h
EdbDisplay.o: /var/local/root/include/TList.h
EdbDisplay.o: /var/local/root/include/TSeqCollection.h
EdbDisplay.o: /var/local/root/include/TCollection.h
EdbDisplay.o: /var/local/root/include/TIterator.h
EdbDisplay.o: /var/local/root/include/TString.h
EdbDisplay.o: /var/local/root/include/TMath.h
EdbDisplay.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
EdbDisplay.o: /var/local/root/include/TRefCnt.h
EdbDisplay.o: /var/local/root/include/TAttLine.h
EdbDisplay.o: /var/local/root/include/TAttFill.h
EdbDisplay.o: /var/local/root/include/TAttText.h
EdbDisplay.o: /var/local/root/include/TAttMarker.h
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
EdbDisplay.o: /var/local/root/include/TArray.h
EdbDisplay.o: /var/local/root/include/TArrayI.h
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
EdbDisplay.o: /var/local/root/include/TROOT.h
EdbDisplay.o: /var/local/root/include/TDirectory.h
EdbDisplay.o: /var/local/root/include/TDatime.h
EdbDisplay.o: /var/local/root/include/TUUID.h
EdbDisplay.o: /var/local/root/include/TInetAddress.h
EdbDisplay.o: /var/local/root/include/TStyle.h
EdbDisplay.o: /var/local/root/include/TAttAxis.h
