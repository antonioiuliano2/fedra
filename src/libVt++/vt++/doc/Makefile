# -*-makefile-*-

DOCXX = doc++
project = vt++

# wwwdir=$(HOME)/www/Bee++
texdir=$(PWD)
ifndef wwwdir
wwwxxx=$(HOME)/backup/public_html/vt++
else
wwwxxx:=$(wwwdir)/$(project)
endif

all: manual.ps $(project).ps

html: $(project).ps manual.ps
	$(DOCXX) -B banner.txt -d $(wwwxxx) Vt.dxx 
	chmod a+x $(wwwxxx)/index.html
	cp $(project).ps manual.ps $(wwwxxx)
	gzip $(wwwxxx)/*.ps

manual.tex:
	$(DOCXX) -t -o $(texdir)/manual.tex\
 -ep a4wide -ep helvetic -ep bookman Vt.dxx

%.ps: %.tex
	latex $(texdir)/$^
	latex $(texdir)/$^
	latex $(texdir)/$^
	dvips -o $(texdir)/$@ $(texdir)/$(addsuffix .dvi, $(basename $^))

ps: manual.ps
	gv $(texdir)/manual.ps

clean:
	\rm -rf core *.swp *~ *.aux *.dvi *.log *.toc
	\rm -rf manual* *.log *.aux *.toc $(project).ps

htmlclean:
	cd $(wwwxxx); \rm *.html *.gif dxx* *.class gifs.db .1.html



