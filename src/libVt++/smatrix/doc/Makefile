# -*-makefile-*-

DOCXX = doc++

project = smatrix
texdir=$(PWD)
#wwwdir=$(PWD)
ifndef wwwdir
wwwxxx=$(HOME)/backup/public_html/smatrix
else
wwwxxx:=$(wwwdir)/$(project)
endif

all: manual.ps $(project).ps

html: $(project).ps manual.ps
	$(DOCXX) -B banner.txt -d $(wwwxxx) $(project).dxx
	chmod a+x $(wwwxxx)/index.html
	cp manual.ps $(project).ps $(wwwxxx)
	gzip $(wwwxxx)/*.ps

manual.tex: $(project).dxx
	$(DOCXX) -t -o $(texdir)/manual.tex -ep a4wide -ep helvetic -ep bookman $(project).dxx

pdf: all
	gs -dNOPAUSE -sDEVICE=pdfwrite -sPAPERSIZE=a4 -dBATCH -sOutputFile=out.pdf $(project).ps manual.ps

fullps: all
	gs -dNOPAUSE -sDEVICE=pswrite -sPAPERSIZE=a4 -dBATCH -sOutputFile=out.ps $(project).ps manual.ps

manual.ps: manual.tex
	@mv $(texdir)/$^ $(texdir)/xxx.tex;\
	sed 's/operator)/operator()/g' $(texdir)/xxx.tex > $(texdir)/$^;\
	rm -f $(texdir)/xxx.tex
	latex $(texdir)/$^
	latex $(texdir)/$^
	latex $(texdir)/$^
	dvips -o $(texdir)/$@ $(texdir)/$(addsuffix .dvi, $(basename $^))

%.ps: %.tex
	latex $(texdir)/$^
	latex $(texdir)/$^
	latex $(texdir)/$^
	dvips -o $(texdir)/$@ $(texdir)/$(addsuffix .dvi, $(basename $^))

ps: manual.ps
	gv $(texdir)/manual.ps

clean:
	rm -rf core *.swp *~ *.aux *.dvi *.log *.toc
	rm -rf manual* *.log *.aux *.toc $(project).ps out.*

htmlclean:
	cd $(wwwxxx); rm *.html *.gif dxx* *.class gifs.db .1.html

