all:	tutorial.ps tutorial.pdf

again:	build tutorial.ps

b:
	bibtex tutorial

build:
	latex tutorial

html:
	@createHTML

tutorial.ps:	tutorial.dvi
	-@dvips tutorial.dvi -o tutorial.ps
	-@touch tutorial.tex

tutorial.pdf: tutorial.dvi
	-@dvipdf tutorial.dvi

tutorial.idx:
	latex tutorial && latex tutorial

tutorial.bbl: *.bib
	bibtex tutorial

tutorial.ind: tutorial.idx
	makeindex -s indexstyle.ids tutorial.idx

tutorial.dvi:	*.tex tutorial.ind
	@latex tutorial
	@bibtex tutorial
	@latex tutorial
	@latex tutorial
	-@echo ""
	-@echo "-----------------------------------------------------------------"
	-@cat tutorial.log | grep "[Mu][In][Sd][Se][If][Ni][Gn][:e][ d]"
	-@echo "-----------------------------------------------------------------"
	-@echo ""
	-@touch tutorial.tex

clean:
	-@rm *.aux *.log *.bbl *.dvi *.blg *.cb *.ind *.idx *.ps *.pdf *.toc core *~ *.tmp *.ilg *.bak 2>/dev/null
