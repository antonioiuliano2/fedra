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

EdbMath.o: /var/local/root/include/TMath.h /var/local/root/include/Rtypes.h
EdbMath.o: /var/local/root/include/RConfig.h
EdbMath.o: /var/local/root/include/RVersion.h
EdbMath.o: /var/local/root/include/DllImport.h
EdbMath.o: /var/local/root/include/Rtypeinfo.h
EdbMath.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/typeinfo
EdbMath.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
EdbMath.o: /usr/include/stdio.h /usr/include/features.h
EdbMath.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
EdbMath.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
EdbMath.o: /usr/include/bits/types.h /usr/include/libio.h
EdbMath.o: /usr/include/_G_config.h /usr/include/wchar.h
EdbMath.o: /usr/include/bits/wchar.h /usr/include/gconv.h
EdbMath.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
EdbMath.o: /usr/include/bits/stdio_lim.h
EdbMath.o: /var/local/root/include/TGenericClassInfo.h
EdbMath.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h EdbMath.h
EdbMath.o: /var/local/root/include/TObject.h
EdbMath.o: /var/local/root/include/Varargs.h
EdbMath.o: /var/local/root/include/TStorage.h
EdbMath.o: /var/local/root/include/Riosfwd.h
EdbMath.o: /var/local/root/include/TBuffer.h /var/local/root/include/Bytes.h
EdbMath.o: /usr/include/string.h
