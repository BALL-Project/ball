###############################################################
# Main Makefile   ($Id: Makefile,v 1.1 1999/08/26 08:02:33 oliver Exp $)
###############################################################

BINFMT=`config/binfmt.sh`

DIRMAKE=cd $(BINFMT); $(MAKE)

default:	config/binary_formats
	@$(DIRMAKE) default

binary_formats:
	@configure

install:
	@$(DIRMAKE) install

clean:
	@$(DIRMAKE) clean

all_clean:
	@$(DIRMAKE) all_clean

lib:
	@$(DIRMAKE) lib

install.static:
	@$(DIRMAKE) install.static

install.shared:
	@$(DIRMAKE) install.shared

collect:
	@$(DIRMAKE) collect

depend:
	@$(DIRMAKE) depend

test:
	@$(DIRMAKE) test

doc:	texdoc htmldoc
	
texdoc:
	@$(DIRMAKE) texdoc

htmldoc:
	@$(DIRMAKE) htmldoc

configure:  config/configure.in config/configure.header
	@echo "Creating new ./configure from config/configure.in"
	@autoconf config/configure.in > configure.tmp
	@cat configure.tmp|sed "2r config/configure.header" >configure
	@-rm configure.tmp
	@chmod 755 configure
 
