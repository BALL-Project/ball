.PHONY: all clean depend

include ../../../config.mak

LIBRARY=pluginSpaceNavigator.$(SHARED_LIB_SUFFIX)
all: $(LIBRARY)

DIRECTORY=EXTENSIONS/SPACENAV/source

CPP_SOURCES=\
	spaceNavigatorPlugin.C\
	spaceNavigatorDriver.C\
	logo.C

ADD_INCLUDES=-I../include ${SPACENAV_INCLUDES}

MOC_SOURCES=\
	spaceNavigatorPlugin_moc.C

include ../../../common.mak

-include .Dependencies

clean: default_clean

%.$(SHARED_LIB_SUFFIX): $(OBJECTS)
	$(DYNAR) $(DYNAROPTS) $(LDFLAGS) $@ $(OBJECTS) ${SPACENAV_LIBS} ${BALL_LIBS} ${VIEW_LIBS}

spaceNavigatorPlugin_moc.C: ../include/spaceNavigatorPlugin.h
	${MOC} ${BALL_INCLUDES} $< -o $@

logo.C: logo.qrc
	${RCC} $< -o $@

install: $(LIBRARY)
	@mkdir -p $(BALL_PATH)/plugins
	@$(CP) pluginSpaceNavigator.$(SHARED_LIB_SUFFIX) $(BALL_PATH)/plugins

