#------------------------------------------------------------------------------
include ../config/ProjectDef.mk
include ../config/RootDef.mk
#------------------------------------------------------------------------------

PROJECT_LIBS += -lvt -lEmath -lEdb

LIB_TGT       = $(LIB_DIR)/libEGA.so

headers       = EdbGA.h EdbIP.h

classes	      = $(headers:.h=.$(SrcSuf))
EGACint	      = $(headers) EGALinkDef.h
SRC           = $(classes) EGACint.$(SrcSuf)
OBJ           = $(SRC:.$(SrcSuf)=.$(ObjSuf))

CXXFLAGS += -I$(INC_DIR)

#------------------------------------------------------------------------------

$(LIB_TGT): $(OBJ)
	$(CXX) -O -shared $(LIBS) -o $(LIB_TGT) $(OBJ)

EGACint.cxx: $(EGACint)
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
