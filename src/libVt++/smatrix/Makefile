##########################################################
# unique name of the project
##########################################################
project   = smatrix

##########################################################
# main makefile
##########################################################
include ../Makefile.setup
include ../Makefile.targets

##########################################################
# install: commands executed before compilation
##########################################################
install:
	./CreateBinaryOp.sh
	mv BinaryOperators.hh include
	./CreateUnaryOp.sh
	mv UnaryOperators.hh include
