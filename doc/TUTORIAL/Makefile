all:	tutorial.ps tutorial.pdf

again:	build tutorial.ps

b:
	bibtex tutorial

build:
	latex tutorial

install:
	@echo "Deleting old tutorial directory..."
	-@rm -rf /www/pub/www.mpi-sb.mpg.de/BALL/TUTORIAL
	@echo "Generating html in the WWW area..."
	@mkdir /www/pub/www.mpi-sb.mpg.de/BALL/TUTORIAL
	/KM/comp-bio/BALL-contrib/latex2html/bin/latex2html -local_icons -init_file latex2html-init.BALL -dir /www/pub/www.mpi-sb.mpg.de/BALL/TUTORIAL tutorial.tex 2>&1|tee l2h.log
	@echo "moving PDF + PS files..."
	mv -f tutorial.pdf tutorial.ps /www/pub/www.mpi-sb.mpg.de/BALL/doc/

html:
	-@rm -fr HTML
	@mkdir HTML
	/KM/comp-bio/BALL-contrib/latex2html/bin/latex2html -local_icons -init_file latex2html-init.BALL -dir HTML tutorial.tex 2>&1|tee l2h.log

tutorial.ps:	tutorial.dvi
	-@dvips tutorial.dvi -o tutorial.ps
	-@touch tutorial.tex

tutorial.pdf: tutorial.dvi
	-@dvi2pdf tutorial.dvi

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
