all:	tutorial.ps

again:	build tutorial.ps

b:
	bibtex tutorial

build:
	latex tutorial

tutorial.ps:	tutorial.dvi
	-@dvips tutorial.dvi -o tutorial.ps
	-@touch tutorial.tex

tutorial.idx:
	latex tutorial && latex tutorial

tutorial.bbl: *.bib
	bibtex tutorial

tutorial.ind: tutorial.idx
	makeindex -s indexstyle.ids tutorial.idx

faq.tex:	/www/pub/www.mpi-sb.mpg.de/BALL/doc/FAQ/faq.tex
	cat /www/pub/www.mpi-sb.mpg.de/BALL/doc/FAQ/faq.tex | sed "s/http:\/\/[^ ]*/\\\\FAQURL{&}/g" > faq.tex

	
tutorial.dvi:	*.tex tutorial.ind
	@latex tutorial
	-@echo ""
	-@echo "-----------------------------------------------------------------"
	-@cat tutorial.log | grep "[Mu][In][Sd][Se][If][Ni][Gn][:e][ d]"
	-@echo "-----------------------------------------------------------------"
	-@echo ""
	-@touch tutorial.tex
