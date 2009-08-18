all:	tutorial.ps tutorial.pdf

again:	build tutorial.ps

b:
	bibtex tutorial

build:
	latex tutorial

html: 
	@latex tutorial
	@bibtex tutorial
	@createHTML

tutorial.ps:	tutorial.dvi
	-@echo "Generating PS file ..."
	-@if test -e tutorial.ps; then rm tutorial.ps 2> /dev/null; fi
	@dvips tutorial.dvi -o tutorial.ps
	-@if test -e tutorial.ps; then \
		echo "PS file 'tutorial.ps' successfully generated"; \
	else \
		echo "Error generating file 'tutorial.ps'!"; \
	fi
	-@touch tutorial.tex

tutorial.pdf: tutorial.dvi
	-@echo "Generating PDF file ..."
	-@if test -e tutorial.pdf; then rm tutorial.pdf 2> /dev/null; fi
	@dvipdf tutorial.dvi
	-@if test -e tutorial.pdf; then \
		echo "PDF file 'tutorial.pdf' successfully generated"; \
	else \
		echo "Error generating file 'tutorial.pdf'!"; \
	fi

tutorial.idx:
	latex tutorial && latex tutorial

tutorial.bbl: *.bib
	bibtex tutorial

tutorial.ind: tutorial.idx
	makeindex -s indexstyle.ist tutorial.idx

tutorial.dvi:	*.tex tutorial.ind
	@latex tutorial
	@bibtex tutorial
	@latex tutorial
	@latex tutorial
	-@echo ""
	-@echo "-----------------------------------------------------------------"
# Find lines containing 'MISSING: ' and 'undefined' and print them.
# Find line 'Output wr'itten to ... to avoid error message of this line
	-@cat tutorial.log | grep "[MuO][Inu][Sdt][Sep][Ifu][Nit][Gn ][:ew][ dr]"
	-@echo "-----------------------------------------------------------------"
	-@echo ""
	-@touch tutorial.tex

clean:
	-@rm -f *.aux *.log *.bbl *.dvi *.blg *.cb *.ind *.idx *.ps *.pdf *.toc core *~ *.tmp *.ilg *.bak 2>/dev/null
