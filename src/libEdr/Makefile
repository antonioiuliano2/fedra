#------------------------------------------------------------------------------
include ../config/ProjectDef.mk
include ../config/RootDef.mk
#------------------------------------------------------------------------------

PROJECT_LIBS += -lEmath -lEdb

LIB_TGT       = $(LIB_DIR)/libEdr.so

headers       = EdbPattern.h EdbPVGen.h EdbPVRec.h

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
EdbPVGen.o: /var/local/root/include/TObjArray.h
EdbPVGen.o: /var/local/root/include/TClonesArray.h
EdbPVGen.o: /var/local/root/include/TAttLine.h
EdbPVGen.o: /var/local/root/include/Gtypes.h /var/local/root/include/Htypes.h
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
EdbPVGen.o: /var/local/root/include/TArrayL.h
EdbPVRec.o: EdbPVRec.h EdbPattern.h /var/local/root/include/TTree.h
EdbPVRec.o: /var/local/root/include/TObjArray.h
EdbPVRec.o: /var/local/root/include/TClonesArray.h
EdbPVRec.o: /var/local/root/include/TAttLine.h
EdbPVRec.o: /var/local/root/include/Gtypes.h /var/local/root/include/Htypes.h
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
EdbPVRec.o: /var/local/root/include/TArrayL.h
EdbPattern.o: EdbPattern.h /var/local/root/include/TTree.h
EdbPattern.o: /var/local/root/include/TObjArray.h
EdbPattern.o: /var/local/root/include/TClonesArray.h
EdbPattern.o: /var/local/root/include/TAttLine.h
EdbPattern.o: /var/local/root/include/Gtypes.h
EdbPattern.o: /var/local/root/include/Htypes.h
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
EdbPattern.o: /var/local/root/include/TArrayL.h
