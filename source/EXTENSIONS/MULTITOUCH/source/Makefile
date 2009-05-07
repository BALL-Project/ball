.PHONY: all clean depend

LIBRARY=pluginMultitouch.so

DIRECTORY=EXTENSIONS/MULTITOUCH/source

all: $(LIBRARY)

ADD_INCLUDES=-I../include

include ../../../common.mak
-include .Dependencies

# Where the headers of the derived classes reside
HEADER_DIR=../include

# Where the UIC-created headers reside
UIC_DIR=../include

CPP_SOURCES=\
	multitouchPlugin.C\
	multitouchDriver.C\
	multitouchSettings.C\
	logo.C

MOC_SOURCES=\
	multitouchPlugin_moc.C\
	multitouchSettings_moc.C

UIC_SOURCES=\
	multitouchSettingsData.C

depend: $(UIC_HDR) $(MOC_SOURCES)

clean: default_clean

default_clean: clean_uic

clean_uic:
	@echo "Cleaning UIC files in $(UIC_DIR)..."
	@$(RM) -f $(UIC_DIR)/*.h
%.so: $(OBJECTS)
	$(CXX) -shared $(LDFLAGS) -o $@ $(OBJECTS)

multitouchPlugin_moc.C: ../include/multitouchPlugin.h
	${MOC} ${BALL_INCLUDES} $< -o $@

logo.C: logo.qrc
	${RCC} $< -o $@

