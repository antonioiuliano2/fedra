#------------------------------------------------------------------------------
include ../config/ProjectDef.mk
include ../config/RootDef.mk
#------------------------------------------------------------------------------

PROJECT_LIBS += -lEdr -lEmath -lEdb -lEphys

LIB_TGT       = $(LIB_DIR)/libEMC.so

headers       = EdbPVGen.h

classes	      = $(headers:.h=.$(SrcSuf))
EMCCint	      = $(headers) EMCLinkDef.h
SRC           = $(classes) EMCCint.$(SrcSuf)
OBJ           = $(SRC:.$(SrcSuf)=.$(ObjSuf))

CXXFLAGS += -I$(INC_DIR)

#------------------------------------------------------------------------------

$(LIB_TGT): $(OBJ)
	$(CXX) -O -shared $(LIBS) -o $(LIB_TGT) $(OBJ)

EMCCint.cxx: $(EMCCint)
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

threadEdbPVGen.o: /var/local/root/include/TRandom.h
threadEdbPVGen.o: /var/local/root/include/TNamed.h
threadEdbPVGen.o: /var/local/root/include/TObject.h
threadEdbPVGen.o: /var/local/root/include/Rtypes.h
threadEdbPVGen.o: /var/local/root/include/RConfig.h
threadEdbPVGen.o: /var/local/root/include/RVersion.h
threadEdbPVGen.o: /var/local/root/include/DllImport.h
threadEdbPVGen.o: /var/local/root/include/Rtypeinfo.h
threadEdbPVGen.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
threadEdbPVGen.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
threadEdbPVGen.o: /usr/include/stdio.h /usr/include/features.h
threadEdbPVGen.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
threadEdbPVGen.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
threadEdbPVGen.o: /usr/include/bits/types.h /usr/include/libio.h
threadEdbPVGen.o: /usr/include/_G_config.h /usr/include/wchar.h
threadEdbPVGen.o: /usr/include/bits/wchar.h /usr/include/gconv.h
threadEdbPVGen.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
threadEdbPVGen.o: /usr/include/bits/stdio_lim.h
threadEdbPVGen.o: /var/local/root/include/TGenericClassInfo.h
threadEdbPVGen.o: /var/local/root/include/Varargs.h
threadEdbPVGen.o: /var/local/root/include/TStorage.h
threadEdbPVGen.o: /var/local/root/include/Riosfwd.h
threadEdbPVGen.o: /var/local/root/include/TBuffer.h
threadEdbPVGen.o: /var/local/root/include/Bytes.h /usr/include/string.h
threadEdbPVGen.o: /var/local/root/include/TList.h
threadEdbPVGen.o: /var/local/root/include/TSeqCollection.h
threadEdbPVGen.o: /var/local/root/include/TCollection.h
threadEdbPVGen.o: /var/local/root/include/TIterator.h
threadEdbPVGen.o: /var/local/root/include/TString.h
threadEdbPVGen.o: /var/local/root/include/TMath.h
threadEdbPVGen.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
threadEdbPVGen.o: /var/local/root/include/TRefCnt.h EdbPVGen.h
threadEdbPVGen.o: /var/local/root/include/TGenPhaseSpace.h
threadEdbPVGen.o: /var/local/root/include/TLorentzVector.h
threadEdbPVGen.o: /var/local/root/include/TVector3.h
threadEdbPVGen.o: /var/local/root/include/TError.h
threadEdbPVGen.o: /var/local/root/include/TVector2.h
threadEdbPVGen.o: /var/local/root/include/TMatrix.h
threadEdbPVGen.o: /var/local/root/include/TMatrixF.h
threadEdbPVGen.o: /var/local/root/include/TMatrixFBase.h
threadEdbPVGen.o: /var/local/root/include/TROOT.h
threadEdbPVGen.o: /var/local/root/include/TDirectory.h
threadEdbPVGen.o: /var/local/root/include/TDatime.h
threadEdbPVGen.o: /var/local/root/include/Htypes.h
threadEdbPVGen.o: /var/local/root/include/TUUID.h
threadEdbPVGen.o: /var/local/root/include/TClass.h
threadEdbPVGen.o: /var/local/root/include/TDictionary.h
threadEdbPVGen.o: /var/local/root/include/Property.h
threadEdbPVGen.o: /var/local/root/include/TObjArray.h
threadEdbPVGen.o: /var/local/root/include/TStreamerInfo.h
threadEdbPVGen.o: /var/local/root/include/TFile.h
threadEdbPVGen.o: /var/local/root/include/TCache.h
threadEdbPVGen.o: /var/local/root/include/THashList.h
threadEdbPVGen.o: /var/local/root/include/TClonesArray.h
threadEdbPVGen.o: /var/local/root/include/TPluginManager.h
threadEdbPVGen.o: /var/local/root/include/TVirtualUtilHist.h
threadEdbPVGen.o: /var/local/root/include/TMatrixFUtils.h
threadEdbPVGen.o: /var/local/root/include/TRotation.h
