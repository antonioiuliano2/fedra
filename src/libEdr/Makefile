#------------------------------------------------------------------------------
include ../config/ProjectDef.mk
include ../config/RootDef.mk
#------------------------------------------------------------------------------

PROJECT_LIBS += -lEmath -lEdb

LIB_TGT       = $(LIB_DIR)/libEdr.so

headers       = EdbPattern.h EdbPVGen.h EdbPVRec.h EdbGA.h EdbDataSet.h

classes	      = $(headers:.h=.$(SrcSuf))
EdrCint	      = $(headers) EdrLinkDef.h
SRC           = $(classes) EdrCint.$(SrcSuf)
OBJ           = $(SRC:.$(SrcSuf)=.$(ObjSuf))

CXXFLAGS += -I$(INC_DIR)

#------------------------------------------------------------------------------

$(LIB_TGT): $(OBJ)
	$(CXX) -O -shared $(LIBS) -o $(LIB_TGT) $(OBJ)

#EdrCint.cxx: $(EdrCint)
#	rootcint -f EdrCint.cxx -c $(INCLUDE) -I$(INC_DIR) $(EdrCint)

EdrCint.cxx: $(EdrCint)
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

EdbDataSet.o: /var/local/root/include/TROOT.h
EdbDataSet.o: /var/local/root/include/TDirectory.h
EdbDataSet.o: /var/local/root/include/TNamed.h
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
EdbDataSet.o: /var/local/root/include/TList.h
EdbDataSet.o: /var/local/root/include/TSeqCollection.h
EdbDataSet.o: /var/local/root/include/TCollection.h
EdbDataSet.o: /var/local/root/include/TIterator.h
EdbDataSet.o: /var/local/root/include/TString.h
EdbDataSet.o: /var/local/root/include/TMath.h
EdbDataSet.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
EdbDataSet.o: /var/local/root/include/TRefCnt.h
EdbDataSet.o: /var/local/root/include/TDatime.h
EdbDataSet.o: /var/local/root/include/Htypes.h
EdbDataSet.o: /var/local/root/include/TUUID.h
EdbDataSet.o: /var/local/root/include/TInetAddress.h EdbDataSet.h
EdbDataSet.o: /var/local/root/include/TObjArray.h
EdbDataSet.o: /var/local/root/include/TFile.h
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
EdbDataSet.o: /var/local/root/include/TCut.h
EdbDataSet.o: /var/local/root/include/TArrayD.h
EdbDataSet.o: /var/local/root/include/TArray.h
EdbDataSet.o: /var/local/root/include/TArrayI.h
EdbDataSet.o: /var/local/root/include/TVirtualTreePlayer.h
EdbDataSet.o: /var/local/root/include/TClass.h
EdbDataSet.o: /var/local/root/include/TDictionary.h
EdbDataSet.o: /var/local/root/include/Property.h
EdbDataSet.o: /var/local/root/include/TArrayL.h
EdbGA.o: EdbGA.h
EdbPVGen.o: /var/local/root/include/TRandom.h
EdbPVGen.o: /var/local/root/include/TNamed.h
EdbPVGen.o: /var/local/root/include/TObject.h
EdbPVGen.o: /var/local/root/include/Rtypes.h
EdbPVGen.o: /var/local/root/include/RConfig.h
EdbPVGen.o: /var/local/root/include/RVersion.h
EdbPVGen.o: /var/local/root/include/DllImport.h
EdbPVGen.o: /var/local/root/include/Rtypeinfo.h
EdbPVGen.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
EdbPVGen.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
EdbPVGen.o: /usr/include/stdio.h /usr/include/features.h
EdbPVGen.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
EdbPVGen.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
EdbPVGen.o: /usr/include/bits/types.h /usr/include/libio.h
EdbPVGen.o: /usr/include/_G_config.h /usr/include/wchar.h
EdbPVGen.o: /usr/include/bits/wchar.h /usr/include/gconv.h
EdbPVGen.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
EdbPVGen.o: /usr/include/bits/stdio_lim.h
EdbPVGen.o: /var/local/root/include/TGenericClassInfo.h
EdbPVGen.o: /var/local/root/include/Varargs.h
EdbPVGen.o: /var/local/root/include/TStorage.h
EdbPVGen.o: /var/local/root/include/Riosfwd.h
EdbPVGen.o: /var/local/root/include/TBuffer.h /var/local/root/include/Bytes.h
EdbPVGen.o: /usr/include/string.h /var/local/root/include/TList.h
EdbPVGen.o: /var/local/root/include/TSeqCollection.h
EdbPVGen.o: /var/local/root/include/TCollection.h
EdbPVGen.o: /var/local/root/include/TIterator.h
EdbPVGen.o: /var/local/root/include/TString.h /var/local/root/include/TMath.h
EdbPVGen.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
EdbPVGen.o: /var/local/root/include/TRefCnt.h EdbPVGen.h EdbPVRec.h
EdbPVGen.o: EdbPattern.h /var/local/root/include/TTree.h
EdbPVGen.o: /var/local/root/include/TClonesArray.h
EdbPVGen.o: /var/local/root/include/TAttLine.h
EdbPVGen.o: /var/local/root/include/Gtypes.h
EdbPVGen.o: /var/local/root/include/TAttFill.h
EdbPVGen.o: /var/local/root/include/TAttMarker.h
EdbPVGen.o: /var/local/root/include/TBranch.h
EdbPVGen.o: /var/local/root/include/TStringLong.h
EdbPVGen.o: /var/local/root/include/TCut.h /var/local/root/include/TArrayD.h
EdbPVGen.o: /var/local/root/include/TArray.h
EdbPVGen.o: /var/local/root/include/TArrayI.h
EdbPVGen.o: /var/local/root/include/TVirtualTreePlayer.h
EdbPVGen.o: /var/local/root/include/TClass.h
EdbPVGen.o: /var/local/root/include/TDictionary.h
EdbPVGen.o: /var/local/root/include/Property.h
EdbPVGen.o: /var/local/root/include/TObjArray.h
EdbPVGen.o: /var/local/root/include/TArrayL.h
EdbPVRec.o: /var/local/root/include/TFile.h /var/local/root/include/TCache.h
EdbPVRec.o: /var/local/root/include/THashList.h EdbPVRec.h EdbPattern.h
EdbPVRec.o: /var/local/root/include/TTree.h
EdbPVRec.o: /var/local/root/include/TClonesArray.h
EdbPVRec.o: /var/local/root/include/TAttLine.h
EdbPVRec.o: /var/local/root/include/Gtypes.h
EdbPVRec.o: /var/local/root/include/TAttFill.h
EdbPVRec.o: /var/local/root/include/TAttMarker.h
EdbPVRec.o: /var/local/root/include/TBranch.h
EdbPVRec.o: /var/local/root/include/TStringLong.h
EdbPVRec.o: /var/local/root/include/TCut.h /var/local/root/include/TArrayD.h
EdbPVRec.o: /var/local/root/include/TArray.h /usr/include/string.h
EdbPVRec.o: /usr/include/features.h /usr/include/sys/cdefs.h
EdbPVRec.o: /usr/include/gnu/stubs.h
EdbPVRec.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
EdbPVRec.o: /var/local/root/include/TArrayI.h
EdbPVRec.o: /var/local/root/include/TVirtualTreePlayer.h
EdbPVRec.o: /var/local/root/include/TClass.h
EdbPVRec.o: /var/local/root/include/TDictionary.h
EdbPVRec.o: /var/local/root/include/Property.h
EdbPVRec.o: /var/local/root/include/TObjArray.h
EdbPVRec.o: /var/local/root/include/TArrayL.h
EdbPattern.o: EdbPattern.h /var/local/root/include/TTree.h
EdbPattern.o: /var/local/root/include/TClonesArray.h
EdbPattern.o: /var/local/root/include/TAttLine.h
EdbPattern.o: /var/local/root/include/Gtypes.h
EdbPattern.o: /var/local/root/include/TAttFill.h
EdbPattern.o: /var/local/root/include/TAttMarker.h
EdbPattern.o: /var/local/root/include/TBranch.h
EdbPattern.o: /var/local/root/include/TStringLong.h
EdbPattern.o: /var/local/root/include/TCut.h
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
EdbPattern.o: /var/local/root/include/TArrayL.h
