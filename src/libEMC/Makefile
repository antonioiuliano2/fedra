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
EdbPVGen.o: /var/local/root/include/TRefCnt.h EdbPVGen.h
EdbPVGen.o: /var/local/root/include/TGenPhaseSpace.h
EdbPVGen.o: /var/local/root/include/TLorentzVector.h
EdbPVGen.o: /var/local/root/include/TVector3.h
EdbPVGen.o: /var/local/root/include/TError.h
EdbPVGen.o: /var/local/root/include/TVector2.h
EdbPVGen.o: /var/local/root/include/TMatrix.h
EdbPVGen.o: /var/local/root/include/TVector.h
EdbPVGen.o: /var/local/root/include/TMatrixUtils.h
EdbPVGen.o: /var/local/root/include/TRotation.h
