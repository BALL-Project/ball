.PHONY: all clean depend

LIBRARY=pluginSpaceNavigator.so

DIRECTORY=EXTENSIONS/SPACENAV/source

CPP_SOURCES=\
	spaceNavigatorPlugin.C\
	spaceNavigatorDriver.C\
	logo.C

ADD_INCLUDES=-I../include ${SPACENAV_INCLUDES}

MOC_SOURCES=\
	spaceNavigatorPlugin_moc.C

all: $(LIBRARY)

include ../../../common.mak
-include .Dependencies

clean: default_clean

%.so: $(OBJECTS)
	$(CXX) -shared $(LDFLAGS) -o $@ $(OBJECTS) ${SPACENAV_LIBS}

spaceNavigatorPlugin_moc.C: ../include/spaceNavigatorPlugin.h
	${MOC} ${BALL_INCLUDES} $< -o $@

logo.C: logo.qrc
	${RCC} $< -o $@

