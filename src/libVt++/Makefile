##########################################################
#
# type:      makefile
#
# created:   21. Nov 2000
#
# author:    Thorsten Glebe
#            HERA-B Collaboration
#            Max-Planck-Institut fuer Kernphysik
#            Saupfercheckweg 1
#            69117 Heidelberg
#            Germany
#            E-mail: T.Glebe@mpi-hd.mpg.de
#
# Description: main BEE makefile
#
##########################################################

HBSCRATCH=$(shell pwd)
export HBSCRATCH
ROOT=$(PWD)
export ROOT

##########################################################
# BEE project setup
##########################################################
include Makefile.setup

##########################################################
# make options
##########################################################
ifndef PROC
MAKEOPT  = -j 2
else
MAKEOPT  = -j $(PROC)
MFLAGS   = ${MAKEOPT}
endif

MAKE := ${MAKE} $(MAKEOPT)

##########################################################
# environment check
##########################################################
ifndef ROOTSYS
errora:
	@echo "Please set ROOTSYS environment variable to specify ROOT location!"
endif

depfile  = .depfile
##########################################################
# targets
##########################################################

shared: install slib

static: install alib

slib:
	@for i in $(projects); do\
	   echo "Make "$$i;\
	   $(MAKE) $(MFLAGS) -C $$i slib ;\
	done

alib:
	@for i in $(projects); do\
	   echo "Make "$$i;\
	   $(MAKE) $(MFLAGS) -C $$i alib;\
	done


all: install slib alib

ifdef PROF
remake: alib
else
remake: slib
endif

clean:
	@rm -rf vt++/*.o vt++/cintdict/vt++dict.* vt++/.depfile $(ldir)/libvt*

depend:
	  @echo "--> Make depend: TODO for vt++"


postinstall:
	@for i in $(instproj); do\
	   $(MAKE) $(MFLAGS) -C $$i install;\
	done

install: postinstall

debug: install
	@for i in $(projects); do\
	   echo "Make "$$i;\
	   $(MAKE) $(MFLAGS) -C $$i slib DEBUG=1;\
	done

docu:
	cd doc; make
	@for i in $(instproj); do\
	   $(MAKE) $(MFLAGS) -C $$i docu;\
	done

html:
	@if [ ! -d $(htmldir) ]; then\
	  echo "create $(htmldir)...";\
	  mkdir -p $(htmldir);\
	fi
	cd doc; make html wwwdir=$(htmldir)
	@for i in $(htmlproj); do\
	   cd $(ROOT)/$$i/doc;\
	   $(MAKE) $(MFLAGS) html wwwdir=$(htmldir);\
	done
	@cd $(ROOT)

manual:
	@echo "Merge files "$(wildcard $(addsuffix /manual.ps, $(docdirs)))" into file Manual.ps"
	gs -dNOPAUSE -sDEVICE=pswrite -dBATCH -sOutputFile=Manual.ps $(wildcard $(addsuffix /manual.ps, $(docdirs)))

cleandoc:
	cd doc; make cleandoc
	@for i in $(instproj); do\
	   $(MAKE) $(MFLAGS) -C $$i cleandoc;\
	done

check:
	@echo " projects   :" $(projects)
	@echo " instproj   :" $(instproj)
	@echo " MAKE       :" $(MAKE)
