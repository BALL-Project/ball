# $Id: Makefile.am,v 1.1.2.2 2002/12/05 10:41:55 anhi Exp $

if USE_BALLVIEW
VIEWPROG=BALLVIEW.objects # libVIEW
MOLVIEWPROG=MOLVIEW.objects # libMOLVIEW
SHARED_LIBS=libBALL.so libVIEW.so libMOLVIEW.so
STATIC_LIBS=libBALL.a libVIEW.a libMOLVIEW.a
else
VIEWPROG=
MOLVIEWPROG=
SHARED_LIBS=libBALL.so
STATIC_LIBS=libBALL.a
endif

#bin_PROGRAMS = BALL.objects libBALL $(VIEWPROG) $(MOLVIEWPROG)
bin_PROGRAMS = BALL.objects $(VIEWPROG) $(MOLVIEWPROG)

BUILT_SOURCES=cleanobjects

cleanobjects:
	$(RM) -f BALL.objects BALLVIEW.objects MOLVIEW.objects

MYLIBS = $(LIBS)
LIBS = 
	
BALL_objects_SOURCES=COMMON/.objects CONCEPT/.objects DATATYPE/.objects ENERGY/.objects FORMAT/.objects KERNEL/.objects MATHS/.objects MOLMEC/AMBER/.objects MOLMEC/CHARMM/.objects MOLMEC/COMMON/.objects MOLMEC/MDSIMULATION/.objects MOLMEC/MINIMIZATION/.objects MOLMEC/PARAMETER/.objects NMR/.objects QSAR/.objects SOLVATION/.objects STRUCTURE/.objects SYSTEM/.objects
BALL_objects_LDFLAGS=
BALL_objects_LDADD=
BALL_objects_LINK=@-cat $(BALL_objects_SOURCES) > BALL.objects

BALLVIEW_objects_SOURCES=VIEW/DATATYPE/.objects VIEW/FUNCTOR/.objects VIEW/GUI/DIALOGS/.objects VIEW/GUI/FUNCTOR/.objects VIEW/GUI/KERNEL/.objects VIEW/GUI/PRIMITIV/.objects VIEW/GUI/WIDGETS/.objects VIEW/KERNEL/.objects VIEW/PRIMITIV/.objects
BALLVIEW_objects_LDFLAGS=
BALLVIEW_objects_LDADD=
BALLVIEW_objects_LINK=@-cat $(BALLVIEW_objects_SOURCES) > BALLVIEW.objects

MOLVIEW_objects_SOURCES=MOLVIEW/COMMON/.objects MOLVIEW/FUNCTOR/.objects MOLVIEW/GUI/DIALOGS/.objects MOLVIEW/GUI/FUNCTOR/.objects MOLVIEW/GUI/KERNEL/.objects MOLVIEW/GUI/PRIMITIV/.objects MOLVIEW/GUI/WIDGETS/.objects MOLVIEW/KERNEL/.objects MOLVIEW/PRIMITIV/.objects
MOLVIEW_objects_LDFLAGS=
MOLVIEW_objects_LDADD=
MOLVIEW_objects_LINK=@-cat $(MOLVIEW_objects_SOURCES) > MOLVIEW.objects

# Shared targets
shared: cleanobjects all-recursive BALL.objects $(VIEWPROG) $(MOLVIEWPROG)
	@-$(ECHO_COMMAND) "Creating library libBALL.so"
	$(DYNAR) $(DYNAROPTS) libBALL.so `cat BALL.objects` $(PYTHON_LIBS) -lm
if USE_BALLVIEW
	@-$(ECHO_COMMAND) "Creating library libVIEW.so"
	$(DYNAR) $(DYNAROPTS) libVIEW.so `cat BALLVIEW.objects` $(PYTHON_LIBS) -lm
	@-$(ECHO_COMMAND) "Creating library libMOLVIEW.so"
	$(DYNAR) $(DYNAROPTS) libMOLVIEW.so `cat MOLVIEW.objects` $(PYTHON_LIBS) -lm
endif	
	
# Static targets
static: cleanobjects all-recursive BALL.objects $(VIEWPROG) $(MOLVIEWPROG)
	@-$(ECHO_COMMAND) "Creating library libBALL.a"
	$(AR) $(AROPTS) libBALL.a `cat BALL.objects`
if USE_BALLVIEW
	@-$(ECHO_COMMAND) "Creating library libVIEW.a"
	$(AR) $(AROPTS) libVIEW.a `cat BALLVIEW.objects`
	@-$(ECHO_COMMAND) "Creating library libMOLVIEW.a"
	$(AR) $(AROPTS) libMOLVIEW.a `cat MOLVIEW.objects`
endif	
	
# This is here so that a simple "make" is enough to build the shared libraries.
# We can't use just the shared target because of the dependencies.
all-local: BALL.objects $(VIEWPROG) $(MOLVIEWPROG)
	@-$(ECHO_COMMAND) "Creating library libBALL.so"
	$(DYNAR) $(DYNAROPTS) libBALL.so `cat BALL.objects` $(PYTHON_LIBS) -lm
if USE_BALLVIEW
	@-$(ECHO_COMMAND) "Creating library libVIEW.so"
	$(DYNAR) $(DYNAROPTS) libVIEW.so `cat BALLVIEW.objects` $(PYTHON_LIBS) -lm
	@-$(ECHO_COMMAND) "Creating library libMOLVIEW.so"
	$(DYNAR) $(DYNAROPTS) libMOLVIEW.so `cat MOLVIEW.objects` $(PYTHON_LIBS) -lm
endif	

install: install.shared

install.static:	
	@-$(ECHO_COMMAND) Installing static libraries under $(BALL_PATH)/lib/$(BINFMT)...
	@-mkdir $(BALL_PATH)/lib 2>/dev/null | cat >/dev/null
	@-mkdir $(BALL_PATH)/lib/$(BINFMT) 2>/dev/null | cat >/dev/null
	$(MV) -f $(STATIC_LIBS) $(BALL_PATH)/lib/$(BINFMT)

install.shared:	
	@-$(ECHO_COMMAND) Installing shared libraries under $(BALL_PATH)/lib/$(BINFMT)...
	@-mkdir $(BALL_PATH)/lib 2>/dev/null | cat >/dev/null 
	@-mkdir $(BALL_PATH)/lib/$(BINFMT) 2>/dev/null | cat >/dev/null
	$(MV) -f $(SHARED_LIBS) $(BALL_PATH)/lib/$(BINFMT)

delete.static:	
	@-$(ECHO_COMMAND) deleting static libraries under $(BALL_PATH)/lib/$(BINFMT)...
	@set -e; for i in $(STATIC_LIBS) ; do $(RM) $(BALL_PATH)/lib/$(BINFMT)/$$i 2>/dev/null ; done

delete.shared:	
	@-$(ECHO) deleting shared libraries under $(BALL_PATH)/lib/$(BINFMT)...
	@set -e; for i in $(SHARED_LIBS) ; do $(RM) $(BALL_PATH)/lib/$(BINFMT)/$$i 2>/dev/null ; done

default:	echodir $(OBJECTS)

echodir:
	@echo "entering $(DIRECTORY)..."

default_clean:
	@-$(RM) -rf *~  core $(OBJECTS) $(MOC_SOURCES) $(ADD_MOC_SOURCES) $(EXECUTABLES) $(EXECUTABLE)
