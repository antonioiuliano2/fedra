#------------------------------------------------------------------------------
 include ../config/ProjectDef.mk
 include ../config/RootDef.mk
#------------------------------------------------------------------------------

LIB_TGT       = $(LIB_DIR)/libEmath.so

headers       = TIndexCell.h EdbMath.h

classes       = $(headers:.h=.$(SrcSuf))
Cint          = $(headers) EmathLinkDef.h
SRC           = $(classes) EmathCint.$(SrcSuf)
OBJ           = $(SRC:.$(SrcSuf)=.$(ObjSuf))

#------------------------------------------------------------------------------

all: $(LIB_TGT)

includes: $(headers)
	cp $(headers) $(INC_DIR)

$(LIB_TGT): $(OBJ)
	$(CXX) -O -shared $(LIBS) -o $(LIB_TGT) $(OBJ)

EmathCint.cxx: $(Cint)
	rootcint -f EmathCint.cxx -c $(INCLUDE) $(Cint)

clean:
	@rm -fr core *.bak *.o $(LIB_TGT) *.log *.out *Cint.cxx *Cint.h *~

depend:
	@makedepend $(ROOTCFLAGS) *.cxx >& /dev/null

.SUFFIXES: .$(SrcSuf)

.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $<

# DO NOT DELETE

threadEdbMath.o: /var/local/root/include/TMath.h
threadEdbMath.o: /var/local/root/include/Rtypes.h
threadEdbMath.o: /var/local/root/include/RConfig.h
threadEdbMath.o: /var/local/root/include/RVersion.h
threadEdbMath.o: /var/local/root/include/DllImport.h
threadEdbMath.o: /var/local/root/include/Rtypeinfo.h
threadEdbMath.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
threadEdbMath.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
threadEdbMath.o: /usr/include/stdio.h /usr/include/features.h
threadEdbMath.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
threadEdbMath.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
threadEdbMath.o: /usr/include/bits/types.h /usr/include/libio.h
threadEdbMath.o: /usr/include/_G_config.h /usr/include/wchar.h
threadEdbMath.o: /usr/include/bits/wchar.h /usr/include/gconv.h
threadEdbMath.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
threadEdbMath.o: /usr/include/bits/stdio_lim.h
threadEdbMath.o: /var/local/root/include/TGenericClassInfo.h
threadEdbMath.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
threadEdbMath.o: EdbMath.h /var/local/root/include/TObject.h
threadEdbMath.o: /var/local/root/include/Varargs.h
threadEdbMath.o: /var/local/root/include/TStorage.h
threadEdbMath.o: /var/local/root/include/Riosfwd.h
threadEdbMath.o: /var/local/root/include/TBuffer.h
threadEdbMath.o: /var/local/root/include/Bytes.h /usr/include/string.h
threadEdbMath.o: /var/local/root/include/TArrayI.h
threadEdbMath.o: /var/local/root/include/TArray.h
threadEdbMath.o: /var/local/root/include/Htypes.h
threadEdbMath.o: /var/local/root/include/TArrayD.h
threadEdbMath.o: /var/local/root/include/TMatrixD.h
threadEdbMath.o: /var/local/root/include/TMatrixDBase.h /usr/include/limits.h
threadEdbMath.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
threadEdbMath.o: /var/local/root/include/TROOT.h
threadEdbMath.o: /var/local/root/include/TDirectory.h
threadEdbMath.o: /var/local/root/include/TNamed.h
threadEdbMath.o: /var/local/root/include/TList.h
threadEdbMath.o: /var/local/root/include/TSeqCollection.h
threadEdbMath.o: /var/local/root/include/TCollection.h
threadEdbMath.o: /var/local/root/include/TIterator.h
threadEdbMath.o: /var/local/root/include/TString.h
threadEdbMath.o: /var/local/root/include/TRefCnt.h
threadEdbMath.o: /var/local/root/include/TDatime.h
threadEdbMath.o: /var/local/root/include/TUUID.h
threadEdbMath.o: /var/local/root/include/TClass.h
threadEdbMath.o: /var/local/root/include/TDictionary.h
threadEdbMath.o: /var/local/root/include/Property.h
threadEdbMath.o: /var/local/root/include/TObjArray.h
threadEdbMath.o: /var/local/root/include/TStreamerInfo.h
threadEdbMath.o: /var/local/root/include/TFile.h
threadEdbMath.o: /var/local/root/include/TCache.h
threadEdbMath.o: /var/local/root/include/THashList.h
threadEdbMath.o: /var/local/root/include/TClonesArray.h
threadEdbMath.o: /var/local/root/include/TPluginManager.h
threadEdbMath.o: /var/local/root/include/TVirtualUtilHist.h
threadEdbMath.o: /var/local/root/include/TError.h
threadEdbMath.o: /var/local/root/include/TMatrixDUtils.h
threadEdbMath.o: /var/local/root/include/TVectorD.h
threadEdbMath.o: /var/local/root/include/TMatrixDSym.h
threadEdbMath.o: /var/local/root/include/TMatrixDSparse.h
